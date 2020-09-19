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

#include "DragonEvent.h"			// �巡�� �̺�Ʈ
#include "AttackEvent.h"			// ���ϱ��� �̺�Ʈ
#include "EledoradoEvent.h"			// Ȳ�ݱ��� �̺�Ʈ
#include "RingAttackEvent.h"		// ��������� �̺�Ʈ

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#include "XMasAttackEvent.h"		// ũ�������� �̺�Ʈ(��ŸŬ�ν� ����)
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventManagement	g_EventManager;	// �̺�Ʈ ������ ��ü

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

	m_wToday_Year	= 0;			// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)
	m_wToday_Month	= 0;			// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)
	m_wToday_Day	= 0;			// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)
	
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

		// *> . �巡�� �̺�Ʈ
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

		// *> . ���ϱ��� �̺�Ʈ
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

		// *> . Ȳ�ݱ��� �̺�Ʈ
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
		// *> . ��������� �̺�Ʈ
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
		// *> . ũ�������� �̺�Ʈ
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

		// *> . �巡�� �̺�Ʈ
		it = m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		if (it != m_mapEventObj.end()) {
			CDragonEvent * lpDragonEvent = (CDragonEvent *) it->second;
			lpDragonEvent->SetMenualStart(FALSE);
		}

		// *> . ���ϱ��� �̺�Ʈ
		it = m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		if (it != m_mapEventObj.end()) {
			CAttackEvent * lpAttackEvent = (CAttackEvent *) it->second;
			lpAttackEvent->SetMenualStart(FALSE);
		}

		// *> . Ȳ�ݱ��� �̺�Ʈ
		it = m_mapEventObj.find(EVENT_ID_ELDORADO);
		if (it != m_mapEventObj.end()) {
			CEledoradoEvent * lpEldoradoEvent = (CEledoradoEvent *) it->second;
			lpEldoradoEvent->SetMenualStart(FALSE);
		}

#ifdef MONSTER_HERD_SYSTEM_20031120
		// *> . ��������� �̺�Ʈ
		it = m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		if (it != m_mapEventObj.end()) {
			CRingAttackEvent * lpRingAttackEvent = (CRingAttackEvent *) it->second;
			lpRingAttackEvent->SetMenualStart(FALSE);
		}
#endif

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
		// *> . ũ�������� �̺�Ʈ
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

	// ��¥�� ����Ǿ����� üũ�Ͽ� ������ �̺�Ʈ ���� �����͸� �ʱ�ȭ ���ش�.
	if (m_wToday_Year != sysTime.wYear ||
		m_wToday_Month != sysTime.wMonth ||
		m_wToday_Day != sysTime.wDay
		)
	{
		it = m_vtEventTime.begin();

		for (;it != m_vtEventTime.end();it++) {
			(*it).m_bEventStarted = false;
		}

		LogAddTD("�ۡ�[Event Management] ��¥ �����. %02d %02d %02d", m_wToday_Year, m_wToday_Month, m_wToday_Day);
		
		m_wToday_Year	= sysTime.wYear;
		m_wToday_Month	= sysTime.wMonth;
		m_wToday_Day	= sysTime.wDay;
	}

	// �� �̺�Ʈ�� �ð��� üũ�Ͽ� ������ ��� �����Ѵ�.
	it = m_vtEventTime.begin();

	for (;it != m_vtEventTime.end();it++) {
		
		if (sysTime.wHour == (*it).m_iHour && sysTime.wMinute == (*it).m_iMinute && (*it).m_bEventStarted == false) {
			// Ư�� �̺�Ʈ�� �� �ð��� �Ǿ��ٸ� ��� �����Ѵ�.
			LogAddTD("�ۡ�[Event Management] �̺�Ʈ������. (%d) %02d %02d (state=%d)",
				(*it).m_eEventKind, (*it).m_iHour, sysTime.wMinute, (*it).m_bEventStarted );

			(*it).m_bEventStarted = true;
			StartEvent ((*it).m_eEventKind);
		}
	}
}


void CEventManagement::StartEvent (int eEventKind)
{
	switch (eEventKind) {
	case EVENT_ID_DRAGONHERD :		// �巡�ﱺ��
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_DRAGONHERD);
			if (it != m_mapEventObj.end()) {
				CDragonEvent * lpDragonEvent = (CDragonEvent *) it->second;
				lpDragonEvent->Start_Menual();
			}
		}
		break;
	case EVENT_ID_UNDERTROOP :		// ���ϱ���
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_UNDERTROOP);
			if (it != m_mapEventObj.end()) {
				CAttackEvent * lpAttackEvent = (CAttackEvent *) it->second;
				lpAttackEvent->Start_Menual();
			}
		}
		break;
	case EVENT_ID_ELDORADO :		// Ȳ�ݱ���
		{
			map<int, LPVOID>::iterator it = m_mapEventObj.find(EVENT_ID_ELDORADO);
			if (it != m_mapEventObj.end()) {
				CEledoradoEvent * lpEldoradoEvent = (CEledoradoEvent *) it->second;
				lpEldoradoEvent->Start_Menual();
			}
		}
		break;
#ifdef MONSTER_HERD_SYSTEM_20031120
	case EVENT_ID_WHITEMAGE :		// ��������籺��
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
	case EVENT_ID_SANTACLAUS :		// ��ŸŬ�ν� ����
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


bool CEventManagement::Load (LPSTR lpszFileName)		// �̺�Ʈ ���� �ε�
{
	m_bHasData = false;

	Clear();
	
	// *> . ��ũ��Ʈ�� �о �ʿ��� �����͸� �����Ѵ�.
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
			{	// �̺�Ʈ�� ������ �ð������� �� ����
				EVENT_ID_TIME pDET;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				pDET.m_eEventKind						= (int)TokenNumber;		// �̺�Ʈ ����
				Token = (*GetToken)(); pDET.m_iHour		= (int)TokenNumber;		// �̺�Ʈ ���� ��
				Token = (*GetToken)(); pDET.m_iMinute	= (int)TokenNumber;		// �̺�Ʈ ���� ��

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


void CEventManagement::RegEvent (int eEventKind, LPVOID lpEventObj)	// �̺�Ʈ ������ ��ü�� ����Ѵ�.
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

	// *> . ��ũ��Ʈ�� �о �ʿ��� �����͸� �����Ѵ�.
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