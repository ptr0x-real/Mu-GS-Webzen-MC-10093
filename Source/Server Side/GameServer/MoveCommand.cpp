// MoveCommand.cpp: implementation of the CMoveCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\Prodef.h"
#include "..\\common\\winutil.h"
#include "..\\Common\\classdef.h"
#include "..\\Include\\public.h"
#include "..\\Include\\Readscript.h"

#include "user.h"
#include "Protocol.h"
#include "Sprotocol.h"


#include "MoveCommand.h"

#ifdef AUTH_GAMESERVER
#include "..\\common\\WzMemScript.h"
#endif

#ifdef DEVILSQUARE_EXTEND_20050221
#include "DevilSquare.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include ".\ChaosCastle.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include ".\CastleSiege.h"
#endif

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
#include ".\CastleSiegeSync.h"
#endif

#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
#include ".\BloodCastle.h"
#include ".\KalimaGate.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#include "CashShop.h"
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
#include "CashItemPeriodSystem.h"
#endif // MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521

#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ADD_NEWPVP_PKFIELD
#include "Gamemain.h"
#endif // ADD_NEWPVP_PKFIELD

CMoveCommand	gMoveCommand;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveCommand::CMoveCommand()
{

}

CMoveCommand::~CMoveCommand()
{

}

void CMoveCommand::Init()
{
	memset(m_MoveCommandData, 0, sizeof(m_MoveCommandData));
	for( int i = 0; i <	MAX_MOVE_COMMAND_COUNT; i++ )
	{
		m_MoveCommandData[i].Index = -1;
	}
}

BOOL CMoveCommand::Load(char* filename)
{
	Init();

	if((SMDFile=fopen(filename,"r")) == NULL)
	{
		MsgBox("MoveCommand %s File Load Error", filename);
		return FALSE;
	}
	SMDToken Token;

	int	counter = 0;

	memset(m_MoveCommandData,0,sizeof(MOVE_COMMAND_DATA)*MAX_MOVE_COMMAND_COUNT);

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END || counter >= MAX_MOVE_COMMAND_COUNT) break;		
		if(Token == NUMBER)
		{	
			m_MoveCommandData[counter].Index = (int)TokenNumber;
			Token = (*GetToken)(); strcpy(m_MoveCommandData[counter].Name,TokenString);
			Token = (*GetToken)(); m_MoveCommandData[counter].NeedZen = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].NeedLevel = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].MapNumber = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].X1 = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].Y1 = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].X2 = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].Y2 = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].VIP = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveCommandData[counter].Reset = (int)TokenNumber;
			counter++;
		}
		else
		{
			Init();
			fclose(SMDFile);
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}
	
	fclose(SMDFile);
	return TRUE;
}

#ifdef AUTH_GAMESERVER
BOOL CMoveCommand::Load(char *Buffer, int iSize)
{
	Init();

	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	SMDToken Token;

	int	counter = 0;

	while( true )
	{
		Token = WzMemScript.GetToken();
		if(Token == END) break;		
		if(Token == NUMBER)
		{	
			m_MoveCommandData[counter].Index = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); strcpy(m_MoveCommandData[counter].Name,WzMemScript.GetString());
			Token = WzMemScript.GetToken(); strcpy(m_MoveCommandData[counter].EngName,WzMemScript.GetString());
			
			Token = WzMemScript.GetToken(); m_MoveCommandData[counter].NeedZen = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MoveCommandData[counter].NeedLevel = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MoveCommandData[counter].GateNumber = WzMemScript.GetNumber();
			counter++;
		}
		else
		{
			Init();			
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}	
	return TRUE;
}
#endif


#ifdef DARKLORD_WORK
BOOL CMoveCommand::LoadMoveLevel(char* filename)
{
	memset(m_MoveLevel, -1, sizeof(m_MoveLevel));

	if((SMDFile=fopen(filename,"r")) == NULL)
	{
		MsgBox("MoveLevel %s File Load Error", filename);
		return FALSE;
	}
	SMDToken Token;

	int	counter = 0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;		
		if(Token == NUMBER)
		{	
			m_MoveLevel[counter].MoveLevel = (int)TokenNumber;
			
			Token = (*GetToken)(); m_MoveLevel[counter].MapNumber = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveLevel[counter].X = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveLevel[counter].Y = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveLevel[counter].TX = (int)TokenNumber;
			Token = (*GetToken)(); m_MoveLevel[counter].TY = (int)TokenNumber;
			counter++;
		}
		else
		{
			Init();
			fclose(SMDFile);
			MsgBox("MoveLevel script load fail");
			return FALSE;
		}
	}
	
	fclose(SMDFile);
	return TRUE;
}

int	CMoveCommand::GetMoveLevel(int mapnumber, int x, int y, int Class)
{
	for( int i = 0; i < MAX_MOVE_COMMAND_COUNT; i++ )
	{
		if( m_MoveLevel[i].MapNumber == mapnumber )
		{
			if( x >= m_MoveLevel[i].X && x <= m_MoveLevel[i].TX &&
				y >= m_MoveLevel[i].Y && y <= m_MoveLevel[i].TY )
			{
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108 // ����� ���� ��� ���˻糪 ��ũ�ε嵵 Movereq.txt ������ ���� ����
				if( mapnumber == MAP_INDEX_SWAMP_OF_CALMNESS )
					return m_MoveLevel[i].MoveLevel;
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
				
				if( Class == CLASS_DARKLORD || Class == CLASS_MAGUMSA )					
					return (m_MoveLevel[i].MoveLevel*2)/3;
				else
					return m_MoveLevel[i].MoveLevel;
			}			
		}
	
	}
	return -1;
}
#endif

#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
// 0. ���� ������ üũ
BOOL CMoveCommand::CheckMainToMove( LPOBJECTSTRUCT lpObj )
{
	if( CHECK_BLOODCASTLE(lpObj->MapNumber) ) 
		return FALSE;

#ifdef CHAOSCASTLE_SYSTEM_20040408
	if( CHECK_CHAOSCASTLE(lpObj->MapNumber) )
		return FALSE;
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
	if( CHECK_KALIMAMAP(lpObj->MapNumber) )
		return FALSE;
#endif

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
	if( lpObj->MapNumber == MAP_INDEX_DEVILSQUARE )
#endif
		return FALSE;

#ifdef ADD_KANTURU_20060627	// ĭ���� �����ʿ��� ��ũ�ε� ��ȯ ����
	if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		return FALSE;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ������� ������ ��� �ִ� ĳ�� ��ȯ ����
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
	{
		if(g_IllusionTempleEvent.CheckRelics(lpObj->m_Index))
		{
			return FALSE;
		}
	}
#endif

	return TRUE;
}
// 1. ���� ������ üũ
BOOL CMoveCommand::CheckEquipmentToMove( LPOBJECTSTRUCT lpObj, INT iTargetMapNumber )
{
	// ��Ʋ���� �Ǵ� õ���ʿ����� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
	// ��Ʋ������ �̵� �Ұ�� ���ϸ��Ƹ� Ÿ�� �̵��Ҽ� ����
	if( iTargetMapNumber == MAP_INDEX_ATHLANSE )	
	{	
		if( lpObj->pInventory[EQUIPMENT_HELPER].IsItem())
		{							
			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) )
			{
				return FALSE;
			}

			if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
			{
				return FALSE;
			}
		}
	}

	if( iTargetMapNumber == MAP_INDEX_ICARUS )
	{
		if( (lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,3) && 
			 lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(13,30) &&	// ����
#ifdef ADD_ITEM_FENRIR_01_20051110
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13,37) &&	// �渱
#endif
#ifdef MODIFY_RECALL_PARTY_SKILL_SUMMONER_BUGFIX_20080829
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 36) &&	// ��ǳ�� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 37) &&	// �ð��� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 38) &&	// ȯ���� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 39) &&	// �ĸ��� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 40) &&	// ������ ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 41) &&	// ��ȯ ���� 1�� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 42) &&	// ��ȯ ���� 2�� ����
			lpObj->pInventory[EQUIPMENT_WING].m_Type != MAKE_ITEMNUM(12, 43) &&	// ��ȯ ���� 3�� ����
			lpObj->pInventory[EQUIPMENT_HELPER].m_Type != MAKE_ITEMNUM(13, 4) &&	// ��ũ ȣ��
#endif // MODIFY_RECALL_PARTY_SKILL_SUMMONER_BUGFIX_20080829
			(lpObj->pInventory[EQUIPMENT_WING].m_Type < MAKE_ITEMNUM(12,0) || lpObj->pInventory[EQUIPMENT_WING].m_Type > MAKE_ITEMNUM(12,6) )) ||
			 lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,2) ||
			 lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) ||
			 lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) 
		  )
		{	
			return FALSE;
		}

		// ��ȯ ���� ����
		if( lpObj->m_RecallMon >= 0 ) 
		{
			GCRecallMonLife(lpObj->m_Index, 60, 0);
			gObjMonsterCallKill(lpObj->m_Index);
		}
	}

	return TRUE;
}
// 2. �������̽� üũ
BOOL CMoveCommand::CheckInterfaceToMove( LPOBJECTSTRUCT lpObj )
{
	// �������̽��� ��� ���̸�..
	if( lpObj->m_IfState.use == 1 )
		return FALSE;

#ifdef PERSONAL_SHOP_20040113
	if( lpObj->m_bPShopOpen == TRUE )
		return FALSE;
#endif
	
	return TRUE;
}
#endif

int	CMoveCommand::FindIndex(char* mapname)
{
	for( int i = 0; i < MAX_MOVE_COMMAND_COUNT; i++ )
	{
		if( m_MoveCommandData[i].Index == -1 )
			return -1;

		if( _stricmp(mapname, m_MoveCommandData[i].Name ) == 0)
		{
			return i;
		}
	}

	return -1;
}

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
int CMoveCommand::FindIndex(int iMapIndex)
{
	for( int i = 0; i < MAX_MOVE_COMMAND_COUNT; i++ )
	{
		if( m_MoveCommandData[i].Index == -1 )
		{
			return -1;
		}
		
		if( m_MoveCommandData[i].Index == iMapIndex )
		{
			return i;
		}
	}
	
	return -1;
}
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221
BOOL CMoveCommand::CheckMoveMapBound(int iMapIndex)
{
	switch( iMapIndex )
	{
	case 1:		// �η��þ� (1)
	case 2:		// �븮�� (2)
//����3�߰��� ����ƽ� ���� �ε����� 3��ŭ �и�
//movereq�� index �������� �ϸ� ���� �ڵ� ������ ���ص� �ɰ� ������
//���� �ý����� ���� �ε����� �����- ���� movereq�� index�� ������ ���� (riverstyx)
#ifdef MODIFY_MOVECOMMAND_20080415
	case 3:		// ��������
	case 6:		// ����ƽ�
	case 10:	// ���� 1
	case 16:	// �ν�Ʈ Ÿ�� 1
#else
	case 3:		// ����ƽ�
	case 7:		// ���� 1
	case 13:	// �ν�Ʈ Ÿ�� 1
#endif // MODIFY_MOVECOMMAND_20080415
		return TRUE;
	default:
		return FALSE;
	}

	return TRUE;
}
#endif // ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
BOOL CMoveCommand::Move(LPOBJECTSTRUCT lpObj, int iMapIndex)
#else
BOOL CMoveCommand::Move(LPOBJECTSTRUCT lpObj, char* mapname)
#endif // ADD_MAPMOVE_PROTOCOL_20090327
{
	int	m_number   = -1;
	int	overLevel  = 0;
	int	subMoney   = 2000;
	
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	char mapname[255];
	int index = FindIndex( iMapIndex );
	if( index == -1 )
	{
		LogAddTD( "[MapMove Failed] Invalid Map Index (%s/%s) (Index : %d)", lpObj->AccountID, lpObj->Name, iMapIndex );
		return FALSE;
	}
	memcpy( mapname, m_MoveCommandData[index].Name, 255 );
#else
	int index = FindIndex(mapname);
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221
	if( g_bUseMoveMapBound == TRUE )
	{
	#ifdef MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
		if( g_CashItemPeriodSystem.CheckMapBoundFreeItemUse( lpObj ) == FALSE )
	#else
		if( lpObj->m_iPeriodItemEffectIndex == -1 )
	#endif // MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
		{
			if( CheckMoveMapBound( index ) == FALSE )
			{
				return FALSE;
			}
		}
	}
#endif // ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221

#ifdef MODIFY_MOVEMAP_BUGFIX_20080109	// movereq������ �ε����� ����
	int nMoveIndex = m_MoveCommandData[index].Index;
#endif	// MODIFY_MOVEMAP_BUGFIX_20080109

#ifdef ADD_NEWPVP_PKFIELD
	// NON-PK������ ������ ��ī�������� ������ �̵� �Ұ� VULCANUS_MOVEREQINDEX	42
	if (nMoveIndex == 42)
	{
		if (gNonPK)
		{
			// "NON-PK ���������� �̵��� �� �����ϴ�."
			GCServerMsgStringSend(lMsg.Get(3418), lpObj->m_Index, 1);
			return FALSE;
		}
	}
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130				// ������ �Ⱓ �߿��� �������̶� �̵�������� �η��������� �� �� ����.
#ifdef MODIFY_MOVEMAP_BUGFIX_20080109	// movereq������ �ε����� ����
	if (nMoveIndex == 24) {
#else	// MODIFY_MOVEMAP_BUGFIX_20080109
	if (index == 23) {									// CS_MOVENUM_LORENDEEP
#endif	// MODIFY_MOVEMAP_BUGFIX_20080109
		if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_SYNC_STATE_STARTSIEGE) {
			return FALSE;
		}

		if (g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == FALSE &&
			g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE
			) 
		{
			return FALSE;
		}
	}
#endif
	
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013	// ��������� '/�̵� ��Ʋ����, /�̵� ��ī�罺'
	if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
#ifdef MODIFY_MOVEMAP_BUGFIX_20080109
		&& (nMoveIndex == 11 || nMoveIndex == 12 || nMoveIndex == 13		// ��Ʋ����
		|| nMoveIndex == 23))												// ��ī�罺
#else	// MODIFY_MOVEMAP_BUGFIX_20080109
		&& (index == 10 || index == 11 || index == 12		// ��Ʋ����
		|| index == 25))									// ��ī�罺
#endif	// MODIFY_MOVEMAP_BUGFIX_20080109
	{
		return FALSE;
	}
#endif
	
#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	// PC�� ������ �̵� ����

	#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
		// 20060918 ���� ( Į���� ���� �̵� ���� : PC�� ���� ����, �Ķ� ���ָӴ� ��� ���� )
		if( lpObj->m_bPCBangCouponUser || lpObj->m_lFreeKalimaTime )
	#else
		if( lpObj->m_lFreeKalimaTime )
	#endif
		{
			if( MoveFreeKalima( lpObj, mapname ) )
				return TRUE;
		}

	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	if( strcmp( mapname, lMsg.Get(3346) ) == 0 )			// "Į����"
	{
		GCServerMsgStringSend(lMsg.Get(1256), lpObj->m_Index, 1);
	}
#endif // PCBANG_FREE_KALIMA_EVENT_20060124

	if( index == -1 )
	{

#if TESTSERVER == 1	// �׼��� �̵�
		return TestServerMove(lpObj, mapname);
#else
		return FALSE;		// �ش�Ǵ� �̵� �����Ͱ� ����
#endif
	}


	int	NeedLevel = m_MoveCommandData[index].NeedLevel;
	int	NeedZen = m_MoveCommandData[index].NeedZen;
	int	MapNumber = m_MoveCommandData[index].MapNumber;

#if TESTSERVER == 1
	//NeedLevel = 0;
#endif

#ifdef DARKLORD_WORK
	if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else					
	if( lpObj->Class == CLASS_MAGUMSA )	
#endif
	{	
		if( NeedLevel > 0 )
		{
#ifdef MODIFY_MAGUMSA_MOVE_LEVEL_20040517
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// ����� ���� ���˻�, ��ũ�ε�� Movereq.txt ������ ���� ����
			//if(GateNumber != 273)
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
			NeedLevel = ((NeedLevel*2)/3);
#else
			NeedLevel = ((NeedLevel/3)*2);
#endif			
		}
	}

	if( g_TerrainManager.CheckTerrain(MapNumber) == true )
	{
		LogAddTD("[%s][%s] Use Move [%d][%d][%d] - [%d][%d][%d]", 
			lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->m_AccountType, lpObj->m_iResetNumber,
			NeedLevel, m_MoveCommandData[index].VIP, m_MoveCommandData[index].Reset); 
		if( lpObj->Level >= NeedLevel )
		{
			if(lpObj->m_AccountType < m_MoveCommandData[index].VIP)
			{
				char szTemp[256];
				wsprintf(szTemp, "Necesitas ser minimo nivel VIP %d", m_MoveCommandData[index].VIP);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			}
			else if(lpObj->m_iResetNumber < m_MoveCommandData[index].Reset)
			{
				char szTemp[256];
				wsprintf(szTemp, "Necesitas minimo tener %d Resets", m_MoveCommandData[index].Reset);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			}
			else if( lpObj->Money-NeedZen >= 0 ) 
			{
				if( gObjMovePlayer(lpObj->m_Index, m_MoveCommandData[index].MapNumber, m_MoveCommandData[index].X1, m_MoveCommandData[index].Y1, m_MoveCommandData[index].X2, m_MoveCommandData[index].Y2) )
				{
#ifdef PERSONAL_SHOP_20040113	// ���� �ʿ� �̵��� ��츦 ����Ͽ� ���λ��� ����Ʈ�� ������ �ٽ� �׸���� ����
					lpObj->m_bPShopRedrawAbs = true;		
#endif
					lpObj->Money -= NeedZen;
					
					LogAddTD("[%s][%s] Use [%s %s] Success : reduces money %d - %d", 
							lpObj->AccountID, lpObj->Name, 
							lMsg.Get(3001), mapname,
							lpObj->Money, NeedLevel); 
					GCMoneySend(lpObj->m_Index, lpObj->Money);

#ifdef ADD_RAKLION_20080408		// ��Ŭ���� ���������� �̵��� ó�� �߰�
					if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
						g_RaklionBattleUserMng.DeleteUserData( lpObj->m_Index );
#endif // ADD_RAKLION_20080408

					return TRUE;
				}				
			}	
			else
			{
				GCServerMsgStringSend(lMsg.Get(1126), lpObj->m_Index, 1);
			}
		}
		else
		{	// %d ���� �̻� �̵� �Ҽ� �ִ� �� �Դϴ�
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(1117), NeedLevel);
			GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
		}
	}

	//"[%s][%s] �̵� ��ɾ� ����
	LogAddTD("[%s][%s] Use [%s %s] Fail MapNumber = %d", 
			 lpObj->AccountID, lpObj->Name, 
			 lMsg.Get(3001), mapname,
			 MapNumber); 
	return FALSE;
}

#if TESTSERVER == 1
BOOL CMoveCommand::TestServerMove(LPOBJECTSTRUCT lpObj, char* mapname)
{
	int m_number=-1, overLevel=0, subMoney=0;
#ifdef FOR_BLOODCASTLE
	if( strcmp(mapname, "����1") == 0 )			// ����ĳ��1
	{
		m_number = 66;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 0;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 11, 0);
	}
	else if( strcmp(mapname, "����2") == 0 )			// ����ĳ��2
	{
		m_number = 67;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 1;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 12, 0);
	}
	else if( strcmp(mapname, "����3") == 0 )			// ����ĳ��3
	{
		m_number = 68;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 2;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 13, 0);
	}
	else if( strcmp(mapname, "����4") == 0 )			// ����ĳ��4
	{
		m_number = 69;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 3;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 14, 0);
	}
	else if( strcmp(mapname, "����5") == 0 )			// ����ĳ��5
	{
		m_number = 70;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 4;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 15, 0);
	}
	else if( strcmp(mapname, "����6") == 0 )			// ����ĳ��6
	{
		m_number = 71;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 5;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 16, 0);
	}
#ifdef BLOODCASTLE_EXTEND_20040314			// ����ĳ�� Ȯ������ ���� /�̵� ���� �� �� �ִ� �� �߰�
	else if( strcmp(mapname, "����7") == 0 )			// ����ĳ��6
	{
		m_number = 80;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 6;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 17, 0);
	}
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010			// /�̵� ����8 �߰�
	else if( strcmp(mapname, "����8") == 0 )			// ����ĳ��8
	{
		m_number = 271;
		subMoney = 0;

		lpObj->m_cBloodCastleIndex = 7;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;

		overLevel = GetMapMoveLevel(lpObj, 18, 0);
	}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408
	else if( strcmp(mapname, "ī����1") == 0 )			// ī���� ĳ��1
	{
		m_number = GATE_CHAOSCASTLE_1;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 18, 0);
	}
	else if( strcmp(mapname, "ī����2") == 0 )			// ī���� ĳ��2
	{
		m_number = GATE_CHAOSCASTLE_2;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 19, 0);
	}
	else if( strcmp(mapname, "ī����3") == 0 )			// ī���� ĳ��3
	{
		m_number = GATE_CHAOSCASTLE_3;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 20, 0);
	}
	else if( strcmp(mapname, "ī����4") == 0 )			// ī���� ĳ��4
	{
		m_number = GATE_CHAOSCASTLE_4;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 21, 0);
	}
	else if( strcmp(mapname, "ī����5") == 0 )			// ī���� ĳ��5
	{
		m_number = GATE_CHAOSCASTLE_5;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 22, 0);
	}
	else if( strcmp(mapname, "ī����6") == 0 )			// ī���� ĳ��6
	{
		m_number = GATE_CHAOSCASTLE_6;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 23, 0);
	}
#endif	
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// /�̵� ī����7 
	else if( strcmp(mapname, "ī����7") == 0 )			// ī���� ĳ��7
	{
		m_number = GATE_CHAOSCASTLE_7;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 53, 0);
	}
#endif	
#ifdef ADD_NEW_MAP_KALIMA_20040518									// Į���� '/�̵�' ��� �� ó��
	else if( strcmp(mapname, "Į����1") == 0 )			// Į���� 1
	{
		m_number = 88;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 24, 0);
	}
	else if( strcmp(mapname, "Į����2") == 0 )			// Į���� 2
	{
		m_number = 89;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 25, 0);
	}
	else if( strcmp(mapname, "Į����3") == 0 )			// Į���� 3
	{
		m_number = 90;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 26, 0);
	}
	else if( strcmp(mapname, "Į����4") == 0 )			// Į���� 4
	{
		m_number = 91;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 27, 0);
	}
	else if( strcmp(mapname, "Į����5") == 0 )			// Į���� 5
	{
		m_number = 92;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 28, 0);
	}
	else if( strcmp(mapname, "Į����6") == 0 )			// Į���� 6
	{
		m_number = 93;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 29, 0);
	}
#ifdef HIDDEN_KALIMA_20050706
	else if( strcmp(mapname, "Į����7") == 0 )			// Į���� 7
	{
		m_number = 116;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 29, 0);
	}
#endif
#endif
#ifdef ADD_NEW_MAP_CASTLE_20041115
	else if( strcmp(mapname, "����") == 0 )				// ����
	{
		m_number = 94;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 30, 0);
	}
	else if( strcmp(mapname, "�η�����") == 0 )			// ���� (������ ������ ���)
	{
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130				// ������ �Ⱓ �߿��� �������̶� �̵�������� �η��������� �� �� ����.
		if (g_CastleSiegeSync.GetCastleState() != CASTLESIEGE_SYNC_STATE_STARTSIEGE) {
			if (g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE ||
				g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE
				) 
			{
#endif
		m_number = 101;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 30, 0);
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
			}
		}
#endif
	}
#endif
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
	
#if TESTSERVER == 1
	else if( strcmp(mapname, "�����") == 0 )			// �� ����� ��
	{
		m_number = 95;
		subMoney = 0;
		overLevel = GetMapMoveLevel(lpObj, 31, 0);
	}
#endif
	
#ifdef ADD_NEW_MAP_AIDA_20050617
	else if( strcmp(mapname, "���̴�") == 0 )			// ���̴� ��
	{
		m_number = 119;
		subMoney = 0;

		overLevel = GetMapMoveLevel(lpObj, MAP_INDEX_AIDA, 0);
	}
#endif

#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
	else if( strcmp(mapname, "����1") == 0 )			// ũ���̿��� 1�� ��
	{
		m_number = 118;
		subMoney = 0;

		overLevel = GetMapMoveLevel(lpObj, MAP_INDEX_CRYWOLF_FIRSTZONE, 0);
	}
#endif

#ifdef ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
	else if( strcmp(mapname, "����2") == 0 )			// ũ���̿��� 2�� ��
	{
		m_number = 121;
		subMoney = 0;

		overLevel = GetMapMoveLevel(lpObj, MAP_INDEX_CRYWOLF_SECONDZONE, 0);
	}
#endif

#endif
	else if( strcmp(mapname, "����1") == 0 )			// �Ǹ��� ����
	{
		m_number = 58;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 0;

		overLevel = GetMapMoveLevel(lpObj, 9, 20);
	}
	else if( strcmp(mapname, "����2") == 0 )	// �Ǹ��� ����
	{
		m_number = 59;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 1;

		overLevel = GetMapMoveLevel(lpObj, 9, 20);
	}
	else if( strcmp(mapname, "����3") == 0 )	// �Ǹ��� ����
	{
		m_number = 60;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 2;

		overLevel = GetMapMoveLevel(lpObj, 9, 20);
	}
	else if( strcmp(mapname, "����4") == 0 )	// �Ǹ��� ����
	{
		m_number = 61;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 3;

		overLevel = GetMapMoveLevel(lpObj, 9, 20);
	}
#ifdef DEVILSQUARE_EXTEND_20050221
	else if( strcmp(mapname, "����5") == 0 )	// �Ǹ��� ����
	{
		m_number = 111;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 4;

		overLevel = GetMapMoveLevel(lpObj, 32, 20);
	}
	else if( strcmp(mapname, "����6") == 0 )	// �Ǹ��� ����
	{
		m_number = 112;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 5;

		overLevel = GetMapMoveLevel(lpObj, 32, 20);
	}
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// /�̵� ����7
	else if( strcmp(mapname, "����7") == 0 )
	{
		m_number = 270;
		subMoney = 0;

		lpObj->m_nEventExp = 0;
		lpObj->m_nEventScore = 0;
		lpObj->m_nEventMoney = 0;
		lpObj->m_bDevilSquareIndex = 6;

		overLevel = GetMapMoveLevel(lpObj, 32, 400);
	}
#endif

#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
#if ( TESTSERVER == 1 )
	else if( strcmp( mapname, "ĭ����2" ) == 0 )
	{
		gObjTeleport( lpObj->m_Index, MAP_INDEX_KANTURU2, 141, 184 );
		return TRUE;
	}
#endif
#endif	// #ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
	else
	{
		return FALSE;
	}
	
	if( m_number >= 0 )
	{
		if( overLevel <= lpObj->Level )
		{							
			if( lpObj->Money-subMoney >= 0 ) 
			{
				if( gObjMoveGate(lpObj->m_Index, m_number) )
				{
					lpObj->Money -= subMoney;
					LogAddTD("[%s][%s] Instant move command reduces money %d - %d [%d]", 
							lpObj->AccountID, lpObj->Name, lpObj->Money, subMoney,
							lpObj->MapNumber); //"[%s][%s] �̵� ��ɾ� ��� �� ���� %d - %d"
					GCMoneySend(lpObj->m_Index, lpObj->Money);
				}
				else
				{
					LogAddTD("[%s][%s] Instant move command fail m_number = %d", lpObj->AccountID, lpObj->Name, m_number); //"[%s][%s] �̵� ��ɾ� ����
				}
			}
			else
			{
				GCServerMsgStringSend(lMsg.Get(1126), lpObj->m_Index, 1);
			}
		}
		else 
		{
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(1117), overLevel);
			GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			return 0;
		}
	}

	return TRUE;
}

#endif

// happycat@20050201
/*
BOOL	CMoveCommand::MoveFree2Kalima(LPOBJECTSTRUCT lpObj, char * szpMapName)
{
	// Just only for COREA!!
	const int	iNumKalima = 6;
	const char * MapNameTable[iNumKalima] = { "Į����1", "Į����2", "Į����3", "Į����4", "Į����5", "Į����6" };

	if( iOverLevel == iMoveMapLevel )
	
	int iMoveLevel = 0, iGateNumber = 88;
	for (int  i = 0; i < iNumKalima; i++ )
	{
		if (strcmp("Į����", szpMapName) == 0 )
		{
			iMoveLevel	= GetMapMoveLevel(lpObj, 24 + i, 0);
			iGateNumber	+= i;
			break;
		}
	}

	if (iMoveLevel && (iMoveLevel <= lpObj->Level))
	{
		return gObjMoveGate(lpObj->m_Index, iGateNumber);		
	}
	

	return FALSE;
}
*///UNHAPPY!!!!CAT!!!

//UNHAPPYCAT
#ifdef ADD_NEW_MAP_KALIMA_20040518
INT	GetKalimaGateLevel( LPOBJECTSTRUCT lpObj )
{
	if (lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD) 
	{
		for (int iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) 
		{
			if ( (lpObj->Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND_MAGUMSA) && (lpObj->Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND_MAGUMSA))
				return iLevel;
		}
	}
	else 
	{
		for (int iLevel = 0 ; iLevel < MAX_KALIMAGATE_COUNT ; iLevel++) 
		{
			if ( (lpObj->Level >= g_sttKALIMAGATE_LEVEL[iLevel].iLOWER_BOUND) && (lpObj->Level <= g_sttKALIMAGATE_LEVEL[iLevel].iUPPER_BOUND))
				return iLevel;
		}
	}
	
	return -1;
}

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
BOOL	CMoveCommand::MoveFreeKalima(LPOBJECTSTRUCT lpObj, char* mapname)
{
	int iKalimaGateLevel = GetKalimaGateLevel( lpObj );
#ifdef MODIFY_MAP_MOVE_LEVEL_MESSAGE_SEND_20060830
	int selectedKalimaLevel = -1;
#else
	int selectedKalimaLevel = 0;
#endif

	int m_number=-1;
	// MODIFY_LOCALIZING_WTF_20060309 ���ö���¡ wtf ���� ������Ʈ 0.99.60 - 0.99.94
	if( strcmp(mapname, lMsg.Get(3347) ) == 0 )			// "Į����1"
	{
		m_number = GATE_INDEX_KALIMA_1;
		selectedKalimaLevel = 0;
	}
	else if( strcmp(mapname, lMsg.Get(3348) ) == 0 )	// "Į����2"
	{
		m_number = GATE_INDEX_KALIMA_2;
		selectedKalimaLevel = 1;
	}
	else if( strcmp(mapname, lMsg.Get(3349) ) == 0 )	// "Į����3"
	{
		m_number = GATE_INDEX_KALIMA_3;
		selectedKalimaLevel = 2;
	}
	else if( strcmp(mapname, lMsg.Get(3350) ) == 0 )	// "Į����4"
	{
		m_number = GATE_INDEX_KALIMA_4;
		selectedKalimaLevel = 3;
	}
	else if( strcmp(mapname, lMsg.Get(3351) ) == 0 )	// "Į����5"
	{
		m_number = GATE_INDEX_KALIMA_5;
		selectedKalimaLevel = 4;
	}
	else if( strcmp(mapname, lMsg.Get(3352) ) == 0 )	// "Į����6"
	{
		m_number = GATE_INDEX_KALIMA_6;
		selectedKalimaLevel = 5;
	}
#ifdef HIDDEN_KALIMA_20050706
	else if( strcmp(mapname, lMsg.Get(3353) ) == 0 )	// "Į����7"
	{
		m_number = GATE_INDEX_KALIMA_7;
		selectedKalimaLevel = 6;
	}
#endif // HIDDEN_KALIMA_20050706
	
#ifdef MODIFY_MAP_MOVE_LEVEL_MESSAGE_SEND_20060830
	if( selectedKalimaLevel == -1 )
	{
		return FALSE;
	}
#endif

	if( selectedKalimaLevel > iKalimaGateLevel )
	{
		char szTemp[256];
		if( lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD )
		{
			wsprintf( szTemp, lMsg.Get(1117), g_sttKALIMAGATE_LEVEL[selectedKalimaLevel].iLOWER_BOUND_MAGUMSA );
		}	
		else
		{
			wsprintf( szTemp, lMsg.Get(1117), g_sttKALIMAGATE_LEVEL[selectedKalimaLevel].iLOWER_BOUND );
		}
		GCServerMsgStringSend( szTemp, lpObj->m_Index, 1);
		return FALSE;
	}

	if( m_number >= 0 )
	{
#ifdef MODIFY_KALIMAMOVE_BUGFIX_20060221
		if( gObjMoveGate(lpObj->m_Index, m_number) )
		{
			LogAddTD("[Coupon Event][Free Kalima Move][%s][%s] Instant move command [%d]->[%d]", 
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, m_number);

			return TRUE;
		}
		else
		{
			LogAddTD("[Coupon Event][Free Kalima Move][%s][%s] Instant move command fail m_number = %d", lpObj->AccountID, lpObj->Name, m_number); //"[%s][%s] �̵� ��ɾ� ����
		}
		
#else
		if( CHECK_KALIMAMAP( m_number ) )
		{
			if( gObjMoveGate(lpObj->m_Index, m_number) )
			{
				LogAddTD("[Coupon Event][Free Kalima Move][%s][%s] Instant move command [%d]->[%d]", 
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, m_number);

				return TRUE;
			}
			else
			{
				LogAddTD("[Coupon Event][Free Kalima Move][%s][%s] Instant move command fail m_number = %d", lpObj->AccountID, lpObj->Name, m_number); //"[%s][%s] �̵� ��ɾ� ����
			}
		}
#endif
	}
	return FALSE;
}
#endif // PCBANG_FREE_KALIMA_EVENT_20060124

// happycat@20050201
BOOL	CMoveCommand::MoveFree2Kalima(LPOBJECTSTRUCT lpObj)
{
	int iIdx = GetKalimaGateLevel( lpObj );	

#ifndef PCBANG_FREE_KALIMA_EVENT_20060124

#ifdef HIDDEN_KALIMA_20050706
	
#ifdef MODIFY_KALIMAMOVE_COMMAND_FOR_BLUEPOUCH_20050811
	// ���� Į������ ������.
	if( iIdx >= MAX_KALIMAGATE_COUNT - 1 )
	{
		// �ٷ� �Ʒ� �ܰ� Į����(6Į������ �̵�)
		iIdx = MAX_KALIMAGATE_COUNT - 2;
	}
#endif // MODIFY_KALIMAMOVE_COMMAND_FOR_BLUEPOUCH_20050811

#endif // HIDDEN_KALIMA_20050706

#endif // PCBANG_FREE_KALIMA_EVENT_20060124
	
	if (iIdx < 0) return FALSE;
	
	int iGateNumber = 88 + iIdx;	

	return gObjMoveGate(lpObj->m_Index, iGateNumber);	
}
#endif

#ifdef ADD_NEWPVP_PKFIELD
LPMOVE_COMMAND_DATA CMoveCommand::GetMoveCommandData(int nMapIndex)
{
	int nIndex = -1;
	for (int i=0; i<MAX_MOVE_COMMAND_COUNT; ++i)
	{
		if (m_MoveCommandData[i].Index == nMapIndex)
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex <0)
		return NULL;

	return &m_MoveCommandData[nIndex];
}
#endif // ADD_NEWPVP_PKFIELD