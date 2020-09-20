// Mercenary.cpp: implementation of the Mercenary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mercenary.h"
#include "Math.h"
#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214

CMercenary	g_CsNPC_Mercenary;


#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "Gamemain.h"
#include "MapClass.h"
#include "MonsterAttr.h"
extern	CMonsterAttr	gMAttr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMercenary::CMercenary()
{
	m_iMercenaryCount = 0;
}

CMercenary::~CMercenary()
{

}

BOOL CMercenary::CreateMercenary( INT iIndex, INT iMercenaryTypeIndex, UCHAR cTX, UCHAR cTY )
{

	LPOBJECTSTRUCT lpObj	= &gObj[iIndex];
	INT iMonsterIndex		= -1;

	UCHAR cX = cTX;
	UCHAR cY = cTY;

	// 1. �� �Ӽ� �˻�
	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);
	if( btMapAttr != MAP_ATTR_NONE ) 
	{
		
	}

	// 2. �� �˻�
	if( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
	{	
		MsgOutput(iIndex, lMsg.Get(1627) );	// "���� ���� �ƴմϴ�." 
		return FALSE;
	}

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	// 5. ���� ���� �ð����� Ȯ��.
	if( g_CastleSiegeSync.GetCastleState() != CASTLESIEGE_SYNC_STATE_STARTSIEGE )
	{
		MsgOutput(iIndex, lMsg.Get(1630) ); // "�뺴�� ���� ���� �� ��ȯ�� �� �ֽ��ϴ�."
		return FALSE;
	}

	if( iMercenaryTypeIndex == NPC_INDEX_BOWMAN || iMercenaryTypeIndex == NPC_INDEX_SPEARMAN )
	{
		// 3. ĳ�� ���� ���̵� �˻�. �������� ��밡��.
		if( lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND )
		{
			MsgOutput(iIndex, lMsg.Get(1628) ); // "�������� ��ȯ�����մϴ�."
			return FALSE;
		}

		// 4. ��å �˻�.
		if( lpObj->GuildStatus!=G_MASTER && lpObj->GuildStatus!= G_SUB_MASTER )
		{
			MsgOutput(iIndex, lMsg.Get(1629) );//"�뺴�� �渶,�α渶�� ��ȯ�����մϴ�." 
			return FALSE;
		}
	}
	
	// 6. ���� �߿� �����ɼ� �ִ� �뺴�� ���� ���ѵǾ� �ִ�.
	if( m_iMercenaryCount > MAX_MERCENARY_NUMBER )
	{
		MsgOutput(iIndex, lMsg.Get(1631) ); // "�ʿ� ������ �� �ִ� �ִ� �뺴���� �Ѿ����ϴ�."
		return FALSE;
	}
	
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
	iMonsterIndex = gObjAddMonster(lpObj->MapNumber);
#else
	iMonsterIndex = gObjAdd();
#endif

	if( iMonsterIndex >= 0 )
	{
		LPMONSTER_ATTRIBUTE		lpMATTR;
		lpMATTR = gMAttr.GetAttr(iMercenaryTypeIndex);
		
		if (lpMATTR == NULL) 
		{
			gObjDel (iMonsterIndex);
			return FALSE;
		}
		
		gObjSetMonster(iMonsterIndex, iMercenaryTypeIndex);
		
		gObj[iMonsterIndex].Live						= 1;
		gObj[iMonsterIndex].Life						= lpMATTR->m_Hp;
		gObj[iMonsterIndex].MaxLife						= lpMATTR->m_Hp;
		
		gObj[iMonsterIndex].m_PosNum					= -1;
		gObj[iMonsterIndex].X							= cX;
		gObj[iMonsterIndex].Y							= cY;
		gObj[iMonsterIndex].MTX							= cX;
		gObj[iMonsterIndex].MTY							= cY;
		gObj[iMonsterIndex].TX							= cX;
		gObj[iMonsterIndex].TY							= cY;
		gObj[iMonsterIndex].m_OldX						= cX;
		gObj[iMonsterIndex].m_OldY						= cY;
		gObj[iMonsterIndex].StartX						= cX;
		gObj[iMonsterIndex].StartY						= cY;
		gObj[iMonsterIndex].MapNumber					= lpObj->MapNumber;
		gObj[iMonsterIndex].m_MoveRange					= 0;
		
		gObj[iMonsterIndex].Level						= lpMATTR->m_Level;
		
		gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;			// ����
		gObj[iMonsterIndex].MaxRegenTime				= 1000;
		gObj[iMonsterIndex].Dir							= 1;						// ������ ȭ���� �������� �Ա��� �׻� �Ʒ��� ���Ѵ�.
	
		gObj[iMonsterIndex].RegenTime					= GetTickCount();	
		
		gObj[iMonsterIndex].m_Attribute					= 0;
		gObj[iMonsterIndex].DieRegen					= 0;
		
		gObj[iMonsterIndex].m_btCsNpcType				= CS_NPC_TYPE_INS_DFN;		// ������ ��
		gObj[iMonsterIndex].m_btCsJoinSide				= CS_SIEGESIDE_DEFEND;

		MsgOutput(iIndex, lMsg.Get(1632) ); // "�뺴��ȯ ����"
		
		m_iMercenaryCount++;

#ifdef EXTEND_LOG_SYSTEM_03_20060816
		if( lpObj->lpGuild != NULL )
		{
			LogAddTD( "[CastleSiege] Mercenary is summoned [%d] - [%d][%d] [%s][%s][%d] - (Guild : %s)",
				iMonsterIndex, iMercenaryTypeIndex, m_iMercenaryCount, lpObj->AccountID, lpObj->Name, lpObj->GuildStatus, lpObj->lpGuild->Name );
		}
		else
		{
			LogAddTD( "[CastleSiege] Mercenary is summoned [%d] - [%d][%d] [%s][%s][%d]",
				iMonsterIndex, iMercenaryTypeIndex, m_iMercenaryCount, lpObj->AccountID, lpObj->Name, lpObj->GuildStatus );
		}
#else
		LogAddTD( "[CastleSiege] Mercenary is summoned [%d] - [%d][%d] [%s][%s][%d]", iMonsterIndex, iMercenaryTypeIndex, m_iMercenaryCount, lpObj->AccountID, lpObj->Name, lpObj->GuildStatus );
#endif // EXTEND_LOG_SYSTEM_03_20060816
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(1633) ); // "�뺴��ȯ ����"
		return FALSE;
	}

#endif

	return TRUE;
}


BOOL CMercenary::DeleteMercenary( INT iIndex )
{
	if( iIndex < 0 || iIndex > MAX_OBJECT-1 ) 
		return FALSE;

	m_iMercenaryCount--;

	LogAddTD( "[CastleSiege] Mercenary is broken [%d] - [%d]", iIndex, m_iMercenaryCount);

	if( m_iMercenaryCount < 0 )
		m_iMercenaryCount = 0;
	
	return TRUE;
}

BOOL CMercenary::SearchEnemy( LPOBJECTSTRUCT lpObj )
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	INT iTargetNumber = -1;
	INT iAttackRange = 0;

	lpObj->TargetNumber = -1;
	
	// ���� ���� ����
	if( lpObj->Class == NPC_INDEX_BOWMAN )
	{
		iAttackRange = 5;
	}
	else if( lpObj->Class == NPC_INDEX_SPEARMAN )
	{
		iAttackRange = 3;
	}
	
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		iTargetNumber = lpObj->VpPlayer2[i].number;
		if( iTargetNumber >= 0 )
		{
			if( gObj[iTargetNumber].Type == OBJTYPE_CHARACTER && gObj[iTargetNumber].Live )
			{
				if( !gObj[iTargetNumber].Teleport )
				{
					// �ڽŰ� ���� ĳ�� ���� ���̵�� �������� �ʴ´�.
					if( gObj[iTargetNumber].m_btCsJoinSide == lpObj->m_btCsJoinSide )
					{
						continue;
					}

					int iTx = lpObj->X-gObj[iTargetNumber].X;
					int iTy = lpObj->Y-gObj[iTargetNumber].Y;

					int iDis = (int)sqrt((double)(iTx*iTx)+(iTy*iTy));
					lpObj->VpPlayer2[i].dis = iDis;
					

					// Y ���� �˻� 
					if( lpObj->Dir == 1 )
					{
						if( abs(iTx) <= 2 )
						{
							INT iMinY = lpObj->Y - iAttackRange;

							if( iMinY <= gObj[iTargetNumber].Y && lpObj->Y >= gObj[iTargetNumber].Y)
							{
								lpObj->TargetNumber = iTargetNumber;
								return TRUE;
							}
						}
					}
					// X ���� �˻�
					if( lpObj->Dir == 3 )
					{
						if ( abs(iTy) <= 2 )
						{
							INT iMinX = lpObj->X - iAttackRange;

							if( iMinX <= gObj[iTargetNumber].X && lpObj->X >= gObj[iTargetNumber].X)
							{
								lpObj->TargetNumber = iTargetNumber;
								return TRUE;
							}	
						}
					}

				}
			}
		}
	}	

#endif
	return FALSE;
}	

VOID CMercenary::MercenaryAct( INT iIndex )
{
	if (!gObjIsConnected(iIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iIndex];

	if( lpObj->VPCount2 < 1 ) return;

	if( SearchEnemy( lpObj ) && lpObj->TargetNumber >= 0 ) 
	{
		// �����Ĵ� �ð��� ���� ������Ŵ
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	
	}
	else 
	{
		// ������ ���Ҷ��� ���� üŷ�ϱ� ����.. movespeed �� ���..
		lpObj->NextActionTime = lpObj->m_MoveSpeed;	
	}
}


#endif