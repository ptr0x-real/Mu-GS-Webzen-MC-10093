// CastleCrownSwitch.cpp: implementation of the CCastleCrownSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleCrownSwitch.h"

#ifdef CASTLE_NPC_CROWN_SWITCH_WORK_20041210

CCastleCrownSwitch	g_CsNPC_CastleCrownSwitch;

#include "CastleSiege.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleCrownSwitch::CCastleCrownSwitch			()
{

}

CCastleCrownSwitch::~CCastleCrownSwitch			()
{

}

VOID CCastleCrownSwitch::CastleCrownSwitchAct	(			// 왕관 스위치의 행동방식
	INT iIndex
	)
{
	if (!gObjIsConnected(iIndex))
		return;

	if (gObj[iIndex].Type != OBJTYPE_NPC || !CHECK_NPC_CROWN_SWITCH(gObj[iIndex].Class)) {
		return;
	}

	if (!gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(gObj[iIndex].Class))) {
		g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);
		if (g_CastleSiege.GetRegCrownAvailable() == TRUE) {
			g_CastleSiege.SetRegCrownAvailable(FALSE);
			g_CastleSiege.NotifyCrownState(1);
		}
		return;
	}

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpUserObj	= &gObj[g_CastleSiege.GetCrownSwitchUserIndex(gObj[iIndex].Class)];


#ifdef MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017
	// 왕관 스위치를 보고 있는 유저에게 정보를 보내준다.
	// 길드이름 / 유저이름
	// PMSG_ANS_NOTIFY_CROWNSWITCH_INFO 프로토콜을 보냄..
	g_CastleSiege.NotifyCrownSwitchInfo( iIndex );

#endif


	if (
		(lpUserObj->MapNumber == MAP_INDEX_CASTLESIEGE) 
		&& (lpUserObj->m_btCsJoinSide >= CS_SIEGESIDE_ATTACK) 
//		&& (lpUserObj->m_bCsGuildInvolved == true)
		) 
	{
		if (abs(lpObj->Y - lpUserObj->Y) <= 3 && abs(lpObj->X - lpUserObj->X) <= 3 )
		{
			// 조건이 계속 유지 됨
			INT iIDX1	= g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH1);
			INT iIDX2	= g_CastleSiege.GetCrownSwitchUserIndex(CS_NPC_CROWN_SWITCH2);
			if (gObjIsConnected(iIDX1) && 
				gObjIsConnected(iIDX2) &&
				(gObj[iIDX1].m_btCsJoinSide == gObj[iIDX2].m_btCsJoinSide)
				) 
			{
				if (g_CastleSiege.GetRegCrownAvailable() == FALSE) {
					// 왕관을 등록할 수 있는 상태이다.
					g_CastleSiege.SetRegCrownAvailable(TRUE);
					g_CastleSiege.NotifyCrownState(0);
				}
			}
		}
		else {
			GCAnsCsAccessSwitchState (lpUserObj->m_Index, lpObj->m_Index, -1, 0);
			g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);
			if (g_CastleSiege.GetRegCrownAvailable() == TRUE) {
				g_CastleSiege.SetRegCrownAvailable(FALSE);
				g_CastleSiege.NotifyCrownState(1);
			}
		
			// #LOGADD_CASTLESIEGE_20050404 (b4nfter)
			LogAddTD("[CastleSiege] [%s][%s] Push Castle Crown Switch Canceled (GUILD:%s) - CS X:%d/Y:%d",
				lpUserObj->AccountID,
				lpUserObj->Name,
				lpUserObj->GuildName,
				lpObj->X,
				lpObj->Y
				);
		}
	}
	else {
		GCAnsCsAccessSwitchState (lpUserObj->m_Index, lpObj->m_Index, -1, 0);
		g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);
		if (g_CastleSiege.GetRegCrownAvailable() == TRUE) {
			g_CastleSiege.SetRegCrownAvailable(FALSE);
			g_CastleSiege.NotifyCrownState(1);
		}
	}
}


#endif

