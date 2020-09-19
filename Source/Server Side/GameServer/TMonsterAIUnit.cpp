#include "Stdafx.h"
#include "TMonsterAIUnit.h"

#include "Include\ReadScript.h"

#include "User.h"
#include "protocol.h"
#include "gObjMonster.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725


BOOL			TMonsterAIUnit::s_bDataLoad = FALSE;
TMonsterAIUnit	TMonsterAIUnit::s_MonsterAIUnitArray[MAX_MONSTER_AI_UNIT];


TMonsterAIUnit::TMonsterAIUnit()
{
	Reset();
}

TMonsterAIUnit::~TMonsterAIUnit()
{

}

void TMonsterAIUnit::Reset()
{	
	m_iUnitNumber		= _INVALID;
	m_iDelayTime		= _ZERO;
	
	m_lpAutomata		= NULL;		// AutoMata

	m_lpAIClassNormal	= NULL;		// 일반 행동 속성
	m_lpAIClassMove		= NULL;		// 이동 행동 속성
	m_lpAIClassAttack	= NULL;		// 공격 행동 속성
	m_lpAIClassHeal		= NULL;		// 치료 행동 속성
	m_lpAIClassAvoid	= NULL;		// 회피 행동 속성
	m_lpAIClassHelp		= NULL;		// 도움 행동 속성
	m_lpAIClassSpecial	= NULL;		// 특수 행동 속성

	m_lpAIClassEvent	= NULL;		// 시나리오 행동 속성
	
	ZeroMemory( m_szUnitName, MAX_AIUNITNAME );


	m_lppAIClassMap[MAE_STATE_NORMAL]	= &m_lpAIClassNormal;
	m_lppAIClassMap[MAE_STATE_MOVE]		= &m_lpAIClassMove;
	m_lppAIClassMap[MAE_STATE_ATTACK]	= &m_lpAIClassAttack;
	m_lppAIClassMap[MAE_STATE_HEAL]		= &m_lpAIClassHeal;
	m_lppAIClassMap[MAE_STATE_AVOID]	= &m_lpAIClassAvoid;
	m_lppAIClassMap[MAE_STATE_HELP]		= &m_lpAIClassHelp;
	m_lppAIClassMap[MAE_STATE_SPECIAL]	= &m_lpAIClassSpecial;
	m_lppAIClassMap[MAE_STATE_EVENT]	= &m_lpAIClassEvent;

}

int TMonsterAIUnit::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Unit] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Unit] - Can't Open %s ", lpszFileName);
			return false;
		}

		DelAllAIUnit();
		
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
					char	szUnitName[MAX_AIUNITNAME] = {0,};

					int		iUnitNumber		= _INVALID;
					int		iDelayTime		= _ZERO;
					int		iAutomata		= _INVALID;
					int		iAIClassNormal	= _INVALID;
					int		iAIClassMove	= _INVALID;
					int		iAIClassAttack	= _INVALID;
					int		iAIClassHeal	= _INVALID;
					int		iAIClassAvoid	= _INVALID;
					int		iAIClassHelp	= _INVALID;
					int		iAIClassSpecial	= _INVALID;
					int		iAIClassEvent	= _INVALID;
				
					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iUnitNumber	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(szUnitName, TokenString, MAX_SKILLELEMENTNAME);
					Token = GetToken(); iDelayTime		= (int)TokenNumber;
					Token = GetToken(); iAutomata		= (int)TokenNumber;
					Token = GetToken(); iAIClassNormal	= (int)TokenNumber;
					Token = GetToken(); iAIClassMove	= (int)TokenNumber;
					Token = GetToken(); iAIClassAttack	= (int)TokenNumber;
					Token = GetToken(); iAIClassHeal	= (int)TokenNumber;
					Token = GetToken(); iAIClassAvoid	= (int)TokenNumber;
					Token = GetToken(); iAIClassHelp	= (int)TokenNumber;
					Token = GetToken(); iAIClassSpecial	= (int)TokenNumber;
					Token = GetToken(); iAIClassEvent	= (int)TokenNumber;
				
					// 3. >	데이터 유효성 검사
					if( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
					{
						MsgBox("[Monster AI Unit] - UnitNumber(%d) Error (%s) File. ", iUnitNumber, lpszFileName);
						continue;
					}
					if( iAutomata < 0 || iAutomata >= MAX_MONSTERAI_AUTOMATA )
					{
						MsgBox("[Monster AI Unit] - AutomatNumber(%d) Error (%s) File. ", iAutomata, lpszFileName);
						continue;
					}

					// 4. > 데이터 세팅
					s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber		= iUnitNumber;
					s_MonsterAIUnitArray[iUnitNumber].m_iDelayTime		= iDelayTime;
					s_MonsterAIUnitArray[iUnitNumber].m_lpAutomata		= TMonsterAIAutomata::FindAutomata(iAutomata);
					
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassNormal	= TMonsterAIElement::FindAIElement(iAIClassNormal);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassMove	= TMonsterAIElement::FindAIElement(iAIClassMove);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAttack	= TMonsterAIElement::FindAIElement(iAIClassAttack);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHeal	= TMonsterAIElement::FindAIElement(iAIClassHeal);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassAvoid	= TMonsterAIElement::FindAIElement(iAIClassAvoid);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassHelp	= TMonsterAIElement::FindAIElement(iAIClassHelp);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassSpecial= TMonsterAIElement::FindAIElement(iAIClassSpecial);
					s_MonsterAIUnitArray[iUnitNumber].m_lpAIClassEvent	= TMonsterAIElement::FindAIElement(iAIClassEvent);
					
					CopyMemory( s_MonsterAIUnitArray[iUnitNumber].m_szUnitName, szUnitName, MAX_AIUNITNAME );
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI Unit] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Unit] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	 TMonsterAIUnit::DelAllAIUnit()
{
	for( int i=0; i<MAX_MONSTER_AI_UNIT; i++ )
	{
		s_MonsterAIUnitArray[i].Reset();
	}

	return 0;
}

TMonsterAIUnit* TMonsterAIUnit::FindAIUnit( int iUnitNumber )
{
	if( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_AI_UNIT )
	{
		LogAddTD("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ", iUnitNumber);
		return NULL;
	}

	if( s_MonsterAIUnitArray[iUnitNumber].m_iUnitNumber == iUnitNumber )
	{
		return &s_MonsterAIUnitArray[iUnitNumber];
	}
	else
		LogAddTD("[Monster AI Unit] FindAIUnit() Error - (UnitNumber=%d) ", iUnitNumber);
	
	return NULL;
}

int TMonsterAIUnit::RunAIUnit( int iIndex )
{	
	LPOBJECTSTRUCT lpObj = &gObj[iIndex];

	//----------------------------------------------------------------------------------------
	//
	// 1. > 오토마타 작동
	//
	//----------------------------------------------------------------------------------------

	// 현재 행동 속성 클래스 -> 다음 행동 속성 클래스
	// 상태 전이 조건 Type도 같이 사용한다.

	// 오토마타 작동 시간에 Delay를 줘야 하는가? - 일단 사용하지 않음.

	if( m_lpAutomata == NULL )
		return _FAIL;

#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
	// 오토마타 작동 시간 검사
	if( GetTickCount() - lpObj->m_iLastAutomataRuntime < lpObj->m_iLastAutomataDelay )
	{
	#if TESTSERVER == 1
		DebugLog("[AI Automata] [%s] Delay Error : Automata 현재 상태 %d Delay %d Gap:%d\n", 
			lpObj->Name,
			lpObj->m_iCurrentAIState,
			lpObj->m_iLastAutomataDelay, GetTickCount() - lpObj->m_iLastAutomataRuntime );
	#endif
		return _FAIL;
	}
	else
	{
	#if TESTSERVER == 1
		DebugLog("[AI Automata] [%s] Delay OK : Automata 현재 상태 %d Delay %d %d\n", 
			lpObj->Name,
			lpObj->m_iCurrentAIState,
			GetTickCount() - lpObj->m_iLastAutomataRuntime,
			lpObj->m_iLastAutomataDelay );
	#endif
	}
#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
	
	TMonsterAIState *pAIState = m_lpAutomata->RunAutomata( iIndex );
	
	if( pAIState == NULL ) 
		return _FAIL;

#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
	// 마지막 Automata 실행 시간을 갱신한다.
	lpObj->m_iLastAutomataRuntime = GetTickCount();
#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907

	//----------------------------------------------------------------------------------------

	

	//----------------------------------------------------------------------------------------
	//
	// 2. > 다음 행동 속성에 대한 AI 행동 속성 APPLY
	//
	//----------------------------------------------------------------------------------------
	
	// 특수 행동 속성이 있다면 무조건 먼저 적용한다. 
	//		- 무적 상태등 처리를 위해서!!!
	//		- 이벤트 종료에 따른 몬스터 사라지기
	// 특수 행동 속성 사용!!!
	//		- 특수 행동 속성 적용 : 다크엘프는 특수 행동 처리 가능하게 한다. CrywolfDarkElfAct()
	
	// 특수 행동 속성은 특정 함수를 Call 해주고 그 함수에 처리를 넣어둔다. (소환등등)
	
	TMonsterAIElement* pAIElement = *m_lppAIClassMap[pAIState->m_iNextState];
	
	if( pAIElement == NULL )
		return _FAIL;

	int iRetExec = pAIElement->ForceAIElement( iIndex, 0, pAIState );

	if( iRetExec == _FAIL )
		return _FAIL;
	//----------------------------------------------------------------------------------------


	lpObj->m_iCurrentAIState = pAIState->m_iNextState;

	return _SUCCESS;
	
}

#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725
