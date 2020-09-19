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

	m_lpAIClassNormal	= NULL;		// �Ϲ� �ൿ �Ӽ�
	m_lpAIClassMove		= NULL;		// �̵� �ൿ �Ӽ�
	m_lpAIClassAttack	= NULL;		// ���� �ൿ �Ӽ�
	m_lpAIClassHeal		= NULL;		// ġ�� �ൿ �Ӽ�
	m_lpAIClassAvoid	= NULL;		// ȸ�� �ൿ �Ӽ�
	m_lpAIClassHelp		= NULL;		// ���� �ൿ �Ӽ�
	m_lpAIClassSpecial	= NULL;		// Ư�� �ൿ �Ӽ�

	m_lpAIClassEvent	= NULL;		// �ó����� �ൿ �Ӽ�
	
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
					// 1. >	������ �ʱ�ȭ
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
				
					// 2. >	������ �б�
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
				
					// 3. >	������ ��ȿ�� �˻�
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

					// 4. > ������ ����
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
	// 1. > ���丶Ÿ �۵�
	//
	//----------------------------------------------------------------------------------------

	// ���� �ൿ �Ӽ� Ŭ���� -> ���� �ൿ �Ӽ� Ŭ����
	// ���� ���� ���� Type�� ���� ����Ѵ�.

	// ���丶Ÿ �۵� �ð��� Delay�� ��� �ϴ°�? - �ϴ� ������� ����.

	if( m_lpAutomata == NULL )
		return _FAIL;

#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
	// ���丶Ÿ �۵� �ð� �˻�
	if( GetTickCount() - lpObj->m_iLastAutomataRuntime < lpObj->m_iLastAutomataDelay )
	{
	#if TESTSERVER == 1
		DebugLog("[AI Automata] [%s] Delay Error : Automata ���� ���� %d Delay %d Gap:%d\n", 
			lpObj->Name,
			lpObj->m_iCurrentAIState,
			lpObj->m_iLastAutomataDelay, GetTickCount() - lpObj->m_iLastAutomataRuntime );
	#endif
		return _FAIL;
	}
	else
	{
	#if TESTSERVER == 1
		DebugLog("[AI Automata] [%s] Delay OK : Automata ���� ���� %d Delay %d %d\n", 
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
	// ������ Automata ���� �ð��� �����Ѵ�.
	lpObj->m_iLastAutomataRuntime = GetTickCount();
#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907

	//----------------------------------------------------------------------------------------

	

	//----------------------------------------------------------------------------------------
	//
	// 2. > ���� �ൿ �Ӽ��� ���� AI �ൿ �Ӽ� APPLY
	//
	//----------------------------------------------------------------------------------------
	
	// Ư�� �ൿ �Ӽ��� �ִٸ� ������ ���� �����Ѵ�. 
	//		- ���� ���µ� ó���� ���ؼ�!!!
	//		- �̺�Ʈ ���ῡ ���� ���� �������
	// Ư�� �ൿ �Ӽ� ���!!!
	//		- Ư�� �ൿ �Ӽ� ���� : ��ũ������ Ư�� �ൿ ó�� �����ϰ� �Ѵ�. CrywolfDarkElfAct()
	
	// Ư�� �ൿ �Ӽ��� Ư�� �Լ��� Call ���ְ� �� �Լ��� ó���� �־�д�. (��ȯ���)
	
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
