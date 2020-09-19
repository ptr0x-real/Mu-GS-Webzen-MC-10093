#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIRule.h"

#include "..\Include\ReadScript.h"
#include "..\Common\WinUtil.h"

BOOL				TMonsterAIRule::s_bDataLoad = FALSE;
int					TMonsterAIRule::s_iMonsterCurrentAIUnitTable[MAX_MONSTER_CLASS];

int					TMonsterAIRule::s_iMonsterAIRuleInfoCount;
TMonsterAIRuleInfo	TMonsterAIRule::s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULEINFO];

TMonsterAIRule::TMonsterAIRule()
{
	DelAllAIRule();
}

TMonsterAIRule::~TMonsterAIRule()
{

}

int TMonsterAIRule::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Rule] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Rule] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllAIRule();

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
					int	iAIRuleNumber = _INVALID;
					int	iMonsterClass = _INVALID;
					TMonsterAIRuleInfo	MonsterAIRuleInfo;
				
					// 2. >	������ �б�
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iAIRuleNumber	= (int)TokenNumber;
					
					Token = GetToken(); iMonsterClass	= (int)TokenNumber;
					Token = GetToken(); CopyMemory(MonsterAIRuleInfo.m_szRuleDesc, TokenString, MAX_MONSTERAI_RULEDESC);

					Token = GetToken(); MonsterAIRuleInfo.m_iMonsterAIUnit		= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iRuleCondition		= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iWaitTime			= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iContinuanceTime	= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iMonth				= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iDay				= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iWeekDay			= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iHour				= (int)TokenNumber;
					Token = GetToken(); MonsterAIRuleInfo.m_iMinute				= (int)TokenNumber;
				
					// 3. >	������ ��ȿ�� �˻�
					if( iAIRuleNumber < 0 || iAIRuleNumber >= MAX_MONSTER_AI_RULEINFO )
					{
						MsgBox("[Monster AI Rule] - AIRuleNumber(%d) Error (%s) File. ", iAIRuleNumber, lpszFileName);
						continue;
					}
					if( iMonsterClass < 0 || iMonsterClass >= MAX_MONSTER_CLASS )
					{
						MsgBox("[Monster AI Rule] - MonsterClass(%d) Error (%s) File. ", iMonsterClass, lpszFileName);
						continue;
					}

					// 4. > ������ ����
					MonsterAIRuleInfo.m_iRuleNumber			= iAIRuleNumber;
					MonsterAIRuleInfo.m_iMonsterClass		= iMonsterClass;
					
					// Find �� �ʿ䰡 ���� ������ �迭�� s_iMonsterAIRuleInfoCount�� �̿��ؼ� ����
					// m_iRuleNumber�� Key�� ������� �ʴ´�. ����!
					CopyMemory( &s_MonsterAIRuleInfoArray[s_iMonsterAIRuleInfoCount], &MonsterAIRuleInfo, sizeof(MonsterAIRuleInfo));
					s_iMonsterAIRuleInfoCount++;
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI Rule ] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Rule] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	TMonsterAIRule::DelAllAIRule()
{
	for( int i=0; i<MAX_MONSTER_AI_RULEINFO; i++ )
	{
		s_MonsterAIRuleInfoArray[i].Reset();
	}

	for( int j=0; j<MAX_MONSTER_CLASS; j++ )
	{
		s_iMonsterCurrentAIUnitTable[j] = 0;
	}

	s_iMonsterAIRuleInfoCount = _ZERO;

	return 0;
}

int	TMonsterAIRule::GetCurrentAIUnit( int iMonsterClass )
{

	if( iMonsterClass < 0 || iMonsterClass >= MAX_MONSTER_CLASS )
	{
		LogAddTD("[Monster AI Rule] GetCurrentAIUnit() Error - (MonsterClass=%d) ", iMonsterClass);
		return _ZERO; // AI ����
	}

	return s_iMonsterCurrentAIUnitTable[iMonsterClass];
}

void TMonsterAIRule::MonsterAIRuleProc()
{
	int	iChangedMonsterClassList[MAX_MONSTER_AI_RULEINFO]	= { _INVALID,	};
	int	iChangedAIUnitList[MAX_MONSTER_AI_RULEINFO]			= { _ZERO,		};
	int iChangedListCount = 0;

	// 1. �� RuleInfo�� Valid �� ��� AI Unit Table���� ���� ����Ʈ�� �̾� ����.
	for( int i=0 ; i<s_iMonsterAIRuleInfoCount; i++ )
	{
		TMonsterAIRuleInfo &RuleInfo = s_MonsterAIRuleInfoArray[i];

		if( RuleInfo.m_iMonsterClass < 0 || RuleInfo.m_iMonsterClass >= MAX_MONSTER_CLASS ) 
			return;
		
		// 2. Rule �� Valid �� ��� ���� ����Ʈ�� ����Ѵ�
		if( RuleInfo.IsValid() )
		{
			iChangedMonsterClassList[iChangedListCount] = RuleInfo.m_iMonsterClass;
			iChangedAIUnitList[iChangedListCount]		= RuleInfo.m_iMonsterAIUnit;
			iChangedListCount++;
		}
		// 3. Rule �� Invalid �� ���
		else
		{
			s_iMonsterCurrentAIUnitTable[ RuleInfo.m_iMonsterClass ] = _ZERO;
		}
	}


	// 4. �ߺ� ������ �ϱ� ���� ���� List�� �̿��ؼ� AI Unit Table�� �����Ѵ�.
	for( int j=0 ; j<iChangedListCount; j++ )
	{
		// ����! ���� ����Ŭ������ �ߺ� ���� �� ��� ������ ������ ��ȿ�ϴ�.!
		s_iMonsterCurrentAIUnitTable[ iChangedMonsterClassList[j] ] = iChangedAIUnitList[j];
	}

}

#endif