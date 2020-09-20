#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

#include "TMonsterAIAgro.h"
#include "MapClass.h"
#include "Gamemain.h"


TMonsterAIAgro::TMonsterAIAgro()
{
	ResetAll();
}

TMonsterAIAgro::~TMonsterAIAgro()
{

}
void TMonsterAIAgro::ResetAll()
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		m_Agro[i].Reset();
	}
}

int	TMonsterAIAgro::SetAgro( int iTargetIndex, int iValue )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() == _INVALID )
		{
			m_Agro[i].SetAgro( iTargetIndex, iValue );
			return _SUCCESS;
		}
	}

	return _FAIL;
}

int	TMonsterAIAgro::DelAgro( int iTargetIndex )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() == iTargetIndex )
		{
			m_Agro[i].Reset();
			return _SUCCESS;
		}
	}
	
	return _FAIL;
}

int	TMonsterAIAgro::GetAgro( int iTargetIndex )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() == iTargetIndex )
		{
			return m_Agro[i].GetAgroValue();
		}
	}
	
	return _INVALID;
}	

int	TMonsterAIAgro::IncAgro( int iTargetIndex, int iIncValue )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() == iTargetIndex )
		{
			return m_Agro[i].IncAgro(iIncValue);
		}
	}
	
	return _INVALID;
}

int	TMonsterAIAgro::DecAgro( int iTargetIndex, int iDecValue )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() == iTargetIndex )
		{
			return m_Agro[i].DecAgro(iDecValue);
		}
	}
	
	return _INVALID;
}

void TMonsterAIAgro::DecAllAgro( int iDecValue )
{
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() != _INVALID )
		{
			m_Agro[i].DecAgro(iDecValue);
		}
		else
		{
			m_Agro[i].Reset();
		}
	}
	
}

int	TMonsterAIAgro::GetMaxAgroUserIndex( int iMonsterIndex )
{
	int iMaxAgroUserIndex = _INVALID;
	int iMaxAgro		  = _INVALID;
	
	for( int i=0; i<MAX_MONSTER_AI_AGRO; i++ )
	{
		if( m_Agro[i].GetUserIndex() != _INVALID )
		{
			LPOBJECTSTRUCT lpObj = &gObj[m_Agro[i].GetUserIndex()];
			
			// 0. > GM 은 타겟이 될 수 없다.
			if( (lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN )
			{
				continue;
			}

			// 1. > 인덱스가 맞고, 게임 중인 유저인가?
			if( !gObjIsConnected(lpObj) )
			{
				m_Agro[i].Reset();
				continue;
			}

			// 2. > 타겟이 죽음
			if( !lpObj->Live )
			{
				m_Agro[i].Reset();
				continue;
			}
		
			// 3. > 거리 검사
			if( gObjCalDistance(&gObj[iMonsterIndex], lpObj) > 7 )
			{
				// 임계 거리 밖으로 나갔다가 다시 임계 거리 안으로 들어 올 수 있다.
				// 이 문제의 해결을 위해 일정 시간마다 Agro를 감소 시켜준다.(MonsteAI에서)
				continue;
			}

			// 4. > 안전지대에 있는지 검사.

			BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
			if( btAttr&MAP_ATTR_SAFTYZONE )
			{	
				continue;
			}

			if( m_Agro[i].GetAgroValue() > iMaxAgro )
			{
				iMaxAgroUserIndex = m_Agro[i].GetUserIndex();
				iMaxAgro = m_Agro[i].GetAgroValue();
			}
		}

	}

	return iMaxAgroUserIndex;
}

#endif