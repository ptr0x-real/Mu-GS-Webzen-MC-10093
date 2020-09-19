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

// 500ms ���� �۵��Ѵ�.
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
				// ������ ��� �� �޼��� ó�� �ʿ�!
				MonsterStateMsgProc( lpObj->m_Index );
			}
#else
	#ifdef ADD_KANTURU_20060627
			if( lpObj->m_iCurrentAI )
			{
				// ������ ��� �� �޼��� ó�� �ʿ�!
				MonsterStateMsgProc( lpObj->m_Index );
			}
	#endif
#endif			
			continue;
		}
		
		// 1. ���� Current AI Unit�� AI Rule�� ���� ���� ��Ų��.
		TMonsterAI::UpdateCurrentAIUnit( lpObj->m_Index );
		
		// 2. AI ó��
		if( TMonsterAI::RunAI( lpObj->m_Index, lpObj->Class ) == _FAIL )
		{
			// AI ó�� ���� �� ���� ��ƾ ó��!
			//gObjMonsterProcess(lpObj);
		}
		
		// 3. Agro ���� ���� ��Ų��.
		// �� �þ߿��� ���� ��쿡�� ���� ��Ų��.
		// lpObj->m_Agro.DecAllAgro( 1 );
		
		
	}
}

int TMonsterAI::RunAI( int iIndex, int iMonsterClass )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	// 1. �޼��� ó��
	MonsterStateMsgProc( iIndex );

	// 2. ���� AI ���� ���θ� �˻��Ѵ�.
	if( lpObj->m_iCurrentAI == _ZERO )
		return _FAIL;

	TMonsterAIUnit *lpAIUnit = TMonsterAIUnit::FindAIUnit( lpObj->m_iCurrentAI );

	if( !lpAIUnit )
		return _FAIL;

	// 3. AI �۵� �ð��� �˻��Ѵ�. ( 1�ʴ� �ѹ� �Ǵ� ���� �ð� )
	if( GetTickCount() - lpObj->m_iLastAIRunTime < lpAIUnit->m_iDelayTime )
		return _FAIL;

	// 4. AI Unit �� �۵� ��Ų��.
	if( lpAIUnit->RunAIUnit(iIndex) == _FAIL )
		return _FAIL;

	// 5. ������ AI ���� �ð��� �����Ѵ�.
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
			// �ش� �޽����� ����� �ð��� �Ǿ��ٸ�
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

// ���Ž� ���� ��ƾ ó���� ���� �ʿ��ϴ�.
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	void TMonsterAI::ProcessStateMsg( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex, int aMsgSubCode )
#else
	void TMonsterAI::ProcessStateMsg( LPOBJECTSTRUCT lpObj, int iMsgCode, int iIndex )
#endif
{
	switch( iMsgCode )
	{
	case 0 :
		// ���� ���ߴ�.
		// Attack �Լ����� �޼����� �߰��Ѵ�.
		{
			// ���� �������� ���� Agro �� ���� ���� �ش�.
			// ���������� �� �� ���� ������ �޼����� �����ִ� �κ�,
			// ��, ���� ��ƾ���� Agro�� ���� ���� ���.

			
			// �þ߿� ��Ÿ���� ���� Agro �� ���� ���� �ش�.
	
		}
		break;
	case 1 :
		// ����(���Ͱ�) �׾���
		// gObjLifeCheck()���� �޼����� �߰��Ѵ�.
		{	
			gObjMonsterDieGiveItem(lpObj, &gObj[iIndex]);

#ifdef CRYWOLF_MONSTER_WORK_20050914
//			g_Crywolf.CrywolfMonsterDieProc(lpObj->m_Index, iIndex);
#endif

		}
		break;
	case 2 :
		// ���� ������ ���ߴ�. (�ڷ� ��������)
		// ����ġ�� ���� ��ų ������ ������ ��
		// Attack �Լ����� �޼����� �߰��Ѵ�.
		{
			if( gObj[iIndex].Live )
				gObjBackSpring(lpObj, &gObj[iIndex]);
		}
		break;
	case 3 :
		// ���� ����ڰ� �׾���..
		// gObjLifeCheck()���� �޼����� �߰��Ѵ�.
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
		// ������ ���� ��� �ӹ��Ÿ���.
		{
			lpObj->m_ActState.Emotion		= 3;
			lpObj->m_ActState.EmotionCount	= 1;
			
			// Agro Inc
		}

	case 21:
		// Agro�� ���� ��Ų��.
		{
			// ȸ��, ���� ��� ��ų�� ����ߴ�

			// Agro Inc
		}
		break;
	case 22:
		// Agro�� ���� ��Ų��
		{
			// Agro Dec
		}
	case 23:
		{
		//	ȸ�� ��û�� ���� ȸ�� �����ش�.
		}
		break;

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	case MSG_ATTACK_NORMAL :  // �������ִ� �Ϲ� ���� ����
		gObjAttack(lpObj, &gObj[iIndex], 0, 0, 0);		
		//MsgOutput(iIndex, "[MSG_ATTACKPROC][NORMAL ATTACK] %s... ", lpObj->Name);
		break;
	case MSG_MONSTER_SKILL_POISON :	// ������ �ִ� ��ų ���� ����
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[iIndex];
			// �� ��ų
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
			if( lpTargetObj->m_PoisonType == 0 ) // ���� �ߵ� ������ �ɷ��ִٸ� �ٽ� �ɸ��� �ʰ�..
		#else
			if( lpTargetObj->m_PoisonBeattackCount == 0 ) // ���� �ߵ� ������ �ɷ��ִٸ� �ٽ� �ɸ��� �ʰ�..
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
			{
				if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
				{
		#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonType = AT_SKILL_POISON;
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonBeattackCount	= aMsgSubCode;	// �� ����
					lpTargetObj->lpAttackObj			= lpObj;
					lpTargetObj->m_ViewSkillState		|= 0x01; //	1��° ��Ʈ
					GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
				}
			}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004			
		}
		break;
	case MSG_MONSTER_SKILL_SPRING: // ������ �ִ� �б� ����
		{
			// aMsgSubCode : �Ÿ�
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

	// 1. Rule�� ���� ���� AI Unit�� ���ٸ� �⺻�� ���
	if( iCurrentAI == _ZERO )
	{
		if( lpObj->m_iBasicAI )
			iCurrentAI = lpObj->m_iBasicAI;
	}
	
	// 2. ����
	lpObj->m_iCurrentAI = iCurrentAI;

	if( iOldCurrentAI != iCurrentAI )
	{
		// �Ʒ� �׽�Ʈ ����� iCurrentAI �� 0 �� �� �޸� NULL ������ ����.
		// �׽�Ʈ �ÿ��� ���
		UTIL.SendCrywolfChattingMsg( lpObj->m_Index, 
									 "��AI %s ���� %s �� �ٲ۴�!!", 	
									 TMonsterAIUnit::FindAIUnit(iOldCurrentAI)->m_szUnitName, 
									 TMonsterAIUnit::FindAIUnit(iCurrentAI)->m_szUnitName		);

	}
	
	return _SUCCESS;
}

// Ư�� AI �̵��� 100ms ���� �����Ѵ�.
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
		// 1) �̵� ���� ��� �̵��� ���� ���� �ʱ�ȭ
		// 2) �ʱ�ȭ�� ��ġ�� Ŭ���̾�Ʈ�鿡�� �˸�.

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
//					LogAddC( 2, " �� źŻ�ν� ��ǥ = ( %d, %d ) ", lpObj->X, lpObj->Y );

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