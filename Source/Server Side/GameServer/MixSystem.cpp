// MixSystem.cpp: implementation of the CMixSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MixSystem.h"

#include "StdAfx.h"
#include "User.h"
#include "Include\Prodef.h"
#include "Common\winutil.h"
#include "Include\GMProtocol.h"
#include "Include\cb_protocol.h"
#include "wzUdp.h"
#include "protocol.h"
#include "Gamemain.h"
#include "DSProtocol.h"
#include "Common\SetItemOption.h"

#include "BloodCastle.h"

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219
#include "CastleSiegeSync.h"
#endif

#ifdef MU_CRYWOLF_BENEFIT_20051215
#include "CrywolfSync.h"
#endif

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	#include "EventItemBagExMng.h"
	extern CEventItemBagExMng g_EventItemBagExMng;
	extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
	#else
	#include "CashLotterySystem.h"
	extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
	extern CCashLotterySystem g_CashLotterySystem;
	#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef MODIFY_MIX_SYSTEM_20070518

#ifdef ADD_SHIELD_POINT_01_20060403
extern int g_ShieldSystemOn;
#endif 

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804
// [0x17] 숨겨진 보물상자 이벤트 경품 등록을 요청한다.
//extern VOID EGReqRegHTOfflineGift (INT iIndex);
#endif

extern WzUdp			gUdpSocCE;		// Chaos Event 
extern WzUdp			gUdpSocCER;		// Chaos Event 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMixSystem g_MixSystem;

#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
extern ITEM_ATTRIBUTE ItemAttribute[MAX_ITEM];
#endif // ADD_SOCKET_OPTION_LOG_02_20080821

CMixSystem::CMixSystem()
{
	CHAOS_GEM_ITEMINDEX			= MAKE_ITEMNUM( 12, 15 );	// 혼돈의보석
	BLESS_GEM_ITEMINDEX			= MAKE_ITEMNUM( 14, 13 );	// 축복의보석
	SOUL_GEM_ITEMINDEX			= MAKE_ITEMNUM( 14, 14 );	// 영혼의보석
	CREATURE_GEM_ITEMINDEX		= MAKE_ITEMNUM( 14, 22 );	// 창조의보석

	BLESS_GEM_BUNDLE_ITEMINDEX	= MAKE_ITEMNUM( 12, 30 );	// 축복의보석 묶음
	SOUL_GEM_BULDLE_ITEMINDEX	= MAKE_ITEMNUM( 12, 31 );	// 영혼의보석 묶음

	CONDOR_FEATHER_ITEMINDEX	= MAKE_ITEMNUM( 13, 53 );	// 콘도르의깃털
	CONDOR_FLAME_ITEMINDEX		= MAKE_ITEMNUM( 13, 52 );	// 콘도르의불꽃

	WING_OF_STORM_ITEMINDEX		= MAKE_ITEMNUM( 12, 36 );	// 폭풍의날개
	WING_OF_ETERNAL_ITEMINDEX	= MAKE_ITEMNUM( 12, 37 );	// 시공의날개
	WING_OF_ILLUSION_ITEMINDEX	= MAKE_ITEMNUM( 12, 38 );	// 환영의날개
	WING_OF_RUIN_ITEMINDEX		= MAKE_ITEMNUM( 12, 39 );	// 파멸의날개
	CAPE_OF_EMPEROR_ITEMINDEX	= MAKE_ITEMNUM( 12, 40 );	// 제왕의망토
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203
	WING_OF_DIMENSION_ITEMINDEX = MAKE_ITEMNUM( 12, 43 );	// 차원의 날개
#endif

	m_i3rdWingOpt_LuckRate		= 50;	// (1/1000)

#ifdef TEST_3RD_20070622
	m_i3rdWingOpt_LuckRate		= 500;	// (1/1000)
#endif
}

CMixSystem::~CMixSystem()
{

}

BOOL CMixSystem::ChaosBoxCheck( LPOBJECTSTRUCT lpObj )
{	
	if( lpObj->pChaosBox == NULL ) 
	{
		return FALSE;
	}
	return TRUE;
}

// 카오스 고블린 조합창(카오스박스) 초기화
BOOL CMixSystem::ChaosBoxInit( LPOBJECTSTRUCT lpObj )
{
	int n;

	if( lpObj->pChaosBox != NULL ) 
	{
		for( n=0; n<MAX_CHAOSBOXITEMS; n++)
		{
			lpObj->pChaosBox[n].Clear();
		}
		for( n=0; n<MAX_CHAOSBOXSIZE; n++)
		{
			lpObj->pChaosBoxMap[n] = 0xFF;
		}
		return TRUE;
	}
	
	lpObj->pChaosBox = new CItem [MAX_CHAOSBOXSIZE];
	if( lpObj->pChaosBox == NULL )
	{
		return FALSE;
	}

	lpObj->pChaosBoxMap = new BYTE [MAX_CHAOSBOXSIZE];
	if( lpObj->pChaosBoxMap == NULL )
	{
		delete lpObj->pChaosBox;
		return FALSE;
	}
	for( n=0; n<MAX_CHAOSBOXSIZE; n++)
	{
		lpObj->pChaosBoxMap[n] = 0xFF;
	}
	return TRUE;
}

// 카오스 박스 믹스 후 아이템들 처리
BOOL CMixSystem::ChaosBoxItemDown( LPOBJECTSTRUCT lpObj )
{
	int n;

	if( lpObj->pChaosBox == NULL ) 
	{
		return FALSE;
	}

	for( n = 0; n < MAX_CHAOSBOXITEMS; n++ )
	{
		if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12,15) ||	// 혼돈의 보석
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,13) ||	// 축복의 보석
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,14)		// 영혼의 보석
#ifdef	ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			|| lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,83) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,84) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,85) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,86) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,87) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,88) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,89) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,90) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,91) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13,92) ||
			lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,53)		
#endif
			)
		{
			lpObj->pChaosBox[n].Clear();
		}
		else 
		{
			int op;
			
			op = lpObj->pChaosBox[n].m_Option1;
			if( op > 0 ) 
			{
				if( rand()%2 == 0 ) 
				{
					op--;
				}
			}
			lpObj->pChaosBox[n].m_Option1 = op;

			op = lpObj->pChaosBox[n].m_Option3;
			if( op > 0 ) 
			{
				if( rand()%2 == 0 ) 
				{
					op--;	
				}
			}
			lpObj->pChaosBox[n].m_Option3 = op;

			if( lpObj->pChaosBox[n].m_Level > 0 )
			{
				lpObj->pChaosBox[n].m_Level = rand() % ( lpObj->pChaosBox[n].m_Level );
			}

			float levelitemdur =  (float)ItemGetDurability(lpObj->pChaosBox[n].m_Type, 
														   lpObj->pChaosBox[n].m_Level, 
														   lpObj->pChaosBox[n].IsExtItem(),
														   lpObj->pChaosBox[n].IsSetItem());
			
			lpObj->pChaosBox[n].m_Durability = levelitemdur*lpObj->pChaosBox[n].m_Durability/lpObj->pChaosBox[n].m_BaseDurability;

#ifdef ADD_380ITEM_NEWOPTION_20060711
			lpObj->pChaosBox[n].Convert(lpObj->pChaosBox[n].m_Type, 
										lpObj->pChaosBox[n].m_Option1, 
										lpObj->pChaosBox[n].m_Option2, 
										lpObj->pChaosBox[n].m_Option3, 
										lpObj->pChaosBox[n].m_NewOption,
										lpObj->pChaosBox[n].m_SetOption,
										lpObj->pChaosBox[n].m_ItemOptionEx);
#else
			lpObj->pChaosBox[n].Convert(lpObj->pChaosBox[n].m_Type, 
										lpObj->pChaosBox[n].m_Option1, 
										lpObj->pChaosBox[n].m_Option2, 
										lpObj->pChaosBox[n].m_Option3, 
										lpObj->pChaosBox[n].m_NewOption,
										lpObj->pChaosBox[n].m_SetOption);
#endif // ADD_380ITEM_NEWOPTION_20060711

		}
	}
	return TRUE;
}

// 카오스박스 믹스 시킨다.
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
int CMixSystem::ChaosBoxMix(LPOBJECTSTRUCT lpObj, BOOL & Result2, int &iWingIndex)
#else
int CMixSystem::ChaosBoxMix(LPOBJECTSTRUCT lpObj, BOOL & Result2)
#endif
{
	BYTE ExOption[8];

	int ChaosDiamond = 0;	// 혼돈의 보석 카운트
	int ChaosItems = 0;		// 카오스박스 안에 있는 유효한 아이템 

	if( lpObj->pChaosBox == NULL ) 
	{
		return 0;
	}

	int value = 0;	// 조합 확률에 쓰이는 수치
	int add = 0;
	int nv = 0;	
	
	Result2 = FALSE;

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	iWingIndex = -1;
	int iWingCharmItemNum = -1;
#endif

	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosMoney		= 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	for( int n = 0; n < MAX_CHAOSBOXITEMS; n++ )
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
#ifdef NEW_FORSKYLAND3
			lpObj->pChaosBox[n].OldValue();
#endif

			add = 0;
			nv  = 0;

			if( lpObj->pChaosBox[n].m_Level >= 4 && ( lpObj->pChaosBox[n].m_Option3 * 4 ) >= 4 )
			{	
#ifdef NEW_FORSKYLAND3
				nv     = lpObj->pChaosBox[n].m_OldBuyMoney;
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
#else
				nv     = lpObj->pChaosBox[n].m_BuyMoney;
				value += lpObj->pChaosBox[n].m_BuyMoney;
#endif
				add = 1;
				if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(2,6) ||	// 카오스드라곤도끼
					lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(4,6) ||	// 카오스네이쳐보우
					lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(5,7) )	// 카오스번개지팡이
				{
					Result2 = TRUE;
				}
			}
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12,15) )		// 혼돈의 보석
			{
#ifdef NEW_FORSKYLAND3
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv     = lpObj->pChaosBox[n].m_OldBuyMoney;
#else
				value += lpObj->pChaosBox[n].m_BuyMoney;
				nv     = lpObj->pChaosBox[n].m_BuyMoney;
#endif
				ChaosDiamond++;
				add = 1;				
			}			
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,13) ||	// 축복의 보석
			         lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,14) )	// 영혼의 보석
			{
#ifdef NEW_FORSKYLAND3
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv     = lpObj->pChaosBox[n].m_OldBuyMoney;
#else
				value += lpObj->pChaosBox[n].m_BuyMoney;
				nv     = lpObj->pChaosBox[n].m_BuyMoney;
#endif
				add = 1;
			}
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 83) || lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 84)
				|| lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 85) || lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 86)
				|| lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 87)) 
			{
				iWingCharmItemNum = lpObj->pChaosBox[n].m_Type;
			}
#endif
#ifdef JAPAN_ROCFEATHER_CBMIX_BLOCK_20040303	// 일본 로크의 깃털이 일반 조합이 들어가면 무조건 조합실패 되도록
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13,14 ) ) 
			{
				return 0;
			}
#endif
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			// 행운의 부적 아이템 추가
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			if( add ) 
			{
				ChaosItems++;
			}
			ItemIsBufExOption( ExOption, &lpObj->pChaosBox[n] );

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAdd( "[%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
#else
			LogAdd( "[%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(),
				lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1,
				lpObj->pChaosBox[n].m_Option2,
				lpObj->pChaosBox[n].m_Option3, 
				lpObj->pChaosBox[n].m_Number, 
				(int)lpObj->pChaosBox[n].m_Durability, nv,
				ExOption[0],
				ExOption[1],
				ExOption[2],
				ExOption[3],
				ExOption[4],
				ExOption[5],
				ExOption[6],
				lpObj->pChaosBox[n].m_SetOption				
				);
		}		
	}
/*	
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if(iCharmOfLuckCount > 0 && iWingCharmItemNum > -1 ){
		// 행운의 부적과 날개 부적은 조합될 수 없다.(같이 사용할 수 없다)
		return 0;
	}
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
*/
	if( ChaosDiamond == 0 ) 
	{
		value = 0;
	}
	if( ChaosItems < 2 ) 
	{
		value = 0;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		return 0;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	lpObj->ChaosSuccessRate = value / 20000;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}
	lpObj->ChaosMoney = lpObj->ChaosSuccessRate*10000;
	
#if TESTSERVER == 1
	char msg[255];
	wsprintf(msg, "카오스조합확률=%d", lpObj->ChaosSuccessRate);
	GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
#endif

	// 레벨 4 에 +4 옵 이상만 value 더해줌
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	LogAddTD("[%s][%s] CBMix need Zen : %d SuccessRate : %d, CharmRate : %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, lpObj->ChaosSuccessRate, iCharmOfLuckCount);
#else
	LogAddTD("[%s][%s] CBMix need Zen : %d SuccessRate : %d",lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, lpObj->ChaosSuccessRate);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	// 6차 과금 아이템 날개 조합 부적 ( 해당 클래스에 맞는 아이템이 나온다. )
	if(iWingCharmItemNum > -1) {
		// 날개 조합 부적의 인덱스를 추출해서 해당 날개의 index를 구하자.
		int iWingCharmIndex = iWingCharmItemNum ^ (13 << ITEM_BITSHIFT);
		switch( iWingCharmIndex )
		{
		case 83: //사탄날개 부적
			iWingIndex = 2;
			break;
		case 84: // 천공날개 부적
			iWingIndex = 1;
			break;
		case 85: // 요정날개 부적
			iWingIndex = 0;
			break;
		case 86: // 재앙날개 부적
			iWingIndex = 41;
			break;
		case 87: // 망토조합 부적 
#ifdef MODIFY_WINGCHAM_DARKLOAD3RD_WING_20090914
			iWingIndex = 30;
#else
			iWingIndex = 40;
#endif //MODIFY_WINGCHAM_DARKLOAD3RD_WING_20090914
			break;
		default:
			// 이건 아직 등록되지 않은 조합 부적이네.. (해당 부적이 없는지 확인하거나 type이 13이 맞는지 확인하라..
			LogAddTD("[MixSystem] WingCharmItemNum => %d , WingCharmIndex => %d",iWingCharmItemNum ,iWingCharmIndex);
			break;
		}
	}
	
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129

	return value;
}

void CMixSystem::DefaultChaosMix( LPOBJECTSTRUCT lpObj )
{
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB( ( LPBYTE )&pMsg, 0x86, sizeof( pMsg ) );
	pMsg.Result		= 0x00;	

	int	 aIndex = lpObj->m_Index;
	BOOL fail=TRUE;
	BOOL MixResult2;

	lpObj->ChaosLock = TRUE;

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	int iWingIndex = -1;
	
	if( ChaosBoxMix(lpObj, MixResult2, iWingIndex) == 0 )
#else
	if( ChaosBoxMix(lpObj, MixResult2) == 0 )
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	{	// 조합할 아이템이 잘못되었다
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 카오스 일반 조합
	INT iChaosTaxMoney		= (INT)((INT64)lpObj->ChaosMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	lpObj->ChaosMoney		= lpObj->ChaosMoney + iChaosTaxMoney;
	if (lpObj->ChaosMoney < 0)
		lpObj->ChaosMoney	= 0;
#endif

	if( lpObj->Money < lpObj->ChaosMoney )
	{
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
		if( lpObj->ChaosSuccessRate> 0 ) 
		{
			if( lpObj->ChaosSuccessRate >= 100 ) lpObj->ChaosSuccessRate = 100;
			if( (rand()%100) <= (lpObj->ChaosSuccessRate-1) )
			{
				fail			= FALSE;

				pMsg.Result		= 0x01;
				
				int level = rand()%5;
				int Option1=0, Option2=0, Option3=0;

				if((rand()%100) < (6+(lpObj->ChaosSuccessRate/5)) ) Option1 = 1;
				if((rand()%100) < (4+(lpObj->ChaosSuccessRate/5)) ) Option2 = 1;	// 영석 바를때 행운 

				int optionrand, optionc;

				optionc = (rand()%3);
				optionrand = (rand()%100);

				switch( optionc )
				{
				case 0 :
					if( optionrand < (4+(lpObj->ChaosSuccessRate/5)) ) Option3 = 3;	// 옵션
					break;
				case 1 :
					if( optionrand < (8+(lpObj->ChaosSuccessRate/5)) ) Option3 = 2;	// 옵션
					break;
				case 2 :
					if( optionrand < (12+(lpObj->ChaosSuccessRate/5)) ) Option3 = 1;	// 옵션
					break;
				}
				if( MixResult2 == TRUE )
				{
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 1차날개(재앙의 날개) 생성(추가)
					int itemindex = rand()%4;
					int itemtype  = 0;
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
					if(iWingIndex > -1) {
						// 날개 조합 부적을 사용하면 해당 날개가 100% 나온다.
#ifdef MODIFY_WINGCHAM_DARKLOAD3RD_WING_20090914
						if(30 == iWingIndex)	// 군주의 망토 (군주의 망토만 index가 13, 나머지 날개 및 제왕의 망토는 12)
							itemtype = MAKE_ITEMNUM(13, iWingIndex);
						else
							itemtype = MAKE_ITEMNUM(12, iWingIndex);
#else
						itemtype = MAKE_ITEMNUM(12, iWingIndex);
#endif	//MODIFY_WINGCHAM_DARKLOAD3RD_WING_20090914
					} else {
						if( itemindex == 0 ) itemtype = MAKE_ITEMNUM(12,0);
						else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(12,1);
						else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(12,2);
						else if( itemindex == 3 ) itemtype = MAKE_ITEMNUM(12,41);	// 신규캐릭터 1차날개(재앙의 날개)
					}
					
#else
					if( itemindex == 0 ) itemtype = MAKE_ITEMNUM(12,0);
					else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(12,1);
					else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(12,2);
					else if( itemindex == 3 ) itemtype = MAKE_ITEMNUM(12,41);	// 신규캐릭터 1차날개(재앙의 날개)
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
					int itemindex = rand()%3;
					int itemtype  = 0;
					if( itemindex == 0 ) itemtype = MAKE_ITEMNUM(12,0);
					else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(12,1);
					else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(12,2);
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef CHAOS_MIX_WING_ITEMLEVEL_FIX
					ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, 0, 255, Option1, Option2, Option3);
#else
					ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, level, 255, Option1, Option2, Option3);
#endif
				}
				else
				{
					int itemindex = rand()%3;
					int itemtype  = 0;

					if( itemindex == 0 )      itemtype = MAKE_ITEMNUM(2,6);
					else if( itemindex == 1 ) itemtype = MAKE_ITEMNUM(4,6);
					else if( itemindex == 2 ) itemtype = MAKE_ITEMNUM(5,7);
					ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, level, 255, Option1, Option2, Option3);
				}
			}
		}
		
		lpObj->Money -= lpObj->ChaosMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 카오스 일반 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
		GCMoneySend(lpObj->m_Index, lpObj->Money);							

		if( fail == TRUE )
		{
			ChaosBoxItemDown(lpObj);
			GCUserChaosBoxSend(lpObj);
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			LogAddTD("[%s][%s] CBMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney );		
			lpObj->ChaosLock = FALSE;
		}
		else 
		{
			LogAddTD("[%s][%s] CBMix Success Rate:%d Money : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);
		}
		if( gChaosEvent )
			CBUPS_ItemRequest(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
	}	
	gObjInventoryCommit(lpObj->m_Index);
}



// 악마의 광장에서 검사된 아이템 로그 출력
void CMixSystem::LogDQChaosItem(LPOBJECTSTRUCT lpObj)
{
	BYTE ExOption[8];
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex[%d] HO:[%d,%d]", 
		#else
				LogAddTD("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex[%d] HO:[%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						lpObj->pChaosBox[n].m_Level,
						lpObj->AccountID, lpObj->Name, 
						n, 
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
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#else
				LogAddTD("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						(int)lpObj->pChaosBox[n].m_Durability,
						lpObj->AccountID, lpObj->Name, 
						n, 
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
						ExOption[6]
						);
	#endif // ADD_380ITEM_NEWOPTION_20060711

#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[DevilSquare] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] 380Ex[%d]", 
		#else
				LogAddTD("[DevilSquare] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] 380Ex[%d]", 
		#endif MODIFY_ITEM_SERIAL_EXPAND_20080227
						lpObj->AccountID, lpObj->Name, 
						n, 
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
						lpObj->pChaosBox[n].m_ItemOptionEx >> 7
						);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[DevilSquare] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#else
				LogAddTD("[DevilSquare] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						lpObj->AccountID, lpObj->Name, 
						n, 
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
						ExOption[6]
						);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // EXTEND_LOG_SYSTEM_03_20060816

		}		
	}
}

// 악마의 광장 초대권을 받을수 있는 조건이 되는지 체크
// 악마의눈, 악마의열쇠, 혼돈의 보석, 젠 30000
BOOL CMixSystem::CheckDevilSquareItem(LPOBJECTSTRUCT lpObj, int& eventitemcount, int& itemlevel)
{
	BOOL bChaoseGem = FALSE;
	BOOL bEyeOfDevil = FALSE;
	BOOL bKeyOfDevil = FALSE;

	eventitemcount = 0;
	BOOL FoundOtherItem = FALSE;

	int	 level = -1;
	BOOL bLevelCheck = FALSE;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
	BOOL bCharmOfLuckOver = FALSE;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	LogAdd("[DevilSquare] Search DevilSquareItem");	

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				bChaoseGem = TRUE;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 17) )
			{	// 악마의 눈
				eventitemcount++;
				bEyeOfDevil = TRUE;

				if( level !=  lpObj->pChaosBox[n].m_Level )
				{
					if( level == -1 )
					{						
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 18) )
			{	// 악마의 열쇠
				eventitemcount++;
				bKeyOfDevil = TRUE;

				if( level != lpObj->pChaosBox[n].m_Level )
				{
					if( level == -1 )
					{						
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
				if( iCharmOfLuckCount > 10 )
				{
					bCharmOfLuckOver = TRUE;
				}
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{	// 이외의 아이템이 올려져 있다
				FoundOtherItem = TRUE;
			}
		}		
	}

	itemlevel = level;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate = iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( FoundOtherItem )
	{	// 다른 아이템이 포함되어 있으므로 기존의 카오스 시스템으로 넘어감
		LogAdd("[DevilSquare] Other DQItems Found");
		return FALSE;
	}

	if( bLevelCheck )
	{	// 레벨이 다른 아이템이 있으므로 조합되지 않는다
		LogAdd("[DevilSquare] DiffLevel DQItems Found");
		return 3;	// 레벨이 다른 아이템이 있다
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( bCharmOfLuckOver == TRUE )
	{
		LogAdd("[DevilSquare] Charm Of Luck Count Over");
		return 4;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( bChaoseGem && bEyeOfDevil && bKeyOfDevil )
	{
		LogDQChaosItem(lpObj);
		return 1;	// 혼석까지 있을경우
	}	

	if( bEyeOfDevil && bKeyOfDevil)
	{
		LogDQChaosItem(lpObj);
		return 2;	// 혼석은 없고, 이벤트 아이템만 있을경우
	}

	LogAdd("[DevilSquare] DQItems Not Found");
	return FALSE;
}

BOOL CMixSystem::DevilSquareEventChaosMix(LPOBJECTSTRUCT lpObj, BOOL bCheckType, int iItemLevel)
{
	BOOL fail=TRUE;

	int aIndex = lpObj->m_Index;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	
	
	lpObj->ChaosLock = TRUE;	
	LogAddTD("[DevilSquare] Chaos Mix Start");

	int nChaosNeedMoney = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
	iCharmOfLuckCount = lpObj->ChaosSuccessRate;

	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		lpObj->ChaosLock = FALSE;

		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	switch(iItemLevel)
	{
#ifdef DEVILSQUARE_EXTEND_20050221		// !!! . 임시 -> 카오스 조합 추가
		case 0:
			lpObj->ChaosSuccessRate = 60;			
			nChaosNeedMoney = 100000;
		break;

		case 1:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel1;
			nChaosNeedMoney = 100000;
		break;

		case 2:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel2;
			nChaosNeedMoney = 200000;
		break;

		case 3:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel3;
			nChaosNeedMoney = 400000;
		break;

		case 4:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel4;
			nChaosNeedMoney = 700000;
		break;

		case 5:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel5;
			nChaosNeedMoney = 1100000;
		break;
		
		case 6:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel6;
			nChaosNeedMoney = 1600000;
		break;
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 광장7 카오스조합
		case 7:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel7;
			nChaosNeedMoney = 2000000;
		break;
#endif
#else
		case 0:
			lpObj->ChaosSuccessRate = 60;			
			nChaosNeedMoney = 40000;
		break;

		case 1:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel1;
			nChaosNeedMoney = 40000;
		break;

		case 2:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel2;
			nChaosNeedMoney = 80000;
		break;

		case 3:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel3;
			nChaosNeedMoney = 160000;
		break;

		case 4:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel4;
			nChaosNeedMoney = 300000;
		break;
#endif

		default:			
			LogAdd("[DevilSquare] [%s][%s] Invalid DQItem Level [%d]", 
					lpObj->AccountID, lpObj->Name, iItemLevel);
			pMsg.Result		= 0x08;		// 정해진 아이템 레벨이 아니다
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return TRUE;
		break;
	}
	
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 데빌스퀘어 입장권
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( (lpObj->Money-nChaosNeedMoney) < 0 )
	{			
		LogAddTD("[DevilSquare] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}	
	
#ifdef MU_CRYWOLF_BENEFIT_20051215
	// MVP 성공 : 카오스 조합 성공률 증가 ( 데빌스퀘어 입장권)	
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE 
		&& g_iCrywolfApplyMvpBenefit	
	  )
	{
		lpObj->ChaosSuccessRate += g_CrywolfSync.GetPlusChaosRate();
	}
#endif

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int iRate = rand() % 100;
	if( bCheckType == 1 )
	{	
		if( iRate < lpObj->ChaosSuccessRate )
		{	// 초대권만 나온다
			int itemtype  = MAKE_ITEMNUM(14,19);
			ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, iItemLevel, 0, 0, 0, 0);				
			fail = FALSE;
		}		
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 데빌스퀘어 입장권
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	
#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[17] != AUTH_CHECK_17 )
		DestroyGIocp();
#endif

	if( fail == TRUE )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef EXTEND_LOG_SYSTEM_03_20060816
		LogAddTD("[DevilSquare,%d] [%s][%s] CBMix Fail %d Money : %d-%d",
			iItemLevel, lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney );
#else
		LogAddTD("[DevilSquare] [%s][%s] CBMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney );
#endif // EXTEND_LOG_SYSTEM_03_20060816
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
#ifdef EXTEND_LOG_SYSTEM_03_20060816
		LogAddTD("[DevilSquare,%d] [%s][%s] CBMix Success Rate:%d Money : %d",
			iItemLevel, lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, nChaosNeedMoney);
#else
		LogAddTD("[DevilSquare] [%s][%s] CBMix Success Rate:%d Money : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, nChaosNeedMoney);
#endif // EXTEND_LOG_SYSTEM_03_20060816
	}	
	gObjInventoryCommit(lpObj->m_Index);
	return TRUE;
}
void CMixSystem::DevilSquareItemChaosMix(LPOBJECTSTRUCT lpObj)
{	
	lpObj->ChaosLock = TRUE;
	
#ifndef GAMESERVER_TO_CASTLESIEGESERVER
	if( gDevilSquareEvent )
#endif
	{	// 악마의 광장 이벤트가 진행 중이면, 초대권 체크
		PMSG_CHAOSMIXRESULT	pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
		pMsg.Result		= 0x00;	

		int aIndex = lpObj->m_Index;

		int	iEventItemCount;
		int iItemLevel;

		BOOL bret =CheckDevilSquareItem(lpObj, iEventItemCount, iItemLevel);

		if( bret )
		{
			if( bret == 3 )
			{	// 레벨이 다른 DQ 아이템이 있다
				pMsg.Result		= 0x07;		
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}

			if( bret == 2 )
			{	// 혼석이 없다
				pMsg.Result		= 0x07;		
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] Not Found Chaos Gem [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			if( bret == 4 )
			{
				pMsg.Result		= 0xF0;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] Charm Of Luck Over 10% [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			if( iEventItemCount > 2 )
			{
				pMsg.Result		= 0x03;		// 열쇠또는 눈의 수가 너무 많다
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				LogAdd("[DevilSquare] Too many Devil's Key or Eyes [%d]", iEventItemCount);
				lpObj->ChaosLock = FALSE;
				return;
			}

			if( lpObj->Level < 10 )
			{
				pMsg.Result		= 0x04;		// 악마의 광장 초대권 조합은 레벨 10이상만 할수 있다
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}

			DevilSquareEventChaosMix(lpObj, bret, iItemLevel);
			return;
		}
		else if( iEventItemCount > 1 )
		{	
			pMsg.Result		= 0x06;		// 조합조건에 맞지 않는데, 열쇠또는 눈이 하나라도 포함되어 있다
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}	
	
	lpObj->ChaosLock = FALSE;
	return;
}

// 악마의 광장에서 검사된 아이템 로그 출력
#ifdef EXTEND_LOG_SYSTEM_03_20060816
void CMixSystem::LogPlusItemLevelChaosItem(LPOBJECTSTRUCT lpObj, int iPlusMixLevel)
#else
void CMixSystem::LogPlusItemLevelChaosItem(LPOBJECTSTRUCT lpObj)
#endif // EXTEND_LOG_SYSTEM_03_20060816
{
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	int iMixLevel = 0;
#endif
	BYTE ExOption[8];
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

#ifdef EXTEND_LOG_SYSTEM_03_20060816
			if( iPlusMixLevel == CHAOS_TYPE_UPGRADE_10 )
			{
				iMixLevel = 1;
			}
			else if( iPlusMixLevel == CHAOS_TYPE_UPGRADE_11 )
			{
				iMixLevel = 2;
			}
			else if( iPlusMixLevel == CHAOS_TYPE_UPGRADE_12 )
			{
				iMixLevel = 3;
			}
			else if( iPlusMixLevel == CHAOS_TYPE_UPGRADE_13 )
			{
				iMixLevel = 4;
			}

	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PlusItemLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]", 
		#else
			LogAddTD("[PlusItemLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					iMixLevel,
					lpObj->AccountID, lpObj->Name, 
					n, 
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
		#ifdef EXTEND_LOG_SYSTEM_07_20070320
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pChaosBox[n] ),
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[n] )
		#else
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[n] ),
					g_kJewelOfHarmonySystem.GetItemStrengthenOption( &lpObj->pChaosBox[n] )
		#endif // EXTEND_LOG_SYSTEM_07_20070320
					);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PlusIte mLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
		#else
			LogAddTD("[PlusIte mLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					iMixLevel,
					lpObj->AccountID, lpObj->Name, 
					n, 
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
					lpObj->pChaosBox[n].m_SetOption
					);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d]", 
		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, 
					n, 
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
					lpObj->pChaosBox[n].m_ItemOptionEx >> 7
					);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, 
					n, 
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
					lpObj->pChaosBox[n].m_SetOption
					);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // EXTEND_LOG_SYSTEM_03_20060816
			
		}		
	}
}


// 악마의 광장에서 검사된 아이템 로그 출력
void CMixSystem::LogChaosItem(LPOBJECTSTRUCT lpObj, char* sLogType)
{
	BYTE ExOption[8];
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
			LogAddTD("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] Socket[%d,%d,%d,%d,%d]", 
					sLogType, lpObj->AccountID, lpObj->Name, 
					n, 
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
					g_kJewelOfHarmonySystem.GetItemOptionLevel( &lpObj->pChaosBox[n] ),
					lpObj->pChaosBox[n].m_SocketOption[0],
					lpObj->pChaosBox[n].m_SocketOption[1],
					lpObj->pChaosBox[n].m_SocketOption[2],
					lpObj->pChaosBox[n].m_SocketOption[3],
					lpObj->pChaosBox[n].m_SocketOption[4]
					);
#else
	#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]", 
		#else
			LogAddTD("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					sLogType, lpObj->AccountID, lpObj->Name, 
					n, 
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
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#else
			LogAddTD("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]", 
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					sLogType, lpObj->AccountID, lpObj->Name, 
					n, 
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
					ExOption[6]
					);
	#endif // MODIFY_LOG_SYSTEM_2_3_20060828
#endif // ADD_SOCKET_OPTION_LOG_02_20080821
		}		
	}
}

// +9이상 아이템에 대한 조합
// +10, 11 구분한다 mixType == 1 +10 조합 mixType == 2 +11조합
BOOL CMixSystem::PlusItemLevelChaosMix(LPOBJECTSTRUCT lpObj, int mixType)
{
	int ChaosGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;	
	int Plus9ItemCount = 0;
	int Plus10ItemCount = 0;	
	int PlusItemPos = -1;
	int OtherItemFound = 0;
	
#ifdef ITEM_12_13_20040401
	int Plus11ItemCount		= 0;
	int Plus12ItemCount		= 0;
	int ExtraBlessGemCount	= 0;
	int ExtraSoulGemCount	= 0;
#endif

#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
	//grooving ChaosMixCham
	int iChaosMixCharmCnt	= 0;
#endif // ADD_CHAOSMIXCHARM_ITEM_20080702	

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330
	BYTE btItemLuckValue = 0;
#endif // MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				ChaosGemCount++;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축석
				BlessGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영석
				SoulGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 96 ) )
			{
				iChaosMixCharmCnt++;
			}
#endif //ADD_CHAOSMIXCHARM_ITEM_20080702
			else
			{
				if( lpObj->pChaosBox[n].m_Level == 9 )
				{
					Plus9ItemCount++;
					PlusItemPos = n;
				}
				else if( lpObj->pChaosBox[n].m_Level == 10 )
				{
					Plus10ItemCount++;
					PlusItemPos = n;
				}
#ifdef ITEM_12_13_20040401
				else if( lpObj->pChaosBox[n].m_Level == 11 )
				{
					Plus11ItemCount++;
					PlusItemPos = n;
				}
				else if( lpObj->pChaosBox[n].m_Level == 12 )
				{
					Plus12ItemCount++;
					PlusItemPos = n;
				}
#endif
				else
				{
					OtherItemFound++;
				}
			}
		}
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		PMSG_CHAOSMIXRESULT	pMixResult;
		PHeadSetB((LPBYTE)&pMixResult, 0x86, sizeof( pMixResult ));

		LogAddTD("[PlusItemLevel] [%s][%s] CBMix Charm of luck over 10% (%d)", 
			   lpObj->AccountID, lpObj->Name, iCharmOfLuckCount );
		pMixResult.Result		= 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMixResult, pMixResult.h.size);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( OtherItemFound )
	{	// 조합할수 없다
		return FALSE;
	}
#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
	// 카오스조합부적이 1개 이상이면 조합 할수 없다.
	if( iChaosMixCharmCnt > 1 )
	{
	/*
	LogAddTD("[PlusItemLevel] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]", 
	lpObj->AccountID, lpObj->Name, lpObj->Money, lpObj->ChaosMoney);
		*/
		PMSG_CHAOSMIXRESULT	pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
#endif //ADD_CHAOSMIXCHARM_ITEM_20080702

	int MixType = 0;

#ifdef ITEM_12_13_20040401 // 이상이상 아래 두줄..

	lpObj->ChaosMoney = 0;	

	if( Plus9ItemCount == 1 &&  ChaosGemCount == 1 && 
		//BlessGemCount >= 1 && SoulGemCount >= 1 &&
		BlessGemCount == 1 && SoulGemCount == 1 && 
		Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{	// +10 조합
		MixType = CHAOS_TYPE_UPGRADE_10;
		lpObj->ChaosMoney = 2000000;
		ExtraBlessGemCount = BlessGemCount - 1;
		ExtraSoulGemCount = SoulGemCount - 1;
	}
	else if( Plus10ItemCount == 1 &&  ChaosGemCount == 1 && 
			 //BlessGemCount >= 2 && SoulGemCount >= 2 && 
			 BlessGemCount == 2 && SoulGemCount == 2 && 
			 Plus9ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{	// +11 조합
		MixType = CHAOS_TYPE_UPGRADE_11;
		lpObj->ChaosMoney = 4000000;
		ExtraBlessGemCount = BlessGemCount - 2;
		ExtraSoulGemCount = SoulGemCount - 2;
	}	
	else if( Plus11ItemCount == 1 &&  ChaosGemCount == 1 && 
			 //BlessGemCount >= 3 && SoulGemCount >= 3 && 
			 BlessGemCount == 3 && SoulGemCount == 3 && 
			 Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus12ItemCount == 0 )
	{	// +12 조합
		MixType = CHAOS_TYPE_UPGRADE_12;
#ifdef FOR_CHINA	// 중국은 조합 비용 증가
		lpObj->ChaosMoney = 8000000;
#else
		lpObj->ChaosMoney = 6000000;
#endif
		ExtraBlessGemCount = BlessGemCount - 3;
		ExtraSoulGemCount = SoulGemCount - 3;
	}
	else if( Plus12ItemCount == 1 &&  ChaosGemCount == 1 && 
			 //BlessGemCount >= 4 && SoulGemCount >= 4 && 
			 BlessGemCount == 4 && SoulGemCount == 4 && 
			 Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 )
	{	// +13 조합
		MixType = CHAOS_TYPE_UPGRADE_13;
#ifdef FOR_CHINA	// 중국은 조합 비용 증가
		lpObj->ChaosMoney = 16000000;
#else
		lpObj->ChaosMoney = 8000000;
#endif
		ExtraBlessGemCount = BlessGemCount - 4;
		ExtraSoulGemCount = SoulGemCount - 4;
	}
#else
	if( Plus9ItemCount == 1 &&  ChaosGemCount == 1 && 
		BlessGemCount == 1 && SoulGemCount == 1 && Plus10ItemCount == 0)
	{	// +10조합
		MixType = 1;
		lpObj->ChaosMoney = 2000000;
	}
	else if( Plus10ItemCount == 1 &&  ChaosGemCount == 1 && 
			 BlessGemCount == 2 && SoulGemCount == 2 && Plus9ItemCount == 0 )
	{	// +11 조합
		MixType = 2;
		lpObj->ChaosMoney = 4000000;
	}
	else {
		return FALSE;		//#BUGFIX_20040701	-> 일반조합이 아닌 카오스무기조합으로 +10, +11.. 무기들을 혼석만으로 그냥 만들 수 있었다.
	}
#endif	

#ifdef CHAOS_MIX_UPGRADE
	if( MixType != mixType )
		MixType = 0;		// 해당 조합 타입이 아니다
#endif

	if( MixType == 0 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	
	
	lpObj->ChaosLock = TRUE;	

#ifdef EXTEND_LOG_SYSTEM_03_20060816
	LogPlusItemLevelChaosItem(lpObj, MixType);
#else
	LogPlusItemLevelChaosItem(lpObj);
#endif // EXTEND_LOG_SYSTEM_03_20060816
	LogAddTD("[PlusItemLevel] Chaos Mix Start");	


#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - +10, +11, +12, +13 아이템
	INT iChaosTaxMoney		= (INT)((INT64)lpObj->ChaosMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	lpObj->ChaosMoney		= lpObj->ChaosMoney + iChaosTaxMoney;
	if (lpObj->ChaosMoney < 0)
		lpObj->ChaosMoney	= 0;
#endif

	if( lpObj->Money < lpObj->ChaosMoney )
	{			
		LogAddTD("[PlusItemLevel] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]", 
			   lpObj->AccountID, lpObj->Name, lpObj->Money, lpObj->ChaosMoney);
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}	

	lpObj->Money -= lpObj->ChaosMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - +10, +11, +12, +13 아이템
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef MODIFY_ITEM_UPGRADE_MIX_RATE_FIX_20080627
	BYTE btItemGrade = 0;

	if( lpObj->pChaosBox[PlusItemPos].IsExtItem() == TRUE 
		|| lpObj->pChaosBox[PlusItemPos].IsSetItem() == TRUE
		|| g_kItemSystemFor380.Is380Item( &lpObj->pChaosBox[PlusItemPos] ) == TRUE
	  )
	{
		btItemGrade = 2;
	}
#ifndef MODIFY_PLUSITEMMIX_SOCKET_ITEM_DIVISION_20080905				// !! NOT
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	else if( g_SocketOptionSystem.IsSocketItem( &lpObj->pChaosBox[PlusItemPos] ) == true )
	#else
	else if( g_SocketOptionSystem.IsEnableSocketItem( &lpObj->pChaosBox[PlusItemPos] ) == true )
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	{
		btItemGrade = 3;
	}
#endif // MODIFY_PLUSITEMMIX_SOCKET_ITEM_DIVISION_20080905
	else
	{
		btItemGrade = 1;
	}
	
#ifdef MODIFY_PLUSITEMMIX_SOCKET_ITEM_DIVISION_20080905
	if( g_SocketOptionSystem.IsSocketItem( &lpObj->pChaosBox[PlusItemPos] ) == true )
	{
		btItemGrade = 3;
	}
#endif // MODIFY_PLUSITEMMIX_SOCKET_ITEM_DIVISION_20080905
	
#ifdef MODIFY_PLUS_ITEM_MIX_WING_SET_NORMAL_ITEM_20080916
	if(	( ( lpObj->pChaosBox[PlusItemPos].m_Type >= MAKE_ITEMNUM( 12, 0 ) )
		&& ( lpObj->pChaosBox[PlusItemPos].m_Type <= MAKE_ITEMNUM( 12, 6 ) ) )
#if GAME_VERSION >= G_V_S3_E2
		|| ( ( lpObj->pChaosBox[PlusItemPos].m_Type >= MAKE_ITEMNUM( 12, 36 ) )
		&& ( lpObj->pChaosBox[PlusItemPos].m_Type <= MAKE_ITEMNUM( 12, 43 ) ) )
#endif
#if GAME_VERSION >= G_V_99B
		|| ( lpObj->pChaosBox[PlusItemPos].m_Type == MAKE_ITEMNUM( 13, 30 ) )
#endif
	  )
	{
		btItemGrade = 1;
	}
#endif // MODIFY_PLUS_ITEM_MIX_WING_SET_NORMAL_ITEM_20080916

	// 10레벨 조합일 경우
	if( lpObj->pChaosBox[PlusItemPos].m_Level == 9 )
	{
		// 일반 아이템의 경우 60% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination10Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 50% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination10Rate+10;
		}
		// 소켓 아이템의 경우 40% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination10Rate;
		}
	}
#ifdef UPDATE_CHAOSMIX_LEVELUP_RATE_20080904
	// 11레벨 조합일 경우
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 10 )
	{
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate;
		}
	}
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 11 )
	{
		// 일반 아이템의 경우 55% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate;
		}
	}
	// 12~13레벨 조합일 경우
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 12 )
	{
		// 일반 아이템의 경우 55% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate;
		}
	}
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 13 )
	{
		// 일반 아이템의 경우 55% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate;
		}
	}
#else	// UPDATE_CHAOSMIX_LEVELUP_RATE_20080904	
	// 11~13레벨 조합일 경우
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 10 )
	{
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate;
		}
	}
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 11 )
	{
		// 일반 아이템의 경우 55% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate;
		}
	}
	// 12~13레벨 조합일 경우
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 12 )
	{
		// 일반 아이템의 경우 55% 확률 적용
		if( btItemGrade == 1 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+20;
		}
		// 엑설런트, 세트, 380 아이템의 경우 45% 확률 적용
		else if( btItemGrade == 2 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate+10;
		}
		// 소켓 아이템의 경우 35% 확률 적용
		else if( btItemGrade == 3 )
		{
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate;
		}
	}
#endif	// UPDATE_CHAOSMIX_LEVELUP_RATE_20080904
	else
	{
		lpObj->ChaosSuccessRate = 45;
	}
#else

	if( lpObj->pChaosBox[PlusItemPos].m_Level == 9 )
	{
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination10Rate;
	}
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 10)
	{
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination11Rate;
	}
#ifdef ITEM_12_13_20040401
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 11)
	{
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination12Rate;
	}
	else if( lpObj->pChaosBox[PlusItemPos].m_Level == 12)
	{
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iCombination13Rate;
	}
#else
	else
	{
		lpObj->ChaosSuccessRate = 45;
	}
#endif

#endif // MODIFY_ITEM_UPGRADE_MIX_RATE_FIX_20080627

	if( lpObj->pChaosBox[PlusItemPos].m_Option2 != 0 )
	{
		//lpObj->ChaosSuccessRate += 25;		
		lpObj->ChaosSuccessRate += 20;		
	}

#ifdef ITEM_12_13_20040401
	// 12, 13 엑설런트 아이템 증가에 대한 우려로 주석처리
	/*
	if( ExtraBlessGemCount >= 1 && ExtraBlessGemCount <= MAX_CHAOSBOXITEMS )
	{
		// 축석의 OldValue = 100000
		lpObj->ChaosSuccessRate += ExtraBlessGemCount * 100000 / 20000;
	}

	if( ExtraSoulGemCount >= 1 && ExtraSoulGemCount <= MAX_CHAOSBOXITEMS)
	{
		// 영석의 OldValue = 70000
		lpObj->ChaosSuccessRate += ExtraSoulGemCount * 70000 / 20000; 
	}
	*/

	if( lpObj->ChaosSuccessRate > g_GlobalConfig.m_iMaxCombinationRate )
			lpObj->ChaosSuccessRate = g_GlobalConfig.m_iMaxCombinationRate;	// +10, +11, +12 아이템 최대 성공률은 75%
#endif 

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	// 50%확률로 조합성공
		lpObj->pChaosBox[PlusItemPos].m_Level++;	// 레벨업				
		pMsg.Result  = 1;

		CItem NewItem;

		float levelitemdur =  (float)ItemGetDurability(lpObj->pChaosBox[PlusItemPos].m_Type, 
													   lpObj->pChaosBox[PlusItemPos].m_Level, 
												       lpObj->pChaosBox[PlusItemPos].IsExtItem(),
													   lpObj->pChaosBox[PlusItemPos].IsSetItem());
		
		NewItem.m_Level      = lpObj->pChaosBox[PlusItemPos].m_Level;
		NewItem.m_Durability = levelitemdur*lpObj->pChaosBox[PlusItemPos].m_Durability/lpObj->pChaosBox[PlusItemPos].m_BaseDurability;
		
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		// 강화옵션은 그대로 가야한다. 시리얼 발급 아님.
		NewItem.m_JewelOfHarmonyOption = lpObj->pChaosBox[PlusItemPos].m_JewelOfHarmonyOption;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530


#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		BYTE SocketOption[MAX_SOCKET_SLOT];
		BYTE SocketBonusOption;

		g_SocketOptionSystem.GetSocketOption( &lpObj->pChaosBox[PlusItemPos], SocketOption, SocketBonusOption );

		NewItem.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, 
			            lpObj->pChaosBox[PlusItemPos].m_Option1, 
						lpObj->pChaosBox[PlusItemPos].m_Option2, 
						lpObj->pChaosBox[PlusItemPos].m_Option3, 
						lpObj->pChaosBox[PlusItemPos].m_NewOption,
						lpObj->pChaosBox[PlusItemPos].m_SetOption,
						lpObj->pChaosBox[PlusItemPos].m_ItemOptionEx,
						SocketOption,
						SocketBonusOption);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		NewItem.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, 
			            lpObj->pChaosBox[PlusItemPos].m_Option1, 
						lpObj->pChaosBox[PlusItemPos].m_Option2, 
						lpObj->pChaosBox[PlusItemPos].m_Option3, 
						lpObj->pChaosBox[PlusItemPos].m_NewOption,
						lpObj->pChaosBox[PlusItemPos].m_SetOption,
						lpObj->pChaosBox[PlusItemPos].m_ItemOptionEx);
	#else
		NewItem.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, 
			            lpObj->pChaosBox[PlusItemPos].m_Option1, 
						lpObj->pChaosBox[PlusItemPos].m_Option2, 
						lpObj->pChaosBox[PlusItemPos].m_Option3, 
						lpObj->pChaosBox[PlusItemPos].m_NewOption,
						lpObj->pChaosBox[PlusItemPos].m_SetOption);	
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		
		NewItem.m_Number = lpObj->pChaosBox[PlusItemPos].m_Number;

		ItemByteConvert(pMsg.ItemInfo, NewItem);
		
		ChaosBoxInit(lpObj);
		gObjChaosBoxInsertItemPos(lpObj->m_Index, NewItem, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);		

		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		//GCUserChaosBoxSend(lpObj);
		
#ifdef MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330
	#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		if( btItemLuckValue != 0 )
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d", 
					   lpObj->AccountID, lpObj->Name, 
					   lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level, iCharmOfLuckCount);
		}
		else
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d", 
					   lpObj->AccountID, lpObj->Name, 
					   lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level, iCharmOfLuckCount);
		}
	#else
		if( btItemLuckValue != 0 )
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d]", 
					   lpObj->AccountID, lpObj->Name, 
					   lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level);
		}
		else
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d]", 
					   lpObj->AccountID, lpObj->Name, 
					   lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level);
		}
	#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#else
	#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		#ifdef ITEM_12_13_20040401
			// 행운에 대한 내부적 카오스 성공조합률 조정에 따라 +5 로그 추가됨..(비밀누설 가능성 방지)
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level, iCharmOfLuckCount);

		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d], CharmRate : %d",
				lpObj->AccountID, lpObj->Name, 
				lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level, iCharmOfLuckCount);
		#endif
	#else
		#ifdef ITEM_12_13_20040401
			// 행운에 대한 내부적 카오스 성공조합률 조정에 따라 +5 로그 추가됨..(비밀누설 가능성 방지)
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d]", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level);

		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Success %d Money : %d-%d [%d]", 
				lpObj->AccountID, lpObj->Name, 
				lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, NewItem.m_Level);
		#endif
	#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#endif // MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330

	}
	else
	{	// 50%확률로 조합실패
#ifdef ITEM_12_13_20040401
		int _itemlebel = lpObj->pChaosBox[PlusItemPos].m_Level+1;
#endif

#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702
		if( iChaosMixCharmCnt )// 카오스조합 부적 사용시
		{
			ChaosMixCharmItemUsed( lpObj );
			//ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj);
			DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		}
		else
		{
			ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj);
			DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		}
#else
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif // ADD_CHAOSMIXCHARM_ITEM_20080702

#ifdef MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330
	#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		if( btItemLuckValue != 0 )
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d], CharmRate : %d",  
					lpObj->AccountID, lpObj->Name, 
					lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, _itemlebel, iCharmOfLuckCount);
		}
		else
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d], CharmRate : %d",  
					lpObj->AccountID, lpObj->Name, 
					lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, _itemlebel, iCharmOfLuckCount);
		}
	#else
		if( btItemLuckValue != 0 )
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d]",  
					lpObj->AccountID, lpObj->Name, 
					lpObj->ChaosSuccessRate+5, lpObj->Money, lpObj->ChaosMoney, _itemlebel);
		}
		else
		{
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d]",  
					lpObj->AccountID, lpObj->Name, 
					lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, _itemlebel);
		}
	#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#else
	#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		#ifdef ITEM_12_13_20040401
			// 카오스조합 실패시 어떤 레벨의 아이템 조합에 실패했는지 추가됨.
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d], CharmRate : %d",  
				lpObj->AccountID, lpObj->Name, 
				lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, _itemlebel, iCharmOfLuckCount);
			
		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
				lpObj->AccountID, lpObj->Name, 
				lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, iCharmOfLuckCount);
		#endif
	#else
		#ifdef ITEM_12_13_20040401
			// 카오스조합 실패시 어떤 레벨의 아이템 조합에 실패했는지 추가됨.
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d [%d]",  
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, _itemlebel);

		#else
			LogAddTD("[PlusItemLevel] [%s][%s] CBMix Fail %d Money : %d-%d",  
				lpObj->AccountID, lpObj->Name, 
				lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney);
		#endif
	#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#endif // MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330
		
	}	

	lpObj->ChaosLock = FALSE;
	return TRUE;
}

#ifdef ADD_CHAOSMIXCHARM_ITEM_20080702

void CMixSystem::ChaosMixCharmItemUsed(LPOBJECTSTRUCT lpObj)
{
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			// 아이템 레벨을 0으로 만든다.
			lpObj->pChaosBox[n].m_Level = 0;
			
			int iItemNum = lpObj->pChaosBox[n].m_Type;
			
			if( IsDeleteItem( iItemNum ) == TRUE )
			{
				lpObj->pChaosBox[n].Clear();
				lpObj->pChaosBoxMap[n] = 0xFF;
			}
		}
	}
}

BOOL CMixSystem::IsDeleteItem(const int iItemNum)
{
	switch(iItemNum)
	{
	case MAKE_ITEMNUM2( 12, 15 ):// 혼석
	case MAKE_ITEMNUM2( 14, 13 ):// 축석
	case MAKE_ITEMNUM2(14, 14): // 영석
	case MAKE_ITEMNUM2(14, 53): // 행운의 부적
	case MAKE_ITEMNUM2(14, 96): // 카오스조합부적
		return TRUE;
	}
	
	return FALSE;
}

#endif //ADD_CHAOSMIXCHARM_ITEM_20080702

// 디노란트 조합
BOOL CMixSystem::PegasiaChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int UniriaCount = 0;
	int ChoasGemCount = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 2) && lpObj->pChaosBox[n].m_Durability == 255 )
			{	// 유니리아
				UniriaCount++;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		}
	}

#ifdef NEW_FORSKYLAND2
	if( UniriaCount != 10 || ChoasGemCount != 1 )
#else
	if( UniriaCount != 3 || ChoasGemCount != 1 )
#endif
	{
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;

		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return FALSE;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef NEW_FORSKYLAND2
	int nChaosNeedMoney = 500000;
#else
	int nChaosNeedMoney = 250000;
#endif

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 디노란트 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}

	LogChaosItem(lpObj, "DinorantMix");
	LogAddTD("[DinorantMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 70;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 디노란트 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int itemtype  = MAKE_ITEMNUM(13,3);

		int op3 = 0;


		if( (rand()%100) < 30 )
		{	// 30%
			op3 = 1<<(rand()%3);

			if( rand()%5 == 0 )
			{	// 20%
				op3 |= 1<<(rand()%3);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, 0, 255, 1, 0, op3);				
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DinorantMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[DinorantMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

		return 1;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DinorantMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[DinorantMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;
	return 0;
}

// 스탯 열매 조합
BOOL CMixSystem::CircleChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int CreatureGemCount = 0;
	int ChoasGemCount = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 22) )
			{	// 창조의 보석
				CreatureGemCount++;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		}
	}

	if( CreatureGemCount != 1 || ChoasGemCount != 1 )
	{	// 조합에 필요한 아이템이 없다
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;

		pMsg.Result = 0xF0;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return TRUE;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( lpObj->Level < 10 )
	{
		pMsg.Result		= 0x04;		// 레벨 10이상만 할수 있다
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}

	int nChaosNeedMoney = 3000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 스탯열매 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}

	LogChaosItem(lpObj, "CircleMix");
	LogAddTD("[CircleMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 90;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 스탯열매 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);	

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int itemtype  = MAKE_ITEMNUM(13,15);
		int itemlevel;		
		int iCircleRate = rand()%100;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		if( iCircleRate < 30 )
		{	
			itemlevel = FRUIT_ENERGY;
		}
		else if( iCircleRate < 55 )
		{	
			itemlevel = FRUIT_VITALITY;
		}
		else if( iCircleRate < 75 )
		{	
			itemlevel = FRUIT_DEXTERITY;
		}
		else if( iCircleRate < 95 )
		{	
			itemlevel = FRUIT_STRENGTH;
		}
		else
		{	
			itemlevel = FRUIT_LEADERSHIP;
		}
#else
		if( iCircleRate < 30 )
		{	// 라이프 서클
			itemlevel = 0;
		}
		else if( iCircleRate < 60 )
		{	// 파워 서클
			itemlevel = 1;
		}
		else if( iCircleRate < 80 )
		{	// 에너지 서클
			itemlevel = 2;
		}
		else
		{	// 파워 서클
			itemlevel = 3;
		}
#endif

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, itemlevel, 255, 1, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[CircleMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[CircleMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return 1;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[CircleMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[CircleMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;
	return 0;
}

// 2차 날개/망토 조합
BOOL CMixSystem::WingChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChoasGemCount = 0;
	int LokesFeathersCount = 0;	

	int WingIndex = -1;
	int	iChaosMoney = 0;

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	int iWingCharmItemNum		= -1;
#endif
	
#ifdef MODIFY_WINGMIX_454BUG_20040512
	DWORD iWingChaosMoney = 0;	
#else
	int	iWingChaosMoney = 0;	
#endif

#ifdef DARKLORD_WORK
	int	iSleeveOfLord = 0;
#endif

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
	BYTE ExOption[8];
	#endif
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
	
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
#ifdef DARKLORD_WORK
			if( (lpObj->pChaosBox[n].m_Type >= MAKE_ITEMNUM(12, 3) &&  lpObj->pChaosBox[n].m_Type <= MAKE_ITEMNUM(12, 6)) ||
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 절망의 날개 조합
				lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12,42) ||
#endif				
				lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,30) )
#else
			if( lpObj->pChaosBox[n].m_Type >= MAKE_ITEMNUM(12, 3) &&  lpObj->pChaosBox[n].m_Type <= MAKE_ITEMNUM(12, 6))
#endif
			{	// 이미 날개 조합된 아이템들이 끼어있다면 이 날개조합은 이루어지지 않는다. (2003.10.22 - B4nFter)
				lpObj->ChaosLock = FALSE;
				return FALSE;
			}

			if( lpObj->pChaosBox[n].m_Type >= MAKE_ITEMNUM(12, 0) &&  lpObj->pChaosBox[n].m_Type <= MAKE_ITEMNUM(12, 2))
			{	// 날개
				//if( lpObj->pChaosBox[n].m_Level >= 1 || lpObj->pChaosBox[n].m_Option2 || lpObj->pChaosBox[n].m_Option3 >= 1 )
				{	// 최소 레벨이 있거나 행운, 추가 옵션중에 하나가 있어야 된다
					WingCount++;
					WingIndex = n;
					iWingChaosMoney = 	lpObj->pChaosBox[n].m_BuyMoney;
				}
				
			}
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 2차날개 조합재료에 재앙의 날개 추가
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 41))
			{		
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = 	lpObj->pChaosBox[n].m_BuyMoney;
			}
#endif
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 14) )
			{	// 로크의 깃털 
#ifdef DARKLORD_WORK
				if( lpObj->pChaosBox[n].m_Level == 0 )
					LokesFeathersCount++;
				else	// 군주의 소매
					iSleeveOfLord++;
#else
				LokesFeathersCount++;				
#endif
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			else if (lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 88) || lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 89) 
				|| lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 90) || lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 91)
				|| lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 13, 92) ) 
			{
				iWingCharmItemNum = lpObj->pChaosBox[n].m_Type;
			}
			
#endif
			else
			{
#ifdef WING_MIX_MODIFY
				if( lpObj->pChaosBox[n].IsExtItem() && lpObj->pChaosBox[n].m_Level >= 4 )
#else
				if( lpObj->pChaosBox[n].IsExtItem() && lpObj->pChaosBox[n].m_Level >= 4 &&
					(lpObj->pChaosBox[n].m_Type<MAKE_ITEMNUM(12,3) || lpObj->pChaosBox[n].m_Type>MAKE_ITEMNUM(12,6)) )
#endif			
				{	// 엑설런트 아이템(+4, 옵션+4이상)
					iChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;				
				}
			}
#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
			#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
			LogAddTD("[WingMix,2] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]",
			#else
			LogAddTD("[WingMix,2] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]",
			#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name,
				n,
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
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
		}
	}

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

/*
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if(iCharmOfLuckCount > 0 && iWingCharmItemNum > -1 ){
		// 행운의 부적과 날개 부적은 조합될 수 없다.(같이 사용할 수 없다)
		lpObj->ChaosLock = FALSE;
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return FALSE;
	}
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
*/

#ifdef DARKLORD_WORK
	if( iSleeveOfLord == 1 )
	{	// 망토 조합
		if( WingCount != 1 || ChoasGemCount != 1 || LokesFeathersCount != 0 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			return FALSE;	// 조합 아이템이 없거나 로크의 깃털이 있다.
		}
	}
	else if( LokesFeathersCount == 1 )
	{	// 날개 조합
		if( WingCount != 1 || ChoasGemCount != 1 || iSleeveOfLord != 0 )
		{
			pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return FALSE;	// 조합 아이템이 없거나 군주의 소매가 있다이 있다.
		}
	}
	else 
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return FALSE;
	}
#else
	if( WingCount != 1 || ChoasGemCount != 1 || LokesFeathersCount != 1 )
	{	// 조합에 필요한 아이템이 없다
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return FALSE;
	}
#endif	

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result		= 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 5000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 2차날개 (다크로드 망토) 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{		
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}

	CItem* pWing = &lpObj->pChaosBox[WingIndex];

	//lpObj->ChaosSuccessRate = pWing->m_Level*4;		// 날개 레벨 * 2 ( 0 ~ 22 % )
	//lpObj->ChaosSuccessRate += (pWing->m_Option2*5);	// 행운 * 5		( 5% )
	//lpObj->ChaosSuccessRate += (pWing->m_Option3*4);	// 옵션 *4		( 0 ~ 16 % )

	lpObj->ChaosSuccessRate = iWingChaosMoney/4000000;
	lpObj->ChaosSuccessRate += iChaosMoney/40000;		// 엑셀런트 아이템은 기존의 카오스 조합(돈으로 보는것하고) 같이한다


	if(lpObj->ChaosSuccessRate < g_GlobalConfig.m_iWingCombinationRate)
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iWingCombinationRate;

#ifdef MODIFY_CHAOSMIX_MONEY_OVER_MAX_FIX_RATE_20071114
	if( lpObj->ChaosSuccessRate < 0 )
	{
		lpObj->ChaosSuccessRate = 100;
	}
#endif // MODIFY_CHAOSMIX_MONEY_OVER_MAX_FIX_RATE_20071114	

	if( lpObj->ChaosSuccessRate == 0 )
	{
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return FALSE;	// 0%면 조합이 안되게 한다
	}

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogChaosItem(lpObj, "WingMix,2");
	LogAddTD("[WingMix,2] Chaos Mix Start");	
#else
	LogChaosItem(lpObj, "WingMix");
	LogAddTD("[WingMix] Chaos Mix Start");	
#endif // MODIFY_LOG_SYSTEM_2_3_20060828

	if( lpObj->ChaosSuccessRate > 100 )
		lpObj->ChaosSuccessRate = 100;	// 최대 확률은 90%

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#if TESTSERVER == 1
	char msg[255];
	wsprintf(msg, "날개조합확률=%d", lpObj->ChaosSuccessRate);
	GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
#endif
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 2차날개 (다크로드 망토) 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);	

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	// 성공 날개 생성	
		int itemlevel = 0;
		int type;
		int index;

#ifdef DARKLORD_WORK
		if( iSleeveOfLord )
		{	// 군주의 소매 생성
			type = 13;
			index = 30;
		}
		else
		{	// 날개 생성
			type = 12;
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 절망의 날개 생성
			int tmpIndex  = rand() % 5;

			if(tmpIndex == 4)	// 절망의 날개
			{
				index = 42;
			}
			else
			{
				index = 3 + tmpIndex;
			}
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			// 6차 과금 아이템 날개 조합 부적 ( 해당 클래스에 맞는 아이템이 나온다. )
			if(iWingCharmItemNum > -1) {
				// 날개 조합 부적의 인덱스를 추출해서 해당 날개의 index를 구하자.
				int iWingCharmIndex = iWingCharmItemNum ^ (13 << ITEM_BITSHIFT);
				switch( iWingCharmIndex )
				{
				case 88: // 드라곤날개 부적 
					index = 5;
					break;
				case 89: // 영혼날개 부적
					index = 4;
					break;
				case 90: // 정령날개 부적
					index = 3;
					break;
				case 91: // 절망날개 부적
					index = 42;
					break;
				case 92: // 암흑날개 부적
					index = 6;
					break;
				default:
					// 이건 아직 등록되지 않은 조합 부적이네.. (해당 부적이 없는지 확인하거나 type이 13이 맞는지 확인하라..
					LogAddTD("[MixSystem][WingChaosMix] WingCharmItemNum => %d , WingCharmIndex => %d",iWingCharmItemNum ,iWingCharmIndex);
					break;
				}
			}			
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#else	// ADD_SUMMONER_SECOND_THIRD_WING_20071130
			index = 3+rand()%4;
#endif	// ADD_SUMMONER_SECOND_THIRD_WING_20071130
		}
#else
		type = 12;
		index = 3+rand()%4;
#endif

		int itemtype = MAKE_ITEMNUM(type, index);

		int Option2 = 0;
		int Option3 = 0;
		int NOption = 0;

		if( rand()%5 == 0 )
		{	// 행운
			Option2 = 1;
		}

		int option3rand, optionc;

		option3rand = (rand()%100);
		optionc		= (rand()%3);

		switch( optionc )
		{	// 꽝 80%
		case 0 :
			if( option3rand < 4 ) Option3 = 3;	// 옵션
			break;
		case 1 :
			if( option3rand < 10 ) Option3 = 2;	// 옵션
			break;
		case 2 :
			if( option3rand < 20 ) Option3 = 1;	// 옵션
			break;
		}
		
#ifdef DARKLORD_WORK
		if( iSleeveOfLord )
		{
			if( rand()%5 == 0 )
			{
				NOption = 1<<(rand()%4);				

#ifndef SLEEVEOFLORD_OPTION_BUG_PATCH
				NOption |= PLUS_WING_OP1_TYPE;			
#endif
			}	

#ifdef SLEEVEOFLORD_OPTION_BUG_PATCH
			// 망토는 무조건 추가 공격력
				NOption |= PLUS_WING_OP1_TYPE;			
#endif
			// 망토일때는 옵션2가 하나 더 들어 간다(통솔 추가)
		} else
#endif
		{
			if( rand()%5 == 0 )
			{
				NOption = 1<<(rand()%3);
			}
			
			if( rand()%2 )
			{
				NOption |= PLUS_WING_OP1_TYPE;
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, itemlevel, 0, 0, Option2, Option3, -1, NOption);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
			LogAddTD("[WingMix,2] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

	#else
			LogAddTD("[WingMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
			LogAddTD("[WingMix,2] [%s][%s] CBMix Success %d Money : %d-%d", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);

	#else
			LogAddTD("[WingMix] [%s][%s] CBMix Success %d Money : %d-%d", 
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return 1;
	}
	else
	{	// 조합실패	
		//ChaosBoxInit(lpObj);
		for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
		{
			lpObj->pChaosBox[n].Clear();		
		}

		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
			LogAddTD("[WingMix,2] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
	#else
			LogAddTD("[WingMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
			LogAddTD("[WingMix,2] [%s][%s] CBMix Fail %d Money : %d-%d",  
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	#else
			LogAddTD("[WingMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
				   lpObj->AccountID, lpObj->Name, 
				   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;
	return 0;
}

// 블러드캐슬
void CMixSystem::BloodCastleItemChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

	int aIndex = lpObj->m_Index;

	// 대천사의서, 블러드본 체크
	int iRET_VAL = g_BloodCastle.CheckChoasMixItem(aIndex);

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 맵 인덱스 변경
	int iMapNum = g_BloodCastle.GetMapNumByBCBridge( iRET_VAL - 1 );
	if (CHECK_BLOODCASTLE( iMapNum ) ) 
	{
#else
	if (CHECK_BLOODCASTLE(iRET_VAL - 1 + MAP_INDEX_BLOODCASTLE1)) {
#endif
		// 특정 레벨의 블러드캐슬 입장권 (투망) 재료가 있다. => 블러드캐슬 조합을 한다.
		if( !g_BloodCastle.BloodCastleChaosMix(aIndex, iRET_VAL) )				
			lpObj->ChaosLock = FALSE;
		return;
	}
	else {
		// 그외의 에러들
		switch(iRET_VAL) {
		case 0 :		// 예외 오류 - 이경우엔 일단 내려간다.
			{
			}
			break;
		case 8 :		// 다른 아이템이 존재 -> 기존의 카오스 시스템으로 넘김
			{
			}
			break;
		case 9 :		// 아이템 레벨이 맞지않음
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 10 :		// 혼석이 없다. (혼석은 가장 기본이 되므로 카오스조합 않됨)
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 11 :		// 블러드 카오스 아이템 중 하나가 없다. (적어도 한개는 있음)
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 12 :		// 이벤트 아이템이 너무 많다.
			{
				pMsg.Result	= 0x08;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 13 :		// 조합에 필요한 돈이 모자르다. (사실 여기의 리턴값에는 없고 조합시 처리 된다.)
			{
				pMsg.Result	= 0x0B;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 14 :		// 조합을 하려는 나의 레벨이 낮다.
			{
				pMsg.Result	= 0x09;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		case 15:
			{
				pMsg.Result	= 0xF0;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		default:
			{
			}
			break;
		}
	}	

	lpObj->ChaosLock = FALSE;
}

#ifdef DARKLORD_WORK
// 다크로드 펫 조합
void CMixSystem::DarkHorseChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	int ChoasGemCount = 0;				// 혼석 수
	int BlessGemCount = 0;				// 축석 수
	int SoulGemCount = 0;				// 영석 수	
	int CreatureGemCount = 0;			// 창석 수	

	int	SoulOfDarkHorse = 0;			// 다크호스의 영혼 수
	
	int	iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;				

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
	int iInvalidItemCount = 0;
#endif

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축석
				BlessGemCount++;
			}
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
			else
#endif
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영석
				SoulGemCount++;				
			}
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
			else
#endif
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 22) )
			{	// 창석		
				CreatureGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 31) && lpObj->pChaosBox[n].m_Level == 0)
			{	// 다크호스의 영혼
				SoulOfDarkHorse++;				
			}
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
#endif

		}
	}

#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
	if( ChoasGemCount != 1 || BlessGemCount != 5 ||  SoulGemCount != 5 || CreatureGemCount != 1 || SoulOfDarkHorse != 1 
		|| iInvalidItemCount > 0 )
#else
	if( ChoasGemCount != 1 || BlessGemCount != 5 ||  SoulGemCount != 5 || CreatureGemCount != 1 || SoulOfDarkHorse != 1 )
#endif
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	LogChaosItem(lpObj, "DarkHorseMix");
	LogAddTD("[DarkHorseMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 60;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 5000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 다크호스 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 다크호스 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int itemlevel = 0;
		int type = 13;
		int index = 4;
		int itemtype = MAKE_ITEMNUM(type, index);

		int Option2 = 0;
		int Option3 = 0;
		int NOption = 0;

		int	Option1 = 0;

		if((rand()%100) < (6+(lpObj->ChaosSuccessRate/5)) ) Option1 = 1;

		if( rand()%5 == 0 )
		{	// 행운
			Option2 = 1;
		}

		int option3rand, optionc;

		option3rand = (rand()%100);
		optionc		= (rand()%3);

		switch( optionc )
		{	// 꽝 80%
		case 0 :
			if( option3rand < 4 ) Option3 = 3;	// 옵션
			break;
		case 1 :
			if( option3rand < 10 ) Option3 = 2;	// 옵션
			break;
		case 2 :
			if( option3rand < 20 ) Option3 = 1;	// 옵션
			break;
		}

		PetItemSerialCreateSend(lpObj->m_Index, 254, 0, 0, itemtype, itemlevel, 0, Option1, Option2, Option3, -1, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DarkHorseMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[DarkHorseMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DarkHorseMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[DarkHorseMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;	
}


void CMixSystem::DarkSpiritChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	int ChoasGemCount = 0;				// 혼석 수
	int BlessGemCount = 0;				// 축석 수
	int SoulGemCount = 0;				// 영석 수	
	int CreatureGemCount = 0;			// 창석 수	

	int	SoulOfSpirit = 0;			// 다크호스의 영혼 수
	
	int	iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
	int iInvalidItemCount = 0;
#endif

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축석
				BlessGemCount++;
			}
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
			else
#endif
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영석
				SoulGemCount++;				
			}
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
			else
#endif
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 22) )
			{	// 창석		
				CreatureGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 31) && lpObj->pChaosBox[n].m_Level == 1 )
			{	// 다크스피릿의 영혼
				SoulOfSpirit++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
			else
			{
				iInvalidItemCount++;
			}
#endif
			
		}
	}

#ifdef MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117
	if( ChoasGemCount != 1 || BlessGemCount != 2 ||  SoulGemCount != 2 || CreatureGemCount != 1 || SoulOfSpirit != 1 
		|| iInvalidItemCount > 0 )
#else
	if( ChoasGemCount != 1 || BlessGemCount != 2 ||  SoulGemCount != 2 || CreatureGemCount != 1 || SoulOfSpirit != 1 )
#endif
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	LogChaosItem(lpObj, "DarkSpiritMix");
	LogAddTD("[DarkSpiritMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 60;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 1000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 다크스피릿 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 다크스피릿 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int itemlevel = 0;
		int type = 13;
		int index = 5;
		int itemtype = MAKE_ITEMNUM(type, index);

		int Option2 = 0;
		int Option3 = 0;
		int NOption = 0;

		int	Option1 = 0;

		if((rand()%100) < (6+(lpObj->ChaosSuccessRate/5)) ) Option1 = 1;

		if( rand()%5 == 0 )
		{	// 행운
			Option2 = 1;
		}

		int option3rand, optionc;

		option3rand = (rand()%100);
		optionc		= (rand()%3);

		switch( optionc )
		{	// 꽝 80%
		case 0 :
			if( option3rand < 4 ) Option3 = 3;	// 옵션
			break;
		case 1 :
			if( option3rand < 10 ) Option3 = 2;	// 옵션
			break;
		case 2 :
			if( option3rand < 20 ) Option3 = 1;	// 옵션
			break;
		}

		PetItemSerialCreateSend(lpObj->m_Index, 254, 0, 0, itemtype, itemlevel, 0, Option1, Option2, Option3, -1, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DarkSpiritMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[DarkSpiritMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[DarkSpiritMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[DarkSpiritMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;	
}
#endif


#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
// 공성 아이템 조합
void CMixSystem::BlessPotionChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	int iBlessGemCount		= 0;			// 축복의 보석 수
	int	iChaosMoney			= 0;			// 조합 비용
	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	
	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축복의 보석
				iBlessGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

#ifdef MODIFY_BLESS_SOUL_POTION_CREATION_BUG_20050711
	if( iBlessGemCount == 0 )
		return;

	if( iInvalidItemCount > 0 )
		return;
#endif

#ifdef MODIFY_LUCKYCHARM_BLOCK_USE_CASTLEPOTION_20071105
	if( iBlessGemCount == 0 || iInvalidItemCount > 0 )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#else
	#ifdef MODIFY_BLESS_SOUL_POTION_CREATION_BUG_20050711
	if( iBlessGemCount == 0 )
		return;
	
	if( iInvalidItemCount > 0 )
		return;
	#endif
#endif // MODIFY_LUCKYCHARM_BLOCK_USE_CASTLEPOTION_20071105	

#ifdef MODIFY_CASTLE_POTION_20051017
	// 내구력 최대가 255 까지므로 보석 개수를 25개까지로 제한해야 한다.
	if( iBlessGemCount > 25 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(1737) );			// "최대 보석 25개로 조합 할 수 있습니다."
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "BlessPotionMix");
	LogAddTD("[BlessPotionMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 100000;
	

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 공성 : 축복의 물약 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		// 돈이 모자르다.
		pMsg.Result		= 0x02;		
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 공성 : 축복의 물약 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(14, 7);
		int iItemLevel		= 0;	// 축복의 물약
		
#ifdef MODIFY_CASTLE_POTION_20051017
		int iItemDurability = 10*iBlessGemCount;
#else
		int iItemDurability = 5*iBlessGemCount;
#endif

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, 0, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

		LogAddTD("[BlessPotionMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		return;
	}
	else
	{	
		// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[BlessPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	}	


	lpObj->ChaosLock = FALSE;	
}

void CMixSystem::SoulPotionChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iSoulGemCount		= 0;			// 축복의 보석 수
	int	iChaosMoney			= 0;			// 조합 비용
	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	
	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영혼의 보석
				iSoulGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}


#ifdef MODIFY_BLESS_SOUL_POTION_CREATION_BUG_20050711
	if( iSoulGemCount == 0 )
		return;

	if( iInvalidItemCount > 0 )
		return;
#endif

#ifdef MODIFY_CASTLE_POTION_20051017
	// 내구력 최대가 255 까지므로 보석 개수를 25개까지로 제한해야 한다.
	if( iSoulGemCount > 25 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( lpObj->m_Index, lMsg.Get(1737) );			// "최대 보석 25개로 조합 할 수 있습니다."
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif


	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "SoulPotionMix");
	LogAddTD("[SoulPotionMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 50000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 공성 : 영혼의 물약 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		// 돈이 모자르다.
		pMsg.Result		= 0x02;		
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 공성 : 영혼의 물약 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(14, 7);
		int iItemLevel		= 1;	// 영혼의 물약

#ifdef MODIFY_CASTLE_POTION_20051017
		int iItemDurability = 10*iSoulGemCount;
#else
		int iItemDurability = 5*iSoulGemCount;
#endif

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, 0, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

		LogAddTD("[BlessPotionMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		return;
	}
	else
	{	
		// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[SoulPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::LifeStoneChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iChoasGemCount		= 0;			// 혼돈의 보석 수
	int iBlessGemCount		= 0;			// 축복의 보석 수
	int iSoulGemCount		= 0;			// 영혼의 보석 수	
	int iProtectionGemCount = 0;			// 수호의 보석 수 
	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	int	iChaosMoney			= 0;			// 조합 비용

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축복의 보석
				iBlessGemCount++;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영혼의 보석
				iSoulGemCount++;				
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 31) )
			{	// 수호의 보석		
				iProtectionGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				iChoasGemCount++;				
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iChoasGemCount != 1 || iBlessGemCount != 5 ||  iSoulGemCount != 5 || iProtectionGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "LifeStoneMix");
	LogAddTD("[LifeStoneMix] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 5000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// 카오스 시스템 세율적용 - 공성 : 라이프 스톤 조합
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// 카오스 조합 세금을 저장 - 공성 : 라이프 스톤 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, MAKE_ITEMNUM(13, 11), 1, 0, 0, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

		LogAddTD("[LifeStoneMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[LifeStoneMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	}	

	lpObj->ChaosLock = FALSE;
}
#endif


#ifdef CASTLE_SPECIAL_ITEMMIX_20050425

#include "ItemBagEx.h"

extern INT				g_iCastleItemMixLimit;		// 성주혜택 특별 아이템 조합 하루 제한 횟수
extern CItemBagEx	*	CastleItemMixItemBag;		// 성주혜택 특별 아이템 조합

// 성주 조합
void CMixSystem::CastleSpecialItemMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iDefendGemCount		= 0;			// 수호의 보석 수
	int iBlessGemMixCount	= 0;			// 축복의 보석 조합 수
	int iSoulGemMixCount	= 0;			// 영혼의 보석 조합 수	
	int iInvalidItemCount	= 0;			// 조합용 아이템 이외

	int	iChaosMoney			= 0;			// 조합 비용

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1 . 성주인지 우선 체크
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) ) {
		if( lpObj->GuildStatus != G_MASTER ) {
#ifdef ADD_CASTLE_TOTAL_GUILD_SCORE_20061123
			LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth( Not GuildMaster )",
				lpObj->AccountID,
				lpObj->Name
				);
#else
			LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth( Not GuildMaster )",
				lpObj->AccountID,
				lpObj->Name
				);
#endif
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}
	else {
		LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth",
			lpObj->AccountID,
			lpObj->Name
			);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif

	// 2 . 제한 횟수를 초과하지 않았는지 체크
#if TESTSERVER != 1
	if (g_iCastleItemMixLimit <= 0) {
		LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - Mix Count Limit Over",
			lpObj->AccountID,
			lpObj->Name
			);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif
	
	// 3 . 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 30) )
			{	// 축복의 보석 조합
				iBlessGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 31) )
			{	// 영혼의 보석
				iSoulGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 31) )
			{	// 수호의 보석		
				iDefendGemCount++;				
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iBlessGemMixCount != 3 ||  iSoulGemMixCount != 3 || iDefendGemCount != 30 || iInvalidItemCount > 0 )
	{
		LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - Item Error (DG:%d, BGx10:%d, SGx10:%d, Other:%d)",
			lpObj->AccountID,
			lpObj->Name,
			iBlessGemMixCount,
			iSoulGemMixCount,
			iDefendGemCount,
			iInvalidItemCount
			);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "Castle Special Mix");
	LogAddTD("[CastleSpecialMix] [%s][%s] Chaos Mix Start",
			lpObj->AccountID,
			lpObj->Name
		);

	lpObj->ChaosSuccessRate	= 100;

	// 4 . 카오스 비용 검사
	int nChaosNeedMoney		= 1000000000;
	if( lpObj->Money < nChaosNeedMoney )
	{	
		LogAddTD("[CastleSpecialMix] [%s][%s] Item Mix Failed - Lack of Money (%d/%d)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->Money,
			nChaosNeedMoney
			);
		pMsg.Result			= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock	= FALSE;
		return;
	}

	// 5 . 조합 결과 아이템 중에 하나를 고름
	CItem objTempItem;
	int nItemIndex;
	int iPickLoopCount		= 500;
	bool bItemPickSuccess	= false;
	while (iPickLoopCount--) {
		if (CastleItemMixItemBag->PickItem(objTempItem, nItemIndex) == FALSE) {
			LogAddTD("[CastleSpecialMix] [%s][%s] Item Pick Failed - Data Error",
				lpObj->AccountID,
				lpObj->Name
				);
			
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
		
		// eventitembag19.txt 의 minlevel은 확률로 사용된다.
		int iItemSuccessRate	= CastleItemMixItemBag->GetMinLevel(nItemIndex);
		if (iItemSuccessRate < rand()%100) {
			bItemPickSuccess	= true;
			break;
		}
	}

	if (bItemPickSuccess == false) {
		LogAddTD("[CastleSpecialMix] [%s][%s] Item Pick Failed - Loop Count Over",
			lpObj->AccountID,
			lpObj->Name
			);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	// *> . 여기까지 오면 실제로 생성할 아이템이 지정된 것이다.

	// 6 . 생성하기 전에 조건에 따라 몇가지 옵션을 수정한다.
	objTempItem.m_Level	= 0;

	// 엑셀런트 옵션이 붙어야 한다면 여기서 실제 수치를 계산 (2 ~ 4개)
	if (objTempItem.m_NewOption) {
		objTempItem.m_NewOption	= 0;
		int iExcelOptionCount	= rand()%3+2;
		while(iExcelOptionCount) {
			int iExcelPosition	= rand()%6;
			BYTE btExcelValue	= 0x01 << iExcelPosition;
			if (!(objTempItem.m_NewOption&btExcelValue)) {
				objTempItem.m_NewOption |= btExcelValue;
				iExcelOptionCount--;
			}
		}

		// 엑셀런트 아이템은 무조건 스클이 붙음
		objTempItem.m_Option1		= 1;

		// 엑셀런트 아이템은 행운 옵션이 붙을 확률이 20%
		if (rand()%100 < 20) {
			objTempItem.m_Option2	= 1;
		}
		else {
			objTempItem.m_Option2	= 0;
		}
	}
	else {
		// 일반 아이템은 무조건 행운 옵션이 붙음
		objTempItem.m_Option2	= 1;
	}

	// 내구력 계산 - 세트옵션은 않들어가므로 고려않함
	objTempItem.m_Durability	= ItemGetDurability(objTempItem.m_Type, objTempItem.m_Level, objTempItem.m_NewOption, 0);

	// 7 . 실제로 조합비용을 삭감한다.
	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 8. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		// 조합성공
		ItemSerialCreateSend(
			lpObj->m_Index, 
			255, 0, 0, 
			objTempItem.m_Type, 
			objTempItem.m_Level, 
			objTempItem.m_Durability, 
			objTempItem.m_Option1, 
			objTempItem.m_Option2, 
			objTempItem.m_Option3,
			-1,
			objTempItem.m_NewOption
			);
		gObjInventoryCommit(lpObj->m_Index);		

		LogAddTD("[CastleSpecialMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);

		g_iCastleItemMixLimit--;
		
#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
		return;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
	}
	else
	{	
		// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[CastleSpecialMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	}	

	lpObj->ChaosLock = FALSE;
}

#endif


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804

void CMixSystem::HiddenTreasureBoxItemMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iBlueCrystal		= 0;			// 푸른 수정
	int iRedCrystal			= 0;			// 붉은 수정
	int iBlackCrystal		= 0;			// 검은 수정
	int iInvalidItemCount	= 0;			// 조합용 아이템 이외

	int	iChaosMoney			= 0;			// 조합 비용

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	
	// 1 . 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 26) )
			{	// 수정 체크
				switch(lpObj->pChaosBox[n].m_Level) {
				case 1:
					iRedCrystal++;
					break;
				case 2:
					iBlueCrystal++;
					break;
				case 3:
					iBlackCrystal++;
					break;
				}
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	// 2 . 조합종류 체크
	INT iCrystalMixType	= -1;
	if (iInvalidItemCount == 0) {
		if (iBlueCrystal == 7) {
			if (iRedCrystal == 0 &&
				iBlackCrystal == 0
				)
			{
				iCrystalMixType = 0;
				lpObj->ChaosSuccessRate	= 100;
			}
		}
		else if (iRedCrystal == 5) {
			if (iBlueCrystal == 0 &&
				iBlackCrystal == 0
				)
			{
				iCrystalMixType = 1;
				lpObj->ChaosSuccessRate	= 100;
			}
		}
		else if (iBlackCrystal == 3) {
			if (iRedCrystal == 0 &&
				iBlueCrystal == 0
				)
			{
				iCrystalMixType = 2;
				lpObj->ChaosSuccessRate	= 100;
			}
		}
		else if (iBlueCrystal == 1
			&& iRedCrystal == 1
			&& iBlackCrystal == 1
			) 
		{
			iCrystalMixType = 3;
			lpObj->ChaosSuccessRate	= 100;		// !!! . 임시
//			lpObj->ChaosSuccessRate	= 5;
		}
		else {
			LogAddTD("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
				lpObj->AccountID,
				lpObj->Name
				);
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;		
		}
	}
	else {
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
			lpObj->AccountID,
			lpObj->Name
			);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "Hidden TreasureBox Mix");
	LogAddTD("[Hidden TreasureBox Event] [%s][%s] Chaos Mix Start",
		lpObj->AccountID,
		lpObj->Name
		);

/*
	// 3 . 카오스 비용 검사
	int nChaosNeedMoney		= 0;
	if( lpObj->Money < nChaosNeedMoney )
	{	
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed - Lack of Money (%d/%d)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->Money,
			nChaosNeedMoney
			);
		pMsg.Result			= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock	= FALSE;
		return;
	}

	// 4 . 실제로 조합비용을 삭감한다.
	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);		
*/

	// 5. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		switch(iCrystalMixType) {
		case 0:		// 푸른 수정 10개
		case 1:		// 붉은 수정 7개
		case 2:		// 검은 수정 3개
			{
				// 조합성공 -> 보물상자 생성
				ItemSerialCreateSend(
					lpObj->m_Index, 
					255, 0, 0, 
					MAKE_ITEMNUM(12,26),
					4, 
					1, 0, 0, 0, -1, 0
					);
			}
			break;
		case 3:		// 푸른 + 붉은 + 검은 수정 각 1개씩
			{
				// 조합성공 -> 보물상자 생성 (프라인 경품)
				ItemSerialCreateSend(
					lpObj->m_Index, 
					255, 0, 0, 
					MAKE_ITEMNUM(12,26),
					5, 
					1, 0, 0, 0, -1, 0
					);
			}
			break;
		default:	// 에러
			{
				LogAddTD("[Hidden TreasureBox Event] [%s][%s] CBMix Failed - iCrystalMixType is wrong : %d", 
					   lpObj->AccountID, 
					   lpObj->Name, 
					   iCrystalMixType
					   );
				DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
			}
			return;
		}

		gObjInventoryCommit(lpObj->m_Index);

		LogAddTD("[Hidden TreasureBox Event] [%s][%s] CBMix Success:%d Type:%d", 
			   lpObj->AccountID, 
			   lpObj->Name, 
			   lpObj->ChaosSuccessRate, 
			   iCrystalMixType
			   );
	}
	else
	{	
		// 조합실패	
		LogAddTD("[Hidden TreasureBox Event] [%s][%s] CBMix Fail Rate:%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate);	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
	}	

	lpObj->ChaosLock = FALSE;
}

#endif


#ifdef ADD_ITEM_FENRIR_01_20051110 
//-----------------------------------------------------------
//
// [ 1 단계 ] - 뿔피리조각 조합
//
// > 재료 - 갑옷 파편(20)		
// > 재료 - 여신의 가호(20)
// 
// > 확률 - 70%
//
//-----------------------------------------------------------
void CMixSystem::Fenrir_01Level_Mix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iStuffCount_01		= 0;			// 갑옷   파편   
	int iStuffCount_02		= 0;			// 여신의 가호 
	int iChaosGemCount		= 0;			// 혼돈의 보석	

	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	int	iChaosMoney			= 0;			// 조합 비용

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 32) )
			{	// 갑옷 파편
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 33) )
			{	// 여신의 가호
				iStuffCount_02 += lpObj->pChaosBox[n].m_Durability;			
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				iChaosGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iStuffCount_01 != 20 || iStuffCount_02 != 20 ||  iChaosGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "Fenrir_01Level_Mix");
	LogAddTD("[Fenrir Mix][Level 01] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = g_iFenrir_01Level_MixRate;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 0;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(13, 35);	// 뿔피리 조각
		int iItemLevel		= 0;	

		int iItemDurability = 1;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, 0, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[Fenrir Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[Fenrir Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	



	lpObj->ChaosLock = FALSE;
}

//-----------------------------------------------------------
//
// [ 2 단계 ] - 부러진 뿔피리 조합
//
// > 재료 - 맹수의 발톱(10)		
// > 재료 - 뿔피리 조각(5)
// 
// > 확률 - 50%
//
//-----------------------------------------------------------
void CMixSystem::Fenrir_02Level_Mix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iStuffCount_01		= 0;			// 맹수의 발톱   
	int iStuffCount_02		= 0;			// 뿔피리 조각 
	int iChaosGemCount		= 0;			// 혼돈의 보석	

	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	int	iChaosMoney			= 0;			// 조합 비용

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 34) )
			{	// 맹수의 발톱
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 35) )
			{	// 뿔피리 조각
				iStuffCount_02++;			
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				iChaosGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iStuffCount_01 != 10 || iStuffCount_02 != 5 ||  iChaosGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "Fenrir_02Level_Mix");
	LogAddTD("[Fenrir Mix][Level 02] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = g_iFenrir_02Level_MixRate;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 0;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(13, 36);	// 부러진 뿔피리 조각
		int iItemLevel		= 0;	

		int iItemDurability = 1;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, 0, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[Fenrir Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney );
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[Fenrir Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;
}

//-----------------------------------------------------------
//
// [ 3 단계 ] - 펜릴의 뿔피리 조합
//
// > 재료 - 부러진 뿔피리(1)		
// > 재료 - 생명의 보석(3)
// > 재료 - 1000만젠
//
// > 확률 - 30%
//
//-----------------------------------------------------------
void CMixSystem::Fenrir_03Level_Mix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iStuffCount_01		= 0;			// 부러진 뿔피리   
	int iLifeGemCount		= 0;			// 생명의 보석 
	int iChaosGemCount		= 0;			// 혼돈의 보석	

	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	int	iChaosMoney			= 0;			// 조합 비용

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 36) )
			{	// 부러진 뿔피리 조각
				iStuffCount_01++;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 16) )
			{	// 생명의 보석
				iLifeGemCount++;			
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				iChaosGemCount++;				
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iStuffCount_01 != 1 || iLifeGemCount != 3 ||  iChaosGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "Fenrir_03Level_Mix");
	LogAddTD("[Fenrir Mix][Level 03] Chaos Mix Start");	

	lpObj->ChaosSuccessRate = g_iFenrir_03Level_MixRate;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	int nChaosNeedMoney = 10000000;

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(13, 37);	// 펜릴의 뿔피리
		int iItemLevel		= 0;	
	
		int iItemDurability = 255;
		int iItemOption1	= 1;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, iItemOption1, 0, 0);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 03] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[Fenrir Mix][Level 03] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 03] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[Fenrir Mix][Level 03] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}	

	lpObj->ChaosLock = FALSE;
}

//-----------------------------------------------------------
//
// [ 4 단계 ] - 펜릴의 뿔피리 업그레이드
//
// > 재료 - 펜릴의 뿔피리(1)		
// > 재료 - 생명의 보석(5)
// > 재료 - 방어구 또는 무기
// 
// > 최대 확률 - 79%
//-----------------------------------------------------------
void CMixSystem::Fenrir_04Upgrade_Mix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iStuffCount_01		= 0;			// 펜릴의 뿔피리  
	int iLifeGemCount		= 0;			// 생명의 보석 
	int iChaosGemCount		= 0;			// 혼돈의 보석
	
	int iAttackStuffCount	= 0;			// 무기구류 아이템 	
	int iDefendStuffCount	= 0;			// 방어구류 아이템 

	int iAttackStuffPrice	= 0;			// 무기구류 아이템 가격
	int iDefendStuffPrice	= 0;			// 방어구류 아이템 가격

	int iInvalidItemCount	= 0;			// 조합용 아이템 이외 

	int	iChaosMoney			= 0;			// 조합 비용

	BOOL bFenrirDamageInc	= FALSE;		// 최종 데미지 증가 옵션
	BOOL bFenrirDamageDec	= FALSE;		// 최종 데미지 감소 옵션

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(13, 37) )
			{	// 펜릴의 뿔피리
				iStuffCount_01++;
			}
			else
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 16) )
			{	// 생명의 보석
				iLifeGemCount++;			
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				iChaosGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type >= ITEM_SWORD && lpObj->pChaosBox[n].m_Type < ITEM_SHIELD 
					&& lpObj->pChaosBox[n].m_Level >=4 && lpObj->pChaosBox[n].m_Option3 >= 1 )
			{	// 무기구류 
				iAttackStuffCount++;
				iAttackStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if( lpObj->pChaosBox[n].m_Type >= ITEM_SHIELD && lpObj->pChaosBox[n].m_Type < ITEM_WING 
					&& lpObj->pChaosBox[n].m_Level >=4 && lpObj->pChaosBox[n].m_Option3 >= 1 )
			{	// 방어구류
				iDefendStuffCount++;
				iDefendStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;

			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903	
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iStuffCount_01 != 1 || iLifeGemCount != 5 ||  iChaosGemCount != 1 || (iAttackStuffCount == 0 && iDefendStuffCount == 0)
		|| iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( iAttackStuffCount > 0 )
	{
		bFenrirDamageInc = TRUE;
		iChaosMoney		 = iAttackStuffPrice;
	}

	if( iDefendStuffCount > 0 )
	{
		bFenrirDamageDec = TRUE;
		iChaosMoney		 = iDefendStuffPrice;
	}
	
	if( bFenrirDamageInc && bFenrirDamageDec )
	{
		// 두 종류 이상은 조합 할 수 없습니다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}
	
	if( !bFenrirDamageInc && !bFenrirDamageDec )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

	// 2. 카오스 비용 검사
	LogChaosItem(lpObj, "Fenrir_04Level_Mix");
	LogAddTD("[Fenrir Mix][Level 04] Chaos Mix Start");	

	int nChaosNeedMoney = 10000000;	// 1000만젠

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			// 카오스 시스템 세율적용
	INT iChaosTaxMoney		= (INT)((INT64)nChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney		= 0;

	nChaosNeedMoney			= nChaosNeedMoney + iChaosTaxMoney;
	if (nChaosNeedMoney < 0)
		nChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= nChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(lpObj->m_Index, lpObj->Money);


	
	
	// 2.1 최대 조합 성공 확률 제한 및 조합 확률 계산 - 아이템 최대 기준값 100만젠

	if( iChaosMoney > 1000000 )
		iChaosMoney = 1000000;

	lpObj->ChaosSuccessRate = (iChaosMoney * 100 ) / 1000000;			// 퍼센트로 정규화

	if( lpObj->ChaosSuccessRate > 79 )
		lpObj->ChaosSuccessRate = 79;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	// 3. 카오스 조합
	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM(13, 37);	// 펜릴의 뿔피리
		int iItemLevel		= 0;	

		int iItemDurability = 255;
		int iItemOption1	= 1;
		int iItemExtOption  = 0;

		if( bFenrirDamageInc )
		{
			iItemExtOption |= PLUS_FENRIR_INCREASE_LAST_DAMAGE;
		}

		if( bFenrirDamageDec )
		{
			iItemExtOption |= PLUS_FENRIR_DECREASE_LAST_DAMAGE;
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, iItemOption1, 0, 0, -1, iItemExtOption);			
		gObjInventoryCommit(lpObj->m_Index);		

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 04] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
#else
		LogAddTD("[Fenrir Mix][Level 04] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[Fenrir Mix][Level 04] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);	
#else
		LogAddTD("[Fenrir Mix][Level 04] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}

	lpObj->ChaosLock = FALSE;
}

#endif // ADD_ITEM_FENRIR_01_20051110


#ifdef ADD_SHIELD_POINT_01_20060403
void CMixSystem::ShieldPotionLv1_Mix(LPOBJECTSTRUCT lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if( g_ShieldSystemOn == FALSE )
	{
		return;
	}

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 3) )
			{	// 큰치료물약 갯수
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( ( iInvalidItemCount > 0 ) || ( iHealthPotionCount != 3 ) )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef UPDATE_SHIELD_OPTION_20060427
	iChaosMixPrice = g_iShieldPotionLv1MixMoney;
#else
	iChaosMixPrice = 10000;
#endif


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
	LogChaosItem( lpObj, "PotionMix][ShieldPotion Lv1 Mix");
	LogAddTD("[PotionMix][ShieldPotion Lv1 Mix] - Mix Start");
#endif

#ifdef UPDATE_SHIELD_OPTION_20060427
	int iRate = rand()%100;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	iRate -= iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if ( iRate < g_iShieldPotionLv1MixSuccessRate )	// 확률 적용
	{
		// CODEREVIEW_SHIELD : 공성물약 참조 - 아이템 두개 생성 안됨.
		int iItemType = MAKE_ITEMNUM(14,35);	// 작은 SD 회복 물약
		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
		gObjInventoryCommit( lpObj->m_Index );

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv1 Mix]");
#endif
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv1 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv1 Potion Mix Success [%s][%s], Money(%d-%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
	else
	{	// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv1 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv1MixSuccessRate, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);	
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv1 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv1MixSuccessRate, lpObj->Money, iChaosMixPrice);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
#else
	// CODEREVIEW_SHIELD : 공성물약 참조 - 아이템 두개 생성 안됨.
	int iItemType = MAKE_ITEMNUM(14,35);	// 작은 SD 회복 물약
	ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
	gObjInventoryCommit( lpObj->m_Index );

	LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv1 Mix]");
	LogAddTD("[ChaosMix][Shield Potion] Lv1 Potion Mix Success [%s][%s], Money(%d-%d)",
		lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // UPDATE_SHIELD_OPTION_20060427
	
	lpObj->ChaosLock = FALSE;
}

void CMixSystem::ShieldPotionLv2_Mix(LPOBJECTSTRUCT lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 38) )
			{	// 작은 복합물약 갯수
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( ( iInvalidItemCount > 0 ) || ( iHealthPotionCount != 3 ) )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef UPDATE_SHIELD_OPTION_20060427
	iChaosMixPrice = g_iShieldPotionLv2MixMoney;
#else
	iChaosMixPrice = 50000;
#endif

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

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	LogChaosItem( lpObj, "PotionMix][ShieldPotion Lv2 Mix");
	LogAddTD("[PotionMix][ShieldPotion Lv2 Mix] - Mix Start");
#endif

	GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef UPDATE_SHIELD_OPTION_20060427
	int iRate = rand()%100;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	iRate -= iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if ( iRate < g_iShieldPotionLv2MixSuccessRate )	// 확률 적용
	{
		int iItemType = MAKE_ITEMNUM(14,36);	// 중간 SD 회복 물약

		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
		gObjInventoryCommit( lpObj->m_Index );

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv2 Mix]");
#endif
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv2 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv2 Potion Mix Success [%s][%s], Money(%d-%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
	else
	{	// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv2 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv2MixSuccessRate, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);	
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv2 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv2MixSuccessRate, lpObj->Money, iChaosMixPrice);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
#else
	int iItemType = MAKE_ITEMNUM(14,36);	// 중간 SD 회복 물약

	ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
	gObjInventoryCommit( lpObj->m_Index );

	LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv2 Mix]");
	LogAddTD("[ChaosMix][Shield Potion] Lv2 Potion Mix Success [%s][%s], Money(%d-%d)",
		lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // UPDATE_SHIELD_OPTION_20060427

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::ShieldPotionLv3_Mix(LPOBJECTSTRUCT lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 39) )
			{	// 중간 복합물약 갯수
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( ( iInvalidItemCount > 0 ) || ( iHealthPotionCount != 3 ) )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef UPDATE_SHIELD_OPTION_20060427
	iChaosMixPrice = g_iShieldPotionLv3MixMoney;
#else
	iChaosMixPrice = 100000;
#endif

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
	LogChaosItem( lpObj, "PotionMix][ShieldPotion Lv3 Mix");
	LogAddTD("[PotionMix][ShieldPotion Lv3 Mix] - Mix Start");
#endif

#ifdef UPDATE_SHIELD_OPTION_20060427
	int iRate = rand()%100;

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	iRate -= iCharmOfLuckCount;
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

	if ( iRate < g_iShieldPotionLv3MixSuccessRate )	// 확률 적용
	{
		int iItemType = MAKE_ITEMNUM(14,37);	// 큰 SD 회복 물약

		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
		gObjInventoryCommit( lpObj->m_Index );

#ifndef MODIFY_LOG_SYSTEM_2_3_20060828
		LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv3 Mix]");
#endif
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv3 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv3 Potion Mix Success [%s][%s], Money(%d-%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
	else
	{	// 조합실패	
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
		LogAddTD("[ChaosMix][Shield Potion] Lv3 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv3MixSuccessRate, lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);	
#else
		LogAddTD("[ChaosMix][Shield Potion] Lv3 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   g_iShieldPotionLv3MixSuccessRate, lpObj->Money, iChaosMixPrice);	
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
	}
#else
	int iItemType = MAKE_ITEMNUM(14,37);	// 큰 SD 회복 물약

	ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0 );
	gObjInventoryCommit( lpObj->m_Index );

	LogChaosItem( lpObj, "[PotionMix][ShieldPotion Lv3 Mix]");
	LogAddTD("[ChaosMix][Shield Potion] Lv3 Potion Mix Success [%s][%s], Money(%d-%d)",
		lpObj->AccountID, lpObj->Name, lpObj->Money, iChaosMixPrice);
#endif // UPDATE_SHIELD_OPTION_20060427

	lpObj->ChaosLock = FALSE;
}
#endif

#ifdef ADD_THIRD_WING_20070525	// 3차 날개 조합
// 조합 가능한 아이템인가?
BOOL CMixSystem::IsMixPossibleItem( int pItemType )
{
	if( pItemType == CONDOR_FEATHER_ITEMINDEX 
		|| pItemType == CONDOR_FLAME_ITEMINDEX	
		)
	{
		return TRUE;
	}

	return FALSE;
}

// 3차 날개 조합 1단계
//////////////////////////////////////////////////////////////////////////
// +9 4옵(자생1%) 이상의 2차날개
// +7 4옵 이상 세트아이템
// 혼돈의 보석1개, 영혼의 보석 10개붂음 1개, 창조의 보석1개
//
// 조합 성공 확률 (세트아이템의 총 구입가격)/300000
// 최대 확률 60%
// 조합비용 : 성공확률 * 20만젠
//
// 조합 성공 : 콘도르의 깃털
// 조합 실패 :	2차날개 - 옵션0, 레벨 2~3 떨어짐
//				세트아이템 - 옵션0, 레벨 2~3 떨어짐
//				석, 조합비용 사라짐
//				
//////////////////////////////////////////////////////////////////////////
// 날개 아이템인지 검사
// 1차 날개
BOOL CMixSystem::CheckWingItem( int pType )
{
	if( pType == MAKE_ITEMNUM( 12, 0 )		// 요정날개
		|| pType == MAKE_ITEMNUM( 12, 1 )	// 천공의날개
		|| pType == MAKE_ITEMNUM( 12, 2 )	// 사탄날개
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 신규캐릭터 1차날개(재앙의 날개) 아이템 검사
		|| pType == MAKE_ITEMNUM( 12, 41 )	// 신규캐릭터 1차날개(재앙의 날개)
#endif
		)
	{
		return TRUE;
	}

	return FALSE;
}

// 2차 날개
BOOL CMixSystem::Check2ndWingItem( int pType )
{
	if( pType == MAKE_ITEMNUM( 12, 3 )		// 정령의날개
		|| pType == MAKE_ITEMNUM( 12, 4 )	// 영혼의날개
		|| pType == MAKE_ITEMNUM( 12, 5 )	// 드라곤의날개
		|| pType == MAKE_ITEMNUM( 12, 6 )	// 암흑의날개
		|| pType == MAKE_ITEMNUM( 13, 30 )	// 군주의망토
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130
		|| pType == MAKE_ITEMNUM( 12, 42 )	// 절망의 날개
#endif
		)
	{
		return TRUE;
	}

	return FALSE;
}

// 3차 날개
BOOL CMixSystem::Check3rdWingItem( int pType )
{
	if( pType == WING_OF_STORM_ITEMINDEX		// 폭풍의날개
		|| pType == WING_OF_ETERNAL_ITEMINDEX	// 시공의날개
		|| pType == WING_OF_ILLUSION_ITEMINDEX	// 환영의날개
		|| pType == WING_OF_RUIN_ITEMINDEX		// 파멸의날개
		|| pType == CAPE_OF_EMPEROR_ITEMINDEX	// 제왕의망토
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203
		|| pType == WING_OF_DIMENSION_ITEMINDEX	// 차원의 날개
#endif
		)
	{
		return TRUE;
	}

	return FALSE;
}

// 특정 조건을 만족하는 아이템인가? (주어진 수치 이상이어야 한다.)
BOOL CMixSystem::CheckLevelCondition( CItem* pItem, short pLevel, BYTE pOpt1, BYTE pOpt2, BYTE pOpt3, BYTE pSetOpt, BYTE pExOpt )
{
	if( pItem == NULL )
		return FALSE;
	
	if( pLevel != 0 && pItem->m_Level < pLevel )
		return FALSE;

	if( pOpt1 != 0 && pItem->m_Option1 < pOpt1 )	
		return FALSE;
	
	if( pOpt2 != 0 && pItem->m_Option2 < pOpt2 )
		return FALSE;

	if( pOpt3 != 0 && pItem->m_Option3 < pOpt3 )
		return FALSE;

	if( pSetOpt != 0 && pItem->m_SetOption == 0 )
		return FALSE;			

	if( pExOpt != 0 && pItem->m_NewOption == 0 )
			return FALSE;			

	return TRUE;
}

void CMixSystem::ThirdWingLevel1_Mix( LPOBJECTSTRUCT lpObj )
{
	lpObj->ChaosLock = TRUE;

	int iSecondWing			= 0;	// 2차날개
	int iSetItem			= 0;	// 세트 아이템

	int iChaosGemCount		= 0;	// 혼돈의 보석
	int iSoulGembundle		= 0;	// 영혼의 보석 묶음
	int iCreatureGemCount	= 0;	// 창조의 보석

	int iInvalidItemCount	= 0;	// 조합용 아이템 외의 아이템

	int iSetItemMoney		= 0;	// 세트 아이템 구입 비용
	int iChaosNeedMoney		= 0;	// 조합 비용	

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int iCharmOfLuckCount	= 0;	// 행운의 부적
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;
	
	// 1. 카오스조합창 아이템 검사
	for( int n = 0; n < MAX_CHAOSBOXITEMS; n++ )
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( Check2ndWingItem( lpObj->pChaosBox[n].m_Type ) )
			{	// 2차날개
				if( CheckLevelCondition( &lpObj->pChaosBox[n], 9, 0, 0, 1 ) )
					iSecondWing++;
			}
			else if( CheckLevelCondition( &lpObj->pChaosBox[n], 7, 0, 0, 1, 1 ) )
			{	// 세트 아이템
				iSetItem++;			
				iSetItemMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if( lpObj->pChaosBox[n].m_Type == CHAOS_GEM_ITEMINDEX )
			{	// 혼돈의 보석
				iChaosGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == SOUL_GEM_BULDLE_ITEMINDEX )
			{	// 영혼의 보석 묶음
				iSoulGembundle++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == CREATURE_GEM_ITEMINDEX )
			{	// 창조의 보석
				iCreatureGemCount++;				
			}
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else
			{
				iInvalidItemCount++;
			}
		}
	}

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	if( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	if( iSecondWing != 1 || iSetItem < 1 ||  iChaosGemCount != 1 || iSoulGembundle != 1 || iCreatureGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;		
		return;
	}

	// 2. 카오스 비용 검사
	LogChaosItem( lpObj, "ThirdWingLevel1_Mix" );
	LogAddTD( "[ThirdWing Mix][Level 01] Chaos Mix Start" );	

#ifdef TEST_3RD_20070622
	if( iSetItemMoney > 0 )
		lpObj->ChaosSuccessRate = iSetItemMoney / 30000;
	if( lpObj->ChaosSuccessRate > 100 )
		lpObj->ChaosSuccessRate = 100;
	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;	
#else	
	if( iSetItemMoney > 0 )
		lpObj->ChaosSuccessRate = iSetItemMoney / 300000;

#ifdef MODIFY_THIRD_WING_MIX_RATE_20080116
	if( lpObj->ChaosSuccessRate > g_GlobalConfig.m_iThirdWingCombinationRate )
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iThirdWingCombinationRate;
#else
	if( lpObj->ChaosSuccessRate > g_GlobalConfig.m_iThirdWingCombinationRate )
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iThirdWingCombinationRate;
#endif // MODIFY_THIRD_WING_MIX_RATE_20080116
	else if( lpObj->ChaosSuccessRate < 1 )
		lpObj->ChaosSuccessRate = 1;

	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;	
#endif

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	// 카오스 시스템 세율적용
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			
	INT iChaosTaxMoney		= (INT)((INT64)iChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
		iChaosTaxMoney		= 0;

	iChaosNeedMoney			= iChaosNeedMoney + iChaosTaxMoney;
	if( iChaosNeedMoney < 0 )
		iChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < iChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= iChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend( lpObj->m_Index, lpObj->Money );		

	// 3. 카오스 조합
	if( ( rand() % 100 ) < lpObj->ChaosSuccessRate )
	{	
		int iItemType		= MAKE_ITEMNUM( 13, 53 );	// 콘도르의 깃털
		int iItemLevel		= 0;
		int iItemDurability = 1;

		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, iItemLevel, iItemDurability, 0, 0, 0 );			
		gObjInventoryCommit( lpObj->m_Index );

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD( "[ThirdWing Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney,
			   iCharmOfLuckCount );
#else
		LogAddTD( "[ThirdWing Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney );
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		return;
	}
	else
	{	// 조합실패		
		ThirdWingMixFail( lpObj );		
		GCUserChaosBoxSend( lpObj );
		DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD( "[ThirdWing Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney,
			   iCharmOfLuckCount );
#else
		LogAddTD( "[ThirdWing Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney );	
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	}

	lpObj->ChaosLock = FALSE;
}

// 3차 날개 조합 2단계
void CMixSystem::ThirdWingLevel2_Mix( LPOBJECTSTRUCT lpObj )
{
	lpObj->ChaosLock = TRUE;
	
	int iExItem				= 0;	// 엑설런트 아이템
	int iCondorFeather		= 0;	// 콘도르의깃털
	int iCondorFlame		= 0;	// 콘도르의불꽃

	int iChaosGemCount		= 0;	// 혼돈의 보석
	int iSoulGemBundle		= 0;	// 영혼의 보석 묶음
	int iBlessGemBundle		= 0;	// 축복의 보석 묶음
	int iCreatureGemCount	= 0;	// 창조의 보석

	int iInvalidItemCount	= 0;	// 조합용 아이템 외의 아이템

#ifdef MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423	
	DWORD iExItemMoney		= 0;	// 엑설런트 아이템 구입 비용
#else	// MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
	int iExItemMoney		= 0;	// 엑설런트 아이템 구입 비용
#endif	// MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
	int iChaosNeedMoney		= 0;	// 조합 비용	

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB( ( LPBYTE )&pMsg, 0x86, sizeof( pMsg ) );
	pMsg.Result	= 0x00;
	
	// 1. 카오스조합창 아이템 검사
	for( int n = 0; n < MAX_CHAOSBOXITEMS; n++ )
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{			
			if( CheckLevelCondition( &lpObj->pChaosBox[n], 7, 0, 0, 1, 0, 1 ) )
			{	// 엑설런트 아이템
				iExItem++;			
				iExItemMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if( lpObj->pChaosBox[n].m_Type == CONDOR_FEATHER_ITEMINDEX )
			{	// 콘도르의깃털
				iCondorFeather++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == CONDOR_FLAME_ITEMINDEX )
			{	// 콘도르의불꽃
				iCondorFlame++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == CHAOS_GEM_ITEMINDEX )
			{	// 혼돈의보석
				iChaosGemCount++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == BLESS_GEM_BUNDLE_ITEMINDEX )
			{	// 축복의보석 묶음
				iBlessGemBundle++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == SOUL_GEM_BULDLE_ITEMINDEX )
			{	// 영혼의보석 묶음
				iSoulGemBundle++;				
			}
			else if( lpObj->pChaosBox[n].m_Type == CREATURE_GEM_ITEMINDEX )
			{	// 창조의보석
				iCreatureGemCount++;				
			}
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else
			{
				iInvalidItemCount++;
			}
		}
	}

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	if( iCharmOfLuckCount > 10 )
	{
		// 행운의 부적 최대 사용 갯수 초과
		pMsg.Result = 0xF0;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	if( iExItem < 1 || iCondorFeather != 1 || iCondorFlame != 1 
		|| iChaosGemCount != 1 || iBlessGemBundle != 1 || iSoulGemBundle != 1 || iCreatureGemCount != 1
		|| iInvalidItemCount > 0 )
	{
		pMsg.Result		= 0x07;		// 조합할 아이템이 잘못되었다
		DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;		
		return;
	}

	// 2. 카오스 비용 검사
	LogChaosItem( lpObj, "ThirdWingLevel2_Mix" );
	LogAddTD( "[ThirdWing Mix][Level 02] Chaos Mix Start" );	

#ifdef TEST_3RD_20070622
	if( iExItemMoney > 0 )
		lpObj->ChaosSuccessRate = iExItemMoney / 2000000;
	if( lpObj->ChaosSuccessRate > 100 )
		lpObj->ChaosSuccessRate = 100;
	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;	
#else
#ifdef MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
	if( iExItemMoney > 0 )
		lpObj->ChaosSuccessRate = iExItemMoney / 3000000 + 1;
#else	// MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
	if( iExItemMoney > 0 )
		lpObj->ChaosSuccessRate = iExItemMoney / 3000000;
#endif	// MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
	if( lpObj->ChaosSuccessRate > g_GlobalConfig.m_iThirdWingCombinationRate )
		lpObj->ChaosSuccessRate = g_GlobalConfig.m_iThirdWingCombinationRate;
	else if( lpObj->ChaosSuccessRate < 1 )
		lpObj->ChaosSuccessRate = 1;
	
	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;	
#endif

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	iChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;	
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	// 카오스 시스템 세율적용
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219			
	INT iChaosTaxMoney		= (INT)((INT64)iChaosNeedMoney * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
		iChaosTaxMoney		= 0;

	iChaosNeedMoney			= iChaosNeedMoney + iChaosTaxMoney;
	if( iChaosNeedMoney < 0 )
		iChaosNeedMoney		= 0;
#endif

	if( lpObj->Money < iChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
	lpObj->Money -= iChaosNeedMoney;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130			// 카오스 조합 세금을 저장
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );
#endif
	GCMoneySend( lpObj->m_Index, lpObj->Money );		

	// 3. 카오스 조합
	if( ( rand() % 100 ) < lpObj->ChaosSuccessRate )
	{	
		// 날개 랜덤
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203	// 신규캐릭터 3차 날개 조합
		int i3rdWingIndex	= rand() % 6;
#else
		int i3rdWingIndex	= rand() % 5;
#endif
		int iItemType		= 0;

		if( i3rdWingIndex == 0 )      iItemType = WING_OF_STORM_ITEMINDEX;		// 폭풍의날개
		else if( i3rdWingIndex == 1 ) iItemType = WING_OF_ETERNAL_ITEMINDEX;	// 시공의날개
		else if( i3rdWingIndex == 2 ) iItemType = WING_OF_ILLUSION_ITEMINDEX;	// 환영의날개
		else if( i3rdWingIndex == 3 ) iItemType = WING_OF_RUIN_ITEMINDEX;		// 파멸의날개
		else if( i3rdWingIndex == 4 ) iItemType = CAPE_OF_EMPEROR_ITEMINDEX;	// 제왕의망토
#ifdef ADD_SUMMONER_THIRD_WING_MIX_20071203	// 신규캐릭터 3차 날개 조합
		else if( i3rdWingIndex == 5 ) iItemType = WING_OF_DIMENSION_ITEMINDEX;	// 차원의 날개
#endif

		// 옵션 랜덤
		// 날개옵션1 - 스킬 : 날개에는 스킬이 없다.		
		int iOption1 = 0;
		
		// 날개옵션2 - 행운
		int iOption2 = 0;	
		if( rand() % 1000 <= m_i3rdWingOpt_LuckRate )
		{
			iOption2 = 1;
		}

		// 날개옵션3 - 추가 옵션 무조건 붙는다.
		// 1:추가 공격력 2:추가 마력 3:추가 방어력 4:생명자동회복 
		int iOption3 = 0;				

		// 옵션 종류 고르기
		// **캐릭터당 3가지 옵션 중 하나가 적용될 수 있다. (100%)
		// 생명자동회복을 제외한 추가 가능한 두 가지 옵션을 먼저 계산하여 조건에 맞지 않으면
		// 생명자동회복으로 적용한다.
		// *****어떤 옵션인지에 대한 것은 엑설런트 옵션을 사용한다.
		int iNOption = 0; 
		int iOpt3TypeRand = rand()%2;
		int iOpt3RandRate;
		switch( iOpt3TypeRand )
		{
		case 0:
			iOpt3RandRate = rand()%1000;
			if( iOpt3RandRate < 400 ) 
				iNOption |= PLUS_THIRD_WING_RAND_OPT_1;
			break;
		case 1:
			iOpt3RandRate = rand()%1000;
			if( iOpt3TypeRand < 300 ) 
				iNOption |= PLUS_THIRD_WING_RAND_OPT_2;
			break;
		}

		// 옵션에 단계 붙이기 (0~4)
		int iOpt3TypeLevel = rand()%4;
		int iOpt3TypeLevelRate = rand()%1000;

#ifdef TEST_3RD_20070622
		// 기본값은 +0 : 0
		iOption3 = 0;
		switch( iOpt3TypeLevel )
		{
		case 1:
			if( iOpt3TypeLevelRate < 250 ) 
				iOption3 = 1;	// +4
			break;
		case 2:
			if( iOpt3TypeLevelRate < 250 ) 
				iOption3 = 2;	// +8
			break;
		case 3:
			if( iOpt3TypeLevelRate < 200 ) 
				iOption3 = 3;	// +12
			break;
		}	
#else
		// 기본값은 +0 : 0
		iOption3 = 0;
		switch( iOpt3TypeLevel )
		{
		case 1:
			if( iOpt3TypeLevelRate < 120 ) 
				iOption3 = 1;	// +4
			break;
		case 2:
			if( iOpt3TypeLevelRate < 60 ) 
				iOption3 = 2;	// +8
			break;
		case 3:
			if( iOpt3TypeLevelRate < 30 ) 
				iOption3 = 3;	// +12
			break;
		}	
#endif

		// 랜덤 옵션 iNOption : 엑설런트 옵션을 사용한다		
		int iRandOpt		= rand()%4;		// 옵션 4가지 중 하나
		int iRandOptRate	= rand()%1000;	// 옵션이 붙을 확률
		switch( iRandOpt )
		{
		case 0:	// (공격이 명중한 경우) %5 확률로 적 방어력 무시
			if( iRandOptRate < 40 )
				iNOption |= PLUS_THIRD_WING_5PER_IGNORE_DEFENSE;
			break;
		case 1:	// (공격당한 경우) 5% 확률로 적 공격력 50% 돌려줌
			if( iRandOptRate < 20 )
				iNOption |= PLUS_THIRD_WING_5PER_ATTACK_TURNING;
			break;
		case 2:	// (공격당한 경우) 5% 확률로 유저 생명 100% 순간 회복
			if( iRandOptRate < 70 )
				iNOption |= PLUS_THIRD_WING_RECOVER_100PER_LIFE;
			break;
		case 3:	// (공격당한 경우) 5% 확률로 유저 마력 100% 순간 회복
			if( iRandOptRate < 70 )
				iNOption |= PLUS_THIRD_WING_RECOVER_100PER_MASIC;
			break;
		}
		
		ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, iItemType, 0, 0, iOption1, iOption2, iOption3, -1, iNOption );				
			
		gObjInventoryCommit( lpObj->m_Index );

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD( "[ThirdWing Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney,
			   iCharmOfLuckCount );
#else
		LogAddTD( "[ThirdWing Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d", 
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney );
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			
		return;
	}
	else
	{	// 조합실패		
		ThirdWingMixFail( lpObj );		
		GCUserChaosBoxSend( lpObj );
		DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD( "[ThirdWing Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney,
			   iCharmOfLuckCount );
#else
		LogAddTD( "[ThirdWing Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, iChaosNeedMoney );
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	}

	lpObj->ChaosLock = FALSE;
}

// 3차 날개 조합 실패
void CMixSystem::ThirdWingMixFail( LPOBJECTSTRUCT lpObj )		
{
	if( lpObj->pChaosBox == NULL ) 
	{
		return;
	}

	for( int n = 0; n < MAX_CHAOSBOXITEMS; n++ )
	{		
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( Check2ndWingItem( lpObj->pChaosBox[n].m_Type ) ) 
			{	
				// 1. 날개		
				ThirdWingMixFailItemPanalty( &lpObj->pChaosBox[n] );
			}
			else if( CheckLevelCondition( &lpObj->pChaosBox[n], 0, 0, 0, 0, 1 ) )
			{	
				// 2. 셋템
				ThirdWingMixFailItemPanalty( &lpObj->pChaosBox[n] );
			}
			else if( CheckLevelCondition( &lpObj->pChaosBox[n], 0, 0, 0, 0, 0, 1 ) )
			{
				// 3. 엑설런트 아이템
				ThirdWingMixFailItemPanalty( &lpObj->pChaosBox[n] );
			}
			else
			{
				// 3. 그 밖의 모든 아이템 삭제
				lpObj->pChaosBox[n].Clear();	
			}
		}
	}	
}

// 3차 날개 조합 실패했을 때 아이템에 대한 패널티
void CMixSystem::ThirdWingMixFailItemPanalty( CItem* pItem )
{
	if( pItem == NULL )
		return;

	// 레벨 2-3 낮추기
	if( rand() % 2 < 1 )
	{
		pItem->m_Level -= 2;
	}
	else 
	{
		pItem->m_Level -= 3;
	}
	
	// 옵 초기화
	pItem->m_Option3 = 0;
			
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	pItem->Convert( pItem->m_Type, 
					pItem->m_Option1, 
					pItem->m_Option2, 
					pItem->m_Option3, 
					pItem->m_NewOption,
					pItem->m_SetOption,
					pItem->m_ItemOptionEx,
					pItem->m_SocketOption,
					pItem->m_BonusSocketOption);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
	pItem->Convert( pItem->m_Type, 
					pItem->m_Option1, 
					pItem->m_Option2, 
					pItem->m_Option3, 
					pItem->m_NewOption,
					pItem->m_SetOption,
					pItem->m_ItemOptionEx );
	#else
	pItem->Convert( pItem->m_Type, 
					pItem->m_Option1, 
					pItem->m_Option2, 
					pItem->m_Option3, 
					pItem->m_NewOption,
					pItem->m_SetOption );
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
}
#endif	// ADD_THIRD_WING_20070525

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CMixSystem::SetItemChaosMix(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	int ChoasGemCount = 0;				// 혼석 수
	int BlessGemCount = 0;				// 영석 수
	int SoulGemCount = 0;				// 영석 수	
	int	MetalOfAncientCount = 0;		// 고대의 금속 수
	int	MixItemCount = 0;
	int	MixSetItemIndex = 0;			// 세트아이템이 될 아이템의 인덱스
	int	iMixItemChaosMoney = 0;
	int	iChaosMoney = 0;
	int	iMetalOfAncientItemLevel = -1;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;		

	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 13) )
			{	// 축석
				BlessGemCount++;
				if( BlessGemCount > 3 )
					iChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;
			}
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 14) )
			{	// 영석
				SoulGemCount++;
				if( SoulGemCount > 3 )
					iChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;
			}
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 16) )
			{	// 생석				
				iChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼석
				ChoasGemCount++;				
			}
#if GAME_VERSION >= G_V_99B
			else if( (lpObj->pChaosBox[n].m_Level >= 6 && lpObj->pChaosBox[n].IsExtItem()) )
			{	// +6 이상 엑설런트 아이템
				if( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type) )
				{	// 세트 아이템으로 변경할수 있으면
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if( ((lpObj->pChaosBox[n].m_Type>=ITEM_HELPER+8 && lpObj->pChaosBox[n].m_Type<ITEM_HELPER+14) || 
					 (lpObj->pChaosBox[n].m_Type>=ITEM_HELPER+20 && lpObj->pChaosBox[n].m_Type<ITEM_HELPER+28)) && 
					  lpObj->pChaosBox[n].m_Option3 >= 2 && lpObj->pChaosBox[n].m_Level >= 3 ) //반지,목걸이는 +3이상 옵 2이상
			{
				if( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type) )
				{	// 세트 아이템으로 변경할수 있으면
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += 	lpObj->pChaosBox[n].m_BuyMoney;
				}				
			}
#endif
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14,27) )
			{	// 고대의 금속
				MetalOfAncientCount++;				
				iMetalOfAncientItemLevel = lpObj->pChaosBox[n].m_Level;
			}
			else
			{				
				DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;		
			}			
		}
	}

	if( ChoasGemCount < 1 || BlessGemCount < 3 ||  SoulGemCount < 3 || 
		MetalOfAncientCount != 1 || MixItemCount != 1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;		
		return;
	}

	LogChaosItem(lpObj, "SetItemMix");
	LogAddTD("[SetItemMix] Chaos Mix Start");	

#if GAME_VERSION >= G_V_99B
	if( !gSetItemOption.CheckMixContition(lpObj->pChaosBox[MixSetItemIndex].m_Type, 
										 iMetalOfAncientItemLevel) )
	{	// 고대의금속과 해당 아이템의 조합 조건이 맞지 않다
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif

	lpObj->ChaosSuccessRate = iMixItemChaosMoney/40000;		// 조합 될 아이템 확률
	lpObj->ChaosSuccessRate = iChaosMoney/400000;			// 보석 가격으로 아이템 확률

	if( lpObj->ChaosSuccessRate > 80 )
	{	// 최대 확률은 80%
		lpObj->ChaosSuccessRate = 80;
	}

	int nChaosNeedMoney = (lpObj->ChaosSuccessRate-50)*1000000;		// 필요 젠은 (성공확률-50)*백만젠
	if( lpObj->Money < nChaosNeedMoney )
	{	
		pMsg.Result		= 0x02;		// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

#if GAME_VERSION >= G_V_99B
	int setoption = gSetItemOption.MakeSetOption(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel);
	if( setoption == 0 )
	{	// 해당 세트 옵션을 찾을수 없다
		pMsg.Result		= 0x00;		// 실패
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif

	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);		

#if TESTSERVER == 1
	lpObj->ChaosSuccessRate = 100;
#endif

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{
#if GAME_VERSION >= G_V_99B
		lpObj->pChaosBox[MixSetItemIndex].m_SetOption = setoption;

		if( rand()%100 < 80 )
		{	// 80%가 +5 스탯 상승			
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 0x04;
		}
		else
		{	// 20%가 +10 스탯 상승			
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 0x08;
		}
#endif
		
		pMsg.Result  = 1;

		CItem NewItem;
		float levelitemdur =  (float)ItemGetDurability(lpObj->pChaosBox[MixSetItemIndex].m_Type, 
													   lpObj->pChaosBox[MixSetItemIndex].m_Level, 
												       lpObj->pChaosBox[MixSetItemIndex].IsExtItem(),
													   lpObj->pChaosBox[MixSetItemIndex].IsSetItem());

		
		NewItem.m_Level      = lpObj->pChaosBox[MixSetItemIndex].m_Level;
		NewItem.m_Durability = levelitemdur*lpObj->pChaosBox[MixSetItemIndex].m_Durability/lpObj->pChaosBox[MixSetItemIndex].m_BaseDurability;

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		// 강화옵션은 그대로 가야한다. 시리얼 발급 아님.
		NewItem.m_JewelOfHarmonyOption = lpObj->pChaosBox[MixSetItemIndex].m_JewelOfHarmonyOption;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

		
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		NewItem.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, 
			            lpObj->pChaosBox[MixSetItemIndex].m_Option1, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option2, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option3, 
						lpObj->pChaosBox[MixSetItemIndex].m_NewOption,
						lpObj->pChaosBox[MixSetItemIndex].m_SetOption,
						lpObj->pChaosBox[MixSetItemIndex].m_ItemOptionEx,
						lpObj->pChaosBox[MixSetItemIndex].m_SocketOption,
						lpObj->pChaosBox[MixSetItemIndex].m_BonusSocketOption);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		NewItem.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, 
			            lpObj->pChaosBox[MixSetItemIndex].m_Option1, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option2, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option3, 
						lpObj->pChaosBox[MixSetItemIndex].m_NewOption,
						lpObj->pChaosBox[MixSetItemIndex].m_SetOption,
						lpObj->pChaosBox[MixSetItemIndex].m_ItemOptionEx);	
	#else
		NewItem.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, 
			            lpObj->pChaosBox[MixSetItemIndex].m_Option1, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option2, 
						lpObj->pChaosBox[MixSetItemIndex].m_Option3, 
						lpObj->pChaosBox[MixSetItemIndex].m_NewOption,
						lpObj->pChaosBox[MixSetItemIndex].m_SetOption);	
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

		
		NewItem.m_Number = lpObj->pChaosBox[MixSetItemIndex].m_Number;

		ItemByteConvert(pMsg.ItemInfo, NewItem);
		
		ChaosBoxInit(lpObj);
		gObjChaosBoxInsertItemPos(lpObj->m_Index, NewItem, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);		

		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		//GCUserChaosBoxSend(lpObj);
#if GAME_VERSION >= G_V_99B
		LogAddTD("[SetItemMix] [%s][%s] CBMix Success %d Money : %d-%d (SetName:%s)", 
			     lpObj->AccountID, lpObj->Name, 
			     lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney, 
				 gSetItemOption.GetSetOptionName(lpObj->pChaosBox[MixSetItemIndex].m_Type, setoption ));
#else
		LogAddTD("[SetItemMix] [%s][%s] CBMix Success %d Money : %d-%d", 
			lpObj->AccountID, lpObj->Name, 
			lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney);
#endif

		return;
	}
	else
	{	// 조합실패	

		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[SetItemMix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   lpObj->AccountID, lpObj->Name, 
			   lpObj->ChaosSuccessRate, lpObj->Money, nChaosNeedMoney);	
	}	

	lpObj->ChaosLock = FALSE;	
}

// 카오스 이벤트 아이템을 요청한다.
void CMixSystem::CBUPS_ItemRequest(int aIndex, char *AccountId, char *Name)
{
	LogAddTD("[DevilSquare] 이벤트 경품 요청 [%s][%s]", AccountId, Name);

	CB_PMSG_REQ_PRESENT	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x01;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.Sequence	= aIndex;
	memcpy(pMsg.AccountId, AccountId, MAX_IDSTRING);
	memcpy(pMsg.GameId   , Name     , MAX_IDSTRING);

	gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

void ChaosEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case 0x01 :
	//	CBUPR_ItemRequestRecv((LPCB_PMSG_REQ_PRESENT_RESULT)aRecv);
		break;
	}
}

/*
#define DEVILSQUARE_EVENTMONEY	10000
// 카오스 이벤트 아이템을 요청 결과
void CMixSystem::CBUPR_ItemRequestRecv(LPCB_PMSG_REQ_PRESENT_RESULT lpMsg)
{
	LPOBJECTSTRUCT	lpObj;
	char			szId[MAX_IDSTRING+1];
	char			szName[MAX_IDSTRING+1];
	char			szPresentName[MAX_PRESENTNAME+1];

	int				aIndex;
	PMSG_PRIZE_INFO	pMsg;
	
	szId[MAX_IDSTRING] = '\0';
	szName[MAX_IDSTRING] = '\0';
	memset(szPresentName, 0, MAX_PRESENTNAME+1);

	memcpy(szId,   lpMsg->AccountId, MAX_IDSTRING);
	memcpy(szName, lpMsg->GameId   , MAX_IDSTRING);
	memcpy(szPresentName, lpMsg->presentname, MAX_PRESENTNAME);
	aIndex = lpMsg->Sequence;

	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( strcmp(szId, lpObj->AccountID) != 0) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d][%s == %s]", __FILE__,__LINE__, szId, lpObj->AccountID);
		return;
	}
	if( strcmp(szName, lpObj->Name) != 0) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	
	
	if( lpMsg->present != -1 ) 
	{
		char szTemp[256]="";
		wsprintf(szTemp, lMsg.Get(1601), szName, szPresentName);	
		AllSendServerMsg(szTemp);

		pMsg.h.c		= PMHC_BYTE;
		pMsg.h.headcode	= 0x02;
		pMsg.h.size		= sizeof( pMsg );
		pMsg.present	= lpMsg->present;
		pMsg.Seq_present= lpMsg->Seq_present;
		pMsg.servernumber=gGameServerCode;
		memcpy(pMsg.AccountID, lpMsg->AccountId, MAX_IDSTRING);
		memcpy(pMsg.GameID,    lpMsg->GameId, MAX_IDSTRING);

		gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[DevilSquare] Present Prize Notice %d(%s) %d %s %s", lpMsg->present, szPresentName, lpMsg->Seq_present, szId, szName);

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, 511, 0, 255, 0, 0, 0);
		lpObj->Money -= DEVILSQUARE_EVENTMONEY;
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		PMSG_CHAOSMIXRESULT	pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
		pMsg.Result		= 0x05;			// 경품이 당첨되었다
		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}	
	else
	{
		int itemtype  = MAKE_ITEMNUM(14,19);
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, itemtype, 0, 0, 0, 0, 0);
		lpObj->Money -= DEVILSQUARE_EVENTMONEY;
		GCMoneySend(lpObj->m_Index, lpObj->Money);
		lpObj->ChaosLock = FALSE;
		LogAddTD("[DevilSquare] [%s][%s] CBMix Success %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney );
	}
}
//*/

#ifdef MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
void CMixSystem::ChaosCardMix(LPOBJECTSTRUCT lpObj)
{
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	int iLotteryItemCount = 0;
	int iInvalidItemCount = 0;

	int iItemHeight = 0;
	int iItemWidth = 0;

#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	int iChaosCardType = 0;
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

	// 1. 카오스조합창 아이템 검사
	for( int n=0; n<MAX_CHAOSBOXITEMS; n++)
	{
		if( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			// 복권 아이템
			if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM(14, 54) )
			{
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
				iChaosCardType = 1;
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
				iLotteryItemCount++;
			}
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 92 ) )					// 카오스카드 골드
			{
				iChaosCardType = 2;
				iLotteryItemCount++;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 93 ) )					// 카오스카드 레어
			{
				iChaosCardType = 3;
				iLotteryItemCount++;
			}
			else if( lpObj->pChaosBox[n].m_Type == MAKE_ITEMNUM( 14, 95 ) )					// 카오스카드 미니
			{
				iChaosCardType = 4;
				iLotteryItemCount++;
			}
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	// 2. 복권 아이템은 복권 아이템 한장만 들어가야 한다.
	if( iInvalidItemCount > 0 || iLotteryItemCount != 1 )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	if( iChaosCardType == 0 )
	{
		// 카오스카드의 종류가 정확하지 않다.
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

	LogChaosItem( lpObj, "CashLottery");

	// 3. 아이템 크기를 확인
	//g_CashLotterySystem.GetRequireInvenSize( &iItemHeight, &iItemWidth );

	iItemHeight = MAX_LOTTERY_ITEM_HEIGHT;
	iItemWidth = MAX_LOTTERY_ITEM_WIDTH;

	// 4. 아이템이 들어갈 만한 공간이 있는지 체크
	if( CheckInventoryEmptySpace( lpObj, iItemHeight, iItemWidth ) == FALSE )
	{
		pMsg.Result = 0xF1;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 5. 복권에서 나올 아이템을 선택
	CItem LotteryItem;
#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326
	if( iChaosCardType == 1 )
	{
		g_EventItemBagExMng.GetItem( "ChaosCard", &LotteryItem );
	}
	else if( iChaosCardType == 2 )
	{
		g_EventItemBagExMng.GetItem( "ChaosCardGold", &LotteryItem );
	}
	else if( iChaosCardType == 3 )
	{
		g_EventItemBagExMng.GetItem( "ChaosCardRare", &LotteryItem );
	}
	else if( iChaosCardType == 4 )
	{
		g_EventItemBagExMng.GetItem( "ChaosCardMini", &LotteryItem );
	}
	
	if( LotteryItem.IsItem() == FALSE )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		LogAddTD("[ChaosMix][LotteryItemMix] Lottery Item Mix Failed. Can't Get Item from List.");
		return;
	}
#else
	if( g_CashLotterySystem.GetItem( &LotteryItem ) == -1 )
	{
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		LogAddTD("[ChaosMix][LotteryItemMix] Lottery Item Mix Failed. Can't Get Item from List.");
		return;
	}
#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

	ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, LotteryItem.m_Type, LotteryItem.m_Level, LotteryItem.m_Durability, LotteryItem.m_Option1, LotteryItem.m_Option2, LotteryItem.m_Option3, lpObj->m_Index, LotteryItem.m_NewOption, 0 );
	gObjInventoryCommit( lpObj->m_Index );
	LogAddTD("[ChaosMix][ChaosCardMix] Chaos Card Item Mix Success [%s][%s]",
		lpObj->AccountID, lpObj->Name);

	BYTE btExOption[6];
	ItemIsBufExOption( btExOption, &LotteryItem );

	LogAddTD("[CashShop][ChaosCardMix] - User(ID:%s,Name:%s) Item(Name:%s,Type:%d,Level:%d,Dur:%d,Skill:%d,Luck:%d,AddOption:%d,Ex:(%d,%d,%d,%d,%d,%d))",
		lpObj->AccountID,
		lpObj->Name,
		ItemAttribute[LotteryItem.m_Type].Name,
		LotteryItem.m_Type,
		LotteryItem.m_Level,
		(int)LotteryItem.m_Durability,
		LotteryItem.m_Option1,
		LotteryItem.m_Option2,
		LotteryItem.m_Option3,
		btExOption[0],
		btExOption[1],
		btExOption[2],
		btExOption[3],
		btExOption[4],
		btExOption[5] );
}
#endif // MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
#ifndef MAX_LOTTERY_ITEM_HEIGHT
#define	MAX_LOTTERY_ITEM_HEIGHT		4
#endif
#ifndef MAX_LOTTERY_ITEM_WIDTH
#define MAX_LOTTERY_ITEM_WIDTH		2
#endif

#include "Event.h"

typedef BOOL (*LPFUNC_EVENTITEMBOXOPEN)( LPOBJECTSTRUCT, BYTE, UCHAR, UCHAR, int*);
typedef struct _tagMIX_CHERRYBLOSSOM {
	int		nItemNumber;
	BYTE	bValid;
	BYTE	nCount;

	BYTE	nCherryBlossomCount;
	LPFUNC_EVENTITEMBOXOPEN	pEventItemBoxOpen;
} MIX_CHERRYBLOSSOM, *LPMIX_CHERRYBLOSSOM;

void CMixSystem::CherryBlossomMix(LPOBJECTSTRUCT lpObj)
{
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;	

	lpObj->ChaosLock = TRUE;

	MIX_CHERRYBLOSSOM MixCB = {0};
	for( int n=0; n<MAX_CHAOSBOXITEMS; ++n)
	{
		if( lpObj->pChaosBox[n].IsItem() == FALSE )
			continue;

		if ( MixCB.nItemNumber == 0) 
		{
			MixCB.nItemNumber = lpObj->pChaosBox[n].m_Type;
			if ( MixCB.nItemNumber == MAKE_ITEMNUM(14, 88) ) {
				MixCB.nCount = MAX_OVERLAP_CHERRYBLOSSOM_WHITE;
				MixCB.pEventItemBoxOpen = CherryBlossomEventItemBoxAOpen;
			}
			else if ( MixCB.nItemNumber == MAKE_ITEMNUM(14, 89) ) {
				MixCB.nCount = MAX_OVERLAP_CHERRYBLOSSOM_RED;
				MixCB.pEventItemBoxOpen = CherryBlossomEventItemBoxBOpen;
			}
			else if ( MixCB.nItemNumber == MAKE_ITEMNUM(14, 90) ) {
				MixCB.nCount = MAX_OVERLAP_CHERRYBLOSSOM_GOLD;
				MixCB.pEventItemBoxOpen = CherryBlossomEventItemBoxCOpen;
			} else {
				MixCB.nItemNumber = 0;
				continue;
			}

			MixCB.bValid = TRUE;
		}

		if (MixCB.nItemNumber == lpObj->pChaosBox[n].m_Type) {
			MixCB.nCherryBlossomCount += lpObj->pChaosBox[n].m_Durability;
		} else {
			MixCB.bValid = FALSE;
			break;
		}
	}

	// 1. 벚꽃조합 아이템 검사
	if (MixCB.nCherryBlossomCount != MixCB.nCount)
		MixCB.bValid = FALSE;

	if (!MixCB.nItemNumber || !MixCB.bValid)
	{
		pMsg.Result = 0x00;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 3. 아이템 크기를 확인
	int iItemHeight = MAX_LOTTERY_ITEM_HEIGHT;
	int iItemWidth = MAX_LOTTERY_ITEM_WIDTH;
	
	// 4. 아이템이 들어갈 만한 공간이 있는지 체크
	if ( CheckInventoryEmptySpace_( lpObj, iItemHeight, iItemWidth ) == FALSE )
	{
		pMsg.Result = 0xF1;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (MixCB.pEventItemBoxOpen == NULL)
	{
		pMsg.Result = 0xF1;
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 5. 나올 아이템을 선택
	int nMoney = 0;
	BOOL bResult = MixCB.pEventItemBoxOpen(lpObj, 255, 0, 0, &nMoney);
	if( bResult == FALSE )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[%s][%s] CherryBlossomMix Fail %d Money : %d-%d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->Money, lpObj->ChaosMoney );		
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
		LogAddTD("[%s][%s] CherryBlossomMix Success Rate:%d Money : %d", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->ChaosMoney);

		if (nMoney > 0) {
			if( lpObj->Money + nMoney < MAX_ZEN )
			{
				//벚꽃가지조합으로 젠이 생성된 경우
				lpObj->Money += nMoney;
				GCMoneySend(lpObj->m_Index, lpObj->Money);

				memset(pMsg.ItemInfo, 0xFF, ITEM_BUFFER_SIZE);
				pMsg.Result = 0x01;
				ChaosBoxInit(lpObj);
				GCUserChaosBoxSend(lpObj);
				DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );

#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
				gObjInventoryCommit(lpObj->m_Index);
				return;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
			}
			else
			{
				pMsg.Result = 0xF1;
				DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );

				LogAddTD("[%s] Overflow Zen [%s][%s] [%d/%d User Zen/Zen]", "CherryBlossomMix", lpObj->AccountID, lpObj->Name, lpObj->Money, nMoney );
			}
		}
		lpObj->ChaosLock = FALSE;
	}
	gObjInventoryCommit(lpObj->m_Index);
}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
//////////////////////////////////////////////////////////////////////////
// 1. 소켓 아이템 조합
// 1-1. 시드를 아이템으로부터 추출
void CMixSystem::MixExtractSeed(LPOBJECTSTRUCT lpObj)
{
	int iExtItemCount = 0;
	int iSetItemCount = 0;
	int iHarmonyJewelCount = 0;
	int iChaosJewelCount = 0;
	int iCreateJewelCount = 0;
	int iInvalidItemCount = 0;
	int iSuccessRate = 0;
	DWORD dwMixMoney = 0;
	int iMixNeedMoney = 0;
	int iChaosTaxMoney = 0;

#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
	int iSetItemPosition = 0;
	int iExtItemPosition = 0;
	int iHarmonyJewelPosition = 0;
	int iChaosJewelPosition = 0;
	int iCreationJewelPosition = 0;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	lpObj->ChaosLock = TRUE;

	// 시드 추출을 위한 재료 아이템 확인
	// 시드 추출에 필요한 아이템은 엑설런트(4레벨이상), 세트아이템(4레벨이상), 조화의보석, 혼돈의보석, 창조의보석
	for( int i = 0 ; i < MAX_CHAOSBOXITEMS ; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() == TRUE )
		{
			// 엑설런트 아이템 확인 (4레벨 이상)
			if( lpObj->pChaosBox[i].IsExtItem() == TRUE && lpObj->pChaosBox[i].m_Level >= 4 )
			{
				iExtItemCount++;
				dwMixMoney += lpObj->pChaosBox[i].m_BuyMoney;
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
				iExtItemPosition = i;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			}
			// 세트 아이템 확인 (4레벨 이상)
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
			else if( lpObj->pChaosBox[i].IsSetItem() != 0 && lpObj->pChaosBox[i].m_Level >= 4 )
#else
			else if( lpObj->pChaosBox[i].IsSetItem() == TRUE && lpObj->pChaosBox[i].m_Level >= 4 )
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
			{
				iSetItemCount++;
				dwMixMoney += lpObj->pChaosBox[i].m_BuyMoney;
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
				iSetItemPosition = i;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			}
			// 조화의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 42 ) )
			{
				iHarmonyJewelCount++;
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
				iHarmonyJewelPosition = i;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			}
			// 혼돈의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 12, 15 ) )
			{
				iChaosJewelCount++;
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
				iChaosJewelPosition = i;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			}
			// 창조의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 22 ) )
			{
				iCreateJewelCount++;
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
				iCreationJewelPosition = i;
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	// 확인된 아이템 개수를 비교해서 요구 아이템 에 안맞는 경우 취소
	if( iInvalidItemCount > 0 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	if( iExtItemCount != 1 && iSetItemCount != 1 && iHarmonyJewelCount != 1 && iChaosJewelCount != 1 && iCreateJewelCount != 1 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosTaxMoney = (INT)((INT64)g_RequireMoneyForSeedExtract * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMixNeedMoney = g_RequireMoneyForSeedExtract + iChaosTaxMoney;

	// 추출시 요구 젠 확인
	if( lpObj->Money < iMixNeedMoney )
	{
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}
	
#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogChaosItem( lpObj, "SeedExtract");
#endif // ADD_SOCKET_OPTION_LOG_20080715

	lpObj->Money -= iMixNeedMoney;
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );

	GCMoneySend( lpObj->m_Index, lpObj->Money );

	// 성공 확률 확인
	iSuccessRate = 80 + ( dwMixMoney / 2000000 );
	if( iSuccessRate > MAX_SEED_EXTRACT_SUCCESS_RATE )
	{
		iSuccessRate = MAX_SEED_EXTRACT_SUCCESS_RATE;
	}

	if( rand() % 100 > iSuccessRate )
	{
		// 실패
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
		lpObj->pChaosBox[iSetItemPosition].m_Level -= rand() % 3;
		lpObj->pChaosBox[iExtItemPosition].m_Level -= rand() % 3;

		lpObj->pChaosBox[iHarmonyJewelPosition].Clear();
		lpObj->pChaosBox[iChaosJewelPosition].Clear();
		lpObj->pChaosBox[iCreationJewelPosition].Clear();

		GCUserChaosBoxSend( lpObj );
		DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );

		lpObj->ChaosLock = FALSE;
#else
		lpObj->ChaosLock = FALSE;
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
#ifdef ADD_SOCKET_OPTION_LOG_20080715
		LogAddTD( "[%s][%s] Seed Extract Mix Failed.", lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715
		return;
	}
	else
	{
		// 시드 아이템 생성 확률에 따라 시드를 생성
		BYTE btSeedIndex = 0;
		CSeedItem* lpSeedData;

		btSeedIndex = g_SocketOptionSystem.GetExtractSeed();
		lpSeedData = g_SocketOptionSystem.GetSeedData( btSeedIndex );
		if( lpSeedData != NULL )
		{
			// 시드의 인덱스로 해당 시드의 데이터로 아이템 데이터를 만든다.
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			pMsg.Result = 0x01;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, lpSeedData->m_ItemCode, lpSeedData->m_ItemLevel, 1, 0, 0, 0, lpObj->m_Index, 0, 0 );
			gObjInventoryCommit( lpObj->m_Index );
#ifdef ADD_SOCKET_OPTION_LOG_20080715
			LogAddTD( "[%s][%s] Seed Extract Mix Success - SeedInfo ( Index : %d, Type : %d, Level : %d, ItemCode : %d )",
				lpObj->AccountID, lpObj->Name,
				lpSeedData->m_SeedIndex, lpSeedData->m_SeedType, lpSeedData->m_ItemLevel, lpSeedData->m_ItemCode );
#endif // ADD_SOCKET_OPTION_LOG_20080715

#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
			return;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
		}
		else
		{
#ifdef MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
			lpObj->pChaosBox[iSetItemPosition].m_Level -= rand() % 3;
			lpObj->pChaosBox[iExtItemPosition].m_Level -= rand() % 3;

			lpObj->pChaosBox[iHarmonyJewelPosition].Clear();
			lpObj->pChaosBox[iChaosJewelPosition].Clear();
			lpObj->pChaosBox[iCreationJewelPosition].Clear();

			GCUserChaosBoxSend( lpObj );
#else
			ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj);
#endif // MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903
#ifdef ADD_SOCKET_OPTION_LOG_20080715
			LogAddTD( "[%s][%s] Seed Extract Mix Failed - SeedData is NULL",
				lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715
		}
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
		lpObj->ChaosLock = FALSE;
	#ifndef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722					// !! NOT
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	}

#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
	lpObj->ChaosLock = FALSE;

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
}
// 1-2. 시드와 스피어 합성
void CMixSystem::MixCompositeSeedSphere(LPOBJECTSTRUCT lpObj)
{
	int iSeedItemCount = 0;
	int iSphereItemCount = 0;
	int iInvalidItemCount = 0;
	int iChaosJewelCount = 0;
	int iCreateJewelCount = 0;
	int iChaosTaxMoney = 0;
	int iNeedMoney = 0;
	DWORD dwMixMoney = 0;
	int iSuccessRate = 0;
	CItem* lpSeedItem = NULL;
	CItem* lpSphereItem = NULL;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	lpObj->ChaosLock = TRUE;

	for( int i = 0 ; i < MAX_CHAOSBOXITEMS ; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() == TRUE )
		{
			if( g_SocketOptionSystem.IsSeedItem( lpObj->pChaosBox[i].m_Type ) == true )
			{
				iSeedItemCount++;
				dwMixMoney += lpObj->pChaosBox[i].m_BuyMoney;
				lpSeedItem = &lpObj->pChaosBox[i];
			}
			else if( g_SocketOptionSystem.IsSphereItem( lpObj->pChaosBox[i].m_Type ) == true )
			{
				iSphereItemCount++;
				dwMixMoney += lpObj->pChaosBox[i].m_BuyMoney;
				lpSphereItem = &lpObj->pChaosBox[i];
			}
			// 혼돈의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 12, 15 ) )
			{
				iChaosJewelCount++;
			}
			// 창조의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 22 ) )
			{
				iCreateJewelCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iInvalidItemCount > 0 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	if( iSeedItemCount != 1 && iSphereItemCount != 1 && iChaosJewelCount != 1 && iCreateJewelCount != 1 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 카오스 조합시 공성 세금을 확인
	iChaosTaxMoney = (INT)((INT64)g_RequireMoneyForSeedSphereComposite * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iNeedMoney = g_RequireMoneyForSeedSphereComposite + iChaosTaxMoney;

	// 추출시 요구 젠 확인
	if( lpObj->Money < iNeedMoney )
	{
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogChaosItem( lpObj, "SeedSphereComposite");
#endif // ADD_SOCKET_OPTION_LOG_20080715

	iSuccessRate = 80 + ( dwMixMoney / 200000 );
	if( iSuccessRate > MAX_SEEDSPHERE_COMPOSITE_SUCCESS_RATE )
	{
		iSuccessRate = MAX_SEEDSPHERE_COMPOSITE_SUCCESS_RATE;
	}

	lpObj->Money -= iNeedMoney;
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );

	GCMoneySend( lpObj->m_Index, lpObj->Money );

	if( rand() % 100 > iSuccessRate )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj);

#ifdef ADD_SOCKET_OPTION_LOG_20080715
		LogAddTD( "[%s][%s] SeedSphere Composite Mix Failed.",
			lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715

#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710				// !! NOT
		DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
		return;
	#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	}
	else
	{
		BYTE btSeedIndex = 0;
		BYTE btSphereLevel = 0;
		CSeedSphereItem SeedSphereItem;

		btSeedIndex = g_SocketOptionSystem.GetSeedIndex( lpSeedItem->m_Type, lpSeedItem->m_Level );
		btSphereLevel = g_SocketOptionSystem.GetSphereLevel( lpSphereItem->m_Type );
		// 생성될 시드스피어를 확인
		g_SocketOptionSystem.MakeSeedSphere( &SeedSphereItem, btSeedIndex, btSphereLevel );
		if( SeedSphereItem.m_ItemCode != 0 )
		{
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			pMsg.Result = 0x01;
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704
			ItemSerialCreateSend( lpObj->m_Index, 255, 0, 0, SeedSphereItem.m_ItemCode, SeedSphereItem.m_ItemLevel, 1, 0, 0, 0, lpObj->m_Index, 0, 0 );
			gObjInventoryCommit( lpObj->m_Index );
#ifdef ADD_SOCKET_OPTION_LOG_20080715
			LogAddTD( "[%s][%s] SeedSphere Composite Mix Success - SeedSphere Info ( Index : %d, Type : %d, Level : %d, ItemCode : %d )",
				lpObj->AccountID, lpObj->Name,
				SeedSphereItem.m_SeedSphereIndex, SeedSphereItem.m_SeedType, SeedSphereItem.m_SphereLevel, SeedSphereItem.m_ItemCode );
#endif // ADD_SOCKET_OPTION_LOG_20080715

#ifdef MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
			return;
#endif	// MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211
		}
		else
		{
			ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj);
#ifdef ADD_SOCKET_OPTION_LOG_20080715
			LogAddTD( "[%s][%s] SeedSphere Composite Mix Failed - SeedSphere Data is NULL",
				lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715
		}
	}

	lpObj->ChaosLock = FALSE;

#ifndef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715				// !! NOT
	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
}
// 1-3. 시드스피어를 소켓 아이템에 적용
void CMixSystem::MixSetSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex)
{
	int iSocketItemCount = 0;
	int iInvalidItemCount = 0;
	int iSeedSphereItemCount = 0;
	int iChaosJewelCount = 0;
	int iCreateJewelCount = 0;
	CItem* lpSocketItem = NULL;
	CItem* lpSeedSphereItem = NULL;
	CItem NewItem;
	int iChaosTaxMoney = 0;
	int iNeedMoney = 0;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	if( btSocketSlotIndex >= MAX_SOCKET_SLOT )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->ChaosLock = TRUE;

	for( int i = 0 ; i < MAX_CHAOSBOXITEMS ; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() == TRUE )
		{
			// 소켓 아이템이 맞는지 확인
			if( g_SocketOptionSystem.IsEnableSocketItem( lpObj->pChaosBox[i].m_Type ) == true )
			{
				// 해당 아이템의 소켓 슬롯이 비어있는지 확인
				if( lpObj->pChaosBox[i].m_SocketOption[btSocketSlotIndex] == SOCKETSLOT_EMPTY )
				{
					iSocketItemCount++;
					lpSocketItem = &lpObj->pChaosBox[i];
				}
			}
			// 시드스피어를 확인
			else if( g_SocketOptionSystem.IsSeedSphereItem( lpObj->pChaosBox[i].m_Type ) == true )
			{
				iSeedSphereItemCount++;
				lpSeedSphereItem = &lpObj->pChaosBox[i];
			}
			// 혼돈의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 12, 15 ) )
			{
				iChaosJewelCount++;
			}
			// 창조의 보석 확인
			else if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 22 ) )
			{
				iCreateJewelCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iInvalidItemCount > 0 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	if( iSocketItemCount != 1 && iSeedSphereItemCount != 1 && iChaosJewelCount != 1 && iCreateJewelCount != 1 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 동일한 속성에 같은 옵션이 있는지 확인한다.
	if( g_SocketOptionSystem.CheckItemForSameSeedType( lpSocketItem, lpSeedSphereItem->m_Type, lpSeedSphereItem->m_Level ) == false )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 카오스 조합시 공성 세금을 확인
	iChaosTaxMoney = (INT)((INT64)g_RequireMoneyForSeedSphereSet * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iNeedMoney = g_RequireMoneyForSeedSphereSet + iChaosTaxMoney;

	if( lpObj->Money < iNeedMoney )
	{
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

#ifndef MODIFY_SOCKET_OPTION_SYSTEM_20080725				// !! NOT
	lpObj->Money -= iNeedMoney;
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );

	GCMoneySend( lpObj->m_Index, lpObj->Money );
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogChaosItem( lpObj, "SetSeedSphere");
#endif // ADD_SOCKET_OPTION_LOG_20080715

	BYTE btSeedSphereIndex = 0;

	btSeedSphereIndex = g_SocketOptionSystem.GetSeedSphereIndexForItemCode( lpSeedSphereItem->m_Type, lpSeedSphereItem->m_Level );
	if( btSeedSphereIndex == SOCKETSLOT_NONE )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
#ifdef ADD_SOCKET_OPTION_LOG_20080715
		LogAddTD( "[%s][%s] Set Seed Mix Failed - SeedSphere is NULL",
			lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715
		return;
	}

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	if( g_SocketOptionSystem.CheckSetSeedSphereItemType( lpSocketItem, btSeedSphereIndex ) == false )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
#ifdef ADD_SOCKET_OPTION_LOG_20080715
		LogAddTD( "[%s][%s] Set Seed Mix Failed - Wrong Item Type for Set Socket",
			lpObj->AccountID, lpObj->Name );
#endif // ADD_SOCKET_OPTION_LOG_20080715
		return;
	}
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

#ifdef MODIFY_SOCKET_OPTION_SYSTEM_20080725
	lpObj->Money -= iNeedMoney;
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );

	GCMoneySend( lpObj->m_Index, lpObj->Money );
#endif // MODIFY_SOCKET_OPTION_SYSTEM_20080725

	g_SocketOptionSystem.SetSocketSlot( lpSocketItem, btSeedSphereIndex, btSocketSlotIndex );

	float levelitemdur =  (float)ItemGetDurability( lpSocketItem->m_Type, 
													lpSocketItem->m_Level, 
												    lpSocketItem->IsExtItem(),
													lpSocketItem->IsSetItem());

	NewItem.m_Number		= lpSocketItem->m_Number;
	NewItem.m_Level			= lpSocketItem->m_Level;
	NewItem.m_Durability	= levelitemdur * lpSocketItem->m_Durability / lpSocketItem->m_BaseDurability;
	NewItem.m_JewelOfHarmonyOption = lpSocketItem->m_JewelOfHarmonyOption;

	NewItem.Convert( lpSocketItem->m_Type,
					 lpSocketItem->m_Option1,
					 lpSocketItem->m_Option2,
					 lpSocketItem->m_Option3,
					 lpSocketItem->m_NewOption,
					 lpSocketItem->m_SetOption,
					 lpSocketItem->m_ItemOptionEx,
					 lpSocketItem->m_SocketOption,
					 lpSocketItem->m_BonusSocketOption );

	lpObj->ChaosLock = FALSE;

	ItemByteConvert(pMsg.ItemInfo, NewItem);

	ChaosBoxInit( lpObj );
	gObjChaosBoxInsertItemPos( lpObj->m_Index, NewItem, 0, -1 );
#ifdef MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_20080910
	gObjChaosItemSet(lpObj->m_Index, 0, 1);
#else
	gObjInventoryCommit( lpObj->m_Index );
#endif // MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_20080910
	GCUserChaosBoxSend(lpObj);

#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
	LogAddTD( "[%s][%s] Set SeedSphere Mix Success - ItemInfo ( Name : %s, ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name,
		ItemAttribute[NewItem.m_Type].Name, NewItem.m_Type, NewItem.m_Level, NewItem.m_SocketOption[0], NewItem.m_SocketOption[1], NewItem.m_SocketOption[2], NewItem.m_SocketOption[3], NewItem.m_SocketOption[4], NewItem.m_BonusSocketOption );
#else
	#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogAddTD( "[%s][%s] Set SeedSphere Mix Success - ItemInfo ( ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name,
		NewItem.m_Type, NewItem.m_Level, NewItem.m_SocketOption[0], NewItem.m_SocketOption[1], NewItem.m_SocketOption[2], NewItem.m_SocketOption[3], NewItem.m_SocketOption[4], NewItem.m_BonusSocketOption );
	#endif // ADD_SOCKET_OPTION_LOG_20080715
#endif // ADD_SOCKET_OPTION_LOG_02_20080821

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}
// 1-4. 소켓 아이템에서 시드스피어를 삭제
void CMixSystem::MixRemoveSeedSphere(LPOBJECTSTRUCT lpObj, BYTE btSocketSlotIndex)
{
	int iSocketItemCount = 0;
	int iInvalidItemCount = 0;
	int iSeedSphereItemCount = 0;
	int iSocketItemPosition = 0;
	int iSeedSpherePosition = 0;
	int iNeedMoney = 0;
	int iChaosTaxMoney = 0;

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result	= 0x00;

	if( btSocketSlotIndex >= MAX_SOCKET_SLOT )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->ChaosLock = TRUE;

	for( int i = 0 ; i < MAX_CHAOSBOXITEMS ; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() == TRUE )
		{
			// 소켓 아이템이 맞는지 확인
			if( g_SocketOptionSystem.IsEnableSocketItem( lpObj->pChaosBox[i].m_Type ) == true )
			{
				// 해당 아이템의 소켓 슬롯이 비어있는지 확인
				if( lpObj->pChaosBox[i].m_SocketOption[btSocketSlotIndex] != SOCKETSLOT_EMPTY && lpObj->pChaosBox[i].m_SocketOption[btSocketSlotIndex] != SOCKETSLOT_NONE )
				{
					iSocketItemCount++;
					iSocketItemPosition = i;
				}
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iInvalidItemCount > 0 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	if( iSocketItemCount != 1 )
	{
		pMsg.Result = 0x07;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
		lpObj->ChaosLock = FALSE;
		return;
	}

	// 카오스 조합시 공성 세금을 확인
	iChaosTaxMoney = (INT)((INT64)g_RequireMoneyForSeedSphereRemove * (INT64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (INT64)100);
	if( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iNeedMoney = g_RequireMoneyForSeedSphereRemove + iChaosTaxMoney;

	if( lpObj->Money < iNeedMoney )
	{
		pMsg.Result		= 0x02;						// 돈이 모자르다.
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->Money -= iNeedMoney;
	g_CastleSiegeSync.AddTributeMoney( iChaosTaxMoney );

	GCMoneySend( lpObj->m_Index, lpObj->Money );

#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogChaosItem( lpObj, "SeedSphereRemove");
#endif // ADD_SOCKET_OPTION_LOG_20080715

	// 해당 소켓 슬롯의 시드스피어를 삭제한다.
	g_SocketOptionSystem.ClearSocketSlot( &lpObj->pChaosBox[iSocketItemPosition], btSocketSlotIndex );

	lpObj->ChaosLock = FALSE;

	gObjInventoryCommit( lpObj->m_Index );
	GCUserChaosBoxSend(lpObj);

#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
	LogAddTD( "[%s][%s] SeedSphere Remove Success - ItemInfo ( Name : %s, ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name,
		ItemAttribute[lpObj->pChaosBox[iSocketItemPosition].m_Type].Name,
		lpObj->pChaosBox[iSocketItemPosition].m_Type, lpObj->pChaosBox[iSocketItemPosition].m_Level,
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[0],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[1],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[2],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[3],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[4],
		lpObj->pChaosBox[iSocketItemPosition].m_BonusSocketOption );
#else
	#ifdef ADD_SOCKET_OPTION_LOG_20080715
	LogAddTD( "[%s][%s] SeedSphere Remove Success - ItemInfo ( ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name,
		lpObj->pChaosBox[iSocketItemPosition].m_Type, lpObj->pChaosBox[iSocketItemPosition].m_Level,
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[0],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[1],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[2],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[3],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[4],
		lpObj->pChaosBox[iSocketItemPosition].m_BonusSocketOption );
	#endif // ADD_SOCKET_OPTION_LOG_20080715
#endif // ADD_SOCKET_OPTION_LOG_02_20080821

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#endif	// MODIFY_MIX_SYSTEM_20070518 	