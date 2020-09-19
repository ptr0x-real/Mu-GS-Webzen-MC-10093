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

// �ʿ� ������
// ���� �ε���, ȿ�� ī�װ�(�ε尡��), ȿ��Ÿ��1/2, ȿ����ġ1/2, ���ӽð�
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
				// ���� ���� ȿ���߿� �����ص� �Ǵ� ȿ���� �����Ѵ�.
			}
			else if( DebuffCount > MAX_BUFF_EFFECT_COUNT )
			{
				// ���� ����� ȿ���߿� �����ص� �Ǵ� ȿ���� �����Ѵ�.
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

			// �ش� ClearType�� �´� ȿ���� �������� �ش�.
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

			// �α׾ƿ� �ÿ��� ��� ȿ���� �ʱ�ȭ ���� �ش�.
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

			// ���� ȿ���� ���� �ð��� 3���� ������ ȿ���� ���� ������� �ش�.
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
				// ȿ�� ����
				gObjRemoveBuffEffect( lpObj, lpObj->m_BuffEffectList[i].BuffIndex );
			}
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#else

			if( ( GetTickCount() - lpObj->m_BuffEffectList[i].EffectSetTime ) > ( lpObj->m_BuffEffectList[i].EffectDuration * 1000 ) )
			{
				// ȿ�� ����
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

	// ItemAddOption���� �ҷ��� �� �ִ� �����ʹ� ItemAddOption�� �����Ѵ�.
	// ItemAddOption�� ����ϴ� ȿ���� ��� �������� ������� ��� ����� ȿ���̴�.
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
		// ItemAddOption�� ������ �������� �����Ѵ�.
		return false;
	}

	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData( iItemCode );

	int iRemoveBuffIndex = 0;
	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect( lpObj, iBuffIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, lpItemEffectInfo->iEffectValidTime );

	if( iRemoveBuffIndex != -1 )
	{
		if( iRemoveBuffIndex > 0 )
		{
			// ���� �� ȿ���� �ִ�.
			//GCStateInfoSend( lpObj, 0, 0 );
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0 );
		}
		else
		{
			// ���� �� ȿ���� ����.
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
			// ���� �� ȿ���� �ִ�.
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpBuffData->BuffEffectType, 0, 0 );
		}
		else
		{
			// ���� �� ȿ���� ����.
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
	
	// ItemAddOption���� �ҷ��� �� �ִ� �����ʹ� ItemAddOption�� �����Ѵ�.
	// ItemAddOption�� ����ϴ� ȿ���� ��� �������� ������� ��� ����� ȿ���̴�.
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
		// ItemAddOption�� ������ �������� �����Ѵ�.
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
			// ���� �� ȿ���� �ִ�.
			//GCStateInfoSend( lpObj, 0, 0 );
			GCUseBuffEffect( lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0 );
		}
		else
		{
			// ���� �� ȿ���� ����.
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

	// �ɷ�ġ ��/�� ������ ��� �ش� �ɷ�ġ�� �� ���·� �������� �ش�.
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
					// !! ������ ������ �ָ� ���� �������� ��ĭ�� �и��� ������ Ŭ����� ������ �־�� �Ѵ�.
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
					// !! ������ ������ �ָ� ���� �������� ��ĭ�� �и��� ������ Ŭ����� ������ �־�� �Ѵ�.
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
					// !! ������ ������ �ָ� ���� �������� ��ĭ�� �и��� ������ Ŭ����� ������ �־�� �Ѵ�.
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
					// !! ������ ������ �ָ� ���� �������� ��ĭ�� �и��� ������ Ŭ����� ������ �־�� �Ѵ�.
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

	// ī����ĳ���� ���� �� �ִ� ���� GM ��ũ�� ����Ѵ�.
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

	// ����Ʈ�� �߰� ���ص� �Ǵ� ���� ȿ���� �����Ѵ�.
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
		// ���� �ε��� �߿� ����Ʈ�� �߰� �� �� �ʿ� ���� ����Ʈ�� �߰��Ѵ�.
		case BUFFTYPE_NONE:
			break;
		default:
			// ����Ʈ�� �߰��� �־�� �ϴ� ����Ʈ
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
	pMsg.wEffectType		= EffectType;				// �ʿ� ���� ����
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
	
	if( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) )		//����������
	{
		//�ؿ� �Ϲ�����, �����ͷ������� ��� ������������ �����(����ġ ����)
		//�ѱ��� ��� �����ͷ����� ���������� ����ȵ�(����ġ ���)
		//����ġ ���� ����
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//���������
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//ǳ��������
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//ġ��������
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // ����� ���� ������
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // ǳ���� ���� ������ 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		)
	{
		// ����ġ ���
		int iAddExpRate = 0;
		
		iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		//�̺�Ʈ���� ���Ӱ�� ���ھ�� ���������� ����
		//�����ͷ��������� ���������, ǳ�������� ���� ����
#ifndef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		if(bRewardExp || g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ) {
			iAddExpRate = 100;
		}
#else
		// ���� ���� 6�� ������ ���� ���� ����� ����� ǳ���� ���� �߰� by natinda
		BOOL bMasterLevel = g_MasterLevelSystem.IsMasterLevelUser( lpObj ); 
		
		if(bMasterLevel ) {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// ������ ���������� �ش� ������ ������ ���� ���� ������ ����ϱ� ������ ������ �޴´�.
				if(bRewardExp) iAddExpRate = 100; //���� ����ġ�� ���� ����.
				// pass
			} else {
				iAddExpRate = 100;
			}
		} else {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// �Ϲ� �̿����ε� ������ ���� ���������� ����ϰ� �ִ�. ���� �ȵ�. ���� ��뵵 �ȵǰ�����..
				iAddExpRate = 100;
			} else {
				if(bRewardExp) iAddExpRate = 100; //���� ����ġ�� ���� ����.
				
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
// CheckItemOptForGetExp, CPCBangPointSystem::CheckItemOptForGetExp, CNewPVP::CheckItemOptForGetExp ������ �����Ѵ�.
void CheckItemOptForGetExpEx( LPOBJECTSTRUCT lpObj, INT64 &iExp, BOOL bRewardExp)
{
	INT64 iRetExp = iExp;
	/*
#ifdef ADD_NEWPVP_PKFIELD
	// ��ī�������㿡�� PC�� ������(����ġ ���) ��� ����
	if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber)) 
	{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
		// ������ ���� �������Դ� �ش���� �ʴ´�.
		if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
			return;
#endif // MASTER_LEVEL_UP_SYSTEM_20070912
		
	//	INT64 iRetExp = iExp;
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
		{
			// ����ġ ���� ����
			iRetExp = 0;
		}
#ifdef MODIFY_PCS_MARKS_GLORYOFGLADIATOR_DUPLICATION_20090715
		else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//���������
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//ǳ��������
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 )
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//ġ��������
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // ����� ���� ������
			|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // ǳ���� ���� ������ 
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
				iAddExpRate = 100; //���� ����ġ�� ���� ����.
			} else {
				// ����ġ 50% ����
				iAddExpRate = 150;
			}
			
			iRetExp = iRetExp * iAddExpRate / 100;
		}
	}
#endif // ADD_PREMIUMITEM_PANDA

	//����������
	if( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK3 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) 
		)
	{
		//�ؿ� �Ϲ�����, �����ͷ������� ��� ������������ �����(����ġ ����)
		//�ѱ��� ��� �����ͷ����� ���������� ����ȵ�(����ġ ���)
		//����ġ ���� ����
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1 )	//���������
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2 ) 	//ǳ��������
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 )
#ifdef ADD_CASHITEM_4RD_SCROLL_20080723
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK5 ) 	//ġ��������
#endif // ADD_CASHITEM_4RD_SCROLL_20080723
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) // ����� ���� ������
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) // ǳ���� ���� ������ 
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		)
	{
		// ����ġ ���
		int iAddExpRate = 0;
		
		iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		
		//�̺�Ʈ���� ���Ӱ�� ���ھ�� ���������� ����
		//�����ͷ��������� ���������, ǳ�������� ���� ����
		
#ifndef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		//���� 
		if( bRewardExp || g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
			iAddExpRate = 100;
		}
#else
		// ���� ���� 6�� ������ ���� ���� ����� ����� ǳ���� ���� �߰� by natinda
		BOOL bMasterLevel = g_MasterLevelSystem.IsMasterLevelUser( lpObj ); 
		
		if(bMasterLevel ) {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// ������ ���������� �ش� ������ ������ ���� ���� ������ ����ϱ� ������ ������ �޴´�.
				if(bRewardExp) iAddExpRate = 100; //���� ����ġ�� ���� ����.
				// pass
			} else {
				iAddExpRate = 100;
			}
		} else {
			if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
				// �Ϲ� �̿����ε� ������ ���� ���������� ����ϰ� �ִ�. ���� �ȵ�. ���� ��뵵 �ȵǰ�����..
				iAddExpRate = 100;
			} else {
				if(bRewardExp) iAddExpRate = 100; //���� ����ġ�� ���� ����.
				
				// pass
			}
		}
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		
		iRetExp = iRetExp * iAddExpRate / 100;
	}
#ifdef MODIFY_PCS_MARKS_GLORYOFGLADIATOR_SEQUENCE_20090715
	// ��ī�������㿡�� PC�� ������(����ġ ���) ��� ����
	else if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR))
	{		
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
		// ������ ���� �������Դ� �ش���� �ʴ´�.
		if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
			return;
#endif // MASTER_LEVEL_UP_SYSTEM_20070912
		
		if (g_NewPVP.IsVulcanusMap(lpObj->MapNumber)) 
		{
			// �Ҵ��� ȿ���� ���� �ʾҴ�.
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
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PARTY_EXP_INCREASE_SCROLL )	//��Ƽ�� ����ġ ��� ��ũ��
		)
		{
			// ��Ƽ�� �ƴ� ��� ����ġ�� 130%
			int iAddExpRate = 130;
			
			if( lpObj->PartyNumber >= 0 )
			{
				for( int i=0; i<MAX_PARTYUSER; ++i )
				{
					LPOBJECTSTRUCT lpPartyObj = &gObj[gParty.m_PartyS[lpObj->PartyNumber].Number[i]];
					int iPartyDis = gObjCalDistance(lpObj, lpPartyObj);
					
					if( lpObj->m_Index == lpPartyObj->m_Index )
						continue;

					// ��Ƽ���϶� 130%���� �߰� ��Ƽ�������� 10%�� ����
					if( lpObj->MapNumber == lpPartyObj->MapNumber && iPartyDis < 10 )
						iAddExpRate += 10;
				}
			}
			
			//�̺�Ʈ���� ���Ӱ�� ���ھ�� ���������� ����
			//�����ͷ��������� ��Ƽ�� ����ġ ���� ���� ����
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
				iAddExpRate = 100; //���� ����ġ�� ���� ����.
			} else {
				// ����ġ 50% ����
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