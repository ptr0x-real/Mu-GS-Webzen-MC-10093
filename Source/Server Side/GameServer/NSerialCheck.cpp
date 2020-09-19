// NSerialCheck.cpp
// Ŭ���̾�Ʈ�� �ø����� �����.
// ���� Ŭ���̾�Ʈ���� �������� �ø����� ������ Ʋ���ٸ�.. 
// ���и� �����Ѵ�.
// LastUpdate : 2002/11/21 
// Code : SaemSong
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NSerialCheck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NSerialCheck::NSerialCheck()
{
	init();
}

NSerialCheck::~NSerialCheck()
{

}

void NSerialCheck::init()
{
	m_serial = 255;
	m_SendSeiral = 0;
}

// Ŭ���̾�Ʈ���� ��ȣȭ ��Ŷ�� �ø����� ����ؼ�(+1�Ȱ�) �����´�.
// �ѷ� InCHeck �Լ��� üŷ�� �ϰԵȴ�.
BOOL NSerialCheck::InCheck(BYTE serial)
{
	BYTE m_s=m_serial;

	m_s++;

	// ������ �ø����� ���� �ø��󺸴� 1 ũ�� ��¥ ������, �ƴϸ� ��¥ ������ �̴�.
	if( m_s == serial )
	{
		m_serial = serial;
		return TRUE;
	}
	return FALSE;
}

int NSerialCheck::GetSerial()
{
	return m_serial;
}

BYTE NSerialCheck::GetSendSerial()
{
	return m_SendSeiral++;
}