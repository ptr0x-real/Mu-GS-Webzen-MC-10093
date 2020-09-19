#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714

#include "TMonsterSkillElement.h"

#include "User.h"
#include "ObjUseSkill.h"
#include "Include\ReadScript.h"

#include "gObjMonster.h"
#include "Gamemain.h"

BOOL					TMonsterSkillElement::s_bDataLoad = FALSE;
TMonsterSkillElement	TMonsterSkillElement::s_MonsterSkillElementArray[MAX_MONSTER_SKILL_ELEMENT];

TMonsterSkillElement::TMonsterSkillElement()
{
	Reset();
}

TMonsterSkillElement::~TMonsterSkillElement()
{

}

void TMonsterSkillElement::Reset()
{
	m_iSuccessRate		= _INVALID;
	m_iContinuanceTime	= _INVALID;
	
	m_iIncAndDecType	= _INVALID;
	m_iIncAndDecValue	= _INVALID;

	m_iNullifiedSkill	= _INVALID;

	m_iCharacterClass	= _INVALID;
	m_iCharacterLevel	= _INVALID;

	m_iElementType		= _INVALID;
	m_iElementNumber	= _INVALID;
	ZeroMemory( m_szElementName, MAX_SKILLELEMENTNAME );
}

int TMonsterSkillElement::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster Element] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster Element] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllSkillElement();

		SMDToken Token;
		
		int iType = -1;
		
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{	
					// 1. >	데이터 초기화
					char	szElementName[MAX_SKILLELEMENTNAME] = {0,};
					int		iElementNumber	= _INVALID;
					int		iElementType	= _INVALID;
					int		iSuccessRate	= _INVALID;
					int		iContinuanceTime= _INVALID;	
					int		iIncAndDecType	= _INVALID;
					int		iIncAndDecValue	= _INVALID;
					int		iNullifiedMagic = _INVALID;
					int		iCharacterClass	= _INVALID;
					int		iCharacterLevel	= _INVALID;

					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iElementNumber	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(szElementName, TokenString, MAX_SKILLELEMENTNAME);
					Token = GetToken(); iElementType	= (int)TokenNumber;
					Token = GetToken(); iSuccessRate	= (int)TokenNumber;
					Token = GetToken(); iContinuanceTime= (int)TokenNumber;
					Token = GetToken(); iIncAndDecType	= (int)TokenNumber;
					Token = GetToken(); iIncAndDecValue	= (int)TokenNumber;
					Token = GetToken(); iNullifiedMagic	= (int)TokenNumber;
					Token = GetToken(); iCharacterClass	= (int)TokenNumber;
					Token = GetToken(); iCharacterLevel	= (int)TokenNumber;

					// 3. >	데이터 유효성 검사
					if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
					{
						MsgBox("[Monster Element] - ElementNumber(%d) Error (%s) File. ", iElementNumber, lpszFileName);
						continue;
					}

					char szMsg[200] = {0,};
					wsprintf( szMsg, "☆Num:%d,Name:%s,Type:%d,Rate:%d,Time:%d,I&DType:%d,I&DValue:%d,Null:%d,Class:%d, Level:%d\n", 
						iElementNumber, szElementName, iElementType, iSuccessRate, iContinuanceTime,
						iIncAndDecType, iIncAndDecValue, iNullifiedMagic, iCharacterClass, iCharacterLevel );
					OutputDebugString(szMsg);

					// 4. > 데이터 세팅
					s_MonsterSkillElementArray[iElementNumber].m_iSuccessRate		= iSuccessRate;
					s_MonsterSkillElementArray[iElementNumber].m_iContinuanceTime	= iContinuanceTime;
					
					s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecType		= iIncAndDecType;
					s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecValue	= iIncAndDecValue;
					
					s_MonsterSkillElementArray[iElementNumber].m_iNullifiedSkill	= iNullifiedMagic;
					
					s_MonsterSkillElementArray[iElementNumber].m_iCharacterClass	= iCharacterClass;
					s_MonsterSkillElementArray[iElementNumber].m_iCharacterLevel	= iCharacterLevel;
					
					s_MonsterSkillElementArray[iElementNumber].m_iElementType		= iElementType;
					s_MonsterSkillElementArray[iElementNumber].m_iElementNumber		= iElementNumber;
					
					CopyMemory( s_MonsterSkillElementArray[iElementNumber].m_szElementName, szElementName, MAX_SKILLELEMENTNAME );
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster Element] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster Element] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	 TMonsterSkillElement::DelAllSkillElement()
{
	for( int i=0; i<MAX_MONSTER_SKILL_ELEMENT; i++ )
	{
		s_MonsterSkillElementArray[i].Reset();
	}

	return 0;
}

TMonsterSkillElement* TMonsterSkillElement::FindSkillElement( int iElementNumber )
{
	if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
	{
		LogAddTD("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ", iElementNumber);
		return NULL;
	}

	if( s_MonsterSkillElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &s_MonsterSkillElementArray[iElementNumber];
	}
	else
		LogAddTD("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ", iElementNumber);
	
	return NULL;
}

void TMonsterSkillElement::ForceSkillElement( int iIndex, int iTargetIndex )
{
//	LogAddC(LOGC_RED, "[Force Skill-Element] Applying (%s) Element ", GetElementName());

	// 1. > General value Checking
	
	// 성공 확률 검사
	if( rand() % MAX_MONSTER_SKILL_SUCCESS_RATE > m_iSuccessRate)
	{
//		LogAddC(LOGC_RED, "[Force Skill-Element] Failed to apply (%s) Element by SuccessRate", GetElementName());
		return;
	}
	
	// 2. > Applying
	switch( m_iElementType )
	{
	case MSE_TYPE_STUN				:
		ApplyElementStun			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_MOVE				:
		ApplyElementMove			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_HP				:
		ApplyElementHP				( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_MP				:
		ApplyElementMP				( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_AG				:
		ApplyElementAG				( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_DEFENSE			:
		ApplyElementDefense			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_ATTACK			:
		ApplyElementAttack			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_DURABILITY		:
		ApplyElementDurability		( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_SUMMON			:
		ApplyElementSummon			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_PUSH				:
		ApplyElementPush			( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_STAT_ENERGY		:
		ApplyElementStatEnergy		( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_STAT_STRENGTH		:
		ApplyElementStatStrength	( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_STAT_DEXITERITY	:
		ApplyElementStatDexiterity	( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_STAT_VITALITY		:
		ApplyElementStatVitality	( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_REMOVE_SKILL		:
		ApplyElementRemoveSkill		( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_RESIST_SKILL		:
		ApplyElementResistSkill		( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_IMMUNE_SKILL		:
		ApplyElementImmuneSkill		( iIndex, iTargetIndex );
		break;

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724
	case MSE_TYPE_TELEPORT_SKILL:
		ApplyElementTeleportSkill	( iIndex, iTargetIndex );
		break;
	case MSE_TYPE_DOUBLE_HP			:	
		ApplyElementDoubleHP		( iIndex, iTargetIndex );
		break;		
	case MSE_TYPE_POISON			:
		ApplyElementPoison			(iIndex, iTargetIndex);
		break;
	case MSE_TYPE_NORMALATTACK		:
		ApplyElementNormalAttack	(iIndex, iTargetIndex);
		break;
#endif				
		
#ifdef ADD_RAKLION_20080408
	case MSE_TYPE_IMMUNE_ALL_SKILL		:
		ApplyElementImmuneAllSkill	(iIndex, iTargetIndex);
		break;
#endif // ADD_RAKLION_20080408
	};

}

int	TMonsterSkillElement::ApplyElementStun( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	// -1인 경우 지속, 나머지는 /1000하여 준다.
	int iBuffTime = 0;
	if( m_iContinuanceTime == -1 )
	{
		iBuffTime = BUFFTIME_INFINITE;
	}
	else
	{
#ifdef MODIFY_RAKLION_BUGFIX_20080707		// 초 단위로 입력되므로 /1000하면 안된다.
		iBuffTime = m_iContinuanceTime;
#else
		iBuffTime = m_iContinuanceTime / 1000;
#endif // MODIFY_RAKLION_BUGFIX_20080707
	}

	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, iBuffTime );
#else
	lpTargetObj->m_iSkillStunTime = m_iContinuanceTime;
	
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_STUNED );
	GCStateInfoSend(lpTargetObj, 1, STATE_STUNED);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjSetPosition( iTargetIndex, lpTargetObj->X, lpTargetObj->Y);
	
	return 0;
}

int	TMonsterSkillElement::ApplyElementMove( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	return 0;
}

int	TMonsterSkillElement::ApplyElementHP( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	int iIncDecValue = _ZERO;

	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTINC || m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC 
			|| m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTINC || m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			iIncDecValue = (lpTargetObj->Life) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}
		
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTINC || m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC 
			|| m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTINC || m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC || m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}

		// 3. > 주기 적용
		if( m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPTime		= m_iContinuanceTime;
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP			= iIncDecValue;
			
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle		= MSE_INCDEC_TYPE_CYCLE_PERCENT;

			if( m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle	= MSE_INCDEC_TYPE_CYCLE_CONSTANT;
			else
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle	= MSE_INCDEC_TYPE_CYCLE_PERCENT;
		}
	}

	lpTargetObj->Life += iIncDecValue;

	if( lpTargetObj->Life < 0 )
	{
		lpTargetObj->Life = 0;
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0);
#endif
		return _SUCCESS;
	}
	
	if( lpTargetObj->Life > lpTargetObj->MaxLife + lpTargetObj->AddLife )
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0);
#endif
		return _SUCCESS;
	}

#ifdef ADD_SHIELD_POINT_01_20060403
	GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
	GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0);
#endif

	return 0;
}

int	TMonsterSkillElement::ApplyElementMP( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];
	
	int iIncDecValue = _ZERO;

	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (lpTargetObj->Mana) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
			
		}
		
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC)
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->Mana += iIncDecValue;

	if( lpTargetObj->Mana < 0 )
	{
		lpTargetObj->Mana = 0;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpObj->BP);
		return _SUCCESS;
	}
	
	if( lpTargetObj->Mana > lpTargetObj->MaxMana + lpTargetObj->AddMana )
	{
		lpTargetObj->Mana = lpTargetObj->MaxMana + lpTargetObj->AddMana;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
		return _SUCCESS;
	}

	GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

	return 0;
}

int	TMonsterSkillElement::ApplyElementAG( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	int iIncDecValue = _ZERO;

	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (lpTargetObj->BP) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
			
		}
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC)
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}
	
	lpTargetObj->BP += iIncDecValue;

	if( lpTargetObj->BP < 0 )
	{
		lpTargetObj->BP = 0;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
		return _SUCCESS;
	}
	
	if( lpTargetObj->Mana > lpTargetObj->MaxBP + lpTargetObj->AddBP )
	{
		lpTargetObj->Mana = lpTargetObj->MaxBP + lpTargetObj->AddBP;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
		return _SUCCESS;
	}

	GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

	return 0;
}

int	TMonsterSkillElement::ApplyElementDefense( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	int iIncDecValue = _ZERO;

	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (lpTargetObj->m_Defense) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
			
		}
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC)
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime	= m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefense		= iIncDecValue;

	return 0;
}

int	TMonsterSkillElement::ApplyElementAttack( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	int iIncDecValue = _ZERO;

	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (lpTargetObj->m_Defense) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
			
		}
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC)
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime	= m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttack		= iIncDecValue;

	return 0;
}

int	TMonsterSkillElement::ApplyElementDurability( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	if( lpTargetObj->Type != OBJTYPE_CHARACTER )
		return 0;

	int iEquipmentPos = rand()%(EQUIPMENT_WING-EQUIPMENT_HELM+1) + EQUIPMENT_HELM;
	
	CItem	*lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];
	
	if( lpEquipment == NULL || !lpEquipment->IsItem() )
		return 0;

	int	iDurEquipment	= lpEquipment->m_Durability;
	int iIncDecValue	= _ZERO;
	
	if( m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		// 1. > 퍼센트로 적용
		if( m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (iDurEquipment) * m_iIncAndDecValue / 100;
			
			// 1.2 > 감소
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
			
		}
		
		// 2. > 상수 값으로 적용
		if( m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC)
		{
			iIncDecValue = m_iIncAndDecValue;
			
			// 2.2 > 감소 
			if( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	iDurEquipment += iIncDecValue;

	if( lpEquipment != NULL )
	{
		lpEquipment->m_Durability = iDurEquipment;

		if( lpEquipment->m_Durability < 0 )
			lpEquipment->m_Durability = 0;

		GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
	}

	return 0;
}

int	TMonsterSkillElement::ApplyElementSummon( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	if( lpObj->Class == 459 && lpObj->Connected==3 && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS )
	{
		if( iIndex == iTargetIndex )
		{
			LogAddTD( "[TMonsterSkillElement][ApplyElementSummon] Selupan use summon." );
			return 0;
		}
	}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

	if (lpTargetObj->Connected < CSS_GAMEPLAYING || lpTargetObj->Type != OBJTYPE_MONSTER)
		return 0;

	// . > m_iIncAndDecValue 값이 소환자로 부터의 소환 반경 (RADIUS) 이다 

	// 리젠을 시킨다.
	lpTargetObj->Life					= lpTargetObj->MaxLife+lpTargetObj->AddLife;
	lpTargetObj->Mana					= lpTargetObj->MaxMana+lpTargetObj->AddMana;
	lpTargetObj->Live					= 1;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_POISON );
	gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_FREEZE );
	gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_STONE );
#else
	lpTargetObj->m_PoisonBeattackCount	= 0;
	lpTargetObj->m_ColdBeattackCount	= 0;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	lpTargetObj->m_ViewState			= 0;
	lpTargetObj->Teleport				= 0;
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjClearBuffEffect( lpTargetObj, CLEAR_TYPE_LOGOUT );
#else
	lpTargetObj->m_ViewSkillState		= 0;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	for( INT i=0; i<MAXSELFDEFENSE; i++)
	{
		lpTargetObj->SelfDefenseTime[i]	= 0;
	}

	gObjTimeCheckSelfDefense(lpTargetObj);
	gObjViewportListProtocolDestroy(lpTargetObj);
	gObjViewportClose(lpTargetObj);

	lpTargetObj->m_ActState.Attack		= 0;
	lpTargetObj->m_ActState.EmotionCount= 0;
	lpTargetObj->m_ActState.Escape		= 0;
	lpTargetObj->m_ActState.Move		= 0;
	lpTargetObj->m_ActState.Rest		= 0;
	lpTargetObj->m_ActState.Emotion		= 0;
	lpTargetObj->TargetNumber			= -1;
	lpTargetObj->NextActionTime			= 5000;
	
	// . > 소환자로 부터 특정 범위내로 리젠 시킨다.
	if( m_iIncAndDecValue < 0 )
		m_iIncAndDecValue = 10;

	UCHAR cX, cY;
	BOOL bGetPosition = FALSE;
	
	INT iCount = 100;
	while (iCount--) 
	{
		cX = rand() % (m_iIncAndDecValue + 1) * (rand() % 2 ? 1 : -1) + lpObj->X;
		cY = rand() % (m_iIncAndDecValue + 1) * (rand() % 2 ? 1 : -1) + lpObj->Y;
		
		BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);
		if( btMapAttr == MAP_ATTR_NONE )	
		{
			bGetPosition = TRUE;
			break;
		}
	}

	if (!bGetPosition) 
	{
		lpObj->Live			= 0;
		lpObj->m_State		= OBJST_DYING;	
		lpObj->RegenTime	= GetTickCount();
		lpObj->DieRegen		= 1;			
		return FALSE;
	}

	lpTargetObj->X			= cX;
	lpTargetObj->Y			= cY;

	lpTargetObj->MTX		= lpTargetObj->X;
	lpTargetObj->MTY		= lpTargetObj->Y;
	lpTargetObj->TX			= lpTargetObj->X;
	lpTargetObj->TY			= lpTargetObj->Y;
	lpTargetObj->StartX		= (BYTE)lpTargetObj->X;
	lpTargetObj->StartY		= (BYTE)lpTargetObj->Y;

	gObjMonsterHitDamageInit(lpTargetObj);
		
	CreateFrustrum(lpTargetObj->X, lpTargetObj->Y, lpTargetObj->m_Index);
	
	lpTargetObj->DieRegen	= 0;
	lpTargetObj->m_State	= OBJST_CREATE;
	
	// 소환 전에 Agro를 초기화 한다.
	lpTargetObj->m_Agro.ResetAll();
	
	gObjViewportListCreate(lpTargetObj->m_Index);

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725

	return 0;
}

int	TMonsterSkillElement::ApplyElementPush( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	if( lpObj->Class==459 && lpObj->Connected==CSS_GAMEPLAYING && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS )
	{
		// 세루판의 최초스킬의 타겟이 자기자신일 경우 return
		if( iIndex == iTargetIndex )
			return 0;
	}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
	
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	// 밀기 공격 : m_iNullifiedSkill - 딜레이 시간 설정
	//             m_iIncAndDecValue - 타일거리
	if ( m_iNullifiedSkill == _INVALID )
		m_iNullifiedSkill = 0;

#ifdef MODIFY_MONSTERSKILL_SPRING_POISON_BUGFIX_20060817	
	// 기본값 3 타일로 설정
	if ( m_iIncAndDecValue < 1 )
		m_iIncAndDecValue = 3;
#endif // MODIFY_MONSTERSKILL_SPRING_POISON_BUGFIX_20060817

	if ( m_iNullifiedSkill == 0 ) // 즉시
		gObjBackSpring2( lpTargetObj, lpObj, m_iIncAndDecValue );
	else
		gObjAddMsgSendDelay( lpObj, MSG_MONSTER_SKILL_SPRING, iTargetIndex, m_iNullifiedSkill, m_iIncAndDecValue);
#else
	gObjBackSpring2( lpTargetObj, lpObj, 3 );
#endif
	
	return 0;
}

int	TMonsterSkillElement::ApplyElementStatEnergy( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	return 0;
}

int	TMonsterSkillElement::ApplyElementStatStrength( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	return 0;
}	

int	TMonsterSkillElement::ApplyElementStatDexiterity( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	return 0;
}

int	TMonsterSkillElement::ApplyElementStatVitality( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	return 0;
}

int	TMonsterSkillElement::ApplyElementRemoveSkill( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	gObjUseSkill.RemoveSpecificSkillState( lpTargetObj, m_iNullifiedSkill );
	
	return 0;
}

int	TMonsterSkillElement::ApplyElementResistSkill( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistNumber  = m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistTime	= m_iContinuanceTime;

	return 0;
}

int	TMonsterSkillElement::ApplyElementImmuneSkill( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber  = m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime	= m_iContinuanceTime;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iBuffTime = 0;

	if( m_iContinuanceTime == -1 )
	{
		iBuffTime = BUFFTIME_INFINITE;
	}
	else
	{
		iBuffTime = m_iContinuanceTime / 1000;
	}
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, iBuffTime );
#else
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_IMMUNE_TO_SKILL );
	GCStateInfoSend(lpTargetObj, 1, STATE_IMMUNE_TO_SKILL);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	return 0;
}

#ifdef MODIFY_NEW_MONSTER_SKILL_01_20060724

#include "Common\winutil.h"
int	TMonsterSkillElement::ApplyElementTeleportSkill( int iIndex, int iTargetIndex )
{
	// 범위 : m_iNullifiedSkill;
#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531

	if( !CHECK_LIMIT(iIndex, MAX_OBJECT) )
		return 0;

	// 스턴이 걸려 있으면 순간이동 할 수 없다.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( &gObj[iIndex], BUFFTYPE_STUN ) == true )
	#else
	if( gObj[iIndex].m_iSkillStunTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return 0;
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태에서는 순간이동을 할 수 없다
	if( gObjCheckUsedBuffEffect( &gObj[iIndex], BUFFTYPE_SLEEP ) == true )
	{
		return 0;
	}
#endif
	
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	BYTE x, y;
	// 랜덤하게 위치 선택 : 개선 필요.

	int depth = 3+rand()%4; // 3 ~ 6
	if ( rand()%2 == 0 )
		x = lpObj->X + depth;
	else
		x = lpObj->X - depth;
	if ( rand()%2 == 0 )
		y = lpObj->Y + depth;
	else
		y = lpObj->Y - depth;


	if( !gObjCheckTeleportArea(iIndex, x, y) )
	{
		LogAddC(LOGC_RED, "[%s][%s] Try Teleport Not Move Area [%d,%d]", 
			   lpObj->AccountID, lpObj->Name, x, y);
		return 0;
	}


	PMSG_MAGICATTACK_RESULT	pAttack;
	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD 
	pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TELEPORT );
	pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TELEPORT );
#else
	pAttack.MagicNumber   = AT_SKILL_TELEPORT;
#endif
	pAttack.SourceNumberH = HIBYTE(iIndex);
	pAttack.SourceNumberL = LOBYTE(iIndex);	
	pAttack.TargetNumberH = HIBYTE(iIndex);
	pAttack.TargetNumberL = LOBYTE(iIndex);	

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(iIndex, (LPBYTE)&pAttack, pAttack.h.size);

	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);


	// 텔레포트 사용
	LogAddC(LOGC_RED, "[%s] Warp [%d,%d] -> [%d,%d]", lpObj->Name, lpObj->X, lpObj->Y, x, y);
	gObjTeleportMagicUse(iIndex, x, y);

	// 타겟해제
	lpObj->TargetNumber = -1;

	return 0;
}

int TMonsterSkillElement::ApplyElementDoubleHP( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	if( lpTargetObj->Connected < CSS_GAMEPLAYING )
		return 0;

	// 일반 공격 2회 공격
	gObjAddMsgSendDelay( lpObj, MSG_ATTACK_NORMAL, iTargetIndex, 400);
	gObjAddMsgSendDelay( lpObj, MSG_ATTACK_NORMAL, iTargetIndex, 1000);
#endif

	return 0;
}

int TMonsterSkillElement::ApplyElementPoison( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	if( lpTargetObj->Connected < CSS_GAMEPLAYING )
		return 0;

	// 독 스킬 공격 : m_iNullifiedSkill - 딜레이 시간 설정
	//                m_iIncAndDecValue - 지속시간 (단위 : 초)
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
	{
		if( m_iNullifiedSkill < 1 )
		{
			if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
			{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
				lpTargetObj->lpAttackObj = lpObj;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
				gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, m_iIncAndDecValue );
			}
		}
		else
		{
			gObjAddMsgSendDelay( lpObj, MSG_MONSTER_SKILL_POISON, iTargetIndex,  m_iNullifiedSkill, m_iIncAndDecValue);
		}
	}
#else
	#ifdef ADD_SKILL_WITH_COMBO		
	if( lpTargetObj->m_PoisonType == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
	#else
	if( lpTargetObj->m_PoisonBeattackCount == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
	#endif	//#ifdef ADD_SKILL_WITH_COMBO
	{

	#ifdef MODIFY_MONSTERSKILL_SPRING_POISON_BUGFIX_20060817
		if ( m_iNullifiedSkill < 1 ) // 즉시 처리
		{
			if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
				{
		#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonType = AT_SKILL_POISON;
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonBeattackCount	= m_iIncAndDecValue;	// 초 설정
					lpTargetObj->lpAttackObj			= lpObj;
					lpTargetObj->m_ViewSkillState		|= 0x01; //	1번째 비트
					GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
				}
		}
		else
		{
			gObjAddMsgSendDelay( lpObj, MSG_MONSTER_SKILL_POISON, iTargetIndex,  m_iNullifiedSkill, m_iIncAndDecValue);
		}
	#else
		gObjAddMsgSendDelay( lpObj, MSG_MONSTER_SKILL_POISON, iTargetIndex,  m_iNullifiedSkill, m_iIncAndDecValue);
	#endif // MODIFY_MONSTERSKILL_SPRING_POISON_BUGFIX_20060817
	}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	return 0;
}

int TMonsterSkillElement::ApplyElementNormalAttack( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];

	if( lpTargetObj->Connected < CSS_GAMEPLAYING )
		return 0;
	
	// 일반 공격 : m_iNullifiedSkill - 딜레이 시간 설정
	if ( m_iNullifiedSkill == _INVALID )
		m_iNullifiedSkill = 0;

	// 특정 아이템을 장착하지 않은 경우 즉사처리
	// m_iIncAndDecType : 아이템 Type
	// m_iIncAndDecValue : 아이템 Index
	if ( m_iIncAndDecType != _INVALID && m_iIncAndDecValue != _INVALID )
	{
#ifdef MODIFY_BATTLE_OF_NIGHTMARE_MOONSTONE_PENDANT_BUGFIX_20080731
		if ( ( lpTargetObj->pInventory[EQUIPMENT_AMULET].m_Type		== MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue)
			&& lpTargetObj->pInventory[EQUIPMENT_AMULET].m_Durability > 0)
			|| (lpTargetObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue) 
			&& lpTargetObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability > 0)
			|| (lpTargetObj->pInventory[EQUIPMENT_RING_LEFT].m_Type	== MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue) 
			&& lpTargetObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability > 0)
			)
		{	// 해당 아이템을 착용 하였고 내구도가 0 이상이면 즉사 처리 안됨
		}
		else	// 해당 아이템을 착용하지 않았거나 내구도가 0이면 즉사 처리
		{
			// 즉사 처리
			lpTargetObj->Life			= 0.0;
#ifdef ADD_SHIELD_POINT_01_20060403
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0, 0);
#else
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0);
#endif
			return 0;
			
		}
#else
		if ( lpTargetObj->pInventory[EQUIPMENT_AMULET].m_Type		!= MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue) 
			&& lpTargetObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type != MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue) 
			&& lpTargetObj->pInventory[EQUIPMENT_RING_LEFT].m_Type	!= MAKE_ITEMNUM(m_iIncAndDecType, m_iIncAndDecValue) 
			)
		{
			// 즉사 처리
			lpTargetObj->Life			= 0.0;
#ifdef ADD_SHIELD_POINT_01_20060403
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0, 0);
#else
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0);
#endif
			return 0;
			
		}
#endif // MODIFY_BATTLE_OF_NIGHTMARE_MOONSTONE_PENDANT_BUGFIX_20080731
	}


	if ( m_iNullifiedSkill == 0 ) // 즉시 공격 
		gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
	else
		gObjAddMsgSendDelay( lpObj, MSG_ATTACK_NORMAL, iTargetIndex, m_iNullifiedSkill);	

	return 0;
}

#endif // MODIFY_NEW_MONSTER_SKILL_01_20060724

#ifdef ADD_RAKLION_20080408
int	TMonsterSkillElement::ApplyElementImmuneAllSkill( int iIndex, int iTargetIndex )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];
	
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneAllTime	= m_iContinuanceTime;
	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iBuffTime = 0;
	
	if( m_iContinuanceTime == -1 )
	{
		iBuffTime = BUFFTIME_INFINITE;
	}
	else
	{
		iBuffTime = m_iContinuanceTime / 1000;
	}
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, iBuffTime );
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, iBuffTime );
#else
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_IMMUNE_TO_SKILL );
	GCStateInfoSend(lpTargetObj, 1, STATE_IMMUNE_TO_SKILL);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	return 0;
}
#endif // ADD_RAKLION_20080408
#endif