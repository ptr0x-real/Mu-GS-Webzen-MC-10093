// MasterSkillSystem.cpp: implementation of the CMasterSkillSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterSkillSystem.h"

#include "Include\ReadScript.h"
#include "DSProtocol.h"
#include "MagicDamage.h"
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
#include "ObjCalCharacter.h"
#include "GameMain.h"
#include "SkillAdditionInfo.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "PartyClass.h"
extern PartyClass	gParty;
extern int g_EnergyCheckOff;
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122

CMasterSkillSystem g_MasterSkillSystem;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterSkillSystem::CMasterSkillSystem()
{
	Init();
}

CMasterSkillSystem::~CMasterSkillSystem()
{

}

void CMasterSkillSystem::Init()
{
	for( int iCount = 0; iCount < MAX_SKILL_TABLE_DATA; iCount++ )
	{
		ZeroMemory( &m_MLSkillTable[iCount], sizeof( ML_SKILL_TABLE ) );
	}

	for( int iRankCount = 0; iRankCount < MAX_RANK; iRankCount++ )
	{
		ZeroMemory( &m_iRankTable[iRankCount], sizeof( INT ) * ( MAX_RANK_LEVEL - 1 ) );
	}	
}

void CMasterSkillSystem::Load( LPCSTR lpszFileName )
{
	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[MasterLevelSkillTree] - File load error : File Name Error" );
		return;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			MsgBox( "[MasterLevelSkillTree] - Can't Open %s ", lpszFileName );			
			return;
		}
		
		// 모든 데이터 초기화.
		Init();

		SMDToken Token;
		INT		iIndex = -1;

		INT		iRankCount = 0;
		INT		iRankLevelCount = 0;

		while( TRUE ) 
		{
			Token = GetToken();
			if( Token == END ) break;
			iIndex = (INT)TokenNumber;

			while( TRUE )
			{
				// 1 .> 마스터레벨 스킬 단계별 투자 요구 포인트
				if( iIndex == 0 )
				{
					// . >	데이터 읽기
					Token = GetToken(); 
					if( strcmp( "end", TokenString ) == NULL ) 
						break;

					m_iRankTable[iRankCount][iRankLevelCount] = (INT)TokenNumber;
					m_iRankPointSum[iRankCount] += (INT)TokenNumber;	// 스킬 단계별 투자 요구 포인트를 모두 더한 값이다.

					++iRankLevelCount;					
					if( iRankLevelCount >= MAX_RANK_LEVEL )
					{
						iRankLevelCount = 0;
						++iRankCount;			
					}
				}
				// 2 .> 마스터레벨 스킬 트리
				else if( iIndex > 0 )
				{
					// 미리 계산해 둔 정보
					m_MLSkillTable[iIndex].btClass	= iIndex / 4;			// 캐릭터 클래스 번호
					m_MLSkillTable[iIndex].btType	= (BYTE)( iIndex % 4 );	// 스킬 테이블 타입

					// 몇줄짜리인가 정보(UI상에서 세로 줄 수)
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;

					m_MLSkillTable[iIndex].btWidth = (BYTE)TokenNumber;								

					// 스킬 테이블 정보를 가져온다.
					for( INT iRow = 0; iRow < MAX_TABLE_ROW; iRow++ )
					{
						for( INT iColumn = 0; iColumn < MAX_TABLE_COLUMN; iColumn++ )
						{
							Token = GetToken(); 
							m_MLSkillTable[iIndex].iAbility[iRow][iColumn] = (INT)TokenNumber;
						}
					}
				}
				else
				{
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) 
						break;
				}
			}
		}
		fclose( SMDFile );
		
		LogAddTD( "[MasterLevelSkillTree] - %s file is Loaded", lpszFileName );
	}
	catch(...) 
	{
		DWORD dwError = GetLastError();
		MsgBox( "[MasterLevelSkillTree] - Loading Exception Error (%s) File. ", lpszFileName );
	}
}


//////////////////////////////////////////////////////////////////////////
// S E T
//////////////////////////////////////////////////////////////////////////

VOID CMasterSkillSystem::InitPassiveSkillData( LPOBJECTSTRUCT lpObj )
{
	ZeroMemory( &lpObj->m_MPSkillOpt, sizeof( MASTERLEVEL_PASSIVE_SKILL_OPTION ) );
}


VOID CMasterSkillSystem::CalcPassiveSkillData( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel )
{
	// 스킬은 사용할 때 필요 힘과 민첩을 확인한다.
	// (열매 아이템을 사용 할 경우 힘과 민첩이 다운될 수 있기 때문)
	// 필요 힘
	INT iReqStr = MagicDamageC.GetRequireStrength( iMLSkill );
	if( lpObj->Strength < iReqStr )
		return;

	// 필요 민첩
	INT iReqDex = MagicDamageC.GetRequireDexterity( iMLSkill );
	if( lpObj->Dexterity < iReqDex )
		return;
	
	INT iSkillValue = MagicDamageC.SkillGet( iMLSkill );

	INT iBaseMLS = GetBaseMasterLevelSkill( iMLSkill );
	switch( iBaseMLS )
	{
	case AT_SKILL_ML_ATTACK_SUCCESS_RATE:				// 공격성공률 상승
		lpObj->m_MPSkillOpt.iMpsAttackSuccessRate		= iSkillValue;	
		break;

	case AT_SKILL_ML_PVP_ATTACKDAMAGE_RATE:				// 대인공격률 상승	
		lpObj->m_MPSkillOpt.iMpsPVPAttackDmgRate		= iSkillValue;
		break;

	case AT_SKILL_ML_PVP_BLOCKING_RATE:					// 대인방어율 증가		
		lpObj->m_MPSkillOpt.iMpsPVPBlockingRate			= iSkillValue;
		break;

	case AT_SKILL_ML_DOWN_DUR1:							// 내구 감소1
		lpObj->m_MPSkillOpt.iMpsDownDur1				= iSkillValue;
		lpObj->m_MPSkillOpt.btMpsDownDur1Level			= iSkillLevel;
		break;

	case AT_SKILL_ML_DOWN_DUR2:							// 내구 감소2
		lpObj->m_MPSkillOpt.iMpsDownDur2				= iSkillValue;
		break;

	case AT_SKILL_ML_RESISTANCE_POISON:					// 독 저항력 상승
		lpObj->m_MPSkillOpt.iMpsResistancePoison		= iSkillValue;		
		break;

	case AT_SKILL_ML_RESISTANCE_THUNDER:				// 번개 저항력 상승
		lpObj->m_MPSkillOpt.iMpsResistanceThunder		= iSkillValue;		
		break;

	case AT_SKILL_ML_RESISTANCE_ICE:					// 얼음 저항력 상승
		lpObj->m_MPSkillOpt.iMpsResistanceIce			= iSkillValue;		
		break;

	case AT_SKILL_ML_AUTO_RECOVER_LIFE:					// 생명력 자동회복 강화
		lpObj->m_MPSkillOpt.iMpsAutoRecoverLife			= iSkillValue;
		break;

	case AT_SKILL_ML_PLUS_ZEN:							// 젠 증가
		lpObj->m_MPSkillOpt.iMpsPlusZen					= iSkillValue;
		break;

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107			// 패시브 2차 - CalcPassiveSkillData(...) 
	case AT_SKILL_ML_DEFENCE:							// 방어력 증가
		lpObj->m_MPSkillOpt.iMpsDefence					= iSkillValue;
		break;

	case AT_SKILL_ML_MAX_HP:							// 최대HP 증가
		lpObj->m_MPSkillOpt.iMpsMaxHP					= iSkillValue;
		break;
		
	case AT_SKILL_ML_MAX_AG:							// 최대AG 증가
		lpObj->m_MPSkillOpt.iMpsMaxBP					= iSkillValue;
		break;

	case AT_SKILL_ML_MONSTER_KILL_MANA:					// 몬스터 킬 시 마나회복
		lpObj->m_MPSkillOpt.iMpsMonsterDieGetMana		 = iSkillValue;			
		break;

	case AT_SKILL_ML_MONSTER_KILL_HP:					// 몬스터 킬 시 생명력회복
		lpObj->m_MPSkillOpt.iMpsMonsterDieGetLife		 = iSkillValue;
		break;

	case AT_SKILL_ML_MONSTER_KILL_SD:					// 몬스터 킬 시 SD회복
		lpObj->m_MPSkillOpt.iMpsMonsterDieGetSD			 = iSkillValue;
		break;

	case AT_SKILL_ML_PLUS_EXP:							// 경험치 상승
		lpObj->m_MPSkillOpt.iMpsPlusExp					 = iSkillValue;
		break;

#endif	// ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107
		
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터레벨 스킬 4랭크
	case AT_SKILL_ML_MAX_SD:							// 쵀대 SD 증가
		lpObj->m_MPSkillOpt.iMpsMaxSD					= iSkillValue;
		break;	
	case AT_SKILL_ML_SD_SPEED:							// SD 회복 속도 상승
		lpObj->m_MPSkillOpt.iMpsSDSpeed					= iSkillValue;
		break;		
	case AT_SKILL_ML_MAX_ATTACK_DAMAGE:					// 최대 공격력 상승
		lpObj->m_MPSkillOpt.iMpsMaxAttackDamage			= iSkillValue;
		break;
	case AT_SKILL_ML_MIN_ATTACK_DAMAGE:					// 최소 공격력 상승
		lpObj->m_MPSkillOpt.iMpsMinAttackDamage			= iSkillValue;
		break;
	case AT_SKILL_ML_DECREASE_MANA:						// 마나 감소 강화
		lpObj->m_MPSkillOpt.iMpsDecreaseMana			= iSkillValue;
		break;
	case AT_SKILL_ML_MAX_MAGIC_DAMAGE:					// 최대 마력 상승(흑마법사)
		lpObj->m_MPSkillOpt.iMpsMaxMagicDamage			= iSkillValue;
		break;
	case AT_SKILL_ML_MIN_MAGIC_DAMAGE:					// 최소 마력 상승(흑마법사)
		lpObj->m_MPSkillOpt.iMpsMinMagicDamage			= iSkillValue;
		break;
	case AT_SKILL_ML_PET_DUR_DOWN_SPEED:				// 펫 내구 소모 속도 감소
		lpObj->m_MPSkillOpt.iMpsPetDurDownSpeed			= iSkillValue;
		break;
	case AT_SKILL_ML_MAX_ATTACK_MAGIC_DAMAGE:			// 최대 공격력/마력 상승(마검사)
		lpObj->m_MPSkillOpt.iMpsMaxAttackMagicDamage	= iSkillValue;
		break;
	case AT_SKILL_ML_MIN_ATTACK_MAGIC_DAMAGE:			// 최소 공격력/마력 상승(마검사)
		lpObj->m_MPSkillOpt.iMpsMinAttackMagicDamage	= iSkillValue;
		break;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	}
	
	// 변경된 수치 적용.
	gObjCalCharacter( lpObj->m_Index );

}




//////////////////////////////////////////////////////////////////////////
// C H E C K
//////////////////////////////////////////////////////////////////////////

// 스킬을 사용할 수 있는지 조건을 검사한다.
INT	 CMasterSkillSystem::CheckSkillCondition( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel )
{	
	if( gObjIsConnected( lpObj ) == FALSE )
	{
		GCResultSend( lpObj->m_Index, 0x51, 0x03 );
		return RM_INVALID_USER;
	}
	
	if( iMLSkill < 0 || iMLSkill > MAX_SKILL - 1 ) 
	{
		LogAdd(lMsg.Get(457),__FILE__, __LINE__);
		return RM_INVALID_SKILL_NUM;
	}

	// 조건 확인
	//////////////////////////////////////////////////////////////////////////	
	// 마스터레벨 사용자 확인
	if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		return RM_INVALID_USER;
	
	// 마스터레벨 스킬인지 확인
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 액티브 스킬 확인 추가
	if( !g_MasterSkillSystem.CheckMasterLevelSkill( iMLSkill ) )
#else
	if( MagicDamageC.GetSkillUseType( iMLSkill ) != SKILL_USETYPE_MASTERSKILL_PASSIVE )
#endif
		return RM_INVALID_CONDITION;
	
	// 필요마스터 레벨 포인트	
	INT iReqMLPoint = MagicDamageC.GetRequireMLPoint( iMLSkill );
	if( lpObj->m_iMasterLevelPoint < iReqMLPoint )
		return RM_INVALID_CONDITION;

	// 선행스킬 검사.
	if( !CheckCompleteBrandOfSkill( lpObj, iMLSkill ) )
	{
		return RM_INVALID_CONDITION;
	}

	// 이 전 랭크의 요구 포인트를 달성 했나?	-> 클라이언트에서 검사.
//	if( iSkillLevel == BASE_ML_SKILL_LEVEL )
//	{		
//		INT iSkillRank = MagicDamageC.GetSkillRank( iMLSkill );		
//	}
	
	// 랭크 그룹 확인 : 레벨이 있는 경우 같은 그룹인지 확인해야한다.
	INT iSkillGroup = MagicDamageC.GetSkillGroup( iMLSkill );
	
	INT iTargetSkillGroup = 0;
	if( iSkillLevel > BASE_RANK_LEVEL )
	{
		for( INT iCount = 0; iCount < MAX_MAGIC; iCount++ )
		{
			if( lpObj->Magic[iCount].IsMagic() == TRUE )
			{
				iTargetSkillGroup = MagicDamageC.GetSkillGroup( lpObj->Magic[iCount].m_Skill );				
				if( iTargetSkillGroup > 0 )
				{
					if( iTargetSkillGroup	== iSkillGroup											// 같은 그룹일 때
						&& lpObj->Magic[iCount].m_Skill	== ( iMLSkill - BASE_ML_SKILL_LEVEL )		// 스킬 번호가 하나 낮고
						&& lpObj->Magic[iCount].m_Level	== ( iSkillLevel - BASE_ML_SKILL_LEVEL_GAP ) // 한 단계 낮은 레벨의 스킬이면															
						)
					{	
						return RM_OK;	// 이거다!
					}	
				}
			}
		}
		return RM_INVALID_CONDITION;	// 랭크그룹이 있어야하는데 없다!
	}
	
	return RM_OK;
}

BOOL CMasterSkillSystem::CheckCompleteBrandOfSkill( LPOBJECTSTRUCT lpObj, INT iMLSkill )
{
	INT iBrandOfSkill = MagicDamageC.GetBrandOfSkill( iMLSkill );
	if( iBrandOfSkill <= 0 )
	{
		// 선행 스킬이 필요 없는 애들.
		return TRUE;
	}
	else
	{	// 선행 스킬이 필요한 애들.
		for( INT iCount = 0; iCount < MAX_MAGIC; iCount++ )
		{
			if( lpObj->Magic[iCount].IsMagic() == TRUE )
			{
				if( lpObj->Magic[iCount].m_Skill == iBrandOfSkill )
				{	// 있다.
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;	// 없다.
}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
// 스킬 번호로 마스터레벨 스킬인지 확인한다.
BOOL CMasterSkillSystem::CheckMasterLevelSkill( INT iMLSkill )										
{
	if( MagicDamageC.GetSkillUseType( iMLSkill ) == SKILL_USETYPE_MASTERSKILL_PASSIVE 
		|| MagicDamageC.GetSkillUseType( iMLSkill ) == SKILL_USETYPE_MASTERSKILL_ACTIVE
		)
		return TRUE;

	return FALSE;
}

// 일반 스킬번호가 기본이되는 마스터레벨 스킬을 갖구있어?
BOOL CMasterSkillSystem::IsExistMLSBasedSkill( LPOBJECTSTRUCT lpObj, INT iSkill )
{
	INT iBaseMLS = 0;

	for( INT iCount = 0; iCount < MAX_MAGIC; iCount++ )
	{
		if( lpObj->Magic[iCount].IsMagic() == TRUE )
		{
			// 마스터레벨 스킬일 때
			if( CheckMasterLevelSkill( lpObj->Magic[iCount].m_Skill ) )
			{
				// 기본 마스터레벨스킬 선행스킬이 궁금해하는 일반 스킬번호와 일치하면 오케이.
				iBaseMLS = GetBaseMasterLevelSkill( lpObj->Magic[iCount].m_Skill );
				if( iBaseMLS > 0 )
				{
					if( iSkill == MagicDamageC.GetBrandOfSkill( iBaseMLS ) )
						return TRUE;					
				}
			}
		}
	}	
	
	return FALSE;
}
#endif

//////////////////////////////////////////////////////////////////////////
// G E T
//////////////////////////////////////////////////////////////////////////

#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107
// 스킬과 레벨값으로 기본 마스터레벨 스킬 번호를 얻는다.
INT	CMasterSkillSystem::GetBaseMasterLevelSkill( INT iMLSkill )
{
	if( !CheckMasterLevelSkill( iMLSkill ) )
		return -1;
	
	// 마스터레벨스킬의 레벨을 먼저 구한다.
	int iSkillLevel = MagicDamageC.GetSkillLevel( iMLSkill );
	if( iSkillLevel <= 0 )
		return -1;	// 마스터레벨 스킬은 1부터 시작한다.

	int iBaseMLS = ( iMLSkill - iSkillLevel ) + 1;
	return iBaseMLS;
}

#endif



#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// gObjMagicAdd_MLS(...)
//////////////////////////////////////////////////////////////////////////
// A D D
//////////////////////////////////////////////////////////////////////////

// 마스터레벨 스킬을 마법리스트에 추가한다.
// ** 일반 스킬은 gObjMagicAdd에서 처리한다.
INT	CMasterSkillSystem::gObjMagicAdd_MLS( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel )
{
	int skill = -1;
	int n;

	skill = -1;

	int reqeng;

	if( g_EnergyCheckOff == TRUE )
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy( iMLSkill );
		if( reqeng < 0 ) 
		{
			LogAdd( lMsg.Get(549) );
			return -1;
		}

#ifdef MODIFY_SKILL_REQUIREENERGY_20080423
	if (lpObj->Class == CLASS_KNIGHT)	reqeng -= 10;
	if (lpObj->Class == CLASS_ELF)		reqeng -= 5;
#endif // MODIFY_SKILL_REQUIREENERGY_20080423
	
		if( ( lpObj->Energy + lpObj->AddEnergy ) < reqeng ) 
		{
			LogAdd( lMsg.Get(549) );
			return -1;
		}
	}	

	if( MagicDamageC.SkillGetRequireClass( lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, iMLSkill ) < 1 )
	{
		LogAdd( lMsg.Get(550) );
		return -1;
	}

	// 같은 마법이 있는지 검색해서 같은 마법은 등록이 안되게 한다.
	for( n = 0; n < MAX_MAGIC; n++ )
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{			
			if( lpObj->Magic[n].m_Skill == iMLSkill )
			{
				LogAdd( lMsg.Get(555), lpObj->Magic[n].m_Skill , iMLSkill );
				return -1;
			}
		}
	}

	// 패시브 스킬
	if( MagicDamageC.GetSkillUseType( iMLSkill ) == SKILL_USETYPE_MASTERSKILL_PASSIVE )
	{
		// 마스터레벨 스킬이고 레벨이 2 이상이면(기본 마스터스킬 레벨이 아니면) 갱신해야한다.
		if( iSkillLevel > BASE_ML_SKILL_LEVEL )
		{
			for( n = 0; n < MAX_MAGIC; n++ )
			{
				if( lpObj->Magic[n].IsMagic() == TRUE 
					&& MagicDamageC.GetSkillGroup( lpObj->Magic[n].m_Skill ) == MagicDamageC.GetSkillGroup( iMLSkill )
					&& lpObj->Magic[n].m_Skill == ( iMLSkill - BASE_ML_SKILL_LEVEL_GAP ) 
					&& lpObj->Magic[n].m_Level == ( iSkillLevel - BASE_ML_SKILL_LEVEL_GAP ) 
					)
				{
					skill = lpObj->Magic[n].UpdateMasterSkill( iMLSkill, iSkillLevel );
					if( skill < 0 ) 
					{
						return -1;
					}	
					return n;
				}
			}
		}
	}
	// 액티브 스킬
	else if( MagicDamageC.GetSkillUseType( iMLSkill ) == SKILL_USETYPE_MASTERSKILL_ACTIVE )
	{	
		// ** 액티브 스킬은 선행스킬을 기준으로 업데이트한다. ( 패시브는 작업도중 규칙이 뒤죽박죽 됐음!! )
		for( n = 0; n < MAX_MAGIC; n++ )
		{
			if( lpObj->Magic[n].IsMagic() == TRUE 
				&& lpObj->Magic[n].m_Skill == MagicDamageC.GetBrandOfSkill( iMLSkill )
				)
			{
				skill = lpObj->Magic[n].UpdateMasterSkill( iMLSkill, iSkillLevel );
				if( skill < 0 ) 
				{
					return -1;
				}	
				return n;
			}
		}
	}
	else
	{
		// 에러!!
		return -1;	
	}	

	// 추가 안된건 여기서 다~ 추가한다. (처음추가되는거얌)
	for( n = 0; n < MAX_MAGIC; n++ )
	{
		if( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set( iMLSkill, iSkillLevel );
			if( skill < 0 ) 
			{
				LogAdd( lMsg.Get(553), __FILE__, __LINE__ );
				return -1;
			}			
			lpObj->MagicCount++;
			return n;
		}
	}

	LogAdd( lMsg.Get(554), __FILE__, __LINE__ );
	return -1;
}

#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107



//////////////////////////////////////////////////////////////////////////
// Client <-> GS 처리
//////////////////////////////////////////////////////////////////////////

// CG [0xF3][0x52] 마스터레벨 스킬 습득 요청	
VOID CMasterSkillSystem::CGReqGetMasterLevelSkill( LPPMSG_REQ_MASTERLEVEL_SKILL lpMsg, int aIndex )	
{
	// 유효성 검사
	if( gObjIsConnected( aIndex ) == FALSE )
	{
		CloseClient ( aIndex );
		return;
	}

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];	

	PMSG_ANS_MASTERLEVEL_SKILL pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xF3, 0x52, sizeof( PMSG_ANS_MASTERLEVEL_SKILL ) );
	pMsg.btResult		= 0;
	pMsg.nMLPoint		= lpObj->m_iMasterLevelPoint;
	pMsg.nSkillNum		= -1;
	pMsg.nSkillLevel	= 0;

	INT iSkillLevel = MagicDamageC.GetSkillLevel( lpMsg->iMasterLevelSkill );
	INT iResult =  CheckSkillCondition( lpObj, lpMsg->iMasterLevelSkill, iSkillLevel );
	if( iResult == RM_OK )	// 성공
	{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
		if( gObjMagicAdd_MLS( lpObj, lpMsg->iMasterLevelSkill, iSkillLevel ) < 0 )
#else
		if( gObjMagicAdd( lpObj, lpMsg->iMasterLevelSkill, iSkillLevel ) < 0 )
#endif
		{
			// 추가(갱신) 실패
			iResult = RM_ADD_MAGIC_FAIL;
			LogAddTD( "[MasterSkill] [%s][%s] Fail - Add Magic List, Skill:%d Level:%d",
				lpObj->AccountID, lpObj->Name,
				lpMsg->iMasterLevelSkill, iSkillLevel );
		}
		else
		{
			// 추가(갱신) 성공
			// 마스터레벨 포인트를 깎고
			lpObj->m_iMasterLevelPoint -= MagicDamageC.GetRequireMLPoint( lpMsg->iMasterLevelSkill );
			
			pMsg.btResult		= iResult;
			pMsg.nMLPoint		= lpObj->m_iMasterLevelPoint;
			pMsg.nSkillNum		= lpMsg->iMasterLevelSkill;
			pMsg.nSkillLevel	= iSkillLevel;			

			// 패시브 스킬 정보 재계산
			CalcPassiveSkillData( lpObj, lpMsg->iMasterLevelSkill, iSkillLevel );
			LogAddTD( "[MasterSkill] [%s][%s] Success - Add Magic List, Skill:%d Level:%d MLPoint:%d",
				lpObj->AccountID, lpObj->Name,
				lpMsg->iMasterLevelSkill, iSkillLevel, lpObj->m_iMasterLevelPoint );
		}
	}
	else
	{ 
		pMsg.btResult = iResult;
	}

	DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );
}



#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
//////////////////////////////////////////////////////////////////////////
// A C T I V E    S K I L L   R U N N I N G
//////////////////////////////////////////////////////////////////////////
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
BOOL CMasterSkillSystem::RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x, BYTE y, BYTE aTargetPos )
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
BOOL CMasterSkillSystem::RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo )
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
{
	if( lpObj == NULL || lpMagic == NULL )
		return FALSE;

	// 기본 스킬을 찾는다. ( 다섯가지 모두 스킬을 선언하려니 많잖아. 레벨로 구분한다. )
	INT iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill );

	switch( iBaseMLS )
	{
	case AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE:		// 소울바리어 강화
		MLS_WizardMagicDefense( lpObj, aTargetIndex, lpMagic );	
		break;		

	case AT_SKILL_ML_STRENGTHEN_HEALING:			// 치료 강화
		MLS_Healing( lpObj, aTargetIndex, lpMagic );
		break;				

	case AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT:		// 회오리베기 강화
	case AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA:	
		MLS_Wheel( lpObj, lpMagic, aTargetIndex, bCombo );	
		break;	

	case AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE:		// 스웰라이프 강화	
		MLS_KnightSkillAddLife( lpObj, lpMagic );		
		break;	

	case AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK:	// 어스쉐이크 강화	
		MLS_DarkHorseAttack( lpObj, aTargetIndex, lpMagic );
		break;	

	case AT_SKILL_ML_STRENGTHEN_FIREBURST:			// 파이어버스트 강화
		MLS_FireBurst( lpObj, aTargetIndex, lpMagic );
		break;
		
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터레벨 4랭크 액티브스킬
	case AT_SKILL_ML_STRENGTHEN_EXPICE:				// 서든 아이스 강화
		MLS_SuddenIce(lpObj->m_Index, lpMagic, x, y, aTargetIndex); 
		break;
	case AT_SKILL_ML_STRENGTHEN_STRIKE:				// 블로우 강화(기사)
		MLS_KnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);
		break;
	case AT_SKILL_ML_STRENGTHEN_BLOWOFFURY:			// 분노의 일격 강화(기사)
		MLS_SkillBlowOfFury(lpObj->m_Index, lpMagic, x, y, aTargetIndex, bCombo);
		break;
	case AT_SKILL_ML_STRENGTHEN_DEFENSE:			// 방어력 향상 강화
		MLS_SkillDefense(lpObj->m_Index, aTargetIndex, lpMagic);
		break;
	case AT_SKILL_ML_STRENGTHEN_ATTACK:				// 공격력 향상 강화
		MLS_SkillAttack(lpObj->m_Index, aTargetIndex, lpMagic);
		break;
	case AT_SKILL_ML_STRENGTHEN_SWORD6:				// 파워슬래쉬 강화
		MLS_PowerSlash(lpObj->m_Index, lpMagic, x, y, aTargetPos, aTargetIndex);
		break;
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

	default:
		break;
	}

	return TRUE;
}

// 소울바리어 강화 (법사방어력증가)
VOID CMasterSkillSystem::MLS_WizardMagicDefense( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic )
{
	LPOBJECTSTRUCT lpTargetObj = &gObj[aTargetIndex];

	int skillSuccess = 1;
	
	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA ) return;

	if( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{	// 파티가 아니면 사용할수 없다
		return;
	}

	int iDefenseValue = 0;
	int iSkillValidTime = 0;

	// 1. 데미지가 감소되는 수치
	iDefenseValue = 10 + ( ( lpObj->Dexterity + lpObj->AddDexterity ) / g_GlobalConfig.m_iShieldDexterity ) + ( ( lpObj->Energy + lpObj->AddEnergy ) / g_GlobalConfig.m_iShieldEnergy );
	int iSkillValue = MagicDamageC.SkillGet( lpMagic->m_Skill );
	iDefenseValue += iSkillValue;

	if(iDefenseValue > g_GlobalConfig.m_iShieldMaxDefense) iDefenseValue = g_GlobalConfig.m_iShieldMaxDefense;

	// 2. 소모되는 마나 수치(1/1000)
	int iManaRate = g_SkillAdditionInfo.GetSoulBarrierManaRate_Level( lpMagic->m_Level );

	// 3. 적용시간
	iSkillValidTime = 60 + ( ( lpObj->Energy + lpObj->AddEnergy ) / g_GlobalConfig.m_iShieldTime );

	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, iDefenseValue, EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA, iManaRate, iSkillValidTime );
	GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpTargetObj->m_Index, skillSuccess );
}

// 치료강화 (요정)
VOID CMasterSkillSystem::MLS_Healing( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic )
{	
	LPOBJECTSTRUCT lpTargetObj = &gObj[aTargetIndex];

	int skillSuccess = 1;

	if( lpTargetObj->Type != OBJTYPE_CHARACTER ) 
		if( lpTargetObj->m_RecallMon == -1 ) return;
	
	if( lpObj->Class != CLASS_ELF ) return;
	
	// 카오스 캐슬에서는 요정에 힐을 자신에게만 해줄 수 있다.
	if( CHECK_CHAOSCASTLE( lpObj->MapNumber) ) 
	{
		if( lpObj->m_Index != lpTargetObj->m_Index )
			return;
	}

	float fAddLife = 5 + ( ( lpObj->Energy + lpObj->AddEnergy ) / 5 );
	INT iSkillValue = MagicDamageC.SkillGet( lpMagic->m_Skill );
	if( iSkillValue > 0 )
		fAddLife += (float)( fAddLife * iSkillValue / 100 );

	lpTargetObj->Life += fAddLife;
	
	if( lpTargetObj->Life > ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) ) 
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	}

	if( lpTargetObj->Type == OBJTYPE_CHARACTER ) 
	{
		GCReFillSend( lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield );

		GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpTargetObj->m_Index, skillSuccess );

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// 치료 마법 사용 시 Target을 보고 있는 몬스터의 Agro를 증가 시켜준다.
		for( int n = 0; n < MAXVIEWPORTOBJECT; n++ )
		{
			if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
			{
				if( lpObj->VpPlayer2[n].state )
				{
					LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

					if( lpMonster->m_iCurrentAI )
						lpMonster->m_Agro.IncAgro( lpObj->m_Index, lpObj->Energy/5/40 );
				}
			}
		}
#endif
	}
	else if( lpTargetObj->m_RecallMon >= 0 )
	{	// 소환 몬스터일 경우에는 자신의 HP를 알려준다
		GCRecallMonLife( lpTargetObj->m_RecallMon, (int)lpTargetObj->MaxLife, (int)lpTargetObj->Life );		
	}
}

// 회오리베기 강화(기사, 마검사)
VOID CMasterSkillSystem::MLS_Wheel( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, INT aTargetIndex, BOOL isCombo )
{	
	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( (( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER ) && ( gObj[tObjNum].m_RecallMon < 0 )) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// 회오리베기는 공성전 중에 사람에게도 적용된다.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
					EnableAttack = TRUE;
				}
#endif					
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck( lpObj, &gObj[CallMonIndex] ) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{	
					int iSkillDis = MagicDamageC.GetSkillDistance( lpMagic->m_Skill );
#ifdef MODIFY_ML_STRENGTHEN_WHEEL_RANGE_BUGFIX_20080630
					if( gObjCalDistance( lpObj, &gObj[tObjNum] ) <= iSkillDis )
#else
					if( gObjCalDistance( lpObj, &gObj[tObjNum] ) < iSkillDis )
#endif // MODIFY_ML_STRENGTHEN_WHEEL_RANGE_BUGFIX_20080630
					{	
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand() % 2 )
							{
								bAttack = TRUE;
							}							
						}
						else
						{
							bAttack = TRUE;
						}

						if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
						{
							continue;
						}
						
						if( bAttack )
						{
							gObjAttack( lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo );

							if( isCombo )
								GCMagicAttackNumberSend( lpObj, AT_SKILL_COMBO, gObj[tObjNum].m_Index, 1 );
						}
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
}

// 스웰라이프 강화(기사 생명력 증가)
VOID CMasterSkillSystem::MLS_KnightSkillAddLife( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic )
{
	int skillSuccess = 1;

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[14] != AUTH_CHECK_14 )
		DestroyGIocp();
#endif

	if( lpObj->Type != OBJTYPE_CHARACTER ) 
	{
		if( lpObj->m_RecallMon == -1 ) 
			return;
	}

	if( lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA ) 
		return;

	int number;
	int partynum=0;
	int totallevel=0;
	int partycount;
	int dis;
	int ApplyPartyIndex[MAX_PARTYUSER];	

	int viewplayer=0;
	int viewpercent = 0;
	
	LPOBJECTSTRUCT lpPartyObj;

	memset( ApplyPartyIndex, -1, sizeof(int) * MAX_PARTYUSER );
	partynum = lpObj->PartyNumber;	

	if( partynum != -1 )
	{
		partycount = gParty.m_PartyS[partynum].Count;
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];			
				// 같은 맵에 존재하거나..
				if( lpObj->MapNumber == lpPartyObj->MapNumber ) 
				{
					dis = gObjCalDistance( lpObj, &gObj[number] );
					if( dis < 10 )
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}	

	if( viewplayer > 1 )
	{		
		if		( viewplayer == 2 )	viewpercent = 1;
		else if	( viewplayer == 3 )	viewpercent = 2;
		else if	( viewplayer == 4 )	viewpercent = 3;		
		else						viewpercent = 4;
	}	

	int iSkillValue = MagicDamageC.SkillGet( lpMagic->m_Skill );
	int iaddLifepower = 12 + ( lpObj->Vitality + lpObj->AddVitality ) / g_GlobalConfig.m_iInnerVitality + 
							   ( lpObj->Energy + lpObj->AddEnergy ) / g_GlobalConfig.m_iInnerEnergy + viewpercent + iSkillValue;

	if(iaddLifepower > g_GlobalConfig.m_iInnerMaxHP) iaddLifepower = g_GlobalConfig.m_iInnerMaxHP;

	int iLifeTime  = 60 + ( lpObj->Energy + lpObj->AddEnergy ) / g_GlobalConfig.m_iInnerTime;
	int iaddlife;

	if( partynum == -1 )
	{	
		// 파티가 안되어 있는 상태이다.
		iaddlife = (int)lpObj->MaxLife * iaddLifepower / 100;
		gObjAddBuffEffect( lpObj, BUFFTYPE_HP_INC, EFFECTTYPE_HP, iaddlife, 0, 0, iLifeTime );

		GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpObj->m_Index, skillSuccess );		
	}
	else
	{	// 파티 상태
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			if( ApplyPartyIndex[n] != -1 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[ApplyPartyIndex[n]];

				iaddlife = (int)lpPartyObj->MaxLife * iaddLifepower / 100;
				gObjAddBuffEffect( lpPartyObj, BUFFTYPE_HP_INC, EFFECTTYPE_HP, iaddlife, 0, 0, iLifeTime );
				
				GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpPartyObj->m_Index, skillSuccess );
			}
		}
	}
}

// 어스쉐이크 강화
BOOL CMasterSkillSystem::MLS_DarkHorseAttack( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic )
{	
	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;


#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpObj->m_Index, 1 );

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110) 
				{	// 함정, 성문, 석상이 아니면.
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
						EnableAttack = TRUE;
					}
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
							 || DuelIndex == tObjNum
#endif
							 )
					{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
						EnableAttack = TRUE;
					}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// 어스쉐이크는 공성전 중에 사람에게도 적용된다.
					else if (
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{
						EnableAttack = TRUE;
					}
#endif					
					else
					{	// 길드전 체크
						int CallMonIndex = gObj[tObjNum].m_Index;
						if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
						{
							if( gObj[tObjNum].m_RecallMon >= 0 ) 
							{
								CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
							}
						}
						if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
						{	// 대상이 길전상대이면 공격가능
							EnableAttack = TRUE;
						}
					}
				}

				if( EnableAttack )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{	
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  500, lpMagic->m_Skill);							
							
						}						
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
	return TRUE;
}

// 파이어버스트 강화
BOOL CMasterSkillSystem::MLS_FireBurst( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic )
{	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
	
	int StartDis = 1;
	int tObjNum;

	//float fangle = (float)GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);
	//SkillFrustrum(aTargetIndex, fangle, 3.f, 4.f);

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	GCMagicAttackNumberSend( lpObj, lpMagic->m_Skill, lpTargetObj->m_Index, 1 );
	gObjAttack( lpObj, lpTargetObj, lpMagic, 0, 1, 0 );	

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	int		delaytime;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 && lpTargetObj->m_Index != tObjNum )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}	
				else if( tObjNum == lpTargetObj->m_Index 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}	
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{	
					attackcheck = FALSE;

					if( loopcount == 0 )
					{
						attackcheck = TRUE;
					}
					else
					{
						if( !(rand()%3) )
						{
							attackcheck = TRUE;
						}
					}
					
					if( attackcheck )
					{
						if( gObjCalDistance( lpTargetObj, &gObj[tObjNum]) < 3 )
						//if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aTargetIndex) )
						{	
							delaytime = 500+(rand()*17)%300;								

							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  delaytime, lpMagic->m_Skill);
							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0);
						}
					}
				}
			}
		}	
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{	
			break;
		}
	}
	return TRUE;
}
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// 마스터 레벨 4랭크 추가
// 범위 스킬 사용
BOOL CMasterSkillSystem::MLS_SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP, int nSuccessRate, int nDelay)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int count = 0;
	int tObjNum;
	BOOL attackcheck;
#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

#ifdef MODIFY_SEASON4_NEWSKILL_20080909
	int	nHitCount = 0;
	BOOL bAttack = FALSE;
#endif	// MODIFY_SEASON4_NEWSKILL_20080909
	
	// aIndex Connected 체크
	if(x == 0 && y == 0 )
	{
		x = lpObj->X;
		y = lpObj->Y;
	}

	// 스킬 사용 가능범위체크
	int iSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
	if( MLS_CalDistance(lpObj->X, lpObj->Y, x, y) <= iSkillDistance )
	{
		BOOL	EnableAttack;
		while(1)
		{
			if( lpObj->VpPlayer2[count].state )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				
				if( tObjNum >= 0 )
				{
					EnableAttack = FALSE;
					
					// 대상이 나를 보고 있는 유저(VpPlayer2[])인지 확인한다.
					
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
						EnableAttack = TRUE;
					}	
					else if( (tObjNum == aTargetIndex && bPVP == TRUE)	// bPVP 가 FALSE 일 경우는 결투에서도 사용못함
#ifdef DUEL_SYSTEM_20031028
						|| DuelIndex == tObjNum
#endif
						)
					{
						EnableAttack = TRUE;
					}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
					else if (// 공성전 중에 사람에게도 적용된다.
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{

						EnableAttack = TRUE;
					}
#endif
					else
					{	// 길드전 체크
						int CallMonIndex = gObj[tObjNum].m_Index;
						if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
						{
							if( gObj[tObjNum].m_RecallMon >= 0 ) 
							{
								CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
							}
						}
						if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
						{	// 대상이 길전상대이면 공격가능
							EnableAttack = TRUE;
						}
					}
					
					if( EnableAttack)
					{
						attackcheck = FALSE;

						if (nSuccessRate == 0)
						{
							attackcheck = TRUE;
						}
						else if( !(rand()% nSuccessRate) )
						{
							attackcheck = TRUE;
						}					

						if(attackcheck)
						{
							// x, y에서 aDistance셀 반경
							if( gObj[tObjNum].X >= x-aDistance && gObj[tObjNum].X  <= x+aDistance
 								&& gObj[tObjNum].Y >= y-aDistance && gObj[tObjNum].Y <= y+aDistance )
							{
								gObj[tObjNum].lpAttackObj = lpObj;

#ifdef MODIFY_SEASON4_NEWSKILL_20080909
							nHitCount++;
							if(nHitCount > 5)
							{
								if( rand()%2 )
								{
									bAttack = TRUE;
								}							
							}
							else 
							{
								bAttack = TRUE;
							}

							if(nHitCount > 8)
							{
								bAttack = FALSE;
							}

							if(bAttack == TRUE)
							{
								if (nDelay == 0 )
								{
									// 지연 없는 공격
									gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);
								}
								else
								{
									// 지연 이 있을 경우
									gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, nDelay, lpMagic->m_Skill);
								}
							}
#else	// MODIFY_SEASON4_NEWSKILL_20080909
								if (nDelay == 0 )
								{
									// 지연 없는 공격
									gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);
								}
								else
								{
									// 지연 이 있을 경우
									gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, nDelay, lpMagic->m_Skill);
								}
#endif	// MODIFY_SEASON4_NEWSKILL_20080909
								
								bRet = TRUE;
							}
						}
					}
				}
			}		
			count++;
			if( count > MAXVIEWPORTOBJECT-1 )
			{
				break;
			}
		}
	}

	return bRet;
}

// 서든 아이스 강화
BOOL CMasterSkillSystem::MLS_SuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
#ifdef MODIFY_SEASON4_NEWSKILL_20080909
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int FirstHit = 0;
	int	HitCount = 0;
	BOOL bAttack;

	LPOBJECTSTRUCT lpTargetObj =  &gObj[aTargetIndex];

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// 서든아이스는 공성전 중에 사람에게도 적용된다.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
					EnableAttack = TRUE;
				}
#endif					
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{	
					if( MLS_CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{	
						bAttack = FALSE;
						
						if( HitCount >= 5 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( HitCount > 10 )
						{
							bAttack = FALSE;
						}

						if( bAttack )
						{
							int delay = rand()%500;

							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);						
						}						
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}

	return TRUE;
#else // MODIFY_SEASON4_NEWSKILL_20080909
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;

	// 범위 스킬 사용
	bRet = MLS_SkillAreaMonsterAttack(aIndex, lpMagic, x, y, aTargetIndex, 4, TRUE, 2);

	return bRet;
#endif	// MODIFY_SEASON4_NEWSKILL_20080909
}

#include "zzzmathlib.h"
#include "Math.h"

int CMasterSkillSystem::MLS_CalDistance(int x, int y, int x1, int y1)
{
	if( (x == x1) && 
		(y == y1) )
	{
		return 0;
	}
	float tx = (float)(x-x1);
	float ty = (float)(y-y1);
			
	return (int)(sqrt((tx*tx)+(ty*ty)));
}

int CMasterSkillSystem::MLS_GetAngle(int x, int y, int tx, int ty)
{
	double	diffX = x-tx;
	double	diffY = y-ty;
	double rad = atan2(diffY, diffX);
	int angle = (int)(rad*180/Q_PI+90);	
	if( angle < 0 )	angle = 360+angle;	
	
	return angle;
}

void CMasterSkillSystem::MLS_SkillFrustrum(int aIndex, float fangle, float tx,float ty)	// 캐릭터..
{	
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];
	Vector(-tx ,ty ,0.f,p[0]);
	Vector( tx ,ty ,0.f,p[1]);
	Vector( 1 ,0.f ,0.f,p[2]);
	Vector(-1 ,0.f ,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,fangle,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t vFrustrum[4];
	
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);
		lpObj->fSkillFrustrumX[i] = (int)(vFrustrum[i][0])+lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)(vFrustrum[i][1])+lpObj->Y;
	}
}

// 블로우 강화
BOOL CMasterSkillSystem::MLS_KnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo)
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;
	int tObjNum;

	float fangle = (float)MLS_GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);
	
#ifdef MODIFY_MASTER_SKILL_DISTANCE_BUGFIX_20080909
	float fSkillDis = (float)MagicDamageC.GetSkillDistance( lpMagic->m_Skill ) + 1.f;
	MLS_SkillFrustrum(aIndex, fangle, 1.5f, fSkillDis);
#else	// MODIFY_MASTER_SKILL_DISTANCE_BUGFIX_20080909
	MLS_SkillFrustrum(aIndex, fangle, 1.5f, 3.f);
#endif	// MODIFY_MASTER_SKILL_DISTANCE_BUGFIX_20080909

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}	
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{	
					attackcheck = FALSE;

					if( loopcount == 0 )
					{
						attackcheck = TRUE;
					}
					else
					{
						if( !(rand()%3) )
						{
							attackcheck = TRUE;
						}
					}

					if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
					{
						continue;
					}
					
					if( attackcheck )
					{
						if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{	
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo);

							if(isCombo)
								GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, gObj[tObjNum].m_Index, 1);						
						}
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			loopcount++;
			count = 0;
			if( loopcount >= 3 )			
				break;
		}
	}
	return TRUE;
}

// 분노의 일격 강화
void CMasterSkillSystem::MLS_SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	int	HitCount = 0;
	BOOL bAttack = FALSE;

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// 분노의일격은 공성전 중에 사람에게도 적용된다.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
					EnableAttack = TRUE;
				}
#endif					
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					int iSkillDis = MagicDamageC.GetSkillDistance( lpMagic->m_Skill );
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) <= iSkillDis )		// 분노의 일격 강화 스크립 데이타 사용(grooving)
					{
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
						{
							continue;
						}

						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  500, lpMagic->m_Skill, isCombo);							

							if(isCombo)
								GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aIndex, 1);
						}
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
}

//방어력 증가 강화 (약30초간 (아아템레벨+1)*3만큼 증가)
void CMasterSkillSystem::MLS_SkillDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	int skill_level = lpMagic->m_Level;

	float skill_defense;

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_ELF ) return;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬에서는 요정에 방어력향상을 자신에게만 해줄 수 있다.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if( skill_level < 0 || skill_level > 7 ) skill_level = 0;

	float aMLSkillValue = (float)MagicDamageC.SkillGet( lpMagic->m_Skill ) / 10.f;

	skill_defense = (float)((skill_level+1)*2.0f+((float)(lpObj->Energy+lpObj->AddEnergy)/(float)aMLSkillValue));
	skill_defense *= g_GlobalConfig.m_fPlusDef;

	// 기사의 경우 -10%, +10% 의 영향을 준다.
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		skill_defense += (skill_defense/10);
	}
	//LogAdd("방어력증가 마법 사용 2 : %f", skill_defense);
	
	// 이미 적용된 수치보다 낮으면 적용/갱신하지 않는다.

	if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, skill_defense, 0 ) == true )
	{
		GCMagicAttackNumberSend( lpObj, AT_SKILL_ML_STRENGTHEN_DEFENSE, lpTargetObj->m_Index, 0 );	// 스킬사용 실패		
		return;
	}

	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, EFFECTTYPE_IMPROVE_DEFENSE, skill_defense, 0, 0, 60 );

	// 방어력 증가 사용 시 Target을 보고 있는 몬스터의 Agro를 증가 시켜준다.
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if( lpMonster->m_iCurrentAI )
					lpMonster->m_Agro.IncAgro(lpObj->m_Index, (int)skill_defense / 10 );
			}
		}
	}

	GCMagicAttackNumberSend(lpObj, AT_SKILL_DEFENSE, lpTargetObj->m_Index, skillSuccess);
}

//공격력 증가 강화 (약30초간 (아아템레벨+1)*3만큼 증가)
void CMasterSkillSystem::MLS_SkillAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
	
	int skill_level = lpMagic->m_Level;

	float skill_attack;

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_ELF ) return;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬에서는 요정에 공격력 향상을 자신에게만 해줄 수 있다.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if( skill_level < 0 || skill_level > 7 ) skill_level = 0;

	float aMLSkillValue = (float)MagicDamageC.SkillGet( lpMagic->m_Skill ) / 10.f;

	skill_attack = (float)((skill_level+1)*3.0f+((float)(lpObj->Energy+lpObj->AddEnergy)/(float)aMLSkillValue))*g_GlobalConfig.m_fPlusDmg;

	//LogAdd("공격력증가 마법 사용 1 : %f", skill_attack);
	// 기사의 경우 -10%, +10% 의 영향을 준다.
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		skill_attack += (skill_attack/10);
	}
	//LogAdd("공격력증가 마법 사용 2 : %f", skill_attack);

	if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, skill_attack, 0 ) == true )
	{
		GCMagicAttackNumberSend( lpObj, AT_SKILL_ML_STRENGTHEN_ATTACK, lpTargetObj->m_Index, 0 );	// 스킬사용 실패
		return;
	}

	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, skill_attack, 0, 0, 60 );
	
	// 공격력 증가 사용 시 Target을 보고 있는 몬스터의 Agro를 증가 시켜준다.
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if( lpMonster->m_iCurrentAI )
					lpMonster->m_Agro.IncAgro(lpObj->m_Index, (int)skill_attack / 10);
			}
		}
	}

	GCMagicAttackNumberSend(lpObj, AT_SKILL_ATTACK, lpTargetObj->m_Index, skillSuccess);
}

void CMasterSkillSystem::MLS_MaGumSkillDefenseDown(int aIndex, int aTargetIndex)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	if( lpObj->Type == OBJTYPE_CHARACTER) {
		// 사용자 중에서는 마검사만 쓸 수 있다.
		if( lpObj->Class != CLASS_MAGUMSA ) return;
	}

	LogAdd("마검사 몬스터방어력감소 : %d", 10);

	if( retCalcSkillResistance(lpTargetObj, TRUE)  )
	{
		gObjAddBuffEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_DEC, EFFECTTYPE_DECREASE_DEFENSE, 10, 0, 0, 10 );

		GCMagicAttackNumberSend(lpObj, AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN, lpTargetObj->m_Index, skillSuccess);
	}
}

// 파워 슬래쉬 강화
void CMasterSkillSystem::MLS_PowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE Targetangle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;	

	// 파워 슬래쉬 사거리 수정grooving
#ifdef MLS_POWERSLASH_DISTANCE_20090216
	int iSkillDis = MagicDamageC.GetSkillDistance( lpMagic->m_Skill );
	
	float fangle = (float)(Targetangle*360/255);
	
	MLS_SkillFrustrum(aIndex, fangle, (float)iSkillDis + 1, (float)iSkillDis + 1);
#else
	::SkillFrustrum(Targetangle, aIndex);
#endif //	MLS_POWERSLASH_DISTANCE_20090216

	int tObjNum;
	int count = 0;

	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// 대상이 몬스터이고, 소환몹이 아니면 공격가능
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// 클라이언트에서 대상을 강제 공격했을때와 결투 대상일때는 공격가능
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// 데쓰포이즌은 공성전 중에 사람에게도 적용된다.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
					EnableAttack = TRUE;
				}
#endif
				else
				{	// 길드전 체크
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// 대상이 길전상대이면 공격가능
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{
					
#ifndef MLS_POWERSLASH_DISTANCE_20090216
					int iSkillDis = MagicDamageC.GetSkillDistance( lpMagic->m_Skill );
#endif // MLS_POWERSLASH_DISTANCE_20090216

					if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) && 
						(gObjCalDistance(lpObj, &gObj[tObjNum]) <= iSkillDis) // 파워 슬래쉬 강화 스크립 데이타 사용(grooving)
						)
					{
						bAttack = FALSE;
						if( HitCount > 10 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 5 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( bAttack )
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);					

						HitCount++;
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
}

// 파이어 스크림 강화
BOOL CMasterSkillSystem::MLS_FireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic )
{
	if( g_SkillAdditionInfo.GetFireScreamSkill() == FALSE )
	{
		// 파이어 스크림을 처리하지 않는다.
		return FALSE;
	}

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	if( !gObjIsConnected( lpObj->m_Index ) && !gObjIsConnected( lpTargetObj->m_Index ) )
		return FALSE;
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) 
		return FALSE;

	if( lpObj->Class != CLASS_DARKLORD )
		return FALSE;

	// 공격
	gObjAttack( lpObj, lpTargetObj, lpMagic, 0, 0 );
	return TRUE;
}

#endif // UPDATE_MASTER_LEVEL_4RANK_20080428

#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
// 강화된 무기스킬 제거(실질적으로는 lpObj->Magic[n]에서 지우지 않음)
BOOL CMasterSkillSystem::MLS_WeaponSkillDel(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
{
	INT iBaseMLS = 0;

	for( INT iCount = 0; iCount < MAX_MAGIC; iCount++ )
	{
		if( lpObj->Magic[iCount].IsMagic() == TRUE )
		{
			// 마스터레벨 스킬일 때
			if( CheckMasterLevelSkill( lpObj->Magic[iCount].m_Skill ) )
			{
				// 기본 마스터레벨스킬 선행스킬이 궁금해하는 일반 스킬번호와 일치하면 오케이.
				iBaseMLS = GetBaseMasterLevelSkill( lpObj->Magic[iCount].m_Skill );
				if( iBaseMLS > 0 )
				{
					int BrandSkill = MagicDamageC.GetBrandOfSkill( iBaseMLS );
					if(aSkill == BrandSkill)
					{
						switch(BrandSkill)
						{
						case AT_SKILL_SWORD6:		// 파워슬래쉬
						case AT_SKILL_CROSSBOW:		// 다발
							{
								GCMagicListOneDelSend(lpObj->m_Index, iCount, lpObj->Magic[iCount].m_Skill, (BYTE)lpObj->Magic[iCount].m_Level, 0 );
								return TRUE;
							}
							break;
						}
					}
				}
			}
		}
	}	
	
	return FALSE;
}

// 강화된 무기스킬 추가(실질적으로는 lpObj->Magic[n]에서 추가하지 않음)
BOOL CMasterSkillSystem::MLS_WeaponSkillAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level)
{
	INT iBaseMLS = 0;

	for( INT iCount = 0; iCount < MAX_MAGIC; iCount++ )
	{
		if( lpObj->Magic[iCount].IsMagic() == TRUE )
		{
			// 마스터레벨 스킬일 때
			if( CheckMasterLevelSkill( lpObj->Magic[iCount].m_Skill ) )
			{
				// 기본 마스터레벨스킬 선행스킬이 궁금해하는 일반 스킬번호와 일치하면 오케이.
				iBaseMLS = GetBaseMasterLevelSkill( lpObj->Magic[iCount].m_Skill );
				if( iBaseMLS > 0 )
				{
					if( aSkill == MagicDamageC.GetBrandOfSkill( iBaseMLS ) )
					{
						GCMagicListOneSend(lpObj->m_Index, iCount, lpObj->Magic[iCount].m_Skill, (BYTE)lpObj->Magic[iCount].m_Level, 0 );
						return TRUE;					
					}
				}
			}
		}
	}	

	return FALSE;
}

// 사용가능한 무기스킬인지 확인
BOOL CMasterSkillSystem::CheckUsableWeaponSkill(LPOBJECTSTRUCT lpObj, int nCount)
{
	INT iBaseMLS = 0;
	if( CheckMasterLevelSkill( lpObj->Magic[nCount].m_Skill ) )
	{
		// 기본 마스터레벨스킬 선행스킬이 궁금해하는 일반 스킬번호와 일치하면 오케이.
		iBaseMLS = GetBaseMasterLevelSkill( lpObj->Magic[nCount].m_Skill );
		if( iBaseMLS > 0 )
		{
			if( MagicDamageC.GetSkillUseType(iBaseMLS) == 4 )
			{
				//  무기에 붙어있는 스킬을 여기에 추가한다
				int BrandSkill = MagicDamageC.GetBrandOfSkill( iBaseMLS );
				switch(BrandSkill)
				{
				case AT_SKILL_SWORD6:		// 파워슬래쉬
				case AT_SKILL_CROSSBOW:		// 다발
					{
						BOOL bUseSkill = FALSE;
						if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() == TRUE)
						{
							if(lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Special[0] == BrandSkill)
							{
								bUseSkill = TRUE;
							}
						}
						
						if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].IsItem() == TRUE)
						{
							if(lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Special[0] == BrandSkill)
							{
								bUseSkill = TRUE;
							}
						}
						
						if(bUseSkill == FALSE)
						{
							return FALSE;
						}
					}
					break;
				}
			}
		}
	}

	return TRUE;
}
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807

#endif	// MASTER_LEVEL_SKILL_SYSTEM_20071122
