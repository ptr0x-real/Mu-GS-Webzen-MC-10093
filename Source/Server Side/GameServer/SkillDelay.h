// SkillDelay.h: interface for the CSkillDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLDELAY_H__6BDF289C_20D9_46DE_BA06_B7D300A22D83__INCLUDED_)
#define AFX_SKILLDELAY_H__6BDF289C_20D9_46DE_BA06_B7D300A22D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillDelay  
{
	DWORD		LastSkillUseTime[MAX_SKILL];
public:
	CSkillDelay();
	virtual ~CSkillDelay();

	void		Init();
	BOOL		Check(BYTE skill);
};

#endif // !defined(AFX_SKILLDELAY_H__6BDF289C_20D9_46DE_BA06_B7D300A22D83__INCLUDED_)
