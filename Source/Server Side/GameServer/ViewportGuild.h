// ViewportGuild.h: interface for the CViewportGuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWPORTGUILD_H__CD68CCCD_B2F9_4EB6_96A8_568510FB4E31__INCLUDED_)
#define AFX_VIEWPORTGUILD_H__CD68CCCD_B2F9_4EB6_96A8_568510FB4E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CViewportGuild  
{
public:
	short m_GuildNumber[MAXVIEWPORTOBJECT];
	short m_GuildCount;

public:
	CViewportGuild();
	virtual ~CViewportGuild();


	void Init();
	BOOL Add(int number, int aIndex);
	

};

#endif // !defined(AFX_VIEWPORTGUILD_H__CD68CCCD_B2F9_4EB6_96A8_568510FB4E31__INCLUDED_)
