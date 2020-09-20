// PCBangPointSystem.h: interface for the PCBangPointSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_)
#define AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef PCBANG_POINT_SYSTEM_20070206
// **PC방포인트시스템 전용아이템의 규모가 커진다면 아이템 및 옵션처리를 분리하여 처리 할 것.(RKFKA)


// 규칙에 맞추어 인덱스를 부여한다.
enum PCBANG_POINT_RULE_TIME_TYPE
{
	PG_RULE_TYPE_FIRST		= 0,	// 최초 규칙 시간		
	PG_RULE_TYPE_COMMON		= 1,	// 일반 규칙 시간
};

// PC방 포인트 시스템 적용 대상
enum PCBANG_POINT_APPLICATION_LEVEL
{
	PP_APPLICATION_OF_NONE				= 0,	// 적용하지 않는다.
	PP_PCBANG_USER_ONLY					= 1,	// PC방 유저에게 적용한다.
	PP_APPLICATION_OF_ALL				= 2,	// 모든 유저에 대해 적용한다.
												// (기본적으로 PC방 유저에게 적용되는 시스템이지만 확장성을 고려한다.)
};

#ifdef PCBANG_KUZIMO

extern short g_nPCBangPointFirstRuleTime;			// 최초 규칙 시간 단위(60초 * 10 = 10분) 
extern short g_nPCBangPointFirstRuleTimePoint;		// 최초 규칙 시간을 만족했을 때 부여 포인트
extern short g_nPCBangPointCommonRuleTime;			// 일반 규칙 시간 단위(60초 * 10 = 10분) 
extern short g_nPCBangPointCommonRuleTimePoint;	// 일반 규칙 시간을 만족했을 때 부여 포인트
extern short g_nPCBangMaxPoint;					// 최대 적립 가능한 포인트

// FeverTime 기간에는 (평소 부여 포인트 * g_nPCBangPointFeverMultipleValue)포인트가 적립된다.
// g_nPCBangPointFeverStartHour와 g_nPCBangPointFeverEndHour가 같으면 동작하지 않는다.
// g_nPCBangPointFeverStartHour = 4, g_nPCBangPointFeverEndHour = 5라면 4시부터 작동하며, 5시가 되면 작동 해제된다.
extern short g_nPCBangPointFeverStartHour;			// 시간은 0시부터 24시까지 넣을 수 있다. 0시, 24시는 동일하다.
extern short g_nPCBangPointFeverEndHour;			//
extern short g_nPCBangPointFeverMultipleValue;

extern short g_nPCBangResetDayOfWeek;				// 리셋 요일을 정한다. 0(일), 1(월)... 
extern short g_nPCBangResetHour;		// 리셋 시간. 보통 8시부터 10시까지 점검이다. 부하를 덜기 위해서 9시로 설정한다.

#else

#ifdef MODIFY_PCBANG_POINT_TIME_EVENT_20070709
const short g_nPCBangPointFirstRuleTime			= 900;		// 최초 규칙 시간 단위	
const short g_nPCBangPointFirstRuleTimePoint	= 3;		// 최초 규칙 시간을 만족했을 때 부여 포인트
const short g_nPCBangPointCommonRuleTime		= 300;		// 일반 규칙 시간 단위
const short g_nPCBangPointCommonRuleTimePoint	= 1;		// 일반 규칙 시간을 만족했을 때 부여 포인트
const short g_nPCBangMaxPoint					= 1008;		// 최대 적립 가능한 포인트
#else
const short g_nPCBangPointFirstRuleTime			= 1800;		// 최초 규칙 시간 단위	
const short g_nPCBangPointFirstRuleTimePoint	= 3;		// 최초 규칙 시간을 만족했을 때 부여 포인트
const short g_nPCBangPointCommonRuleTime		= 600;		// 일반 규칙 시간 단위
const short g_nPCBangPointCommonRuleTimePoint	= 1;		// 일반 규칙 시간을 만족했을 때 부여 포인트
const short g_nPCBangMaxPoint					= 1008;		// 최대 적립 가능한 포인트
#endif	// MODIFY_PCBANG_POINT_TIME_EVENT_20070709

#endif

#ifdef ADD_PCBANG_EXPRATE
extern float g_fPCBangAddExperience;
#endif // ADD_PCBANG_EXPRATE



const short MAX_PCBANG_POINT_ITEM_COUNT			= 6;		// PC방 포인트 아이템 수
typedef struct 
{
	//BOOL	bPCBangPointUser;			// PC방 포인트 시스템 유저인가? 0:아니다/1:그렇다
	BOOL	bIsDataLoadSuccess;			// 이벤트서버로부터 데이터를 정상적으로 전달받았나? 0:아니다/1:그렇다

	BYTE	btCurrentRule;				// 현재 유저에게 적용되는 규칙
	short	nCurrentAppRuleTime;		// 유저에게 적용되고 있는 규칙 시간
	short	nCurrentAppRulePoint;		// 유저에게 적용할 포인트

	int		nPCBangPoint;				// 현재 포인트
	BOOL	bIsSuccessFirstRuleTime;	// 최초 규칙 시간을 경과했나? 0:아니다/1:그렇다
	
	DWORD	dwCheckRuleTime;			// 사용시간(지정된 규칙시간이 지나면 갱신한다.)	

#ifdef PCBANG_KUZIMO
	short	nResetYear;
	short	nResetMonth;
	short	nResetDay;
	short	nResetHour;
#endif
	
	VOID Clear()						// 데이터 초기화
	{
		bIsDataLoadSuccess			= FALSE;
		btCurrentRule				= PG_RULE_TYPE_FIRST;	
		nCurrentAppRuleTime			= g_nPCBangPointFirstRuleTime;
		nCurrentAppRulePoint		= g_nPCBangPointFirstRuleTimePoint;
		nPCBangPoint				= 0;			
		bIsSuccessFirstRuleTime		= FALSE;	
		dwCheckRuleTime				= 0;			
#ifdef PCBANG_KUZIMO
		nResetYear					= 2001;
		nResetMonth					= 1;
		nResetDay					= 1;
		nResetHour					= 0;
#endif
	}
} PCBANG_POINT_INFO, *LPPCBANG_POINT_INFO;

// PC방 포인트용 아이템 옵션
typedef struct {
#ifdef MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904	// int형으로 모자른 경우 타입확장 변경 필요
	int		dwItemType;					// 아이템 타입
#else // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	DWORD	dwItemType;					// 아이템 타입
#endif // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	short	nBuyingPoint;				// 구입 포인트
	short	nConditionPoint;			// 구입 조건. 이 이상의 포인트가 있어야 구입할 수 있다.
	BYTE	btAbility;					// 아이템의 기능 0:일반아이템 1:버프형
	DWORD	dwKeepTime_Sec;				// 지속시간(초단위)
	short	nAddExpPercent;				// 경험치 상승률
	short	nGetItemPercent;			// 아이템 획득 확률
	BYTE	btNoMoreExp;				// 경험치 증가 막음
} PCBANG_POINT_ITEM_OPTION, *LPPCBANG_POINT_ITEM_OPTION;


class CPCBangPointSystem  
{
private:
	BOOL	m_bFileDataLoad;
	BOOL	m_bPCBangPointSystemOn;		// 시스템 On/Off
	BOOL	m_bPCBangPointRankingEvent;	// 누적 포인트 랭킹 이벤트 On/Off

	BYTE	m_btPCBangPointShopNum;		// PC방 포인트 상점 번호

	PCBANG_POINT_ITEM_OPTION	m_ItemOption[MAX_PCBANG_POINT_ITEM_COUNT + 1];	// PC방 포인트 전용 아이템 옵션

	short	m_nBoxOfGreenChaosItemUsingPoint;

public:
	CPCBangPointSystem();
	virtual ~CPCBangPointSystem();

	// Init
	VOID	InitItemOption();									// 초기화

	// Load
	BOOL	LoadData( LPSTR lpszFileName );						// PC방 포인트 시스템 관련 정보를 읽는다.

	// Check
	BOOL	CheckPCBangPointSystemUser( int iIndex );			// PC방 포인트 시스템 해당 유저인가?
	BOOL	CheckPCBangPointItem( DWORD dwItemNum );			// PC방 포인트용 아이템인가?	
	BOOL	CheckPCBangPointShopOpenAttr( BYTE btMapNum, short X, short Y );	// PC방 포인트 상점을 열 수 있는 좌표인가?

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC	
	VOID	CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp );
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#ifdef ADD_PCBANG_EXPRATE
	BOOL	CheckPCBangAddExperience( LPOBJECTSTRUCT lpObj, INT64 &iExp );
#endif // ADD_PCBANG_EXPRATE

	// Set Info
	VOID	SetPCBangPointUser( int iIndex, BOOL bIsPCBangUser );	// PC방 유저 설정하기

	// Run
	BOOL	Run( LPOBJECTSTRUCT lpObj );						// PC방 포인트를 추가한다.

	// GS <-> ES
	VOID	GEReqPCBangPointInfo( int iIndex );					// 이벤트서버에 PC방 포인트 정보를 요청한다.
	VOID	EGRecvPCBangPointInfo( LPBYTE lpMsg );				// 이벤트 서버로부터 정보를 받았다.

	VOID	GEReqUpdatePCBangPoint( int iIndex );					// PC방 포인트를 갱신한다.
	VOID	EGRecvUpdatePCBangPoint( LPBYTE lpMsg );				// PC방 포인트 갱신 요청에 대한 응답을 받았다.

#ifdef PCBANG_KUZIMO
	VOID	GEReqPCBangPointInfo2( int iIndex );				
	VOID	EGRecvPCBangPointInfo2( LPBYTE lpMsg );		
	VOID	GEReqUpdatePCBangPoint2( int iIndex );
	VOID	EGRecvUpdatePCBangPoint2( LPBYTE lpMsg );
#endif

	VOID	GEReqUpdateRankingEvent( int iIndex, short nUsePoint );	// Ranking Event

	// Client <-> GS
	VOID	SendPCBangPointInfo( int iIndex );					// 클라이언트로 PC방 포인트를 보낸다.
	VOID	PCBangPointShopOpen( int iIndex );					// PC방 포인트 상점의 정보를 보낸다.
	BOOL	BuyPCBangPointItem( int iIndex, BYTE btShopPos );	// PC방 포인트 상점에서 아이템을 구입

#ifdef PCBANG_KUZIMO
	static VOID WeeklyTimerCallBack(MWeeklyTimer *pkMyDate, VOID *pArg);	// 해당 주마다 리셋 조건시에 콜백된다.
#endif
};

extern CPCBangPointSystem g_PCBangPointSystem;

#endif // PCBANG_POINT_SYSTEM_20070206

#endif // !defined(AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_)
