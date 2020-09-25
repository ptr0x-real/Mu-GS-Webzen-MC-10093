// KanturuMonsterMng.cpp: implementation of the CKanturuMonsterMng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef ADD_KANTURU_20060627

#include "KanturuMonsterMng.h"

#include "Include\ReadScript.h"
#include "MapClass.h"
extern MapClass		MapC[];

#include "KanturuDefine.h"


CKanturuMonsterMng	g_KanturuMonsterMng;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuMonsterMng::CKanturuMonsterMng()
{
	ResetLoadData();	
	ResetRegenMonsterObjData();
}

CKanturuMonsterMng::~CKanturuMonsterMng()
{

}

void CKanturuMonsterMng::ResetLoadData()
{
	for( int iCount = 0; iCount < MAX_KANTURU_MONSTER_SETBASE_INFO; iCount++ )
	{
		memset( &m_SetBaseInfo[iCount], -1, sizeof( KANTURU_MONSTER_SETBASE_INFO ) );
	}

	m_iMaxMonsterCount = 0;
}

// �ʿ� ������ ������ ������ �ʱ�ȭ�Ѵ�.
void CKanturuMonsterMng::ResetRegenMonsterObjData()
{
	for( int iCount = 0; iCount < m_KanturuMonster.GetCount(); iCount++ )
	{
		if( m_KanturuMonster.m_iObjIndex[iCount] != _INVALID )
		{
			gObjDel( m_KanturuMonster.m_iObjIndex[iCount] );
		}
	}

	if( m_iMayaObjIndex != -1 )
	{
		gObjDel( m_iMayaObjIndex );
	}
	
	m_KanturuMonster.Reset();
	m_iMaxRegenMonsterCount		= 0;
	m_iAliveMonsterCount		= 0;

	m_iMayaObjIndex				= -1;
#ifdef KANTURU_TEST_200607119
	m_iNightmareObjIndex		= -1;
#endif
}

// ĭ���� �������� ������ �Ϲ� ���Ͱ� �׾��ٸ� �׿����� ó���� �� �ش�.
void CKanturuMonsterMng::MonsterDie( int iIndex )
{
	LPOBJECTSTRUCT	lpObj = &gObj[iIndex];
	for( int iCount = 0; iCount < m_KanturuMonster.GetCount(); iCount++ )
	{
		if( m_KanturuMonster.m_iObjIndex[iCount] == iIndex )
		{
			--m_iAliveMonsterCount;

		//	int iMaxHitUser	= gObjMonsterTopHitDamageUser( lpObj );
		//	LogAddTD( "[ KANTURU ][ BossMap Common Monster ] %s(Index:%d) Killer-[%s][%s], MonsterCount:%d",
		//				gObj[iIndex].Name, gObj[iIndex].Class, 
		//				gObj[iMaxHitUser].AccountID, gObj[iMaxHitUser].Name, m_iAliveMonsterCount );
			return;
		}
	}	
}
//--------------------------------------------------------------------------------------
BOOL CKanturuMonsterMng::LoadData( LPSTR lpszFileName )
{
	m_bFileDataLoad = FALSE;

	if( lpszFileName == NULL || !strcmp( lpszFileName, "" ) ) 
	{
		MsgBox( "[ KANTURU ][ MonsterSetBase ] - File load error : File Name Error" );
		return FALSE;
	}

	try
	{		
		if( ( SMDFile = fopen( lpszFileName, "r" ) ) == NULL )	
		{
			DWORD dwError = GetLastError();
			MsgBox( "[ KANTURU ][ MonsterSetBase ] - Can't Open %s ", lpszFileName );			
			return FALSE;
		}
		
		// ��� ������ �ʱ�ȭ.
		ResetLoadData();

		SMDToken Token;
		int iType = -1;

		BYTE	btGroup			= 0;
		WORD    wType			= 0;
		BYTE	btMapNumber		= 0;
		BYTE    btDistance		= 0;		
		BYTE	btX				= 0;
		BYTE	btY				= 0;
		BYTE	btDir			= 0;		

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
					btGroup			= 0;
					wType			= 0;
					btMapNumber		= 0;
					btDistance		= 0;		
					btX				= 0;
					btY				= 0;
					btDir			= 0;

					// 2. >	������ �б�
					Token = GetToken();
					if( strcmp( "end", TokenString ) == NULL ) break;					
					
					btGroup = ( BYTE )TokenNumber;
					Token = GetToken(); wType		= ( WORD )TokenNumber;
					Token = GetToken(); btMapNumber	= ( BYTE )TokenNumber;
					Token = GetToken(); btDistance	= ( BYTE )TokenNumber;						
					Token = GetToken(); btX			= ( BYTE )TokenNumber;						
					Token = GetToken(); btY			= ( BYTE )TokenNumber;						
					Token = GetToken(); btDir		= ( BYTE )TokenNumber;						
						
					// 3. >	������ ��ȿ�� �˻�
					if( m_iMaxMonsterCount < 0 || m_iMaxMonsterCount >= MAX_KANTURU_MONSTER_SETBASE_INFO )
					{
						MsgBox( "[ KANTURU ][ MonsterSetBase ] - Exceed Max Info Count (%d)", m_iMaxMonsterCount );
						break;
					}
					
					// 4. > ������ ����
					SetMonsterSetBaseInfo( m_iMaxMonsterCount, btGroup, wType, btMapNumber, btDistance, btX, btY, btDir );

					++m_iMaxMonsterCount;					
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
		
		LogAddC( 2, "[ KANTURU ][ MonsterSetBase ] - %s file is Loaded", lpszFileName );

		m_bFileDataLoad = TRUE;	
	}
	catch(...) 
	{
		MsgBox( "[ KANTURU ][ MonsterSetBase ] Loading Exception Error (%s) File. ", lpszFileName );
	}

	return m_bFileDataLoad;	
}

void CKanturuMonsterMng::SetMonsterSetBaseInfo( int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir )
{
	m_SetBaseInfo[iIndex].btGroup		= btGroup;
	m_SetBaseInfo[iIndex].wType			= wType;
	m_SetBaseInfo[iIndex].btMapNumber	= btMapNumber;
	m_SetBaseInfo[iIndex].btDistance	= btDistance;
	m_SetBaseInfo[iIndex].btX			= btX;
	m_SetBaseInfo[iIndex].btY			= btY;
	m_SetBaseInfo[iIndex].btDir			= btDir;
}

//--------------------------------------------------------------------------------------
// ���߿��� ������ ���� ������ �׷��ȣ�� ���߾� �����Ѵ�. ������ ������ ���� �����ش�.
int CKanturuMonsterMng::SetKanturuMonster( int iGroupNumber )
{
	int				iMapNumber = 0;
	WORD			wType = 0;
	LPOBJECTSTRUCT	lpObj;

	int iResult;
	int iRegenCount = 0;		// �� ������ ���͸� �����ߴ����� ���� ����.
	for( int iCount = 0; iCount < MAX_KANTURU_MONSTER_SETBASE_INFO; iCount++ )
	{
		// ���� �׷� ��ȣ�� ���͸� �����Ѵ�.
		if( m_SetBaseInfo[iCount].btGroup == iGroupNumber && m_SetBaseInfo[iCount].wType != 0 )
		{		
			iResult = gObjAddMonster( m_SetBaseInfo[iCount].btMapNumber );

			if( iResult < 0 )
			{
				LogAddTD( "[ KANTURU ][ SetBossMapMonster ] Fail - Type:%d Map[%d]-[%d][%d]",
						m_SetBaseInfo[iCount].wType, m_SetBaseInfo[iCount].btMapNumber, m_SetBaseInfo[iCount].btX, m_SetBaseInfo[iCount].btY );
			}
			else
			{
				lpObj = ( LPOBJECTSTRUCT )&gObj[iResult];				
				lpObj->m_PosNum     = -1;									// �������� �ʴ� �����̴�.
				lpObj->X			= m_SetBaseInfo[iCount].btX;
				lpObj->Y			= m_SetBaseInfo[iCount].btY;
				lpObj->MapNumber	= m_SetBaseInfo[iCount].btMapNumber;
							

				// gObjGetRandomFreeLocation 
				if( GetPosition( iCount, lpObj->MapNumber, lpObj->X, lpObj->Y ) == FALSE )
				{
					gObjDel( iResult );
					continue;
				}
				
				lpObj->TX			= lpObj->X;
				lpObj->TY			= lpObj->Y;
				lpObj->m_OldX		= lpObj->X;
				lpObj->m_OldY		= lpObj->Y;				
				lpObj->StartX       = ( BYTE )lpObj->X;
				lpObj->StartY       = ( BYTE )lpObj->Y;	
				lpObj->m_MoveRange	= m_SetBaseInfo[iCount].btDistance;
				lpObj->DieRegen		= 0;

				if( m_SetBaseInfo[iCount].btDir == 0xFF )
				{
					lpObj->Dir			= rand() % 8;				// ������ ����
				}
				else
				{
					lpObj->Dir			= m_SetBaseInfo[iCount].btDir;
				}
				
				if( !gObjSetMonster( iResult, m_SetBaseInfo[iCount].wType ) )
				{
					gObjDel( iResult );
					continue;
				}
				
				if( m_SetBaseInfo[iCount].wType == 364 )
				{
					// ������ Object Index - ���ߴ� ��������� �ƴϴ�.
					m_iMayaObjIndex = iResult;

					LogAddTD( "[ KANTURU ][ SetBossMapMonster ] %s(Index:%d / ObjIndex:%d) Map:%d-[%d][%d]",										
										gObj[iResult].Name, gObj[iResult].Class, iResult, 
										gObj[iResult].MapNumber, gObj[iResult].X, gObj[iResult].Y );
					continue;
				}
				
				if( gObj[iResult].Type == OBJTYPE_MONSTER )
				{
					// ĭ���� ���Ϳ� ���� ����
					m_KanturuMonster.AddObj( iResult );
					++m_iMaxRegenMonsterCount;
					++m_iAliveMonsterCount;	

					LogAddTD( "[ KANTURU ][ SetBossMapMonster ] Count:%d %s(Index:%d / ObjIndex:%d) Map:%d-[%d][%d]",
										m_iAliveMonsterCount,
										gObj[iResult].Name, gObj[iResult].Class, iResult, 
										gObj[iResult].MapNumber, gObj[iResult].X, gObj[iResult].Y );
				}

				++iRegenCount;
			}
		}
	}
	return iRegenCount;
}

// ������ ��ǥ�� �������� ���� ��ġ������ ��ǥ�� �����Ѵ�.
BOOL CKanturuMonsterMng::GetPosition( int TableNum, short MapNumber, short &x, short &y )
{	
	if( TableNum < 0 || ( TableNum > ( MAX_KANTURU_MONSTER_SETBASE_INFO - 1 ) ) ) 
	{
		LogAdd( "ERROR : %s %d", __FILE__, __LINE__ );
		return FALSE;
	}

	int iOrx, iOry;
	iOrx = m_SetBaseInfo[TableNum].btX;
	iOry = m_SetBaseInfo[TableNum].btY;

	BYTE btAttr = MapC[MapNumber].GetAttr( iOrx, iOry );
	if( ( ( btAttr & MAP_ATTR_SAFTYZONE ) != MAP_ATTR_SAFTYZONE ) && 
		( ( btAttr & MAP_ATTR_BLOCK ) != MAP_ATTR_BLOCK ) && 
		( ( btAttr & MAP_ATTR_HOLLOW ) != MAP_ATTR_HOLLOW ) ) // ����ִ�
	{
		x = iOrx;
		y = iOry;
		return TRUE;
	}
	
	int iCount = 100;
	// ��ȿ���� ���� ��ǥ�� ���� ��˻��Ͽ� ��ȿ�� ��ǥ�� �����ش�.
	while( iCount-- ) 
	{
		iOrx = m_SetBaseInfo[TableNum].btX - 3;
		iOry = m_SetBaseInfo[TableNum].btY - 3;
		
		iOrx += ( rand() % 7 );
		iOry += ( rand() % 7 );	

		btAttr = MapC[MapNumber].GetAttr( iOrx, iOry );
		if( ( ( btAttr & MAP_ATTR_SAFTYZONE ) != MAP_ATTR_SAFTYZONE ) && 
			( ( btAttr & MAP_ATTR_BLOCK ) != MAP_ATTR_BLOCK ) && 
			( ( btAttr & MAP_ATTR_HOLLOW ) != MAP_ATTR_HOLLOW ) ) // ����ִ�
		{
			x = iOrx;
			y = iOry;
			return TRUE;
		}
	}

	return FALSE;	
}

//--------------------------------------------------------------------------------------
// ĭ���� �������� ������ ���Ͱ� �� ����ִ� ���Ͱ� �ִ°�?
int CKanturuMonsterMng::IsExistAliveMonster()
{
	// ���Ͱ� ��� ���
	if( m_iAliveMonsterCount <= 0 )
		return FALSE;

	// ����ִ� ���Ͱ� �ִ�.
	return TRUE;
}

#endif