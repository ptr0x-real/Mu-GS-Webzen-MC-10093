//----------------------------------------------------------------------------------
// FileName				: RaklionSelupan.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ����(������) �ൿ Ŭ����
//----------------------------------------------------------------------------------
// RaklionSelupan.cpp: implementation of the CRaklionSelupan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_RAKLION_20080408

#include "RaklionSelupan.h"

#include "TMonsterSkillManager.h"
#include "gObjMonster.h"
#include "MonsterSetBase.h"
#include "user.h"
#include "MagicInf.h"
#include "Include\public.h"

#include "RaklionDefine.h"
#include "RaklionBattleUserMng.h"
#include "RaklionUtil.h"
static CRaklionUtil g_RaklionUtil;
extern CMonsterSetBase	gMSetBase;

extern	void gObjCharZeroSet(int aIndex);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionSelupan::CRaklionSelupan()
{
	Init();
}

CRaklionSelupan::~CRaklionSelupan()
{

}

// ��� ������ �ʱ�ȭ
void CRaklionSelupan::Init()
{
	m_iSelupanObjIndex = _INVALID;
	m_iNowBerserkLevel = 0;
	m_iSelupanMinDamage = 0;
	m_iSelupanMaxDamage = 0;
}

// ���� ����� �о�� ��ų
void CRaklionSelupan::RaklionSelupanAct_FirstSkill()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = GetTargetOfSelupan();

	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{		
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	}
#ifdef MODIFY_RAKLION_SELUPAN_FIRST_SKILL_BUGFIX_20080721
	//else
#else
	else
#endif // MODIFY_RAKLION_SELUPAN_FIRST_SKILL_BUGFIX_20080721
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_FIRST_SKILL;
	
		// ��ų�� ����Ѵ�.
#ifdef ADD_RAKLION_20080408
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_SPECIAL_PUSH, 0, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ FirstSkill ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// �⺻ ����(��)
void CRaklionSelupan::RaklionSelupanAct_PoisonAttack()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = GetTargetOfSelupan();
#else
	INT iTargetIndex = gObj[iSelupanIndex].TargetNumber;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
	if( iTargetIndex == -1 )
	{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_RAKLION_20080408
		// Ÿ���� ������ ���� �����̵��� �Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_POISON_ATTACK;
		
#ifdef ADD_RAKLION_20080408
		// ��ų�� ����Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 1, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ PoisonAttack ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ���̽� ����
void CRaklionSelupan::RaklionSelupanAct_IceStorm()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = GetTargetOfSelupan();
#else
	INT iTargetIndex = gObj[iSelupanIndex].TargetNumber;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_RAKLION_20080408
		// Ÿ���� ������ ���� �����̵��� �Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_ICE_STORM;

#ifdef ADD_RAKLION_20080408
		// ��ų�� ����Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 2, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ IceStorm ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ���̽� ��Ʈ����ũ
void CRaklionSelupan::RaklionSelupanAct_IceStrike()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = GetTargetOfSelupan();
#else
	INT iTargetIndex = gObj[iSelupanIndex].TargetNumber;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_RAKLION_20080408
		// Ÿ���� ������ ���� �����̵��� �Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_ICE_STRIKE;
		
#ifdef ADD_RAKLION_20080408
		// ��ų�� ����Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 3, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ IceStrike ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ���� ��ȯ
void CRaklionSelupan::RaklionSelupanAct_SummonMonster()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = iSelupanIndex;
#else
	INT iTargetIndex = -1;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

#ifdef ADD_RAKLION_20080408
	// ��ų�� ����Ѵ�. (������ ��ȯ�� �ߴٰ� �˷��ֱ⸸ �Ѵ�.)
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_SPECIAL_SOMMON, 4 );
#endif // ADD_RAKLION_20080408
	
	// ���� ���͸� ���� �����ִ� �Լ�
	RegenSummonMonster();

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ SummonMonster ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// �ڱ� �ڽ� ġ��
void CRaklionSelupan::RaklionSelupanAct_Heal()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;

	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// ��ų�� ����Ѵ�.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_HEAL0, 5 );
#endif // ADD_RAKLION_20080408

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ Heal ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// �󸮱�
void CRaklionSelupan::RaklionSelupanAct_Freeze()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = GetTargetOfSelupan();
#else
	INT iTargetIndex = gObj[iSelupanIndex].TargetNumber;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	if( iTargetIndex == -1 )
	{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_RAKLION_20080408
		// Ÿ���� ������ ���� �����̵��� �Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
#ifdef ADD_RAKLION_20080408
		// ��ų�� ����Ѵ�.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 6 );
#endif // ADD_RAKLION_20080408
	}

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Freeze ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ���� �̵�
void CRaklionSelupan::RaklionSelupanAct_Teleport()
{
	INT iSelupanIndex = GetSelupanObjIndex();
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	INT iTargetIndex = GetTargetOfSelupan();
#else
	INT iTargetIndex = gObj[iSelupanIndex].TargetNumber;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	LPOBJECTSTRUCT lpObjTarget = &gObj[iTargetIndex];

#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	if( iTargetIndex < 0 )
		iTargetIndex = iSelupanIndex;
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_RAKLION_20080408
	// Ÿ���� ������ ���� �����̵��� �Ѵ�.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Teleport ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ���� ����
void CRaklionSelupan::RaklionSelupanAct_Invincibility()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

#ifdef ADD_RAKLION_20080408
	// ��ų�� ����Ѵ�.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_DEFENSE0, 8 );
#endif // ADD_RAKLION_20080408
	
	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Invincibility ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// ����ȭ ��ų ����
void CRaklionSelupan::RaklionSelupanAct_Berserk_Cancel()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
	// ������ ���������� �ǵ�����.
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage;
	
	LogAddTD( "[ RAKLION ][ Berserk Cancel ] MinDamage(%d) Damage(%d~%d)",
		lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// ����ȭ 1�ܰ�
void CRaklionSelupan::RaklionSelupanAct_Berserk1()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

#ifdef ADD_RAKLION_20080408
	// ��ų ����� �˸���.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408

	// ����ȭ �ܰ� ����
	m_iNowBerserkLevel = 1;
	INT iIncreaseAttackValue = 0;

	// ������ ���⼭ �ȴ�.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk1 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// ����ȭ 2�ܰ�
void CRaklionSelupan::RaklionSelupanAct_Berserk2()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// ��ų ����� �˸���.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// ����ȭ �ܰ� ����
	m_iNowBerserkLevel = 2;
	INT iIncreaseAttackValue = 0;
	
	// ������ ���⼭ �ȴ�.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;
	
	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk2 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// ����ȭ 3�ܰ�
void CRaklionSelupan::RaklionSelupanAct_Berserk3()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// ��ų ����� �˸���.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// ����ȭ �ܰ� ����
	m_iNowBerserkLevel = 3;
	INT iIncreaseAttackValue = 0;
	
	// ������ ���⼭ �ȴ�.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk3 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// ����ȭ 4�ܰ�
void CRaklionSelupan::RaklionSelupanAct_Berserk4()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// ��ų ����� �˸���.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// ����ȭ �ܰ� ����
	m_iNowBerserkLevel = 4;
	INT iIncreaseAttackValue = 0;
	
	// ������ ���⼭ �ȴ�.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;
	
	// ������ �ӽ� ����
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk4 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

//-------------------------------------------------------------------------------------------
// ���� �������� HP�� ���´�.
DWORD CRaklionSelupan::GetSelupanHP()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	return lpObj->Life;
}

// �Է¹��� %�� �������� HP�� �����Ѵ�.
void CRaklionSelupan::SetSelupanHP( INT iSelupanHPPercent )
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	FLOAT iSelupanMAXLife = lpObj->MaxLife + lpObj->AddLife;

	lpObj->Life = iSelupanMAXLife * iSelupanHPPercent / 100;
}

// �������� ObjIndex�� ã�� �����Ѵ�.
void CRaklionSelupan::InitSelupanIndex()
{
	// ��� ��ü�� �������� �ε����� ��ġ�ϴ� ��ü�� �迭 �ε����� �����Ѵ�.
	for( int iCount = 0; iCount < MAX_MONSTER; iCount++ )
	{
		if( gObj[iCount].Connected==3 && gObj[iCount].Class == RAKLION_SELUPAN )
		{
			m_iSelupanObjIndex = iCount;
		}
	}

	if( m_iSelupanObjIndex == _INVALID )
	{
		LogAddC( LOGC_RED, "[ RAKLION ] [ InitSelupanIndex ] error : SelupanObjIndex(%d)", m_iSelupanObjIndex );
	}
		
}

//-------------------------------------------------------------------------------------------
// �������� �������ش�.
BOOL CRaklionSelupan::RegenSelupan()
{
	INT iSelupanPosNum = -1;
	for( int n=0; n<gMSetBase.m_Count; n++ )
	{
		// �������� ���� ��ũ��Ʈ ��ǥ�� ã������ ���̺� �ѹ��� ã�´�.
		if( gMSetBase.m_Mp[n].m_Type == RAKLION_SELUPAN )
		{
			iSelupanPosNum = n;
		}
	}

	LPOBJECTSTRUCT lpObj = NULL;
	int result = 0;

	result = gObjAddMonster( MAP_INDEX_RAKLION_BOSS );

	if( result < 0 )
	{
		//error
		LogAddTD( "[RAKLION] CreateSelupan error" );
		return FALSE;
	}

	lpObj = &gObj[result];

	gObjSetPosMonster( result, iSelupanPosNum );
	gObjSetMonster( result, RAKLION_SELUPAN );

	LogAddTD("[RAKLION] Create Selupan : X (%d) / Y(%d)", lpObj->X, lpObj->Y );

	// �������� �ִ� ��ü���� ����Ʈ ����
	//////////////////////////////////////////////////////////////////////////
	LPOBJECTSTRUCT lpObjUser = NULL;
	for( int n=0; n<MAX_OBJECT; n++)
	{
		lpObjUser = &gObj[n];
		if( lpObjUser->MapNumber == MAP_INDEX_RAKLION_BOSS )
			gObjStateSetCreate(n);
	}
				
	for( int n=0; n<MAX_OBJECT; n++)
	{
		lpObjUser = &gObj[n];
		if( lpObjUser->MapNumber == MAP_INDEX_RAKLION_BOSS )
			gObjViewportListDestroy(n);
	}
	
	for( int n=0; n<MAX_OBJECT; n++)
	{
		lpObjUser = &gObj[n];
		if( lpObjUser->MapNumber == MAP_INDEX_RAKLION_BOSS )
			gObjViewportListCreate(n);
	}
	
	for( int n=0; n<MAX_OBJECT; n++)
	{
		lpObjUser = &gObj[n];
		if( lpObjUser->MapNumber == MAP_INDEX_RAKLION_BOSS )
			gObjViewportListProtocol(n);
	}

	gObjSetState();
	//////////////////////////////////////////////////////////////////////////

	// �������� ������ ���
	m_iSelupanMinDamage = lpObj->m_AttackDamageMin;
	m_iSelupanMaxDamage = lpObj->m_AttackDamageMax;

	return TRUE;
}

// �������� ���ش�.
void CRaklionSelupan::HideSelupan()
{
	INT iSelupanIndex = GetSelupanObjIndex();

	gObjViewportListProtocolDestroy( &gObj[iSelupanIndex] );
	gObjViewportClose( &gObj[iSelupanIndex] );

	gObjDel ( iSelupanIndex );
}

// �������� ù��° Ÿ���� ���´�.
INT CRaklionSelupan::GetTargetOfSelupan()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
	if( lpObj->TargetNumber >= 0 && lpObj->TargetNumber < MAX_OBJECT )
	{
		return lpObj->TargetNumber;
	}
	else
		return gObjMonsterSearchEnemy( lpObj, lpObj->Type );
}

// �������� ���� ��ų�� �������� �����̸� �����Ѵ�.
void CRaklionSelupan::SetSelupanSkillDelay( INT iDelay )
{
	if( iDelay < 0 || iDelay > RAKLION_SELUPAN_SKILL_DELAY_MAX )
	{
		// ���� �߻�
		LogAddC( LOGC_RED, "[ RAKLION ][ SetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay );
		return;
	}

	m_iSkillDelay = iDelay;
}

// �������� ���� ��ų�� ���������� �����̸� ���´�.
INT CRaklionSelupan::GetSelupanSkillDelay()
{
	if( m_iSkillDelay < 0 || m_iSkillDelay > RAKLION_SELUPAN_SKILL_DELAY_MAX )
	{
		// ���� �߻�
		LogAddC( LOGC_RED, "[ RAKLION ][ GetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay );
		return -1;
	}

	return m_iSkillDelay;
}

// ���� ����ȭ�� �ܰ踦 ���´�.
INT CRaklionSelupan::GetBerserkLevel()
{
	return m_iNowBerserkLevel;
}

// �������� �ּ�, �ִ� �������� �Է� ���� ������ �����Ѵ�.
void CRaklionSelupan::InitSelupanDamage( INT iMinDamage, INT iMaxDamage )
{
	m_iSelupanMinDamage = iMinDamage;
	m_iSelupanMaxDamage = iMaxDamage;
}

//-------------------------------------------------------------------------------------------
// �������� ObjIndex�� �����Ѵ�.
void CRaklionSelupan::SetSelupanObjIndex( int iIndex )
{
	if( iIndex < 0 || iIndex > MAX_MONSTER )
	{
		// ���� �߻�
		LogAddC( LOGC_RED, "[ RAKLION ][ SetSelupanObjIndex ] SelupanObjIndex error : Index(%d)", iIndex );
		return;
	}

	m_iSelupanObjIndex = iIndex;
}

// �������� ObjIndex�� ���´�.
INT CRaklionSelupan::GetSelupanObjIndex()
{
	// ������ �ε����� �������ش�.
	InitSelupanIndex();

	return m_iSelupanObjIndex;
}

//-------------------------------------------------------------------------------------------
// �������� ��ȯ ���͵��� �����Ѵ�.
void CRaklionSelupan::RegenSummonMonster()
{
	LPOBJECTSTRUCT lpObj	= NULL;
	INT iPosTableNum		= -1;
	INT iClass				= -1;

	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		lpObj = &gObj[iCount];

		// ��Ŭ���� �������� ���Ͱ� �ƴϸ� return
		if( lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS )
			continue;

		if( lpObj->Class == RAKLION_COOLUTIN || lpObj->Class == RAKLION_IRON_KNIGHT )
		{
			// ���� ��� �ִ� ���ʹ� �������� �ʴ´�.
			if( lpObj->Connected==3 && lpObj->DieRegen==0 && lpObj->Live==1 )
				continue;

			// ������ Ŭ���� ���
			iClass = lpObj->Class;

			// ������ ��ġ���̺� ��ȣ ���
			iPosTableNum = lpObj->m_PosNum;
			
#ifdef MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
			// ������ ��ü�� �ʱ�ȭ
			gObjCharZeroSet( iCount );
			gObjDel( lpObj->m_Index );
			lpObj->Class = 0;
#endif // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205

			// ���� ��ü�� �߰��Ѵ�.
			int result = gObjAddMonster( MAP_INDEX_RAKLION_BOSS );
			lpObj = &gObj[result];

			if( result >= 0 )		// ���� ��ü �߰� ����
			{
				// ���� ��ġ ����
				gObjSetPosMonster( result, iPosTableNum );

				// ���� ���� ����
				gObjSetMonster( result, iClass );

				lpObj->Live = 1;
				lpObj->DieRegen = 0;

#ifdef MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				// ������ ��ü�� �ʱ�ȭ
				//gObjCharZeroSet( iCount );
#else // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				// ������ ��ü�� �ʱ�ȭ
				gObjCharZeroSet( iCount );
#endif // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				
				LogAddTD("[RAKLION] Summoned Monster : %s => X-(%d) Y-(%d)", lpObj->Name, lpObj->X, lpObj->Y ); 
			}
			else					// ���� ��ü �߰� ����
			{
				//error
				LogAddTD( "[RAKLION] CreateSelupan error" );
				return;
			}
		}
	}	
}

// ��ȯ�� ���͸� ���ش�.
void CRaklionSelupan::DeleteSummonMonster()
{
	LPOBJECTSTRUCT lpObj = NULL;

	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		lpObj = &gObj[iCount];

		// ��Ŭ���� �������� ���Ͱ� �ƴϸ� return
		if( lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS )
			continue;

		// ��ƾ(457), ���̾���Ʈ(458)�� ã�´�.
		if( lpObj->Class == RAKLION_COOLUTIN || lpObj->Class == RAKLION_IRON_KNIGHT )
		{
			gObjViewportListProtocolDestroy( lpObj );
			gObjViewportClose( lpObj );
			
			gObjDel ( lpObj->m_Index );

			lpObj->Live = 0;
			lpObj->DieRegen = 1;
		}
	}
	
	LogAddC( LOGC_LIGHTRED, "[ RAKLION ] [ DeleteSummonMonster ] Delete All SummonMonster" );
}


#endif