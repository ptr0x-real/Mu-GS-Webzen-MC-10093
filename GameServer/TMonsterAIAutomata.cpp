#include "Stdafx.h"

#include "TMonsterAIAutomata.h"
#include "TMonsterAIUtil.h"
#include "TMonsterAIGroup.h"

#include "User.h"
#include "..\Include\ReadScript.h"
#include "..\Common\WinUtil.h"



#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

BOOL				TMonsterAIAutomata::s_bDataLoad = FALSE;
TMonsterAIAutomata	TMonsterAIAutomata::s_MonsterAIAutomataArray[MAX_MONSTERAI_AUTOMATA];

static TMonsterAIUtil	MONSTER_UTIL;

TMonsterAIAutomata::TMonsterAIAutomata()
{
	Reset();
}

TMonsterAIAutomata::~TMonsterAIAutomata()
{

}

void TMonsterAIAutomata::Reset()
{	
	for( int i=0; i<MAX_MONSTER_AI_ELEMENT_STATE; i++ )
	{
		for( int j=0; j<MAX_MONSTERAI_STATE_TRANSITION_INFO; j++ )

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		{
			m_AIState[i][j].Reset();
		}
	#ifndef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907 // not define
			m_dwTickAutomata		  = 0;
			m_dwTickNextAutomataDelay = 0;
	#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
#else
			m_AIState[i][j].Reset();
#endif

	}

	for( int k=0; k<MAX_MONSTER_AI_ELEMENT_STATE; k++ )
		m_AIStateTransCount[k]	= 0;

	m_iAutomataNumber		= _INVALID;	
}

int TMonsterAIAutomata::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Automata] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Automata] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllAutomata();

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
					
					// 1. >	������ �ʱ�ȭ
					char	szStateTransitionDesc[MAX_MONSTERAI_STATE_TRANS_DESC] = {0,};

					int		iAutomataNumber			= _INVALID;
					int		iPriority				= _INVALID;
					int		iCurrentState			= _INVALID;
					int		iNextState				= _INVALID;
					int		iTransitionType			= _INVALID;
					int		iTransitionRate			= _INVALID;
					int		iTransitionValueType	= _INVALID;
					int		iTransitionValue		= _INVALID;
					int		iDelayTime				= _INVALID;
						
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iAutomataNumber	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(szStateTransitionDesc, TokenString, MAX_MONSTERAI_STATE_TRANS_DESC);
					Token = GetToken(); iPriority				= (int)TokenNumber;
					Token = GetToken(); iCurrentState			= (int)TokenNumber;
					Token = GetToken(); iNextState				= (int)TokenNumber;
					Token = GetToken(); iTransitionType			= (int)TokenNumber;
					Token = GetToken(); iTransitionRate			= (int)TokenNumber;
					Token = GetToken(); iTransitionValueType	= (int)TokenNumber;
					Token = GetToken(); iTransitionValue		= (int)TokenNumber;
					Token = GetToken(); iDelayTime				= (int)TokenNumber;
					
				
					// 3. >	������ ��ȿ�� �˻�
					if( iAutomataNumber < 0 || iAutomataNumber >= MAX_MONSTERAI_AUTOMATA )
					{
						MsgBox("[Monster AI Automata] - AutomataNumber(%d) Error (%s) File. ", iAutomataNumber, lpszFileName);
						continue;
					}
					if( iCurrentState < 0 || iCurrentState >= MAX_MONSTER_AI_ELEMENT_STATE )
					{
						MsgBox("[Monster AI Automata] - CurrentState(%d) Error (%s) File. ", iCurrentState, lpszFileName);
						continue;
					}
					if( iNextState < 0 || iNextState >= MAX_MONSTER_AI_ELEMENT_STATE )
					{
						MsgBox("[Monster AI Automata] - NextState(%d) Error (%s) File. ", iNextState, lpszFileName);
						continue;
					}
					if( iPriority < 0 || iPriority >= MAX_MONSTERAI_STATE_TRANSITION_INFO )
					{
						MsgBox("[Monster AI Automata] - Priority(%d) Error (%s) File. ", iPriority, lpszFileName);
						continue;
					}

				
					// 4. > ������ ����
					TMonsterAIState & AIState = s_MonsterAIAutomataArray[iAutomataNumber].m_AIState[iCurrentState][iPriority];
					
					AIState.m_iPriority				= iPriority;
					AIState.m_iCurrentState			= iCurrentState;
					AIState.m_iNextState			= iNextState;
					AIState.m_iTransitionType		= iTransitionType;
					AIState.m_iTransitionRate		= iTransitionRate;
					AIState.m_iTransitionValueType	= iTransitionValueType;
					AIState.m_iTransitionValue		= iTransitionValue;
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
					AIState.m_iDelayTime			= iDelayTime;
#endif
					CopyMemory( AIState.m_szStateTransitionDesc, szStateTransitionDesc, MAX_MONSTERAI_STATE_TRANS_DESC );

					LogAddC( 2, "[Monster AI Automata] Loaded Transition Desc . [ %s ]", AIState.m_szStateTransitionDesc);
					
					s_MonsterAIAutomataArray[iAutomataNumber].m_iAutomataNumber = iAutomataNumber;
					s_MonsterAIAutomataArray[iAutomataNumber].m_AIStateTransCount[iCurrentState]++;
				}	
			}
		}
		fclose(SMDFile);
	
		LogAddC( 2, "[Monster AI Automata] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Automata] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	 TMonsterAIAutomata::DelAllAutomata()
{
	for( int i=0; i<MAX_MONSTERAI_AUTOMATA; i++ )
	{
		s_MonsterAIAutomataArray[i].Reset();
	}

	return 0;
}

TMonsterAIAutomata* TMonsterAIAutomata::FindAutomata( int iAutomataNumber )
{
	if( iAutomataNumber < 0 || iAutomataNumber >= MAX_MONSTERAI_AUTOMATA )
	{
		LogAddTD("[Monster AI Automata] FindAutomata() Error - (AutomataNumber=%d) ", iAutomataNumber);
		return NULL;
	}

	if( s_MonsterAIAutomataArray[iAutomataNumber].m_iAutomataNumber == iAutomataNumber )
	{
		return &s_MonsterAIAutomataArray[iAutomataNumber];
	}
	else
		LogAddTD("[Monster AI Automata] FindAutomata() Error - (AutomataNumber=%d) ", iAutomataNumber);
	
	return NULL;
}

TMonsterAIState* TMonsterAIAutomata::RunAutomata( int iIndex )
{
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	if( !CHECK_LIMIT( lpObj->m_iCurrentAIState, MAX_MONSTER_AI_ELEMENT_STATE ) )
		return NULL;
	
	//-----------------------------------------------------------------------------------------
	//
	//	�� ���� ���� �Ǵ� ���� �غ�
	//
	//-----------------------------------------------------------------------------------------

	LPOBJECTSTRUCT lpTargetObj = NULL;
	int iMaxAgro = _INVALID;
	
	// [ 1. Target ���� ] - Agro ���� �̿��ؼ� Ÿ���� ���� �Ѵ�.
	int iTargetIndex = lpObj->m_Agro.GetMaxAgroUserIndex(lpObj->m_Index);
	
#ifdef KANTURU_TEST_200607119
	DebugLog("%s�� ��׷� %d Ÿ�� : %d", lpObj->Name, lpObj->m_Agro.GetAgro(iTargetIndex), iTargetIndex);
#endif
	
	if( iTargetIndex != _INVALID )
	{
		lpTargetObj = &gObj[iTargetIndex];

		// �� �þ߿� Target �� �ִ°�?
		if( MONSTER_UTIL.FindMonViewportObj( lpObj->m_Index, lpTargetObj->m_Index ) )
		{
			iMaxAgro = lpObj->m_Agro.GetAgro( iTargetIndex );
//			MONSTER_UTIL.SendChattingMsg( lpObj->m_Index, "[AM]Ÿ�ټ���:%s (Agro:%d)", lpTargetObj->Name, iMaxAgro);
//			MsgOutput( lpTargetObj->m_Index, "%s(%d) �� Ÿ���� ��", lpObj->Name,lpObj->m_Index );

#ifdef KANTURU_TEST_200607119
			DebugLog("%s(%d) �� Ÿ���� ��", lpObj->Name,lpObj->m_Index );
#endif
			
			lpObj->TargetNumber = iTargetIndex;
			
		}
		else
		{
			lpObj->TargetNumber = _INVALID;
			lpTargetObj			= NULL;
		}

	}	

	//-----------------------------------------------------------------------------------------
	//
	//	�� ���� ���� ����
	//
	//-----------------------------------------------------------------------------------------

	BOOL bRateSuccess = FALSE;

	// ���� ����
	int	iCurrentState = lpObj->m_iCurrentAIState;
	
	// ���� ���¿����� �ִ� ���� ���� ���� ��´�
	int	iMaxStateTransCount = m_AIStateTransCount[iCurrentState];

	// Priority ������ �����Ѵ�. ( 0 �� ���� ���� �켱 �����̴�. )
	// Priority ������ ���� ���·� Transition �������� �˻��Ѵ�.
	for( int iPriority=0; iPriority<iMaxStateTransCount; iPriority++ )
	{
		TMonsterAIState& AIState = m_AIState[iCurrentState][iPriority];
		BOOL bTransition = FALSE;
		
		switch( AIState.m_iTransitionType )
		{
		case MAI_STATE_TRANS_NO_ENEMY	:	// �� ����
			if( lpTargetObj == NULL )
			{
				bTransition = TRUE;
			}
			break;

		case MAI_STATE_TRANS_IN_ENEMY	:	// ���� ���� �� �� �߰�
			if( lpTargetObj != NULL )
			{
				if( gObjCalDistance(lpObj, lpTargetObj) <= lpObj->m_AttackRange )
				{
					bTransition = TRUE;
				}
			}
			break;

		case MAI_STATE_TRANS_OUT_ENEMY	:	// ���� ���� �� �� �߰�
			if( lpTargetObj != NULL )
			{	
				if( gObjCalDistance(lpObj, lpTargetObj) > lpObj->m_AttackRange )
				{
					bTransition = TRUE;
				}
			}
			break;

		case MAI_STATE_TRANS_DEC_HP		:	// HP ����
			if( lpObj->Life < AIState.m_iTransitionValue )
			{
				bTransition = TRUE;
			}
			break;
			
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		case MAI_STATE_TRANS_IMMEDIATELY : // ������ ����
			bTransition = TRUE;
			break;
		case MAI_STATE_TRANS_DEC_HP_PER	: // HP % ����
			{
				AIState.m_iTransitionValue = max(AIState.m_iTransitionValue, 0);
				AIState.m_iTransitionValue = min(AIState.m_iTransitionValue, 100);
				int iLife = (lpObj->MaxLife + lpObj->AddLife) * (AIState.m_iTransitionValue) / 100;
				if ( lpObj->Life < iLife )
				{
					bTransition = TRUE;
				}
			}
			break;
#endif

		case MAI_STATE_TRANS_AGRO_UP	:	// Agro UP
			if( lpTargetObj != NULL )
			{
				if( iMaxAgro >= AIState.m_iTransitionValue )
				{
					bTransition = TRUE;
				}
			}
			break;

		case MAI_STATE_TRANS_AGRO_DOWN	:	// Agro DOWN
			if( lpTargetObj != NULL )
			{
				if( iMaxAgro <= AIState.m_iTransitionValue )
				{
					bTransition = TRUE;
				}
			}
			break;

		case MAI_STATE_TRANS_GROUP_SOMMON : // �׷� ��ȯ
			if( lpObj->m_iGroupNumber )
			{
				if( TMonsterAIGroup::FindGroupMemberToSommon(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid ) )
				{
					bTransition = TRUE;
				}
			}

		case MAI_STATE_TRANS_GROUP_HEAL :	// �׷� ġ��
			if( lpObj->m_iGroupNumber )
			{
				if( TMonsterAIGroup::FindGroupMemberToHeal(lpObj->m_Index,lpObj->m_iGroupNumber,lpObj->m_iGroupMemberGuid, 5) )
				{
					bTransition = TRUE;
				}
			}
			break;
		}

		// ���� ���� �Ұ��� : continue
		if( !bTransition )
			continue;

#ifndef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907 // not define
	#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
			DWORD dwTick = GetTickCount() - m_dwTickAutomata;
			if ( dwTick < m_dwTickNextAutomataDelay )
			{
	#if TESTSERVER == 1
				DebugLog("%s Delay %d, %d\n", AIState.m_szStateTransitionDesc , m_dwTickNextAutomataDelay , dwTick );
	#endif
				continue;
			}
	#endif
#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907

		// 4. ���� ���� Ȯ�� ����
		if( rand() % 100 < AIState.m_iTransitionRate )
		{
			// 5. ���� ���� ���� �� Break
			bRateSuccess = TRUE;

			// ��ü Transition & State ������ Return �����ν� "�Ǵ�"�� �ٰŸ� ��´�.
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		#if TESTSERVER == 1
			DebugLog("[State Trans.] Success BY %% Probability - %s %d  Delay:%d\n", AIState.m_szStateTransitionDesc, iPriority, AIState.m_iDelayTime);
		#endif
		
	#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
			lpObj->m_iLastAutomataDelay = AIState.m_iDelayTime; // ���̵� Automata�� Delay�� ����.
	#else
			m_dwTickAutomata = GetTickCount();
			m_dwTickNextAutomataDelay = AIState.m_iDelayTime;
	#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
#else
			//LogAddC( LOGC_BLUE, "[State Trans.] Success BY %% Probability - %s %d", AIState.m_szStateTransitionDesc, iPriority);
#endif
			return &m_AIState[iCurrentState][iPriority];
		}
		else
		{
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		#if TESTSERVER == 1
			DebugLog("[State Trans.] Fail BY %% Probability - %s %d\n", AIState.m_szStateTransitionDesc, iPriority);
		#endif
#endif
			// ���� ���¸� ������.
			//LogAddC( LOGC_LIGHTRED, "[State Trans.] Failed BY %% Probability - %s ", AIState.m_szStateTransitionDesc );
		}

	}


	// 6. ���� ���� ���� �� �α� ���
	if( !bRateSuccess )
	{
		//LogAddC( LOGC_RED, "[Monster AI Automata] State Transition Failed by Probability - %s ", lpObj->Name );
	}

	return NULL;
}

#endif ADD_NEW_MONSTER_SYSTEM_02_20050725
