// TRandomPoolMgr.cpp: implementation of the TRandomPoolMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TRandomPoolMgr.h"

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117//grooving
#include "LargeRand.h"
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

//#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TRandomPoolMgr::TRandomPoolMgr()
{
	InitPool();
}

TRandomPoolMgr::~TRandomPoolMgr()
{
	
}

void TRandomPoolMgr::InitPool()
{
	m_listPool.clear();
	m_iSumWeight = 0;
}

bool TRandomPoolMgr::AddValue(int index, int weight)
{
	bool bRet = false;

	if ( index < 0 )
		return bRet;

	map<int, int>::iterator iter = m_listPool.find(index);
	if ( iter == m_listPool.end() )
	{
		m_listPool[index] = weight;
		bRet = true;
		m_iSumWeight += weight;
	}

	return bRet;
}
	
int  TRandomPoolMgr::GetRandomValue( eRandomPoolSelectType eSelectType )
{
	int iReturnValue = 0;

	int iPoolSize = m_listPool.size();
	if ( iPoolSize == 0 )
		return iReturnValue;

	// ����ġ ���� 0 �� ���� ������ �������ù������ ����
	if ( m_iSumWeight <= 0 )
	{
		eSelectType = eRANDOMPOOL_BY_RANDOM;
	}

	if ( eSelectType == eRANDOMPOOL_BY_RANDOM )
	{
		// ��ü ��Ͽ��� ����.
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117//grooving
		int iPos = GetLargeRand() % iPoolSize;
#else
		int iPos= rand()%iPoolSize;
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
		int iCount = 0;
		for ( map<int,int>::iterator iter = m_listPool.begin(); iter != m_listPool.end(); ++iter )
		{
			if ( iPos == iCount )
			{
				iReturnValue = iter->first;
				break;
			}
		}
	}
	else if ( eSelectType == eRANDOMPOOL_BY_WEIGHT)
	{
		// ��ü ����ġ�� �ۼ�Ʈ�� Ȯ�� ����
#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117//grooving
		int iRandom = GetLargeRand() % m_iSumWeight;
#else
		int iRandom= rand()%m_iSumWeight;			// ���� Ȯ�� ��
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117
		int iCurRate = 0;					// ���� �ۼ�Ʈ ��

#if TESTSERVER == 1
		char buf[128];
		sprintf (buf, "����ġ�� ���� Ȯ�� ��\t%d \n", iRandom);
		OutputDebugString( buf );
		//LogAddTD(buf);
#endif
		
		for ( map<int,int>::iterator iter = m_listPool.begin(); iter != m_listPool.end(); ++iter )
		{
			int iRate = iter->second;	// ��ü����ġ �տ� ���� ����ġ ����
			iCurRate += iRate;			// ���� Ȯ�� ��

#if TESTSERVER == 1
			sprintf (buf, "%d\tweight\t%d\tȮ����\t%d\t%d\t%d\n", iter->first, iter->second, iRate, iCurRate, m_iSumWeight);
			OutputDebugString( buf );
			//LogAddTD(buf);
#endif

			if ( iCurRate >= iRandom )						// ���� Ȯ�� ���� ���� Ȯ�� ������ ū ��� ����
			{
				iReturnValue = iter->first;
				break;
			}
		}

#if TESTSERVER == 1
		sprintf (buf, "����\t%d\n", iReturnValue);
		OutputDebugString( buf );
		//LogAddTD(buf);
#endif

	}

	return iReturnValue;
}

//#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530