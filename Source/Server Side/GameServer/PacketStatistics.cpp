#include "stdafx.h"

#include "PacketStatistics.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------


Timer::Timer(bool bOnceWake)
:m_bActive(false), m_iWakeTick(0), m_bOnceWake(bOnceWake)
{
}

void Timer::Init(void)
{
	m_bActive = false;
	m_iWakeTick = 0;
}

Timer::~Timer()
{
}

bool Timer::IsWake(void)
{
	if(m_iWakeTick <= GetTickCount())
	{
		if(m_bOnceWake == true)
		{
			m_bActive = false;
		}
		
		return(true);
	}

	return(false);
}

void Timer::SetTimer(DWORD dwMilliSecond)
{
	m_bActive = true;
	m_iWakeTick = GetTickCount() + dwMilliSecond;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

PacketStatistics::PacketStatistics(void)
:m_iCheckDuration(0), m_kTimer(true), m_iPacketCount(0), m_iEncryptPacketCount(0), m_iPacketTotalSize(0)
{
}

void PacketStatistics::Init(DWORD iCheckDuration)
{
	m_iCheckDuration		= iCheckDuration;

	m_iPacketCount			= 0;
	m_iEncryptPacketCount	= 0;
	m_iPacketTotalSize		= 0;

	m_kTimer.Init();
	m_kTimer.SetTimer(m_iCheckDuration);
}

PacketStatistics::~PacketStatistics()
{
}

PacketStatistics::PROCESS_RETURN PacketStatistics::Process(void)
{
	if(m_kTimer.IsWake() == true)
	{
		Init(m_iCheckDuration);

		return(PS_RESET);
	}

	return(PS_NORMAL);
}

void PacketStatistics::AddPacketInfo(bool bEncrypt, unsigned int iPackSize)
{
	m_iPacketCount++;
	m_iPacketTotalSize += iPackSize;

	if(bEncrypt == true) m_iEncryptPacketCount++;
}

void PacketStatistics::GetStatistics(int &iOutPacketCount, int &iOutEncryptPacketCount, unsigned int &iOutPacketTotalSize)
{
	iOutPacketCount			= m_iPacketCount;
	iOutEncryptPacketCount	= m_iEncryptPacketCount;
	iOutPacketTotalSize		= m_iPacketTotalSize;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
