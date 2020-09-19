// 
// �̵��� �ߴ��� ���ߴ��� üũ�ϴ� Ŭ���� �̴�. 
// (���� ���콺 ���� ����)
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
		// ������ ���� �ʴٸ�..
		m_PosCount = 0;
		return TRUE;
	}
	
	m_TmpPosX = x;
	m_TmpPosX = y;
	//LogAddL("���� ��ġ %d %d", x, y);

	if( GetTickCount()-m_Time < 1000 )
	{
		m_Time = GetTickCount();
		// �ð��� �ȵǾ��� ���� ��ġ�� �ٸ��� 
		
	}
	m_PosX[m_PosCount] = x;
	m_PosY[m_PosCount] = y;
	m_PosCount++;
	if( m_PosCount > MAX_MOVECHECK-1 )
		m_PosCount = 0;



	//LogAddL("���� ��ġ ���(%d) %d %d", m_PosCount, x, y);
	
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

