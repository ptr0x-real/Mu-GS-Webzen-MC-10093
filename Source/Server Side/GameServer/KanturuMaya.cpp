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

// 모든 데이터 초기화
void CKanturuMaya::Init()
{
	m_iMayaObjIndex = -1;

	m_iMayaSkillTime = 0;
	m_iIceStormCount = 0;
}


// 마야와의 전투중 몬스터 무리와의 전투
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
			// 아이스스톰 영역 공격을 알린다.			
			// 칸투르 보스맵에 있는 유저에게 무조건 적용한다.
			KANTURU_UTIL.NotifyKanturuWideAreaAttack( m_iMayaObjIndex, iCount, KANTURU_MAYA_SKILL_ICE_STORM );

			// 스킬을 사용한다.
			TMonsterSkillManager::UseMonsterSkill( m_iMayaObjIndex, iCount, MSUS_TYPE_SPECIAL_PUSH );
			++m_iIceStormCount;
		}
	}

	LogAddTD( "[ KANTURU ][ IceStorm ] Skill Using(%d) : Index(%d) %s", m_iIceStormCount, m_iMayaObjIndex, gObj[m_iMayaObjIndex].Name );
}

void CKanturuMaya::KanturuMayaAct_Hands()
{
	// 20초마다 한 번씩 스킬을 사용한다.
	int TickCount = GetTickCount() - m_iMayaSkillTime;
	if( TickCount < 20000 )
		return;

	m_iMayaSkillTime = GetTickCount();
	
	if( m_iMayaObjIndex < 0 || m_iMayaObjIndex > MAX_OBJECT )
	{
		return;
	}
	
	// 칸투르 마야전 맵 안에 있는 모든 유저에게 강제 적용된다.	
	for( int iCount = ALLOC_USEROBJECTSTART; iCount < MAX_OBJECT; iCount++ )
	{
		if( gObj[iCount].Type == OBJTYPE_CHARACTER && gObjIsConnected( iCount )
			&& gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS 
			)
		{
			// 브로큰 샤 영역 공격을 알린다.
			// 칸투르 보스전 전투중인 유저에게 무조건 적용한다.
			KANTURU_UTIL.NotifyKanturuWideAreaAttack( m_iMayaObjIndex, iCount, KANTURU_MAYA_SKILL_BROKEN_SHOWER );			

			// 스킬 사용
			TMonsterSkillManager::UseMonsterSkill( m_iMayaObjIndex, iCount, MSUS_TYPE_ATTACK_RESTRICTION );

#ifdef ADD_KANTURU_CONDITION_20060725
			// 브로큰 샤워 스킬의 경우 문스톤펜던트를 착용하지 않은 상태라면 즉사하게 된다.
			// 문스톤펜던트 착용여부 판단 -> 문스톤펜던트의 착용장소가 반지로 변경
			// 오른쪽
			if( gObj[iCount].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,38) &&
				gObj[iCount].pInventory[EQUIPMENT_RING_RIGHT].m_Durability != 0 )
				continue;

			// 왼쪽
			if( gObj[iCount].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,38) &&
				gObj[iCount].pInventory[EQUIPMENT_RING_LEFT].m_Durability != 0 )
				continue;

			// 만일의 경우에 대비해 유저의 맵 인덱스 채크
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