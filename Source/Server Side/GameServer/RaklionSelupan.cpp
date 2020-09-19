//----------------------------------------------------------------------------------
// FileName				: RaklionSelupan.cpp
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 보스(세루판) 행동 클래스
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

// 모든 데이터 초기화
void CRaklionSelupan::Init()
{
	m_iSelupanObjIndex = _INVALID;
	m_iNowBerserkLevel = 0;
	m_iSelupanMinDamage = 0;
	m_iSelupanMaxDamage = 0;
}

// 최초 등장시 밀어내기 스킬
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
	
		// 스킬을 사용한다.
#ifdef ADD_RAKLION_20080408
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_SPECIAL_PUSH, 0, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ FirstSkill ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 기본 공격(독)
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
		// 타겟이 없으면 랜덤 순간이동을 한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_POISON_ATTACK;
		
#ifdef ADD_RAKLION_20080408
		// 스킬을 사용한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 1, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ PoisonAttack ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 아이스 스톰
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
		// 타겟이 없으면 랜덤 순간이동을 한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_ICE_STORM;

#ifdef ADD_RAKLION_20080408
		// 스킬을 사용한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 2, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ IceStorm ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 아이스 스트라이크
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
		// 타겟이 없으면 랜덤 순간이동을 한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
		CMagicInf cMagicInf;
		ZeroMemory( &cMagicInf, sizeof(CMagicInf) );
		cMagicInf.m_Skill = AT_SKILL_SELUPAN_ICE_STRIKE;
		
#ifdef ADD_RAKLION_20080408
		// 스킬을 사용한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 3, &cMagicInf );
#endif // ADD_RAKLION_20080408
	}

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ IceStrike ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 몬스터 소환
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
	// 스킬을 사용한다. (서버에 소환을 했다고 알려주기만 한다.)
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_SPECIAL_SOMMON, 4 );
#endif // ADD_RAKLION_20080408
	
	// 실제 몬스터를 리젠 시켜주는 함수
	RegenSummonMonster();

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ SummonMonster ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 자기 자신 치료
void CRaklionSelupan::RaklionSelupanAct_Heal()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;

	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// 스킬을 사용한다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_HEAL0, 5 );
#endif // ADD_RAKLION_20080408

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );
	
	LogAddTD( "[ RAKLION ][ Heal ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 얼리기
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
		// 타겟이 없으면 랜덤 순간이동을 한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408
	}
	else
	{
#ifdef ADD_RAKLION_20080408
		// 스킬을 사용한다.
		TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 6 );
#endif // ADD_RAKLION_20080408
	}

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Freeze ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 순간 이동
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
	// 타겟이 없으면 랜덤 순간이동을 한다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 7 );
#endif // ADD_RAKLION_20080408

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Teleport ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 순간 무적
void CRaklionSelupan::RaklionSelupanAct_Invincibility()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

#ifdef ADD_RAKLION_20080408
	// 스킬을 사용한다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_DEFENSE0, 8 );
#endif // ADD_RAKLION_20080408
	
	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Invincibility ] Skill Using : Index(%d) %s", iSelupanIndex, lpObj->Name );
}

// 광폭화 스킬 해제
void CRaklionSelupan::RaklionSelupanAct_Berserk_Cancel()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
	// 최초의 설정값으로 되돌린다.
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage;
	
	LogAddTD( "[ RAKLION ][ Berserk Cancel ] MinDamage(%d) Damage(%d~%d)",
		lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// 광폭화 1단계
void CRaklionSelupan::RaklionSelupanAct_Berserk1()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

#ifdef ADD_RAKLION_20080408
	// 스킬 사용을 알린다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408

	// 광폭화 단계 설정
	m_iNowBerserkLevel = 1;
	INT iIncreaseAttackValue = 0;

	// 적용은 여기서 된다.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk1 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// 광폭화 2단계
void CRaklionSelupan::RaklionSelupanAct_Berserk2()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// 스킬 사용을 알린다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// 광폭화 단계 설정
	m_iNowBerserkLevel = 2;
	INT iIncreaseAttackValue = 0;
	
	// 적용은 여기서 된다.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;
	
	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk2 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// 광폭화 3단계
void CRaklionSelupan::RaklionSelupanAct_Berserk3()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// 스킬 사용을 알린다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// 광폭화 단계 설정
	m_iNowBerserkLevel = 3;
	INT iIncreaseAttackValue = 0;
	
	// 적용은 여기서 된다.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;

	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk3 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

// 광폭화 4단계
void CRaklionSelupan::RaklionSelupanAct_Berserk4()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	INT iTargetIndex = iSelupanIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];
	
#ifdef ADD_RAKLION_20080408
	// 스킬 사용을 알린다.
	TMonsterSkillManager::UseMonsterSkill( iSelupanIndex, iTargetIndex, MSUS_TYPE_ATTACK_RESTRICTION, 9 );
#endif // ADD_RAKLION_20080408
	
	// 광폭화 단계 설정
	m_iNowBerserkLevel = 4;
	INT iIncreaseAttackValue = 0;
	
	// 적용은 여기서 된다.
	iIncreaseAttackValue = m_iNowBerserkLevel * lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;
	lpObj->m_AttackDamageMin = m_iSelupanMinDamage + iIncreaseAttackValue;
	lpObj->m_AttackDamageMax = m_iSelupanMaxDamage + iIncreaseAttackValue;
	
	// 딜레이 임시 세팅
//	SetSelupanSkillDelay( RAKLION_BOSS_SKILL_DELAY );

	LogAddTD( "[ RAKLION ][ Berserk4 ] Skill Using : Index(%d) Damage(%d~%d)",
		iSelupanIndex, lpObj->m_AttackDamageMin, lpObj->m_AttackDamageMax );
}

//-------------------------------------------------------------------------------------------
// 현재 세루판의 HP를 얻어온다.
DWORD CRaklionSelupan::GetSelupanHP()
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	return lpObj->Life;
}

// 입력받은 %로 세루판의 HP를 세팅한다.
void CRaklionSelupan::SetSelupanHP( INT iSelupanHPPercent )
{
	INT iSelupanIndex = GetSelupanObjIndex();
	LPOBJECTSTRUCT lpObj = &gObj[iSelupanIndex];

	FLOAT iSelupanMAXLife = lpObj->MaxLife + lpObj->AddLife;

	lpObj->Life = iSelupanMAXLife * iSelupanHPPercent / 100;
}

// 세루판의 ObjIndex를 찾아 세팅한다.
void CRaklionSelupan::InitSelupanIndex()
{
	// 모든 객체중 세루판의 인덱스와 일치하는 객체의 배열 인덱스를 저장한다.
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
// 세루판을 리젠해준다.
BOOL CRaklionSelupan::RegenSelupan()
{
	INT iSelupanPosNum = -1;
	for( int n=0; n<gMSetBase.m_Count; n++ )
	{
		// 세루판의 최초 스크립트 좌표를 찾기위해 테이블 넘버를 찾는다.
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

	// 보스존에 있는 개체들의 뷰포트 갱신
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

	// 세루판의 데미지 백업
	m_iSelupanMinDamage = lpObj->m_AttackDamageMin;
	m_iSelupanMaxDamage = lpObj->m_AttackDamageMax;

	return TRUE;
}

// 세루판을 없앤다.
void CRaklionSelupan::HideSelupan()
{
	INT iSelupanIndex = GetSelupanObjIndex();

	gObjViewportListProtocolDestroy( &gObj[iSelupanIndex] );
	gObjViewportClose( &gObj[iSelupanIndex] );

	gObjDel ( iSelupanIndex );
}

// 세루판의 첫번째 타겟을 얻어온다.
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

// 세루판의 다음 스킬을 쓸때까지 딜레이를 설정한다.
void CRaklionSelupan::SetSelupanSkillDelay( INT iDelay )
{
	if( iDelay < 0 || iDelay > RAKLION_SELUPAN_SKILL_DELAY_MAX )
	{
		// 에러 발생
		LogAddC( LOGC_RED, "[ RAKLION ][ SetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay );
		return;
	}

	m_iSkillDelay = iDelay;
}

// 세루판의 다음 스킬을 쓸때까지의 딜레이를 얻어온다.
INT CRaklionSelupan::GetSelupanSkillDelay()
{
	if( m_iSkillDelay < 0 || m_iSkillDelay > RAKLION_SELUPAN_SKILL_DELAY_MAX )
	{
		// 에러 발생
		LogAddC( LOGC_RED, "[ RAKLION ][ GetSelupanSkillDelay ] SkillDelay error : Delay(%d)", m_iSkillDelay );
		return -1;
	}

	return m_iSkillDelay;
}

// 현재 광폭화의 단계를 얻어온다.
INT CRaklionSelupan::GetBerserkLevel()
{
	return m_iNowBerserkLevel;
}

// 세루판의 최소, 최대 데미지를 입력 받은 값으로 설정한다.
void CRaklionSelupan::InitSelupanDamage( INT iMinDamage, INT iMaxDamage )
{
	m_iSelupanMinDamage = iMinDamage;
	m_iSelupanMaxDamage = iMaxDamage;
}

//-------------------------------------------------------------------------------------------
// 세루판의 ObjIndex를 세팅한다.
void CRaklionSelupan::SetSelupanObjIndex( int iIndex )
{
	if( iIndex < 0 || iIndex > MAX_MONSTER )
	{
		// 에러 발생
		LogAddC( LOGC_RED, "[ RAKLION ][ SetSelupanObjIndex ] SelupanObjIndex error : Index(%d)", iIndex );
		return;
	}

	m_iSelupanObjIndex = iIndex;
}

// 세루판의 ObjIndex를 얻어온다.
INT CRaklionSelupan::GetSelupanObjIndex()
{
	// 세루판 인덱스를 세팅해준다.
	InitSelupanIndex();

	return m_iSelupanObjIndex;
}

//-------------------------------------------------------------------------------------------
// 세루판의 소환 몬스터들을 리젠한다.
void CRaklionSelupan::RegenSummonMonster()
{
	LPOBJECTSTRUCT lpObj	= NULL;
	INT iPosTableNum		= -1;
	INT iClass				= -1;

	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		lpObj = &gObj[iCount];

		// 라클리온 보스존의 몬스터가 아니면 return
		if( lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS )
			continue;

		if( lpObj->Class == RAKLION_COOLUTIN || lpObj->Class == RAKLION_IRON_KNIGHT )
		{
			// 현재 살아 있는 몬스터는 리젠하지 않는다.
			if( lpObj->Connected==3 && lpObj->DieRegen==0 && lpObj->Live==1 )
				continue;

			// 몬스터의 클래스 백업
			iClass = lpObj->Class;

			// 몬스터의 위치테이블 번호 백업
			iPosTableNum = lpObj->m_PosNum;
			
#ifdef MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
			// 기존의 객체는 초기화
			gObjCharZeroSet( iCount );
			gObjDel( lpObj->m_Index );
			lpObj->Class = 0;
#endif // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205

			// 몬스터 객체를 추가한다.
			int result = gObjAddMonster( MAP_INDEX_RAKLION_BOSS );
			lpObj = &gObj[result];

			if( result >= 0 )		// 몬스터 객체 추가 성공
			{
				// 몬스터 위치 세팅
				gObjSetPosMonster( result, iPosTableNum );

				// 몬스터 정보 세팅
				gObjSetMonster( result, iClass );

				lpObj->Live = 1;
				lpObj->DieRegen = 0;

#ifdef MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				// 기존의 객체는 초기화
				//gObjCharZeroSet( iCount );
#else // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				// 기존의 객체는 초기화
				gObjCharZeroSet( iCount );
#endif // MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205
				
				LogAddTD("[RAKLION] Summoned Monster : %s => X-(%d) Y-(%d)", lpObj->Name, lpObj->X, lpObj->Y ); 
			}
			else					// 몬스터 객체 추가 실패
			{
				//error
				LogAddTD( "[RAKLION] CreateSelupan error" );
				return;
			}
		}
	}	
}

// 소환된 몬스터를 없앤다.
void CRaklionSelupan::DeleteSummonMonster()
{
	LPOBJECTSTRUCT lpObj = NULL;

	for( INT iCount=0; iCount<MAX_MONSTER; ++iCount )
	{
		lpObj = &gObj[iCount];

		// 라클리온 보스존의 몬스터가 아니면 return
		if( lpObj->MapNumber != MAP_INDEX_RAKLION_BOSS )
			continue;

		// 쿨러틴(457), 아이언나이트(458)을 찾는다.
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