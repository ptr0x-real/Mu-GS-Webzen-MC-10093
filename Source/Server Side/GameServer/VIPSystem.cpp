#include "stdafx.h"
#include "VIPSystem.h"
#include "Common\LogProc.h"
#include "GameMain.h"
#include "DBSockMng.h"

#include "..\include\Readscript.h"

CVIPSystem g_VIPSystem;

CVIPSystem::CVIPSystem() : m_bShowInfo(false), m_bShowMessage(false)
{
	this->Init("VIP");
}

CVIPSystem::~CVIPSystem()
{
}

void CVIPSystem::ProcessVIP()
{
	for(int i = ALLOC_USEROBJECTSTART; i < MAX_OBJECT;i++)
	{
		LPOBJECTSTRUCT lpObj = &gObj[i];
		CheckVIP(lpObj);
	}
}

void CVIPSystem::SetVIP(LPOBJECTSTRUCT lpObj, bool bForce)
{
	LPVIP_CONFIG it = GetConfig((bForce ? 0 : lpObj->m_AccountType));

	if(it)
	{
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		lpObj->m_wExprienceRate = it->iPlusExp;
		lpObj->m_wItemDropRate = it->iPlusDrop;
#endif
		lpObj->m_wExcellentDropRate = it->iPlusExcellent;
#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
		lpObj->m_wAncientDropRate = it->iPlusAncient;
#endif
	}

	lpObj->m_bVIPReload = false;
}

void CVIPSystem::CheckVIP(LPOBJECTSTRUCT lpObj)
{
	if(lpObj->Type != OBJTYPE_CHARACTER || lpObj->Connected <= CSS_CHARACTERVIEW)
		return;

	if(lpObj->m_AccountType < 1)
	{
		if(lpObj->m_dwRequestVip < GetTickCount())
		{
			this->RequestVip(lpObj);
			lpObj->m_dwRequestVip = GetTickCount()+(5*60*1000);
		}
		return;
	}

	if(m_bIsVipEnabled == false)
	{
		SetVIP(lpObj,true);
		return;
	}

	time_t CurrentTime = time(NULL);

	if(lpObj->m_JoinTime+lpObj->m_EndTime < CurrentTime)
	{
		lpObj->m_AccountType = 0;
		if((lpObj->Authority & AUTHORITY_VIP_USER) != 0)
			lpObj->Authority ^= AUTHORITY_VIP_USER;
		this->RequestVip(lpObj);
		this->Output("VIP Time Ended: Account[%s][%s]",lpObj->AccountID,CTime(lpObj->m_EndTime).Format("%d/%m/%Y %H:%M:%S"));
		MsgOutput(lpObj->m_Index,"Vip Time Ended: Rates Normal");
		SetVIP(lpObj,false);
	}
	else
	{
		lpObj->Authority |= AUTHORITY_VIP_USER;
		SetVIP(lpObj,false);

		if(lpObj->m_dwRequestVip < GetTickCount())
		{
			this->RequestVip(lpObj);
			lpObj->m_dwRequestVip = GetTickCount()+(5*60*1000);
		}
	}
}

void CVIPSystem::RequestVip(LPOBJECTSTRUCT lpObj)
{
	if(lpObj->m_bRequestVip == true)
		return;

	lpObj->m_bRequestVip = true;

	PMSG_REQ_VIP pMsg;

	ZeroMemory(&pMsg,sizeof(PMSG_REQ_VIP));

	strcpy(pMsg.chAccountID, lpObj->AccountID);

	pMsg.AccountType = lpObj->m_AccountType;
	pMsg.EndTime = time(NULL)-lpObj->m_JoinTime;

	if(pMsg.EndTime > lpObj->m_EndTime)
		pMsg.EndTime = lpObj->m_EndTime;

	lpObj->m_EndTime -= pMsg.EndTime;

	pMsg.h.set(0xAA,sizeof(PMSG_REQ_VIP));

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}

void CVIPSystem::UpdateVip(LPPMSG_ANS_VIP pMsg)
{
	LPOBJECTSTRUCT lpObj = &gObj[pMsg->iUserIndex];

	if(lpObj->Type != OBJTYPE_CHARACTER || lpObj->Connected <= CSS_CHARACTERVIEW)
		return;

	lpObj->m_JoinTime = time(NULL);
	lpObj->m_EndTime = pMsg->EndTime;
	lpObj->m_AccountType = pMsg->AccountType;
	lpObj->m_bRequestVip = false;

	this->CheckVIP(lpObj);
}

#define VIP_FILE ".\\Config\\VIP.ini"

void CVIPSystem::LoadVIP()
{
	SMDToken Token;

	m_VIPConfig.clear();

	SMDFile = fopen(VIP_FILE,"r");

	if(SMDFile == NULL) return;

	int iSection = -1;

	while(true)
	{
		Token = GetToken();

		if(Token == END)
			break;
		else if(Token == NUMBER)
		{
			iSection = TokenNumber;

			if(iSection == 0)
			{
				Token = GetToken();

				if(Token == NUMBER)
					m_bIsVipEnabled = (bool)TokenNumber;
			}
			else if(iSection == 1)
			{
				while(true)
				{
					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)
						break;

					VIP_CONFIG Config;

					Config.iVipLevel = TokenNumber;

					Token = GetToken();
					Config.iPlusExp += TokenNumber;

					Token = GetToken();
					Config.iPlusDrop += TokenNumber;

					Token = GetToken();
					Config.iPlusExcellent = TokenNumber;

					if(Config.iPlusExcellent < 2)
						Config.iPlusExcellent = 2;
					else if(Config.iPlusExcellent > g_GlobalConfig.m_iExcellentDrop)
						Config.iPlusExcellent = g_GlobalConfig.m_iExcellentDrop;

					Token = GetToken();
					Config.iPlusAncient += TokenNumber;

					m_VIPConfig.push_back(Config);
				};
			}
			else if(iSection == 2)
			{
				Token = GetToken();
				m_bShowInfo = TokenNumber > 0;

				Token = GetToken();
				m_bShowMessage = TokenNumber > 0;

				Token = GetToken();

				if(Token == NAME)
					strcpy(m_Message,TokenString);
			}
		}
	};

	fclose(SMDFile);


	if(m_VIPConfig.empty() || GetConfig(0) == NULL)
		m_VIPConfig.push_back(VIP_CONFIG());

	for(int i = ALLOC_USEROBJECTSTART; i < MAX_OBJECT;i++)
	{
		LPOBJECTSTRUCT lpObj = &gObj[i];

		if(lpObj->Type == OBJTYPE_CHARACTER && lpObj->Connected > CSS_CHARACTERVIEW && lpObj->m_AccountType != 0)
			lpObj->m_bVIPReload = true;
	}
}

LPVIP_CONFIG CVIPSystem::GetConfig(int VipLevel)
{
	for(VECTOR_VIP_IT it = m_VIPConfig.begin();it != m_VIPConfig.end();it++)
	{
		LPVIP_CONFIG vip = &(*it);

		if(vip->iVipLevel == VipLevel)
			return vip;
	}

	return NULL;
}