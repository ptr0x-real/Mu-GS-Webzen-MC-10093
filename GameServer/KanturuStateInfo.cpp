// KanturuStateInfo.cpp: implementation of the CKanturuStateInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuStateInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuStateInfo::CKanturuStateInfo()
:m_iState(0),m_iCondition(0),m_iValue(0),m_dwAppliedTickCount(0)
{
	ResetTimeInfo();
}

CKanturuStateInfo::~CKanturuStateInfo()
{

}


#endif