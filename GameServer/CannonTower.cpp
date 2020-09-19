// CannonTower.cpp: implementation of the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CannonTower.h"

#ifdef CASTLE_NPC_CANNON_TOWER_20041223

CCannonTower	g_CsNPC_CannonTower;

#include "CastleSiege.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCannonTower::CCannonTower()
{

}

CCannonTower::~CCannonTower()
{

}

VOID CCannonTower::CannonTowerAct		(				// 캐논타워의 행동방식
	INT iIndex
	)
{
	if (rand()%2)
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	int tObjNum;
	int  count=0;
	PMSG_BEATTACK_COUNT	pCount;
	PMSG_BEATTACK		pAttack;

	// 이 버퍼 크기 주의해야 한다. 
	// 추후 몬스터의 뷰포트개수가 증가했을시 수정되야함! (apple)
	BYTE AttackSendBuff[256];
	int  ASBOfs=0;
	
	pCount.h.c			= PMHC_BYTE;
#ifdef PACKET_CHANGE	
	pCount.h.headcode	= PACKET_MAGIC_ATTACK;
#else
	pCount.h.headcode	= 0x1D;
#endif
	pCount.h.size		= 0;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	pCount.MagicNumberH	= HIBYTE( 0 );
	pCount.MagicNumberL	= LOBYTE( 0 );
#else
	pCount.MagicNumber	= 0;
#endif
	pCount.Count		= 0;
	pCount.X			= (BYTE)lpObj->X;
	pCount.Y			= (BYTE)lpObj->Y;
	ASBOfs				= sizeof( pCount );
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 ) {
					if (gObj[tObjNum].m_btCsJoinSide != CS_SIEGESIDE_DEFEND) {
						if( gObjCalDistance( lpObj, &gObj[tObjNum]) < 7 )
						{
							pAttack.NumberH = HIBYTE(tObjNum);
							pAttack.NumberL = LOBYTE(tObjNum);
							memcpy( AttackSendBuff+ASBOfs, (char*)&pAttack, sizeof( pAttack ));
							ASBOfs += sizeof( pAttack );
							pCount.Count++;
						}
					}
				}
			}
		}
		count++;
		if( count > MAX_MONVIEWPORTOBJECT-1 )
		{
			break;
		}
	}

	if( pCount.Count > 0 )
	{
		// 한명이라도 실제로 공격당한 사람이 있음
		pCount.h.size = ASBOfs;
		memcpy(AttackSendBuff, (char*)&pCount, sizeof(pCount));

		CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
	
		// 캐논타워 주위의 사용자들에게 전체공격 (악마의 불꽃) 이 시전되는 것을 보여준다.
		PMSG_DURATION_MAGIC_SEND	pMsg;
		PHeadSetBE((LPBYTE)&pMsg, 0x1E, sizeof( pMsg ));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pMsg.MagicNumberH	= HIBYTE( AT_SKILL_DEVILFIRE );
		pMsg.MagicNumberL	= LOBYTE( AT_SKILL_DEVILFIRE );
#else
		pMsg.MagicNumber	= AT_SKILL_DEVILFIRE;
#endif
		pMsg.X				= lpObj->X;
		pMsg.Y				= lpObj->Y;
		pMsg.Dir			= 0;
		pMsg.NumberH		= HIBYTE(iIndex);
		pMsg.NumberL		= LOBYTE(iIndex);
		MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

#endif
