// KanturuMaya.cpp: implementation of the CKanturuMaya class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuMaya.h"

#include "TMonsterSkillManager.h"

#include "KanturuDefine.h"
#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
static CKanturuUtil KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuMaya::CKanturuMaya()
{
	Init();
}

CKanturuMaya::~CKanturuMaya()
{

}

// ��� ������ �ʱ�ȭ
void CKanturuMaya::Init()
{
	m_iMayaObjIndex = -1;

	m_iMayaSkillTime = 0;
	m_iIceStormCount = 0;
}


// ���߿��� ������ ���� �������� ����
void CKanturuMaya::KanturuMayaAct_IceStorm( int iSkillUsingRate )
{	
	if( ( rand() % 10000 ) > iSkillUsingRate )
		return;

	if( m_iMayaObjIndex < 0 || m_iMayaObjIndex > MAX_OBJECT )
	{
		return;
	}
	
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Type == OBJTYPE_CHARACTER && gObjIsConnected( iCount ) 
			&& gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS
			)
		{
			// ���̽����� ���� ������ �˸���.			
			// ĭ���� �����ʿ� �ִ� �������� ������ �����Ѵ�.
			KANTURU_UTIL.NotifyKanturuWideAreaAttack( m_iMayaObjIndex, iCount, KANTURU_MAYA_SKILL_ICE_STORM );

			// ��ų�� ����Ѵ�.
			TMonsterSkillManager::UseMonsterSkill( m_iMayaObjIndex, iCount, MSUS_TYPE_SPECIAL_PUSH );
			++m_iIceStormCount;
		}
	}

	LogAddTD( "[ KANTURU ][ IceStorm ] Skill Using(%d) : Index(%d) %s", m_iIceStormCount, m_iMayaObjIndex, gObj[m_iMayaObjIndex].Name );
}

void CKanturuMaya::KanturuMayaAct_Hands()
{
	// 20�ʸ��� �� ���� ��ų�� ����Ѵ�.
	int TickCount = GetTickCount() - m_iMayaSkillTime;
	if( TickCount < 20000 )
		return;

	m_iMayaSkillTime = GetTickCount();
	
	if( m_iMayaObjIndex < 0 || m_iMayaObjIndex > MAX_OBJECT )
	{
		return;
	}
	
	// ĭ���� ������ �� �ȿ� �ִ� ��� �������� ���� ����ȴ�.	
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Type == OBJTYPE_CHARACTER && gObjIsConnected( iCount )
			&& gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			)
		{
			// ���ū �� ���� ������ �˸���.
			// ĭ���� ������ �������� �������� ������ �����Ѵ�.
			KANTURU_UTIL.NotifyKanturuWideAreaAttack( m_iMayaObjIndex, iCount, KANTURU_MAYA_SKILL_BROKEN_SHOWER );			

			// ��ų ���
			TMonsterSkillManager::UseMonsterSkill( m_iMayaObjIndex, iCount, MSUS_TYPE_ATTACK_RESTRICTION );

#ifdef ADD_KANTURU_CONDITION_20060725
			// ���ū ���� ��ų�� ��� ���������Ʈ�� �������� ���� ���¶�� ����ϰ� �ȴ�.
			// ���������Ʈ ���뿩�� �Ǵ� -> ���������Ʈ�� ������Ұ� ������ ����
			// ������
			if( gObj[iCount].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,38) &&
				gObj[iCount].pInventory[EQUIPMENT_RING_RIGHT].m_Durability != 0 )
				continue;

			// ����
			if( gObj[iCount].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,38) &&
				gObj[iCount].pInventory[EQUIPMENT_RING_LEFT].m_Durability != 0 )
				continue;

			// ������ ��쿡 ����� ������ �� �ε��� äũ
			if( gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				LPOBJECTSTRUCT lpMayaObj = &gObj[m_iMayaObjIndex];

				gObj[iCount].Life = 0.0;
				gObjLifeCheck( &gObj[iCount], lpMayaObj, gObj[iCount].Life, 0, 0, 0, 0 );

				LogAddTD("[ KANTURU ][ BrokenShower ] [%s][%s] User Dying cause NOT wearing MoonStone Pandent",
					gObj[iCount].AccountID, gObj[iCount].Name );
			}

#endif // ADD_KANTURU_CONDITION_20060725
		}
	}

	LogAddTD( "[ KANTURU ][ BrokenShower ] Skill Using : Index(%d) %s",	m_iMayaObjIndex, gObj[m_iMayaObjIndex].Name );	
}

#endif