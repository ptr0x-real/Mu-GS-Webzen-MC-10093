// CashItemPeriodSystem.cpp: implementation of the CCashItemPeriodSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashItemPeriodSystem.h"

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

CCashItemPeriodSystem g_CashItemPeriodSystem;
BOOL g_bUsePeriodCheckThread = FALSE;
BOOL g_bRunningPeriodCheckThread = FALSE;

#include "wsJoinServerCli.h"
#include "DBSockMng.h"
#include "ItemAddOption.h"

#ifdef MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
#include "Common\winutil.h"
#endif // MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#include "ObjCalCharacter.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCashItemPeriodSystem::CCashItemPeriodSystem()
{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	InitializeCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
}

CCashItemPeriodSystem::~CCashItemPeriodSystem()
{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	DeleteCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
}

void CCashItemPeriodSystem::Initialize()
{
	iItemPeriodInfoCount = 0;

	for( int i = 0 ; i < MAX_ITEMPERIOD_INFO ; i++ )
	{
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		ItemPeriodInfo[i].btUsedInfo			= 0;
		ItemPeriodInfo[i].btDateInfoUseCount	= 0;
		ItemPeriodInfo[i].wUserIndex			= 0;
		ItemPeriodInfo[i].iUserGuid				= 0;
		ItemPeriodInfo[i].dwItemCheckTickCount	= 0;
		ItemPeriodInfo[i].chCharacterName[0] = '\0';
		for( int j = 0 ; j < MAX_PERIODITEM_DATEINFO ; j++ )
		{
			ItemPeriodInfo[i].DateInfo[j].btUsedInfo		= 0;
			ItemPeriodInfo[i].DateInfo[j].btEffectCategory	= 0;
			ItemPeriodInfo[i].DateInfo[j].btEffectType1		= 0;
			ItemPeriodInfo[i].DateInfo[j].btEffectType2		= 0;
			ItemPeriodInfo[i].DateInfo[j].iItemCode			= -1;
			ItemPeriodInfo[i].DateInfo[j].dwItemUsedTime		= 0;
			ItemPeriodInfo[i].DateInfo[j].dwItemLeftTime		= 0;
		}
#else
		ItemPeriodInfo[i].btUsedInfo			= 0;
		ItemPeriodInfo[i].iUserIndex			= -1;
		ItemPeriodInfo[i].iUserGuid				= -1;
		ItemPeriodInfo[i].iItemCode				= -1;
		ItemPeriodInfo[i].iItemPeriodLeftTime	= 0;
		ItemPeriodInfo[i].btEffectType1			= 0;
		ItemPeriodInfo[i].btEffectType2			= 0;
		memset( ItemPeriodInfo[i].chCharacterName,		0, MAX_IDSTRING + 1 );
		memset( ItemPeriodInfo[i].chItemPeriodBuyDate,	0, MAX_ITEMPERIOD_DATE_LENGTH );
		memset( ItemPeriodInfo[i].chItemPeriodEndDate,	0, MAX_ITEMPERIOD_DATE_LENGTH );

		ItemPeriodInfo[i].dwItemCheckTickCount	= 0;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	}

	if( g_bRunningPeriodCheckThread == FALSE )
	{
		hThreadHandle = CreateThread(NULL, 0, CCashItemPeriodSystem::PeriodCheckThread, &ItemPeriodInfo, 0, NULL);
		if ( hThreadHandle == NULL)
		{
			LogAdd("CreateThread() failed with error %d", GetLastError());
			return;
		}
#ifdef MODIFY_FAIL_CONNECT_CS_AFTER_OPTIONRELOAD_20091008
		g_bRunningPeriodCheckThread = TRUE;
		CloseHandle(hThreadHandle);
	}
#else
	}

	g_bRunningPeriodCheckThread = TRUE;
	CloseHandle(hThreadHandle);
#endif // MODIFY_FAIL_CONNECT_CS_AFTER_OPTIONRELOAD_20091008
}

DWORD WINAPI CCashItemPeriodSystem::PeriodCheckThread(LPVOID lpPeriodInfo)
{
	LPITEMPERIOD_INFO lpPeriodInfoList = (LPITEMPERIOD_INFO)lpPeriodInfo;
	int iIndex = 0;
	DWORD dwCurrentTick = 0;
	LPOBJECTSTRUCT lpObj = NULL;
	int iUseTime = 0;

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int iDateIndex = 0;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	while( true )
	{
		if( ( GetTickCount() - dwCurrentTick ) > PERIODITEM_CHECK_TIME )
		{
			dwCurrentTick = GetTickCount();

			for( iIndex = 0 ; iIndex < MAX_ITEMPERIOD_INFO ; iIndex++ )
			{
				if( lpPeriodInfoList[iIndex].btUsedInfo == 1 )
				{
					iUseTime = GetTickCount() - lpPeriodInfoList[iIndex].dwItemCheckTickCount;
					if( iUseTime > PERIOD_MINUTE )
					{
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
						lpPeriodInfoList[iIndex].dwItemCheckTickCount = GetTickCount();
						for( iDateIndex = 0 ; iDateIndex < MAX_PERIODITEM_DATEINFO ; iDateIndex++ )
						{
							if( lpPeriodInfoList[iIndex].DateInfo[iDateIndex].btUsedInfo == 1 )
							{
								lpPeriodInfoList[iIndex].DateInfo[iDateIndex].dwItemLeftTime--;
								lpPeriodInfoList[iIndex].DateInfo[iDateIndex].dwItemUsedTime++;

								if( lpPeriodInfoList[iIndex].DateInfo[iDateIndex].dwItemLeftTime <= 0 )
								{
									lpObj = &gObj[lpPeriodInfoList[iIndex].wUserIndex];

									g_CashItemPeriodSystem.GDReqPeriodItemUpdate( lpObj, iDateIndex );
									g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj, lpPeriodInfoList[iIndex].DateInfo[iDateIndex].iItemCode );
								}
							}
						}
#else
						lpPeriodInfoList[iIndex].dwItemCheckTickCount = GetTickCount();
						lpPeriodInfoList[iIndex].iItemPeriodLeftTime--;
						lpPeriodInfoList[iIndex].iItemPeriodUsedTime++;

						lpObj = &gObj[lpPeriodInfoList[iIndex].iUserIndex];

						if( lpPeriodInfoList[iIndex].iItemPeriodLeftTime <= 0 )
						{
							g_CashItemPeriodSystem.GDReqPeriodItemUpdate( lpObj );
							g_CashItemPeriodSystem.ClearPeriodItemEffect( lpObj );
						}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
					}
				}
			}
		}
		else
		{
			Sleep( 10 );
		}
	}

	return 0;
}

void CCashItemPeriodSystem::GetUserPeriodItemList(LPOBJECTSTRUCT lpObj)
{
	GDReqPeriodItemList( lpObj );
}

int CCashItemPeriodSystem::CalcItemUseLeftTime(char* pchBuyDate)
{
	int iUsedTime = 0;

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTime tBuyTime;
	CTimeSpan tTimeSpan;

	ConvertStringToTime( pchBuyDate, &tBuyTime );

	tCurrentTime - tBuyTime;

	tTimeSpan = tBuyTime - tCurrentTime;

	iUsedTime = tTimeSpan.GetTotalMinutes();

	return iUsedTime;
}

CTime CCashItemPeriodSystem::CalcItemUseEndTime(int iLeftTime)
{
	CTime tExpireTime;
	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tLeftTime( 0, 0, iLeftTime, 0 );

	tExpireTime = tCurrentTime + tLeftTime;

	return tExpireTime;
}

void CCashItemPeriodSystem::ConvertStringToTime(const char* pchDate, CTime* lpTime)
{
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;
	int iHour = 0;
	int iMinute = 0;

	int iDateLength = 0;

	char chDate[MAX_ITEMPERIOD_DATE_LENGTH];
	char* Token;
	char seps[]   = "-: ";

	if( pchDate == NULL || lpTime == NULL )
	{
		return;
	}

	memcpy( chDate, pchDate, MAX_ITEMPERIOD_DATE_LENGTH );

	iDateLength = strlen( chDate );
	if( iDateLength <= 0 || iDateLength > MAX_ITEMPERIOD_DATE_LENGTH )
	{
		return;
	}

	Token = strtok( chDate, seps );
	iYear = atoi( Token );
	Token = strtok( NULL, seps );
	iMonth = atoi( Token );
	Token = strtok( NULL, seps );
	iDay = atoi( Token );
	Token = strtok( NULL, seps );
	iHour = atoi( Token );
	Token = strtok( NULL, seps );
	iMinute = atoi( Token );

	CTime tReturnTime( iYear, iMonth, iDay, iHour, iMinute, 0 );

	*lpTime = tReturnTime;
}

void CCashItemPeriodSystem::ConvertTimeToString(const CTime* lpTime, char* pchDate)
{
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;
	int iHour = 0;
	int iMinute = 0;

	char chDate[MAX_ITEMPERIOD_DATE_LENGTH];

	iYear	= lpTime->GetYear();
	iMonth	= lpTime->GetMonth();
	iDay	= lpTime->GetDay();
	iHour	= lpTime->GetHour();
	iMinute	= lpTime->GetMinute();

	wsprintf( chDate, "%d-%d-%d %d:%d", iYear, iMonth, iDay, iHour, iMinute );

	memcpy( pchDate, chDate, MAX_ITEMPERIOD_DATE_LENGTH );
}

BOOL CCashItemPeriodSystem::SetPeriodItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode, BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod)
{
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int iPeriodInfoIndex = -1;
	int iItemEffectCategory = 0;
	int iItemInfoIndex = 0;
	int iDateInfoIndex = 0;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	EnterCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619

	iItemEffectCategory = g_ItemAddOption.GetItemEffectOption( iItemCode );
	if( iItemEffectCategory == -1 )
	{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		return FALSE;
	}

	// 기존에 사용중인 아이템 효과를 삭제한다.
	CheckItemUsedInfo( lpObj, iItemCode );
	//if( CheckItemUsedInfo( lpObj, iItemCode ) == false )
	//{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	//	LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	//	return FALSE;
	//}

	if( lpObj->m_iPeriodItemEffectIndex != -1 )		// 이미 적용된 효과가 있는 경우
	{
		iPeriodInfoIndex = lpObj->m_iPeriodItemEffectIndex;

		ItemPeriodInfo[iPeriodInfoIndex].btDateInfoUseCount++;

		for( iDateInfoIndex = 0 ; iDateInfoIndex < MAX_PERIODITEM_DATEINFO ; iDateInfoIndex++ )
		{
			if( ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].btUsedInfo == 0 )
			{
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].btUsedInfo		= 1;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].iItemCode			= iItemCode;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].btEffectType1		= btEffectType1;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].btEffectType2		= btEffectType2;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].btEffectCategory	= iItemEffectCategory;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].dwItemLeftTime	= dwItemPeriod;
				ItemPeriodInfo[iPeriodInfoIndex].DateInfo[iDateInfoIndex].dwItemUsedTime	= 0;

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				SetPeriodItemBuffEffect(lpObj, iItemCode, dwItemPeriod);
#else
				if( g_ItemAddOption.SetItemEffect( lpObj, iItemCode, dwItemPeriod ) == true )
				{
					// 뷰포트에 효과 삭제
#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
					//ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
					//GCStateInfoSend( lpObj, 1, STATE_MARK_USE );
				}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
				LeaveCriticalSection( &csPeriodItemSystem );
	#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
				return TRUE;
			}
		}
	}
	else	// 기존에 적용된 효과가 없는 경우
	{
		for( iItemInfoIndex = 0 ; iItemInfoIndex < MAX_ITEMPERIOD_INFO ; iItemInfoIndex++ )
		{
			if( ItemPeriodInfo[iItemInfoIndex].btUsedInfo == 0 )
			{
				lpObj->m_iPeriodItemEffectIndex = iItemInfoIndex;

				ItemPeriodInfo[iItemInfoIndex].btUsedInfo					= 1;
				ItemPeriodInfo[iItemInfoIndex].iUserGuid					= lpObj->DBNumber;
				ItemPeriodInfo[iItemInfoIndex].wUserIndex					= lpObj->m_Index;
				ItemPeriodInfo[iItemInfoIndex].dwItemCheckTickCount			= GetTickCount();
				ItemPeriodInfo[iItemInfoIndex].btDateInfoUseCount			= 1;
				memcpy( ItemPeriodInfo[iItemInfoIndex].chCharacterName, lpObj->Name, MAX_IDSTRING + 1 );

				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].btUsedInfo		= 1;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].iItemCode		= iItemCode;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].btEffectType1	= btEffectType1;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].btEffectType2	= btEffectType2;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].btEffectCategory = iItemEffectCategory;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].dwItemLeftTime	= dwItemPeriod;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[0].dwItemUsedTime	= 0;

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				SetPeriodItemBuffEffect(lpObj, iItemCode, dwItemPeriod);
#else
				if( g_ItemAddOption.SetItemEffect( lpObj, iItemCode, dwItemPeriod ) == true )
				{
					// 뷰포트에 효과 삭제
	#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
					//ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
	#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
					//GCStateInfoSend( lpObj, 1, STATE_MARK_USE );
				}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
				LeaveCriticalSection( &csPeriodItemSystem );
	#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
				return TRUE;
			}
		}
	}
#else
	if( lpObj->m_iPeriodItemEffectIndex != -1 )
	{
		if( ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex].iUserIndex == lpObj->m_Index )
		{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
			LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
			return FALSE;
		}
	}

	for( int i = 0 ; i < MAX_ITEMPERIOD_INFO ; i++ )
	{
		if( ItemPeriodInfo[i].btUsedInfo == 0 )
		{
			ItemPeriodInfo[i].btUsedInfo			= 1;
			ItemPeriodInfo[i].iUserIndex			= lpObj->m_Index;
			ItemPeriodInfo[i].iUserGuid				= lpObj->DBNumber;
			ItemPeriodInfo[i].iItemCode				= iItemCode;
			ItemPeriodInfo[i].btEffectType1			= btEffectType1;
			ItemPeriodInfo[i].btEffectType2			= btEffectType2;
			ItemPeriodInfo[i].iItemPeriodLeftTime	= dwItemPeriod;
			ItemPeriodInfo[i].iItemPeriodUsedTime	= 0;
			ItemPeriodInfo[i].dwItemCheckTickCount	= GetTickCount();

			lpObj->m_iPeriodItemEffectIndex		= i;

			if( g_ItemAddOption.SetItemEffect( lpObj, iItemCode, dwItemPeriod ) == true )
			{
				// 뷰포트에 효과 삭제
	#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
				ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
	#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
				GCStateInfoSend( lpObj, 1, STATE_MARK_USE );
			}
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
			LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
			return TRUE;
		}
	}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	return FALSE;
}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
BOOL CCashItemPeriodSystem::ClearPeriodItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode)
{
	int iItemInfoIndex = 0;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	EnterCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619

	iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;

	if( iItemInfoIndex < 0 || iItemInfoIndex > MAX_ITEMPERIOD_INFO )
	{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		return FALSE;
	}

	if( ItemPeriodInfo[iItemInfoIndex].btUsedInfo != 1 && ItemPeriodInfo[iItemInfoIndex].wUserIndex != lpObj->m_Index )
	{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		return FALSE;
	}

#ifdef MODIFY_CHECK_PERIOD_ITEM_LIST_AT_ITEMBUY_20070619
	// 버그 추적을 위한 로그 추가
	LogAddTD( "[CashShop][PeriodItemEffectClear] User Data - ID : %s, Name : %s, Effect Data [%d] Code : %d, LeftTime : %d, EffectType1 : %d, EffectType2 : %d, ExpireDate : %s",
		lpObj->AccountID, lpObj->Name,
		iItemInfoIndex,
		ItemPeriodInfo[iItemInfoIndex].iItemCode,
		ItemPeriodInfo[iItemInfoIndex].iItemPeriodLeftTime,
		ItemPeriodInfo[iItemInfoIndex].btEffectType1,
		ItemPeriodInfo[iItemInfoIndex].btEffectType2,
		ItemPeriodInfo[iItemInfoIndex].chItemPeriodEndDate
		);
#endif // MODIFY_CHECK_PERIOD_ITEM_LIST_AT_ITEMBUY_20070619

	ItemPeriodInfo[iItemInfoIndex].btDateInfoUseCount--;

	if( ItemPeriodInfo[iItemInfoIndex].btDateInfoUseCount == 0 || iItemCode == PERIOD_ITEM_CLEAR_ALL )			// 모두 삭제 (초기화)
	{
		ItemPeriodInfo[iItemInfoIndex].btUsedInfo			= 0;
		ItemPeriodInfo[iItemInfoIndex].wUserIndex			= 0;
		ItemPeriodInfo[iItemInfoIndex].iUserGuid			= 0;
		ItemPeriodInfo[iItemInfoIndex].btDateInfoUseCount	= 0;
		ItemPeriodInfo[iItemInfoIndex].dwItemCheckTickCount = 0;
		ItemPeriodInfo[iItemInfoIndex].chCharacterName[0]	= '\0';
		for( int i = 0 ; i < MAX_PERIODITEM_DATEINFO ; i++ )
		{
			g_ItemAddOption.ClearItemEffect( lpObj, ItemPeriodInfo[iItemInfoIndex].DateInfo[i].iItemCode, ITEMUSE_EFFECT_OPTION_CANCEL );

			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].btUsedInfo		= 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].iItemCode		= 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].btEffectCategory = 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].btEffectType1	= 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].btEffectType2	= 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].dwItemLeftTime	= 0;
			ItemPeriodInfo[iItemInfoIndex].DateInfo[i].dwItemUsedTime	= 0;
		}

		lpObj->m_iPeriodItemEffectIndex = -1;
	}
	else
	{
		int iDateInfoIndex = 0;

		for( iDateInfoIndex = 0 ; iDateInfoIndex < MAX_PERIODITEM_DATEINFO ; iDateInfoIndex++ )
		{
			if( ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].iItemCode == iItemCode )
			{
				g_ItemAddOption.ClearItemEffect( lpObj, ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].iItemCode, ITEMUSE_EFFECT_OPTION_CANCEL );

				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].btUsedInfo			= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].iItemCode			= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].btEffectCategory	= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].btEffectType1		= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].btEffectType2		= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].dwItemLeftTime		= 0;
				ItemPeriodInfo[iItemInfoIndex].DateInfo[iDateInfoIndex].dwItemUsedTime		= 0;

	#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
				//DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
	#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
				//GCStateInfoSend( lpObj, 0, STATE_MARK_USE );
			}
		}
	}
#ifdef MODIFY_BUFFITEM_MEDICINE_20080424
	int iBuffType = BUFFTYPE_NONE;
	iBuffType = GetBuffType(iItemCode);
	//비약은 능력치를 올려주는 아이템으로 gObjCalCharacter 호출한다.
	switch (iBuffType)
	{
	case BUFFTYPE_PCS_MEDICINE_STRENGTH:
	case BUFFTYPE_PCS_MEDICINE_DEXTERITY:
	case BUFFTYPE_PCS_MEDICINE_VITALITY:
	case BUFFTYPE_PCS_MEDICINE_ENERGY:
	case BUFFTYPE_PCS_MEDICINE_LEADERSHIP:
		gObjCalCharacter(lpObj->m_Index);
		break;
	}
#endif // MODIFY_BUFFITEM_MEDICINE_20080424

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	return TRUE;
}
#else
BOOL CCashItemPeriodSystem::ClearPeriodItemEffect(LPOBJECTSTRUCT lpObj)
{
	int iItemInfoIndex = 0;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	EnterCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619

	iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;

	if( iItemInfoIndex < 0 || iItemInfoIndex > MAX_ITEMPERIOD_INFO )
	{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		return FALSE;
	}

	if( ItemPeriodInfo[iItemInfoIndex].btUsedInfo != 1 && ItemPeriodInfo[iItemInfoIndex].iUserIndex != lpObj->m_Index )
	{
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
		return FALSE;
	}

#ifdef MODIFY_CHECK_PERIOD_ITEM_LIST_AT_ITEMBUY_20070619
	// 버그 추적을 위한 로그 추가
	LogAddTD( "[CashShop][PeriodItemEffectClear] User Data - ID : %s, Name : %s, Effect Data [%d] Code : %d, LeftTime : %d, EffectType1 : %d, EffectType2 : %d, ExpireDate : %s",
		lpObj->AccountID, lpObj->Name,
		iItemInfoIndex,
		ItemPeriodInfo[iItemInfoIndex].iItemCode,
		ItemPeriodInfo[iItemInfoIndex].iItemPeriodLeftTime,
		ItemPeriodInfo[iItemInfoIndex].btEffectType1,
		ItemPeriodInfo[iItemInfoIndex].btEffectType2,
		ItemPeriodInfo[iItemInfoIndex].chItemPeriodEndDate
		);
#endif // MODIFY_CHECK_PERIOD_ITEM_LIST_AT_ITEMBUY_20070619

	ItemPeriodInfo[iItemInfoIndex].btUsedInfo			= 0;
	ItemPeriodInfo[iItemInfoIndex].iUserIndex			= 0;
	ItemPeriodInfo[iItemInfoIndex].iItemCode			= 0;
	ItemPeriodInfo[iItemInfoIndex].iItemPeriodLeftTime	= 0;
	ItemPeriodInfo[iItemInfoIndex].btEffectType1		= 0;
	ItemPeriodInfo[iItemInfoIndex].btEffectType2		= 0;
	ItemPeriodInfo[iItemInfoIndex].iUserGuid			= 0;
	memset( ItemPeriodInfo[iItemInfoIndex].chCharacterName,		0, MAX_IDSTRING + 1 );
	memset( ItemPeriodInfo[iItemInfoIndex].chItemPeriodBuyDate,	0, MAX_ITEMPERIOD_DATE_LENGTH );
	memset( ItemPeriodInfo[iItemInfoIndex].chItemPeriodEndDate,	0, MAX_ITEMPERIOD_DATE_LENGTH );

	g_ItemAddOption.ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_CANCEL );
#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
	GCStateInfoSend( lpObj, 0, STATE_MARK_USE );

	lpObj->m_iPeriodItemEffectIndex = -1;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	LeaveCriticalSection( &csPeriodItemSystem );
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619

	return TRUE;
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

void CCashItemPeriodSystem::GDReqPeriodItemInsert(LPOBJECTSTRUCT lpObj, int iItemCode, int iItemEffectDate /*단위:분*/)
{
	PMSG_REQ_PERIODITEM_INSERT pMsg;
	int iEffectType1 = 0;
	int iEffectType2 = 0;
	CTime tExpireDate;

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int iEffectUseTime = 0;

	if( g_ItemAddOption.SearchItemEffectType( iItemCode, &iEffectType1, &iEffectType2, &iEffectUseTime ) == false )
	{
		return;
	}
	
	if( iItemEffectDate == ITEM_EFFECTDATE_USE_FILE )
	{
		// ItemAddOption에서의 시간은 초단위기 떄문에 분단위로 변환해 준다.
		iItemEffectDate = iEffectUseTime / 60;
	}

	int iEffectCategory = g_ItemAddOption.GetItemEffectOption( iItemCode );

	if( iEffectCategory == -1 )
	{
		return;
	}

	pMsg.btEffectCategory	= iEffectCategory;
#else
	if( g_ItemAddOption.SearchItemEffectType( iItemCode, &iEffectType1, &iEffectType2 ) == false )
	{
		return;
	}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	pMsg.iUserIndex			= lpObj->m_Index;
	pMsg.iUserGuid			= lpObj->DBNumber;
	pMsg.iItemCode			= iItemCode;
	pMsg.btItemEffectType1	= iEffectType1;
	pMsg.btItemEffectType2	= iEffectType2;
	pMsg.iItemPeriodDate	= iItemEffectDate;
	memcpy( pMsg.chUserID, lpObj->Name, MAX_IDSTRING + 1 );

	tExpireDate = CalcItemUseEndTime( iItemEffectDate );
	wsprintf( pMsg.chExpireDate, "%d-%d-%d %d:%d",
		tExpireDate.GetYear(), tExpireDate.GetMonth(), tExpireDate.GetDay(), tExpireDate.GetHour(), tExpireDate.GetMinute() );

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x03, sizeof( PMSG_REQ_PERIODITEM_INSERT ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_INSERT ) );

	LogAddTD("[CashShop][PeriodItemInsert Req] - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, ExpireDate : %s",
		lpObj->AccountID, lpObj->Name,
		pMsg.iItemCode,
		pMsg.btItemEffectType1,
		pMsg.btItemEffectType2,
		pMsg.iItemPeriodDate,
		pMsg.chExpireDate );
}

void CCashItemPeriodSystem::GDReqPeriodItemList(LPOBJECTSTRUCT lpObj)
{
	PMSG_REQ_PERIODITEM_LIST pMsg;

	pMsg.iUserIndex	= lpObj->m_Index;
	pMsg.iUserGuid	= lpObj->DBNumber;

	memcpy( pMsg.chCharacterName, lpObj->Name, MAX_IDSTRING + 1 );

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x05, sizeof( PMSG_REQ_PERIODITEM_LIST ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_LIST ) );
}

void CCashItemPeriodSystem::GDReqPeriodItemInquiry(LPOBJECTSTRUCT lpObj)
{
	PMSG_REQ_PERIODITEM_INQUIRY pMsg;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid	= lpObj->DBNumber;

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x01, sizeof( PMSG_REQ_PERIODITEM_INQUIRY ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_INQUIRY ) );
}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
void CCashItemPeriodSystem::GDReqPeriodItemUpdate(LPOBJECTSTRUCT lpObj, int iDateInfoIndex)
{
	PMSG_REQ_PERIODITEM_UPDATE pMsg;
	LPITEMPERIOD_INFO lpItemInfo = NULL;

	if( lpObj->m_iPeriodItemEffectIndex < 0 || lpObj->m_iPeriodItemEffectIndex >= MAX_ITEMPERIOD_INFO )
	{
		return;
	}

	if( iDateInfoIndex < 0 || iDateInfoIndex > MAX_PERIODITEM_DATEINFO )
	{
		return;
	}

	lpItemInfo = &ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];

	if( lpItemInfo->btUsedInfo == 0 )
	{
		return;
	}

	pMsg.iUserGuid			= lpObj->DBNumber;
	pMsg.iUserIndex			= lpObj->m_Index;
	pMsg.iItemCode			= lpItemInfo->DateInfo[iDateInfoIndex].iItemCode;
	pMsg.iLeftTime			= lpItemInfo->DateInfo[iDateInfoIndex].dwItemLeftTime;
	pMsg.iUsedTime			= lpItemInfo->DateInfo[iDateInfoIndex].dwItemUsedTime;

	memcpy( pMsg.chCharacterName, lpItemInfo->chCharacterName, MAX_IDSTRING + 1 );

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x07, sizeof( PMSG_REQ_PERIODITEM_UPDATE ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_UPDATE ) );
}
#else
void CCashItemPeriodSystem::GDReqPeriodItemUpdate(LPOBJECTSTRUCT lpObj)
{
	PMSG_REQ_PERIODITEM_UPDATE pMsg;
	LPITEMPERIOD_INFO lpItemInfo = NULL;

	if( lpObj->m_iPeriodItemEffectIndex < 0 || lpObj->m_iPeriodItemEffectIndex >= MAX_ITEMPERIOD_INFO )
	{
		return;
	}

	lpItemInfo = &ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];

	pMsg.iUserGuid	= lpObj->DBNumber;
	pMsg.iItemCode	= lpItemInfo->iItemCode;
	pMsg.iUserIndex	= lpObj->m_Index;
	pMsg.iUsedTime	= lpItemInfo->iItemPeriodUsedTime;
	pMsg.iLeftTime	= lpItemInfo->iItemPeriodLeftTime;

	memcpy( pMsg.chCharacterName, lpObj->Name, MAX_IDSTRING + 1 );

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x07, sizeof( PMSG_REQ_PERIODITEM_UPDATE ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_UPDATE ) );
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

void CCashItemPeriodSystem::DGAnsPeriodItemInsert(LPPMSG_ANS_PERIODITEM_INSERT aRecv)
{
	int iIndex = aRecv->iUserIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	BYTE btResult = 0x00;

	switch( aRecv->btResult )
	{
	case 0x00:		// 성공
		btResult = 0x01;
		break;
	case 0x01:		// 이미 등록된 아이템이 있음
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		// 이미 등록된 아이템이 있더라도 성공으로 한다.
		btResult = 0x01;
#else
		btResult = 0x02;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		break;
	case 0x02:		// DB 오류
		btResult = 0x03;
		break;
	default:		// 오류 (무조건 실패로 간주)
		btResult = 0x01;
		break;
	}

	if( btResult == 0x01 )		// 성공! 아이템의 효과를 적용시켜준다.
	{
		SetPeriodItemEffect( lpObj, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate );
		LogAddTD("[CashShop][PeriodItemInsert Ans] Success - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name,
			aRecv->iItemCode,
			aRecv->btItemEffectType1,
			aRecv->btItemEffectType2,
			aRecv->iItemPeriodDate );
	}
	else
	{
		LogAddTD("[CashShop][PeriodItemInserver Ans] Fail - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, Result : %d",
			lpObj->AccountID,
			lpObj->Name,
			aRecv->iItemCode,
			aRecv->btItemEffectType1,
			aRecv->btItemEffectType2,
			aRecv->iItemPeriodDate,
			btResult );
	}

	GCSendPeriodItemInserResult( lpObj, btResult );
}

void CCashItemPeriodSystem::DGAnsPeriodItemList(LPPMSG_ANS_PERIODITEM_LIST aRecv)
{
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	if( aRecv->iUserIndex <= 0 || aRecv->iUserIndex > MAX_OBJECT )
#else
	if( aRecv->wUserIndex <= 0 || aRecv->wUserIndex > MAX_OBJECT )
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	{
		return;
	}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iUserIndex];
#else
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->wUserIndex];
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int iItemLeftUseTime = 0;
	int iResult = 0;

	iResult = aRecv->btResult;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	if( lpObj->DBNumber != aRecv->iUserGuid )
	{
		return;
	}

	if( iResult == -1 )
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item over 1 item ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name );
	}
	else if( iResult == -2 )
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item usedtime is over ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name );
	}
#else
	if( iResult < 0 )
	{
		return;
	}
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	int iDateInfoCount = aRecv->btItemUseCount;

	for( int i = 0 ; i < iDateInfoCount ; i++ )
	{
		iItemLeftUseTime = aRecv->Date[i].dwItemLeftTime;
		if( iItemLeftUseTime > 0 )
		{
			SetPeriodItemEffect( lpObj, aRecv->Date[i].iItemCode, aRecv->Date[i].btEffectType1, aRecv->Date[i].btEffectType2, iItemLeftUseTime );
			LogAddTD("[CashShop][PeriodItemList Ans] Success - ID : %s, Name : %s, ItemCode : %d, OptionType : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
				lpObj->AccountID, lpObj->Name,
				aRecv->Date[i].iItemCode,
				aRecv->Date[i].btEffectCategory,
				aRecv->Date[i].btEffectType1,
				aRecv->Date[i].btEffectType2,
				iItemLeftUseTime );
		}
		else
		{
			LogAddTD("[CashShop][PeriodItemList Ans] Fail - ID : %s, Name : %s, ItemCode : %d, OptionType : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
				lpObj->AccountID, lpObj->Name,
				aRecv->Date[i].iItemCode,
				aRecv->Date[i].btEffectCategory,
				aRecv->Date[i].btEffectType1,
				aRecv->Date[i].btEffectType2,
				iItemLeftUseTime );
		}
	}
#else
	#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	iItemLeftUseTime = aRecv->iLeftTime;
	#else
	iItemLeftUseTime = CalcItemUseLeftTime( aRecv->chPeriodItemEndDate );
	if( iItemLeftUseTime == 0 )
	{
		iItemLeftUseTime = 1;
	}
	#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

	if( iItemLeftUseTime > 0 )
	{
		SetPeriodItemEffect( lpObj, aRecv->iItemCode, aRecv->btEffectType1, aRecv->btEffectType2, iItemLeftUseTime );
		LogAddTD("[CashShop][PeriodItemList Ans] Success - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name,
			aRecv->iItemCode,
			aRecv->btEffectType1,
			aRecv->btEffectType2,
			iItemLeftUseTime );
	}
	else
	{
		LogAddTD("[CashShop][PeriodItemList Ans] Fail - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name,
			aRecv->iItemCode,
			aRecv->btEffectType1,
			aRecv->btEffectType2,
			iItemLeftUseTime );
	}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
}

void CCashItemPeriodSystem::DGAnsPeriodItemInquiry(LPPMSG_ANS_PERIODITEM_INQUIRY aRecv)
{
}

void CCashItemPeriodSystem::DGAnsPeriodItemUpdate(LPPMSG_ANS_PERIODITEM_UPDATE aRecv)
{
	int iUserIndex = aRecv->iUserIndex;

	if( iUserIndex < 0 || iUserIndex > MAX_OBJECT )
	{
		return;
	}

	LPOBJECTSTRUCT lpObj = &gObj[iUserIndex];
	LPITEMPERIOD_INFO lpItemInfo = NULL;

	if( lpObj->m_iPeriodItemEffectIndex < 0 || lpObj->m_iPeriodItemEffectIndex >= MAX_ITEMPERIOD_INFO )
	{
		return;
	}

	lpItemInfo = &ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	if( lpObj->m_iPeriodItemEffectIndex != lpItemInfo->wUserIndex )
#else
	if( lpObj->m_iPeriodItemEffectIndex != lpItemInfo->iUserIndex )
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	{
		return;
	}

	switch( aRecv->btResult )
	{
	case 0x00:			// 성공
		break;
	case 0x01:			// 아이템이 존재하지 않음
		break;
	case 0x02:			// DB오류
		break;
	case 0x03:			// 사용기간 만료
		// 아이템 효과를 삭제시킨다.
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		if( ClearPeriodItemEffect( lpObj, aRecv->iItemCode ) == TRUE )
#else
		if( ClearPeriodItemEffect( lpObj ) == TRUE )
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		{
			LogAddTD("[CashShop][PeriodItemUpdate Ans] Remove Item Effect - ID : %s, Name : %s, ItemCode : %d, Result : %d",
				lpObj->AccountID,
				lpObj->Name,
				aRecv->iItemCode,
				aRecv->btResult );
		}
		break;
	case 0x04:			// 기타
		break;
	}

	LogAddTD("[CashShop][PeriodItemUpdate Ans] - ID : %s, Name : %s, ItemCode : %d, Result %d",
		lpObj->AccountID, lpObj->Name,
		aRecv->iItemCode,
		aRecv->btResult );
}

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
void CCashItemPeriodSystem::GDReqPeriodItemDelete(LPOBJECTSTRUCT lpObj, char* pchCharacterName)
{
	PMSG_REQ_PERIODITEM_DELETE pMsg;
	LPITEMPERIOD_INFO lpItemInfo = NULL;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid	= lpObj->DBNumber;
	memcpy( pMsg.chCharacterName, pchCharacterName, MAX_IDSTRING+1 );

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x09, sizeof( PMSG_REQ_PERIODITEM_DELETE ) );

	cDBSMng.Send( (char*)&pMsg, sizeof( PMSG_REQ_PERIODITEM_DELETE ) );
}

void CCashItemPeriodSystem::DGAnsPeriodItemDelete(LPPMSG_ANS_PERIODITEM_DELETE aRecv)
{
	LPOBJECTSTRUCT lpObj = NULL;

	if( aRecv->iUserIndex < 0 || aRecv->iUserIndex > MAX_OBJECT )
	{
		return;
	}

	lpObj = &gObj[aRecv->iUserGuid];

	if( lpObj->DBNumber != aRecv->iUserGuid )
	{
		return;
	}

	switch( aRecv->btResult )
	{
	case 0:			// Success
		LogAddTD("[CashShop][PeriodItemDelete Ans] - ID : %s, Name : %s",
			lpObj->AccountID,
			lpObj->Name );
		break;
	case 1:			// Can't find user index
		break;
	}
}
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

void CCashItemPeriodSystem::GCSendPeriodItemInserResult(LPOBJECTSTRUCT lpObj, BYTE btResult)
{
	PMSG_ANS_PERIODITEM_INSERT_RESULT pMsg;

	pMsg.btResult = btResult;

	PHeadSubSetB( (LPBYTE)&pMsg, 0xD0, 0x03, sizeof( PMSG_ANS_PERIODITEM_INSERT_RESULT ) );

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, sizeof( PMSG_ANS_PERIODITEM_INSERT_RESULT ) );
}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
bool CCashItemPeriodSystem::CheckItemUsedInfo(LPOBJECTSTRUCT lpObj, int iItemCode)
{
	int iPeriodItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;
	int iItemEffectType = 0;

	iItemEffectType = g_ItemAddOption.GetItemEffectOption( iItemCode );

	if( iPeriodItemInfoIndex == -1 )
	{
		return true;
	}
	else if( ItemPeriodInfo[iPeriodItemInfoIndex].iUserGuid != lpObj->DBNumber )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_PERIODITEM_DATEINFO; i++ )
	{
		if( ItemPeriodInfo[iPeriodItemInfoIndex].DateInfo[i].btUsedInfo == 1 )
		{
			if( ItemPeriodInfo[iPeriodItemInfoIndex].DateInfo[i].btEffectCategory == iItemEffectType )
			{
				ClearPeriodItemEffect( lpObj, ItemPeriodInfo[iPeriodItemInfoIndex].DateInfo[i].iItemCode );
				return false;
			}
		}
	}

	return true;
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
BOOL CCashItemPeriodSystem::CheckMapBoundFreeItemUse(LPOBJECTSTRUCT lpObj)
{
	int iPeriodItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;

	if( iPeriodItemInfoIndex == -1 )
	{
		return FALSE;
	}

	if( ItemPeriodInfo[iPeriodItemInfoIndex].btUsedInfo == 0 )
	{
		return FALSE;
	}

	for( int i = 0 ; i < MAX_PERIODITEM_DATEINFO ; i++ )
	{
		if( ItemPeriodInfo[iPeriodItemInfoIndex].DateInfo[i].btUsedInfo == 1 )
		{
			switch( ItemPeriodInfo[iPeriodItemInfoIndex].DateInfo[i].iItemCode )
			{
			case MAKE_ITEMNUM2( 13, 59 ):

			case MAKE_ITEMNUM2( 13, 43 ):
			case MAKE_ITEMNUM2( 13, 44 ):
			case MAKE_ITEMNUM2( 13, 45 ):
			case MAKE_ITEMNUM2( 13, 52 ):
#ifdef ADD_CASHITEM_4RD_MARK_20080723
			case MAKE_ITEMNUM2( 13, 62 ):	// 치유의 인장
			case MAKE_ITEMNUM2( 13, 63 ):	// 신성의 인장
#endif // ADD_CASHITEM_4RD_MARK_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			case MAKE_ITEMNUM2( 13, 93):
			case MAKE_ITEMNUM2( 13, 94):
#endif
#ifdef ADD_PARTY_EXP_INCREASE_SCROLL_20090916
			case MAKE_ITEMNUM2( 13, 103 ):
#endif // ADD_PARTY_EXP_INCREASE_SCROLL_20090916
				return TRUE;
			}
		}
	}

	return FALSE;
}

LPITEMPERIOD_INFO CCashItemPeriodSystem::GetPeriodInfo(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_iPeriodItemEffectIndex < 0 )
	{
		return NULL;
	}

	if( ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex].btUsedInfo == 0 )
	{
		return NULL;
	}

	return &ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
int CCashItemPeriodSystem::GetBuffType(int iItemCode)
{
	int iBuffType = BUFFTYPE_NONE;
	switch (iItemCode)
	{
	case MAKE_ITEMNUM2( 13, 43 ) :	iBuffType = BUFFTYPE_PCS_MARK1;					break;	//상승의인장
	case MAKE_ITEMNUM2( 13, 44 ) :	iBuffType = BUFFTYPE_PCS_MARK2;					break;	//풍요의인장
	case MAKE_ITEMNUM2( 13, 45 ) :	iBuffType = BUFFTYPE_PCS_MARK3;					break;	//유지의인장
	case MAKE_ITEMNUM2( 13, 59 ) :	iBuffType = BUFFTYPE_PCS_MARK4;					break;	//이동의인장
#ifdef ADD_CASHITEM_4RD_MARK_20080723
	case MAKE_ITEMNUM2( 13, 62 ) :	iBuffType = BUFFTYPE_PCS_MARK5;					break;	// 치유의인장
	case MAKE_ITEMNUM2( 13, 63 ) :	iBuffType = BUFFTYPE_PCS_MARK6;					break;	// 신성의인장
#endif // ADD_CASHITEM_4RD_MARK_20080723

	case MAKE_ITEMNUM2( 14, 72 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_HASTE;			break;	//신속의스크롤
	case MAKE_ITEMNUM2( 14, 73 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_DEFENSE;		break;	//방어의스크롤
	case MAKE_ITEMNUM2( 14, 74 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_ANGER;			break;	//분노의스크롤
	case MAKE_ITEMNUM2( 14, 75 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_MAGIC;			break;	//마력의스크롤
	case MAKE_ITEMNUM2( 14, 76 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_HEALTH;			break;	//체력의스크롤
	case MAKE_ITEMNUM2( 14, 77 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_MANA;			break;	//마나의스크롤
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
	case MAKE_ITEMNUM2( 14, 97 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_CRITICAL;		break;	//전투의스크롤
	case MAKE_ITEMNUM2( 14, 98 ) :	iBuffType = BUFFTYPE_PCS_SCROLL_EXCELLENT;		break;	//강화의스크롤
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
		
	case MAKE_ITEMNUM2( 14, 78 ) :	iBuffType = BUFFTYPE_PCS_MEDICINE_STRENGTH;		break;	//힘의비약
	case MAKE_ITEMNUM2( 14, 79 ) :	iBuffType = BUFFTYPE_PCS_MEDICINE_DEXTERITY;	break;	//민첩의비약
	case MAKE_ITEMNUM2( 14, 80 ) :	iBuffType = BUFFTYPE_PCS_MEDICINE_VITALITY;		break;	//체력의비약
	case MAKE_ITEMNUM2( 14, 81 ) :	iBuffType = BUFFTYPE_PCS_MEDICINE_ENERGY;		break;	//에너지의비약
	case MAKE_ITEMNUM2( 14, 82 ) :	iBuffType = BUFFTYPE_PCS_MEDICINE_LEADERSHIP;	break;	//통솔의비약

#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
    case MAKE_ITEMNUM2( 13, 81 ) :   iBuffType = BUFFTYPE_CHARM_GUARDIAN; break;		// 수호의부적 
	case MAKE_ITEMNUM2( 13, 82 ) :   iBuffType = BUFFTYPE_CHARM_PROTECTITEM; break;	// 아이템 보호의 부적 
	case MAKE_ITEMNUM2( 13, 93 ) :	iBuffType = BUFFTYPE_PCS_MARK1_FOR_MASTER; break; // 상승의 부적 마스터
	case MAKE_ITEMNUM2( 13, 94 ) :	iBuffType = BUFFTYPE_PCS_MARK2_FOR_MASTER; break; // 풍요의 인장 마스터 
#endif 
		
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
	case MAKE_ITEMNUM2( 13, 103 ) :	iBuffType = BUFFTYPE_PARTY_EXP_INCREASE_SCROLL; break;	// 파티 경험치 증가 주문서
	case MAKE_ITEMNUM2( 13, 104 ) :	iBuffType = BUFFTYPE_MAX_AG_INCREASE_AURA; break; // 최대 AG 증가 오라
	case MAKE_ITEMNUM2( 13, 105 ) :	iBuffType = BUFFTYPE_MAX_SD_INCREASE_AURA; break; // 최대 SD 증가 오라
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915

	default:
		iBuffType = BUFFTYPE_NONE;
	}
	
	return iBuffType;
}

BOOL CCashItemPeriodSystem::SetPeriodItemBuffEffect(LPOBJECTSTRUCT lpObj, int iItemCode, int iItemUsePeriod)
{
	BOOL bResult = FALSE;
	
	int iBuffType = BUFFTYPE_NONE;
	iBuffType = GetBuffType(iItemCode);
	if (iBuffType != BUFFTYPE_NONE) {
		DWORD dwItemUsePeriod = 60 * iItemUsePeriod;
		bResult = gObjAddBuffEffect( lpObj, iBuffType, dwItemUsePeriod);
		
		//비약은 능력치를 올려주는 아이템으로 gObjCalCharacter 호출한다.
		switch (iBuffType)
		{
		case BUFFTYPE_PCS_MEDICINE_STRENGTH:
		case BUFFTYPE_PCS_MEDICINE_DEXTERITY:
		case BUFFTYPE_PCS_MEDICINE_VITALITY:
		case BUFFTYPE_PCS_MEDICINE_ENERGY:
		case BUFFTYPE_PCS_MEDICINE_LEADERSHIP:
			gObjCalCharacter(lpObj->m_Index);
			break;
		}
	} else {
		//bResult = g_ItemAddOption.SetItemEffect( lpObj, iItemCode, iItemUsePeriod );
		if( bResult = g_ItemAddOption.SetItemEffect( lpObj, iItemCode, iItemUsePeriod ) == true )
		{
			// 뷰포트에 효과 삭제
#ifdef MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
			//ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_MARK_USE );
#endif // MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
			//GCStateInfoSend( lpObj, 1, STATE_MARK_USE );
		}
	}
	
	return bResult;
}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
