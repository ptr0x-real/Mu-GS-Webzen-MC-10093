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
	{	// ����� �÷��ô� ���� �ø����� �ȵ�� ���Ƿ� ��ȿ... (Ŭ���̾�Ʈ ������ �����ؾ� �ȴ�)
		return TRUE;
	}
	m_iCount;
	
	for( int i = 0; i < MAHC_MAX_CHECKTABLE; i++ )
	{
		if( m_Table[i].number == aTargetIndex )
		{
			if( serial == MAHC_MAX_SERIAL-1 && m_Table[i].Serial[serial] )
			{	// �ѹ��� ���� �ʱ�ȭ �� �ش�
				memset(m_Table[i].Serial, 0x00, MAHC_MAX_SERIAL);
			}	

			m_Table[i].Serial[serial]++;

			if( skillnum == 9 )
			{	// �Ƿ��� 8���� �ϳ��� �ø����� ������(-_-;;)
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
				{	// ���� ĳ���Ϳ��� ���� �ø����� ��ȣ�� ������ 3���̻� �޾Ҵ�.
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
	{	// ����� �÷��ô� ���� �ø����� �ȵ�� ���Ƿ� ��ȿ... (Ŭ���̾�Ʈ ������ �����ؾ� �ȴ�)
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
			{	// �ѹ��� ���� �ʱ�ȭ �� �ش�
				memset(m_Table[i].Serial, 0x00, MAHC_MAX_SERIAL);
			}	

			m_Table[i].Serial[serial]++;
			if( m_Table[i].Serial[serial]  > 1 )
			{	// ���� ĳ���Ϳ��� ���� �ø����� ��ȣ�� ������ 3���̻� �޾Ҵ�.
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
	// ���̾ũ�� ��ų üũ
BOOL CMultiAttackHackCheck::CheckFireScreamSkill( int iTargetIndex, BYTE btSkillnum, BYTE btSerial )
{
	if( g_SkillAdditionInfo.GetCheckFireScreamSkill() == FALSE )
	{
		// �ø��� üũ�� ���� �ʴ´�.
		return TRUE;
	}

	// ���̾ũ�� ��ų�� �� �ٱ�� �ϳ��� �ø����� �ο��ȴ�.
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
			{	// ���� ĳ���Ϳ��� ���� �ø����� ��ȣ�� ������ 1ȸ �̻� ���� �� ����.
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