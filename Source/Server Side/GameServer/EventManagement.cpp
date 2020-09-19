// EVENT_IDManage.cpp: implementation of the EVENT_IDManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventManagement.h"

#ifdef EVENT_MANAGEMENT_20040711

#include "Include\ReadScript.h"
#include "Include\Prodef.h"
#include "Include\cb_protocol.h"
#include "Common\Winutil.h"
#include "GameMain.h"
#include "Protocol.h"
#include "Event.h"

#include "DragonEvent.h"			// 드래곤 이벤트
#include "AttackEvent.h"			// 지하군단 이벤트
#include "EledoradoEvent.h"			// 황금군단 이벤트
#include "RingAttackEvent.h"		// 백색마법사 이벤트

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "XMasAttackEvent.h"		// 크리스마스 이벤트(산타클로스 군단)
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventManagement	g_EventManager;	// 이벤트 관리자 객체

#ifdef COUPON_EVENT_ITEMLIST_20081022
CouponEventItem	g_CouponEventItemLIst;
#endif


CEventManagement::CEventManagement()
{
	m_bEventStart	= false;
	m_bHasData		= false;

	Clear();
}


CEventManagement::~CEventManagement()
{

}


void CEventManagement::Clear ()
{
	m_vtEventTime.clear();
	m_mapEventObj.clear();

	m_wToday_Year	= 0;			// 날짜가 넘어갔는지 체크하는 변수 (년)
	m_wToday_Month	= 0;			// 날짜가 넘어갔는지 체크하는 변수 (월)
	m_wToday_Day	= 0;			// 날짜가 넘어갔는지 체크하는 변수 (일)
	
}


void CEventManagement::Init (BOOL bEventStart)
{
	m_bEventStart = bEventStart;
	
	if (m_bHasData && m_bEventStart) {
		SYSTEMTIME sysTime;
		GetLocalTime (&sysTime);

		m_wToday_Year	= sysTime.wYear;
		m_wToday_Month	= sysTime.wMonth;
		m_wToday_Day	= sysTime.wDay;

		map<int, LPVOID>::iterator it;

		// *> . 드래곤 이벤트
		it = m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		if (it != m_mapEventObj.end()) {
			CDragonEvent * lpDragonEvent = (CDragonEvent *) it->second;
			lpDragonEvent->SetMenualStart(FALSE);

			vector<EVENT_ID_TIME>::iterator it2 =  m_vtEventTime.begin();
			for (; it2 != m_vtEventTime.end() ; it2++) {
				if ((*it2).m_eEventKind == EVENT_ID_DRAGONHERD)
					lpDragonEvent->SetMenualStart(TRUE);
			}
		}

		// *> . 지하군단 이벤트
		it = m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		if (it != m_mapEventObj.end()) {
			CAttackEvent * lpAttackEvent = (CAttackEvent *) it->second;
			lpAttackEvent->SetMenualStart(FALSE);

			vector<EVENT_ID_TIME>::iterator it2 =  m_vtEventTime.begin();
			for (; it2 != m_vtEventTime.end() ; it2++) {
				if ((*it2).m_eEventKind == EVENT_ID_UNDERTROOP)
					lpAttackEvent->SetMenualStart(TRUE);
			}
		}

		// *> . 황금군단 이벤트
		it = m_mapEventObj.find(EVENT_ID_ELDORADO);
		if (it != m_mapEventObj.end()) {
			CEledoradoEvent * lpEldoradoEvent = (CEledoradoEvent *) it->second;
			lpEldoradoEvent->SetMenualStart(FALSE);

			vector<EVENT_ID_TIME>::iterator it2 =  m_vtEventTime.begin();
			for (; it2 != m_vtEventTime.end() ; it2++) {
				if ((*it2).m_eEventKind == EVENT_ID_ELDORADO)
					lpEldoradoEvent->SetMenualStart(TRUE);
			}
		}

#ifdef MONSTER_HERD_SYSTEM_20031120
		// *> . 백색마법사 이벤트
		it = m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		if (it != m_mapEventObj.end()) {
			CRingAttackEvent * lpRingAttackEvent = (CRingAttackEvent *) it->second;
			lpRingAttackEvent->SetMenualStart(FALSE);

			vector<EVENT_ID_TIME>::iterator it2 =  m_vtEventTime.begin();
			for (; it2 != m_vtEventTime.end() ; it2++) {
				if ((*it2).m_eEventKind == EVENT_ID_WHITEMAGE)
					lpRingAttackEvent->SetMenualStart(TRUE);
			}
		}
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
		// *> . 크리스마스 이벤트
		it = m_mapEventObj.find(EVENT_ID_SANTACLAUS);
		if (it != m_mapEventObj.end()) {
			CBaseAttackEvent* lpAttackEvent = (CBaseAttackEvent*)it->second;
			lpAttackEvent->SetMenualStart(FALSE);
			
			vector<EVENT_ID_TIME>::iterator it2 =  m_vtEventTime.begin();
			for (; it2 != m_vtEventTime.end() ; it2++) {
				if ((*it2).m_eEventKind == EVENT_ID_SANTACLAUS)
					lpAttackEvent->SetMenualStart(TRUE);
			}
		}
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014
	}
	else {
		map<int, LPVOID>::iterator it;

		// *> . 드래곤 이벤트
		it = m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		if (it != m_mapEventObj.end()) {
			CDragonEvent * lpDragonEvent = (CDragonEvent *) it->second;
			lpDragonEvent->SetMenualStart(FALSE);
		}

		// *> . 지하군단 이벤트
		it = m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		if (it != m_mapEventObj.end()) {
			CAttackEvent * lpAttackEvent = (CAttackEvent *) it->second;
			lpAttackEvent->SetMenualStart(FALSE);
		}

		// *> . 황금군단 이벤트
		it = m_mapEventObj.find(EVENT_ID_ELDORADO);
		if (it != m_mapEventObj.end()) {
			CEledoradoEvent * lpEldoradoEvent = (CEledoradoEvent *) it->second;
			lpEldoradoEvent->SetMenualStart(FALSE);
		}

#ifdef MONSTER_HERD_SYSTEM_20031120
		// *> . 백색마법사 이벤트
		it = m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		if (it != m_mapEventObj.end()) {
			CRingAttackEvent * lpRingAttackEvent = (CRingAttackEvent *) it->second;
			lpRingAttackEvent->SetMenualStart(FALSE);
		}
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
		// *> . 크리스마스 이벤트
		it = m_mapEventObj.find(EVENT_ID_SANTACLAUS);
		if (it != m_mapEventObj.end()) {
			CBaseAttackEvent* lpAttackEvent = (CBaseAttackEvent*)it->second;
			lpAttackEvent->SetMenualStart(FALSE);
		}
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014
	}

}


void CEventManagement::Run()
{
	if (!m_bEventStart || !m_bHasData || m_vtEventTime.empty())
		return;

	SYSTEMTIME sysTime;
	GetLocalTime (&sysTime);

	vector <EVENT_ID_TIME>::iterator it;

	// 날짜가 변경되었는지 체크하여 맞으면 이벤트 시작 데이터를 초기화 해준다.
	if (m_wToday_Year != sysTime.wYear ||
		m_wToday_Month != sysTime.wMonth ||
		m_wToday_Day != sysTime.wDay
		)
	{
		it = m_vtEventTime.begin();

		for (;it != m_vtEventTime.end();it++) {
			(*it).m_bEventStarted = false;
		}

		LogAddTD("○●[Event Management] 날짜 변경됨. %02d %02d %02d", m_wToday_Year, m_wToday_Month, m_wToday_Day);
		
		m_wToday_Year	= sysTime.wYear;
		m_wToday_Month	= sysTime.wMonth;
		m_wToday_Day	= sysTime.wDay;
	}

	// 각 이벤트의 시간을 체크하여 맞으면 즉시 시작한다.
	it = m_vtEventTime.begin();

	for (;it != m_vtEventTime.end();it++) {
		
		if (sysTime.wHour == (*it).m_iHour && sysTime.wMinute == (*it).m_iMinute && (*it).m_bEventStarted == false) {
			// 특정 이벤트를 할 시간이 되었다면 즉시 시작한다.
			LogAddTD("○●[Event Management] 이벤트시작함. (%d) %02d %02d (state=%d)",
				(*it).m_eEventKind, (*it).m_iHour, sysTime.wMinute, (*it).m_bEventStarted );

			(*it).m_bEventStarted = true;
			StartEvent ((*it).m_eEventKind);
		}
	}
}


void CEventManagement::StartEvent (int eEventKind)
{
	switch (eEventKind) {
	case EVENT_ID_DRAGONHERD :		// 드래곤군단
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_DRAGONHERD);
			if (it != m_mapEventObj.end()) {
				CDragonEvent * lpDragonEvent = (CDragonEvent *) it->second;
				lpDragonEvent->Start_Menual();
			}
		}
		break;
	case EVENT_ID_UNDERTROOP :		// 지하군단
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_UNDERTROOP);
			if (it != m_mapEventObj.end()) {
				CAttackEvent * lpAttackEvent = (CAttackEvent *) it->second;
				lpAttackEvent->Start_Menual();
			}
		}
		break;
	case EVENT_ID_ELDORADO :		// 황금군단
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_ELDORADO);
			if (it != m_mapEventObj.end()) {
				CEledoradoEvent * lpEldoradoEvent = (CEledoradoEvent *) it->second;
				lpEldoradoEvent->Start_Menual();
			}
		}
		break;
#ifdef MONSTER_HERD_SYSTEM_20031120
	case EVENT_ID_WHITEMAGE :		// 백색마법사군단
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_WHITEMAGE);
			if (it != m_mapEventObj.end()) {
				CRingAttackEvent * lpRingAttackEvent = (CRingAttackEvent *) it->second;
				lpRingAttackEvent->Start_Menual();
			}
		}
		break;
#endif
#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
	case EVENT_ID_SANTACLAUS :		// 산타클로스 군단
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_SANTACLAUS);
			if (it != m_mapEventObj.end()) {
				CBaseAttackEvent* lpAttackEvent = (CBaseAttackEvent*)it->second;
				lpAttackEvent->Start_Menual();
			}
		}
		break;
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014
	}
}


bool CEventManagement::Load (LPSTR lpszFileName)		// 이벤트 정보 로드
{
	m_bHasData = false;

	Clear();
	
	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
	{
		MsgBox("[DOTH] Info file Load Fail [%s]", lpszFileName);
		return false;
	}

	SMDToken Token;
	
	int type = -1;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// 이벤트가 시작할 시간정보를 얻어서 저장
				EVENT_ID_TIME pDET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pDET.m_eEventKind						= (int)TokenNumber;		// 이벤트 종류
				Token = (*GetToken)(); pDET.m_iHour		= (int)TokenNumber;		// 이벤트 시작 분
				Token = (*GetToken)(); pDET.m_iMinute	= (int)TokenNumber;		// 이벤트 시작 분

				pDET.m_bEventStarted = false;
				
				m_vtEventTime.push_back(pDET);
			}
		}
	}
	fclose(SMDFile);

	LogAdd("[DOTH] %s file load!", lpszFileName);

	m_bHasData = true;

	return true;
}


void CEventManagement::RegEvent (int eEventKind, LPVOID lpEventObj)	// 이벤트 종류와 객체를 등록한다.
{
	m_mapEventObj.insert(pair<int, LPVOID> (eEventKind, lpEventObj));
}


#endif

#ifdef COUPON_EVENT_ITEMLIST_20081022

CouponEventItem::CouponEventItem()
{
	
}

CouponEventItem::~CouponEventItem()
{
	m_vtCoupon_Event_Item.clear();
}

BOOL	CouponEventItem::Load(LPSTR lpszFileName)
{
	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		LogAddTD("[EventItemList.txt] - File load error : File Name Error");
		return FALSE;
	}

	// *> . 스크립트를 읽어서 필요한 데이터를 저장한다.
	if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
	{
		MsgBox("[DOTH] Info file Load Fail [%s]", lpszFileName);
		return false;
	}
	
	SMDToken Token;
	
	int type = -1;
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{
				COUPON_EVENT_ITEM pCoupon_Event_Item;
				
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
				
				pCoupon_Event_Item.m_btIndex			= (BYTE)TokenNumber;		
				Token = (*GetToken)(); 
				pCoupon_Event_Item.m_btItemCodeType		= (BYTE)TokenNumber;		
				Token = (*GetToken)(); 
				pCoupon_Event_Item.m_btItemCodeIndex	= (BYTE)TokenNumber;
				Token = (*GetToken)(); 
				pCoupon_Event_Item.m_btItemLevel		= (BYTE)TokenNumber;
				Token = (*GetToken)(); 
				pCoupon_Event_Item.m_btItemDur			= (BYTE)TokenNumber;
				
				m_vtCoupon_Event_Item.push_back(pCoupon_Event_Item);
			}
		}
	}
	fclose(SMDFile);
	
	return TRUE;
}

INT	CouponEventItem::GetItemNum(BYTE btItemIndex, BYTE &btItemLevel, BYTE &btItemDur)
{
	vector<COUPON_EVENT_ITEM>::iterator it2 =  m_vtCoupon_Event_Item.begin();
	
	for (; it2 != m_vtCoupon_Event_Item.end() ; it2++)
	{
		if ((*it2).m_btIndex == btItemIndex)
		{
			BYTE btItemCodeIndex	=	(*it2).m_btItemCodeIndex;
			BYTE btItemCodeType		=	(*it2).m_btItemCodeType;
			btItemLevel				=	(*it2).m_btItemLevel;
			btItemDur				=	(*it2).m_btItemDur;
			
			return ItemGetNumberMake(btItemCodeType, btItemCodeIndex);
		}
	}
	
	return -1;
}

#endif //COUPON_EVENT_ITEMLIST_20081022