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

	// 1. 맵 속성 검사
	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);
	if( btMapAttr != MAP_ATTR_NONE ) 
	{
		
	}

	// 2. 맵 검사
	if( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
	{	
		MsgOutput(iIndex, lMsg.Get(1627) );	// "공성 맵이 아닙니다." 
		return FALSE;
	}

#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	// 5. 공성 시작 시간인지 확인.
	if( g_CastleSiegeSync.GetCastleState() != CASTLESIEGE_SYNC_STATE_STARTSIEGE )
	{
		MsgOutput(iIndex, lMsg.Get(1630) ); // "용병은 공성 시작 후 소환할 수 있습니다."
		return FALSE;
	}

	if( iMercenaryTypeIndex == NPC_INDEX_BOWMAN || iMercenaryTypeIndex == NPC_INDEX_SPEARMAN )
	{
		// 3. 캐슬 조인 사이드 검사. 수성측만 사용가능.
		if( lpObj->m_btCsJoinSide != CS_SIEGESIDE_DEFEND )
		{
			MsgOutput(iIndex, lMsg.Get(1628) ); // "수성측만 소환가능합니다."
			return FALSE;
		}

		// 4. 직책 검사.
		if( lpObj->GuildStatus!=G_MASTER && lpObj->GuildStatus!= G_SUB_MASTER )
		{
			MsgOutput(iIndex, lMsg.Get(1629) );//"용병은 길마,부길마만 소환가능합니다." 
			return FALSE;
		}
	}
	
	// 6. 공성 중에 생성될수 있는 용병의 수가 제한되어 있다.
	if( m_iMercenaryCount > MAX_MERCENARY_NUMBER )
	{
		MsgOutput(iIndex, lMsg.Get(1631) ); // "맵에 존재할 수 있는 최대 용병수를 넘었습니다."
		return FALSE;
	}
	
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
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
		
		gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;			// 몬스터
		gObj[iMonsterIndex].MaxRegenTime				= 1000;
		gObj[iMonsterIndex].Dir							= 1;						// 방향은 화면을 기준으로 입구가 항상 아래로 향한다.
	
		gObj[iMonsterIndex].RegenTime					= GetTickCount();	
		
		gObj[iMonsterIndex].m_Attribute					= 0;
		gObj[iMonsterIndex].DieRegen					= 0;
		
		gObj[iMonsterIndex].m_btCsNpcType				= CS_NPC_TYPE_INS_DFN;		// 수성측 임
		gObj[iMonsterIndex].m_btCsJoinSide				= CS_SIEGESIDE_DEFEND;

		MsgOutput(iIndex, lMsg.Get(1632) ); // "용병소환 성공"
		
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
		MsgOutput(iIndex, lMsg.Get(1633) ); // "용병소환 실패"
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
	
	// 공격 범위 선택
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
					// 자신과 같은 캐슬 조인 사이드는 공격하지 않는다.
					if( gObj[iTargetNumber].m_btCsJoinSide == lpObj->m_btCsJoinSide )
					{
						continue;
					}

					int iTx = lpObj->X-gObj[iTargetNumber].X;
					int iTy = lpObj->Y-gObj[iTargetNumber].Y;

					int iDis = (int)sqrt((double)(iTx*iTx)+(iTy*iTy));
					lpObj->VpPlayer2[i].dis = iDis;
					

					// Y 방향 검사 
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
					// X 방향 검사
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
		// 공격후는 시간을 좀더 지연시킴
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	
	}
	else 
	{
		// 공격을 안할때는 빨리 체킹하기 위해.. movespeed 를 사용..
		lpObj->NextActionTime = lpObj->m_MoveSpeed;	
	}
}


#endif