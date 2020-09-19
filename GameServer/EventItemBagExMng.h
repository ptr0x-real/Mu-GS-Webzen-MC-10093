// EventItemBagExMng.h: interface for the CEventItemBagExMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTITEMBAGEXMNG_H__A2825D36_023E_4482_8788_F53CD72619DD__INCLUDED_)
#define AFX_EVENTITEMBAGEXMNG_H__A2825D36_023E_4482_8788_F53CD72619DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

#include "CashLotterySystem.h"

#define MAX_ITEMBAGEX_NAME_LENGTH		20
#define MAX_ITEMBAGEX_SIZE				5

typedef struct
{
	bool				bIsUsed;
	char				ItembagName[MAX_ITEMBAGEX_NAME_LENGTH];
	CCashLotterySystem*	lpItembag;
}ITEMBAGEX_LIST, *LPITEMBAGEX_LIST;

class CEventItemBagExMng  
{
public:
	CEventItemBagExMng();
	virtual ~CEventItemBagExMng();

	void Initialize();

	bool AddEventItemBagEx(char* pszItemBagName, char* pszItemBagFileName);
	CCashLotterySystem* GetItemBag(char* pszItemBagName);

	int GetItem(char* pszItemBagName, CItem* lpItemInfo);
private:
	ITEMBAGEX_LIST ItemBagExList[MAX_ITEMBAGEX_SIZE];
};

extern CEventItemBagExMng g_EventItemBagExMng;
extern bool g_bUseLotterySystem;

#endif // MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

#endif // !defined(AFX_EVENTITEMBAGEXMNG_H__A2825D36_023E_4482_8788_F53CD72619DD__INCLUDED_)
