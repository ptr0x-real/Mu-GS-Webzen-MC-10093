#pragma once

#include "CriticalSection.h"
#include "File.h"

class CLogSystem// : public Singleton<CLogToFile>
{
public:
	CLogSystem();
	~CLogSystem();

	void Init(const char * szName);
	void Output(char * fmt, ...);
private:
	CCriticalSection	m_Crit;
	CHAR				m_szLogFileName[MAX_PATH];
	CFileSys			m_LogFile;
};
