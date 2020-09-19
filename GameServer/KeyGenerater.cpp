// KeyGenerater.cpp: implementation of the CKeyGenerater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyGenerater.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef ADD_MAPMOVE_PROTOCOL_20090327

CKeyGenerater g_KeyGenerater;

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
#include "LargeRand.h"
#endif // USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE

static int KEY_GENERATE_FILTER[MAX_KEY_GENERATER_FILTER][4] = 
{
	{ 321,	37531879,	8734,	32 },		// 0
	{ 873,	64374332,	3546,	87 },
	{ 537,	24798765,	5798,	32 },
	{ 654,	32498765,	3573,	73 },
	{ 546,	98465432,	6459,	12 },		// 4
	{ 987,	24654876,	5616,	54 },
	{ 357,	34599876,	8764,	98 },
	{ 665,	78641332,	6547,	54 },
	{ 813,	85132165,	8421,	98 },
	{ 454,	57684216,	6875,	45 }
};

CKeyGenerater::CKeyGenerater()
{

}

CKeyGenerater::~CKeyGenerater()
{

}

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
// 1. Ű �� ���� ����
// 1-1. �ʱ��� �õ尪�� �����Ѵ�.
DWORD CKeyGenerater::GenerateSeedValue()
{
	return GetLargeRand();
}
#endif // USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE

// 1-2. Ű������ ���� Ű ���� �����Ѵ�.
DWORD CKeyGenerater::GenerateKeyValue(DWORD dwKeyValue)
{
	DWORD dwRegenerateKeyValue = 0;
	BYTE btNumericValue = 0;

	// ���� ���� ������ ���� ���� Ű ���� �����س���.
	// *, /, +, -�� %���� ��θ� ����Ѵ�.
	// ���⵵�� ������Ű�� ���� Ű���� mod ������ ���� 10���� ���ͷ� ���� �й��Ѵ�.
	btNumericValue = dwKeyValue % MAX_KEY_GENERATER_FILTER;

	dwRegenerateKeyValue = dwKeyValue * KEY_GENERATE_FILTER[btNumericValue][0] + KEY_GENERATE_FILTER[btNumericValue][1] - KEY_GENERATE_FILTER[btNumericValue][2] / KEY_GENERATE_FILTER[btNumericValue][3];

	return dwRegenerateKeyValue;
}

// 1-3. �Էµ� Ű ���� ���� Ű ���� ���Ѵ�.
bool CKeyGenerater::CheckKeyValue(DWORD* dwOldKeyValue, DWORD dwReceiveKeyValue)
{
	// Ŭ���̾�Ʈ�κ��� ���� Ű ���� ������ �����ϰ� �ִ� Ű ���� ���Ѵ�.
	DWORD dwGeneratedKeyValue = 0;

	dwGeneratedKeyValue = GenerateKeyValue( *dwOldKeyValue );
	if( dwReceiveKeyValue == dwGeneratedKeyValue )
	{
		*dwOldKeyValue = dwGeneratedKeyValue;
		return true;
	}

	return false;
}

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
void CKeyGenerater::_SimulationKeyGenerate()
{
	DWORD dwKeyValue = 0;

	dwKeyValue = GenerateSeedValue();

	for( int i = 0 ; i < 10000 ; i++ )
	{
		dwKeyValue = GenerateKeyValue( dwKeyValue );
	}
}
#endif // USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE

#endif // ADD_MAPMOVE_PROTOCOL_20090327