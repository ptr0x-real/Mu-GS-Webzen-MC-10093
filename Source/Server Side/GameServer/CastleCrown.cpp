// CastleCrown.cpp: implementation of the CCastleCrown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleCrown.h"

#ifdef CASTLE_NPC_CROWN_WORK_20041210

CCastleCrown	g_CsNPC_CastleCrown;

#include "CastleSiege.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleCrown::CCastleCrown			()
{

}

CCastleCrown::~CCastleCrown			()
{

}

VOID CCastleCrown::CastleCrownAct	(				// 왕관의 행동방식
	INT iIndex
	)
{
	if (!gObjIsConnected(iIndex))
		return;

	if (gObj[iIndex].Type != OBJTYPE_NPC || gObj[iIndex].Class != CS_NPC_CROWN) {
		return;
	}

	if (!gObjIsConnected(g_CastleSiege.GetCrownUserIndex())) {
		g_CastleSiege.ResetCrownUserIndex();
		g_CastleSiege.SetCrownAccessUserX(0);
		g_CastleSiege.SetCrownAccessUserY(0);
		g_CastleSiege.ResetCrownAccessTickCount();
		return;
	}

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpUserObj	= &gObj[g_CastleSiege.GetCrownUserIndex()];

	if (
		(lpUserObj->MapNumber == MAP_INDEX_CASTLESIEGE) && 
		(lpUserObj->m_btCsJoinSide >= CS_SIEGESIDE_ATTACK) && 
		(lpUserObj->m_bCsGuildInvolved == true)
		) 
	{
		if (lpUserObj->Y == g_CastleSiege.GetCrownAccessUserY() && lpUserObj->X == g_CastleSiege.GetCrownAccessUserX())
		{
			if (gObjIsConnected(g_CastleSiege.GetCrownUserIndex()) &&
				gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1)) &&
				gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2))
				)
			{
				if (
					gObj[g_CastleSiege.GetCrownUserIndex()].m_btCsJoinSide == gObj[g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1)].m_btCsJoinSide &&
					gObj[g_CastleSiege.GetCrownUserIndex()].m_btCsJoinSide == gObj[g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1)].m_btCsJoinSide
					)
				{
					// 조건이 계속 유지 됨
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
					DWORD dwAccessTime = GetTickCount() - g_CastleSiege.GetCrownAccessTickCount();
//					MsgOutput( iIndex, "[직인등록중] 누적시간 : acc(%d) + %d", gObj[iIndex].m_iAccumulatedCrownAccessTime, dwAccessTime );

					LogAddTD( "[CastleSiege] [Reg. Accumulating] Accumulated Crown AccessTime : acc(%d) + %d [%s](%s)(%s)", 
						lpUserObj->m_iAccumulatedCrownAccessTime, dwAccessTime,
						lpUserObj->GuildName, lpUserObj->AccountID, lpUserObj->Name );
#endif

				}
				else 
				{
					GCAnsCsAccessCrownState (lpUserObj->m_Index, 2);
					g_CastleSiege.ResetCrownUserIndex();
					g_CastleSiege.SetCrownAccessUserX(0);
					g_CastleSiege.SetCrownAccessUserY(0);
					g_CastleSiege.ResetCrownAccessTickCount();

					// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
					LogAddTD("[CastleSiege] [%s][%s] Register Castle Crown Canceled (GUILD:%s)",
						lpUserObj->AccountID,
						lpUserObj->Name,
						lpUserObj->GuildName
						);
				}
			}
			else 
			{
				GCAnsCsAccessCrownState (lpUserObj->m_Index, 2);
				g_CastleSiege.ResetCrownUserIndex();
				g_CastleSiege.SetCrownAccessUserX(0);
				g_CastleSiege.SetCrownAccessUserY(0);
				g_CastleSiege.ResetCrownAccessTickCount();
			}
		}
		else 
		{
			GCAnsCsAccessCrownState (lpUserObj->m_Index, 2);
			g_CastleSiege.ResetCrownUserIndex();
			g_CastleSiege.SetCrownAccessUserX(0);
			g_CastleSiege.SetCrownAccessUserY(0);
			g_CastleSiege.ResetCrownAccessTickCount();
		}
	}
	else 
	{
		GCAnsCsAccessCrownState (lpUserObj->m_Index, 2);
		g_CastleSiege.ResetCrownUserIndex();
		g_CastleSiege.SetCrownAccessUserX(0);
		g_CastleSiege.SetCrownAccessUserY(0);
		g_CastleSiege.ResetCrownAccessTickCount();
	}
}


#endif

