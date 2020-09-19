// GuardianStatue.cpp: implementation of the CGuardianStatue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardianStatue.h"

#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207				// 수호석상의 동작

CGuardianStatue	g_CsNPC_GuardianStatue;

#include "CastleSiege.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardianStatue::CGuardianStatue			()
{

}

CGuardianStatue::~CGuardianStatue			()
{

}


VOID CGuardianStatue::GuardianStatueAct		(				// 수호석상의 행동방식
	INT iIndex
	)
{
	if (!gObjIsConnected(iIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iIndex];

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
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
				if (gObj[tObjNum].m_btCsJoinSide == CS_SIEGESIDE_DEFEND) {
					if (abs(lpObj->Y - gObj[tObjNum].Y) <= 3 && abs(lpObj->X - gObj[tObjNum].X) <= 3 )
					{
						// 이 사람이 수성측 길드이고 지정된 반경안에 있다면 여기서 HP, MP, AG를 늘려준다.
						BOOL bLifeChange	= FALSE;
						BOOL bManaChange	= FALSE;
						BOOL bBpChange		= FALSE;

#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].Life < gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife) 
						{
							gObj[tObjNum].Life		+= (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife) * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].Life > gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife)
								gObj[tObjNum].Life	= gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife;
							bLifeChange		= TRUE;
						}
#else
						if (gObj[tObjNum].Life < gObj[tObjNum].MaxLife) 
						{
							gObj[tObjNum].Life		+= gObj[tObjNum].MaxLife * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].Life > gObj[tObjNum].MaxLife)
								gObj[tObjNum].Life	= gObj[tObjNum].MaxLife;
							bLifeChange		= TRUE;
						}
#endif // MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609

#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].Mana < gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) 
						{
							gObj[tObjNum].Mana		+= (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].Mana > gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)
								gObj[tObjNum].Mana	= gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;
							bManaChange		= TRUE;
						}
#else
						if (gObj[tObjNum].Mana < gObj[tObjNum].MaxMana) 
						{
							gObj[tObjNum].Mana		+= gObj[tObjNum].MaxMana * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].Mana > gObj[tObjNum].MaxMana)
								gObj[tObjNum].Mana	= gObj[tObjNum].MaxMana;
							bManaChange		= TRUE;
						}
#endif // MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609

#ifdef MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609
						if (gObj[tObjNum].BP < gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) 
						{
							gObj[tObjNum].BP		+= (gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].BP > gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP)
								gObj[tObjNum].BP	= gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP;
							bBpChange		= TRUE;
						}
#else
						if (gObj[tObjNum].BP < gObj[tObjNum].MaxBP) 
						{
							gObj[tObjNum].BP		+= gObj[tObjNum].MaxBP * (lpObj->m_btCsNpcRgLevel + 1) / 100;
							if (gObj[tObjNum].BP > gObj[tObjNum].MaxBP)
								gObj[tObjNum].BP	= gObj[tObjNum].MaxBP;
							bBpChange		= TRUE;
						}
#endif  // MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609

						if (bLifeChange)
#ifdef ADD_SHIELD_POINT_01_20060403
							GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1, (WORD)gObj[tObjNum].iShield);
#else
							GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1);
#endif

						if (bManaChange || bBpChange)
							GCManaSend(tObjNum, (short)gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
					}
				}
#endif
			}
		}
	}
}

#endif

