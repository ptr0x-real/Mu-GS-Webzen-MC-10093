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
	MsgOutput(iIndex, lMsg.Get(1640) ); // "������� ��ȯ�մϴ�."

	return TRUE;
}

VOID CGuardian::GuardianAct( INT iIndex )
{
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
	
	if (!gObjIsConnected(iIndex))
		return;

	LPOBJECTSTRUCT lpObj	= &gObj[iIndex];

	// ����Ʈ �� �ƹ��� ���ٸ� ������.
	if( lpObj->VPCount < 1 ) return;

	// ����Ʈ �󿡼� �ش�Ǵ� ������� ã�´�.
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
						// �� ����� ������ ����̰� ������ �ݰ�ȿ� �ִٸ� ���⼭ HP, MP, AG�� �÷��ش�.
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
