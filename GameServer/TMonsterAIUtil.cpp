#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIUtil.h"

#include "..\Common\WinUtil.h"
#include "User.h"
#include "protocol.h"
#include "MapClass.h"
#include "Gamemain.h"

#include <math.h>


TMonsterAIUtil::TMonsterAIUtil()
{

}

TMonsterAIUtil::~TMonsterAIUtil()
{

}

BOOL TMonsterAIUtil::FindMonViewportObj( int iObjIndex, int iTargetObjIndex )
{
	if( !CHECK_LIMIT(iObjIndex,MAX_MONSTER) || !CHECK_LIMIT(iTargetObjIndex,MAX_OBJECT) )
		return FALSE;

	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
	{
		if( gObj[iObjIndex].VpPlayer[i].state )
		{
			if( iTargetObjIndex == gObj[iObjIndex].VpPlayer[i].number )
				return TRUE;
		}
	}

	return FALSE;
}

BOOL TMonsterAIUtil::FindMonViewportObj2( int iObjIndex, int iTargetObjIndex )
{
	if( !CHECK_LIMIT(iObjIndex,MAX_MONSTER) || !CHECK_LIMIT(iTargetObjIndex,MAX_OBJECT) )
		return FALSE;

	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
	{
		if( gObj[iObjIndex].VpPlayer2[i].state )
		{
			if( iTargetObjIndex == gObj[iObjIndex].VpPlayer2[i].number )
				return TRUE;
		}
	}

	return FALSE;
}

// ���� �ڽ��� ���� �÷��̾�� �޽��� ����
void TMonsterAIUtil::SendMonsterV2Msg(LPOBJECTSTRUCT lpObj, LPBYTE lpMsg, int size)
{
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
	{
		if( lpObj->VpPlayer2[i].type == OBJTYPE_CHARACTER )
		{
			if( lpObj->VpPlayer2[i].state )
			{
				if( (lpObj->Connected > 1) && (lpObj->Live) )
				{
					DataSend(lpObj->VpPlayer2[i].number, lpMsg, size);
				}
				else
				{
					lpObj->VpPlayer2[i].number = -1;
					lpObj->VpPlayer2[i].state  = STVP_NONE;
					lpObj->VPCount2--;
				}
			}	
		}

	}
}

inline BOOL TMonsterAIUtil::CheckMovingCondition( LPOBJECTSTRUCT lpObj )
{
	// 0. > �׾��� ��� �̵� �� �� ����
	if( !lpObj->Live )
	{
		return FALSE;
	}

	// 0. > ���� �߿� �̵� �� �� ����
	if( lpObj->RegenOk > 0 ) 
	{
		return FALSE;
	}

	// 0. > ���� �̵� �� �̵� �� �� ����
	if( lpObj->Teleport ) 
	{
		return FALSE;
	}
	
	// 0. > Play ���°� �ƴϸ� �̵� �� �� ����
	if( lpObj->m_State != OBJST_PLAYING )
	{
		return FALSE;
	}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true 
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true 
	  )
	{
		return FALSE;
	}
#else
	if( lpObj->m_SkillHarden )
		return FALSE;

	// 2. > ����
	if( lpObj->m_iSkillStunTime > 0 )
		return FALSE;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� �̵��� �� �� ����
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		return FALSE;
	}
#endif
	
	return TRUE;
}

BOOL TMonsterAIUtil::FindPathToMoveMonster( LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY, int iMaxPathCount, BOOL bPreventOverMoving )
{
	
	// 0. > �̵� ���� ������ �˻��Ѵ�.
	if( !CheckMovingCondition(lpObj) )
		return FALSE;

	// 1. > ��ã��
	PATH_t	Path;
	
	BOOL bPathFound = FALSE;
	
	if ( bPreventOverMoving )
	{
		bPathFound = MapC[lpObj->MapNumber].PathFinding2(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
	}
	else
	{
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		bPathFound = MapC[lpObj->MapNumber].PathFinding4(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
#endif
	}

	// 2. > ã�� ���� ���Ϳ� ���� ��Ų��.
	if (bPathFound)
	{
		lpObj->m_LastMoveTime = GetTickCount();

		int iTargetX, iTargetY, iStartX, iStartY, iResultX, iResultY;

		BYTE btTargetDir=0;

		iStartX  = lpObj->X;
		iStartY  = lpObj->Y;

		iResultX = lpObj->X;
		iResultY = lpObj->Y;

		lpObj->PathCount	= Path.PathNum;
		lpObj->PathCur		= 1;	
		lpObj->PathStartEnd = 1;
		
		if( lpObj->PathCount > iMaxPathCount )
			lpObj->PathCount = iMaxPathCount;

		lpObj->PathX[0]		= lpObj->X;
		lpObj->PathY[0]		= lpObj->Y;
		lpObj->PathDir[0]	= lpObj->Dir;

		for( int n=1; n<lpObj->PathCount; n++)
		{
			iTargetX = Path.PathX[n];
			iTargetY = Path.PathY[n];

			btTargetDir = GetPathPacketDirPos( iTargetX-iStartX, iTargetY-iStartY);

			lpObj->PathX[n]		= iTargetX;
			lpObj->PathY[n]		= iTargetY;
			lpObj->PathDir[n]	= btTargetDir;
			
			iResultX += RoadPathTable[btTargetDir*2];
			iResultY += RoadPathTable[(btTargetDir*2)+1];
		}

		// Ÿ�� ���� ��ġ ����
		lpObj->MTX = iResultX;
		lpObj->MTY = iResultY;
		lpObj->TX  = iResultX;
		lpObj->TY  = iResultY;
		
		// �ʼӼ� ����
		if( lpObj->PathCount > 0 )
		{
			MapC[lpObj->MapNumber].ClearStandAttr(iStartX, iStartY);
			MapC[lpObj->MapNumber].SetStandAttr(iResultX, iResultY);
			lpObj->m_OldX = iStartX;
			lpObj->m_OldY = iStartY;
		}
	}

	// ������ iTargetX, iTargetY�� ã���� ��θ� ���ؼ� �˾Ƴ���.

	// 3. > ���� �̵� ���������� ������.
	if( bPathFound )
	{
		SendMonsterMoveMsg( lpObj );
		//vChatSend(lpObj, "�̵����� %d / %d  -> %d / %d", lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY);

		return TRUE;
	}
	
	return FALSE;	
	
}




// ���� ���� ��ġ���� ���� ���� ������ üũ�ϴ� �Լ�
BOOL TMonsterAIUtil::CheckMoveRange(LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY)
{
	return FALSE;
}

// ���� ��ī���� ��ġ ��� ó�� �Լ�
BOOL TMonsterAIUtil::GetXYToPatrol(LPOBJECTSTRUCT lpObj)
{
	int maxmoverange = (lpObj->m_MoveRange*2)+1;
	int searchc=10;
	BYTE tpx, tpy;
	lpObj->NextActionTime = 1000;
				
	while(searchc--)
	{
		__try
		{
			tpx = (lpObj->X-lpObj->m_MoveRange)+(rand()%maxmoverange);
			tpy = (lpObj->Y-lpObj->m_MoveRange)+(rand()%maxmoverange);		
		}
		__except( maxmoverange = 1, EXCEPTION_EXECUTE_HANDLER )
		{
		}
		
		// �ʹ� �ָ� ���� �ʾҳ�?? üũ
		//BOOL mchk = gObjMonsterMoveCheck(lpObj, tpx, tpy);
	
		BYTE attr = MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
	
		if( (  (attr&MAP_ATTR_SAFTYZONE)!=MAP_ATTR_SAFTYZONE) 
			&& ( (attr&MAP_ATTR_STAND)	!=MAP_ATTR_STAND	) 
			&& ( (attr&MAP_ATTR_BLOCK)	!=MAP_ATTR_BLOCK	) 
			&& ( (attr&MAP_ATTR_HOLLOW)	!=MAP_ATTR_HOLLOW	) 
//			&& mchk 
		  )
		{
			lpObj->MTX			= tpx;
			lpObj->MTY			= tpy;
			return TRUE;
		}			
		
	}

	return FALSE;
}

// Ÿ������ ���� Ư�� ��ġ���� �������� ���� ó�� �Լ�
BOOL TMonsterAIUtil::GetXYToEascape(LPOBJECTSTRUCT lpObj)
{
	int tpx, tpy, mtx, mty;
	int tx, ty;
	int searchp=0, sn=0;
	int searchcount=7;
	BYTE attr;
	BOOL result = FALSE;

	if( !CHECK_LIMIT(lpObj->TargetNumber, MAX_OBJECT) ) 
		return FALSE;

	LPOBJECTSTRUCT lpTargetObj = (LPOBJECTSTRUCT)&gObj[lpObj->TargetNumber];

	mtx = tpx = lpTargetObj->X;
	mty = tpy = lpTargetObj->Y;

	tx = lpObj->X;
	ty = lpObj->Y;


	int dis = (int)( (float)lpObj->m_AttackRange / sqrt((double)2) );

	// ���� ���� �ִ� �Ÿ� �߿��� ���� �հ����� ����
	if( lpObj->X < mtx ) tx -= dis;
	if( lpObj->X > mtx ) tx += dis;
	if( lpObj->Y < mty ) ty -= dis;
	if( lpObj->Y > mty ) ty += dis;

	searchp = GetPathPacketDirPos(lpObj->X-tx, lpObj->Y-tx)*2;

	// ���� ���ٸ�.
	if( MapC[lpObj->MapNumber].GetStandAttr( tx, ty) == FALSE )	
	{
		while(searchcount--)
		{
			mtx = lpObj->X+RoadPathTable[searchp];
			mty = lpObj->Y+RoadPathTable[searchp+1];
			
			//  �ڸ��� ���������..
			attr= MapC[lpObj->MapNumber].GetAttr(mtx, mty);
//			result = gObjMonsterMoveCheck(lpObj, mtx, mty);

			
			if(    ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) 
				&& ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) 
				&& ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
				&& ((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) 
//				&& (result==TRUE) 
			  )
			{
				lpObj->MTX = mtx;
				lpObj->MTY = mty;
				return TRUE;
			}

			searchp += 2;

			if( searchp > 15 ) searchp = 0;
		}		
		return FALSE;
	}
	else
	{
		attr= MapC[lpObj->MapNumber].GetAttr(tx, ty);
//		result = gObjMonsterMoveCheck(lpObj, mtx, mty);

	
		if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
			((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) 
//			&& (result==TRUE) 
			) // ��������� �� ���±��̸�..
		{
			lpObj->MTX = tx;
			lpObj->MTY = ty;
			return TRUE;
		}
		
	}

	return FALSE;
}

// ���� �Ÿ� ���� ���� ��ó�� ����
BOOL TMonsterAIUtil::GetXYToChase(LPOBJECTSTRUCT lpObj)
{
	int tpx, tpy, mtx, mty;
	int searchp=0, sn=0;
	int searchcount=7;
	BYTE attr;
	BOOL result = FALSE;

	if( !CHECK_LIMIT(lpObj->TargetNumber, MAX_OBJECT) ) 
		return FALSE;

	LPOBJECTSTRUCT lpTargetObj = (LPOBJECTSTRUCT)&gObj[lpObj->TargetNumber];

	mtx = tpx = lpTargetObj->X;
	mty = tpy = lpTargetObj->Y;
	
	int dis = (int)( (float)lpObj->m_AttackRange / sqrt((double)2) );
	
	// Ÿ�� ��ó�� ��ġ�� ��´�.
	if( lpObj->X < mtx ) tpx -= dis;
	if( lpObj->X > mtx ) tpx += dis;
	if( lpObj->Y < mty ) tpy -= dis;
	if( lpObj->Y > mty ) tpy += dis;

	searchp = GetPathPacketDirPos(lpTargetObj->X-tpx, lpTargetObj->Y-tpy)*2;

	// ���� ���ٸ�.
	if( MapC[lpObj->MapNumber].GetStandAttr( tpx, tpy) == FALSE )	
	{
		while(searchcount--)
		{
			mtx = lpTargetObj->X+RoadPathTable[searchp];
			mty = lpTargetObj->Y+RoadPathTable[searchp+1];
			
			//  �ڸ��� ���������..
			attr= MapC[lpObj->MapNumber].GetAttr(mtx, mty);
//			result = gObjMonsterMoveCheck(lpObj, mtx, mty);

			
			if(    ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) 
				&& ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) 
				&& ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
				&& ((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) 
//				&& (result==TRUE) 
			  )
			{
				lpObj->MTX = mtx;
				lpObj->MTY = mty;
				return TRUE;
			}

			searchp += 2;

			if( searchp > 15 ) searchp = 0;
		}		
		return FALSE;
	}
	else
	{
		attr= MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
//		result = gObjMonsterMoveCheck(lpObj, mtx, mty);

	
		if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
			((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) 
//			&& (result==TRUE) 
			) // ��������� �� ���±��̸�..
		{
			lpObj->MTX = tpx;
			lpObj->MTY = tpy;
			return TRUE;
		}
		
	}

	return FALSE;
}




BOOL TMonsterAIUtil::SendMonsterMoveMsg( LPOBJECTSTRUCT lpObj )
{
	PMSG_RECVMOVE	pMove;
	PHeadSetB((LPBYTE)&pMove, PACKET_MOVE, sizeof( pMove ));

	pMove.NumberH		= HIBYTE(lpObj->m_Index);
	pMove.NumberL		= LOBYTE(lpObj->m_Index);
	pMove.X				= (BYTE)lpObj->MTX;
	pMove.Y				= (BYTE)lpObj->MTY;
	pMove.Path          = lpObj->Dir<<4;

	SendMonsterV2Msg( lpObj, (LPBYTE)&pMove, pMove.h.size );
	
	return TRUE;
}

void TMonsterAIUtil::SendChattingMsg( int iObjIndex, LPSTR lpszMsg, ... )
{
	if (lpszMsg == NULL)
		return;
	
	if( !CHECK_LIMIT(iObjIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT	lpObj = &gObj[iObjIndex];

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, lpszMsg);
    vsprintf(szBuffer, lpszMsg, pArguments);
    va_end(pArguments);
	

	char szChat[MAX_CHAT] = {0,};

	CopyMemory( szChat, szBuffer, MAX_CHAT - 1 );

	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
	{
		if( lpObj->VpPlayer2[i].state )
		{
			int iTargetNumber = lpObj->VpPlayer2[i].number;
			if( CHECK_LIMIT(iTargetNumber, MAX_OBJECT) )
			{
				ChatTargetSend(lpObj, szChat, iTargetNumber);
			}
		}
	}

//	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);

}
#endif