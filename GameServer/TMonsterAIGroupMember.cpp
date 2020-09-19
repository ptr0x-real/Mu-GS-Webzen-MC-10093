#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIGroupMember.h"

TMonsterAIGroupMember::TMonsterAIGroupMember()
{
	Reset();
}

TMonsterAIGroupMember::~TMonsterAIGroupMember()
{

}

void TMonsterAIGroupMember::Reset()
{
	m_iGroupNumber		= _INVALID;
	m_iGuid				= _INVALID;
	m_iClass			= _INVALID;
	m_iRank				= _INVALID;
	m_iStartAI			= _ZERO;
	m_iAI01				= _ZERO;
	m_iAI02				= _ZERO;
	m_iAI03				= _ZERO;
	m_iCreateType		= _INVALID;
	m_iMapNumber		= _INVALID;
	m_iStartX			= _INVALID;
	m_iStartY			= _INVALID;
	m_iStartDir			= _INVALID;
	m_iRegenType		= _INVALID;

	// 파일에서 읽지 않는다. 할당된값.
	m_iObjIndex			= _INVALID;
}
#endif