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
	case CRYWOLF_NPC_ALTAR1 :		// 늑대의 제단 1
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR1].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR2 :		// 늑대의 제단 2
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR2].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR3 :		// 늑대의 제단 3
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR3].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR4 :		// 늑대의 제단 4
		iAltarState = m_AltarInfo[CRYWOLF_ALTAR4].GetAltarState();
		break;

	case CRYWOLF_NPC_ALTAR5 :		// 늑대의 제단 5
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
	case CRYWOLF_NPC_ALTAR1 :		// 늑대의 제단 1
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR1].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR2 :		// 늑대의 제단 2
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR2].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR3 :		// 늑대의 제단 3
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR3].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR4 :		// 늑대의 제단 4
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR4].GetRemainContractCount();
		break;

	case CRYWOLF_NPC_ALTAR5 :		// 늑대의 제단 5
		iAltarRemainContractCount = m_AltarInfo[CRYWOLF_ALTAR5].GetRemainContractCount();
		break;
	}

	return iAltarRemainContractCount;
}

// 늑대의 제단 번호를 얻는다. ( 0 ~ 4 )
int	CCrywolfAltar::GetAlatarNumber( int iClass )
{
	int iAltarNumber = _ZERO;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// 늑대의 제단 1
		iAltarNumber = CRYWOLF_ALTAR1;
		break;

	case CRYWOLF_NPC_ALTAR2 :		// 늑대의 제단 2
		iAltarNumber = CRYWOLF_ALTAR2;
		break;

	case CRYWOLF_NPC_ALTAR3 :		// 늑대의 제단 3
		iAltarNumber = CRYWOLF_ALTAR3;
		break;

	case CRYWOLF_NPC_ALTAR4:		// 늑대의 제단 4
		iAltarNumber = CRYWOLF_ALTAR4;
		break;

	case CRYWOLF_NPC_ALTAR5 :		// 늑대의 제단 5
		iAltarNumber = CRYWOLF_ALTAR5;
		break;
	}

	return iAltarNumber;
}

// 늑대의 제단 사제(유저) 인덱스를 얻는다.
int	 CCrywolfAltar::GetAltarUserIndex( int iClass )
{
	int iUserIndex = _INVALID;

	switch( iClass ) 
	{
	case CRYWOLF_NPC_ALTAR1 :		// 늑대의 제단 1
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR1].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR2 :		// 늑대의 제단 2
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR2].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR3 :		// 늑대의 제단 3
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR3].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR4 :		// 늑대의 제단 4
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR4].m_iUserIndex;
		break;

	case CRYWOLF_NPC_ALTAR5 :		// 늑대의 제단 5
		iUserIndex = m_AltarInfo[CRYWOLF_ALTAR5].m_iUserIndex;
		break;
	}
	
	return iUserIndex;
}

// 늑대의 제단을 유효하게 만든다.
void CCrywolfAltar::SetAltarValidContract( int iAltarObjIndex, int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	// 0. > 계약 시도 중인지 체크
	if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT )
		return;

	// 1. > 계약 발효
	if( AltarInfo.m_dwValidContractTime < GetTickCount() )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( AltarInfo.m_iUserIndex, lMsg.Get(3323) , iAltarNumber );			// "늑대의 [%d] 제단과의 계약이 발효 됩니다."

		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3324),
			gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );						// "[%s] 님이 [%d] 제단과 사제 계약을 맺었습니다."
		
		LogAddTD("[ Crywolf ][Altar Op.]  [%s][%s] Set Valid Contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber);

		AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_CONTRACTED );
	
		// 계약이 발효되고 방어막에 적용되는 상태/정보 전송
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


// 늑대의 제단 사제(유저)를 설정한다.
BOOL CCrywolfAltar::SetAltarUserIndex( int iAltarObjIndex, int iClass, int iUserIndex )
{
	int iAltarNumber = GetAlatarNumber(iClass);
	
	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	DWORD dwCurrentTime = GetTickCount();

	// 0. > 이미 계약한 유저가 있는지 체크
	if( gObjIsConnected(AltarInfo.m_iUserIndex) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3325) , iAltarNumber );			// "늑대의 [%d] 제단은 이미 계약 됐습니다."
		return FALSE;
	}

	// 1. > 재계약이 가능한 시간인지 체크
	if( dwCurrentTime - AltarInfo.m_dwLastValidContractTime < CRYWOLF_ALTER_RECONTRACT_TIME * 1000 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3326) , iAltarNumber );			// "늑대의 [%d] 제단은 아직 재계약이 가능한 시간이 아닙니다."
		return FALSE;
	}

	// 2. > 최대 계약 횟수를 초과 했는지 체크
	if( AltarInfo.m_iContractCount > MAX_CONTRACT_COUNT )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3327) , iAltarNumber );			// "늑대의 [%d] 제단은 최대 계약 횟수를 초과 했습니다."
		return FALSE;
	}
	
	// 3. > 제단과 위치가 같은지 체크
	if( abs(gObj[iAltarObjIndex].Y - gObj[iUserIndex].Y) > 0 
		|| abs(gObj[iAltarObjIndex].X - gObj[iUserIndex].X) > 0 )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( iUserIndex, lMsg.Get(3328) , iAltarNumber );			// "늑대의 [%d] 제단과 계약 위치가 맞지 않습니다. 다시 시도 하십시오."
		return FALSE;
	}
	AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT );
	AltarInfo.m_iUserIndex				= iUserIndex;
	AltarInfo.m_dwAppliedContractTime	= dwCurrentTime;
	AltarInfo.m_dwValidContractTime		= dwCurrentTime + CRYWOLF_ALTER_CONTRACTING_TIME*1000;
	AltarInfo.m_dwLastValidContractTime = dwCurrentTime;
	AltarInfo.m_iContractCount++;

	// 1. 계약 되는 상태 뷰포트에 표시
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( &gObj[iAltarObjIndex], BUFFTYPE_CRYWOLF_CONTRACT_TRY, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	gObj[iAltarObjIndex].m_ViewSkillState = 0;
	ADD_VIEWSKILLSTATE( gObj[iAltarObjIndex].m_ViewSkillState, STATE_CRYWOLF_ALTAR_ATTEMPT );
	GCStateInfoSend( &gObj[iAltarObjIndex], 1, gObj[iAltarObjIndex].m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	// 2. 계약 됨에 따라 석상 방어막 및 상태 변화 값 전송

	// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
	UTIL.SendCrywolfUserAnyMsg( 2, lMsg.Get(3329) , 
			gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );			// "[%s] 님이 [%d] 제단과 사제 계약을 시도하는 중입니다."

	LogAddTD("[ Crywolf ][Altar Op.] [%s][%s] Attempt to contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );

	// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
	MsgOutput( iUserIndex, lMsg.Get(3330) );			// "계약 성립까지 약 5초가 소요됩니다."
	MsgOutput( iUserIndex, lMsg.Get(3331) );			// "계약 완료를 위해 움직이지 마십시오."

	return TRUE;
}

// 늑대의 제단 사제(유저)를 초기화 한다.
void CCrywolfAltar::ResetAltarUserIndex( int iAltarObjIndex, int iClass )
{
	int iAltarNumber = GetAlatarNumber(iClass);

	CCrywolfAltarInfo &AltarInfo = m_AltarInfo[ iAltarNumber ];

	// 0. > 이미 계약한 유저가 있다면,
	if( gObjIsConnected(AltarInfo.m_iUserIndex) )
	{
		// MODIFY_LOCALIZING_WTF_20060309 로컬라이징 wtf 파일 업데이트 0.99.60 - 0.99.94
		MsgOutput( AltarInfo.m_iUserIndex, lMsg.Get(3332) , iAltarNumber );			// "늑대의 [%d] 제단과의 계약이 해제 됩니다."
		
		LogAddTD("[ Crywolf ][Altar Op.] [%s][%s] Remove contract Altar[%d]",
			gObj[AltarInfo.m_iUserIndex].AccountID, gObj[AltarInfo.m_iUserIndex].Name, iAltarNumber );
	}

	if( AltarInfo.m_iContractCount >= MAX_CONTRACT_COUNT )
	{
		AltarInfo.SetAltarState( CRYWOLF_ALTAR_STATE_EXCEEDING_CONTRACT_COUNT );

		// 제단 계약 횟수가 초과 했으므로 불가능 상태를 보내준다.
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
		// 점령 되지 않았다면.
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



// 계약되어 있는 제단의 개수
int	CCrywolfAltar::GetContractedAltarCount()
{
	int iContractedAltarCount = 0;

	for( int i=0; i<MAX_CRYWOLF_ALTAR_COUNT; i++ )
	{
		CCrywolfAltarInfo &AltarInfo = m_AltarInfo[i];

		// 계약 되어 있는지 체크
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

		// 1. 계약 되어 있는지 체크
		if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		// 2. 게임 플레이 중인 사제 인지 체크
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

		// 1. 계약 되어 있는지 체크
		if( AltarInfo.GetAltarState() != CRYWOLF_ALTAR_STATE_CONTRACTED )
			continue;

		// 2. 게임 플레이 중인 사제 인지 체크
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

// 늑대의 제단 행동
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
	
	// 0. > 맵 검사
	if ( lpUserObj->Live
		 && lpUserObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE 
	   ) 
	{
		// 1. > 위치 비교
		if ( abs(lpObj->Y - lpUserObj->Y) <= 0 && abs(lpObj->X - lpUserObj->X) <= 0 )
		{
			// 2. > 효과가 발효된 제단 처리
			if( CheckAltarValidContract(gObj[iIndex].Class) )
			{
				// 현재 상태를 지속 시킨다.
			}
			else
			{
				// 제단을 유효하게 만든다.
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

	// 제단의 상태가 변경 됐으면 프로토콜로 상태 정보를 뿌려준다?
	
}

#endif