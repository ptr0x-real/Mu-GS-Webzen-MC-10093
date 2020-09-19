// KeyGenerater.h: interface for the CKeyGenerater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYGENERATER_H__EC134DC8_2BD1_4EC9_8F3B_09C85AD67F00__INCLUDED_)
#define AFX_KEYGENERATER_H__EC134DC8_2BD1_4EC9_8F3B_09C85AD67F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_MAPMOVE_PROTOCOL_20090327

CONST int MAX_KEY_GENERATER_FILTER	= 10;

class CKeyGenerater  
{
public:
	CKeyGenerater();
	virtual ~CKeyGenerater();

	// 1. Ű �� ���� ����
#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
	// 1-1. �ʱ��� �õ尪�� �����Ѵ�.
	DWORD		GenerateSeedValue();
#endif // USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
	// 1-2. Ű������ ���� Ű ���� �����Ѵ�.
	DWORD		GenerateKeyValue(DWORD dwKeyValue);
	// 1-3. �Էµ� Ű ���� ���� Ű ���� ���Ѵ�. (���� ����)
	bool		CheckKeyValue(DWORD* dwOldKeyValue, DWORD dwReceiveKeyValue);

#ifdef USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
	void		_SimulationKeyGenerate();
#endif // USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
};

extern CKeyGenerater g_KeyGenerater;

#endif // ADD_MAPMOVE_PROTOCOL_20090327

#endif // !defined(AFX_KEYGENERATER_H__EC134DC8_2BD1_4EC9_8F3B_09C85AD67F00__INCLUDED_)