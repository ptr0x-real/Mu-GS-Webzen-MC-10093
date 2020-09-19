// MapItem.h: interface for the CMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPITEM_H__6DF7E787_85B8_4C31_8143_C081366A4436__INCLUDED_)
#define AFX_MAPITEM_H__6DF7E787_85B8_4C31_8143_C081366A4436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common\zzzitem.h"

class CMapItem : public CItem
{
public:
	BYTE px;
	BYTE py;	
	
	BYTE live;
	BYTE Give;			// 누군가 벌써 주웠다..
	DWORD m_State;

	DWORD m_Time;
	int	  m_UserIndex;	// 누구의 아이템인가?
	DWORD m_LootTime;	// 탈취할수 있는 최소 시간
	
	VIEWPORT_PLAYER_STRUCT	VpPlayer[MAXVIEWPORTOBJECT];
	int VPCount;	
public :
	CMapItem();
	virtual ~CMapItem();
	
	void Init();
	
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx);
	#else
		void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number);
	#endif // ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef DARKLORD_WORK
	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE SocketOption[], BYTE SocketBonusOption);
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx);
		#else
			void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp);
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#else
		void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number);
#endif
};

#endif // !defined(AFX_MAPITEM_H__6DF7E787_85B8_4C31_8143_C081366A4436__INCLUDED_)
