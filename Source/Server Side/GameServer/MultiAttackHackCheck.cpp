// MultiAttackHackCheck.cpp: implementation of the CMultiAttackHackCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiAttackHackCheck.h"

#include "User.h"

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	#include "SkillAdditionInfo.h"
#endif
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiAttackHackCheck	gMultiAttackHackCheck[MAX_OBJECT];

CMultiAttackHackCheck::CMultiAttackHackCheck()
{
	Init();
}

CMultiAttackHackCheck::~CMultiAttackHackCheck()
{

}

void CMultiAttackHackCheck::Init()
{
	m_iCount = 0;

	for( int i = 0; i < MAHC_MAX_CHECKTABLE; i++ )
	{
		m_Table[i].number = -1;
		memset(m_Table[i].Serial, 0x00, MAHC_MAX_SERIAL);
	}
}

BOOL CMultiAttackHackCheck::Insert(int aTargetIndex, BYTE skillnum, BYTE serial)
{
	if( gObj[aTargetIndex].Type == OBJTYPE_MONSTER || serial < 0 || serial > MAHC_MAX_SERIAL-1 )
		return TRUE;

	if( skillnum == 12 )
	{	// 아쿠아 플러시는 현재 시리얼이 안들어 오므로 무효... (클라이언트 수정후 적용해야 된다)
		return TRUE;
	}
	m_iCount;
	
	for( int i = 0; i < MAHC_MAX_CHECKTABLE; i++ )
	{
		if( m_Table[i].number == aTargetIndex )
		{
			if( serial == MAHC_MAX_SERIAL-1 && m_Table[i].Serial[serial] )
			{	// 한바퀴 돌면 초기화 해 준다
				memset(m_Table[i].Serial, 0x00, MAHC_MAX_SERIAL);
			}	

			m_Table[i].Serial[serial]++;

			if( skillnum == 9 )
			{	// 악령은 8개가 하나의 시리얼을 가진다(-_-;;)
				if( m_Table[i].Serial[serial]  > 6 )
				{	
#if TESTSERVER == 1
					LogAddTD("Error: %d, %d, %d", skillnum, serial, m_Table[i].Serial[serial]);
#endif
					return FALSE;
				}
			}
			else
			{
				if( m_Table[i].Serial[serial]  > 6 )
				{	// 같은 캐릭터에게 같은 시리얼의 번호의 공격을 3번이상 받았다.
#if TESTSERVER == 1
					LogAddTD("Error: %d, %d, %d", skillnum, serial, m_Table[i].Serial[serial]);
#endif
					return FALSE;
				}
			}
		}
	}

	m_Table[m_iCount].number = aTargetIndex;
	m_Table[m_iCount].Serial[serial]++;

	m_iCount++;
	if( m_iCount >= MAHC_MAX_CHECKTABLE )
	{
		m_iCount = 0;
	}
	return TRUE;
}


#ifdef CHECK_PENETRATION_SKILL
BOOL CMultiAttackHackCheck::CheckPenetrationSkill(int aTargetIndex, BYTE skillnum, BYTE serial)
{
	//if( gObj[aTargetIndex].Type == OBJTYPE_MONSTER )
		//return TRUE;

	if( skillnum != AT_SKILL_PENETRATION )
	{	// 아쿠아 플러시는 현재 시리얼이 안들어 오므로 무효... (클라이언트 수정후 적용해야 된다)
		return TRUE;
	}
	m_iCount;	

	if( serial == 10 )
		Init();
	
	for( int i = 0; i < MAHC_MAX_CHECKTABLE; i++ )
	{
		if( m_Table[i].number == aTargetIndex )
		{
			if( serial == 10 && m_Table[i].Serial[serial] )
			{	// 한바퀴 돌면 초기화 해 준다
				memset(m_Table[i].Serial, 0x00, MAHC_MAX_SERIAL);
			}	

			m_Table[i].Serial[serial]++;
			if( m_Table[i].Serial[serial]  > 1 )
			{	// 같은 캐릭터에게 같은 시리얼의 번호의 공격을 3번이상 받았다.
#if TESTSERVER == 1
				LogAddC(LOGC_LIGHTRED, "Serial:%d,%d", serial, m_Table[i].Serial[serial]);
#endif
				return FALSE;
			}			
		}
	}

	m_Table[m_iCount].number = aTargetIndex;
	m_Table[m_iCount].Serial[serial]++;

	//LogAddC(LOGC_BLUE, "Serial:%d,%d", serial,m_Table[m_iCount].Serial[serial]);

	m_iCount++;
	if( m_iCount >= MAHC_MAX_CHECKTABLE )
	{
		m_iCount = 0;
	}
	return TRUE;
}
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	// 파이어스크림 스킬 체크
BOOL CMultiAttackHackCheck::CheckFireScreamSkill( int iTargetIndex, BYTE btSkillnum, BYTE btSerial )
{
	if( g_SkillAdditionInfo.GetCheckFireScreamSkill() == FALSE )
	{
		// 시리얼 체크를 하지 않는다.
		return TRUE;
	}

	// 파이어스크림 스킬의 한 줄기당 하나의 시리얼이 부여된다.
	if( btSerial >  g_SkillAdditionInfo.GetFireScreamSerialInitCount() - 4)
	{
		Init();
	}
	
	for( int iCount = 0; iCount < MAHC_MAX_CHECKTABLE; iCount++ )
	{
		if( m_Table[iCount].number == iTargetIndex )
		{
			++m_Table[iCount].Serial[btSerial];
			if( m_Table[iCount].Serial[btSerial]  > g_SkillAdditionInfo.GetFireScreamMaxAttackCountSameSerial() )
			{	// 같은 캐릭터에게 같은 시리얼의 번호의 공격을 1회 이상 받을 수 없다.
#if TESTSERVER == 1
				LogAddC( LOGC_LIGHTRED, "[ FireScream ] Check Serial - Over Max Hit [%s][%s] Serial(%d) Hit(%d)", 
					gObj[iTargetIndex].AccountID, gObj[iTargetIndex].Name, btSerial, m_Table[iCount].Serial[btSerial] );
#endif
				return FALSE;
			}
			
			return TRUE;
		}
	}

	m_Table[m_iCount].number = iTargetIndex;
	++m_Table[m_iCount].Serial[btSerial];
	
	++m_iCount;
	if( m_iCount >= MAHC_MAX_CHECKTABLE )
	{
		m_iCount = 0;
	}
	
	return TRUE;
}
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601