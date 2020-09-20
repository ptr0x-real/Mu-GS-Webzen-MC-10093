// CastleSiegeSync.cpp: implementation of the CCastleSiegeSync class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleSiegeSync.h"
#include "MapServerManager.h"
#include "DSProtocol.h"

#ifdef ADD_NEW_UNION_01_20041006
#include "TUnion.h"
#include "TUnionInfo.h"
#endif


#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleSiegeSync::CCastleSiegeSync()
{
	Clear();
}

CCastleSiegeSync::~CCastleSiegeSync()
{

}

VOID CCastleSiegeSync::Clear			()
{
	m_iCurCastleState		= CASTLESIEGE_SYNC_STATE_NONE;
	m_iCurTaxRateChaos		= 0;
	m_iCurTaxRateStore		= 0;
	m_lCastleTributeMoney	= 0;
	m_iCsTributeMoneyTimer	= 0;
	memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
}


VOID CCastleSiegeSync::SetCastleOwnerGuild (
	LPSTR lpszGuildName
	)
{
	if (lpszGuildName == NULL)
		return;

	memset(m_szCastleOwnerGuild, 0, sizeof(m_szCastleOwnerGuild));
	memcpy(m_szCastleOwnerGuild, lpszGuildName, 8);
}


INT	CCastleSiegeSync::GetTaxRateChaos	(INT iIndex)
{
	INT iCurTaxRateChaos	= m_iCurTaxRateChaos;

	if (CheckCastleOwnerMember(iIndex) == TRUE ||
		CheckCastleOwnerUnionMember(iIndex) == TRUE
		)
	{
		iCurTaxRateChaos	= 0;
	}

	return	iCurTaxRateChaos;	
}


INT CCastleSiegeSync::GetTaxRateStore	(INT iIndex)
{	
	INT iCurTaxRateStore	= m_iCurTaxRateStore;

	if (CheckCastleOwnerMember(iIndex) == TRUE ||
		CheckCastleOwnerUnionMember(iIndex) == TRUE
		)
	{
		iCurTaxRateStore	= 0;
	}

	return	iCurTaxRateStore;
}


INT CCastleSiegeSync::GetTaxHuntZone	(INT iIndex, BOOL bCheckOwnerGuild)
{
	INT iCurTaxHuntZone		= m_iCurTaxHuntZone;

	if (bCheckOwnerGuild == TRUE) {
		if (CheckCastleOwnerMember(iIndex) == TRUE ||
			CheckCastleOwnerUnionMember(iIndex) == TRUE
			)
		{
			iCurTaxHuntZone	= 0;
		}
	}

	return	iCurTaxHuntZone;
}


VOID CCastleSiegeSync::AddTributeMoney		(INT iMoney)
{
	if (m_lCastleTributeMoney < 0)
		InterlockedExchange(&m_lCastleTributeMoney, 0);

	if (iMoney <= 0)
		return;

	if (m_lCastleTributeMoney + iMoney > MAX_ZEN)
		return;

	InterlockedExchangeAdd(&m_lCastleTributeMoney, iMoney);
}


VOID CCastleSiegeSync::ResetTributeMoney	()
{
	InterlockedExchange(&m_lCastleTributeMoney, 0);
}


VOID CCastleSiegeSync::AdjustTributeMoney	()			// ������� ���� �������� DB�� ����
{
	if (m_lCastleTributeMoney < 0)
		InterlockedExchange(&m_lCastleTributeMoney, 0);

	if (m_lCastleTributeMoney == 0)
		return;

	m_iCsTributeMoneyTimer++;
	m_iCsTributeMoneyTimer %= MAX_CS_SYNC_MONEY_SEC;
	if (m_iCsTributeMoneyTimer)
		return;

#ifdef MU_CASTLESIEGE_DS_PROTOCOL_20041105
#ifdef MAP_SERVER_WORK_20041030
	GS_GDReqCastleTributeMoney (g_MapServerManager.GetMapSvrGroup(), m_lCastleTributeMoney);
#endif
#endif
}


BOOL CCastleSiegeSync::CheckCastleOwnerMember			(INT iIndex)				// �ش� ����ڰ� ���� ������ ����� �Ҽ����� Ȯ���Ѵ�.
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, ""))
		return FALSE;

	if (strcmp(gObj[iIndex].GuildName, m_szCastleOwnerGuild))
		return FALSE;

	return TRUE;
}


BOOL CCastleSiegeSync::CheckCastleOwnerUnionMember		(							// Ư�� ����ڰ� �ڽ��� ���� ��忬���� �Ͽ����� Ȯ���Ѵ�.
		INT iIndex
		)
{
	if (!gObjIsConnected(iIndex))
		return FALSE;

	if (!strcmp(m_szCastleOwnerGuild, ""))
		return FALSE;

	LPGUILD_INFO lpGuildInfo		= gObj[iIndex].lpGuild;
	if (lpGuildInfo == NULL)
		return FALSE;

#ifdef ADD_NEW_UNION_01_20041006
	TUnionInfo* pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);
	if( pUnionInfo == NULL) {
		return FALSE;
	}
	else {
		if (!strcmp (pUnionInfo->m_szMasterGuild, m_szCastleOwnerGuild)) {
			// ���� ������ �Ͽ��̴�.
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
#endif

	return FALSE;
}


INT	 CCastleSiegeSync::CheckOverlapCsMarks			(INT iIndex)			// Ư�� ������� �κ��丮�� ������ ǥ���� ��ĥ �� �ִٸ� ���° ĭ���� �˷��ش�.
{
	for(int x=MAX_EQUIPMENT; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(14, 21) &&
				gObj[iIndex].pInventory[x].m_Level == 3
				)
			{
				// ������ ǥ���� �����Ѵٸ�
				INT iITEM_DUR	= (INT) gObj[iIndex].pInventory[x].m_Durability;	// ������ ���� (������) �� ��´�.

				if (CHECK_LIMIT(iITEM_DUR, MAX_CS_SYNC_MARKS_OVERLAP)) {
					// ���� M�� �̸��̶�� ���⿡ �� ���� �� �ִ�.
					return x;
				}
			}
		}
	}

	return -1;
}


CCastleSiegeSync	g_CastleSiegeSync;


#endif

