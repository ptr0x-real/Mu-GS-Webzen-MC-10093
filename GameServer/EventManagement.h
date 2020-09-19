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
	int		m_eEventKind;					// �̺�Ʈ�� ����
	int		m_iHour;						// �̺�Ʈ�� ������ �ð�
	int		m_iMinute;						// �̺�Ʈ�� ������ ��
	bool	m_bEventStarted;				// �׳��� �̺�Ʈ�� ���۵Ǿ������� Ȯ��
} EVENT_ID_TIME, * LPEVENT_ID_TIME;


class CEventManagement  
{
	BOOL	m_bEventStart;
	BOOL	m_bHasData;

	vector<EVENT_ID_TIME>					m_vtEventTime;	// �� �̺�Ʈ�� ������ �ð��� ������ ����
	map<int, LPVOID>						m_mapEventObj;	// �� �̺�Ʈ�� ������ ���� ����� �� �ִ� ��ü���� ������ ����

	WORD	m_wToday_Year;					// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)
	WORD	m_wToday_Month;					// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)
	WORD	m_wToday_Day;					// ��¥�� �Ѿ���� üũ�ϴ� ���� (��)

public:
	enum {
		EVENT_ID_DRAGONHERD		= 0,		// �巡�ﱺ��
		EVENT_ID_UNDERTROOP		= 1,		// ���ϱ���
		EVENT_ID_ELDORADO		= 2,		// Ȳ�ݱ���
		EVENT_ID_WHITEMAGE		= 3,		// ��������籺��
#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
		EVENT_ID_SANTACLAUS		= 4,		// ��ŸŬ�ν� �ؿ� ũ�������� �̺�Ʈ
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014		
	};

			CEventManagement();
	virtual	~CEventManagement();

	void	Clear ();						// ������ �ʱ�ȭ
	void	Init (BOOL bEventStart);		// �̺�Ʈ Ȱ��ȭ/��Ȱ��ȭ
	bool	Load (LPSTR lpszFileName);		// �̺�Ʈ ���� �ε�
	void	Run ();							// �̺�Ʈ ���� �޼���
	void	StartEvent (int eEventKind);	// Ư�� �̺�Ʈ�� ��� �����ϴ� �޼���
	void	RegEvent (int eEventKind, LPVOID lpEventObj);	// �̺�Ʈ ������ ��ü�� ����Ѵ�.
};


extern CEventManagement	g_EventManager;		// �̺�Ʈ ������ ��ü



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

