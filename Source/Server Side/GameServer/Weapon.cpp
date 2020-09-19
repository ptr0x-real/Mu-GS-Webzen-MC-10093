// Weapon.cpp: implementation of the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Weapon.h"

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#include "Common\winutil.h"
#include "User.h"
#include "ObjAttack.h"
#include "NpcTalk.h"

CWeapon g_CsNPC_Weapon;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeapon::CWeapon()
{
	for( int i=0; i<MAX_WEAPON_CAL_DAMAGE_INFO; i++ )
	{
		m_WeaponDamagedTargetInfo[i].RESET();
	}

	for( int i=0; i<MAX_WEAPON_DAMAGED_TARGET_INFO; i++ )
	{
		m_WeaponCalDamageInfo[i].RESET();
	}
	
}

CWeapon::~CWeapon()
{

}

BOOL CWeapon::SetWeaponCalDamageInfo( WORD wObjIndex, BYTE btTargetX, BYTE btTargetY,  INT iDelayTime )
{
	for( int i=0; i<MAX_WEAPON_CAL_DAMAGE_INFO; i++ )
	{

		if( m_WeaponCalDamageInfo[i].m_IsUsed == FALSE )
		{
			m_WeaponCalDamageInfo[i].m_iWeaponObjIndex	= wObjIndex;
			m_WeaponCalDamageInfo[i].m_iTargetX			= btTargetX;
			m_WeaponCalDamageInfo[i].m_iTargetY			= btTargetY;
			m_WeaponCalDamageInfo[i].m_iLimitTime		= GetTickCount() + iDelayTime;

			m_WeaponCalDamageInfo[i].m_IsUsed			= TRUE;
			
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWeapon::GetWeaponCalDamageInfo( WORD wObjIndex, BYTE &btTargetX, BYTE &btTargetY )
{
	for( int i=0; i<MAX_WEAPON_CAL_DAMAGE_INFO; i++ )
	{

		if( m_WeaponCalDamageInfo[i].m_IsUsed == TRUE 
			&&  m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == wObjIndex )
		{
			btTargetX = m_WeaponCalDamageInfo[i].m_iTargetX;
			btTargetY = m_WeaponCalDamageInfo[i].m_iTargetY;

			m_WeaponCalDamageInfo[i].RESET();
			
			return TRUE;
		}
	}

	return FALSE;
}

INT	CWeapon::GetAttackDamage( INT iObjClass )
{
	INT iDamage = 0;
	if( iObjClass == NPC_SIEGE_MACHINE_ATTACK )
	{
		iDamage = 300;
	}
	else if( iObjClass == NPC_SIEGE_MACHINE_DEFENSE )
	{
		iDamage = 200;
	}

	return iDamage;
}

BOOL CWeapon::GetTargetPointXY( INT iObjClass, INT iTargetPointIndex, BYTE& btX, BYTE& btY, BOOL bRandomPos )
{
	if( iObjClass == NPC_SIEGE_MACHINE_ATTACK )
	{
		if( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_WEAPON_CS_ATTACKER_TARETINDEX )
		{
			INT iRangeX = WEAPON_CS_ATTACKER[iTargetPointIndex][2] - WEAPON_CS_ATTACKER[iTargetPointIndex][0];
			INT iRangeY = WEAPON_CS_ATTACKER[iTargetPointIndex][3] - WEAPON_CS_ATTACKER[iTargetPointIndex][1];

			if( bRandomPos )
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex][0] + rand() % iRangeX;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex][1] + rand() % iRangeY;
			}
			else
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex][0] + iRangeX / 2;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex][1] + iRangeY / 2;
			}
			

		}
		
	}

	if( iObjClass == NPC_SIEGE_MACHINE_DEFENSE )
	{
		if( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_WEAPON_CS_ATTACKER_TARETINDEX )
		{
			INT iRangeX = WEAPON_CS_DEFENSER[iTargetPointIndex][2] - WEAPON_CS_DEFENSER[iTargetPointIndex][0];
			INT iRangeY = WEAPON_CS_DEFENSER[iTargetPointIndex][3] - WEAPON_CS_DEFENSER[iTargetPointIndex][1];
			
			if( bRandomPos )
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex][0] + rand() % iRangeX;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex][1] + rand() % iRangeY;
			}
			else
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex][0] + iRangeX / 2;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex][1] + iRangeY / 2;
			}
			
		}
	}

	return TRUE;
}

BOOL CWeapon::MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iSkill, INT iSkillSuccess, int& bAllMiss )
{
	return TRUE;
}

BOOL CWeapon::Attack( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, INT iCriticalDamage, INT iActionType )
{
	INT				iSkill = 0;
	INT				iSkillSuccess = 1;

	BYTE			MsgDamage = 0;

	int				ManaChange = FALSE;		


	// 마법 기술 설정
	if( lpMagic != NULL )
	{
		iSkill = lpMagic->m_Skill;
	}

	if( (lpTargetObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// 운영자 캐릭은 공격을 받지 않는다
		return FALSE;
	}

	if( lpObj->MapNumber != lpTargetObj->MapNumber )
	{	// 같은 맵이 아니면, 
		return FALSE;
	}
	
	// 타겟 유저의 연결 상태 확인
	if( lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		if( gObjIsConnected(lpTargetObj) == FALSE ) 
			return FALSE;
	}
	
	// 타켓을 공격해도 되는지 물어본다.
	if( gObjAttackQ(lpTargetObj) == FALSE ) 
		return FALSE;

	lpObj->m_TotalAttackCount++;


	// 공격 지역을 체크 한다. 
//	if( !CheckAttackArea(lpObj, lpTargetObj) )
//		return FALSE;
  	
	int MSBFlag = 0;
	
	if( iCriticalDamage )
		MsgDamage = DT_CRITICAL;
	else
		MsgDamage = 0;
	
	// 공격자의 공격데미지 값을 얻는다.


	// 1. 데미지!!!!!!!!

	int iAttackDamage = GetAttackDamage( lpObj->Class );

	// 2. 공격에 따른 부가적인 영향 ( 내구력 추가 감소 등 );


	// 대상의 방어력을 얻는다
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_06_20080102
	int iTargetdefense = lpTargetObj->m_Defense;
	int iDecreaseDefense = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_DECREASE_DEFENSE );
	iTargetdefense		-= ( iTargetdefense * iDecreaseDefense ) / 100;
#else
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iSkilldefense	= gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_DEFENSE );
	iSkilldefense += gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_IMPROVE_MELEE_DEFENSE );
	int iDecreaseDefense = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_DECREASE_DEFENSE );
	int iTargetdefense	= lpTargetObj->m_Defense + iSkilldefense;
	iTargetdefense		-= ( iTargetdefense * iDecreaseDefense ) / 100;
	#else
	int iSkilldefense	= lpTargetObj->m_SkillDefense;
	int iTargetdefense	= lpTargetObj->m_Defense + iSkilldefense;
	iTargetdefense		-= iTargetdefense * lpTargetObj->m_SkillMagumReduceDefense / 100;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // MODIFY_BUFFSYSTEM_BUGFIX_06_20080102

	// 미스 체크
	int bAllMiss = FALSE;

	if( !MissCheck(lpObj, lpTargetObj, iSkill, iSkillSuccess, bAllMiss) )
		return FALSE;

	if( bAllMiss )
	{
		iAttackDamage =  iAttackDamage*30/100;
	}


	// 데미지를 감소 시킨다.
	if( lpTargetObj->DamageMinus )
	{
		int beforeDamage=iAttackDamage;
		iAttackDamage -=  iAttackDamage*lpTargetObj->DamageMinus/100;
	}

	// 때린녀석의레벨/10 보다 작을 경우 iAttackDamage 수정 
	int iTargetLevel = lpObj->Level/10;
	
	if( iAttackDamage < iTargetLevel )
	{
		if( iTargetLevel < 1 ) 
			iTargetLevel = 1;

		iAttackDamage = iTargetLevel;
	}

	// 방어스킬 사용시
	if( lpTargetObj->m_SkillNumber == AT_SKILL_BLOCKING )
		if( iAttackDamage > 1 ) 
			iAttackDamage >>= 1;

	
	// 상대가 천사정령을 가지고 있다면.. 
	if( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if( iAttackDamage > 1 ) 
		{
			float damage = (float)(iAttackDamage*8)/10;
			iAttackDamage = (int)damage;
		}
	}

	if( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem	*Wing = &lpTargetObj->pInventory[EQUIPMENT_WING];
		if( iAttackDamage > 1 ) 
		{
			if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
			{	// 날개 데미지 흡수 추가 날개는 25% + level
				float damage = (float)(iAttackDamage*(75-(Wing->m_Level*2)))/100;
				iAttackDamage = (int)damage;
			}
			else	
			{	// 날개 데미지 흡수 날개 12% + level*2
				float damage = (float)(iAttackDamage*(88-(Wing->m_Level*2)))/100;
				iAttackDamage = (int)damage;	
			}
		}		
	}


	if( gObjDenorantSprite(lpTargetObj) )
	{	
		// 디노란트 데미지 흡수 10%
		CItem	*Dinorant = &lpTargetObj->pInventory[EQUIPMENT_HELPER];

		// 5%추가 데미지 흡수 옵션 적용
		int	dinorantdecdamage = 90-Dinorant->IsDinorantReduceAttackDamaege();
		
		lpObj->Life -= 1;
		
		if( lpObj->Life < 0 ) 
		{
			lpObj->Life = 0;
		}
		else
		{
			iAttackDamage = iAttackDamage*dinorantdecdamage/100;
		}
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
	}


	if( gObjDarkHorse(lpTargetObj) )
	{	
		// 다크호스 데미지 흡수 (10+(통솔/100))
		CItem	*Darkhorse = &lpTargetObj->pInventory[EQUIPMENT_HELPER];	

		int	decdamage = 100-((30+Darkhorse->m_PetItem_Level)/2);
	
		lpTargetObj->Life -= 1;
		
		if( lpTargetObj->Life < 0 ) 
		{
			lpTargetObj->Life = 0;
		}
		else
		{
			iAttackDamage = iAttackDamage*decdamage/100;
		}
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0);
#endif
	}
	
	if( lpTargetObj->Live ) 
	{		
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && iAttackDamage > 0 )
		{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 소울바리어 : 데미지를 마나로 흡수(CWeapon)
			// 데미지를 마나로
			int iWizardSkillDefense = 0;
			int iManaRate			= 0;
			gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate );
			// 마나 감소			
			int iReplacementMana = 0;
			if( iManaRate > 0 )
				iReplacementMana = (WORD)lpTargetObj->Mana * iManaRate / 1000;	
			else
				iReplacementMana = (WORD)lpTargetObj->Mana * 2 / 100;	

			if( lpTargetObj->Mana > iReplacementMana )
			{	
				// 사용가능한 마나가 있어야 된다
				lpTargetObj->Mana -= iReplacementMana;				

				int decattackdamage = 0;
				if( iWizardSkillDefense > 0 )
					decattackdamage = iAttackDamage * iWizardSkillDefense / 100;	

				// 데미지 감소
				iAttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}

#else		
			// 마나 2%감소
			int iReplacementMana = (WORD)lpTargetObj->Mana*2/100;	
			if( lpTargetObj->Mana > iReplacementMana )
			{	
				// 사용가능한 마나가 있어야 된다
				lpTargetObj->Mana -= iReplacementMana;				
				// 데미지를 마나로
				int iWizardSkillDefense = 0;
				gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense );
				int decattackdamage = iAttackDamage * iWizardSkillDefense / 100;	
				// 데미지 감소
				iAttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}			
#endif	// MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		}
#else
		if( lpTargetObj->m_WizardSkillDefense && iAttackDamage > 0 )
		{	
			// 법사 소울바리어(데미지를 마나로 흡수 시킨다) 
			// 마나 2%감소
			int iReplacementMana = (WORD)lpTargetObj->Mana*2/100;		
			if( lpTargetObj->Mana > iReplacementMana )
			{	
				// 사용가능한 마나가 있어야 된다
				lpTargetObj->Mana -= iReplacementMana;				
				// 데미지를 마나로
				int decattackdamage = iAttackDamage*lpTargetObj->m_WizardSkillDefense/100;	
				// 데미지 감소
				iAttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		lpTargetObj->Life -= iAttackDamage;

		if( lpTargetObj->Life < 0 ) lpTargetObj->Life = 0;			
	}	
	
	if( iAttackDamage >= 1 )
	{	
		if( lpTargetObj->Type == OBJTYPE_CHARACTER )
		{	
			// 공격이 성공하면 내구도를 감소 시킨다			
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}
		
		// 공격을 당했을 때 스턴이나 투명 등 마법이 걸려 있을 경우 풀어준다.

		// 요기서 ^^

	}	

	if( iAttackDamage >= 5 )
	{		
		if( lpTargetObj->Type == OBJTYPE_MONSTER ) // 대상자가 몬스터
		{
			if( (rand()%26) == 0 )	
			{
				// 공격 당한 몬스터를 머뭇거리게 한다.
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
			}
		}
		else 
		{
			if( (rand()%4) == 0 )	
			{
				// 유니리아를 타고 있지 않으면 경직 상태를 준다.
				if( gObjUniriaSprite(lpTargetObj) == FALSE )
				{
					MSBFlag = 1;
				}
			}
		}
	}


	// 소울 바리어의 사용으로 데미지->마나 사용으로 됐을때 보내준다.
	if( ManaChange )
	{			
		GCManaSend(lpTargetObj->m_Index, (short)lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);				
	}

	lpObj->m_Rest = 0;
	
	if( iAttackDamage > 0 )
	{
		gObjLifeCheck(lpTargetObj, lpObj, iAttackDamage, 0, MSBFlag, MsgDamage, iSkill);
	}	

	return TRUE;

}

BOOL CWeapon::AddWeaponDamagedTargetInfo( INT iWeaponIndex, INT iTargetIndex, INT iDelayTime )
{
	for( int i=0; i<MAX_WEAPON_DAMAGED_TARGET_INFO; i++ )
	{
		if( m_WeaponDamagedTargetInfo[i].m_IsUsed == FALSE )
		{
			m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex		= iWeaponIndex;
			m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex		= iTargetIndex;
			m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime	= GetTickCount() + iDelayTime;	
			m_WeaponDamagedTargetInfo[i].m_IsUsed				= TRUE;
			
			return TRUE;
		}
	}
	return FALSE;
}

VOID CWeapon::WeaponAttackProc()
{
	// 1. 데미지 주기
	for( int i=0; i<MAX_WEAPON_DAMAGED_TARGET_INFO; i++ )
	{
		if( m_WeaponDamagedTargetInfo[i].m_IsUsed == TRUE )
		{
			if( m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime > GetTickCount() )
				continue;
			
			LPOBJECTSTRUCT lpTargetObj = &gObj[m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex];
			LPOBJECTSTRUCT lpWeaponObj = &gObj[m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex];

			if( gObjIsConnected( lpTargetObj ) == FALSE )
			{
				m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			if( CHECK_LIMIT(lpWeaponObj->m_Index, MAX_OBJECT) == FALSE )
			{
				m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}
			
			Attack( lpWeaponObj, lpTargetObj, NULL, 0 );
			
			m_WeaponDamagedTargetInfo[i].RESET();
			
		}
		
	}

	
}

VOID CWeapon::WeaponAct( INT iIndex )
{
	// 2. 데미지 줄것인지 판다.
	for( int i=0; i<MAX_WEAPON_CAL_DAMAGE_INFO; i++ )
	{
		if( m_WeaponCalDamageInfo[i].m_IsUsed == TRUE 
			&&  m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == iIndex )
		{

			// 제한 시간이 지났으면 강제로 데미지를 판정한다.
			if( m_WeaponCalDamageInfo[i].m_iLimitTime < GetTickCount() )
			{
				// 걍 데미지 지움.
				m_WeaponCalDamageInfo[i].RESET();
			}
		}
	}
}

#endif