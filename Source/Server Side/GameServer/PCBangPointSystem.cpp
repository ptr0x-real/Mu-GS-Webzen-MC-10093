// PCBangPointSystem.cpp: implementation of the CPCBangPointSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCBangPointSystem.h"

#include "..\Include\ReadScript.h"
#include "..\Include\public.h"
#include "..\\Common\\winutil.h"
#include "..\INCLUDE\cb_protocol.h"
#include "DSProtocol.h"
#include "Event.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#include "DirPath.h"
extern CDirPath		gDirPath;

#include "MapClass.h"
extern MapClass		MapC[];

#include "Shop.h"
extern CShop		ShopC[];

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef PCBANG_KUZIMO
#include "MDate.h"
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206

short g_nPCBangPointFirstRuleTime		= 60 * 10;	// 최초 규칙 시간 단위(60초 * 10 = 10분) 
short g_nPCBangPointFirstRuleTimePoint	= 1;		// 최초 규칙 시간을 만족했을 때 부여 포인트
short g_nPCBangPointCommonRuleTime		= 60 * 10;	// 일반 규칙 시간 단위(60초 * 10 = 10분) 
short g_nPCBangPointCommonRuleTimePoint	= 1;		// 일반 규칙 시간을 만족했을 때 부여 포인트
short g_nPCBangMaxPoint					= 1008;		// 최대 적립 가능한 포인트

// FeverTime 기간에는 (평소 부여 포인트 * g_nPCBangPointFeverMultipleValue)포인트가 적립된다.
// g_nPCBangPointFeverStartHour와 g_nPCBangPointFeverEndHour가 같으면 동작하지 않는다.
// g_nPCBangPointFeverStartHour = 4, g_nPCBangPointFeverEndHour = 5라면 4시부터 작동하며, 5시가 되면 작동 해제된다.
short g_nPCBangPointFeverStartHour		= 20;		// 시간은 0시부터 24시까지 넣을 수 있다. 0시, 24시는 동일하다.
short g_nPCBangPointFeverEndHour		= 24;		//
short g_nPCBangPointFeverMultipleValue	= 2;

short g_nPCBangResetDayOfWeek			= 2;		// 리셋 요일을 정한다. 0(일), 1(월)... 
short g_nPCBangResetHour				= 9;		// 리셋 시간. 보통 8시부터 10시까지 점검이다. 부하를 덜기 위해서 9시로 설정한다.

#ifdef ADD_PCBANG_EXPRATE
float g_fPCBangAddExperience			= 1.f;
#endif // ADD_PCBANG_EXPRATE

CPCBangPointSystem g_PCBangPointSystem;

BYTE g_btPCBangPointApplicationLevel;		// PC방 포인트 시스템 적용 대상

// 포인트 저장시 일반서버/테스트서버 구분을 짓는다.
enum PCBANG_POINT_ITEM_SERVER_TYPE
{
	ST_SERVICESERVER		= 0,
	ST_TESTSERVER			= 1,
#ifdef PCBANG_KUZIMO	// 이부분의 숫자를 바꾸면 않됩니다.  DB에서 서버 식별로 사용합니다.
	ST_TEMPSERVER			= 2,
#endif
};

enum PCBANG_POINT_CHANGE_POINT_TYPE
{
	CT_ADD_FOR_TIME			= 0,
	CT_MINUS_FOR_BUY_ITEM	= 1,
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCBangPointSystem::CPCBangPointSystem()
{
	m_bPCBangPointSystemOn				= FALSE;
	m_bPCBangPointRankingEvent			= FALSE;

	g_btPCBangPointApplicationLevel		= PP_PCBANG_USER_ONLY;

	m_btPCBangPointShopNum				= 14;

	InitItemOption();
}

CPCBangPointSystem::~CPCBangPointSystem()
{

}

//-------------------------------------------------------------------------
// 초기화
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::InitItemOption()
{
	for( int n = 0; n <= MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		memset( &m_ItemOption[n], 0, sizeof( PCBANG_POINT_ITEM_OPTION ) );
#ifdef MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
		m_ItemOption[n].dwItemType = -1;
#endif // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	}	
}

//-------------------------------------------------------------------------
// 데이터 읽어오기
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::LoadData( LPSTR lpszFileName )
{
	// 1. PCBangPointShop.txt 파일 읽기
	m_bFileDataLoad = FALSE;
	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) )
	{
		MsgBox( "[PCBangPointSystem] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )
		{
			DWORD dwError = GetLastError();
			MsgBox( "[PCBangPointSystem] - Can't Open %s ", lpszFileName );
			return FALSE;
		}
		
		// 모든 데이터 초기화.
		InitItemOption();

		int iItemLoadCount = 0;
		SMDToken Token;
		int iType = -1;

		WORD    wType				= 0;
		WORD	wIndex				= 0;		
		short	nBuyingPoint		= 0;	// 구입 포인트
		short	nConditionPoint		= 0;	// 구입 조건	
		BYTE	btAbility			= 0;	// 아이템의 기능 0:일반아이템 1:버프형
		DWORD	dwKeepTime_Sec		= 0;	// 지속시간(초단위)	
		short	nAddExpPercent		= 0;	// 경험치 상승률
		short	nGetItemPercent		= 0;	// 아이템 획득 확률
		BYTE	btNoMoreExp			= 0;	// 경험치 증가 막음

		while( TRUE )
		{
			Token = GetToken();
			if( Token == END ) break;
			iType = ( int )TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{					
					// 1. >	데이터 초기화
					wType				= 0;
					wIndex				= 0;					
					nBuyingPoint		= 0;
					nConditionPoint		= 0;
					btAbility			= 0;
					dwKeepTime_Sec		= 0;	
					nAddExpPercent		= 0;
					nGetItemPercent		= 0;	
					btNoMoreExp			= 0;

					// 2. >	데이터 읽기
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;
					
					wType = ( WORD )TokenNumber;
					Token = GetToken(); wIndex				= ( WORD )TokenNumber;
					Token = GetToken(); nBuyingPoint		= ( short )TokenNumber;
					Token = GetToken(); nConditionPoint		= ( short )TokenNumber;
					Token = GetToken(); btAbility			= ( BYTE )TokenNumber;
					Token = GetToken();	dwKeepTime_Sec		= ( DWORD )TokenNumber;	
					Token = GetToken(); nAddExpPercent		= ( short )TokenNumber;
					Token = GetToken(); nGetItemPercent		= ( short )TokenNumber;	
					Token = GetToken(); btNoMoreExp			= ( BYTE )TokenNumber;
					
					
					// 3. >	데이터 유효성 검사
					if( iItemLoadCount < 0 || iItemLoadCount > MAX_PCBANG_POINT_ITEM_COUNT )
					{
						MsgBox( "[PCBangPointSystem] - Exceed Max Item Count (%d)", iItemLoadCount );
						break;
					}
					
					// 4. > 데이터 세팅
					LPPCBANG_POINT_ITEM_OPTION pInfo = &m_ItemOption[iItemLoadCount];
					pInfo->dwItemType		= MAKE_ITEMNUM( wType, wIndex );
					pInfo->nBuyingPoint		= nBuyingPoint;
					pInfo->nConditionPoint	= nConditionPoint;
					pInfo->btAbility		= btAbility;
					pInfo->dwKeepTime_Sec	= dwKeepTime_Sec;
					pInfo->nAddExpPercent	= nAddExpPercent;
					pInfo->nGetItemPercent	= nGetItemPercent;
					pInfo->btNoMoreExp		= btNoMoreExp;

					++iItemLoadCount;
				}
				else
				{
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;
				}
			}
		}
		fclose( SMDFile );
		
		LogAddC( 2, "[PCBangPointSystem] - %s file is Loaded", lpszFileName );		
	}
	catch(...) 
	{
		MsgBox( "[PCBangPointSystem] Loading Exception Error (%s) File. ", lpszFileName );
	}

	// 2. commonserver.cfg 설정 읽기
	m_bPCBangPointSystemOn				= GetPrivateProfileInt( "GameServerInfo", "PCBangPointSystemOn", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_btPCBangPointApplicationLevel		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointSystemLevel", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	m_bPCBangPointRankingEvent			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointRankingEvent", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );


	//-- Start 20080922 by kuzimo
	g_nPCBangPointFirstRuleTime			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFirstRuleTime", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFirstRuleTimePoint	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFirstRuleTimePoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointCommonRuleTime		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointCommonRuleTime", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointCommonRuleTimePoint	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointCommonRuleTimePoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangMaxPoint					= GetPrivateProfileInt( "GameServerInfo", "PCBangMaxPoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	g_nPCBangPointFeverStartHour		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverStartHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFeverEndHour			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverEndHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFeverMultipleValue	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverMultipleValue", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	g_nPCBangResetDayOfWeek				= GetPrivateProfileInt( "GameServerInfo", "PCBangResetDayOfWeek", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangResetHour					= GetPrivateProfileInt( "GameServerInfo", "PCBangResetHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

#ifdef ADD_PCBANG_EXPRATE
	{
		char szTemp[256];
		GetPrivateProfileString("GameServerInfo", "PCBangAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
		g_fPCBangAddExperience = (float)atof(szTemp);
	}
#endif // ADD_PCBANG_EXPRATE
	//-- End

	LogAddTD( "[PCBangPointSystem][SetPCBangPointInfo] SystemOn:%d, Level:%d, RankingEventOn:%d", 
			m_bPCBangPointSystemOn, g_btPCBangPointApplicationLevel, m_bPCBangPointRankingEvent );

	return m_bFileDataLoad;		
}

//-------------------------------------------------------------------------
// PC방 포인트 시스템 해당 유저인가?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointSystemUser( int iIndex )
{
	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];
	if( lpObj->m_bPCBangPointUser == TRUE )
		return TRUE;

	return FALSE;
}

//-------------------------------------------------------------------------
// PC방 포인트용 아이템인가?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointItem( DWORD dwItemNum )
{
	LPPCBANG_POINT_ITEM_OPTION p = &m_ItemOption[0];
	for( int n = 0; n < MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		if( m_ItemOption[n].dwItemType == dwItemNum )
			return TRUE;
	}
	
	return FALSE;
}

//-------------------------------------------------------------------------
// PC방 포인트 상점을 열 수 있는 좌표인가?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointShopOpenAttr( BYTE btMapNum, short X, short Y )
{
	// 이벤트 맵에서는 상점을 열 수 없다.	
	if( CHECK_BLOODCASTLE( btMapNum ) ||		// 블러드캐슬
		CHECK_CHAOSCASTLE( btMapNum ) ||		// 카오스캐슬
#ifdef DEVILSQUARE_EXTEND_20050221
		CHECK_DEVILSQUARE( btMapNum ) ||		// 데빌스퀘어
#endif
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	
		CHECK_ILLUSION_TEMPLE( btMapNum ) ||	// 환영사원
#endif
		MAP_INDEX_KANTURU_BOSS == btMapNum		// 칸투르 보스맵
		)
	{
		return FALSE;
	}

	// 안전지대 좌표 확인
	BYTE attr= MapC[btMapNum].GetAttr( X, Y );

	if( ( attr & MAP_ATTR_SAFTYZONE ) == MAP_ATTR_SAFTYZONE )
			return TRUE;	
	
	return FALSE;
}

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//-------------------------------------------------------------------------
// PC방 아이템 중 경험치와 관련된 확인( 경험치를 재계산해 돌려준다. )
//-------------------------------------------------------------------------
void CPCBangPointSystem::CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp )
{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// PC방 전용 아이템 처리
	// 마스터 레벨 유저에게는 해당되지 않는다.
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
			// 마스터 레벨도 전용 인장을 사용하면 경험치 재계산 해준다.
		} else {
			return; 	
		}		
	}
#else
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		return; 	
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#endif

#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205	// 버스시스템 수정 - PC방 전용 아이템 처리
	INT64 iRetExp = iExp;
					
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
	{
		// 경험치 증가 막기
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 ) )
	{
		// 경험치 상승
		int iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		iRetExp = iRetExp * iAddExpRate / 100;
	}
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	else if ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) )
	{
		if(g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
			// 버프가 걸려 있어도 마스터 레벨 유저에게만 적용 되도록 한다.
			int iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
			iRetExp = iRetExp * iAddExpRate / 100;
		}
		
	}
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#else
		
	INT64 iRetExp = iExp;
	if( lpObj->m_iPCBangPointItemTime )
	{						
		if( lpObj->m_bPCBangPointNoMoreExp > 0 )
		{
			// 경험치 증가 막기
			iRetExp = 0;
		}
		else if( lpObj->m_iPCBangPointItemAddExpPer > 0 )
		{
			// 경험치 상승
			iRetExp = iRetExp * lpObj->m_iPCBangPointItemAddExpPer / 100;
		}
	}
#endif
	
	iExp = iRetExp;
}
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

//-------------------------------------------------------------------------
// PC방 유저 설정하기
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::SetPCBangPointUser( int iIndex, BOOL bIsPCBangUser )
{
	// 몬스터 제외
	if( gObj[iIndex].Type != OBJTYPE_CHARACTER )
		return;

	if( g_btPCBangPointApplicationLevel == PP_APPLICATION_OF_NONE )
	{
		gObj[iIndex].m_bPCBangPointUser = FALSE;
	}
	else if( g_btPCBangPointApplicationLevel == PP_PCBANG_USER_ONLY )
	{
		gObj[iIndex].m_bPCBangPointUser = bIsPCBangUser;		
	}
	else if( g_btPCBangPointApplicationLevel == PP_APPLICATION_OF_ALL )
	{
		gObj[iIndex].m_bPCBangPointUser = TRUE;		
	}

#ifdef PCBANG_KUZIMO
	// 포인트 정보를 요청한다.
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		LogAddTD( "[PCBangPointSystem][SetPCBangPointUser] [%s]", gObj[iIndex].AccountID );
	}
	g_PCBangPointSystem.GEReqPCBangPointInfo2( iIndex );
#else
	// 포인트 정보를 요청한다.
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		LogAddTD( "[PCBangPointSystem][SetPCBangPointUser] [%s]", gObj[iIndex].AccountID );
		g_PCBangPointSystem.GEReqPCBangPointInfo( iIndex );
	}
#endif

	
}

//-------------------------------------------------------------------------
// PC방 포인트를 추가한다.
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::Run( LPOBJECTSTRUCT lpObj )				
{
	// 0. 몬스터 제외
	if( lpObj->Type != OBJTYPE_CHARACTER )
		return TRUE;
	
	// 1. PC방 포인트 시스템 On/Off 확인
	if( m_bPCBangPointSystemOn == FALSE )
		return TRUE;

	// 2. 적용 대상인가?
	if( lpObj->Connected < CSS_LOGIN )
		return TRUE;

#ifndef PCBANG_KUZIMO
	if( CheckPCBangPointSystemUser( lpObj->m_Index ) == FALSE )
		return TRUE;
#endif

	// 3. EventServer로부터 데이터 전송을 완료받았나?
	if( lpObj->m_PCBangPointInfo.bIsDataLoadSuccess == FALSE )
	{
		return TRUE;
	}

	// 4. 누적 시간이 이상하면 수정.
	if( lpObj->m_PCBangPointInfo.dwCheckRuleTime <= 0 )
	{
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}
	
	// 5. PC방 포인트 아이템을 사용중이면 시간, 포인트가 누적되지 않는다.
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifndef PCBANG_KUZIMO
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 )
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 )
#ifdef ADD_PCBANG_BUFF_MARK4
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK4 )
#endif // ADD_PCBANG_BUFF_MARK4
		)
	{
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}
#endif
#else
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	if( lpObj->m_iPCBangPointItemTime > 0 )
	{
		--lpObj->m_iPCBangPointItemTime;
		if( lpObj->m_iPCBangPointItemTime < 0 )
		{
			lpObj->m_iPCBangPointItemTime = 0;
		}

		if( lpObj->m_iPCBangPointItemTime == 0 )
		{		
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SEAL_ITEM );
			GCStateInfoSend( lpObj, 0, STATE_SEAL_ITEM );

			LogAddTD( "[PCBangPointSystem][Disappear Effect] [%s][%s] (Exp:%d/Item:%d/NoMoreExp:%d)",
					lpObj->AccountID, 
					lpObj->Name, 
					lpObj->m_iPCBangPointItemAddExpPer,
					lpObj->m_iPCBangPointItemGiveItemPer,
					lpObj->m_bPCBangPointNoMoreExp
					);
		}

		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}	
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifdef PCBANG_KUZIMO
	BOOL bFeverTime;
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);

	bFeverTime = FALSE;

	if(g_nPCBangPointFeverStartHour <= g_nPCBangPointFeverEndHour)
	{
		if((sysTime.wHour >= g_nPCBangPointFeverStartHour) && (sysTime.wHour < g_nPCBangPointFeverEndHour))
		{
			bFeverTime = TRUE;
		}
	}
	else
	{
		if((sysTime.wHour >= g_nPCBangPointFeverStartHour) || (sysTime.wHour < g_nPCBangPointFeverEndHour))
		{
			bFeverTime = TRUE;
		}
	}
#endif

	// 6. 시간 확인 및 갱신
	DWORD dwTime = ( GetTickCount() - lpObj->m_PCBangPointInfo.dwCheckRuleTime ) / 1000;

#ifdef PCBANG_KUZIMO
	if(dwTime >= lpObj->m_PCBangPointInfo.nCurrentAppRuleTime)
	{
		//-- 특정 주간마다 포인트 리셋을 한다. 동일 시간에 업데이트되는 DB부하를 줄이기 위해서 조건 내부로 들어왔다.
		if(lpObj->m_kWeeklyTimer.Update() != 2)	// Callback발동시에 시간이 리셋된다. 리셋될때는 포인트를 올리지 말자.
		{
			// 누적시간 갱신
			lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();

			if(CheckPCBangPointSystemUser( lpObj->m_Index ) == TRUE)
			{
				int iAddPCBangPoint = (bFeverTime == TRUE)? lpObj->m_PCBangPointInfo.nCurrentAppRulePoint * g_nPCBangPointFeverMultipleValue :
															lpObj->m_PCBangPointInfo.nCurrentAppRulePoint;

				lpObj->m_PCBangPointInfo.nPCBangPoint += iAddPCBangPoint;

				if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )
					lpObj->m_PCBangPointInfo.nPCBangPoint = 0;
				else if( lpObj->m_PCBangPointInfo.nPCBangPoint > g_nPCBangMaxPoint )
				{
					lpObj->m_PCBangPointInfo.nPCBangPoint =  g_nPCBangMaxPoint;
					GCServerMsgStringSend( lMsg.Get( 3393 ), lpObj->m_Index, 1 );	// "더 이상 포인트를 획득 할 수 없습니다."
				}
				else
				{
					// 정상적으로 포인트가 갱신되었다.
					char szTemp[256];			
					wsprintf( szTemp, lMsg.Get( 3392 ), iAddPCBangPoint );
					GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "포인트 %d 획득"
				}

				// EventServer에 포인트 정보가 변경되었음을 알린다.
				GEReqUpdatePCBangPoint2( lpObj->m_Index );


				LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
					lpObj->AccountID,
					lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
					lpObj->m_PCBangPointInfo.nPCBangPoint,
					lpObj->m_PCBangPointInfo.btCurrentRule );

				// 첫 번째 규칙시간을 통과했다.
				if( lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime == FALSE )
				{
					lpObj->m_PCBangPointInfo.btCurrentRule				= PG_RULE_TYPE_COMMON;
					lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime	= TRUE;
					lpObj->m_PCBangPointInfo.nCurrentAppRuleTime		= g_nPCBangPointCommonRuleTime;
					lpObj->m_PCBangPointInfo.nCurrentAppRulePoint		= g_nPCBangPointCommonRuleTimePoint;			
				}
			}
		}
	}

#else
	if( dwTime >= lpObj->m_PCBangPointInfo.nCurrentAppRuleTime )
	{
		lpObj->m_PCBangPointInfo.nPCBangPoint += lpObj->m_PCBangPointInfo.nCurrentAppRulePoint;

		if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )
			lpObj->m_PCBangPointInfo.nPCBangPoint = 0;
		else if( lpObj->m_PCBangPointInfo.nPCBangPoint > g_nPCBangMaxPoint )
		{
			lpObj->m_PCBangPointInfo.nPCBangPoint =  g_nPCBangMaxPoint;
			GCServerMsgStringSend( lMsg.Get( 3393 ), lpObj->m_Index, 1 );	// "더 이상 포인트를 획득 할 수 없습니다."
		}
		else
		{
			// 정상적으로 포인트가 갱신되었다.
			char szTemp[256];			
			wsprintf( szTemp, lMsg.Get( 3392 ), lpObj->m_PCBangPointInfo.nCurrentAppRulePoint );
			GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "포인트 %d 획득"
		}

		// EventServer에 포인트 정보가 변경되었음을 알린다.
		GEReqUpdatePCBangPoint( lpObj->m_Index );

		// 누적시간 갱신
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();

		LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
			lpObj->AccountID,
			lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
			lpObj->m_PCBangPointInfo.nPCBangPoint,
			lpObj->m_PCBangPointInfo.btCurrentRule );

		// 첫 번째 규칙시간을 통과했다.
		if( lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime == FALSE )
		{
			lpObj->m_PCBangPointInfo.btCurrentRule				= PG_RULE_TYPE_COMMON;
			lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime	= TRUE;
			lpObj->m_PCBangPointInfo.nCurrentAppRuleTime		= g_nPCBangPointCommonRuleTime;
			lpObj->m_PCBangPointInfo.nCurrentAppRulePoint		= g_nPCBangPointCommonRuleTimePoint;			
		}		
	}
#endif

	return TRUE;
}

//-------------------------------------------------------------------------
// 이벤트 서버에 PC방 포인트 정보를 요청한다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqPCBangPointInfo( int iIndex )					
{
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		// EventServer에 정보를 요청한다.
		PMSG_REQ_PCBANG_POINT_INFO pMsg = { 0, };
		PHeadSetB( ( LPBYTE )&pMsg, 0x23, sizeof( pMsg ) );
		
		pMsg.nINDEX = iIndex;
		memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
		
#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
#else
		pMsg.btServerType = ST_SERVICESERVER;
#endif	

		pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

		DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );

		LogAddTD( "[PCBangPointSystem][User Connect] [%s] - %s", gObj[iIndex].AccountID, gObj[iIndex].Ip_addr );
	}
}

//-------------------------------------------------------------------------
// PC방 포인트 정보를 이벤트 서버로부터 정보를 받아 셋팅한다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvPCBangPointInfo( LPBYTE lpMsg )		
{
	LPPMSG_ANS_PCBANG_POINT_INFO pMsg = ( LPPMSG_ANS_PCBANG_POINT_INFO )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	lpObj->m_PCBangPointInfo.bIsDataLoadSuccess	= TRUE;	
	lpObj->m_PCBangPointInfo.nPCBangPoint	= pMsg->nPoint;
	lpObj->m_PCBangPointInfo.dwCheckRuleTime		= GetTickCount(); // 데이터가 정상적으로 로드된 후 시간을 체크한다.

	SendPCBangPointInfo( iIndex );
}

//-------------------------------------------------------------------------
// 이벤트서버에 PC방 포인트 갱신 요청
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdatePCBangPoint( int iIndex )
{
	PMSG_REQ_PCBANG_POINT_NEW_INFO pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x24, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

#ifdef FOR_NEW_TESTSERVER
	pMsg.btServerType = ST_TESTSERVER;
#else
	pMsg.btServerType = ST_SERVICESERVER;
#endif	

	pMsg.nNewPoint = gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC방 포인트 갱신 요청에 대한 응답을 받았다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvUpdatePCBangPoint( LPBYTE lpMsg )
{
	LPPMSG_ANS_PCBANG_POINT_NEW_INFO pMsg = ( LPPMSG_ANS_PCBANG_POINT_NEW_INFO )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	// 이벤트서버에서 데이터 업데이트에 성공했을 때만 클라이언트에 정보를 보낸다.
	if( pMsg->btResult == TRUE )
	{
		SendPCBangPointInfo( iIndex );
	}
}

//-------------------------------------------------------------------------
// 랭킹이벤트 : 누적 사용 포인트로 웹에서 순위를 선정하는 이벤트
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdateRankingEvent( int iIndex, short nUsePoint )
{
//#ifdef PCBANG_KUZIMO
//	#ifdef FOR_TEMP_SERVER
//		return;
//	#endif
//#endif

	// 이벤트가 진행중일때만.
	if( m_bPCBangPointRankingEvent == FALSE )
	{
		return;
	}

	PMSG_PCBANG_POINT_RANKING_EVENT pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x25, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;
#ifdef FOR_NEW_TESTSERVER
	pMsg.btServerType = ST_TESTSERVER;
#else
	pMsg.btServerType = ST_SERVICESERVER;
#endif	
	pMsg.nUsePoint	= nUsePoint;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// 클라이언트로 PC방 포인트를 보낸다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::SendPCBangPointInfo( int iIndex )
{
	PMSG_ANS_GC_PCBANG_POINT_INFO	pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x04, sizeof( pMsg ) );

	pMsg.nPoint		= gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;
	pMsg.nMaxPoint	= g_nPCBangMaxPoint;
#ifdef UPDATE_PCBANGPOINTSYSTEM_BUGFIX
	pMsg.bPCBang	= gObj[iIndex].m_bPCBangPointUser;
#endif // UPDATE_PCBANGPOINTSYSTEM_BUGFIX

	DataSend( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC방 포인트 상점의 정보를 보낸다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::PCBangPointShopOpen( int iIndex )					
{
	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];
	
	// 1. 종료중이면 정보를 보내지 않는다.
	if( lpObj->CloseType != -1 )
		return;

	// 2. 핵 사용 방지용 검사
#ifdef MODIFY_NPCTALK_REQUIREMENT_20051212	
	if( gObjIsConnectedGP( iIndex ) == FALSE )
	{
		LogAddTD( "[ANTI-HACK][PCBangPointShopOpen] Couldn't Open.. (%s)(%s)", lpObj->AccountID, lpObj->Name );
		return;
	}
#endif

	PMSG_ANS_PCBANG_POINT_SHOP_OPEN pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x06, sizeof( pMsg ) );
	pMsg.btResult = 0x00;	// 성공	

#ifndef PCBANG_KUZIMO
	// 3. PC방 포인트 시스템 대상자인지 검사
	if( CheckPCBangPointSystemUser( iIndex ) == FALSE )
	{
		pMsg.btResult = 0x04;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;
	}
#endif

	// 4. 안전지대인지 검사
	if( CheckPCBangPointShopOpenAttr( lpObj->MapNumber, lpObj->X, lpObj->Y ) == FALSE )
	{
		pMsg.btResult = 0x01;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 5. 상점에 아이템이 없다.
	if( ShopC[m_btPCBangPointShopNum].ItemCount < 1 ) 
	{
		pMsg.btResult = 0x02;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}
		
	// 6. 창고 외 다른 인터페이스를 사용중인 경우
	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != I_PCBANG_POINT_SHOP )
	{
		pMsg.btResult = 0x03;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 7. 창고를 열어 달라는 요청이 이미 진행중이다.
	if( lpObj->m_ReqWarehouseOpen )
	{
		pMsg.btResult = 0x03;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 8. 일단 결과를 보낸다.
	DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
	//return;

	// 9. 여기까지 내려오면 상점 목록을 주는 경우	
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719
	// 아이템 복사 방지 - 상점 목록, 인벤토리 목록, 카오스조합 시작 시
	if( gObjFixInventoryPointer( iIndex ) == false ) 
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

	// 10. 상점의 정보를 보낸다.
	PMSG_SHOPITEMCOUNT	pShopItemCount;
	BYTE				SendByte[1024];
	int					lOfs = 0;

	// 상점 인터페이스 설정
	lpObj->TargetShopNumber = m_btPCBangPointShopNum;
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_PCBANG_POINT_SHOP;
	lpObj->m_ShopTime		= 0;

//	pResult.h.c				= PMHCE_BYTE;
//	pResult.h.headcode		= 0x30;
//	pResult.h.size			= sizeof( pResult );
//	pResult.result			= 0x00;
//
//	// 상점 번호를 보낸다.
//	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	// 상점의 아이템 리스트를 보낸다.
	lOfs += sizeof( pShopItemCount );
	int size = lOfs + ShopC[m_btPCBangPointShopNum].SendItemDataLen;
			
	PHeadSetW( ( LPBYTE )&pShopItemCount, 0x31, size );

	pShopItemCount.Type			= 0;
	pShopItemCount.count		= ShopC[m_btPCBangPointShopNum].ItemCount;
	memcpy( SendByte, &pShopItemCount, sizeof( pShopItemCount ) );
	memcpy( SendByte + lOfs, ShopC[m_btPCBangPointShopNum].SendItemData, ShopC[m_btPCBangPointShopNum].SendItemDataLen );

	DataSend( iIndex, ( LPBYTE )SendByte, size );	
}

//-------------------------------------------------------------------------
// PC방 포인트 상점에서 아이템을 구입
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::BuyPCBangPointItem( int iIndex, BYTE btShopPos )				
{
	// 기존의 상점 아이템 구입 함수 : CGBuyRequestRecv
	// PC방 포인트 아이템은 구입시 세율이 적용되지 않으며 포인트로 계산하기 때문에 
	// 기존의 상점 아이템 구입 함수를 사용할 수 없어 이곳에서 처리한다.

	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];

	PMSG_ANS_GC_BUY_PCBANG_POINT_ITEM pMsg;	
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x05, sizeof( pMsg ) );
	pMsg.btResult		= 0x06;		// 일단 실패로.
	pMsg.btInvenPos		= 0XFF;

	// 1. 종료 중
	if( lpObj->CloseType != -1 )
	{
		return FALSE;
	}

#ifndef PCBANG_KUZIMO
	// 2. PC방 포인트 시스템 대상자인지 검사
	if( CheckPCBangPointSystemUser( iIndex ) == FALSE )
	{
		pMsg.btResult = 0x05;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
#endif

	// 3. 안전지대인지 검사
	if( CheckPCBangPointShopOpenAttr( lpObj->MapNumber, lpObj->X, lpObj->Y ) == FALSE )
	{
		pMsg.btResult = 0x01;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	
	// 4. 핵 사용 방지용 검사
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// 아이템 복사 방지 - NPC상점에서 아이템을 살 때
	if( gObjFixInventoryPointer( iIndex ) == false ) 
	{
		LogAdd( "[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__ );		
	}
#endif

	// 5. 상점을 열 때에는 제한시간이 있다.
	if( gObj[iIndex].m_ShopTime == 0 )
	{
		gObj[iIndex].m_ShopTime = 1; 
	}
	
	// 상점에서 살수 있는 최대 시간을 넘었다.
	if( gObj[iIndex].m_ShopTime > 60 )
	{
		if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_PCBANG_POINT_SHOP )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use    = 0;
			lpObj->m_IfState.type   = 0;
		}

		pMsg.btResult = 0x07;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 6. PC방 포인트 상점 사용자인지 확인
	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != I_PCBANG_POINT_SHOP )
	{		
		pMsg.btResult = 0x05;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	
	// 7. 클라이언트에서 온 정보 유효성 검사
	int iShopNum;
	iShopNum = lpObj->TargetShopNumber;
	
	if( iShopNum < 0 || ( iShopNum > MAX_SHOP-1 ) ) 
	{
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	if( btShopPos > MAX_SHOPITEM - 1 )
	{
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	if( ShopC[iShopNum].m_item[btShopPos].IsItem() == FALSE )
	{
		pMsg.btResult = 0x02;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 8. 아이템 확인
	BOOL IsPCBangPointItem = FALSE;
	DWORD dwItemNum = ShopC[iShopNum].m_item[btShopPos].m_Type;
	int iOptIndex = 0;
	LPPCBANG_POINT_ITEM_OPTION p = &m_ItemOption[0];
	for( int n = 0; n < MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		if( m_ItemOption[n].dwItemType == dwItemNum )
		{
			iOptIndex = n;
			IsPCBangPointItem = TRUE;
		}
	}
	if( !IsPCBangPointItem )
	{
		// PC방 포인트 아이템이 아님
		pMsg.btResult = 0x08;	
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 9. 포인트 확인
	if( lpObj->m_PCBangPointInfo.nPCBangPoint < m_ItemOption[iOptIndex].nBuyingPoint )
	{
		// 포인트가 모자란다.
		pMsg.btResult = 0x09;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	if( lpObj->m_PCBangPointInfo.nPCBangPoint < m_ItemOption[iOptIndex].nConditionPoint )
	{
		// 아이템 구입 가능한 조건에 맞지 않는다.
		pMsg.btResult = 0x04;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 10. 아이템 처리
	if( m_ItemOption[iOptIndex].btAbility == 1 )	// 인장 아이템
	{
		ItemByteConvert( pMsg.ItemInfo, ShopC[iShopNum].m_item[btShopPos] );
		pMsg.btResult = 0x00;
		pMsg.btInvenPos = 0xFF;		// *인장 아이템은 인벤토리에 추가되지 않고 바로 적용된다.
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		// 아이템 코드에 따라 적용되는 버프 아이콘을 바꿔준다.
		int iBuffType = BUFFTYPE_NONE;

		if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 43 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK1;
		}
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 44 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK2;
		}
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 45 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK3;
		}
#ifdef ADD_PCBANG_BUFF_MARK4
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 96 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK4;
		}
#endif // ADD_PCBANG_BUFF_MARK4

		// 상세 속성은 ItemAddOption으로 부터 얻어온다.
		gObjAddBuffEffect( lpObj, iBuffType );
		//gObjAddBuffEffect( lpObj, iBuffType, EFFECTTYPE_EXPERIENCE,  );
#else
		// 옵션 적용
		lpObj->m_iPCBangPointItemTime			= m_ItemOption[iOptIndex].dwKeepTime_Sec;
		lpObj->m_iPCBangPointItemAddExpPer		= m_ItemOption[iOptIndex].nAddExpPercent;		
		lpObj->m_iPCBangPointItemGiveItemPer	= m_ItemOption[iOptIndex].nGetItemPercent;
		lpObj->m_bPCBangPointNoMoreExp			= m_ItemOption[iOptIndex].btNoMoreExp;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		
		// 포인트 차감
		lpObj->m_PCBangPointInfo.nPCBangPoint -= m_ItemOption[iOptIndex].nBuyingPoint;

		// 포인트 정보 업데이트
		if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )	// 이런 일이 발생하면 안된다.
			lpObj->m_PCBangPointInfo.nPCBangPoint = 0;

#ifdef PCBANG_KUZIMO
		GEReqUpdatePCBangPoint2( iIndex );
#else
		GEReqUpdatePCBangPoint( iIndex );
#endif
		GEReqUpdateRankingEvent( iIndex, m_ItemOption[iOptIndex].nBuyingPoint );

		LogAddTD( "[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
			lpObj->AccountID, 
			lpObj->Name, 	
			m_ItemOption[iOptIndex].nBuyingPoint,
			lpObj->m_PCBangPointInfo.nPCBangPoint,
			g_nPCBangMaxPoint,
			iShopNum,
			ShopC[iShopNum].m_item[btShopPos].GetName(),
			m_ItemOption[iOptIndex].dwKeepTime_Sec,
			ShopC[iShopNum].m_item[btShopPos].m_Level,
			(int)ShopC[iShopNum].m_item[btShopPos].m_Durability,
			ShopC[iShopNum].m_item[btShopPos].m_Option1,
			ShopC[iShopNum].m_item[btShopPos].m_Option2,
			ShopC[iShopNum].m_item[btShopPos].m_Option3
			);

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
		ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SEAL_ITEM );	
		GCStateInfoSend( lpObj, 1, STATE_SEAL_ITEM );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004	
	}
	else	// 일반 아이템
	{
		// 아이템을 사용자의 인벤토리에 넣어본다.
		pMsg.btInvenPos = gObjShopBuyInventoryInsertItem( iIndex, ShopC[iShopNum].m_item[btShopPos] );
		if( pMsg.btInvenPos != 0xFF )
		{
			ItemByteConvert( pMsg.ItemInfo, ShopC[iShopNum].m_item[btShopPos] );
			pMsg.btResult = 0x00;
			DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );

			// 포인트 차감
			lpObj->m_PCBangPointInfo.nPCBangPoint -= m_ItemOption[iOptIndex].nBuyingPoint;		
			LogAddTD( "[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
				lpObj->AccountID, 
				lpObj->Name, 	
				m_ItemOption[iOptIndex].nBuyingPoint,
				lpObj->m_PCBangPointInfo.nPCBangPoint,
				g_nPCBangMaxPoint,
				iShopNum,
				ShopC[iShopNum].m_item[btShopPos].GetName(),
				m_ItemOption[iOptIndex].dwKeepTime_Sec,
				ShopC[iShopNum].m_item[btShopPos].m_Level,
				(int)ShopC[iShopNum].m_item[btShopPos].m_Durability,
				ShopC[iShopNum].m_item[btShopPos].m_Option1,
				ShopC[iShopNum].m_item[btShopPos].m_Option2,
				ShopC[iShopNum].m_item[btShopPos].m_Option3
				);


			// 포인트 정보 업데이트
			if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )	// 이런 일이 발생하면 안된다.
				lpObj->m_PCBangPointInfo.nPCBangPoint = 0;

#ifdef PCBANG_KUZIMO
			GEReqUpdatePCBangPoint2( iIndex );
#else
			GEReqUpdatePCBangPoint( iIndex );
#endif
			GEReqUpdateRankingEvent( iIndex, m_ItemOption[iOptIndex].nBuyingPoint );
		}
		else
		{
			// 인벤토리에 빈 공간이 없음
			pMsg.btResult = 0x03;
			DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
			return FALSE;			
		}
	}
	return TRUE;
}

#ifdef PCBANG_KUZIMO
VOID CPCBangPointSystem::WeeklyTimerCallBack(MWeeklyTimer *pkMyDate, VOID *pArg)
{
	WORD objIndex = (WORD)pArg;

		// EventServer에 포인트 정보가 변경되었음을 알린다.
	int iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour;

	return;
//	int iOldPoint;

//	iOldPoint = gObj[objIndex].m_PCBangPointInfo.nPCBangPoint;

	pkMyDate->GetWakeDate(iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour);

	LogAddTD( "[PCBangPointSystem][Point Reset] [%s] - OldData(%04d-%02d-%02d %02d Hour %d Points) ResetDate(%04d-%02d-%02d %02d Hour)", 
		       gObj[objIndex].AccountID, gObj[objIndex].m_PCBangPointInfo.nResetYear, gObj[objIndex].m_PCBangPointInfo.nResetMonth, gObj[objIndex].m_PCBangPointInfo.nResetDay, 
			   gObj[objIndex].m_PCBangPointInfo.nResetHour, gObj[objIndex].m_PCBangPointInfo.nPCBangPoint,
			   iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour);

	gObj[objIndex].m_PCBangPointInfo.nResetYear			= iOutWakeYear;
	gObj[objIndex].m_PCBangPointInfo.nResetMonth		= iOutWakeMonth;
	gObj[objIndex].m_PCBangPointInfo.nResetDay			= iOutWakeDay;
	gObj[objIndex].m_PCBangPointInfo.nResetHour			= iOutWakeHour;

	gObj[objIndex].m_PCBangPointInfo.nPCBangPoint		= 0;
	gObj[objIndex].m_PCBangPointInfo.dwCheckRuleTime	= GetTickCount();

//	if(iOldPoint == 0) return;	// PCBangPoint가 0이면 리셋하지 말자. DB에 무리간다.ㅠ

	g_PCBangPointSystem.GEReqUpdatePCBangPoint2(objIndex);
}

//-------------------------------------------------------------------------
// 이벤트 서버에 PC방 포인트 정보를 요청한다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqPCBangPointInfo2( int iIndex )					
{
	// EventServer에 정보를 요청한다.
	PMSG_REQ_PCBANG_POINT_INFO2 pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x29, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	

#ifdef FOR_TEMP_SERVER
	pMsg.btServerType = ST_TEMPSERVER;
#else
	#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
	#else
		pMsg.btServerType = ST_SERVICESERVER;
	#endif
#endif	

	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );

	LogAddTD( "[PCBangPointSystem][User Connect] [%s] - %s", gObj[iIndex].AccountID, gObj[iIndex].Ip_addr );
}

//-------------------------------------------------------------------------
// PC방 포인트 정보를 이벤트 서버로부터 정보를 받아 셋팅한다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvPCBangPointInfo2( LPBYTE lpMsg )		
{
	LPPMSG_ANS_PCBANG_POINT_INFO2 pMsg = ( LPPMSG_ANS_PCBANG_POINT_INFO2 )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	lpObj->m_PCBangPointInfo.bIsDataLoadSuccess	= TRUE;	
	lpObj->m_PCBangPointInfo.nPCBangPoint	= pMsg->nPoint;
	lpObj->m_PCBangPointInfo.dwCheckRuleTime		= GetTickCount(); // 데이터가 정상적으로 로드된 후 시간을 체크한다.

	lpObj->m_PCBangPointInfo.nResetYear		= pMsg->nResetYear;
	lpObj->m_PCBangPointInfo.nResetMonth	= pMsg->nResetMonth;
	lpObj->m_PCBangPointInfo.nResetDay		= pMsg->nResetDay;
	lpObj->m_PCBangPointInfo.nResetHour		= pMsg->nResetHour;

	lpObj->m_kWeeklyTimer.Init(lpObj->m_PCBangPointInfo.nResetYear, lpObj->m_PCBangPointInfo.nResetMonth, lpObj->m_PCBangPointInfo.nResetDay,
		lpObj->m_PCBangPointInfo.nResetHour, g_nPCBangResetDayOfWeek, g_nPCBangResetHour, WeeklyTimerCallBack, (VOID*)iIndex);
	lpObj->m_kWeeklyTimer.Activate(FALSE);

	if(lpObj->m_kWeeklyTimer.Update() != 2)	// 리셋될때는 클라이언트에 포인트를 보내지 말자.
	{
		SendPCBangPointInfo( iIndex );
	}
}

//-------------------------------------------------------------------------
// 이벤트서버에 PC방 포인트 갱신 요청
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdatePCBangPoint2( int iIndex )
{
	PMSG_REQ_PCBANG_POINT_NEW_INFO2 pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x30, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

#ifdef FOR_TEMP_SERVER
	pMsg.btServerType = ST_TEMPSERVER;
#else
	#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
	#else
		pMsg.btServerType = ST_SERVICESERVER;
	#endif
#endif	

	pMsg.nNewPoint = gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;

	pMsg.nResetYear		= gObj[iIndex].m_PCBangPointInfo.nResetYear;
	pMsg.nResetMonth	= gObj[iIndex].m_PCBangPointInfo.nResetMonth;
	pMsg.nResetDay		= gObj[iIndex].m_PCBangPointInfo.nResetDay;
	pMsg.nResetHour		= gObj[iIndex].m_PCBangPointInfo.nResetHour;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC방 포인트 갱신 요청에 대한 응답을 받았다.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvUpdatePCBangPoint2( LPBYTE lpMsg )
{
	LPPMSG_ANS_PCBANG_POINT_NEW_INFO2 pMsg = ( LPPMSG_ANS_PCBANG_POINT_NEW_INFO2 )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	// 이벤트서버에서 데이터 업데이트에 성공했을 때만 클라이언트에 정보를 보낸다.
	if( pMsg->btResult == TRUE )
	{
		SendPCBangPointInfo( iIndex );
	}
}


#endif

#ifdef ADD_PCBANG_EXPRATE
BOOL CPCBangPointSystem::CheckPCBangAddExperience( LPOBJECTSTRUCT lpObj, INT64 &iExp )
{
	if( !lpObj->m_bPCBangPointUser )
		return FALSE;

	INT64 iRetExp = (int)((float)iExp * g_fPCBangAddExperience);
	iExp = iRetExp;
	
	return TRUE;
}
#endif // ADD_PCBANG_EXPRATE

#endif	// PCBANG_POINT_SYSTEM_20070206
