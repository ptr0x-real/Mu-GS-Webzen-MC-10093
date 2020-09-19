#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIMovePath.h"

#include "..\Include\ReadScript.h"
#include "..\Common\WinUtil.h"

TMonsterAIMovePath::TMonsterAIMovePath()
{
	DelAllAIMonsterMovePath();
}

TMonsterAIMovePath::~TMonsterAIMovePath()
{

}

int TMonsterAIMovePath::LoadData( LPSTR lpszFileName )
{
	m_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI MovePath] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI MovePath] - Can't Open %s ", lpszFileName);
			return false;
		}
		
		DelAllAIMonsterMovePath();

		SMDToken Token;
		
		int iType = -1;
		
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				if( iType == 2 )
				{	
					// 1. >	데이터 초기화
					int	iSpotType	= _INVALID;
					int iMapNumber	= _INVALID;
					int iX			= _INVALID;
					int iY			= _INVALID;
				
					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iSpotType	= (int)TokenNumber;
					
					Token = GetToken(); iMapNumber	= (int)TokenNumber;
					Token = GetToken(); iX			= (int)TokenNumber;
					Token = GetToken();	iY			= (int)TokenNumber;
				
					// 4. > 데이터 세팅
					m_MovePathInfo[m_iMovePathSpotCount].m_iType		= iSpotType;
					m_MovePathInfo[m_iMovePathSpotCount].m_iMapNumber	= iMapNumber;
					m_MovePathInfo[m_iMovePathSpotCount].m_iPathX		= iX;
					m_MovePathInfo[m_iMovePathSpotCount].m_iPathY		= iY;

					m_iMovePathSpotCount++;

					// 3. >	데이터 유효성 검사
					if( m_iMovePathSpotCount > MAX_MONSTER_MOVE_PATH_SPOT )
					{
						MsgBox("[Monster AI MovePath] Exceed Max Move Path-Spot ");
						DelAllAIMonsterMovePath();
						return 0;
					}
					
					
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI MovePath ] - %s file is Loaded", lpszFileName);

		m_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI MovePath] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

int	TMonsterAIMovePath::DelAllAIMonsterMovePath()
{
	for( int i=0; i<MAX_MONSTER_MOVE_PATH_SPOT; i++ )
	{
		m_MovePathInfo[i].Reset();
	}

	m_iMovePathSpotCount = _ZERO;

	m_bDataLoad			 = FALSE;

	return 0;
}

#endif