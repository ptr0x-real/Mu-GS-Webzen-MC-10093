// SkillDelay.cpp: implementation of the CSkillDelay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDelay.h"
#include "MagicDamage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillDelay::CSkillDelay()
{
	Init();
}

CSkillDelay::~CSkillDelay()
{

}

void CSkillDelay::Init()
{
	memset(LastSkillUseTime, 0, sizeof(LastSkillUseTime));
}

BOOL CSkillDelay::Check(BYTE skill)
{
	return TRUE;
	
#ifdef DARKLORD_WORK
	int skilldelaytime = MagicDamageC.GetDelayTime(skill);
	if( !skilldelaytime )
		return TRUE;	// ��ų �����̽ð��� ���� ���� �����Ѵ�

	DWORD dwtime = GetTickCount();

	if( LastSkillUseTime[skill]+skilldelaytime < dwtime )
	{
		LastSkillUseTime[skill] = dwtime;
		return TRUE;
	}
	else
		return FALSE;
#endif
}