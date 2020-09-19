// 
// 이동을 했는지 안했는지 체크하는 클래스 이다. 
// (오토 마우스 막기 위해)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveCheck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveCheck::CMoveCheck()
{

}

CMoveCheck::~CMoveCheck()
{

}

void CMoveCheck::Init()
{
	m_Time = GetTickCount();
	
	for( int n=0; n<MAX_MOVECHECK; n++)
	{
		m_PosX[n] = n;
		m_PosY[n] = n;
	}
	m_PosCount = 0;
	m_TmpPosX  = 0;
	m_TmpPosY  = 0;
}

int CMoveCheck::Insert(int x, int y)
{
	if( m_TmpPosX != x || m_TmpPosY != y )
	{
		// 이전과 같지 않다면..
		m_PosCount = 0;
		return TRUE;
	}
	
	m_TmpPosX = x;
	m_TmpPosX = y;
	//LogAddL("고정 위치 %d %d", x, y);

	if( GetTickCount()-m_Time < 1000 )
	{
		m_Time = GetTickCount();
		// 시간이 안되었고 이전 위치랑 다르면 
		
	}
	m_PosX[m_PosCount] = x;
	m_PosY[m_PosCount] = y;
	m_PosCount++;
	if( m_PosCount > MAX_MOVECHECK-1 )
		m_PosCount = 0;



	//LogAddL("고정 위치 등록(%d) %d %d", m_PosCount, x, y);
	
	return TRUE;
}


int CMoveCheck::Check()
{
	int x, y;
	for( y=0; y<MAX_MOVECHECK; y++)
	{
		for( x=0; x<MAX_MOVECHECK; x++)
		{
			if( m_PosX[y] != m_PosX[x] )
			{
				return FALSE;
			}
		}
	}
	for(  y=0; y<MAX_MOVECHECK; y++)
	{
		for( x=0; x<MAX_MOVECHECK; x++)
		{
			if( m_PosY[y] != m_PosY[x] )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

