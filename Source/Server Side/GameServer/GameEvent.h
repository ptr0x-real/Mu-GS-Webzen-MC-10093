// GameEvent.h: interface for the CGameEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEEVENT_H__E3174BF9_442F_400C_887C_C8D632F9B2F9__INCLUDED_)
#define AFX_GAMEEVENT_H__E3174BF9_442F_400C_887C_C8D632F9B2F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_EVENT_MAP				32
#define	MAX_EVENT_DROPITEM_NUM		16
#define MAX_EVENT_DROPITEM_RATE		1000000		// ½Ê¸¸

class	CMobDropItemInfo 
{
public:
	WORD	m_wType;
	BYTE	m_level;
	BYTE	m_option1;
	BYTE	m_option2;
	BYTE	m_option3;
	float	m_fDurability;
};

class CGameEvent  
{
public:
	CGameEvent();
	~CGameEvent();

	void	Init();
	void	LoadItem(const char szDataPath[]);
	BOOL	GetEventItem( const int iMapIndex, CMobDropItemInfo & rMobDropItemInfo );

private:
	CMobDropItemInfo	m_eventItemTable[MAX_EVENT_MAP][MAX_EVENT_DROPITEM_NUM]; 
	DWORD				m_eventItemDropRate[MAX_EVENT_MAP][MAX_EVENT_DROPITEM_NUM];
};

extern	CGameEvent	g_GameEvent;

#endif // !defined(AFX_GAMEEVENT_H__E3174BF9_442F_400C_887C_C8D632F9B2F9__INCLUDED_)
