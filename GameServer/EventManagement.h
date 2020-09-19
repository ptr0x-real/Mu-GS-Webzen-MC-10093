// EventManagement.h: interface for the CEventManagement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENT_IDMANAGE_H__D4313EF1_8B9F_4E73_B1BE_23A1F6E9CAAE__INCLUDED_)
#define AFX_EVENT_IDMANAGE_H__D4313EF1_8B9F_4E73_B1BE_23A1F6E9CAAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef EVENT_MANAGEMENT_20040711


#pragma warning (disable : 4786)

#include <VECTOR>
#include <MAP>

using namespace std;


typedef struct 
{
	int		m_eEventKind;					// 이벤트의 종류
	int		m_iHour;						// 이벤트가 시작할 시간
	int		m_iMinute;						// 이벤트가 시작할 분
	bool	m_bEventStarted;				// 그날의 이벤트가 시작되었는지를 확인
} EVENT_ID_TIME, * LPEVENT_ID_TIME;


class CEventManagement  
{
	BOOL	m_bEventStart;
	BOOL	m_bHasData;

	vector<EVENT_ID_TIME>					m_vtEventTime;	// 각 이벤트가 시작할 시간을 가지고 있음
	map<int, LPVOID>						m_mapEventObj;	// 각 이벤트의 종류에 따라 사용할 수 있는 객체들을 가지고 있음

	WORD	m_wToday_Year;					// 날짜가 넘어갔는지 체크하는 변수 (년)
	WORD	m_wToday_Month;					// 날짜가 넘어갔는지 체크하는 변수 (월)
	WORD	m_wToday_Day;					// 날짜가 넘어갔는지 체크하는 변수 (일)

public:
	enum {
		EVENT_ID_DRAGONHERD		= 0,		// 드래곤군단
		EVENT_ID_UNDERTROOP		= 1,		// 지하군단
		EVENT_ID_ELDORADO		= 2,		// 황금군단
		EVENT_ID_WHITEMAGE		= 3,		// 백색마법사군단
#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
		EVENT_ID_SANTACLAUS		= 4,		// 산타클로스 해외 크리스마스 이벤트
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014		
	};

			CEventManagement();
	virtual	~CEventManagement();

	void	Clear ();						// 데이터 초기화
	void	Init (BOOL bEventStart);		// 이벤트 활성화/비활성화
	bool	Load (LPSTR lpszFileName);		// 이벤트 정보 로드
	void	Run ();							// 이벤트 진행 메서드
	void	StartEvent (int eEventKind);	// 특정 이벤트를 즉시 시작하는 메서드
	void	RegEvent (int eEventKind, LPVOID lpEventObj);	// 이벤트 종류와 객체를 등록한다.
};


extern CEventManagement	g_EventManager;		// 이벤트 관리자 객체



#endif

#ifdef COUPON_EVENT_ITEMLIST_20081022

typedef struct 
{
	BYTE		m_btIndex;
	BYTE		m_btItemCodeType;
	BYTE		m_btItemCodeIndex;
	BYTE		m_btItemLevel;
	BYTE		m_btItemDur;
	
} COUPON_EVENT_ITEM, * LPCOUPON_EVENT_ITEM;

class CouponEventItem
{
	vector<COUPON_EVENT_ITEM>					m_vtCoupon_Event_Item;
	
public:
	CouponEventItem();
	~CouponEventItem();
	
	BOOL	Load(LPSTR lpszFileName);
	INT		GetItemNum(BYTE btItemIndex, BYTE &btItemLevel, BYTE &btItemDur);
	
private:
	
};

extern	CouponEventItem	g_CouponEventItemLIst;

#endif //COUPON_EVENT_ITEMLIST_20081022



#endif		// !defined(AFX_EVENT_IDMANAGE_H__D4313EF1_8B9F_4E73_B1BE_23A1F6E9CAAE__INCLUDED_)

