#ifndef _VIPSYSTEM_H_
#define _VIPSYSTEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include "Log.h"
#include "Include\Sprodef.h"

#include <vector>

typedef struct _VIP_CONFIG
{
	_VIP_CONFIG()
	{
		iVipLevel = 0;
		iPlusExp = 100;
		iPlusDrop = 100;
		iPlusExcellent = 2000;
		iPlusAncient = 100;
	}

	int iVipLevel;
	int iPlusExp;
	int iPlusDrop;
	int iPlusExcellent;
	int iPlusAncient;
} VIP_CONFIG, *LPVIP_CONFIG;

typedef std::vector<VIP_CONFIG> VECTOR_VIP;
typedef VECTOR_VIP::iterator VECTOR_VIP_IT;

class CVIPSystem : private CLogSystem
{
public:
	CVIPSystem();
	~CVIPSystem();
	void ProcessVIP();
	void SetVIP(LPOBJECTSTRUCT lpObj, bool bForce);
	void CheckVIP(LPOBJECTSTRUCT lpObj);
	void RequestVip(LPOBJECTSTRUCT lpObj);
	void UpdateVip(LPPMSG_ANS_VIP pMsg);

	void LoadVIP();

private:
	LPVIP_CONFIG GetConfig(int VipLevel);

public:
	bool m_bIsVipEnabled, m_bShowInfo, m_bShowMessage;
	char m_Message[256];

private:
	VECTOR_VIP m_VIPConfig;
};

extern CVIPSystem g_VIPSystem;

#endif