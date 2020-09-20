// NSerialCheck.cpp
// 클라이언트와 시리얼을 맞춘다.
// 만약 클라이언트에서 보내오는 시리얼의 순차가 틀리다면.. 
// 실패를 리턴한다.
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

// 클라이언트에선 암호화 패킷에 시리얼을 등록해서(+1된값) 보내온다.
// ㅡ럼 InCHeck 함수로 체킹을 하게된다.
BOOL NSerialCheck::InCheck(BYTE serial)
{
	BYTE m_s=m_serial;

	m_s++;

	// 보내온 시리얼이 이전 시리얼보다 1 크면 진짜 데이터, 아니면 가짜 데이터 이다.
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