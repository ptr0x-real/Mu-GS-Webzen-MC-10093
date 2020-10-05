#include "StdAfx.h"
#include "TMonsterAIElement.h"

#include <math.h>
#include "Include\ReadScript.h"

#include "User.h"
#include "protocol.h"
#include "gObjMonster.h"
#include "GameMain.h"

#include "TMonsterSkillManager.h"
#include "TMonsterAIGroup.h"

#include "CrywolfUtil.h"
#include "TMonsterAIUtil.h"

#ifdef ADD_KANTURU_20060627
#include "KanturuUtil.h"

#include "KanturuDefine.h"
#include "KanturuMonsterMng.h"

static CKanturuUtil		KANTURU_UTIL;
#endif // ADD_KANTURU_20060627

#include "DirPath.h"
extern CDirPath	gDirPath;

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

//----------------------------------------------------------------------------------------

BOOL				TMonsterAIElement::s_bDataLoad = FALSE;
TMonsterAIElement	TMonsterAIElement::s_MonsterAIElementArray[MAX_MONSTER_AI_ELEMENT];
TMonsterAIMovePath	TMonsterAIElement::s_MonsterAIMovePath[MAX_MAP];

//----------------------------------------------------------------------------------------

static CCrywolfUtil		UTIL;
static TMonsterAIUtil	MONSTER_UTIL;


TMonsterAIElement::TMonsterAIElement()
{
#if ENABLE_MC_CODE == 1
	s_MonsterAIMovePath = NULL;
#endif // ENABLE_MC_CODE == 1
	Reset();
}

TMonsterAIElement::~TMonsterAIElement()
{
#if ENABLE_MC_CODE == 1
	delete[] s_MonsterAIMovePath;
#endif // ENABLE_MC_CODE == 1
}


void TMonsterAIElement::Reset()
{	
	m_iElementClass		= _INVALID;
	m_iElementType		= _INVALID;
	m_iElementNumber	= _INVALID;

	m_iSuccessRate		= _INVALID;
	m_iDelayTime		= _INVALID;
	m_iTargetType		= _INVALID;
	m_iX				= _INVALID;
	m_iY				= _INVALID;
	
	ZeroMemory( m_szElementName, MAX_AIELEMENTNAME );
}

int TMonsterAIElement::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Element] - File load error : File Name Error");
		return FALSE;
	}

#if ENABLE_MC_CODE == 1
	if (g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	if (s_MonsterAIMovePath == NULL)
		s_MonsterAIMovePath = new TMonsterAIMovePath[g_TerrainManager.Size()];
#endif // ENABLE_MC_CODE == 1

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Element] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllAIElement();

		SMDToken Token;
		
		int iType = -1;
		
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{	
					// 1. >	데이터 초기화
					char	szElementName[MAX_AIELEMENTNAME] = {0,};
					int		iElementNumber	= _INVALID;
					int		iElementClass	= _INVALID;
					int		iElementType	= _INVALID;
					int		iSuccessRate	= _INVALID;
					int		iDelayTime		= _INVALID;
					int		iTargetType		= _INVALID;
					int		iX				= _INVALID;
					int		iY				= _INVALID;
				
					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iElementNumber	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(szElementName, TokenString, MAX_AIELEMENTNAME);
					Token = GetToken(); iElementClass	= (int)TokenNumber;
					Token = GetToken(); iElementType	= (int)TokenNumber;
					Token = GetToken(); iSuccessRate	= (int)TokenNumber;
					Token = GetToken(); iDelayTime		= (int)TokenNumber;
					Token = GetToken(); iTargetType		= (int)TokenNumber;
					Token = GetToken(); iX				= (int)TokenNumber;
					Token = GetToken(); iY				= (int)TokenNumber;
				
					// 3. >	데이터 유효성 검사
					if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_AI_ELEMENT )
					{
						MsgBox("[Monster AI Element] - ElementNumber(%d) Error (%s) File. ", iElementNumber, lpszFileName);
						continue;
					}
					if( iElementClass < 0 || iElementClass >= MAX_MONSTER_AI_ELEMENT_STATE )
					{
						MsgBox("[Monster AI Element] - ElementClass(%d) Error (%s) File. ", iElementClass, lpszFileName);
						continue;
					}


					// 4. > 데이터 세팅
					s_MonsterAIElementArray[iElementNumber].m_iElementNumber	= iElementNumber;
					s_MonsterAIElementArray[iElementNumber].m_iElementClass		= iElementClass;
					s_MonsterAIElementArray[iElementNumber].m_iElementType		= iElementType;
					s_MonsterAIElementArray[iElementNumber].m_iSuccessRate		= iSuccessRate;
					s_MonsterAIElementArray[iElementNumber].m_iDelayTime		= iDelayTime;
					s_MonsterAIElementArray[iElementNumber].m_iTargetType		= iTargetType;
					s_MonsterAIElementArray[iElementNumber].m_iX				= iX;
					s_MonsterAIElementArray[iElementNumber].m_iY				= iY;
					
					CopyMemory( s_MonsterAIElementArray[iElementNumber].m_szElementName, szElementName, MAX_AIELEMENTNAME );
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI Element] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Element] - Loading Exception Error (%s) File. ", lpszFileName);
	}


	// 몬스터 이동 경로를 로드한다.
	s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].LoadData(gDirPath.GetNewPath("MovePath_35.dat"));
	
	return 0;
}

int	 TMonsterAIElement::DelAllAIElement()
{
	for( int i=0; i<MAX_MONSTER_AI_ELEMENT; i++ )
	{
		s_MonsterAIElementArray[i].Reset();
	}

#if ENABLE_MC_CODE == 1
	for (int j = 0; j < g_TerrainManager.Size(); j++)
#else // ENABLE_MC_CODE == 1
	for (int j = 0; j < MAX_MAP; j++)
#endif // ENABLE_MC_CODE == 1
	{
		s_MonsterAIMovePath[j].DelAllAIMonsterMovePath();
	}

	return 0;
}

TMonsterAIElement* TMonsterAIElement::FindAIElement( int iElementNumber )
{
	if( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_AI_ELEMENT )
	{
		LogAddTD("[Monster AI Element] FindAIElement() Error - (ElementNumber=%d) ", iElementNumber);
		return NULL;
	}

	if( s_MonsterAIElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &s_MonsterAIElementArray[iElementNumber];
	}
	else
		LogAddTD("[Monster AI Element] FindAIElement() Error - (ElementNumber=%d) ", iElementNumber);
	
	return NULL;
}

int TMonsterAIElement::ForceAIElement( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// 1. > General value Checking
	if( rand() % 100 > m_iSuccessRate)
	{
#if TESTSERVER == 1
		LogAddC(LOGC_RED, "[Force AI-Element] Failed to apply (%s) Element by SuccessRate", GetElementName());
#endif
		return _FAIL;
	}

	// 2. > Action Time Checking
	// MonsterAIUnit 의 Delay 타임을 이용한다. - 아래 사용 안함.
	//	if( (GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime) ) 
	//		return _FAIL;
	//	lpObj->CurActionTime = GetTickCount();	

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	#if TESTSERVER == 1
		DebugLog("[Force AI-Element] apply (%s)  (%d,%d) %s (%d) idx(%d)\n", GetElementName(), lpObj->X, lpObj->Y, lpObj->Name,lpObj->Class, lpObj->m_Index);
	#endif
#endif

	// 3. > Applying
	switch( m_iElementType )
	{
	case MAE_TYPE_COMMON_NORMAL		:
		ApplyElementCommon			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_MOVE_NORMAL		:
		ApplyElementMove			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_MOVE_TARGET		:
		ApplyElementMoveTarget		( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_GROUP_MOVE		:
		ApplyElementGroupMove		( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_ATTACK_NORMAL		:
		ApplyElementAttack			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_ATTACK_AREA		:
		ApplyElementAttackArea		( iIndex, iTargetIndex, pAIState );
		break;
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	case MAE_TYPE_ATTACK_PENETRATION	:
		ApplyElementAttackPenetration	( iIndex, iTargetIndex, pAIState );
		break;
#endif		
	case MAE_TYPE_HEAL_SELF			:
		ApplyElementHealSelf		( iIndex, iTargetIndex, pAIState );
		break;	
	case MAE_TYPE_HEAL_GROUP		:
		ApplyElementHealGroup		( iIndex, iTargetIndex, pAIState );
		break;	
	case MAE_TYPE_AVOID_NORMAL		:
		ApplyElementAvoid			( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_SOMMON	:		
		ApplyElementSpecialSommon	( iIndex, iTargetIndex, pAIState );
		break;

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724	
	case MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON	:
		ApplyElementNightmareSummon ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_WARP	:
		ApplyElementNightmareWarp ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_SKILLATTACK :
		ApplyElementSkillAttack	( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_CHANGEAI :
		ApplyElementAIChange ( iIndex, iTargetIndex, pAIState );
		break;
	case MAE_TYPE_SPECIAL_IMMUNE	:
		ApplyElementSpecialImmune	( iIndex, iTargetIndex, pAIState );
		break;
#endif // MODIFY_NEW_MONSTER_AI_01_2006724
		

	};

	return _SUCCESS;
	
}

int	TMonsterAIElement::ApplyElementCommon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	//UTIL.SendCrywolfChattingMsg( iIndex, "Element-일반");
	
#ifdef ADD_KANTURU_20060627
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "앗싸 좆쿠나!" );
#endif

	return 0;
}

int	TMonsterAIElement::ApplyElementMove( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	UTIL.SendCrywolfChattingMsg( iIndex, "Element-이동");
#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-이동");
#endif

	// 1. > 이동 중 이고
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 2. > 이동 좌표를 경우에 따라 구한다.
	BOOL bFindXY = FALSE;

	if( pAIState->m_iTransitionType == MAI_STATE_TRANS_OUT_ENEMY )
	{
		// 공격 범위 밖 적 발견 
		bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
	}
	else
	{
		// 적이 없음
		bFindXY = MONSTER_UTIL.GetXYToPatrol( lpObj );
	}
	
	// 3. > 이동 한다.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// 길찾기 성공		
		}
		else
		{
			// 길찾기 실패
		}
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementMoveTarget( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-타겟이동");
#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-타겟이동");
#endif

	// 1. > 이동 중 이고
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 3. > 여러마리가 한 좌표로 이동하려 하면 안되기때문에 범위를 둔다. m_iRange

	// 4. > 목적지에 도착했다면 일반 이동
	if( lpObj->X == m_iX && lpObj->Y == m_iY )
	{
		ApplyElementMove( iIndex, iTargetIndex, pAIState );
		return 0;
	}

	// 5. > 몬스터 이동 PATH 를 이용해서 Target 위치를 찾아간다.
	BOOL	bFindXY			= TRUE;			

	int		iTargetX		= m_iX;
	int		iTargetY		= m_iY;

	int		iTargetDistance	= sqrt( (double)(lpObj->X-iTargetX)*(lpObj->X-iTargetX) + (lpObj->Y-iTargetY)*(lpObj->Y-iTargetY));
	
	// 데이터가 로드 되어 있다면,
	if( s_MonsterAIMovePath[lpObj->MapNumber].m_bDataLoad && (iTargetDistance > 10) )
	{
		int iMinCost = 1000000;
		int iMidX	 = _INVALID;
		int iMidY	 = _INVALID;
		int iSpotNum = _INVALID;

		for( int i=0; i<MAX_MONSTER_MOVE_PATH_SPOT; i++ )
		{
			TMonsterAIMovePathInfo & PathInfo = s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_MovePathInfo[i];

			// 1 ) 나와 가까운 패스스팟들을 찾는다.

			float fDistX = (float)(lpObj->X-PathInfo.m_iPathX);
			float fDistY = (float)(lpObj->Y-PathInfo.m_iPathY);
			int iPathSpotDist = (int)(sqrt((fDistX*fDistX)+(fDistY*fDistY)));

			// 20타일 이내에 있을때
			if( iPathSpotDist < 20 )
			{
				fDistX = (float)(iTargetX-PathInfo.m_iPathX);
				fDistY = (float)(iTargetY-PathInfo.m_iPathY);
				
				// 목적지까지의 비용 계산

				// 이전에 이동 했던 Spot을 가지 않는 처리 필요
				int iMidDist = (int)(sqrt((fDistX*fDistX)+(fDistY*fDistY)));

				if( iMinCost > iMidDist )
				{
					// 현재 도착해 있는 Spot은 제외
					if( iMidDist != 0 )
					{
						iMinCost	= iMidDist;
						iMidX		= PathInfo.m_iPathX;
						iMidY		= PathInfo.m_iPathY;
						
						iSpotNum	= i;
					}	
				}
			}
		}
		
		if( iMinCost != 1000000 )
		{
			iTargetX = iMidX;
			iTargetY = iMidY;
		}
	}

	// 6. > 이동 한다.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, iTargetX, iTargetY, 7) )
		{
			// 길찾기 성공
			lpObj->PathStartEnd = 1;
		}
		else
		{
			// 길찾기 실패
			lpObj->PathStartEnd = 0;
		}		
	}
	
	return 0;
}

int	TMonsterAIElement::ApplyElementGroupMove( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-그룹이동");

	// 1. > 이동 중 이고
	if( lpObj->PathStartEnd != 0 )
	{
		return 0; 
	}

	// 2. > 이동 좌표를 경우에 따라 구한다.
	BOOL bFindXY		= FALSE;
	BOOL bFoundLeader	= TRUE;
	int  iLeaderIndex	= _INVALID;
	
	iLeaderIndex = TMonsterAIGroup::FindGroupLeader( lpObj->m_iGroupNumber );

	if( iLeaderIndex == _INVALID || !gObj[iLeaderIndex].Live )
	{
		bFoundLeader = FALSE;
	}

	if( bFoundLeader && gObjCalDistance(lpObj, &gObj[iLeaderIndex]) > 6 )
	{
		lpObj->TargetNumber = iLeaderIndex;
		bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
	}
	else
	{
		if( pAIState->m_iTransitionType == MAI_STATE_TRANS_OUT_ENEMY )
		{
			// 공격 범위 밖 적 발견 
			bFindXY = MONSTER_UTIL.GetXYToChase( lpObj );
		}
		else
		{
			// 적이 없음
			bFindXY = MONSTER_UTIL.GetXYToPatrol( lpObj );
		}

	}

	// 3. > 이동 한다.
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// 길찾기 성공		
		}
		else
		{
			MONSTER_UTIL.GetXYToPatrol( lpObj );
			MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE);
		}
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementAttack( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-공격");

	// 어떤 스킬 사용할지 선택!

	// pAIState 를 검사해서 Agro 상태가 높을 경우 강력한 공격 스킬 사용한 후 
	// Agro 를 감소 시켜 준다.

	if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
	{
		BOOL bEnableAttack = TRUE;
		// 1) 타겟이 없을때.
		if( lpObj->TargetNumber < 0 )
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
#else	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
			bEnableAttack = FALSE;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		
		// 1) 죽었고
		// 2) 텔레포트 중일 때
		if( !gObj[lpObj->TargetNumber].Live || gObj[lpObj->TargetNumber].Teleport)
		{
			bEnableAttack = FALSE;
		}
		
		// 1) 게임 진행 중이 아니고,
		// 2) 종료 중이고,
		if( gObj[lpObj->TargetNumber].Connected <= CSS_LOGINOK || gObj[lpObj->TargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}
		
		if( !bEnableAttack )
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
		
		// 여기서 스킬매니저가 처리
		
		// AI에서 스킬 처리!!!!!!!

		// 공격 방향을 잡는다.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
		
		// 1. 일반 공격
		if( rand()% 4 == 0 )
		{
			// 공격 거리가 맞는지 검사

			PMSG_ATTACK pAttackMsg;
			pAttackMsg.AttackAction = AT_ATTACK1;
			pAttackMsg.DirDis       = lpObj->Dir;
			pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
			pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
			//CGAttack(&pAttackMsg, lpObj->m_Index);
#else
			CGAttack(&pAttackMsg, lpObj->m_Index);
#endif

			GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index);
			gObjAttack(lpObj, &gObj[lpObj->TargetNumber], NULL, 0, 0);
		}
		else
		{
			// 2. 몬스터 스킬 공격
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
		}
		
		lpObj->m_ActState.Attack = 0;
		
		return _FAIL;
	}
	else
	{
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		if(lpObj->TargetNumber < 0)
			return _FAIL;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711

		// 공격 방향을 잡는다.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
		
		
		PMSG_ATTACK pAttackMsg;
		pAttackMsg.AttackAction = AT_ATTACK1;
		pAttackMsg.DirDis       = lpObj->Dir;
		pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);
		CGAttack(&pAttackMsg, lpObj->m_Index);
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
		GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index, lpTargetObj->m_Index );
#else
		GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index);
#endif
		gObjAttack(lpObj, &gObj[lpObj->TargetNumber], NULL, 0, 0);
	}

	return 0;
}

int	TMonsterAIElement::ApplyElementAttackArea( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-영역공격");

	// 영역 범위 내에 캐릭터들을 찾는다.

	int iTargetX = m_iX + (rand()%5) * (rand()%2==0?1:-1);
	int iTargetY = m_iY + (rand()%5) * (rand()%2==0?1:-1);

	for( int i=ALLOC_USEROBJECTSTART; i < MAX_OBJECT; i++  )
	{
		if( !gObjIsConnected(i) )
			continue;

		LPOBJECTSTRUCT lpTargetObj = &gObj[i];

		if( !lpObj->Live )
			continue;
		
		if( lpObj->MapNumber != lpTargetObj->MapNumber )
			continue;
		
		int	iTargetDistance	= sqrt( (double)(lpTargetObj->X-iTargetX)*(lpTargetObj->X-iTargetX) + (lpTargetObj->Y-iTargetY)*(lpTargetObj->Y-iTargetY));

		// 해당 영역의 캐릭터에게 영역 공격이 이루어졌음을 알린다.
		if( iTargetDistance < 10 )
		{
			PMSG_NOTIFY_REGION_MONSTER_ATTACK pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x0C, sizeof( pMsg ));
			pMsg.btObjClassH	= HIBYTE(lpObj->Class);
			pMsg.btObjClassL	= LOBYTE(lpObj->Class);

			pMsg.btSourceX		= lpObj->X;
			pMsg.btSourceY		= lpObj->Y;

			pMsg.btPointX		= iTargetX;		
			pMsg.btPointY		= iTargetY;
			
			DataSend( lpTargetObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg) );
		}

		// 영역 공격을 받았다.
		if( iTargetDistance < 6 )
		{
			gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
		}
	}
	
	return 0;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
int	TMonsterAIElement::ApplyElementAttackPenetration( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	
	// iTargetIndex값은 RunAIUnit(...)에서 0으로 들어온다.
	// 공격대상을 설정한다.
	iTargetIndex = lpObj->TargetNumber;	
	
	if ( lpObj->TargetNumber == -1 )
		return 0 ;
	
	if( !gObj[iTargetIndex].Live ) 
		return 0;

	TMonsterSkillManager::UseMonsterSkill( iIndex, iTargetIndex, MSUS_TYPE_ATTACK_PENETRATION );

	return 0;
}
#endif	// MODIFY_NEW_MONSTER_AI_01_2006724

int	TMonsterAIElement::ApplyElementAvoid( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-회피" );

#ifdef ADD_KANTURU_20060627	
	KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-회피" );
#endif

	BOOL bFindXY = MONSTER_UTIL.GetXYToEascape( lpObj );
	
	if( bFindXY )
	{
		if( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{
			// 길찾기 성공		
		}
		else
		{
			// 길찾기 실패
		}
	}
	
	return 0;
}

int	TMonsterAIElement::ApplyElementHealSelf( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{	
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-셀프치료" );
	
	lpObj->Life += lpObj->Life * 20 / 100;

	UTIL.SendCrywolfChattingMsg( iIndex, "HP : %d", (INT)lpObj->Life );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC, 0, 0, 0, 0, BUFFTIME_INFINITE );
#else
	ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_DEFENSE );
	GCStateInfoSend( lpObj, 1, lpObj->m_ViewSkillState );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	// 스스로 치료한다

	return 0;
}

int	TMonsterAIElement::ApplyElementHealGroup( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-그룹치료" );

	// 치료할 그룹 멤버가 있는지 검색..(6타일 이내
	TMonsterAIGroupMember* pMemb = TMonsterAIGroup::FindGroupMemberToHeal(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid, 6);
	
	if( pMemb != NULL )
	{
		LPOBJECTSTRUCT lpTargetObj		= &gObj[pMemb->m_iObjIndex];
		
		if( !lpTargetObj->Live )
			return 0;

		TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_HEAL_GROUP );

		if( lpSkillUnit != NULL )
		{
			lpSkillUnit->RunSkill( iIndex, lpTargetObj->m_Index );
		}

		UTIL.SendCrywolfChattingMsg( iIndex, "그룹치료 HP : %d", (INT)lpTargetObj->Life );
		UTIL.SendCrywolfChattingMsg( lpTargetObj->m_Index, "HP : %d", (INT)lpTargetObj->Life );
	}

	return 0;
}


int	TMonsterAIElement::ApplyElementSpecialSommon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	UTIL.SendCrywolfChattingMsg( iIndex, "Element-특수소환" );
	
	// 소환, 즉 리젠 시킬 그룹 멤버가 있는지 검색
	TMonsterAIGroupMember* pMemb = TMonsterAIGroup::FindGroupMemberToSommon(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid );
	
	if( pMemb != NULL )
	{
		LPOBJECTSTRUCT lpTargetObj		= &gObj[pMemb->m_iObjIndex];

		if( lpTargetObj->Live )
			return 0;

		TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_SPECIAL_SOMMON );

		if( lpSkillUnit != NULL )
		{
			lpSkillUnit->RunSkill( iIndex, lpTargetObj->m_Index );
		}

	}

	return 0;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
int TMonsterAIElement::ApplyElementSpecialImmune( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// 무적상태로 설정한다.
	// m_iX : 마법 공격 무적시간 (초)
	// m_iY : 물리 공격 무적시간 (초)
	
	if ( m_iX < 1 ) 
		m_iX = 10;  // 기본값 10초 적용
	if ( m_iY < 1 ) 
		m_iY = 10;	// 기본값 10초 적용

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, m_iX );
	gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, m_iY );
#else
	lpObj->m_ImmuneToHarmCount	=	m_iX;	// 마법공격 무적 시간 적용
	lpObj->m_ImmuneToMagicCount	=	m_iY;	// 물리공격 무적 시간 적용
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	return 0;
}

int TMonsterAIElement::ApplyElementNightmareSummon( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	// 나이트 메어에 특수.
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	// 타겟 상대를 설정해야함.
	if ( lpObj->TargetNumber == -1 )
	{
		// 타겟이 없는 경우 자기를 타겟으로 함.
		iTargetIndex = lpObj->m_Index;
	}
	iTargetIndex = lpObj->TargetNumber;

	// 스킬 확인
	TMonsterSkillUnit *lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( lpObj->m_Index, MSUS_TYPE_SPECIAL_SOMMON );
	if( lpSkillUnit != NULL )
	{
		GCUseMonsterSkillSend( &gObj[iIndex], &gObj[iTargetIndex], lpSkillUnit->m_iUnitNumber );	
		//MsgOutput(iTargetIndex, "[USEMONSTERSKILL] %d ", lpSkillUnit->m_iUnitNumber);

#ifdef ADD_KANTURU_20060627
		// 소환 처리
		int iRegenMonster = g_KanturuMonsterMng.SetKanturuMonster( KANTURU_SET_MONSTER_REGENMONSTER );
#endif

	}
	
	return 0;
}

int TMonsterAIElement::ApplyElementNightmareWarp( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	BYTE x, y;
	x = m_iX;
	y = m_iY;

	/*
	워프 영역 체크 하지 않음.
	if( !gObjCheckTeleportArea(iIndex, x, y) )
	{
		LogAddC(LOGC_RED, "[%s][%s] Try Teleport Not Move Area [%d,%d]", 
			   lpObj->AccountID, lpObj->Name, x, y);
		return 0;
	}
	*/

	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "나이트메어-워프" );

	// 순간이동 스킬 사용 패킷 : 기존 방식으로 테스트 (이미 처리되고 있음. 실제 적용시 삭제)
	PMSG_MAGICATTACK_RESULT	pAttack;
	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD 
	pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TELEPORT );
	pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TELEPORT );
#else
	pAttack.MagicNumber   = AT_SKILL_TELEPORT;
#endif
	pAttack.SourceNumberH = HIBYTE(iIndex);
	pAttack.SourceNumberL = LOBYTE(iIndex);	
	pAttack.TargetNumberH = HIBYTE(iIndex);
	pAttack.TargetNumberL = LOBYTE(iIndex);	
	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(iIndex, (LPBYTE)&pAttack, pAttack.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);

	// 텔레포트 사용
	LogAddC(LOGC_RED, "[%s] Warp [%d,%d] -> [%d,%d]", 
		   lpObj->Name, lpObj->X, lpObj->Y, x, y);
	gObjTeleportMagicUse(iIndex, x, y);

	// 타겟해제
	lpObj->TargetNumber = -1;

	return 0;
}

int TMonsterAIElement::ApplyElementSkillAttack( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];

	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-패턴스킬공격");

	// 어떤 스킬 사용할지 선택!

	// pAIState 를 검사해서 Agro 상태가 높을 경우 강력한 공격 스킬 사용한 후 
	// Agro 를 감소 시켜 준다.

	if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
	{
		BOOL bEnableAttack = TRUE;
		// 1) 타겟이 없을때.
		if( lpObj->TargetNumber < 0 )
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
#else	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
			bEnableAttack = FALSE;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
		
		// 1) 죽었고
		// 2) 텔레포트 중일 때
		if( !gObj[lpObj->TargetNumber].Live || gObj[lpObj->TargetNumber].Teleport)
		{
			bEnableAttack = FALSE;
		}
		
		// 1) 게임 진행 중이 아니고,
		// 2) 종료 중이고,
		if( gObj[lpObj->TargetNumber].Connected <= CSS_LOGINOK || gObj[lpObj->TargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}
		
		if( !bEnableAttack )
		{
			lpObj->TargetNumber			= -1;
			lpObj->m_ActState.Emotion	= 0;
			lpObj->m_ActState.Attack	= 0;
			lpObj->m_ActState.Move		= 0;
			lpObj->NextActionTime		= 1000;
			return _FAIL;
		}
		
		// 여기서 스킬매니저가 처리
		
		// AI에서 스킬 처리!!!!!!!

		// 공격 방향을 잡는다.
		LPOBJECTSTRUCT lpTargetObj	= &gObj[lpObj->TargetNumber];
		lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);

		// 확률 결정
		int iRate1 = m_iTargetType;		// script 값
		int iRate2 = m_iX;				// script 값
		int iRate3 = m_iY;
		int iRandom = rand()%100;


		//LogAddC(LOGC_RED, "[AI](%s) %s r1:%d r2:%d r3:%d rate:%d", GetElementName(), lpObj->Name, iRate1,iRate2,iRate3, iRandom);

		// 0 타입 스킬 발동
		if ( iRandom < iRate1 )
		{
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
		} 
		else if ( iRandom < iRate1 + iRate2 )
		{
			// 1타입 스킬 발동
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 1 );
		} 
		else if ( iRandom < iRate1 + iRate2 + iRate3 )
		{
			// 2타입 스킬 발동
			TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 2 );
		}
		
		lpObj->m_ActState.Attack = 0;
		
		return _FAIL;
	}

	return 0;
}

int TMonsterAIElement::ApplyElementAIChange( int iIndex, int iTargetIndex, TMonsterAIState* pAIState )
{
	//KANTURU_UTIL.SendKanturuChattingMsg( iIndex, "Element-AI변경");
	TMonsterAIGroup::ChangeAIOrder( m_iTargetType, m_iX	);
	LogAddC(LOGC_RED, "[AI Change] Group %d AI Order %d", m_iTargetType, m_iX);
	return 0;
}

#endif // MODIFY_NEW_MONSTER_AI_01_2006724

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725
