#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "CrywolfAltar.h"
#include "CrywolfUtil.h"

static	CCrywolfUtil	UTIL;

CCrywolfAltar g_CrywolfNPC_Altar;

CCrywolfAltar::CCrywolfAltar()
{
	
}

CCrywolfAltar::~CCrywolfAltar()
{

}

int CCrywolfAltar::GetAltarState( int iClass )
{
	int iAltarState = 0;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// ������ ���� 1
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR1].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR2 :		// ������ ���� 2
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR2].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR3 :		// ������ ���� 3
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR3].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR4 :		// ������ ���� 4
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR4].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR5 :		// ������ ���� 5
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR5].GetAltarState();
		break;
	}

	return iAltarState;
}

int CCrywolfAltar::GetAltarRemainContractCount( int iClass )
{
	int iAltarRemainContractCount = 0;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// ������ ���� 1
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR1].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR2 :		// ������ ���� 2
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR2].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR3 :		// ������ ���� 3
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR3].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR4 :		// ������ ���� 4
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR4].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR5 :		// ������ ���� 5
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR5].GetRemainContractCount();
		break;
	}

	return iAltarRemainContractCount;
}

// ������ ���� ��ȣ�� ��´�. ( 0 ~ 4 )
int	CCrywolfAltar::GetAlatarNumber( int iClass )
{
	int iAltarNumber = _ZERO;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// ������ ���� 1
		iAltarNumber = CRYWOLF_ALTAR1;
		break;

	case CRYWOLF_NPC_ALTAR2 :		// ������ ���� 2
		iAltarNumber = CRYWOLF_ALTAR2;
		break;

	case CRYWOLF_NPC_ALTAR3 :		// ������ ���� 3
		iAltarNumber = CRYWOLF_ALTAR3;
		break;

	case CRYWOLF_NPC_ALTAR4:		// ������ ���� 4
		iAltarNumber = CRYWOLF_ALTAR4;
		break;

	case CRYWOLF_NPC_ALTAR5 :		// ������ ���� 5
		iAltarNumber = CRYWOLF_ALTAR5;
		break;
	}

	return iAltarNumber;
}

// ������ ���� ����(����) �ε����� ��´�.
int	 CCrywolfAltar::GetAltarUserIndex( int iClass )
{
	int iUserIndex = _INVALID;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// ������ ���� 1
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR1].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR2 :		// ������ ���� 2
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR2].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR3 :		// ������ ���� 3
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR3].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR4 :		// ������ ���� 4
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR4].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR5 :		// ������ ���� 5
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR5].m_iUserIndex;
		break;
	}
	
	return iUserIndex;
}

// ������ ������ ��ȿ�ϰ� �����.
void CCrywolfAltar::SetAltarValidContract( int iAltarObjIndex, int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	// 0. > ��� �õ� ������ üũ
	if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT )
		return;

	// 1. > ��� ��ȿ
	if( AltarInfo.m_dwValidContractTime < GetTickCount() )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( AltarInfo.m_iUserIndex, lMsg.Get(3323) , iAltarNumber );			// "������ [%d] ���ܰ��� ����� ��ȿ �˴ϴ�."

		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3324),
			gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );						// "[%s] ���� [%d] ���ܰ� ���� ����� �ξ����ϴ�."
		
		LogAddTD("[ Crywolf ][Altar Op.]  [%s][%s] Set Valid Contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber);

		AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_CONTRACTED );
	
		// ����� ��ȿ�ǰ� ���� ����Ǵ� ����/���� ����
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( &gObj[iAltarObjIndex], BUFFTYPE_CRYWOLF_CONTRACT, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
		gObj[iAltarObjIndex].m_ViewSkillState = 0;
		ADD_VIEWSKILLSTATE( gObj[iAltarObjIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_CONTRACTED );
		GCStateInfoSend( &gObj[iAltarObjIndex], 1, gObj[iAltarObjIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}

}

BOOL CCrywolfAltar::CheckAltarValidContract( int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);
	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	if( AltarInfo.GetAltarState() == CRYWOLF_ALTAR_STATE_CONTRACTED )
		return TRUE;

	return FALSE;
}


// ������ ���� ����(����)�� �����Ѵ�.
BOOL CCrywolfAltar::SetAltarUserIndex( int iAltarObjIndex, int iClass, int iUserIndex )
{
	int iAltarNumber = GetAlatarNumber(iClass);
	
	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	DWORD dwCurrentTime = GetTickCount();

	// 0. > �̹� ����� ������ �ִ��� üũ
	if( gObjIsConnected(AltarInfo.m_iUserIndex) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3325) , iAltarNumber );			// "������ [%d] ������ �̹� ��� �ƽ��ϴ�."
		return FALSE;
	}

	// 1. > ������ ������ �ð����� üũ
	if( dwCurrentTime - AltarInfo.m_dwLastValidContractTime < CRYWOLF_ALTER_RECONTRACT_TIME * 1000 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3326) , iAltarNumber );			// "������ [%d] ������ ���� ������ ������ �ð��� �ƴմϴ�."
		return FALSE;
	}

	// 2. > �ִ� ��� Ƚ���� �ʰ� �ߴ��� üũ
	if( AltarInfo.m_iContractCount > MAX_CONTRACT_COUNT )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3327) , iAltarNumber );			// "������ [%d] ������ �ִ� ��� Ƚ���� �ʰ� �߽��ϴ�."
		return FALSE;
	}
	
	// 3. > ���ܰ� ��ġ�� ������ üũ
	if( abs(gObj[iAltarObjIndex].Y - gObj[iUserIndex].Y) > 0 
		|| abs(gObj[iAltarObjIndex].X - gObj[iUserIndex].X) > 0 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3328) , iAltarNumber );			// "������ [%d] ���ܰ� ��� ��ġ�� ���� �ʽ��ϴ�. �ٽ� �õ� �Ͻʽÿ�."
		return FALSE;
	}
	AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT );
	AltarInfo.m_iUserIndex				= iUserIndex;
	AltarInfo.m_dwAppliedContractTime	= dwCurrentTime;
	AltarInfo.m_dwValidContractTime		= dwCurrentTime + CRYWOLF_ALTER_CONTRACTING_TIME*1000;
	AltarInfo.m_dwLastValidContractTime = dwCurrentTime;
	AltarInfo.m_iContractCount++;

	// 1. ��� �Ǵ� ���� ����Ʈ�� ǥ��
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( &gObj[iAltarObjIndex], BUFFTYPE_CRYWOLF_CONTRACT_TRY, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	gObj[iAltarObjIndex].m_ViewSkillState = 0;
	ADD_VIEWSKILLSTATE( gObj[iAltarObjIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_ATTEMPT );
	GCStateInfoSend( &gObj[iAltarObjIndex], 1, gObj[iAltarObjIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	// 2. ��� �ʿ� ���� ���� �� �� ���� ��ȭ �� ����

	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3329) , 
			gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );			// "[%s] ���� [%d] ���ܰ� ���� ����� �õ��ϴ� ���Դϴ�."

	LogAddTD("[ Crywolf ][Altar Op.] [%s][%s] Attempt to contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );

	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	MsgOutput( iUserIndex, lMsg.Get(3330) );			// "��� �������� �� 5�ʰ� �ҿ�˴ϴ�."
	MsgOutput( iUserIndex, lMsg.Get(3331) );			// "��� �ϷḦ ���� �������� ���ʽÿ�."

	return TRUE;
}

// ������ ���� ����(����)�� �ʱ�ȭ �Ѵ�.
void CCrywolfAltar::ResetAltarUserIndex( int iAltarObjIndex, int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	// 0. > �̹� ����� ������ �ִٸ�,
	if( gObjIsConnected(AltarInfo.m_iUserIndex) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
		MsgOutput( AltarInfo.m_iUserIndex, lMsg.Get(3332) , iAltarNumber );			// "������ [%d] ���ܰ��� ����� ���� �˴ϴ�."
		
		LogAddTD("[ Crywolf ][Altar Op.] [%s][%s] Remove contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );
	}

	if( AltarInfo.m_iContractCount >= MAX_CONTRACT_COUNT )
	{
		AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_EXCEEDING_CONTRACT_COUNT );

		// ���� ��� Ƚ���� �ʰ� �����Ƿ� �Ұ��� ���¸� �����ش�.
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( &gObj[iAltarObjIndex], BUFFTYPE_CRYWOLF_CONTRACT_DISABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
		gObj[iAltarObjIndex].m_ViewSkillState = 0;
		ADD_VIEWSKILLSTATE( gObj[iAltarObjIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_DISABLE );
		GCStateInfoSend( &gObj[iAltarObjIndex], 1, gObj[iAltarObjIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}
	else
	{
		// ���� ���� �ʾҴٸ�.
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( &gObj[iAltarObjIndex], BUFFTYPE_CRYWOLF_CONTRACT_ENABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
		gObj[iAltarObjIndex].m_ViewSkillState = 0;
		ADD_VIEWSKILLSTATE( gObj[iAltarObjIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_ENABLE );
		GCStateInfoSend( &gObj[iAltarObjIndex], 1, gObj[iAltarObjIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_NONE );
	}

	AltarInfo.m_iUserIndex				= _INVALID;
	AltarInfo.m_dwAppliedContractTime	= _ZERO;
	AltarInfo.m_dwValidContractTime		= _ZERO;
	AltarInfo.m_dwLastValidContractTime = GetTickCount();

	
}

void CCrywolfAltar::SetAltarObjIndex( int iClass, int iObjIndex )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	ResetAltar(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];
	AltarInfo.SetAltarIndex( iObjIndex );
}

void CCrywolfAltar::SetAllAltarObjectIndex()
{
	for( int i=0; i<MAX_MONSTER; i++ )
	{
		if( !gObjIsConnected(i) )
			continue;
		
		if( gObj[i].MapNumber != MAP_INDEX_CRYWOLF_FIRSTZONE )
			continue;
		
		if( CHECK_CRYWOLF_NPC_ALTAR(gObj[i].Class) )
		{
			g_CrywolfNPC_Altar.SetAltarObjIndex( gObj[i].Class, i );
		}
	}
}

void CCrywolfAltar::ResetAllAltar()
{
	ResetAltar(CRYWOLF_NPC_ALTAR1);
	ResetAltar(CRYWOLF_NPC_ALTAR2);
	ResetAltar(CRYWOLF_NPC_ALTAR3);
	ResetAltar(CRYWOLF_NPC_ALTAR4);
	ResetAltar(CRYWOLF_NPC_ALTAR5);
}

void CCrywolfAltar::ResetAltar( int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	if( AltarInfo.m_iAltarIndex == _INVALID )
		return;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjClearBuffEffect( &gObj[AltarInfo.m_iAltarIndex], CLEAR_TYPE_LOGOUT );
#else
	gObj[AltarInfo.m_iAltarIndex].m_ViewSkillState = 0;
	GCStateInfoSend( &gObj[AltarInfo.m_iAltarIndex], 1, gObj[AltarInfo.m_iAltarIndex].m_ViewSkillState );
#endif // MODIFY_CASTLESIEGE_IDENTIFICATION_20051012

	AltarInfo.Reset();	
}

void CCrywolfAltar::SetAllAltarViewState( int iAltarState )
{
	SetAltarViewState(CRYWOLF_NPC_ALTAR1, iAltarState);
	SetAltarViewState(CRYWOLF_NPC_ALTAR2, iAltarState);
	SetAltarViewState(CRYWOLF_NPC_ALTAR3, iAltarState);
	SetAltarViewState(CRYWOLF_NPC_ALTAR4, iAltarState);
	SetAltarViewState(CRYWOLF_NPC_ALTAR5, iAltarState);
}

void CCrywolfAltar::SetAltarViewState( int iClass, int iAltarState )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	if( AltarInfo.m_iAltarIndex == _INVALID )
		return;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( &gObj[AltarInfo.m_iAltarIndex], BUFFTYPE_CRYWOLF_CONTRACT_ENABLE, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	gObj[AltarInfo.m_iAltarIndex].m_ViewSkillState = 0;
	ADD_VIEWSKILLSTATE( gObj[AltarInfo.m_iAltarIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_ENABLE );
	GCStateInfoSend( &gObj[AltarInfo.m_iAltarIndex], 1, gObj[AltarInfo.m_iAltarIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
}



// ���Ǿ� �ִ� ������ ����
int	CCrywolfAltar::GetContractedAltarCount()
{
	int iContractedAltarCount = 0;

	for( int i=0; i<MAX_CRYWOLF_ALTAR_COUNT; i++ )
	{
		CCrywolfAltarInfo &AltarInfo = m_AltarInfo[i];

		// ��� �Ǿ� �ִ��� üũ
		if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		iContractedAltarCount++;
	}

	return iContractedAltarCount;
}


int	CCrywolfAltar::GetPriestHPSum()
{
	int iPriestHPSum = 0;

	for( int i=0; i<MAX_CRYWOLF_ALTAR_COUNT; i++ )
	{
		CCrywolfAltarInfo &AltarInfo = m_AltarInfo[i];

		// 1. ��� �Ǿ� �ִ��� üũ
		if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		// 2. ���� �÷��� ���� ���� ���� üũ
		if( !gObjIsConnected(AltarInfo.m_iUserIndex) )
			continue;

		LPOBJECTSTRUCT lpUserObj	= &gObj[ AltarInfo.m_iUserIndex ];

		iPriestHPSum += lpUserObj->Life;
	}

	return iPriestHPSum;

}

int	CCrywolfAltar::GetPriestMaxHPSum()
{
	int iPriestHPSum = 0;

	for( int i=0; i<MAX_CRYWOLF_ALTAR_COUNT; i++ )
	{
		CCrywolfAltarInfo &AltarInfo = m_AltarInfo[i];

		// 1. ��� �Ǿ� �ִ��� üũ
		if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		// 2. ���� �÷��� ���� ���� ���� üũ
		if( !gObjIsConnected(AltarInfo.m_iUserIndex) )
			continue;

		LPOBJECTSTRUCT lpUserObj	= &gObj[ AltarInfo.m_iUserIndex ];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		int iAddLife = 0;
		gObjGetValueOfBuffIndex( lpUserObj, BUFFTYPE_HP_INC, &iAddLife );
		iPriestHPSum += lpUserObj->MaxLife + lpUserObj->AddLife + iAddLife;
#else
		iPriestHPSum += lpUserObj->MaxLife + lpUserObj->AddLife + lpUserObj->m_SkillAddLife;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}

	return iPriestHPSum;

}

// ������ ���� �ൿ
void CCrywolfAltar::CrywolfAltarAct( int iIndex )
{

	if (!gObjIsConnected(iIndex))
		return;
	
	if (gObj[iIndex].Type != OBJTYPE_NPC || !CHECK_CRYWOLF_NPC_ALTAR(gObj[iIndex].Class)) 
	{
		return;
	}
	
	int iUserIndex = GetAltarUserIndex( gObj[iIndex].Class );

	if ( !gObjIsConnected( iUserIndex ) ) 
	{
		if( iUserIndex != _INVALID )
			ResetAltarUserIndex( iIndex, gObj[iIndex].Class );

		return;
	}

	if( !gObjIsConnected( &gObj[iUserIndex] ) )
	{
		ResetAltarUserIndex( iIndex, gObj[iIndex].Class );
		return;
	}
	
	LPOBJECTSTRUCT lpObj		= &gObj[ iIndex ];
	LPOBJECTSTRUCT lpUserObj	= &gObj[ iUserIndex ];
	
	// 0. > �� �˻�
	if ( lpUserObj->Live
		 && lpUserObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE 
	   ) 
	{
		// 1. > ��ġ ��
		if ( abs(lpObj->Y - lpUserObj->Y) <= 0 && abs(lpObj->X - lpUserObj->X) <= 0 )
		{
			// 2. > ȿ���� ��ȿ�� ���� ó��
			if( CheckAltarValidContract(gObj[iIndex].Class) )
			{
				// ���� ���¸� ���� ��Ų��.
			}
			else
			{
				// ������ ��ȿ�ϰ� �����.
				SetAltarValidContract( iIndex, gObj[iIndex].Class);
			}

		}
		else 
		{
			ResetAltarUserIndex( iIndex, gObj[iIndex].Class);	
		}
	}
	else 
	{
		ResetAltarUserIndex( iIndex, gObj[iIndex].Class);
	}

	// ������ ���°� ���� ������ �������ݷ� ���� ������ �ѷ��ش�?
	
}

#endif