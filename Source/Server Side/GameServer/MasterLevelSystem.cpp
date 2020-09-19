// MasterLevelSystem.cpp: implementation of the CMasterLevelSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterLevelSystem.h"

#include "..\common\winutil.h"
#include "DirPath.h"
#include "Gamemain.h"
#include "..\\Common\\classdef.h"
#include "ObjCalCharacter.h"
#include "wsJoinServerCli.h"
#include "DBSockMng.h"
#include "DSProtocol.h"

#ifdef ADD_SHIELD_POINT_01_20060403
extern void gObjCalcShieldPoint(LPOBJECTSTRUCT lpObj);
#endif


#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
extern CDirPath			gDirPath;
extern classdef			DCInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterLevelSystem g_MasterLevelSystem;

CMasterLevelSystem::CMasterLevelSystem()
{
	
}

CMasterLevelSystem::~CMasterLevelSystem()
{

}



//////////////////////////////////////////////////////////////////////////
// 마스터레벨 기본 처리
//////////////////////////////////////////////////////////////////////////
VOID CMasterLevelSystem::LoadData()
{
	m_iMinMonsterKillLevel = GetPrivateProfileInt( "GameServerInfo", "ML_MinMonsterKillLevel", 110, gDirPath.GetNewPath( "commonserver.cfg" ) ); 	
	
#ifdef MODIFY_MASTER_LEVEL_ADD_EXPERIENCE_BUGFIX_20090304
	char szTemp[10] = { 0, };
	GetPrivateProfileString("GameServerInfo", "ML_AddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	m_fAddExpRate = (float)atof(szTemp);
#else // MODIFY_MASTER_LEVEL_ADD_EXPERIENCE_BUGFIX_20090304
	m_fAddExpRate			= GetPrivateProfileInt( "GameServerInfo", "ML_AddExperience", 1, gDirPath.GetNewPath( "commonserver.cfg" ) ); 	
#endif // MODIFY_MASTER_LEVEL_ADD_EXPERIENCE_BUGFIX_20090304

	LogAddTD( "MasterLevel Info Set MinMonsterKillLevel:%d, AddExpRate:%d", m_iMinMonsterKillLevel, m_fAddExpRate );

}

// 경험치 테이블 설정
VOID CMasterLevelSystem::SetMasterLevelExpTlb()
{
	m_i64MasterLevelExpTlb[0] = 0;

	INT64	nTotalLevel			= 0;
	INT64	nTotalLevel_Over	= 0;
	INT64	i64Exp				= 0;

	for( int iLevel = 1; iLevel < MAX_MASTER_LEVEL + 1; iLevel++ )
	{
		nTotalLevel = g_GlobalConfig.m_iMaxLevel + iLevel;
		i64Exp = ( 9 + nTotalLevel ) * nTotalLevel * nTotalLevel * 10;

		nTotalLevel_Over = nTotalLevel - 255;
		i64Exp += ( 9 + nTotalLevel_Over ) * nTotalLevel_Over * nTotalLevel_Over * 1000;
		i64Exp = ( i64Exp - 3892250000 ) / 2;
		
		m_i64MasterLevelExpTlb[iLevel] = i64Exp;		
	}

	LogAddTD( "Master level system exp table setting is complete" );
}


// 다음 레벨이 되기위한 최대 경험치
VOID CMasterLevelSystem::gObjNextMLExpCal( LPOBJECTSTRUCT lpObj )
{
	int nNextML = lpObj->m_nMasterLevel + 1;
	lpObj->m_i64NextMasterLevelExp = m_i64MasterLevelExpTlb[nNextML];
}


// 마스터레벨업 처리 (여기서 경험치 추가)
BOOL CMasterLevelSystem::MasterLevelUp( LPOBJECTSTRUCT lpObj, INT64 iAddExp, bool bEventMapReward )
{
	// 3차 전직을 했나?
	if( !IsMasterLevelUser( lpObj ) )
		return FALSE;

	// 최대 마스터레벨인가?
	if( lpObj->m_nMasterLevel >= MAX_MASTER_LEVEL )
	{
		GCServerMsgStringSend( lMsg.Get( 1136 ), lpObj->m_Index, 1 );
		return FALSE;
	}

	// 추가 경험치를 계산 해 준다.
	if(bEventMapReward == true)
	{	
		// 경험치 이벤트 일때 이벤트 맵에서 보상이면 일반 계산
		iAddExp = ( INT64 )( ( float )iAddExp );
	}
	else
	{
		// 경험치 이벤트 일때 일반 몹 잡을때 계산
		iAddExp = ( INT64 )( ( float )iAddExp * m_fAddExpRate );
	}
	
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107		// 패시브 2차 - 경험치 상승
	if( lpObj->m_MPSkillOpt.iMpsPlusExp > 0 )
	{
		iAddExp += (INT64)( iAddExp * lpObj->m_MPSkillOpt.iMpsPlusExp / 100 );
	}
#endif

	// 마스터레벨 업, 해 보자고...
	if( iAddExp > 0 )
	{
#ifdef DARKLORD_WORK	// 펫 아이템 경험치 분배	
		gObjSetExpPetItem( lpObj->m_Index, iAddExp );
#endif
	
		LogAddTD( "ML Experience : Map[%d]-(%d,%d) [%s][%s](%d) %I64d %I64d", 
			lpObj->MapNumber, lpObj->X, lpObj->Y,
			lpObj->AccountID, lpObj->Name, 
			lpObj->m_nMasterLevel, lpObj->m_i64MasterLevelExp, iAddExp );

		if( lpObj->m_i64MasterLevelExp + iAddExp < lpObj->m_i64NextMasterLevelExp ) 
		{
			lpObj->m_i64MasterLevelExp += iAddExp;
		}	
		else
		{	
#ifdef BUGFIX_GAIN_EXP_20071210
			// 레벨업 했다면 추가경험치는 0
			iAddExp = 0;
#endif
			lpObj->m_i64MasterLevelExp	= lpObj->m_i64NextMasterLevelExp;
			lpObj->m_nMasterLevel		+= 1;						// 레벨 올려주고
			lpObj->m_iMasterLevelPoint	+= ML_DEFAULT_ADD_POINT;	// 마스터레벨 포인트도 올려주고

			// 기본 정보 업데이트
			gObjCalCharacter( lpObj->m_Index );

			lpObj->MaxLife	+= DCInfo.DefClass[lpObj->Class].LevelLife;
			lpObj->MaxMana	+= DCInfo.DefClass[lpObj->Class].LevelMana;
			lpObj->Life		= lpObj->MaxLife + lpObj->AddLife;
			lpObj->Mana		= lpObj->MaxMana + lpObj->AddMana;	

			
			// SD도 계산해줘야함
#ifdef ADD_SHIELD_POINT_01_20060403
			gObjCalcShieldPoint( lpObj );
			lpObj->iShield	= lpObj->iMaxShield + lpObj->iAddShield;
#endif // ADD_SHIELD_POINT_01_20060403

			gObjNextMLExpCal( lpObj );

			gObjCalcMaxLifePower( lpObj->m_Index );		// 최대 생명력을 다시 계산한다
			gObjSetBP( lpObj->m_Index );				// 사실 마스터레벨이 오르는 것과는 상관없지만 나중을 위해 계산 해 준다.
			
			// 서버에 저장한다.
			GJSetCharacterInfo( lpObj, lpObj->m_Index );	

			// 클라이언트에 전달한다.
			GCMasterLevelUpInfo( lpObj );

			GCReFillSend( lpObj->m_Index, ( WORD )lpObj->Life, 0xFF, 0, ( WORD )lpObj->iShield );
			GCManaSend( lpObj->m_Index, ( short )lpObj->Mana, 0xFF, 0, lpObj->BP );
		}	

#ifdef BUGFIX_GAIN_EXP_20071210
		// 마스터레벨의 경험치는 여기서 보낸다. 밖으로 나가면 복잡하다~
		GCSendExp_INT64( lpObj->m_Index, 0xFFFF, iAddExp, 0, 0 );
#endif		
	}	

	return TRUE;
}

// 마스터레벨 유저인가?
BOOL CMasterLevelSystem::IsMasterLevelUser( LPOBJECTSTRUCT lpObj )
{
	if( lpObj == NULL )
		return FALSE;

	// 마스터레벨 데이터를 받았나
	if( lpObj->m_bMasterLevelDBLoad == FALSE )
		return FALSE;

	if( lpObj->Level >= g_GlobalConfig.m_iMaxLevel && lpObj->ThirdChangeUp == 1 )
		return TRUE;	

	return FALSE;
}

// 마스터레벨 유저가 경험치를얻을 수 있는 조건인지 체크
BOOL CMasterLevelSystem::CheckMLGetExp( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj )					
{
	// 마스터레벨 유저가 아니면 조건 확인 필요 없음.
	if( IsMasterLevelUser( lpObj ) )
	{
		if( lpTargetObj->Level < m_iMinMonsterKillLevel )
			return FALSE;
	}
	
	return TRUE;
}

// 해당 마스터레벨의 최대 경험치를 돌려준다.
INT64 CMasterLevelSystem::GetMasterLevelExpTlbInfo( int iMasterLevel )
{
	return m_i64MasterLevelExpTlb[iMasterLevel];
}

// 사망 시 경험치 손실 값
INT	CMasterLevelSystem::GetDieDecExpRate( LPOBJECTSTRUCT lpObj )
{
	if( !IsMasterLevelUser( lpObj ) )
		return -1;
	
	int iRetValue = 0;	// (1/1000)
	if( lpObj->m_PK_Level <= PK_LEVEL_DEFAULT ) iRetValue = ML_DIE_DECEXP_RATE_COMMON;
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+1 ) iRetValue = ML_DIE_DECEXP_RATE_PKLEVEL1;
	else if( lpObj->m_PK_Level == PK_LEVEL_DEFAULT+2 ) iRetValue = ML_DIE_DECEXP_RATE_PKLEVEL2;
	else if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+3 ) iRetValue = ML_DIE_DECEXP_RATE_PKLEVEL3;

	return iRetValue;
}

// 사망 시 젠 손실 값
INT	CMasterLevelSystem::GetDieDecZenRate( LPOBJECTSTRUCT lpObj )
{
	if( !IsMasterLevelUser( lpObj ) )
		return -1;	
	
	return ML_DIE_DECZEN_RATE;
}

//////////////////////////////////////////////////////////////////////////
// DS <-> GS 처리
//////////////////////////////////////////////////////////////////////////
// GD [0x30] 마스터레벨 정보 요청 (0xC1)
VOID CMasterLevelSystem::GDReqMasterLevelInfo( LPOBJECTSTRUCT lpObj )
{
#ifdef MODIFY_3RDCHANGEUP_MASTERLEVEL
	if ( !(lpObj->Level >= g_GlobalConfig.m_iMaxLevel && lpObj->ThirdChangeUp == 1) )
	{
		return;
	}
#endif // MODIFY_3RDCHANGEUP_MASTERLEVEL

	// 정보를 이미 불러왔다면 패스
	if( lpObj->m_bMasterLevelDBLoad == TRUE )
		return;
	
	MLP_REQ_MASTERLEVEL_INFO	pMsg;
	pMsg.h.set( ( LPBYTE )&pMsg, 0x30, sizeof( pMsg ) );	
	pMsg.iUserIndex = lpObj->m_Index;
	memcpy( pMsg.szCharName, lpObj->Name, MAX_IDSTRING );
	pMsg.szCharName[MAX_IDSTRING] = 0;
	
	cDBSMng.Send( ( char* )&pMsg, pMsg.h.size );

	LogAddTD( "[%s][%s] Request master level info to DB", lpObj->AccountID, lpObj->Name );
}


// DG [0x30] 마스터레벨 정보 요청 결과 (0xC1)
VOID CMasterLevelSystem::DGAnsMasterLevelInfo( LPBYTE aRecv )
{
	if( aRecv == NULL )
		return;

	LPMLP_ANS_MASTERLEVEL_INFO lpRecvMsg = ( LPMLP_ANS_MASTERLEVEL_INFO )aRecv;
	
	if( gObjIsConnectedGP( lpRecvMsg->iUserIndex ) == FALSE )
		return;		// 게임 실행중이 아닐때에는 패스.

	int iIndex = lpRecvMsg->iUserIndex;
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	// 정보를 이미 불러왔다면 패스
	if( lpObj->m_bMasterLevelDBLoad == TRUE )
		return;

	if( lpRecvMsg->btResult == 0x00 )
	{
		// 정보를 정상적으로 전달받지 못 함.
		LogAddTD( "[%s][%s] MasterLevel Info Load Fail",
					lpObj->AccountID, 
					lpObj->Name
					);
#ifdef MODIFY_MASTER_LEVEL_ATTACK20UP_BUGFIX_20080828
		//return;	
#else
		return;	
#endif // MODIFY_MASTER_LEVEL_ATTACK20UP_BUGFIX_20080828
	}			
	else if( lpRecvMsg->btResult == 0x01 )
	{
		// DB로부터 정보를 받았다.
		lpObj->m_bMasterLevelDBLoad = TRUE;

		// 정보를 처음 불러올 때
		if( lpRecvMsg->nMLevel == 0 && lpRecvMsg->i64NextMLExp == 0 )
		{			
			lpObj->m_nMasterLevel			= lpRecvMsg->nMLevel;				// DB Default Value
			lpObj->m_i64MasterLevelExp		= lpRecvMsg->i64MLExp;				// DB Default Value
			lpObj->m_i64NextMasterLevelExp	= m_i64MasterLevelExpTlb[1];		// Server Default Value
			lpObj->m_iMasterLevelPoint		= lpRecvMsg->nMLPoint;				// DB Default Value

			// 마스터레벨인 경우만 저장한다.
#ifdef MODIFY_3RDCHANGEUP_MASTERLEVEL
			if ( IsMasterLevelUser(lpObj) )
			{
#endif // MODIFY_3RDCHANGEUP_MASTERLEVEL
			LogAddTD( "[%s][%s] MasterLevel Info First Set [MLevel:%d][MLExp:%I64d][MLNextExp:%I64d][MLPoint:%d]",
					lpObj->AccountID, 
					lpObj->Name, 
					lpObj->m_nMasterLevel,
					lpObj->m_i64MasterLevelExp,
					lpObj->m_i64NextMasterLevelExp,
					lpObj->m_iMasterLevelPoint
					);

			// 정상값을 저장한다.
			GDReqMasterLevelInfoSave( lpObj );
#ifdef MODIFY_3RDCHANGEUP_MASTERLEVEL
			}
#endif // MODIFY_3RDCHANGEUP_MASTERLEVEL

		}
		else
		{
			lpObj->m_nMasterLevel			= lpRecvMsg->nMLevel;	
			lpObj->m_i64MasterLevelExp		= lpRecvMsg->i64MLExp;
			lpObj->m_i64NextMasterLevelExp	= lpRecvMsg->i64NextMLExp;
			lpObj->m_iMasterLevelPoint		= lpRecvMsg->nMLPoint;	
			

			LogAddTD( "[%s][%s] Recv MasterLevel Info [MLevel:%d][MLExp:%I64d][MLNextExp:%I64d][MLPoint:%d]",
					lpObj->AccountID, 
					lpObj->Name, 
					lpObj->m_nMasterLevel,
					lpObj->m_i64MasterLevelExp,
					lpObj->m_i64NextMasterLevelExp,
					lpObj->m_iMasterLevelPoint
					);
		}



		// 받은 정보로 기본정보 다시 설정
		// 1. MaxLife
		lpObj->MaxLife    = DCInfo.DefClass[lpObj->Class].Life+\
			((lpObj->Level+lpObj->m_nMasterLevel-1)*DCInfo.DefClass[lpObj->Class].LevelLife)+\
			((lpObj->Vitality-DCInfo.DefClass[lpObj->Class].Vitality)*DCInfo.DefClass[lpObj->Class].VitalityToLife);		

#ifdef MODIFY_ADDLIFE_BUGFIX_20080730
		if( lpObj->Life > lpObj->MaxLife + lpObj->AddLife) 
#else	// MODIFY_ADDLIFE_BUGFIX_20080730
		if( lpObj->Life > lpObj->MaxLife ) 
#endif	// MODIFY_ADDLIFE_BUGFIX_20080730
		{	// AddLife값이 있기 때문에 Life가 MaxLife보다 큰 상태로, 저장될 수 있다. 
			lpObj->Life = lpObj->MaxLife;
		}

		// 2. MaxMana
		lpObj->MaxMana    = DCInfo.DefClass[lpObj->Class].Mana+\
			((lpObj->Level+lpObj->m_nMasterLevel-1)*DCInfo.DefClass[lpObj->Class].LevelMana)+\
			((lpObj->Energy-DCInfo.DefClass[lpObj->Class].Energy)*DCInfo.DefClass[lpObj->Class].EnergyToMana);

#ifdef MODIFY_ADDLIFE_BUGFIX_20080730
		if( lpObj->Mana > lpObj->MaxMana + lpObj->AddMana) 
#else	// MODIFY_ADDLIFE_BUGFIX_20080730
		if( lpObj->Mana > lpObj->MaxMana ) 
#endif	// MODIFY_ADDLIFE_BUGFIX_20080730
		{	// AddMana값이 있기 때문에 Mana가 MaxMana 큰 상태로, 저장될 수 있다. 
			lpObj->Mana = lpObj->MaxMana;
		}		


		gObjCalcMaxLifePower( lpObj->m_Index );		// 최대 생명력을 다시 계산한다
		gObjSetBP( lpObj->m_Index );

#ifdef ADD_SHIELD_POINT_01_20060403
		gObjCalcShieldPoint( lpObj );
		lpObj->iShield	= lpObj->iMaxShield + lpObj->iAddShield;
#endif
		
		LogAddTD( "[%s][%s] Reset Max Value For MasterLevel [MaxLife:%d][MaxMana:%d][MaxSD:%d]",
					lpObj->AccountID, lpObj->Name, 
					lpObj->MaxLife, lpObj->MaxMana,
					lpObj->iShield
					);		
		

		// 클라이언트에 마스터레벨 정보를 준다.
		GCMasterLevelInfo( lpObj );

		// 재 설정된 기본정보도 준다.
		GCReFillSend( lpObj->m_Index, ( WORD )lpObj->Life, 0xFF, 0, ( WORD )lpObj->iShield );
		GCManaSend( lpObj->m_Index, ( short )lpObj->Mana, 0xFF, 0, lpObj->BP );
	}

#ifdef MODIFY_MASTER_LEVEL_ATTACK20UP_BUGFIX_20080828	// 마스터레벨 정보를 받은 후 다시 계산해준다
	gObjCalCharacter( lpObj->m_Index );
#endif // MODIFY_MASTER_LEVEL_ATTACK20UP_BUGFIX_20080828
}


// GD [0x31] 마스터레벨 정보 저장 (0xC1)
VOID CMasterLevelSystem::GDReqMasterLevelInfoSave( LPOBJECTSTRUCT lpObj )
{
#ifdef MODIFY_3RDCHANGEUP_MASTERLEVEL
	// 마스터레벨 유저만 저장한다.
	if ( !IsMasterLevelUser(lpObj) )
		return;
#endif // MODIFY_3RDCHANGEUP_MASTERLEVEL

	// DB에서 정보를 불러오지 못한 경우 패스.
	if( lpObj->m_bMasterLevelDBLoad == FALSE )
	{
		return;
	}

	MLP_REQ_MASTERLEVEL_INFOSAVE	pMsg;
	pMsg.h.set( ( LPBYTE )&pMsg, 0x31, sizeof( pMsg ) );	
	
	memcpy( pMsg.szCharName, lpObj->Name, MAX_IDSTRING );
	pMsg.szCharName[MAX_IDSTRING] = 0;

	pMsg.nMLevel		= lpObj->m_nMasterLevel;
	pMsg.i64MLExp		= lpObj->m_i64MasterLevelExp;
	pMsg.i64NextMLExp	= lpObj->m_i64NextMasterLevelExp;
	pMsg.nMLPoint		= lpObj->m_iMasterLevelPoint;
	
	cDBSMng.Send( ( char* )&pMsg, pMsg.h.size );

	LogAddTD( "[%s][%s] MasterLevel Info Save [MLevel:%d][MLExp:%I64d][MLNextExp:%I64d][MLPoint:%d]",
			lpObj->AccountID, 
			lpObj->Name, 
			lpObj->m_nMasterLevel,
			lpObj->m_i64MasterLevelExp,
			lpObj->m_i64NextMasterLevelExp,
			lpObj->m_iMasterLevelPoint
			);
}












//////////////////////////////////////////////////////////////////////////
// Client <-> GS 처리
//////////////////////////////////////////////////////////////////////////
// GC [0xF3][0x50] 마스터레벨 정보
VOID CMasterLevelSystem::GCMasterLevelInfo( LPOBJECTSTRUCT lpObj )			
{	
	// DB로부터 정보를 정상적으로 받지 못한 경우 재요청
	if( lpObj->m_bMasterLevelDBLoad == FALSE )
	{
		GDReqMasterLevelInfo( lpObj );
		return;
	}

	PMSG_MASTERLEVEL_INFO pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xF3, 0x50, sizeof( PMSG_MASTERLEVEL_INFO ) );

	pMsg.nMLevel			= lpObj->m_nMasterLevel;

	pMsg.btMLExp1			= HIBYTE(HIWORD(HILONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp2			= LOBYTE(HIWORD(HILONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp3			= HIBYTE(LOWORD(HILONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp4			= LOBYTE(LOWORD(HILONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp5			= HIBYTE(HIWORD(LOLONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp6			= LOBYTE(HIWORD(LOLONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp7			= HIBYTE(LOWORD(LOLONG( lpObj->m_i64MasterLevelExp )));
	pMsg.btMLExp8			= LOBYTE(LOWORD(LOLONG( lpObj->m_i64MasterLevelExp )));


	pMsg.btMLNextExp1		= HIBYTE(HIWORD(HILONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp2		= LOBYTE(HIWORD(HILONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp3		= HIBYTE(LOWORD(HILONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp4		= LOBYTE(LOWORD(HILONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp5		= HIBYTE(HIWORD(LOLONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp6		= LOBYTE(HIWORD(LOLONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp7		= HIBYTE(LOWORD(LOLONG( lpObj->m_i64NextMasterLevelExp )));
	pMsg.btMLNextExp8		= LOBYTE(LOWORD(LOLONG( lpObj->m_i64NextMasterLevelExp )));

	pMsg.nMLPoint			= lpObj->m_iMasterLevelPoint;

	pMsg.wMaxLife			= (WORD)( lpObj->MaxLife + lpObj->AddLife );
	pMsg.wMaxMana			= (WORD)( lpObj->MaxMana + lpObj->AddMana );
	pMsg.wMaxShield			= (WORD)( lpObj->iMaxShield + lpObj->iAddShield );
	pMsg.wMaxBP				= (WORD)( lpObj->MaxBP + lpObj->AddBP );	
	
	DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );
}


// GC [0xF3][0x51] 마스터레벨이 올랐다. 
VOID CMasterLevelSystem::GCMasterLevelUpInfo( LPOBJECTSTRUCT lpObj )
{
	PMSG_MASTERLEVEL_UP	pMsg;
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xF3, 0x51, sizeof( PMSG_MASTERLEVEL_INFO ) );

	pMsg.nMLevel			= lpObj->m_nMasterLevel;				// 현재 마스터레벨

	// 여기 작업 다시해야함
	pMsg.nAddMLPoint		= ML_DEFAULT_ADD_POINT;			// 추가된 마스터레벨 포인트
	pMsg.nMLPoint			= lpObj->m_iMasterLevelPoint;	// 사용 가능한 마스터레벨 포인트	
	pMsg.nMaxMLPoint		= MAX_MASTER_LEVEL;				// 최대 마스터레벨 포인트

	pMsg.wMaxLife			= (WORD)( lpObj->MaxLife + lpObj->AddLife );
	pMsg.wMaxMana			= (WORD)( lpObj->MaxMana + lpObj->AddMana );
	pMsg.wMaxShield			= (WORD)( lpObj->iMaxShield + lpObj->iAddShield );
	pMsg.wMaxBP				= (WORD)( lpObj->MaxBP + lpObj->AddBP );

	DataSend( lpObj->m_Index, ( LPBYTE )&pMsg, pMsg.h.size );
	GCSendEffectInfo( lpObj->m_Index, EFFECT_LEVELUP );

	LogAddTD( "[%s][%s] Master Level Up :%d, MLPoint:%d/%d", 
		lpObj->AccountID, 
		lpObj->Name, 
		lpObj->m_nMasterLevel,
		lpObj->m_iMasterLevelPoint, 
		MAX_MASTER_LEVEL );
}

#endif		// MASTER_LEVEL_UP_SYSTEM_20070912
	