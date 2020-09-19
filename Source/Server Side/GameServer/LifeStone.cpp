// LifeStone.cpp: implementation of the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LifeStone.h"

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208

CLifeStone	g_CsNPC_LifeStone;

#include "protocol.h"

#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "Gamemain.h"
#include "MapClass.h"
#include "MonsterAttr.h"
extern	CMonsterAttr	gMAttr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLifeStone::CLifeStone()
{

}

CLifeStone::~CLifeStone()
{

}

BOOL CLifeStone::CreateLifeStone( INT iIndex )
{
//	MsgOutput(iIndex, "라이프스톤을 소환합니다.");
		
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];
	INT iMonsterIndex = -1;

	UCHAR cX = lpObj->X;
	UCHAR cY = lpObj->Y;

	// 공성 시작 시간인지 확인.
	if( g_CastleSiegeSync.GetCastleState() != CASTLESIEGE_SYNC_STATE_STARTSIEGE )
	{
		MsgOutput(iIndex, lMsg.Get(1645) ); // "라이프스톤은 공성 시작 후 소환할 수 있습니다."
		return FALSE;
	}

	//길드마스터인지 확인
	if( lpObj->GuildStatus != G_MASTER )
	{
		return FALSE;
	}

	// 공성에 참여하는 길마인지 확인
	if( lpObj->m_btCsJoinSide < CS_SIEGESIDE_ATTACK )
	{
		MsgOutput( iIndex, lMsg.Get(1641) ); // "공성측만 사용할수 있습니다."
		return FALSE;
	}
	
	// 라이프스톤 존재 확인
	if( lpObj->lpGuild->lpLifeStone != NULL )
	{
		MsgOutput( iIndex, lMsg.Get(1642) ); // "이미 라이프스톤을 소환했습니다."
		return FALSE;
	}
	
	// 라이프스톤 사용 횟수 확인
	// 20050303 횟수 제한을 제거
//	if( lpObj->Class == CLASS_DARKLORD )
//	{
//		// 길마인 다크로드는 다른 Limit 설정
//		if( lpObj->m_btLifeStoneCount >= MAX_LIFESTONE_CREATION_LIMIT02 )	
//		{
//			MsgOutput( iIndex, lMsg.Get(1643), lpObj->m_btLifeStoneCount ); // "라이프스톤을 이미 %d회 사용했습니다."
//			return FALSE;
//		}
//	}
//	else
//	{	
//		if( lpObj->m_btLifeStoneCount >= MAX_LIFESTONE_CREATION_LIMIT01 )	
//		{
//			MsgOutput( iIndex, lMsg.Get(1643), lpObj->m_btLifeStoneCount ); // "라이프스톤을 이미 %d회 사용했습니다." 
//			return FALSE;
//		}
//	}
	
	// 용의 탑에서는 사용할 수 없음.
	if( cX > 150 && cX < 210
		&& cY > 175 && cY < 230)
//	if( cX > 160 && cX < 192
//		&& cY > 187 && cY < 217)
//	if( cX > 162 && cX < 189 
//		&& cY > 189 && cY < 215)
	{
		MsgOutput( iIndex, lMsg.Get(1644) ); // "용의탑에서는 라이프스톤을 사용할수 없습니다."
		return FALSE;
	}
	
	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);
	if( btMapAttr != MAP_ATTR_NONE ) 
	{
		
	}
	
	if( gObj[iIndex].MapNumber != MAP_INDEX_CASTLESIEGE )
	{	
		MsgOutput(iIndex, lMsg.Get(1627) ); // "공성 맵이 아닙니다."
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
		lpMATTR = gMAttr.GetAttr(NPC_INDEX_LIFESTONE);
		
		if (lpMATTR == NULL) 
		{
			gObjDel (iMonsterIndex);
			
			return FALSE;
		}
		
		gObjSetMonster(iMonsterIndex, NPC_INDEX_LIFESTONE);
		
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
		
		gObj[iMonsterIndex].Type						= OBJTYPE_MONSTER;	// 몬스터
		gObj[iMonsterIndex].MaxRegenTime				= 1000;
		gObj[iMonsterIndex].Dir							= 1;				// 방향은 화면을 기준으로 입구가 항상 아래로 향한다.
	
		gObj[iMonsterIndex].RegenTime					= GetTickCount();	
		
		gObj[iMonsterIndex].m_Attribute					= 0;
		gObj[iMonsterIndex].DieRegen					= 0;
		
		gObj[iMonsterIndex].m_btCsNpcType				= CS_NPC_TYPE_INS_ATK;		// 공성측 임
		gObj[iMonsterIndex].m_btCsJoinSide				= lpObj->m_btCsJoinSide;	// 주인의 조인사이드 등록
		gObj[iMonsterIndex].lpGuild						= lpObj->lpGuild;			// 주인 길드 등록.
		gObj[iMonsterIndex].m_btCreationState			= 0;						// 생성 상태는 처음은 0
		
		lpObj->lpGuild->lpLifeStone						= &gObj[iMonsterIndex];
		
#ifdef MODIFY_LIFESTONE_BUGFIX_20070426
		// 라이프스톤 생성시간을 초기화한다.
		gObj[iMonsterIndex].m_iCreatedActivationTime	= 0;
#endif

		MsgOutput(iIndex, lMsg.Get(1646) ); // "라이프스톤소환 성공"
		
//#ifdef EXTEND_LOG_SYSTEM_03_20060816
//		LogAddTD( "[CastleSiege] LifeStone is created - [%s] [%s][%s] (Map:%d)(X:%d, Y:%d) - (Guild : %s)",
//			lpObj->lpGuild->Name, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, cX, cY, lpObj->lpGuild->Name );
//#else
		LogAddTD( "[CastleSiege] LifeStone is created - [%s] [%s][%s] (Map:%d)(X:%d, Y:%d)", lpObj->lpGuild->Name, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, cX, cY );
//#endif // EXTEND_LOG_SYSTEM_03_20060816

		lpObj->m_btLifeStoneCount++;
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(1647) ); // "라이프스톤소환 실패"
		return FALSE;
	}

	return TRUE;
}

BOOL CLifeStone::DeleteLifeStone( INT iIndex )
{
	if( iIndex < 0 || iIndex > MAX_OBJECT-1 ) 
		return FALSE;

	// 길드에 등록되어 있는 라이프스톤 해제
	LPOBJECTSTRUCT lpLifeStone = &gObj[iIndex];
	if( lpLifeStone->lpGuild != NULL )
	{
		LogAddTD( "[CastleSiege] LifeStone is broken - [%s]", lpLifeStone->lpGuild->Name );
		lpLifeStone->lpGuild->lpLifeStone = NULL;
	}

	return TRUE;
}

BOOL CLifeStone::SetReSpawnUserXY( INT iUserIndex )
{
	if (!gObjIsConnected(iUserIndex))
		return FALSE;

	LPOBJECTSTRUCT lpUser = &gObj[iUserIndex];
	
	// 1.
	if (lpUser->MapNumber != MAP_INDEX_CASTLESIEGE)
		return FALSE;

	// 2.
	if( lpUser->lpGuild == NULL )
		return FALSE;

	// 3.
	BYTE btCsJoinSide	= lpUser->m_btCsJoinSide;

	// 4.
	if( lpUser->lpGuild->lpLifeStone == NULL )
		return FALSE;

	LPOBJECTSTRUCT lpLifeStone = lpUser->lpGuild->lpLifeStone;

#ifdef MODIFY_LIFESTONE_BUGFIX_20070426
	// 라이프스톤이 생성중일 때에는 처리하지 않는다.
	if( lpLifeStone->m_iCreatedActivationTime < MAX_LIFESTONE_CREATION_TIME )
		return FALSE;
#endif

	// 나의 길드에 라이프스톤이 있으면 라이프 스톤의 좌표로 세팅
	lpUser->MapNumber = MAP_INDEX_CASTLESIEGE;
	lpUser->X = lpLifeStone->X;
	lpUser->Y = lpLifeStone->Y;
	
	return TRUE;
}

VOID CLifeStone::LifeStoneAct ( INT iIndex )
{
	if (!gObjIsConnected(iIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iIndex];
	
	// 1초마다 Act 처리가 되므로 여기서 생성 상태를 체크한다.
	lpObj->m_iCreatedActivationTime++;

	BYTE btCreationState = lpObj->m_btCreationState;

	if( lpObj->m_iCreatedActivationTime < MAX_LIFESTONE_CREATION_TIME )
	{
		
		lpObj->m_btCreationState = lpObj->m_iCreatedActivationTime / (MAX_LIFESTONE_CREATION_TIME / MAX_LIFESTONE_CREATION_STATE); 
	}
	else
	{
		lpObj->m_btCreationState = MAX_LIFESTONE_CREATION_STATE;
	}

	if( btCreationState != lpObj->m_btCreationState )
	{
		// 라이프 스톤의 생성 상태를 보내준다.
		GCSendObjectCreationState( iIndex );
	}

	// 라이프 스톤이 완전히 생성된 상태가 아니면 작동하지 않는다.
	if( lpObj->m_btCreationState < MAX_LIFESTONE_CREATION_STATE )
		return;

	// 뷰포트 상에 아무도 없다면 끝난다.
	if( lpObj->VPCount < 1 ) return;

	// 뷰포트 상에서 해당되는 사람들을 찾는다.
	INT	tObjNum	= -1;
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				if (gObj[tObjNum].m_btCsJoinSide == lpObj->m_btCsJoinSide) {
					if (abs(lpObj->Y - gObj[tObjNum].Y) <= 3 && abs(lpObj->X - gObj[tObjNum].X) <= 3 )
					{
						// 이 사람이 라이프스톤과 같은 길드이고 지정된 반경안에 있다면 여기서 HP, MP, AG를 늘려준다.
						BOOL bLifeChange	= FALSE;
						BOOL bManaChange	= FALSE;
						BOOL bBpChange		= FALSE;

#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].Life < gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife) 
						{
							gObj[tObjNum].Life		+= (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife)/100;
							if (gObj[tObjNum].Life > gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife)
								gObj[tObjNum].Life	= gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife;
							bLifeChange		= TRUE;
						}
#else
						if (gObj[tObjNum].Life < gObj[tObjNum].MaxLife) 
						{
							gObj[tObjNum].Life		+= gObj[tObjNum].MaxLife/100;
							if (gObj[tObjNum].Life > gObj[tObjNum].MaxLife)
								gObj[tObjNum].Life	= gObj[tObjNum].MaxLife;
							bLifeChange		= TRUE;
						}
#endif // MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						
#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].Mana < gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) 
						{
							gObj[tObjNum].Mana		+= (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)/100;
							if (gObj[tObjNum].Mana > gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)
								gObj[tObjNum].Mana	= gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;
							bManaChange		= TRUE;
						}
#else
						if (gObj[tObjNum].Mana < gObj[tObjNum].MaxMana) 
						{
							gObj[tObjNum].Mana		+= gObj[tObjNum].MaxMana/100;
							if (gObj[tObjNum].Mana > gObj[tObjNum].MaxMana)
								gObj[tObjNum].Mana	= gObj[tObjNum].MaxMana;
							bManaChange		= TRUE;
						}
#endif // MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						
#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].BP < gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) 
						{
							gObj[tObjNum].BP		+= (gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP)/100;
							if (gObj[tObjNum].BP > gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP)
								gObj[tObjNum].BP	= gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP;
							bBpChange		= TRUE;
						}
#else
						if (gObj[tObjNum].BP < gObj[tObjNum].MaxBP) 
						{
							gObj[tObjNum].BP		+= gObj[tObjNum].MaxBP/100;
							if (gObj[tObjNum].BP > gObj[tObjNum].MaxBP)
								gObj[tObjNum].BP	= gObj[tObjNum].MaxBP;
							bBpChange		= TRUE;
						}
#endif	// MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609

						if (bLifeChange)
						{
#ifdef ADD_SHIELD_POINT_01_20060403
							GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1, (WORD)gObj[tObjNum].iShield);
#else
							GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1);
#endif
						}

						if (bManaChange || bBpChange)
							GCManaSend(tObjNum, (short)gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
					}
				}
			}
		}
	}
}


#endif

