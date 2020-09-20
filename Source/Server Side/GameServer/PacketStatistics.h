#ifndef PACKETSTATISTICS_H
#define PACKETSTATISTICS_H

class Timer {
private:
	DWORD	m_iWakeTick;
	bool	m_bOnceWake;
	bool	m_bActive;

public:
	Timer(bool bOnceWake = true);	// SetTimer�� ���� ���� �Ŀ� IsWake�� �ѹ��� �۵��� ���ΰ� �ƴϸ� ���������� �۵��� ���ΰ�.
	~Timer();

	void	Init(void);

	bool	IsWake(void);
	void	SetTimer(DWORD dwMilliSecond);
};

class PacketStatistics {
public:
	enum PROCESS_RETURN {
		PS_NORMAL	= 1,
		PS_RESET	= 2,
	};
private:
	Timer			m_kTimer;
	DWORD			m_iCheckDuration;

	int				m_iPacketCount;
	int				m_iEncryptPacketCount;
	unsigned int	m_iPacketTotalSize;

private:

public:
	PacketStatistics(void);		// 1000�� 1��
	~PacketStatistics();

	void			Init(DWORD iCheckDuration = 1000);
	PROCESS_RETURN	Process(void);		// 1 : Normal, 2 : Reset 

	void			AddPacketInfo(bool bEncrypt, unsigned int iPackSize);
	void			GetStatistics(int &iOutPacketCount, int &iOutEncryptPacketCount, unsigned int &iOutPacketTotalSize);
};


#endif