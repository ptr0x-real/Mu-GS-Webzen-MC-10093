#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714

#include "TMonsterSkillUnit.h"

#include "Include\ReadScript.h"

BOOL				TMonsterSkillUnit::s_bDataLoad = FALSE;
TMonsterSkillUnit	TMonsterSkillUnit::s_MonsterSkillUnitArray[MAX_MONSTER_SKILL_UNIT];

TMonsterSkillUnit::TMonsterSkillUnit()
{
	Reset();
}

TMonsterSkillUnit::~TMonsterSkillUnit()
{

}

void TMonsterSkillUnit::Reset()
{
	m_iUnitTargetType	= _INVALID;
	m_iUnitScopeType	= _INVALID;
	m_iUnitScopeValue	= _INVALID;
	m_iDelay			= _INVALID;
	
	m_iUnitNumber		= _INVALID;
	ZeroMemory( m_szUnitName, MAX_SKILLUNITNAME );

	m_iElementsCount	= _INVALID;

	for( int i=0; i<MAX_MONSTER_SKILLELEMENT_SLOT; i++ )
	{
		m_lpElementsSlot[i] = NULL;
	}

}

int TMonsterSkillUnit::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster Skill Unit] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster Skill Unit] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllSkillUnit();
		
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
					char	szUnitName[MAX_SKILLELEMENTNAME] = {0,};
					
					int		iUnitNumber		= _INVALID;
					int		iUnitTargetType	= _INVALID;
					int		iUnitScopeType	= _INVALID;
					int		iUnitScopeValue	= _INVALID;
					int		iDelay			= _INVALID;

					int		iElementCount	= 0;
					int		iElementsSlot[MAX_MONSTER_SKILLELEMENT_SLOT] = {_INVALID,};
				

					// 2. >	데이터 읽기
 					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iUnitNumber = (int)TokenNumber;
					Token = GetToken(); CopyMemory(szUnitName, TokenString, MAX_SKILLUNITNAME);
					Token = GetToken(); iUnitTargetType	= (int)TokenNumber;
					Token = GetToken(); iUnitScopeType	= (int)TokenNumber;
					Token = GetToken(); iUnitScopeValue	= (int)TokenNumber;
					Token = GetToken(); iDelay			= (int)TokenNumber;

					for( int i=0 ; i<MAX_MONSTER_SKILLELEMENT_SLOT; i++ )
					{
						Token = GetToken(); iElementsSlot[i] = (int)TokenNumber;
					}

					// 3. >	데이터 유효성 검사
					if( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_SKILL_UNIT )
					{
						MsgBox("[Monster SkillUnit] - Unit(%d) Error (%s) File. ", iUnitNumber, lpszFileName);
						continue;
					}

					char szMsg[200] = {0,};
					wsprintf( szMsg, "★Num:%d,Name:%s,TType:%d,SType:%d,SValue:%d,Delay:%d, E0:%d,E1:%d,E2:%d,E3:%d,E4:%d\n", 
						iUnitNumber, szUnitName, iUnitTargetType, iUnitScopeType, iUnitScopeValue, iDelay,
						iElementsSlot[0],iElementsSlot[1],iElementsSlot[2],iElementsSlot[3],iElementsSlot[4]);
					OutputDebugString(szMsg);

					// 4. > 데이터 세팅
					s_MonsterSkillUnitArray[iUnitNumber].m_iUnitNumber		= iUnitNumber;
					s_MonsterSkillUnitArray[iUnitNumber].m_iUnitTargetType	= iUnitTargetType;
					s_MonsterSkillUnitArray[iUnitNumber].m_iUnitScopeType	= iUnitScopeType;
					s_MonsterSkillUnitArray[iUnitNumber].m_iUnitScopeValue	= iUnitScopeValue;
					s_MonsterSkillUnitArray[iUnitNumber].m_iDelay			= iDelay;
		
					CopyMemory( s_MonsterSkillUnitArray[iUnitNumber].m_szUnitName, szUnitName, MAX_SKILLUNITNAME );
					
					
					for( int i=0; i<MAX_MONSTER_SKILLELEMENT_SLOT; i++ )
					{
						int iElement = iElementsSlot[i];
						if( iElement != _INVALID )
						{
							TMonsterSkillElement* pMonsterSkillElement =  TMonsterSkillElement::FindSkillElement(iElement);

							if( pMonsterSkillElement != NULL )
							{
								// 유효 엘리먼트만 Count 한다.
								iElementCount++;
								s_MonsterSkillUnitArray[iUnitNumber].m_lpElementsSlot[i] = pMonsterSkillElement;
								LogAddC( 4, "[Monster Skill Unit] - Found SkillElement (%d)(%s)", iElement, pMonsterSkillElement->GetElementName() );
							}
							else
							{
								LogAddC( 2, "[Monster Skill Unit] - Not Found SkillElement (%d)", iElement );
							}
						}
					}

					s_MonsterSkillUnitArray[iUnitNumber].m_iElementsCount = iElementCount;
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster Skill Unit] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster Skill Unit] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	 TMonsterSkillUnit::DelAllSkillUnit()
{
	for( int i=0; i<MAX_MONSTER_SKILL_UNIT; i++ )
	{
		s_MonsterSkillUnitArray[i].Reset();
	}

	return 0;
}

TMonsterSkillUnit* TMonsterSkillUnit::FindSkillUnit( int iUnitNumber )
{
	if( iUnitNumber < 0 || iUnitNumber >= MAX_MONSTER_SKILL_UNIT )
	{
		LogAddTD("[Monster SkillUnit] FindSkillUnit() Error - (UnitNumber=%d) ", iUnitNumber);
		return NULL;
	}

	if( s_MonsterSkillUnitArray[iUnitNumber].m_iUnitNumber == iUnitNumber )
	{
		return &s_MonsterSkillUnitArray[iUnitNumber];
	}
	else
		LogAddTD("[Monster SkillUnit] FindSkillUnit() Error - (UnitNumber=%d) ", iUnitNumber);
	
	return NULL;
}

// 스킬이 작동 된다.
void TMonsterSkillUnit::RunSkill(  int iIndex, int iTargetIndex  )
{
 	char szElementsName[MAX_MONSTER_SKILLELEMENT_SLOT][MAX_SKILLUNITNAME] = {0,};

	for( int i=0; i<m_iElementsCount; i++ )
	{
		if( m_lpElementsSlot[i] == NULL )
		{
			LogAddC(LOGC_RED, "[Monster SkillUnit] SkillElement is NULL" );
			continue;
		}

		m_lpElementsSlot[i]->ForceSkillElement( iIndex, iTargetIndex );

#if TESTSERVER == 1
 		CopyMemory( szElementsName[i], m_lpElementsSlot[i]->GetElementName(), MAX_SKILLELEMENTNAME);
#endif
		
	}
	
#if TESTSERVER == 1
//	MsgOutput( iTargetIndex, "%s 몬스터스킬에 맞음", m_szUnitName );
//	LogAddC(LOGC_GREEN, "[Monster SkillUnit] Running (%s) Skill - %s,%s,%s,%s,%s ", 
//		m_szUnitName, &szElementsName[0], &szElementsName[1], szElementsName[2], szElementsName[3], szElementsName[4]);
#endif
}

#endif