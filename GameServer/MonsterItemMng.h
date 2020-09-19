// MonsterItemMng.h: interface for the CMonsterItemMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERITEMMNG_H__7B5290EA_2511_4CC6_822C_364CC3FC3D7A__INCLUDED_)
#define AFX_MONSTERITEMMNG_H__7B5290EA_2511_4CC6_822C_364CC3FC3D7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MONSTERLEVEL		150
#define MAX_MONSTERGETITEMCOUNT	1000

class CItem;

class CMonsterItemMng  
{
public:
	CItem*	m_MonsterInvenItems[MAX_MONSTERLEVEL];
	int     m_iMonsterInvenItemCount[MAX_MONSTERLEVEL];
	
	char	MonsterName[255];

public:
	CMonsterItemMng();
	virtual ~CMonsterItemMng();

	void	Init();
	void	Clear();
	

	BYTE	InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1=0, int op2=0, int op3=0);
	CItem*	GetItem(int monsterlevel);

	void	gObjGiveItemSearch(int monsterlevel, int maxlevel);
};

extern CMonsterItemMng g_MonsterItemMng;

#endif // !defined(AFX_MONSTERITEMMNG_H__7B5290EA_2511_4CC6_822C_364CC3FC3D7A__INCLUDED_)
