// PCBangPointSystem.cpp: implementation of the CPCBangPointSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCBangPointSystem.h"

#include "..\Include\ReadScript.h"
#include "..\Include\public.h"
#include "..\\Common\\winutil.h"
#include "..\INCLUDE\cb_protocol.h"
#include "DSProtocol.h"
#include "Event.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#include "DirPath.h"
extern CDirPath		gDirPath;

#include "MapClass.h"
extern MapClass		MapC[];

#include "Shop.h"
extern CShop		ShopC[];

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef PCBANG_KUZIMO
#include "MDate.h"
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206

short g_nPCBangPointFirstRuleTime		= 60 * 10;	// ���� ��Ģ �ð� ����(60�� * 10 = 10��) 
short g_nPCBangPointFirstRuleTimePoint	= 1;		// ���� ��Ģ �ð��� �������� �� �ο� ����Ʈ
short g_nPCBangPointCommonRuleTime		= 60 * 10;	// �Ϲ� ��Ģ �ð� ����(60�� * 10 = 10��) 
short g_nPCBangPointCommonRuleTimePoint	= 1;		// �Ϲ� ��Ģ �ð��� �������� �� �ο� ����Ʈ
short g_nPCBangMaxPoint					= 1008;		// �ִ� ���� ������ ����Ʈ

// FeverTime �Ⱓ���� (��� �ο� ����Ʈ * g_nPCBangPointFeverMultipleValue)����Ʈ�� �����ȴ�.
// g_nPCBangPointFeverStartHour�� g_nPCBangPointFeverEndHour�� ������ �������� �ʴ´�.
// g_nPCBangPointFeverStartHour = 4, g_nPCBangPointFeverEndHour = 5��� 4�ú��� �۵��ϸ�, 5�ð� �Ǹ� �۵� �����ȴ�.
short g_nPCBangPointFeverStartHour		= 20;		// �ð��� 0�ú��� 24�ñ��� ���� �� �ִ�. 0��, 24�ô� �����ϴ�.
short g_nPCBangPointFeverEndHour		= 24;		//
short g_nPCBangPointFeverMultipleValue	= 2;

short g_nPCBangResetDayOfWeek			= 2;		// ���� ������ ���Ѵ�. 0(��), 1(��)... 
short g_nPCBangResetHour				= 9;		// ���� �ð�. ���� 8�ú��� 10�ñ��� �����̴�. ���ϸ� ���� ���ؼ� 9�÷� �����Ѵ�.

#ifdef ADD_PCBANG_EXPRATE
float g_fPCBangAddExperience			= 1.f;
#endif // ADD_PCBANG_EXPRATE

CPCBangPointSystem g_PCBangPointSystem;

BYTE g_btPCBangPointApplicationLevel;		// PC�� ����Ʈ �ý��� ���� ���

// ����Ʈ ����� �Ϲݼ���/�׽�Ʈ���� ������ ���´�.
enum PCBANG_POINT_ITEM_SERVER_TYPE
{
	ST_SERVICESERVER		= 0,
	ST_TESTSERVER			= 1,
#ifdef PCBANG_KUZIMO	// �̺κ��� ���ڸ� �ٲٸ� �ʵ˴ϴ�.  DB���� ���� �ĺ��� ����մϴ�.
	ST_TEMPSERVER			= 2,
#endif
};

enum PCBANG_POINT_CHANGE_POINT_TYPE
{
	CT_ADD_FOR_TIME			= 0,
	CT_MINUS_FOR_BUY_ITEM	= 1,
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCBangPointSystem::CPCBangPointSystem()
{
	m_bPCBangPointSystemOn				= FALSE;
	m_bPCBangPointRankingEvent			= FALSE;

	g_btPCBangPointApplicationLevel		= PP_PCBANG_USER_ONLY;

	m_btPCBangPointShopNum				= 14;

	InitItemOption();
}

CPCBangPointSystem::~CPCBangPointSystem()
{

}

//-------------------------------------------------------------------------
// �ʱ�ȭ
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::InitItemOption()
{
	for( int n = 0; n <= MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		memset( &m_ItemOption[n], 0, sizeof( PCBANG_POINT_ITEM_OPTION ) );
#ifdef MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
		m_ItemOption[n].dwItemType = -1;
#endif // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	}	
}

//-------------------------------------------------------------------------
// ������ �о����
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::LoadData( LPSTR lpszFileName )
{
	// 1. PCBangPointShop.txt ���� �б�
	m_bFileDataLoad = FALSE;
	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) )
	{
		MsgBox( "[PCBangPointSystem] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )
		{
			DWORD dwError = GetLastError();
			MsgBox( "[PCBangPointSystem] - Can't Open %s ", lpszFileName );
			return FALSE;
		}
		
		// ��� ������ �ʱ�ȭ.
		InitItemOption();

		int iItemLoadCount = 0;
		SMDToken Token;
		int iType = -1;

		WORD    wType				= 0;
		WORD	wIndex				= 0;		
		short	nBuyingPoint		= 0;	// ���� ����Ʈ
		short	nConditionPoint		= 0;	// ���� ����	
		BYTE	btAbility			= 0;	// �������� ��� 0:�Ϲݾ����� 1:������
		DWORD	dwKeepTime_Sec		= 0;	// ���ӽð�(�ʴ���)	
		short	nAddExpPercent		= 0;	// ����ġ ��·�
		short	nGetItemPercent		= 0;	// ������ ȹ�� Ȯ��
		BYTE	btNoMoreExp			= 0;	// ����ġ ���� ����

		while( TRUE )
		{
			Token = GetToken();
			if( Token == END ) break;
			iType = ( int )TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{					
					// 1. >	������ �ʱ�ȭ
					wType				= 0;
					wIndex				= 0;					
					nBuyingPoint		= 0;
					nConditionPoint		= 0;
					btAbility			= 0;
					dwKeepTime_Sec		= 0;	
					nAddExpPercent		= 0;
					nGetItemPercent		= 0;	
					btNoMoreExp			= 0;

					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;
					
					wType = ( WORD )TokenNumber;
					Token = GetToken(); wIndex				= ( WORD )TokenNumber;
					Token = GetToken(); nBuyingPoint		= ( short )TokenNumber;
					Token = GetToken(); nConditionPoint		= ( short )TokenNumber;
					Token = GetToken(); btAbility			= ( BYTE )TokenNumber;
					Token = GetToken();	dwKeepTime_Sec		= ( DWORD )TokenNumber;	
					Token = GetToken(); nAddExpPercent		= ( short )TokenNumber;
					Token = GetToken(); nGetItemPercent		= ( short )TokenNumber;	
					Token = GetToken(); btNoMoreExp			= ( BYTE )TokenNumber;
					
					
					// 3. >	������ ��ȿ�� �˻�
					if( iItemLoadCount < 0 || iItemLoadCount > MAX_PCBANG_POINT_ITEM_COUNT )
					{
						MsgBox( "[PCBangPointSystem] - Exceed Max Item Count (%d)", iItemLoadCount );
						break;
					}
					
					// 4. > ������ ����
					LPPCBANG_POINT_ITEM_OPTION pInfo = &m_ItemOption[iItemLoadCount];
					pInfo->dwItemType		= MAKE_ITEMNUM( wType, wIndex );
					pInfo->nBuyingPoint		= nBuyingPoint;
					pInfo->nConditionPoint	= nConditionPoint;
					pInfo->btAbility		= btAbility;
					pInfo->dwKeepTime_Sec	= dwKeepTime_Sec;
					pInfo->nAddExpPercent	= nAddExpPercent;
					pInfo->nGetItemPercent	= nGetItemPercent;
					pInfo->btNoMoreExp		= btNoMoreExp;

					++iItemLoadCount;
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
		
		LogAddC( 2, "[PCBangPointSystem] - %s file is Loaded", lpszFileName );		
	}
	catch(...) 
	{
		MsgBox( "[PCBangPointSystem] Loading Exception Error (%s) File. ", lpszFileName );
	}

	// 2. commonserver.cfg ���� �б�
	m_bPCBangPointSystemOn				= GetPrivateProfileInt( "GameServerInfo", "PCBangPointSystemOn", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_btPCBangPointApplicationLevel		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointSystemLevel", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	m_bPCBangPointRankingEvent			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointRankingEvent", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );


	//-- Start 20080922 by kuzimo
	g_nPCBangPointFirstRuleTime			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFirstRuleTime", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFirstRuleTimePoint	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFirstRuleTimePoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointCommonRuleTime		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointCommonRuleTime", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointCommonRuleTimePoint	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointCommonRuleTimePoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangMaxPoint					= GetPrivateProfileInt( "GameServerInfo", "PCBangMaxPoint", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	g_nPCBangPointFeverStartHour		= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverStartHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFeverEndHour			= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverEndHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangPointFeverMultipleValue	= GetPrivateProfileInt( "GameServerInfo", "PCBangPointFeverMultipleValue", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

	g_nPCBangResetDayOfWeek				= GetPrivateProfileInt( "GameServerInfo", "PCBangResetDayOfWeek", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );
	g_nPCBangResetHour					= GetPrivateProfileInt( "GameServerInfo", "PCBangResetHour", 1, gDirPath.GetNewPath( "commonserver.cfg" ) );

#ifdef ADD_PCBANG_EXPRATE
	{
		char szTemp[256];
		GetPrivateProfileString("GameServerInfo", "PCBangAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
		g_fPCBangAddExperience = (float)atof(szTemp);
	}
#endif // ADD_PCBANG_EXPRATE
	//-- End

	LogAddTD( "[PCBangPointSystem][SetPCBangPointInfo] SystemOn:%d, Level:%d, RankingEventOn:%d", 
			m_bPCBangPointSystemOn, g_btPCBangPointApplicationLevel, m_bPCBangPointRankingEvent );

	return m_bFileDataLoad;		
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ �ý��� �ش� �����ΰ�?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointSystemUser( int iIndex )
{
	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];
	if( lpObj->m_bPCBangPointUser == TRUE )
		return TRUE;

	return FALSE;
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ�� �������ΰ�?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointItem( DWORD dwItemNum )
{
	LPPCBANG_POINT_ITEM_OPTION p = &m_ItemOption[0];
	for( int n = 0; n < MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		if( m_ItemOption[n].dwItemType == dwItemNum )
			return TRUE;
	}
	
	return FALSE;
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ������ �� �� �ִ� ��ǥ�ΰ�?
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::CheckPCBangPointShopOpenAttr( BYTE btMapNum, short X, short Y )
{
	// �̺�Ʈ �ʿ����� ������ �� �� ����.	
	if( CHECK_BLOODCASTLE( btMapNum ) ||		// ����ĳ��
		CHECK_CHAOSCASTLE( btMapNum ) ||		// ī����ĳ��
#ifdef DEVILSQUARE_EXTEND_20050221
		CHECK_DEVILSQUARE( btMapNum ) ||		// ����������
#endif
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	
		CHECK_ILLUSION_TEMPLE( btMapNum ) ||	// ȯ�����
#endif
		MAP_INDEX_KANTURU_BOSS == btMapNum		// ĭ���� ������
		)
	{
		return FALSE;
	}

	// �������� ��ǥ Ȯ��
	BYTE attr= MapC[btMapNum].GetAttr( X, Y );

	if( ( attr & MAP_ATTR_SAFTYZONE ) == MAP_ATTR_SAFTYZONE )
			return TRUE;	
	
	return FALSE;
}

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//-------------------------------------------------------------------------
// PC�� ������ �� ����ġ�� ���õ� Ȯ��( ����ġ�� ������ �����ش�. )
//-------------------------------------------------------------------------
void CPCBangPointSystem::CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp )
{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// PC�� ���� ������ ó��
	// ������ ���� �������Դ� �ش���� �ʴ´�.
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) ) {
			// ������ ������ ���� ������ ����ϸ� ����ġ ���� ���ش�.
		} else {
			return; 	
		}		
	}
#else
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		return; 	
#endif //ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#endif

#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205	// �����ý��� ���� - PC�� ���� ������ ó��
	INT64 iRetExp = iExp;
					
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) )
	{
		// ����ġ ���� ����
		iRetExp = 0;
	}
	else if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 ) )
	{
		// ����ġ ���
		int iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
		iRetExp = iRetExp * iAddExpRate / 100;
	}
#ifdef ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
	else if ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK1_FOR_MASTER) || gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCS_MARK2_FOR_MASTER) )
	{
		if(g_MasterLevelSystem.IsMasterLevelUser( lpObj ) ){
			// ������ �ɷ� �־ ������ ���� �������Ը� ���� �ǵ��� �Ѵ�.
			int iAddExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
			iRetExp = iRetExp * iAddExpRate / 100;
		}
		
	}
#endif // ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
#else
		
	INT64 iRetExp = iExp;
	if( lpObj->m_iPCBangPointItemTime )
	{						
		if( lpObj->m_bPCBangPointNoMoreExp > 0 )
		{
			// ����ġ ���� ����
			iRetExp = 0;
		}
		else if( lpObj->m_iPCBangPointItemAddExpPer > 0 )
		{
			// ����ġ ���
			iRetExp = iRetExp * lpObj->m_iPCBangPointItemAddExpPer / 100;
		}
	}
#endif
	
	iExp = iRetExp;
}
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

//-------------------------------------------------------------------------
// PC�� ���� �����ϱ�
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::SetPCBangPointUser( int iIndex, BOOL bIsPCBangUser )
{
	// ���� ����
	if( gObj[iIndex].Type != OBJTYPE_CHARACTER )
		return;

	if( g_btPCBangPointApplicationLevel == PP_APPLICATION_OF_NONE )
	{
		gObj[iIndex].m_bPCBangPointUser = FALSE;
	}
	else if( g_btPCBangPointApplicationLevel == PP_PCBANG_USER_ONLY )
	{
		gObj[iIndex].m_bPCBangPointUser = bIsPCBangUser;		
	}
	else if( g_btPCBangPointApplicationLevel == PP_APPLICATION_OF_ALL )
	{
		gObj[iIndex].m_bPCBangPointUser = TRUE;		
	}

#ifdef PCBANG_KUZIMO
	// ����Ʈ ������ ��û�Ѵ�.
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		LogAddTD( "[PCBangPointSystem][SetPCBangPointUser] [%s]", gObj[iIndex].AccountID );
	}
	g_PCBangPointSystem.GEReqPCBangPointInfo2( iIndex );
#else
	// ����Ʈ ������ ��û�Ѵ�.
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		LogAddTD( "[PCBangPointSystem][SetPCBangPointUser] [%s]", gObj[iIndex].AccountID );
		g_PCBangPointSystem.GEReqPCBangPointInfo( iIndex );
	}
#endif

	
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ�� �߰��Ѵ�.
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::Run( LPOBJECTSTRUCT lpObj )				
{
	// 0. ���� ����
	if( lpObj->Type != OBJTYPE_CHARACTER )
		return TRUE;
	
	// 1. PC�� ����Ʈ �ý��� On/Off Ȯ��
	if( m_bPCBangPointSystemOn == FALSE )
		return TRUE;

	// 2. ���� ����ΰ�?
	if( lpObj->Connected < CSS_LOGIN )
		return TRUE;

#ifndef PCBANG_KUZIMO
	if( CheckPCBangPointSystemUser( lpObj->m_Index ) == FALSE )
		return TRUE;
#endif

	// 3. EventServer�κ��� ������ ������ �Ϸ�޾ҳ�?
	if( lpObj->m_PCBangPointInfo.bIsDataLoadSuccess == FALSE )
	{
		return TRUE;
	}

	// 4. ���� �ð��� �̻��ϸ� ����.
	if( lpObj->m_PCBangPointInfo.dwCheckRuleTime <= 0 )
	{
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}
	
	// 5. PC�� ����Ʈ �������� ������̸� �ð�, ����Ʈ�� �������� �ʴ´�.
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifndef PCBANG_KUZIMO
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK1 )
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK2 ) 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 )
#ifdef ADD_PCBANG_BUFF_MARK4
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK4 )
#endif // ADD_PCBANG_BUFF_MARK4
		)
	{
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}
#endif
#else
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	if( lpObj->m_iPCBangPointItemTime > 0 )
	{
		--lpObj->m_iPCBangPointItemTime;
		if( lpObj->m_iPCBangPointItemTime < 0 )
		{
			lpObj->m_iPCBangPointItemTime = 0;
		}

		if( lpObj->m_iPCBangPointItemTime == 0 )
		{		
			DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SEAL_ITEM );
			GCStateInfoSend( lpObj, 0, STATE_SEAL_ITEM );

			LogAddTD( "[PCBangPointSystem][Disappear Effect] [%s][%s] (Exp:%d/Item:%d/NoMoreExp:%d)",
					lpObj->AccountID, 
					lpObj->Name, 
					lpObj->m_iPCBangPointItemAddExpPer,
					lpObj->m_iPCBangPointItemGiveItemPer,
					lpObj->m_bPCBangPointNoMoreExp
					);
		}

		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();
		return TRUE;
	}	
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifdef PCBANG_KUZIMO
	BOOL bFeverTime;
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);

	bFeverTime = FALSE;

	if(g_nPCBangPointFeverStartHour <= g_nPCBangPointFeverEndHour)
	{
		if((sysTime.wHour >= g_nPCBangPointFeverStartHour) && (sysTime.wHour < g_nPCBangPointFeverEndHour))
		{
			bFeverTime = TRUE;
		}
	}
	else
	{
		if((sysTime.wHour >= g_nPCBangPointFeverStartHour) || (sysTime.wHour < g_nPCBangPointFeverEndHour))
		{
			bFeverTime = TRUE;
		}
	}
#endif

	// 6. �ð� Ȯ�� �� ����
	DWORD dwTime = ( GetTickCount() - lpObj->m_PCBangPointInfo.dwCheckRuleTime ) / 1000;

#ifdef PCBANG_KUZIMO
	if(dwTime >= lpObj->m_PCBangPointInfo.nCurrentAppRuleTime)
	{
		//-- Ư�� �ְ����� ����Ʈ ������ �Ѵ�. ���� �ð��� ������Ʈ�Ǵ� DB���ϸ� ���̱� ���ؼ� ���� ���η� ���Դ�.
		if(lpObj->m_kWeeklyTimer.Update() != 2)	// Callback�ߵ��ÿ� �ð��� ���µȴ�. ���µɶ��� ����Ʈ�� �ø��� ����.
		{
			// �����ð� ����
			lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();

			if(CheckPCBangPointSystemUser( lpObj->m_Index ) == TRUE)
			{
				int iAddPCBangPoint = (bFeverTime == TRUE)? lpObj->m_PCBangPointInfo.nCurrentAppRulePoint * g_nPCBangPointFeverMultipleValue :
															lpObj->m_PCBangPointInfo.nCurrentAppRulePoint;

				lpObj->m_PCBangPointInfo.nPCBangPoint += iAddPCBangPoint;

				if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )
					lpObj->m_PCBangPointInfo.nPCBangPoint = 0;
				else if( lpObj->m_PCBangPointInfo.nPCBangPoint > g_nPCBangMaxPoint )
				{
					lpObj->m_PCBangPointInfo.nPCBangPoint =  g_nPCBangMaxPoint;
					GCServerMsgStringSend( lMsg.Get( 3393 ), lpObj->m_Index, 1 );	// "�� �̻� ����Ʈ�� ȹ�� �� �� �����ϴ�."
				}
				else
				{
					// ���������� ����Ʈ�� ���ŵǾ���.
					char szTemp[256];			
					wsprintf( szTemp, lMsg.Get( 3392 ), iAddPCBangPoint );
					GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "����Ʈ %d ȹ��"
				}

				// EventServer�� ����Ʈ ������ ����Ǿ����� �˸���.
				GEReqUpdatePCBangPoint2( lpObj->m_Index );


				LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
					lpObj->AccountID,
					lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
					lpObj->m_PCBangPointInfo.nPCBangPoint,
					lpObj->m_PCBangPointInfo.btCurrentRule );

				// ù ��° ��Ģ�ð��� ����ߴ�.
				if( lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime == FALSE )
				{
					lpObj->m_PCBangPointInfo.btCurrentRule				= PG_RULE_TYPE_COMMON;
					lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime	= TRUE;
					lpObj->m_PCBangPointInfo.nCurrentAppRuleTime		= g_nPCBangPointCommonRuleTime;
					lpObj->m_PCBangPointInfo.nCurrentAppRulePoint		= g_nPCBangPointCommonRuleTimePoint;			
				}
			}
		}
	}

#else
	if( dwTime >= lpObj->m_PCBangPointInfo.nCurrentAppRuleTime )
	{
		lpObj->m_PCBangPointInfo.nPCBangPoint += lpObj->m_PCBangPointInfo.nCurrentAppRulePoint;

		if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )
			lpObj->m_PCBangPointInfo.nPCBangPoint = 0;
		else if( lpObj->m_PCBangPointInfo.nPCBangPoint > g_nPCBangMaxPoint )
		{
			lpObj->m_PCBangPointInfo.nPCBangPoint =  g_nPCBangMaxPoint;
			GCServerMsgStringSend( lMsg.Get( 3393 ), lpObj->m_Index, 1 );	// "�� �̻� ����Ʈ�� ȹ�� �� �� �����ϴ�."
		}
		else
		{
			// ���������� ����Ʈ�� ���ŵǾ���.
			char szTemp[256];			
			wsprintf( szTemp, lMsg.Get( 3392 ), lpObj->m_PCBangPointInfo.nCurrentAppRulePoint );
			GCServerMsgStringSend( szTemp, lpObj->m_Index, 1 );		// "����Ʈ %d ȹ��"
		}

		// EventServer�� ����Ʈ ������ ����Ǿ����� �˸���.
		GEReqUpdatePCBangPoint( lpObj->m_Index );

		// �����ð� ����
		lpObj->m_PCBangPointInfo.dwCheckRuleTime = GetTickCount();

		LogAddTD( "[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
			lpObj->AccountID,
			lpObj->m_PCBangPointInfo.nCurrentAppRulePoint,
			lpObj->m_PCBangPointInfo.nPCBangPoint,
			lpObj->m_PCBangPointInfo.btCurrentRule );

		// ù ��° ��Ģ�ð��� ����ߴ�.
		if( lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime == FALSE )
		{
			lpObj->m_PCBangPointInfo.btCurrentRule				= PG_RULE_TYPE_COMMON;
			lpObj->m_PCBangPointInfo.bIsSuccessFirstRuleTime	= TRUE;
			lpObj->m_PCBangPointInfo.nCurrentAppRuleTime		= g_nPCBangPointCommonRuleTime;
			lpObj->m_PCBangPointInfo.nCurrentAppRulePoint		= g_nPCBangPointCommonRuleTimePoint;			
		}		
	}
#endif

	return TRUE;
}

//-------------------------------------------------------------------------
// �̺�Ʈ ������ PC�� ����Ʈ ������ ��û�Ѵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqPCBangPointInfo( int iIndex )					
{
	if( gObj[iIndex].m_bPCBangPointUser == TRUE )
	{
		// EventServer�� ������ ��û�Ѵ�.
		PMSG_REQ_PCBANG_POINT_INFO pMsg = { 0, };
		PHeadSetB( ( LPBYTE )&pMsg, 0x23, sizeof( pMsg ) );
		
		pMsg.nINDEX = iIndex;
		memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
		
#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
#else
		pMsg.btServerType = ST_SERVICESERVER;
#endif	

		pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

		DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );

		LogAddTD( "[PCBangPointSystem][User Connect] [%s] - %s", gObj[iIndex].AccountID, gObj[iIndex].Ip_addr );
	}
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ������ �̺�Ʈ �����κ��� ������ �޾� �����Ѵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvPCBangPointInfo( LPBYTE lpMsg )		
{
	LPPMSG_ANS_PCBANG_POINT_INFO pMsg = ( LPPMSG_ANS_PCBANG_POINT_INFO )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	lpObj->m_PCBangPointInfo.bIsDataLoadSuccess	= TRUE;	
	lpObj->m_PCBangPointInfo.nPCBangPoint	= pMsg->nPoint;
	lpObj->m_PCBangPointInfo.dwCheckRuleTime		= GetTickCount(); // �����Ͱ� ���������� �ε�� �� �ð��� üũ�Ѵ�.

	SendPCBangPointInfo( iIndex );
}

//-------------------------------------------------------------------------
// �̺�Ʈ������ PC�� ����Ʈ ���� ��û
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdatePCBangPoint( int iIndex )
{
	PMSG_REQ_PCBANG_POINT_NEW_INFO pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x24, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

#ifdef FOR_NEW_TESTSERVER
	pMsg.btServerType = ST_TESTSERVER;
#else
	pMsg.btServerType = ST_SERVICESERVER;
#endif	

	pMsg.nNewPoint = gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ���� ��û�� ���� ������ �޾Ҵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvUpdatePCBangPoint( LPBYTE lpMsg )
{
	LPPMSG_ANS_PCBANG_POINT_NEW_INFO pMsg = ( LPPMSG_ANS_PCBANG_POINT_NEW_INFO )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	// �̺�Ʈ�������� ������ ������Ʈ�� �������� ���� Ŭ���̾�Ʈ�� ������ ������.
	if( pMsg->btResult == TRUE )
	{
		SendPCBangPointInfo( iIndex );
	}
}

//-------------------------------------------------------------------------
// ��ŷ�̺�Ʈ : ���� ��� ����Ʈ�� ������ ������ �����ϴ� �̺�Ʈ
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdateRankingEvent( int iIndex, short nUsePoint )
{
//#ifdef PCBANG_KUZIMO
//	#ifdef FOR_TEMP_SERVER
//		return;
//	#endif
//#endif

	// �̺�Ʈ�� �������϶���.
	if( m_bPCBangPointRankingEvent == FALSE )
	{
		return;
	}

	PMSG_PCBANG_POINT_RANKING_EVENT pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x25, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;
#ifdef FOR_NEW_TESTSERVER
	pMsg.btServerType = ST_TESTSERVER;
#else
	pMsg.btServerType = ST_SERVICESERVER;
#endif	
	pMsg.nUsePoint	= nUsePoint;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� PC�� ����Ʈ�� ������.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::SendPCBangPointInfo( int iIndex )
{
	PMSG_ANS_GC_PCBANG_POINT_INFO	pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x04, sizeof( pMsg ) );

	pMsg.nPoint		= gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;
	pMsg.nMaxPoint	= g_nPCBangMaxPoint;
#ifdef UPDATE_PCBANGPOINTSYSTEM_BUGFIX
	pMsg.bPCBang	= gObj[iIndex].m_bPCBangPointUser;
#endif // UPDATE_PCBANGPOINTSYSTEM_BUGFIX

	DataSend( iIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ������ ������ ������.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::PCBangPointShopOpen( int iIndex )					
{
	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];
	
	// 1. �������̸� ������ ������ �ʴ´�.
	if( lpObj->CloseType != -1 )
		return;

	// 2. �� ��� ������ �˻�
#ifdef MODIFY_NPCTALK_REQUIREMENT_20051212	
	if( gObjIsConnectedGP( iIndex ) == FALSE )
	{
		LogAddTD( "[ANTI-HACK][PCBangPointShopOpen] Couldn't Open.. (%s)(%s)", lpObj->AccountID, lpObj->Name );
		return;
	}
#endif

	PMSG_ANS_PCBANG_POINT_SHOP_OPEN pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x06, sizeof( pMsg ) );
	pMsg.btResult = 0x00;	// ����	

#ifndef PCBANG_KUZIMO
	// 3. PC�� ����Ʈ �ý��� ��������� �˻�
	if( CheckPCBangPointSystemUser( iIndex ) == FALSE )
	{
		pMsg.btResult = 0x04;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;
	}
#endif

	// 4. ������������ �˻�
	if( CheckPCBangPointShopOpenAttr( lpObj->MapNumber, lpObj->X, lpObj->Y ) == FALSE )
	{
		pMsg.btResult = 0x01;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 5. ������ �������� ����.
	if( ShopC[m_btPCBangPointShopNum].ItemCount < 1 ) 
	{
		pMsg.btResult = 0x02;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}
		
	// 6. â�� �� �ٸ� �������̽��� ������� ���
	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != I_PCBANG_POINT_SHOP )
	{
		pMsg.btResult = 0x03;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 7. â�� ���� �޶�� ��û�� �̹� �������̴�.
	if( lpObj->m_ReqWarehouseOpen )
	{
		pMsg.btResult = 0x03;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return;	
	}

	// 8. �ϴ� ����� ������.
	DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
	//return;

	// 9. ������� �������� ���� ����� �ִ� ���	
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719
	// ������ ���� ���� - ���� ���, �κ��丮 ���, ī�������� ���� ��
	if( gObjFixInventoryPointer( iIndex ) == false ) 
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
#endif

	// 10. ������ ������ ������.
	PMSG_SHOPITEMCOUNT	pShopItemCount;
	BYTE				SendByte[1024];
	int					lOfs = 0;

	// ���� �������̽� ����
	lpObj->TargetShopNumber = m_btPCBangPointShopNum;
	lpObj->m_IfState.use    = 1;
	lpObj->m_IfState.type   = I_PCBANG_POINT_SHOP;
	lpObj->m_ShopTime		= 0;

//	pResult.h.c				= PMHCE_BYTE;
//	pResult.h.headcode		= 0x30;
//	pResult.h.size			= sizeof( pResult );
//	pResult.result			= 0x00;
//
//	// ���� ��ȣ�� ������.
//	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	// ������ ������ ����Ʈ�� ������.
	lOfs += sizeof( pShopItemCount );
	int size = lOfs + ShopC[m_btPCBangPointShopNum].SendItemDataLen;
			
	PHeadSetW( ( LPBYTE )&pShopItemCount, 0x31, size );

	pShopItemCount.Type			= 0;
	pShopItemCount.count		= ShopC[m_btPCBangPointShopNum].ItemCount;
	memcpy( SendByte, &pShopItemCount, sizeof( pShopItemCount ) );
	memcpy( SendByte + lOfs, ShopC[m_btPCBangPointShopNum].SendItemData, ShopC[m_btPCBangPointShopNum].SendItemDataLen );

	DataSend( iIndex, ( LPBYTE )SendByte, size );	
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ �������� �������� ����
//-------------------------------------------------------------------------
BOOL CPCBangPointSystem::BuyPCBangPointItem( int iIndex, BYTE btShopPos )				
{
	// ������ ���� ������ ���� �Լ� : CGBuyRequestRecv
	// PC�� ����Ʈ �������� ���Խ� ������ ������� ������ ����Ʈ�� ����ϱ� ������ 
	// ������ ���� ������ ���� �Լ��� ����� �� ���� �̰����� ó���Ѵ�.

	LPOBJECTSTRUCT lpObj = ( LPOBJECTSTRUCT )&gObj[iIndex];

	PMSG_ANS_GC_BUY_PCBANG_POINT_ITEM pMsg;	
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xD0, 0x05, sizeof( pMsg ) );
	pMsg.btResult		= 0x06;		// �ϴ� ���з�.
	pMsg.btInvenPos		= 0XFF;

	// 1. ���� ��
	if( lpObj->CloseType != -1 )
	{
		return FALSE;
	}

#ifndef PCBANG_KUZIMO
	// 2. PC�� ����Ʈ �ý��� ��������� �˻�
	if( CheckPCBangPointSystemUser( iIndex ) == FALSE )
	{
		pMsg.btResult = 0x05;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
#endif

	// 3. ������������ �˻�
	if( CheckPCBangPointShopOpenAttr( lpObj->MapNumber, lpObj->X, lpObj->Y ) == FALSE )
	{
		pMsg.btResult = 0x01;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	
	// 4. �� ��� ������ �˻�
#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ���� - NPC�������� �������� �� ��
	if( gObjFixInventoryPointer( iIndex ) == false ) 
	{
		LogAdd( "[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__ );		
	}
#endif

	// 5. ������ �� ������ ���ѽð��� �ִ�.
	if( gObj[iIndex].m_ShopTime == 0 )
	{
		gObj[iIndex].m_ShopTime = 1; 
	}
	
	// �������� ��� �ִ� �ִ� �ð��� �Ѿ���.
	if( gObj[iIndex].m_ShopTime > 60 )
	{
		if( lpObj->m_IfState.use && lpObj->m_IfState.type == I_PCBANG_POINT_SHOP )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use    = 0;
			lpObj->m_IfState.type   = 0;
		}

		pMsg.btResult = 0x07;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 6. PC�� ����Ʈ ���� ��������� Ȯ��
	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != I_PCBANG_POINT_SHOP )
	{		
		pMsg.btResult = 0x05;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	
	// 7. Ŭ���̾�Ʈ���� �� ���� ��ȿ�� �˻�
	int iShopNum;
	iShopNum = lpObj->TargetShopNumber;
	
	if( iShopNum < 0 || ( iShopNum > MAX_SHOP-1 ) ) 
	{
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	if( btShopPos > MAX_SHOPITEM - 1 )
	{
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}
	if( ShopC[iShopNum].m_item[btShopPos].IsItem() == FALSE )
	{
		pMsg.btResult = 0x02;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 8. ������ Ȯ��
	BOOL IsPCBangPointItem = FALSE;
	DWORD dwItemNum = ShopC[iShopNum].m_item[btShopPos].m_Type;
	int iOptIndex = 0;
	LPPCBANG_POINT_ITEM_OPTION p = &m_ItemOption[0];
	for( int n = 0; n < MAX_PCBANG_POINT_ITEM_COUNT; n++ )
	{
		if( m_ItemOption[n].dwItemType == dwItemNum )
		{
			iOptIndex = n;
			IsPCBangPointItem = TRUE;
		}
	}
	if( !IsPCBangPointItem )
	{
		// PC�� ����Ʈ �������� �ƴ�
		pMsg.btResult = 0x08;	
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 9. ����Ʈ Ȯ��
	if( lpObj->m_PCBangPointInfo.nPCBangPoint < m_ItemOption[iOptIndex].nBuyingPoint )
	{
		// ����Ʈ�� ���ڶ���.
		pMsg.btResult = 0x09;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	if( lpObj->m_PCBangPointInfo.nPCBangPoint < m_ItemOption[iOptIndex].nConditionPoint )
	{
		// ������ ���� ������ ���ǿ� ���� �ʴ´�.
		pMsg.btResult = 0x04;
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
		return FALSE;
	}

	// 10. ������ ó��
	if( m_ItemOption[iOptIndex].btAbility == 1 )	// ���� ������
	{
		ItemByteConvert( pMsg.ItemInfo, ShopC[iShopNum].m_item[btShopPos] );
		pMsg.btResult = 0x00;
		pMsg.btInvenPos = 0xFF;		// *���� �������� �κ��丮�� �߰����� �ʰ� �ٷ� ����ȴ�.
		DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		// ������ �ڵ忡 ���� ����Ǵ� ���� �������� �ٲ��ش�.
		int iBuffType = BUFFTYPE_NONE;

		if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 43 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK1;
		}
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 44 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK2;
		}
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 45 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK3;
		}
#ifdef ADD_PCBANG_BUFF_MARK4
		else if( ShopC[iShopNum].m_item[btShopPos].m_Type == MAKE_ITEMNUM( 13, 96 ) )
		{
			iBuffType = BUFFTYPE_PCBANG_POINT_MARK4;
		}
#endif // ADD_PCBANG_BUFF_MARK4

		// �� �Ӽ��� ItemAddOption���� ���� ���´�.
		gObjAddBuffEffect( lpObj, iBuffType );
		//gObjAddBuffEffect( lpObj, iBuffType, EFFECTTYPE_EXPERIENCE,  );
#else
		// �ɼ� ����
		lpObj->m_iPCBangPointItemTime			= m_ItemOption[iOptIndex].dwKeepTime_Sec;
		lpObj->m_iPCBangPointItemAddExpPer		= m_ItemOption[iOptIndex].nAddExpPercent;		
		lpObj->m_iPCBangPointItemGiveItemPer	= m_ItemOption[iOptIndex].nGetItemPercent;
		lpObj->m_bPCBangPointNoMoreExp			= m_ItemOption[iOptIndex].btNoMoreExp;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		
		// ����Ʈ ����
		lpObj->m_PCBangPointInfo.nPCBangPoint -= m_ItemOption[iOptIndex].nBuyingPoint;

		// ����Ʈ ���� ������Ʈ
		if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )	// �̷� ���� �߻��ϸ� �ȵȴ�.
			lpObj->m_PCBangPointInfo.nPCBangPoint = 0;

#ifdef PCBANG_KUZIMO
		GEReqUpdatePCBangPoint2( iIndex );
#else
		GEReqUpdatePCBangPoint( iIndex );
#endif
		GEReqUpdateRankingEvent( iIndex, m_ItemOption[iOptIndex].nBuyingPoint );

		LogAddTD( "[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
			lpObj->AccountID, 
			lpObj->Name, 	
			m_ItemOption[iOptIndex].nBuyingPoint,
			lpObj->m_PCBangPointInfo.nPCBangPoint,
			g_nPCBangMaxPoint,
			iShopNum,
			ShopC[iShopNum].m_item[btShopPos].GetName(),
			m_ItemOption[iOptIndex].dwKeepTime_Sec,
			ShopC[iShopNum].m_item[btShopPos].m_Level,
			(int)ShopC[iShopNum].m_item[btShopPos].m_Durability,
			ShopC[iShopNum].m_item[btShopPos].m_Option1,
			ShopC[iShopNum].m_item[btShopPos].m_Option2,
			ShopC[iShopNum].m_item[btShopPos].m_Option3
			);

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
		ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_SEAL_ITEM );	
		GCStateInfoSend( lpObj, 1, STATE_SEAL_ITEM );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004	
	}
	else	// �Ϲ� ������
	{
		// �������� ������� �κ��丮�� �־��.
		pMsg.btInvenPos = gObjShopBuyInventoryInsertItem( iIndex, ShopC[iShopNum].m_item[btShopPos] );
		if( pMsg.btInvenPos != 0xFF )
		{
			ItemByteConvert( pMsg.ItemInfo, ShopC[iShopNum].m_item[btShopPos] );
			pMsg.btResult = 0x00;
			DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );

			// ����Ʈ ����
			lpObj->m_PCBangPointInfo.nPCBangPoint -= m_ItemOption[iOptIndex].nBuyingPoint;		
			LogAddTD( "[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
				lpObj->AccountID, 
				lpObj->Name, 	
				m_ItemOption[iOptIndex].nBuyingPoint,
				lpObj->m_PCBangPointInfo.nPCBangPoint,
				g_nPCBangMaxPoint,
				iShopNum,
				ShopC[iShopNum].m_item[btShopPos].GetName(),
				m_ItemOption[iOptIndex].dwKeepTime_Sec,
				ShopC[iShopNum].m_item[btShopPos].m_Level,
				(int)ShopC[iShopNum].m_item[btShopPos].m_Durability,
				ShopC[iShopNum].m_item[btShopPos].m_Option1,
				ShopC[iShopNum].m_item[btShopPos].m_Option2,
				ShopC[iShopNum].m_item[btShopPos].m_Option3
				);


			// ����Ʈ ���� ������Ʈ
			if( lpObj->m_PCBangPointInfo.nPCBangPoint < 0 )	// �̷� ���� �߻��ϸ� �ȵȴ�.
				lpObj->m_PCBangPointInfo.nPCBangPoint = 0;

#ifdef PCBANG_KUZIMO
			GEReqUpdatePCBangPoint2( iIndex );
#else
			GEReqUpdatePCBangPoint( iIndex );
#endif
			GEReqUpdateRankingEvent( iIndex, m_ItemOption[iOptIndex].nBuyingPoint );
		}
		else
		{
			// �κ��丮�� �� ������ ����
			pMsg.btResult = 0x03;
			DataSend( iIndex, ( LPBYTE )&pMsg, pMsg.h.size );
			return FALSE;			
		}
	}
	return TRUE;
}

#ifdef PCBANG_KUZIMO
VOID CPCBangPointSystem::WeeklyTimerCallBack(MWeeklyTimer *pkMyDate, VOID *pArg)
{
	WORD objIndex = (WORD)pArg;

		// EventServer�� ����Ʈ ������ ����Ǿ����� �˸���.
	int iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour;

	return;
//	int iOldPoint;

//	iOldPoint = gObj[objIndex].m_PCBangPointInfo.nPCBangPoint;

	pkMyDate->GetWakeDate(iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour);

	LogAddTD( "[PCBangPointSystem][Point Reset] [%s] - OldData(%04d-%02d-%02d %02d Hour %d Points) ResetDate(%04d-%02d-%02d %02d Hour)", 
		       gObj[objIndex].AccountID, gObj[objIndex].m_PCBangPointInfo.nResetYear, gObj[objIndex].m_PCBangPointInfo.nResetMonth, gObj[objIndex].m_PCBangPointInfo.nResetDay, 
			   gObj[objIndex].m_PCBangPointInfo.nResetHour, gObj[objIndex].m_PCBangPointInfo.nPCBangPoint,
			   iOutWakeYear, iOutWakeMonth, iOutWakeDay, iOutWakeHour);

	gObj[objIndex].m_PCBangPointInfo.nResetYear			= iOutWakeYear;
	gObj[objIndex].m_PCBangPointInfo.nResetMonth		= iOutWakeMonth;
	gObj[objIndex].m_PCBangPointInfo.nResetDay			= iOutWakeDay;
	gObj[objIndex].m_PCBangPointInfo.nResetHour			= iOutWakeHour;

	gObj[objIndex].m_PCBangPointInfo.nPCBangPoint		= 0;
	gObj[objIndex].m_PCBangPointInfo.dwCheckRuleTime	= GetTickCount();

//	if(iOldPoint == 0) return;	// PCBangPoint�� 0�̸� �������� ����. DB�� ��������.��

	g_PCBangPointSystem.GEReqUpdatePCBangPoint2(objIndex);
}

//-------------------------------------------------------------------------
// �̺�Ʈ ������ PC�� ����Ʈ ������ ��û�Ѵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqPCBangPointInfo2( int iIndex )					
{
	// EventServer�� ������ ��û�Ѵ�.
	PMSG_REQ_PCBANG_POINT_INFO2 pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x29, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	

#ifdef FOR_TEMP_SERVER
	pMsg.btServerType = ST_TEMPSERVER;
#else
	#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
	#else
		pMsg.btServerType = ST_SERVICESERVER;
	#endif
#endif	

	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );

	LogAddTD( "[PCBangPointSystem][User Connect] [%s] - %s", gObj[iIndex].AccountID, gObj[iIndex].Ip_addr );
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ������ �̺�Ʈ �����κ��� ������ �޾� �����Ѵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvPCBangPointInfo2( LPBYTE lpMsg )		
{
	LPPMSG_ANS_PCBANG_POINT_INFO2 pMsg = ( LPPMSG_ANS_PCBANG_POINT_INFO2 )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	lpObj->m_PCBangPointInfo.bIsDataLoadSuccess	= TRUE;	
	lpObj->m_PCBangPointInfo.nPCBangPoint	= pMsg->nPoint;
	lpObj->m_PCBangPointInfo.dwCheckRuleTime		= GetTickCount(); // �����Ͱ� ���������� �ε�� �� �ð��� üũ�Ѵ�.

	lpObj->m_PCBangPointInfo.nResetYear		= pMsg->nResetYear;
	lpObj->m_PCBangPointInfo.nResetMonth	= pMsg->nResetMonth;
	lpObj->m_PCBangPointInfo.nResetDay		= pMsg->nResetDay;
	lpObj->m_PCBangPointInfo.nResetHour		= pMsg->nResetHour;

	lpObj->m_kWeeklyTimer.Init(lpObj->m_PCBangPointInfo.nResetYear, lpObj->m_PCBangPointInfo.nResetMonth, lpObj->m_PCBangPointInfo.nResetDay,
		lpObj->m_PCBangPointInfo.nResetHour, g_nPCBangResetDayOfWeek, g_nPCBangResetHour, WeeklyTimerCallBack, (VOID*)iIndex);
	lpObj->m_kWeeklyTimer.Activate(FALSE);

	if(lpObj->m_kWeeklyTimer.Update() != 2)	// ���µɶ��� Ŭ���̾�Ʈ�� ����Ʈ�� ������ ����.
	{
		SendPCBangPointInfo( iIndex );
	}
}

//-------------------------------------------------------------------------
// �̺�Ʈ������ PC�� ����Ʈ ���� ��û
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::GEReqUpdatePCBangPoint2( int iIndex )
{
	PMSG_REQ_PCBANG_POINT_NEW_INFO2 pMsg = { 0, };
	PHeadSetB( ( LPBYTE )&pMsg, 0x30, sizeof( pMsg ) );
	
	pMsg.nINDEX = iIndex;
	memcpy( pMsg.szUID, gObj[iIndex].AccountID, MAX_IDSTRING );
	pMsg.szUID[sizeof( pMsg.szUID ) - 1] = 0;

#ifdef FOR_TEMP_SERVER
	pMsg.btServerType = ST_TEMPSERVER;
#else
	#ifdef FOR_NEW_TESTSERVER
		pMsg.btServerType = ST_TESTSERVER;
	#else
		pMsg.btServerType = ST_SERVICESERVER;
	#endif
#endif	

	pMsg.nNewPoint = gObj[iIndex].m_PCBangPointInfo.nPCBangPoint;

	pMsg.nResetYear		= gObj[iIndex].m_PCBangPointInfo.nResetYear;
	pMsg.nResetMonth	= gObj[iIndex].m_PCBangPointInfo.nResetMonth;
	pMsg.nResetDay		= gObj[iIndex].m_PCBangPointInfo.nResetDay;
	pMsg.nResetHour		= gObj[iIndex].m_PCBangPointInfo.nResetHour;

	DataSendEventChip( ( char* )&pMsg, sizeof( pMsg ) );
}

//-------------------------------------------------------------------------
// PC�� ����Ʈ ���� ��û�� ���� ������ �޾Ҵ�.
//-------------------------------------------------------------------------
VOID CPCBangPointSystem::EGRecvUpdatePCBangPoint2( LPBYTE lpMsg )
{
	LPPMSG_ANS_PCBANG_POINT_NEW_INFO2 pMsg = ( LPPMSG_ANS_PCBANG_POINT_NEW_INFO2 )lpMsg;
	int iIndex = pMsg->nINDEX;

	if( !CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	// �̺�Ʈ�������� ������ ������Ʈ�� �������� ���� Ŭ���̾�Ʈ�� ������ ������.
	if( pMsg->btResult == TRUE )
	{
		SendPCBangPointInfo( iIndex );
	}
}


#endif

#ifdef ADD_PCBANG_EXPRATE
BOOL CPCBangPointSystem::CheckPCBangAddExperience( LPOBJECTSTRUCT lpObj, INT64 &iExp )
{
	if( !lpObj->m_bPCBangPointUser )
		return FALSE;

	INT64 iRetExp = (int)((float)iExp * g_fPCBangAddExperience);
	iExp = iRetExp;
	
	return TRUE;
}
#endif // ADD_PCBANG_EXPRATE

#endif	// PCBANG_POINT_SYSTEM_20070206
