// ItemAddOption.cpp: implementation of the CItemAddOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemAddOption.h"

#ifdef ITEM_ADD_OPTION_20061019

#ifdef TESTCODE
#include <windows.h>
#endif

#include "..\\common\\winutil.h"
#include "..\include\ReadScript.h"

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
#include "CashItemPeriodSystem.h"
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

CItemAddOption g_ItemAddOption;

extern ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemAddOption::CItemAddOption()
{
	Initialize();
}

CItemAddOption::~CItemAddOption()
{

}

void CItemAddOption::Load(char* chFileName)
{
	Initialize();

	if( (SMDFile = fopen( chFileName, "r" ) ) == FALSE )
	{
		// 실패
		return;
	}

	SMDToken Token;
	int iItemAddOptionIndex = 0;

	while( true )
	{
		Token = (*GetToken)();
		if( Token == END )
		{
			break;
		}

		if( Token == NUMBER )
		{
			int iType = (int)TokenNumber;
			while( true )
			{
				if( iType == 0 )		// 380 아이템 (380아이템은 380아이템 로드 부분에서 로드 함으로 무시.)
				{
					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}
				}

				if( iType == 1 )		// 할로윈 데이 이벤트 물약
				{
					int iItemNumber		= 0;
					int iItemType		= 0;
					int iItemIndex		= 0;
					int iItemOption1	= 0;
					int iItemValue1		= 0;
					int iItemOption2	= 0;
					int iItemValue2		= 0;
					int iItemValidTime	= 0;

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}

					iItemType = (int)TokenNumber;
					Token = (*GetToken)();
					iItemIndex = (int)TokenNumber;
					Token = (*GetToken)();
					iItemOption1 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValue1 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemOption2 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValue2 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValidTime = (int)TokenNumber;

					iItemNumber = MAKE_ITEMNUM( iItemType, iItemIndex );

					// 효과 취소를 위한 옵션 타입을 넣어 주어야 한다.
					m_ItemAddOption[iItemAddOptionIndex].wOptionType		= (WORD)iType;

					m_ItemAddOption[iItemAddOptionIndex].iItemNumber		= iItemNumber;
					m_ItemAddOption[iItemAddOptionIndex].iItemType			= iItemType;
					m_ItemAddOption[iItemAddOptionIndex].iItemIndex			= iItemIndex;
					m_ItemAddOption[iItemAddOptionIndex].wEffectType1		= iItemOption1;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValue1		= iItemValue1;
					m_ItemAddOption[iItemAddOptionIndex].wEffectType2		= iItemOption2;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValue2		= iItemValue2;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime	= iItemValidTime;

	#if TESTSERVER == 1
					LogAdd("%d %d %d %d %d %d %d %d",
						iItemNumber, iItemType, iItemIndex, iItemOption1, iItemValue1, iItemOption2, iItemValue2, iItemValidTime );
	#endif
					iItemAddOptionIndex++;
					if( iItemAddOptionIndex > MAX_ITEM_ADD_OPTION_ROW )
					{
						MsgBox("[ItemAddOption] Overflow ItemAddOption Table Index");
					}
				}
	#ifdef MODIFY_ITEMADDOPTION_LOAD_CATEGORY_20070712
				else if( iType >= 2 )
	#else
				else if( iType == 2 )
	#endif // MODIFY_ITEMADDOPTION_LOAD_CATEGORY_20070712
				{
					int iItemNumber		= 0;
					int iItemType		= 0;
					int iItemIndex		= 0;
					int iItemOption1	= 0;
					int iItemValue1		= 0;
					int iItemOption2	= 0;
					int iItemValue2		= 0;
					int iItemValidTime	= 0;

					Token = (*GetToken)();
					if( strcmp("end", TokenString ) == NULL )
					{
						break;
					}

					iItemType = (int)TokenNumber;
					Token = (*GetToken)();
					iItemIndex = (int)TokenNumber;
					Token = (*GetToken)();
					iItemOption1 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValue1 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemOption2 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValue2 = (int)TokenNumber;
					Token = (*GetToken)();
					iItemValidTime = (int)TokenNumber;

					iItemNumber = MAKE_ITEMNUM( iItemType, iItemIndex );

					// 효과 취소를 위한 옵션 타입을 넣어 주어야 한다.
					m_ItemAddOption[iItemAddOptionIndex].wOptionType		= (WORD)iType;

					m_ItemAddOption[iItemAddOptionIndex].iItemNumber		= iItemNumber;
					m_ItemAddOption[iItemAddOptionIndex].iItemType			= iItemType;
					m_ItemAddOption[iItemAddOptionIndex].iItemIndex			= iItemIndex;
					m_ItemAddOption[iItemAddOptionIndex].wEffectType1		= iItemOption1;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValue1		= iItemValue1;
					m_ItemAddOption[iItemAddOptionIndex].wEffectType2		= iItemOption2;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValue2		= iItemValue2;
					m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime	= iItemValidTime;

	#if TESTSERVER == 1
					LogAdd("%d %d %d %d %d %d %d %d",
						iItemNumber, iItemType, iItemIndex, iItemOption1, iItemValue1, iItemOption2, iItemValue2, iItemValidTime );
	#endif
					iItemAddOptionIndex++;
					if( iItemAddOptionIndex > MAX_ITEM_ADD_OPTION_ROW )
					{
						MsgBox("[ItemAddOption] Overflow ItemAddOption Table Index");
					}
				}
			}
		}
	}

	fclose(SMDFile);
}

void CItemAddOption::Initialize()
{
	for( int i = 0; i < MAX_ITEM_ADD_OPTION_ROW; i++ )
	{
		m_ItemAddOption[i].iItemNumber		= 0;
		m_ItemAddOption[i].iItemType		= 0;
		m_ItemAddOption[i].iItemIndex		= 0;
		m_ItemAddOption[i].wEffectType1		= 0;
		m_ItemAddOption[i].iEffectValue1	= 0;
		m_ItemAddOption[i].wEffectType2		= 0;
		m_ItemAddOption[i].iEffectValue2	= 0;
		m_ItemAddOption[i].iEffectValidTime	= 0;
	}
}

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
bool CItemAddOption::ClearItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode, int iEffectOption)
{
	LPITEMEFFECT lpItemEffect = NULL;

	lpItemEffect = _SearchItemEffect( iItemCode );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	_ClearItemEffect( lpObj, iEffectOption, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
	_ClearItemEffect( lpObj, iEffectOption, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );

#ifdef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
	gObjCalCharacter( lpObj->m_Index );
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023

	return true;
}
// 아이템의 효과 적용을 위해 gObjCalCharacter() 함수 내에서 호출되도록 한다.
bool CItemAddOption::PrevSetItemEffect(LPOBJECTSTRUCT lpObj)
{
	LPITEMPERIOD_INFO lpPeriodItemInfo = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	lpPeriodItemInfo = g_CashItemPeriodSystem.GetPeriodInfo( lpObj );
	if( lpPeriodItemInfo == NULL )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_PERIODITEM_DATEINFO ; i++ )
	{
		if( lpPeriodItemInfo->DateInfo[i].btUsedInfo == 1 )
		{
			lpItemEffect = _SearchItemEffect( lpPeriodItemInfo->DateInfo[i].iItemCode );
			if( lpItemEffect != NULL )
			{
				switch( lpItemEffect->wEffectType1 )
				{
				case ITEMADDOPTION_HP:
				case ITEMADDOPTION_MANA:
				case ITEMADDOPTION_STRENGTH:
				case ITEMADDOPTION_DEXTERITY:
				case ITEMADDOPTION_VITALITY:
				case ITEMADDOPTION_ENERGY:
#ifndef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023				// !! NOT
				case ITEMADDOPTION_LEADERSHIP:
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
					_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
					break;
				}

				switch( lpItemEffect->wEffectType2 )
				{
				case ITEMADDOPTION_HP:
				case ITEMADDOPTION_MANA:
				case ITEMADDOPTION_STRENGTH:
				case ITEMADDOPTION_DEXTERITY:
				case ITEMADDOPTION_VITALITY:
				case ITEMADDOPTION_ENERGY:
#ifndef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023				// !! NOT
				case ITEMADDOPTION_LEADERSHIP:
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
					_SetItemEffect( lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );
					break;
				}
			}
		}
	}

	return true;
}

bool CItemAddOption::NextSetItemEffect(LPOBJECTSTRUCT lpObj)
{
	LPITEMPERIOD_INFO lpPeriodItemInfo = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	lpPeriodItemInfo = g_CashItemPeriodSystem.GetPeriodInfo( lpObj );
	if( lpPeriodItemInfo == NULL )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_PERIODITEM_DATEINFO ; i++ )
	{
		if( lpPeriodItemInfo->DateInfo[i].btUsedInfo == 1 )
		{
			lpItemEffect = _SearchItemEffect( lpPeriodItemInfo->DateInfo[i].iItemCode );
			if( lpItemEffect != NULL )
			{
				switch( lpItemEffect->wEffectType1 )
				{
				case ITEMADDOPTION_HP:
				case ITEMADDOPTION_MANA:
				case ITEMADDOPTION_STRENGTH:
				case ITEMADDOPTION_DEXTERITY:
				case ITEMADDOPTION_VITALITY:
				case ITEMADDOPTION_ENERGY:
#ifndef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023			// !! NOT
				case ITEMADDOPTION_LEADERSHIP:
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
#ifdef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
				case ITEMADDOPTION_ATTACKSPEED:
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709					
					break;
				default:
					_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
					break;
				}

				switch( lpItemEffect->wEffectType2 )
				{
				case ITEMADDOPTION_HP:
				case ITEMADDOPTION_MANA:
				case ITEMADDOPTION_STRENGTH:
				case ITEMADDOPTION_DEXTERITY:
				case ITEMADDOPTION_VITALITY:
				case ITEMADDOPTION_ENERGY:
#ifndef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023			// !! NOT
				case ITEMADDOPTION_LEADERSHIP:
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
#ifdef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
				case ITEMADDOPTION_ATTACKSPEED:
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
					break;
				default:
					_SetItemEffect( lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );
					break;
				}
			}
		}
	}

	return true;
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
bool CItemAddOption::PrevSetItemLastEffectForHallowin(LPOBJECTSTRUCT lpObj)
{
	LPITEMEFFECT lpItemEffect = NULL;

	int iItemEffectType = lpObj->m_wItemEffectType;

	lpItemEffect = _SearchItemEffect( iItemEffectType );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	if( lpItemEffect->wEffectType1 == ITEMADDOPTION_HP 
		|| lpItemEffect->wEffectType1 == ITEMADDOPTION_MANA 
	  )
	{
		_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
	}

	return true;
}

bool CItemAddOption::NextSetItemLastEffectForHallowin(LPOBJECTSTRUCT lpObj)
{
	LPITEMEFFECT lpItemEffect = NULL;

	int iItemEffectType = lpObj->m_wItemEffectType;

	lpItemEffect = _SearchItemEffect( iItemEffectType );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	if( lpItemEffect->wEffectType1 != ITEMADDOPTION_HP 
		&& lpItemEffect->wEffectType1 != ITEMADDOPTION_MANA 
	  )
	{
		_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
	}
	// !! 할로윈의 경우는 한개만 적용
	// 지속 효과에 대한 적용은 유저에게 알릴 필요가 없다.
//	_SetItemEffect( lpObj, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2, iItemEffectType, lpItemEffect->iEffectValidTime );

	return true;
}

void CItemAddOption::SendItemUse(LPOBJECTSTRUCT lpObj, int iItemNumber, int iOptionType, int iEffectType, int iEffectValue, int iValidTime )
{
	if( lpObj == NULL )
	{
		return;
	}

	if( iItemNumber == 0 || iEffectType == 0 )
	{
		return;
	}

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, iOptionType, iEffectType, iValidTime );
#else
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, iOptionType, iEffectType );
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef FOR_KOREA
	char szTemp[256];

	wsprintf( szTemp, "%s 효과가 적용되어 %s %d 증가합니다. %d 분 동안 유지됩니다.",
		ItemAttribute[iItemNumber].Name, ItemEffectName[iEffectType-1], iEffectValue, iValidTime/60 );
	//wsprintf( szTemp, lMsg.Get( 3391 ), ItemAttribute[iItemNumber].Name, 
	GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
#endif // FOR_KOREA
}
#endif // MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
bool CItemAddOption::SetItemEffect(LPOBJECTSTRUCT lpObj, int iItemNumber, int iItemUseTime)
#else
bool CItemAddOption::SetItemEffect(LPOBJECTSTRUCT lpObj, int iItemNumber)
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
{
	LPITEMEFFECT lpItemEffectBefore = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	// 0. 유효한 사용자인지 확인한다.
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return false;
	}

	// 1. 기존에 사용되고 있는 효과의 여부를 확인한다.
	// 2. 만약 사용되고 있는 효과가 있다면 초기화 시킨다.
#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	lpItemEffect = _SearchItemEffect( iItemNumber );

	if( lpItemEffect == NULL )
	{
		return false;
	}

	#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	// 기간제 아이템의 경우 미리 카테고리를 채크하기 때문에 2번 채크할 필요는 없다.
	#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	lpItemEffectBefore = GetCurrentEffectType( lpObj );
	if( lpItemEffectBefore != NULL )
	{
		if( lpItemEffectBefore->wOptionType == lpItemEffect->wOptionType )
		{
			ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_CANCEL );
		}
	}
#else
	ClearItemEffect( lpObj, ITEMUSE_EFFECT_OPTION_CANCEL );

	// 3. 사용할 아이템의 효과를 확인한다.
	lpItemEffect = _SearchItemEffect( iItemNumber );

	if( lpItemEffect == NULL )
	{
		return false;
	}
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

	// 4. 각 효과의 수치를 적용시킨다.
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	if( iItemUseTime > 0 )
	{
		_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
		SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1, iItemUseTime * 60 );
		_SetItemEffect( lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );
		SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2, iItemUseTime * 60 );
	}
	else
	{
		_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
		SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1, lpItemEffect->iEffectValidTime );
		_SetItemEffect( lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );
		SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2, lpItemEffect->iEffectValidTime );
	}
	#else
	_SetItemEffect( lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
	SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1, lpItemEffect->iEffectValidTime );
	_SetItemEffect( lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );
	SendItemUse( lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2, lpItemEffect->iEffectValidTime );
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#else
	_SetItemEffect( lpObj, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1, iItemNumber, lpItemEffect->iEffectValidTime );
	_SetItemEffect( lpObj, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2, iItemNumber, lpItemEffect->iEffectValidTime );
#endif // MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030

	// 5. 효과의 지속 시간을 정한다.
#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	#ifdef MODIFY_ITEMADDOPTION_SET_EFFECT_20070801
	if( lpItemEffect->wOptionType == 1 )		// 할로윈 아이템만 해당 변수를 이용한다.
	{
		if( iItemUseTime > 0 )
		{
			lpObj->m_wItemEffectType = lpItemEffect->iItemNumber;
			lpObj->m_iItemEffectValidTime = iItemUseTime * 60;
		}
		else
		{
			lpObj->m_wItemEffectType = lpItemEffect->iItemNumber;
			lpObj->m_iItemEffectValidTime = lpItemEffect->iEffectValidTime;
		}
	}
	#else
	if( iItemUseTime > 0 )
	{
		lpObj->m_wItemEffectType = lpItemEffect->iItemNumber;
		lpObj->m_iItemEffectValidTime = iItemUseTime * 60;
	}
	else
	{
		lpObj->m_wItemEffectType = lpItemEffect->iItemNumber;
		lpObj->m_iItemEffectValidTime = lpItemEffect->iEffectValidTime;
	}
	#endif // MODIFY_ITEMADDOPTION_SET_EFFECT_20070801
#else
	lpObj->m_wItemEffectType = lpItemEffect->iItemNumber;
	lpObj->m_iItemEffectValidTime = lpItemEffect->iEffectValidTime;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
	gObjCalCharacter( lpObj->m_Index );
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023

	return true;
}

bool CItemAddOption::SetItemEffectForTest(LPOBJECTSTRUCT lpObj, int iEffectType, int iEffectValue, int iEffectValidTime)
{
	if( iEffectType <= 0 )
	{
		return false;
	}

	if( iEffectValidTime <= 0 )
	{
		iEffectValidTime = 60;
	}

#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	_SetItemEffect( lpObj, iEffectType, iEffectValue );
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, 1, iEffectType, iEffectValidTime );
	#else
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, 1, iEffectType );
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	SendItemUse( lpObj, iEffectType, 1, iEffectType, iEffectValue, iEffectValidTime );
#else
	_SetItemEffect( lpObj, 1, iEffectType, iEffectValue, 0, iEffectValidTime );
#endif

	return true;
}

#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
bool CItemAddOption::_SetItemEffect(LPOBJECTSTRUCT lpObj, int iEffectType, int iEffectValue)
#else
bool CItemAddOption::_SetItemEffect(LPOBJECTSTRUCT lpObj, int iOptionType, int iEffectType, int iEffectValue, int iItemNumber, int iEffectValidTime)
#endif
{
	if( iEffectType <= 0 )
	{
		return false;
	}

#ifdef MODIFY_CASHITEM_EFFECT_REITERATION_APPLY_20090903
	if(iEffectType == ITEMADDOPTION_ATTACKSPEED ||
		iEffectType == ITEMADDOPTION_DAMAGE ||
		iEffectType == ITEMADDOPTION_DEFENSE ||
		iEffectType == ITEMADDOPTION_HP ||
		iEffectType == ITEMADDOPTION_MANA )		
		return false;
#endif //MODIFY_CASHITEM_EFFECT_REITERATION_APPLY_20090903

	switch( iEffectType )
	{
	case ITEMADDOPTION_HP:
		lpObj->AddLife				+= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
#endif
		break;
	case ITEMADDOPTION_MANA:
		lpObj->AddMana				+= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
#endif
		break;
	case ITEMADDOPTION_DAMAGE:
		lpObj->m_AttackDamageMaxLeft += iEffectValue;
		lpObj->m_AttackDamageMinLeft += iEffectValue;
		lpObj->m_AttackDamageMaxRight += iEffectValue;
		lpObj->m_AttackDamageMinRight += iEffectValue;
		lpObj->m_MagicDamageMin		+= iEffectValue;
		lpObj->m_MagicDamageMax		+= iEffectValue;
		break;

#ifndef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709					// !! NOT
	case ITEMADDOPTION_ATTACKSPEED:
		lpObj->m_AttackSpeed		+= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		lpObj->m_MagicSpeed			+= iEffectValue;
#endif
		break;
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
		
#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	case ITEMADDOPTION_EXPERIENCE:
		lpObj->m_wExprienceRate		= iEffectValue;
		lpObj->m_btMoveMapBound		= 1;
		break;
	case ITEMADDOPTION_ITEMDROPRATE:
		lpObj->m_wItemDropRate		= iEffectValue;
		lpObj->m_btMoveMapBound		= 1;
		break;
	case ITEMADDOPTION_EXPERIENCE_ZERO:
		lpObj->m_wExprienceRate		= 0;
		lpObj->m_btMoveMapBound		= 1;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205
	case ITEMADDOPTION_DEFENSE:
		lpObj->m_Defense			+= iEffectValue*10/20;
		break;
	case ITEMADDOPTION_STRENGTH:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddStrength			+= iEffectValue;
#else
		lpObj->Strength				+= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_DEXTERITY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddDexterity			+= iEffectValue;
#else
		lpObj->Dexterity			+= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_VITALITY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddVitality			+= iEffectValue;
#else
		lpObj->Vitality				+= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_ENERGY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddEnergy			+= iEffectValue;
#else
		lpObj->Energy				+= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_LEADERSHIP:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddLeadership		+= iEffectValue;
#else
		lpObj->Leadership			+= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
#ifdef ADD_ELITE_SCROLL_20070521
	case ITEMADDOPTION_MELEEDAMAGE:
		lpObj->m_AttackDamageMaxLeft += iEffectValue;
		lpObj->m_AttackDamageMinLeft += iEffectValue;
		lpObj->m_AttackDamageMaxRight += iEffectValue;
		lpObj->m_AttackDamageMinRight += iEffectValue;
		break;
	case ITEMADDOPTION_MAGICDAMAGE:
		lpObj->m_MagicDamageMin		+= iEffectValue;
		lpObj->m_MagicDamageMax		+= iEffectValue;
		break;
#endif // ADD_ELITE_SCROLL_20070521
#ifdef MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
	case ITEMADDOPTION_MAPMOVEFREE:
		break;
#endif // MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
	default:
		return false;
		break;
	}

#ifndef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, iOptionType, iEffectType, iEffectValidTime );
	#else
	GCUseEffectItem( lpObj, ITEMUSE_EFFECT_OPTION_USE, iOptionType, iEffectType );
	#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#if 0 //#ifdef FOR_KOREA
	wsprintf( szTemp, "%s 효과가 적용되어 %s %d 증가합니다. %d 분 동안 유지됩니다.",
		ItemAttribute[iItemNumber].Name, ItemEffectName[iEffectType-1], iEffectValue, iEffectValidTime/60 );
	GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
#endif // FOR_KOREA
	//wsprintf( szTemp, "%d 분 동안 유지됩니다.", iEffectValidTime/60 );
	//GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
#endif // MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030

#ifndef MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
//	gObjCalCharacter( lpObj->m_Index );
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
#endif // MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023

	return true;
}

bool CItemAddOption::_ClearItemEffect(LPOBJECTSTRUCT lpObj, int iEffectOption, int iOptionType, int iEffectType, int iEffectValue)
{
	if( iEffectType <= 0 )
	{
		return false;
	}
	
#ifdef MODIFY_CASHITEM_EFFECT_REITERATION_APPLY_20090903
	if(iEffectType == ITEMADDOPTION_ATTACKSPEED ||
		iEffectType == ITEMADDOPTION_DAMAGE ||
		iEffectType == ITEMADDOPTION_DEFENSE ||
		iEffectType == ITEMADDOPTION_HP ||
		iEffectType == ITEMADDOPTION_MANA )		
		return false;
#endif //MODIFY_CASHITEM_EFFECT_REITERATION_APPLY_20090903

	switch( iEffectType )
	{
	case ITEMADDOPTION_HP:
		lpObj->AddLife				-= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		if( lpObj->Life > lpObj->MaxLife + lpObj->AddLife )
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		}
		GCReFillSend( lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield );
		GCReFillSend( lpObj->m_Index, (WORD)(lpObj->MaxLife + lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#endif
		break;
	case ITEMADDOPTION_MANA:
		lpObj->AddMana				-= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		if( lpObj->Mana > lpObj->MaxMana + lpObj->AddMana )
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}
		GCManaSend( lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
		GCManaSend( lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
#endif
		break;
	case ITEMADDOPTION_DAMAGE:
		lpObj->m_AttackDamageMaxLeft -= iEffectValue;
		lpObj->m_AttackDamageMinLeft -= iEffectValue;
		lpObj->m_AttackDamageMaxRight -= iEffectValue;
		lpObj->m_AttackDamageMinRight -= iEffectValue;
		lpObj->m_MagicDamageMin		-= iEffectValue;
		lpObj->m_MagicDamageMax		-= iEffectValue;
		break;

#ifndef MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709					// !! NOT
	case ITEMADDOPTION_ATTACKSPEED:
		lpObj->m_AttackSpeed		-= iEffectValue;
#ifdef MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		lpObj->m_MagicSpeed			-= iEffectValue;
#endif
		break;
#endif // MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709

#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	case ITEMADDOPTION_EXPERIENCE:
		lpObj->m_wExprienceRate	= 100;
		lpObj->m_btMoveMapBound = 0;
		break;
	case ITEMADDOPTION_ITEMDROPRATE:
		lpObj->m_wItemDropRate	= 100;
		lpObj->m_btMoveMapBound = 0;
		break;
	case ITEMADDOPTION_EXPERIENCE_ZERO:
		lpObj->m_wExprienceRate = 100;
		lpObj->m_btMoveMapBound = 0;
		break;
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif // ADD_PCS_MARK_OF_EXP_20070205
	case ITEMADDOPTION_DEFENSE:
		lpObj->m_Defense			-= iEffectValue*10/20;
		break;
	case ITEMADDOPTION_STRENGTH:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddStrength			-= iEffectValue;
#else
		lpObj->Strength				-= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_DEXTERITY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddDexterity			-= iEffectValue;
#else
		lpObj->Dexterity			-= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_VITALITY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddVitality				-= iEffectValue;
#else
		lpObj->Vitality				-= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_ENERGY:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddEnergy			-= iEffectValue;
#else
		lpObj->Energy				-= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
	case ITEMADDOPTION_LEADERSHIP:
#ifdef MODIFY_ITEMADDOPTION_VARIABLE_20070725
		lpObj->AddLeadership		-= iEffectValue;
#else
		lpObj->Leadership			-= iEffectValue;
#endif // MODIFY_ITEMADDOPTION_VARIABLE_20070725
		break;
#ifdef ADD_ELITE_SCROLL_20070521
	case ITEMADDOPTION_MELEEDAMAGE:
		lpObj->m_AttackDamageMaxLeft -= iEffectValue;
		lpObj->m_AttackDamageMinLeft -= iEffectValue;
		lpObj->m_AttackDamageMaxRight -= iEffectValue;
		lpObj->m_AttackDamageMinRight -= iEffectValue;
		break;
	case ITEMADDOPTION_MAGICDAMAGE:
		lpObj->m_MagicDamageMin		-= iEffectValue;
		lpObj->m_MagicDamageMax		-= iEffectValue;
		break;
#endif // ADD_ELITE_SCROLL_20070521
#ifdef MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
	case ITEMADDOPTION_MAPMOVEFREE:
		break;
#endif // MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
	default:
		return false;
		break;
	}

	// 사용자에게 효과 취소를 알린다.
	#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	GCUseEffectItem( lpObj, iEffectOption, iOptionType, iEffectType, 0 );
	#else
	GCUseEffectItem( lpObj, iEffectOption, iOptionType, iEffectType );
	#endif ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

	return true;
}

bool CItemAddOption::ClearItemEffect(LPOBJECTSTRUCT lpObj, int iEffectOption)
{
	LPITEMEFFECT lpItemEffect = NULL;

	lpItemEffect = GetCurrentEffectType( lpObj );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	_ClearItemEffect( lpObj, iEffectOption, lpItemEffect->wOptionType, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1 );
	_ClearItemEffect( lpObj, iEffectOption, lpItemEffect->wOptionType, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2 );

	lpObj->m_wItemEffectType = 0;
	lpObj->m_iItemEffectValidTime = 0;

	return true;
}

LPITEMEFFECT CItemAddOption::GetCurrentEffectType(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_wItemEffectType <= 0 )
	{
		return NULL;
	}

	return _SearchItemEffect( lpObj->m_wItemEffectType );
}

LPITEMEFFECT CItemAddOption::_SearchItemEffect(int iItemNumber)
{
	for( int i = 0; i < MAX_ITEM_ADD_OPTION_ROW ; i++ )
	{
		if( m_ItemAddOption[i].iItemNumber == iItemNumber )
		{
			return &m_ItemAddOption[i];
		}
	}

	return NULL;
}

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
bool CItemAddOption::SearchItemEffectType(int iItemNumber, int* iEffectType1, int* iEffectType2, int* iEffectUseTime)
{
	LPITEMEFFECT lpItemEffect = NULL;

	lpItemEffect = _SearchItemEffect( iItemNumber );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	*iEffectType1 = lpItemEffect->wEffectType1;
	*iEffectType2 = lpItemEffect->wEffectType2;
	*iEffectUseTime	= lpItemEffect->iEffectValidTime;

	return true;
}
#else
bool CItemAddOption::SearchItemEffectType(int iItemNumber, int* iEffectType1, int* iEffectType2)
{
	LPITEMEFFECT lpItemEffect = NULL;

	lpItemEffect = _SearchItemEffect( iItemNumber );
	if( lpItemEffect == NULL )
	{
		return false;
	}

	*iEffectType1 = lpItemEffect->wEffectType1;
	*iEffectType2 = lpItemEffect->wEffectType2;

	return true;
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
int CItemAddOption::GetItemEffectOption(int iItemCode)
{
	LPITEMEFFECT lpItemEffect = NULL;
	int iEffectOption = -1;

	lpItemEffect = _SearchItemEffect( iItemCode );
	if( lpItemEffect == NULL )
	{
		return iEffectOption;
	}

	iEffectOption = lpItemEffect->wOptionType;

	return iEffectOption;
}
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
bool CItemAddOption::IsValidEffect(int iItemNumber)
{
	if( _SearchItemEffect( iItemNumber ) == NULL )
	{
		return false;
	}

	return true;
}

LPITEMEFFECT CItemAddOption::GetItemEffectData(int iItemNumber)
{
	return _SearchItemEffect( iItemNumber );
}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#endif