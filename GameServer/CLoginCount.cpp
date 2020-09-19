
#include "stdafx.h"
#include "CLoginCount.h"

CLoginCount::CLoginCount()
{
	Init();
}

CLoginCount::~CLoginCount()
{

}

void CLoginCount::Init()
{
	m_count = 0;
}


void CLoginCount::Add()
{
	m_count++;
}


void CLoginCount::Delete()
{
	m_count--;
	if( m_count < 0 )
	{
		m_count = 0;
	}
}

int CLoginCount::Get()
{
	return m_count;
}
