// QuestUtil.h: interface for the CQuestUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUTIL_H__89815125_2CFA_4FF8_8048_7E0A080C8B5D__INCLUDED_) && defined(MODIFY_QUEST_SYSTEM_20070525)
#define AFX_QUESTUTIL_H__89815125_2CFA_4FF8_8048_7E0A080C8B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

class CQuestUtil  
{
public:
	CQuestUtil();
	virtual ~CQuestUtil();
	
	void GDReqQuestMonsterKillInfo( LPOBJECTSTRUCT lpObj );

	void DGGetQuestMonKillInfo( LPBYTE aRecv );
	void GDReqQuestMonKillInfoSave( LPOBJECTSTRUCT lpObj );

};

extern CQuestUtil	g_QuestUtil;

#endif // !defined(AFX_QUESTUTIL_H__89815125_2CFA_4FF8_8048_7E0A080C8B5D__INCLUDED_)
