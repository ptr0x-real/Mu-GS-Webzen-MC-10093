#include "Stdafx.h"

#include "TMonsterAI.h"

#include "User.h"
#include "protocol.h"

#include "Include\ReadScript.h"
#include "Common\WinUtil.h"

#include "CrywolfUtil.h"
#include "TMonsterAIUtil.h"
#include "Crywolf.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

static TMonsterAIUtil	MONSTER_UTIL;
static CCrywolfUtil		UTIL;

TMonsterAI::TMonsterAI()
{

}

TMonsterAI::~TMonsterAI()
{

}

// 500ms 마다 작동한다.
void TMonsterAI::MonsterAIProc()
{
	TMonsterAIRule::MonsterAIRuleProc();

	for( int n=0; n<MAX_MONSTER; n++)
	{
		LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[n];
		
		if( !lpObj->m_iCurrentAI || !lpObj->Live || lpObj->Connected != CSS_GAMEPLAYING )
		{
#ifdef CRYWOLF_MONSTER_WORK_20050914
			if( lpObj->m_iCurrentAI )
			{
				// 아이템 드랍 등 메세지 처리 필요!
				MonsterStateMsgProc( lpObj->m_Index );
			}
#else
	#ifdef ADD_KANTURU_20060627
			if( lpObj->m_iCurrentAI )
			{
				// 아이템 드랍 등 메세지 처리 필요!
				MonsterStateMsgProc( lpObj->m_Index );
			}
	#endif
#endif			
			continue;
		}
		
		// 1. 몬스터 Current AI Unit를 AI Rule에 따라 변경 시킨다.
		TMonsterAI::UpdateCurrentAIUnit( lpObj->m_Index );
		
		// 2. AI 처리
		if( TMonsterAI::RunAI( lpObj->m_Index, lpObj->Class ) == _FAIL )
		{
			// AI 처리 실패 시 기존 루틴 처리!
			//gObjMonsterProcess(lpObj);
		}
		
		// 3. Agro 값을 감소 시킨다.
		// 내 시야에서 없을 경우에만 감소 시킨다.
		// lpObj->m_Agro.DecAllAgro( 1 );
		
		
	}
}

int TMonsterAI::RunAI( int iIndex, int iMonsterClass )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	// 1. 메세지 처리
	MonsterStateMsgProc( iIndex );

	// 2. 현재 AI 선택 여부를 검사한다.
	if( lpObj->m_iCurrentAI == _ZERO )
		return _FAIL;

	TMonsterAIUnit *lpAIUnit = TMonsterAIUnit::FindAIUnit( lpObj->m_iCurrentAI );

	if( !lpAIUnit )
		return _FAIL;

	// 3. AI 작동 시간을 검사한다. ( 1초당 한번 또는 적당 시간 )
	if( GetTickCount() - lpObj->m_iLastAIRunTime < lpAIUnit->m_iDelayTime )
		return _FAIL;

	// 4. AI Unit 을 작동 시킨다.
	if( lpAIUnit->RunAIUnit(iIndex) == _FAIL )
		return _FAIL;

	// 5. 마지막 AI 실행 시간을 갱신한다.
	lpObj->m_iLastAIRunTime = GetTickCount();
	
	return _SUCCESS;
}


void TMonsterAI::MonsterStateMsgProc( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	for( int i=0; i<MAXMESSAGE; i++)
	{
		if( gSMMsg[iIndex][i].MsgCode >= 0 )
		{
			// 해당 메시지가 실행될 시간이 되었다면
			if( GetTickCount() > gSMMsg[iIndex][i].MsgTime )	
			{
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
				ProcessStateMsg(lpObj, gSMMsg[iIndex][i].MsgCode, gSMMsg[iIndex][i].SendUser, gSMMsg[iIndex][i].SubCode);
#else
				ProcessStateMsg(lpObj, gSMMsg[iIndex][i].MsgCode, gSMMsg[iIndex][i].SendUser);
#endif

				gSMMsg[iIndex][i].MsgCode = _NONE;
			}
		}
	}	

}

// 레거시 몬스터 루틴 처리를 위해 필요하다.
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	void TMonsterAI::ProcessStateMsg( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex, int aMsgSubCode )
#else
	void TMonsterAI::ProcessStateMsg( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex )
#endif
{
	switch( iMsgCode )
	{
	case 0 :
		// 공격 당했다.
		// Attack 함수에서 메세지를 추가한다.
		{
			// 공격 데미지량 마다 Agro 를 증가 시켜 준다.
			// 데미지량을 알 수 없기 때문에 메세지를 보내주는 부분,
			// 즉, 공격 루틴에서 Agro를 증가 시켜 줬다.

			
			// 시야에 나타났을 때도 Agro 를 증가 시켜 준다.
	
		}
		break;
	case 1 :
		// 내가(몬스터가) 죽었다
		// gObjLifeCheck()에서 메세지를 추가한다.
		{	
			gObjMonsterDieGiveItem(lpObj, &gObj[iIndex]);

#ifdef CRYWOLF_MONSTER_WORK_20050914
//			g_Crywolf.CrywolfMonsterDieProc(lpObj->m_Index, iIndex);
#endif

		}
		break;
	case 2 :
		// 강한 공격을 당했다. (뒤로 물러난다)
		// 돌려치기 같은 스킬 공격을 당했을 때
		// Attack 함수에서 메세지를 추가한다.
		{
			if( gObj[iIndex].Live )
				gObjBackSpring(lpObj, &gObj[iIndex]);
		}
		break;
	case 3 :
		// 공격 대상자가 죽었다..
		// gObjLifeCheck()에서 메세지를 추가한다.
		{
			// Agro Del
			if( lpObj->TargetNumber != -1 )
				lpObj->m_Agro.DelAgro(lpObj->TargetNumber);

			lpObj->TargetNumber				= -1;
			lpObj->LastAttackerID			= -1;
			lpObj->m_ActState.Emotion		= 0;
			lpObj->m_ActState.Attack		= 0;
			lpObj->m_ActState.Move			= 0;
			lpObj->NextActionTime			= 1000;

			
		}
		break;
	case 4 :	
		// 공격을 당해 잠시 머뭇거린다.
		{
			lpObj->m_ActState.Emotion		= 3;
			lpObj->m_ActState.EmotionCount	= 1;
			
			// Agro Inc
		}

	case 21:
		// Agro를 증가 시킨다.
		{
			// 회복, 방어력 향상 스킬을 사용했다

			// Agro Inc
		}
		break;
	case 22:
		// Agro를 감소 시킨다
		{
			// Agro Dec
		}
	case 23:
		{
		//	회복 요청이 오면 회복 시켜준다.
		}
		break;

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	case MSG_ATTACK_NORMAL :  // 딜레이있는 일반 공격 설정
		gObjAttack(lpObj, &gObj[iIndex], 0, 0, 0);		
		//MsgOutput(iIndex, "[MSG_ATTACKPROC][NORMAL ATTACK] %s... ", lpObj->Name);
		break;
	case MSG_MONSTER_SKILL_POISON :	// 딜레이 있는 스킬 공격 설정
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[iIndex];
			// 독 스킬
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
			{
				if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
				{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					lpTargetObj->lpAttackObj = lpObj;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, aMsgSubCode );
				}
			}
	#else
		#ifdef ADD_SKILL_WITH_COMBO		
			if( lpTargetObj->m_PoisonType == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
		#else
			if( lpTargetObj->m_PoisonBeattackCount == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
			{
				if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
				{
		#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonType = AT_SKILL_POISON;
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonBeattackCount	= aMsgSubCode;	// 초 설정
					lpTargetObj->lpAttackObj			= lpObj;
					lpTargetObj->m_ViewSkillState		|= 0x01; //	1번째 비트
					GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
				}
			}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			
		}
		break;
	case MSG_MONSTER_SKILL_SPRING: // 딜레이 있는 밀기 공격
		{
			// aMsgSubCode : 거리
			LPOBJECTSTRUCT lpTargetObj = &gObj[iIndex];
			gObjBackSpring2( lpTargetObj, lpObj, aMsgSubCode );
		}
		break;
#endif // ADD_NIGHTMARE_AI_20060728
	}
}

int	TMonsterAI::UpdateCurrentAIUnit	( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !lpObj->Live )
		return _FAIL;

	int iOldCurrentAI	= lpObj->m_iCurrentAI;
	int iCurrentAI		= TMonsterAIRule::GetCurrentAIUnit(lpObj->Class);

	// 1. Rule에 의해 현재 AI Unit이 없다면 기본값 사용
	if( iCurrentAI == _ZERO )
	{
		if( lpObj->m_iBasicAI )
			iCurrentAI = lpObj->m_iBasicAI;
	}
	
	// 2. 적용
	lpObj->m_iCurrentAI = iCurrentAI;

	if( iOldCurrentAI != iCurrentAI )
	{
		// 아래 테스트 방법은 iCurrentAI 가 0 일 때 메모리 NULL 에러가 난다.
		// 테스트 시에만 사용
		UTIL.SendCrywolfChattingMsg( lpObj->m_Index, 
									 "★AI %s 에서 %s 로 바꾼다!!", 	
									 TMonsterAIUnit::FindAIUnit(iOldCurrentAI)->m_szUnitName, 
									 TMonsterAIUnit::FindAIUnit(iCurrentAI)->m_szUnitName		);

	}
	
	return _SUCCESS;
}

// 특수 AI 이동은 100ms 마다 실행한다.
void TMonsterAI::MonsterMoveProc()
{
	for( int i=0; i<MAX_MONSTER; i++)
	{		
		if( gObj[i].m_iCurrentAI )
		{
			MonsterMove( i );
		}

	}	
}

inline void TMonsterAI::MonsterMove( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !MONSTER_UTIL.CheckMovingCondition(lpObj) )
	{
		// 1) 이동 못할 경우 이동에 대한 값들 초기화
		// 2) 초기화된 위치를 클라이언트들에게 알림.

		lpObj->PathCur		= 0;
		lpObj->PathCount	= 0;
		lpObj->PathStartEnd = 0;

		ZeroMemory( lpObj->PathX,	sizeof(short) * MAX_ROADPATH );
		ZeroMemory( lpObj->PathY,	sizeof(short) * MAX_ROADPATH );
		ZeroMemory( lpObj->PathDir, sizeof(short) * MAX_ROADPATH );
		
		return;
	}

	if( lpObj->PathCount )
	{
		DWORD dwMoveTime = 0;
		DWORD dwDelayTime = 0;

		if( lpObj->DelayLevel )
		{
			dwDelayTime = 300;
		}
		else
		{
			dwDelayTime = 0;
		}
		
		lpObj->m_MoveSpeed = 300;

		if( (lpObj->PathDir[lpObj->PathCur]%2) == 0 )
		{
			dwMoveTime = (DWORD)((lpObj->m_MoveSpeed+dwDelayTime)*1.3);
		}
		else
		{
			dwMoveTime = (lpObj->m_MoveSpeed+dwDelayTime); 
		}
		
		
		if( (GetTickCount()-lpObj->PathTime) > dwMoveTime )  
		{ 
			if( lpObj->PathCur < MAX_ROADPATH ) 
			{
				lpObj->X   = lpObj->PathX[lpObj->PathCur];
				lpObj->Y   = lpObj->PathY[lpObj->PathCur];
				lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

//				if( lpObj->Type == OBJTYPE_MONSTER )
//					LogAddC( 2, " ★ 탄탈로스 좌표 = ( %d, %d ) ", lpObj->X, lpObj->Y );

				lpObj->PathTime = GetTickCount();
				lpObj->PathCur++;
			
				if( lpObj->PathCur >= lpObj->PathCount ) 
				{
					lpObj->PathCur		= 0;
					lpObj->PathCount	= 0;
					lpObj->PathStartEnd = 0;
				}

			}
			
			CreateFrustrum(lpObj->X,lpObj->Y, iIndex);
		}
					
	}
	else
	{
		lpObj->PathCur		= 0;
		lpObj->PathCount	= 0;
		lpObj->PathStartEnd = 0;

		ZeroMemory( lpObj->PathX,	sizeof(short) * MAX_ROADPATH );
		ZeroMemory( lpObj->PathY,	sizeof(short) * MAX_ROADPATH );
		ZeroMemory( lpObj->PathDir, sizeof(short) * MAX_ROADPATH );
	}
				
}


#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725