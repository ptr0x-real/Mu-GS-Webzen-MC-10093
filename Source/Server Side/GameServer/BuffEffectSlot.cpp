// BuffEffectSlot.cpp: implementation of the CBuffEffectSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuffEffectSlot.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#include "..\common\winutil.h"
#include "ItemAddOption.h"

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_PARTY_EXP_INCREASE_SCROLL_20090916
#include "PartyClass.h"
extern PartyClass		gParty;
#endif // ADD_PARTY_EXP_INCREASE_SCROLL_20090916

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffEffectSlot g_BuffEffectSlot;

CBuffEffectSlot::CBuffEffectSlot()
{

}

CBuffEffectSlot::~CBuffEffectSlot()
{

}

// 필요 데이터
// 버프 인덱스, 효과 카테고리(로드가능), 효과타입1/2, 효과수치1/2, 지속시간
int CBuffEffectSlot::SetEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, BYTE btEffectType1, BYTE btEffectType2, int iEffectValue1, int iEffectValue2, int iDuration )
{
	LPBUFF_EFFECT_DATA lpBuffData = NULL;
	LPBUFF_EFFECT_DATA lpPrevBuffData = NULL;
	int BuffCount = 0;
	int DebuffCount = 0;
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	int iRemovedBuffIndex = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211

	if( lpObj == NULL )
	{
		return -1;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return -1;
	}

	if( g_BuffScript.CheckVaildBuffEffect( iBuffIndex ) == false )
	{
		return -1;
	}

	lpBuffData = g_BuffScript.GetBuffData( iBuffIndex );

	if( lpBuffData == NULL )
	{
		return -1;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex != 0 )
		{
			lpPrevBuffData = g_BuffScript.GetBuffData( lpObj->m_BuffEffectList[i].BuffIndex );
#ifdef MODIFY_GETBUFFDATA_RETURNVALUE_NULLCHECK_BUGFIX_20080923
			if(lpPrevBuffData == NULL)
			{
				continue;
			}
#endif	// MODIFY_GETBUFFDATA_RETURNVALUE_NULL_BUGFIX_20080923			
			if( lpPrevBuffData->BuffEffectType == lpBuffData->BuffEffectType )
			{
				RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
				iRemovedBuffIndex = lpPrevBuffData->BuffIndex;
#else
				lpObj->m_BuffEffectList[i].BuffIndex = iBuffIndex;
				lpObj->m_BuffEffectList[i].EffectCategory = lpBuffData->BuffEffectType;
				lpObj->m_BuffEffectList[i].EffectType1 = btEffectType1;
				lpObj->m_BuffEffectList[i].EffectType2 = btEffectType2;
				lpObj->m_BuffEffectList[i].EffectValue1 = iEffectValue1;
				lpObj->m_BuffEffectList[i].EffectValue2 = iEffectValue2;
				lpObj->m_BuffEffectList[i].EffectDuration = iDuration;
				lpObj->m_BuffEffectList[i].EffectSetTime = GetTickCount();

				g_BuffEffect.SetBuffEffect( lpObj, btEffectType1, iEffectValue1 );
				g_BuffEffect.SetBuffEffect( lpObj, btEffectType2, iEffectValue2 );

				return lpPrevBuffData->BuffIndex;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			}

			if( lpPrevBuffData->BuffType == TYPE_BUFF )
			{
				BuffCount++;
			}
			else if( lpPrevBuffData->BuffType == TYPE_DEBUFF )
			{
				DebuffCount++;
			}

			if( BuffCount > MAX_BUFF_EFFECT_COUNT )
			{
				// 기존 버프 효과중에 삭제해도 되는 효과를 삭제한다.
			}
			else if( DebuffCount > MAX_BUFF_EFFECT_COUNT )
			{
				// 기존 디버프 효과중에 삭제해도 되는 효과를 삭제한다.
			}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			if( i > lpObj->m_BuffEffectCount )
			{
				break;
			}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}

#ifndef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211					// !! NOT
		if( lpObj->m_BuffEffectList[i].BuffIndex == 0 )
		{
			lpObj->m_BuffEffectList[i].BuffIndex = iBuffIndex;
			lpObj->m_BuffEffectList[i].EffectCategory = lpBuffData->BuffEffectType;
			lpObj->m_BuffEffectList[i].EffectType1 = btEffectType1;
			lpObj->m_BuffEffectList[i].EffectType2 = btEffectType2;
			lpObj->m_BuffEffectList[i].EffectValue1 = iEffectValue1;
			lpObj->m_BuffEffectList[i].EffectValue2 = iEffectValue2;
			lpObj->m_BuffEffectList[i].EffectDuration = iDuration;
			lpObj->m_BuffEffectList[i].EffectSetTime = GetTickCount();

			g_BuffEffect.SetBuffEffect( lpObj, btEffectType1, iEffectValue1 );
			g_BuffEffect.SetBuffEffect( lpObj, btEffectType2, iEffectValue2 );

			return 0;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex == 0 )
		{
			lpObj->m_BuffEffectList[i].BuffIndex = iBuffIndex;
			lpObj->m_BuffEffectList[i].EffectCategory = lpBuffData->BuffEffectType;
			lpObj->m_BuffEffectList[i].EffectType1 = btEffectType1;
			lpObj->m_BuffEffectList[i].EffectType2 = btEffectType2;
			lpObj->m_BuffEffectList[i].EffectValue1 = iEffectValue1;
			lpObj->m_BuffEffectList[i].EffectValue2 = iEffectValue2;
			lpObj->m_BuffEffectList[i].EffectDuration = iDuration;
			lpObj->m_BuffEffectList[i].EffectSetTime = GetTickCount();

			g_BuffEffect.SetBuffEffect( lpObj, btEffectType1, iEffectValue1 );
			g_BuffEffect.SetBuffEffect( lpObj, btEffectType2, iEffectValue2 );

			break;
		}
	}

	return iRemovedBuffIndex;
#else
	return -1;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
}

bool CBuffEffectSlot::RemoveEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	bool bIsRemoveEffect = false;

	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return false;
	}

	if( g_BuffScript.CheckVaildBuffEffect( iBuffIndex ) == false )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( bIsRemoveEffect == true && lpObj->m_BuffEffectList[i].BuffIndex != 0 && i > 0 )
		{
			memcpy( &lpObj->m_BuffEffectList[i-1], &lpObj->m_BuffEffectList[i], sizeof( EFFECTLIST ) );
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
			memset( &lpObj->m_BuffEffectList[i], 0, sizeof( EFFECTLIST ) );
#else
			lpObj->m_BuffEffectList[i].BuffIndex = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
		}

		if( lpObj->m_BuffEffectList[i].BuffIndex == iBuffIndex )
		{
			g_BuffEffect.ClearBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
			g_BuffEffect.ClearBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			memset( &lpObj->m_BuffEffectList[i], 0, sizeof( EFFECTLIST ) );
#else
			lpObj->m_BuffEffectList[i].BuffIndex = 0;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			bIsRemoveEffect = true;
		}
	}

	return bIsRemoveEffect;
}

bool CBuffEffectSlot::CheckUsedEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	if( g_BuffScript.CheckVaildBuffEffect( iBuffIndex ) == false )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex == iBuffIndex )
		{
			return true;
		}
	}

	return false;
}

void CBuffEffectSlot::RemoveBuffVariable(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	switch( iBuffIndex )
	{
	case BUFFTYPE_FREEZE:
		lpObj->DelayActionTime = 0;
		lpObj->DelayLevel = 0;
		lpObj->m_IceType = 0;
		break;
	}
}

int CBuffEffectSlot::ClearEffect(LPOBJECTSTRUCT lpObj, eBuffClearType ClearType)
{
	LPBUFF_EFFECT_DATA lpBuffData = NULL;
	int iRemoveEffectCount = 0;

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( CheckUsedEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex ) == true )
		{
			lpBuffData = g_BuffScript.GetBuffData( lpObj->m_BuffEffectList[i].BuffIndex );
			if( lpBuffData == NULL )
			{
				continue;
			}

			// 해당 ClearType에 맞는 효과를 삭제시켜 준다.
			if( lpBuffData->ClearType == ClearType )
			{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex ) == true )
				{
					iRemoveEffectCount++;
					i--;
				}
#else
				iRemoveEffectCount++;
				RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			}

			// 로그아웃 시에는 모든 효과를 초기화 시켜 준다.
			if( ClearType == CLEAR_TYPE_LOGOUT )
			{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex ) == true )
				{
					iRemoveEffectCount++;
					i--;
				}
#else
				iRemoveEffectCount++;
				RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			}

			if( ClearType == CLEAR_TYPE_DIEREGEN )
			{
				if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_INVISABLE )
				{
				}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex ) == true )
				{
					iRemoveEffectCount++;
					i--;
				}
#else
				iRemoveEffectCount++;
				RemoveEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			}
		}
	}

	return iRemoveEffectCount;
}

LPBUFF_EFFECT_DATA CBuffEffectSlot::GetEffectData(int iBuffIndex)
{
	LPBUFF_EFFECT_DATA lpBuffData = NULL;

	if( g_BuffScript.CheckVaildBuffEffect( iBuffIndex ) == false )
	{
		return NULL;
	}

	lpBuffData = g_BuffScript.GetBuffData( iBuffIndex );

	return lpBuffData;
}

int CBuffEffectSlot::GetBuffClearType(int iBuffIndex)
{
	LPBUFF_EFFECT_DATA lpBuffData = NULL;

	if( g_BuffScript.CheckVaildBuffEffect( iBuffIndex ) == false )
	{
		return -1;
	}

	lpBuffData = g_BuffScript.GetBuffData( iBuffIndex );

	return lpBuffData->ClearType;
}

void gObjCheckBuffEffectList(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return;
	}

	if( ( lpObj->Type == OBJTYPE_CHARACTER && lpObj->Connected >= CSS_GAMEPLAYING )
		|| lpObj->Type == OBJTYPE_MONSTER )
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
#else
		for( int i = 0 ; i < lpObj->m_BuffEffectCount ; i++ )
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		{
			if( lpObj->m_BuffEffectList[i].EffectDuration == BUFFTIME_INFINITE )
			{
				continue;
			}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE )
			{
				continue;
			}

			// 버프 효과의 적용 시간이 3초인 버프의 효과는 따로 적용시켜 준다.
			switch( lpObj->m_BuffEffectList[i].BuffIndex )
			{
			case BUFFTYPE_POISON:
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( lpObj->m_BuffEffectList[i].EffectDuration % 3 == 0 )
				{
		#ifdef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
					g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
					g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
		#else
					gObjSetActiveEffectAtTick( lpObj );
		#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
				}
	#else
				DWORD iDuration;

				iDuration = GetTickCount();

				if( iDuration - lpObj->m_BuffEffectList[i].EffectSetTime >= 3000 )
				{
					lpObj->m_BuffEffectList[i].EffectSetTime = GetTickCount();
					lpObj->m_BuffEffectList[i].EffectDuration -= 3;
					gObjSetActiveEffectAtTick( lpObj );
				}
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				break;
			default:
		#ifdef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
				g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
				g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
		#else
				gObjSetActiveEffectAtTick( lpObj );
		#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
				break;
			}

	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			TRACE_LOG( "%d\n", lpObj->m_BuffEffectList[i].EffectDuration );
			lpObj->m_BuffEffectList[i].EffectDuration--;
			if( lpObj->m_BuffEffectList[i].EffectDuration <= 0 )
			{
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
			}
	#else
			if( ( GetTickCount() - lpObj->m_BuffEffectList[i].EffectSetTime ) > ( lpObj->m_BuffEffectList[i].EffectDuration * 1000 ) )
			{
				// 효과 삭제
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
			}
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#else

			if( ( GetTickCount() - lpObj->m_BuffEffectList[i].EffectSetTime ) > ( lpObj->m_BuffEffectList[i].EffectDuration * 1000 ) )
			{
				// 효과 삭제
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
			}
			else
			{
				gObjSetActiveEffectAtTick( lpObj );
			}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		}
	}
}

bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}

	// ItemAddOption에서 불러올 수 있는 데이터는 ItemAddOption을 참조한다.
	// ItemAddOption을 사용하는 효과는 모두 아이템을 사용했을 경우 생기는 효과이다.
	int iItemCode = 0;
	LPITEMEFFECT lpItemEffectInfo = NULL;
	LPBUFF_EFFECT_DATA lpBuffData = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectData( iBuffIndex );
	if( lpBuffData == NULL )
	{
		return false;
	}

	iItemCode = MAKE_ITEMNUM( lpBuffData->ItemType, lpBuffData->ItemIndex );

	if( g_ItemAddOption.IsValidEffect( iItemCode ) == false )
	{
		// ItemAddOption에 지정된 아이템이 존재한다.
		return false;
	}

	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData( iItemCode );

	int iRemoveBuffIndex = 0;
	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect( lpObj, iBuffIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, lpItemEffectInfo->iEffectValidTime );

	if( iRemoveBuffIndex != -1 )
	{
		if( iRemoveBuffIndex > 0 )
		{
			// 삭제 된 효과가 있다.
			//GCStateInfoSend( lpObj, 0, 0 );
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0 );
		}
		else
		{
			// 삭제 된 효과가 없다.
			lpObj->m_BuffEffectCount++;
		}

		GCUseBuffEffect( lpObj, iBuffIndex, 0, lpItemEffectInfo->wOptionType, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->iEffectValidTime );
		return true;
	}

	return false;
}

bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, BYTE EffectType1, int EffectValue1, BYTE EffectType2, int EffectValue2, int Duration)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}

	LPBUFF_EFFECT_DATA lpBuffData = NULL;
	lpBuffData = g_BuffEffectSlot.GetEffectData( iBuffIndex );

	int iRemoveBuffIndex = 0;
	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect( lpObj, iBuffIndex, EffectType1, EffectType2, EffectValue1, EffectValue2, Duration );

	if( iRemoveBuffIndex != -1 )
	{
		if( iRemoveBuffIndex > 0 )
		{
			// 삭제 된 효과가 있다.
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpBuffData->BuffEffectType, 0, 0 );
		}
		else
		{
			// 삭제 된 효과가 없다.
			lpObj->m_BuffEffectCount++;
		}

		GCUseBuffEffect( lpObj, iBuffIndex, 0, lpBuffData->BuffEffectType, EffectType1, Duration );
		return true;
	}

	return false;
}

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, int Duration)
{
	if( lpObj == NULL )
	{
		return false;
	}
	
	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}
	
	// ItemAddOption에서 불러올 수 있는 데이터는 ItemAddOption을 참조한다.
	// ItemAddOption을 사용하는 효과는 모두 아이템을 사용했을 경우 생기는 효과이다.
	int iItemCode = 0;
	LPITEMEFFECT lpItemEffectInfo = NULL;
	LPBUFF_EFFECT_DATA lpBuffData = NULL;
	
	lpBuffData = g_BuffEffectSlot.GetEffectData( iBuffIndex );
	if( lpBuffData == NULL )
	{
		return false;
	}
	
	iItemCode = MAKE_ITEMNUM( lpBuffData->ItemType, lpBuffData->ItemIndex );
	
	if( g_ItemAddOption.IsValidEffect( iItemCode ) == false )
	{
		// ItemAddOption에 지정된 아이템이 존재한다.
		return false;
	}
	
	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData( iItemCode );
	
	int iRemoveBuffIndex = 0;
	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect( lpObj, iBuffIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2
		, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, Duration);
	
	if( iRemoveBuffIndex != -1 )
	{
		if( iRemoveBuffIndex > 0 )
		{
			// 삭제 된 효과가 있다.
			//GCStateInfoSend( lpObj, 0, 0 );
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0 );
		}
		else
		{
			// 삭제 된 효과가 없다.
			lpObj->m_BuffEffectCount++;
		}
		
		GCUseBuffEffect( lpObj, iBuffIndex, 0, lpItemEffectInfo->wOptionType, lpItemEffectInfo->wEffectType1, Duration);
		return true;
	}
	
	return false;
}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

bool gObjRemoveBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}

	LPBUFF_EFFECT_DATA lpBuffData = NULL;
	lpBuffData = g_BuffEffectSlot.GetEffectData( iBuffIndex );
	if( lpBuffData == NULL )
	{
		return false;
	}

	// 능력치 증/감 버프의 경우 해당 능력치를 원 상태로 복원시켜 준다.
	if( g_BuffEffectSlot.RemoveEffect( lpObj, iBuffIndex ) == true )
	{
		g_BuffEffectSlot.RemoveBuffVariable( lpObj, iBuffIndex );
		lpObj->m_BuffEffectCount--;

		GCUseBuffEffect( lpObj, iBuffIndex, 1, 0, lpBuffData->BuffEffectType, 0 );
		return true;
	}

	return false;
}

bool gObjClearBuffEffect(LPOBJECTSTRUCT lpObj, eBuffClearType ClearType)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}

	int iRemoveBuffEffectCount = 0;

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	int iBuffIndex = 0;
	switch( ClearType )
	{
	case CLEAR_TYPE_CASTLESIEGE:
		for( iBuffIndex = 0  ; iBuffIndex < MAX_BUFF_SLOT_INDEX ; iBuffIndex++ )
		{
			switch( lpObj->m_BuffEffectList[iBuffIndex].BuffIndex )
			{
			case BUFFTYPE_CASTLE_DEFENSE_MARK:
			case BUFFTYPE_CASTLE_ATTACK_MARK:
			case BUFFTYPE_CASTLE_ATTACK_MARK_2:
			case BUFFTYPE_CASTLE_ATTACK_MARK_3:
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex ) == true )
				{
					iRemoveBuffEffectCount++;
					// !! 버프를 삭제해 주면 다음 버프들이 한칸씩 밀리기 때문에 클리어시 유의해 주어야 한다.
					iBuffIndex--;
				}
	#else
				iRemoveBuffEffectCount++;
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex );
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				break;
			}
		}
		break;
	case CLEAR_TYPE_PCBANG_SEAL_FINISH:
		for( iBuffIndex = 0  ; iBuffIndex < MAX_BUFF_SLOT_INDEX ; iBuffIndex++ )
		{
			switch( lpObj->m_BuffEffectList[iBuffIndex].BuffIndex )
			{
			case BUFFTYPE_PCBANG_POINT_MARK1:
			case BUFFTYPE_PCBANG_POINT_MARK2:
			case BUFFTYPE_PCBANG_POINT_MARK3:
#ifdef ADD_PCBANG_BUFF_MARK4
			case BUFFTYPE_PCBANG_POINT_MARK4:
#endif // ADD_PCBANG_BUFF_MARK4	
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex ) == true )
				{
					iRemoveBuffEffectCount++;
					// !! 버프를 삭제해 주면 다음 버프들이 한칸씩 밀리기 때문에 클리어시 유의해 주어야 한다.
					iBuffIndex--;
				}
	#else
				iRemoveBuffEffectCount++;
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex );
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				break;
			}
		}
		break;
	case CLEAR_TYPE_NON_PCS_ITEM_EFFECT:
		for( iBuffIndex = 0 ; iBuffIndex < MAX_BUFF_SLOT_INDEX ; iBuffIndex++ )
		{
			switch( lpObj->m_BuffEffectList[iBuffIndex].BuffIndex )
			{
			case BUFFTYPE_PCBANG_POINT_MARK1:
			case BUFFTYPE_PCBANG_POINT_MARK2:
			case BUFFTYPE_PCBANG_POINT_MARK3:
#ifdef ADD_PCBANG_BUFF_MARK4
			case BUFFTYPE_PCBANG_POINT_MARK4:
#endif // ADD_PCBANG_BUFF_MARK4
			case BUFFTYPE_PCS_MARK1:
			case BUFFTYPE_PCS_MARK2:
			case BUFFTYPE_PCS_MARK3:
			case BUFFTYPE_PCS_MARK4:
#ifdef ADD_CASHITEM_4RD_MARK_20080723
			case BUFFTYPE_PCS_MARK5:
			case BUFFTYPE_PCS_MARK6:
#endif // ADD_CASHITEM_4RD_MARK_20080723
			case BUFFTYPE_PCS_SCROLL_ANGER:
			case BUFFTYPE_PCS_SCROLL_DEFENSE:
			case BUFFTYPE_PCS_SCROLL_HASTE:
			case BUFFTYPE_PCS_SCROLL_MAGIC:
			case BUFFTYPE_PCS_SCROLL_HEALTH:
			case BUFFTYPE_PCS_SCROLL_MANA:
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
			case BUFFTYPE_PCS_SCROLL_CRITICAL:
			case BUFFTYPE_PCS_SCROLL_EXCELLENT:
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
			case BUFFTYPE_PCS_MEDICINE_STRENGTH:
			case BUFFTYPE_PCS_MEDICINE_DEXTERITY:
			case BUFFTYPE_PCS_MEDICINE_VITALITY:
			case BUFFTYPE_PCS_MEDICINE_ENERGY:
			case BUFFTYPE_PCS_MEDICINE_LEADERSHIP:
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			case BUFFTYPE_CHARM_GUARDIAN:
			case BUFFTYPE_CHARM_PROTECTITEM:
			case BUFFTYPE_PCS_MARK1_FOR_MASTER:
			case BUFFTYPE_PCS_MARK2_FOR_MASTER:
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129			
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
			case BUFFTYPE_PARTY_EXP_INCREASE_SCROLL:
			case BUFFTYPE_MAX_AG_INCREASE_AURA:
			case BUFFTYPE_MAX_SD_INCREASE_AURA:
#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
				break;
			default:
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex ) == true )
				{
					iRemoveBuffEffectCount++;
					// !! 버프를 삭제해 주면 다음 버프들이 한칸씩 밀리기 때문에 클리어시 유의해 주어야 한다.
					iBuffIndex--;
				}
	#else
				iRemoveBuffEffectCount++;
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex );
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				break;
			}
		}
		break;
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
	case CLEAR_TYPE_LOGOUT:
		for( iBuffIndex = 0 ; iBuffIndex < MAX_BUFF_SLOT_INDEX ; iBuffIndex++ )
		{
		#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
				if( gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex ) == true )
				{
					iRemoveBuffEffectCount++;
					// !! 버프를 삭제해 주면 다음 버프들이 한칸씩 밀리기 때문에 클리어시 유의해 주어야 한다.
					iBuffIndex--;
				}
		#else
				iRemoveBuffEffectCount++;
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[iBuffIndex].BuffIndex );
		#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
		}
		
		lpObj->m_BuffEffectCount = 0;
		break;
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
	default:
		iRemoveBuffEffectCount = g_BuffEffectSlot.ClearEffect( lpObj, ClearType );
		break;
	}
#else
	iRemoveBuffEffectCount = g_BuffEffectSlot.ClearEffect( lpObj, ClearType );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
#ifndef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	lpObj->m_BuffEffectCount -= iRemoveBuffEffectCount;
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	if( lpObj->m_BuffEffectCount < 0 || lpObj->m_BuffEffectCount > MAX_BUFF_SLOT_INDEX )
	{
		lpObj->m_BuffEffectCount = 0;
	}
	
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
	gObjSendBuffList( lpObj );
#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220

	return true;
}

bool gObjCheckUsedBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->m_Index < 0 || lpObj->m_Index > MAX_OBJECT )
	{
		return false;
	}

	return g_BuffEffectSlot.CheckUsedEffect( lpObj, iBuffIndex );
}

bool gObjChangeBuffValidTime(LPOBJECTSTRUCT lpObj, int iBuffIndex, int iTime)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return false;
	}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
	int iDuration = GetTickCount();
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( i > lpObj->m_BuffEffectCount )
		{
			return false;
		}

		if( lpObj->m_BuffEffectList[i].BuffIndex == iBuffIndex )
		{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			lpObj->m_BuffEffectList[i].EffectDuration += iTime;
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			if( lpObj->m_BuffEffectList[i].EffectDuration < 0 )
	#else
			if( ( iDuration - lpObj->m_BuffEffectList[i].EffectSetTime ) >= ( lpObj->m_BuffEffectList[i].EffectDuration * 1000 ) )
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
			{
				gObjRemoveBuffEffect( lpObj, iBuffIndex );
			}
#else
			lpObj->m_BuffEffectList[i].EffectDuration += iTime;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
			return true;
		}
	}

	return false;
}

void gObjSetActiveEffectAtTick(LPOBJECTSTRUCT lpObj)
{
	int EffectValue = 0;

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex != BUFFTYPE_NONE )
		{
			g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType1, lpObj->m_BuffEffectList[i].EffectValue1 );
			g_BuffEffect.SetActiveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].EffectType2, lpObj->m_BuffEffectList[i].EffectValue2 );
		}
	}
}

// return Viewport Effect count
// error : -1 success : 0~
int gObjMakeViewportState(LPOBJECTSTRUCT lpObj, BYTE* lpBuffer)
{
	if( lpObj == NULL )
	{
		return -1;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return -1;
	}

	int iBuffCount = 0;

	// 카오스캐슬에 입장 해 있는 경우는 GM 마크만 출력한다.
#ifndef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109				// !! NOT
	if( CHECK_CHAOSCASTLE( lpObj->MapNumber ) )
	{
		if((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
		{
			lpBuffer[0] = BUFFTYPE_GM_MARK;
			iBuffCount = 1;
		}

		return iBuffCount;
	}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109

	// 뷰포트에 추가 안해도 되는 버프 효과를 선택한다.
	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( iBuffCount >= MAX_BUFF_SLOT_INDEX || iBuffCount >= lpObj->m_BuffEffectCount )
		{
			break;
		}

		if( lpObj->m_BuffEffectList[i].BuffIndex == 0 )
		{
			continue;
		}

		switch( lpObj->m_BuffEffectList[i].BuffIndex )
		{
		// 버프 인덱스 중에 뷰포트에 추가 해 줄 필요 없는 이펙트을 추가한다.
		case BUFFTYPE_NONE:
			break;
		default:
			// 뷰포트에 추가해 주어야 하는 이펙트
			lpBuffer[iBuffCount] = lpObj->m_BuffEffectList[i].BuffIndex;
			iBuffCount++;
			break;
		}
	}

	return iBuffCount;
}

bool gObjCheckPowerfulEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, int EffectValue1, int EffectValue2)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex == iBuffIndex )
		{
			if( lpObj->m_BuffEffectList[i].EffectValue1 > EffectValue1 )
			{
				return true;
			}

			if( lpObj->m_BuffEffectList[i].EffectValue2 > EffectValue2 )
			{
				return true;
			}
		}
	}

	return false;
}

int gObjGetTotalValueOfEffect(LPOBJECTSTRUCT lpObj, int EffectType)
{
	if( lpObj == NULL )
	{
		return 0;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return 0;
	}

	int iTotalValue = 0;

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
		if( lpObj->m_BuffEffectList[i].BuffIndex == BUFFTYPE_NONE )
		{
			continue;
		}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128

		if( lpObj->m_BuffEffectList[i].EffectType1 == EffectType )
		{
			iTotalValue += lpObj->m_BuffEffectList[i].EffectValue1;
		}

		if( lpObj->m_BuffEffectList[i].EffectType2 == EffectType )
		{
			iTotalValue += lpObj->m_BuffEffectList[i].EffectValue2;
		}
	}

	return iTotalValue;
}

bool gObjGetValueOfBuffIndex(LPOBJECTSTRUCT lpObj, int iBuffIndex, int* EffectValue1, int* EffectValue2)
{
	if( lpObj == NULL )
	{
		return false;
	}

	if( lpObj->Connected < CSS_GAMEPLAYING )
	{
		return false;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex == iBuffIndex )
		{
			*EffectValue1 = lpObj->m_BuffEffectList[i].EffectValue1;
			if( EffectValue2 != NULL )
			{
				*EffectValue2 = lpObj->m_BuffEffectList[i].EffectValue2;
			}
			return true;
		}
	}

	return false;
}

void GCUseBuffEffect(LPOBJECTSTRUCT lpObj, BYTE BuffEffectIndex, BYTE EffectUseOption, WORD OptionType, WORD EffectType, int LeftTime)
{
	PMSG_USEEFFECTITEM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof( pMsg ));

	pMsg.btBuffEffectType	= BuffEffectIndex;
	pMsg.byEffectOption		= EffectUseOption;
	pMsg.wOptionType		= OptionType;
	pMsg.wEffectType		= EffectType;				// 필요 하지 않음
	pMsg.iLeftTime			= LeftTime;

	//LogAdd( "[%s][%s] BuffIndex : %d, EffectOption : %d, OptionType : %d, Time : %d", lpObj->AccountID, lpObj->Name, BuffEffectIndex, EffectUseOption, OptionType, LeftTime );

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

#ifdef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
	if( EffectUseOption == 0 )
	{
		GCBuffStateInfoSend( lpObj, 1, BuffEffectIndex );
	}
	else
	{
		GCBuffStateInfoSend( lpObj, 0, BuffEffectIndex );
	}
#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
}

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
void gObjSendBuffList(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return;
	}

	if( lpObj->Type != OBJTYPE_CHARACTER )
	{
		return;
	}

	if( lpObj->m_BuffEffectCount <= 0 )
	{
		return;
	}

	for( int i = 0 ; i < MAX_BUFF_SLOT_INDEX ; i++ )
	{
		if( lpObj->m_BuffEffectList[i].BuffIndex != BUFFTYPE_NONE )
		{
			GCBuffStateInfoSend( lpObj, 1, lpObj->m_BuffEffectList[i].BuffIndex );
		}
	}
}
#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
void CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp, BOOL bRewardExp)
{
	INT64 iRetExp = iExp;
	
	if( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) )		//유지의인장
	{
		//해외 일반유저, 마스터레벨유저 모두 유지의인장이 적용됨(경험치 유지)
		//한국의 경우 마스터레벨은 유지의인장 적용안됨(경험치 상승)
		//경험치 증가 막기
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//상승의인장
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//풍요의인장
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//치유의인장
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // 상승의 인장 마스터
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // 풍요의 인장 마스터 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		)
	{
		// 경험치 상승
		int iAddExpRate = 0;
		
		iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		//이벤트맵의 게임결과 스코어는 인장적용을 안함
		//마스터레벨유저는 상승의인장, 풍요의인장 적용 안함
#ifndef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		if(bRewardExp || g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ) {
			iAddExpRate = 100;
		}
#else
		// 유료 과금 6차 마스터 레벨 전용 상승의 인장과 풍요의 인장 추가 by natinda
		BOOL bMasterLevel = g_MasterLevelSystem.IsMasterLevelUser( lpObj ); 
		
		if(bMasterLevel ) {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// 마스터 레벨이지만 해당 유저는 마스터 레벨 전용 인장을 사용하기 때문에 적용을 받는다.
				if(bRewardExp) iAddExpRate = 100; //보상 경험치는 변동 없다.
				// pass
			} else {
				iAddExpRate = 100;
			}
		} else {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// 일반 이용자인데 마스터 레벨 전용인장을 사용하고 있다. 적용 안됨. 물론 사용도 안되겟지만..
				iAddExpRate = 100;
			} else {
				if(bRewardExp) iAddExpRate = 100; //보상 경험치는 변동 없다.
				
				// pass
			}
		}
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		
		iRetExp = iRetExp * iAddExpRate / 100;
	}

	iExp = iRetExp;
}
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
BOOL gObjAddBuffEffectWideArea(LPOBJECTSTRUCT lpTargetObj, int nDistance, int &nAffectedCount
							   , int iBuffIndex, BYTE EffectType1, int EffectValue1, BYTE EffectType2, int EffectValue2, int Duration)
{
	if (lpTargetObj == NULL)
		return FALSE;
	
	for( int i = ALLOC_USEROBJECTSTART; i < MAX_OBJECT; i++ )
	{
		LPOBJECTSTRUCT lpObj = &gObj[i];
		if (lpObj->Type == OBJTYPE_CHARACTER && gObjIsConnected(i) 
			&& lpObj->MapNumber == lpTargetObj->MapNumber)
		{
			if (gObjCalDistance(lpTargetObj, lpObj) < nDistance)
			{
				gObjAddBuffEffect(lpObj, iBuffIndex, EffectType1, EffectValue1, EffectType2, EffectValue2, Duration);
				nAffectedCount++;
			}
		}
	}
	
	return TRUE;
}
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
// CheckItemOptForGetExp, CPCBangPointSystem::CheckItemOptForGetExp, CNewPVP::CheckItemOptForGetExp 로직을 통합한다.
void CheckItemOptForGetExpEx( LPOBJECTSTRUCT lpObj, INT64 &iExp, BOOL bRewardExp)
{
	INT64 iRetExp = iExp;
	/*
#ifdef ADD_NEWPVP_PKFIELD
	// 불카누스폐허에선 PC방 아이템(경험치 상승) 사용 못함
	if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber)) 
	{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
		// 마스터 레벨 유저에게는 해당되지 않는다.
		if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
			return;
#endif // MASTER_LEVEL_UP_SYSTEM_20070912
		
	//	INT64 iRetExp = iExp;
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
		{
			// 경험치 증가 막기
			iRetExp = 0;
		}
#ifdef MODIFY_PCS_MARKS_GLORYOFGLADIATOR_DUPLICATION_20090715
		else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//상승의인장
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//풍요의인장
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 )
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//치유의인장
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // 상승의 인장 마스터
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // 풍요의 인장 마스터 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			)
		{}
		else
#endif // MODIFY_PCS_MARKS_GLORYOFGLADIATOR_DUPLICATION_20090715
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR))
		{
			int iAddExpRate = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_VULCANUS_EXPERIENCE);
			iRetExp += iRetExp * iAddExpRate / 100;
		}

		iExp = iRetExp;
		
		return;
	}
#endif // ADD_NEWPVP_PKFIELD
	*/
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() && lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 80) 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0)
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
		{
			iRetExp = 0;
		} 
		else
		{
			int iAddExpRate = 0;
			if(bRewardExp) {
				iAddExpRate = 100; //보상 경험치는 변동 없다.
			} else {
				// 경험치 50% 증가
				iAddExpRate = 150;
			}
			
			iRetExp = iRetExp * iAddExpRate / 100;
		}
	}
#endif // ADD_PREMIUMITEM_PANDA

	//유지의인장
	if( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) 
		)
	{
		//해외 일반유저, 마스터레벨유저 모두 유지의인장이 적용됨(경험치 유지)
		//한국의 경우 마스터레벨은 유지의인장 적용안됨(경험치 상승)
		//경험치 증가 막기
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//상승의인장
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//풍요의인장
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 )
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//치유의인장
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // 상승의 인장 마스터
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // 풍요의 인장 마스터 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		)
	{
		// 경험치 상승
		int iAddExpRate = 0;
		
		iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		
		//이벤트맵의 게임결과 스코어는 인장적용을 안함
		//마스터레벨유저는 상승의인장, 풍요의인장 적용 안함
		
#ifndef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		//기존 
		if( bRewardExp || g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
			iAddExpRate = 100;
		}
#else
		// 유료 과금 6차 마스터 레벨 전용 상승의 인장과 풍요의 인장 추가 by natinda
		BOOL bMasterLevel = g_MasterLevelSystem.IsMasterLevelUser( lpObj ); 
		
		if(bMasterLevel ) {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// 마스터 레벨이지만 해당 유저는 마스터 레벨 전용 인장을 사용하기 때문에 적용을 받는다.
				if(bRewardExp) iAddExpRate = 100; //보상 경험치는 변동 없다.
				// pass
			} else {
				iAddExpRate = 100;
			}
		} else {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// 일반 이용자인데 마스터 레벨 전용인장을 사용하고 있다. 적용 안됨. 물론 사용도 안되겟지만..
				iAddExpRate = 100;
			} else {
				if(bRewardExp) iAddExpRate = 100; //보상 경험치는 변동 없다.
				
				// pass
			}
		}
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		
		iRetExp = iRetExp * iAddExpRate / 100;
	}
#ifdef MODIFY_PCS_MARKS_GLORYOFGLADIATOR_SEQUENCE_20090715
	// 불카누스폐허에선 PC방 아이템(경험치 상승) 사용 못함
	else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR))
	{		
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
		// 마스터 레벨 유저에게는 해당되지 않는다.
		if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
			return;
#endif // MASTER_LEVEL_UP_SYSTEM_20070912
		
		if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber)) 
		{
			// 팬더펫 효과를 받지 않았다.
			if( iRetExp == iExp )
			{
				int iAddExpRate = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_VULCANUS_EXPERIENCE);
				iRetExp += iRetExp * iAddExpRate / 100;

				iExp = iRetExp;
				
				return;
			}
		}
	}
#endif // MODIFY_PCS_MARKS_GLORYOFGLADIATOR_SEQUENCE_20090715
#ifdef ADD_PARTY_EXP_INCREASE_SCROLL_20090916
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PARTY_EXP_INCREASE_SCROLL )	//파티시 경험치 상승 스크롤
		)
		{
			// 파티가 아닐 경우 경험치는 130%
			int iAddExpRate = 130;
			
			if( lpObj->PartyNumber >= 0 )
			{
				for( int i=0; i<MAX_PARTYUSER; ++i )
				{
					LPOBJECTSTRUCT lpPartyObj = &gObj[gParty.m_PartyS[lpObj->PartyNumber].Number[i]];
					int iPartyDis = gObjCalDistance(lpObj, lpPartyObj);
					
					if( lpObj->m_Index == lpPartyObj->m_Index )
						continue;

					// 파티중일땐 130%에서 추가 파티원수마다 10%씩 증가
					if( lpObj->MapNumber == lpPartyObj->MapNumber && iPartyDis < 10 )
						iAddExpRate += 10;
				}
			}
			
			//이벤트맵의 게임결과 스코어는 인장적용을 안함
			//마스터레벨유저는 파티시 경험치 증가 적용 안함
			if(bRewardExp || g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ) {
				iAddExpRate = 100;
			}
			
			iRetExp = iRetExp * iAddExpRate / 100;
		}
#endif // ADD_PARTY_EXP_INCREASE_SCROLL_20090916

	/*
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem() && lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13, 80) 
		&& lpObj->pInventory[EQUIPMENT_HELPER].m_Durability > 0)
	{
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
		{
			iRetExp = 0;
		} 
		else
		{
			int iAddExpRate = 0;
			if(bRewardExp) {
				iAddExpRate = 100; //보상 경험치는 변동 없다.
			} else {
				// 경험치 50% 증가
				iAddExpRate = 150;
			}
			
			iRetExp = iRetExp * iAddExpRate / 100;
		}
	}
#endif // ADD_PREMIUMITEM_PANDA
	*/
	
	iExp = iRetExp;
}
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC