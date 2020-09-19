// Guardian.cpp: implementation of the CGuardian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guardian.h"

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116

CGuardian	g_CsNPC_Guardian;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardian::CGuardian()
{

}

CGuardian::~CGuardian()
{

}

BOOL CGuardian::CreateGuardian( INT iIndex )
{
	MsgOutput(iIndex, lMsg.Get(1640) ); // "가디언을 소환합니다."

	return TRUE;
}

VOID CGuardian::GuardianAct( INT iIndex )
{
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
	
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
				if (gObj[tObjNum].m_btCsJoinSide == lpObj->m_btCsJoinSide) {
					if (abs(lpObj->Y - gObj[tObjNum].Y) <= 3 && abs(lpObj->X - gObj[tObjNum].X) <= 3 )
					{
						// 이 사람이 수성측 길드이고 지정된 반경안에 있다면 여기서 HP, MP, AG를 늘려준다.
						gObj[tObjNum].Life		+= 100;
						gObj[tObjNum].Mana		+= 100;
						gObj[tObjNum].BP		+= 100;
						if (gObj[tObjNum].Life > gObj[tObjNum].MaxLife)
							gObj[tObjNum].Life	= gObj[tObjNum].MaxLife;
						if (gObj[tObjNum].Mana > gObj[tObjNum].MaxMana)
							gObj[tObjNum].Mana	= gObj[tObjNum].MaxMana;
						if (gObj[tObjNum].BP > gObj[tObjNum].MaxBP)
							gObj[tObjNum].BP	= gObj[tObjNum].MaxBP;

#ifdef ADD_SHIELD_POINT_01_20060403
						GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1, (WORD)gObj[tObjNum].iShield);
#else
						GCReFillSend(tObjNum, (WORD)gObj[tObjNum].Life, 0xFF, 1);
#endif

						GCManaSend(tObjNum, (short)gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
					}
				}
			}
		}
	}
#endif
}

#endif
