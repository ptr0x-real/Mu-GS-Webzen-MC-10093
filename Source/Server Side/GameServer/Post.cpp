#include "stdafx.h"
#include "Post.h"
#include "user.h"
#include "TNotice.h"
#include "protocol.h"
#include "LogProc.h"
#include "..\\common\\winutil.h"
#include "..\\include\\define.h"
#include "GameMain.h"

CPostSystem g_PostSystem;

CPostSystem::CPostSystem() : m_bPostEnabled(false),m_iPostMoney(-1),m_iPostLevel(-1)
{
}

void CPostSystem::Load(const char *filename)
{
	this->m_bPostEnabled = GetPrivateProfileInt("GameServerInfo","PostSystemEnabled", TRUE, filename);
	this->m_iPostLevel = GetPrivateProfileInt("GameServerInfo", "PostLevel", 6, filename);
	this->m_iPostMoney = GetPrivateProfileInt("GameServerInfo", "PostMoney", 50000, filename);

	if(this->CheckSetting() == false)
		this->m_bPostEnabled = false;
}

void CPostSystem::SendPostMessage(int aIndex,char* message)
{
	int PostState = GetState();

	if(PostState != POST_ENABLED)
	{
		SendErrorMsg(aIndex);
		return;
	}

	if(!CheckUser(aIndex))
		return;

	PMSG_CHATDATA_WHISPER pNotice;

	memcpy(pNotice.id,gObj[aIndex].Name,10);
	// ----
	sprintf(pNotice.chatmsg,"[POST]:%s",message);
	// ----
	PHeadSetB((LPBYTE)&pNotice, 0x02, sizeof(pNotice));

	DataSendAll((LPBYTE)&pNotice,pNotice.h.size);

	gObj[aIndex].Money -= m_iPostMoney;

	GCMoneySend(aIndex,gObj[aIndex].Money);
}

bool CPostSystem::CheckSetting()
{
	bool bRet = SETTINGOK;

	if(!this->m_bPostEnabled)
	{
		LogAddTD("Post System Disabled");
		return SETTINGERROR;
	}

	LogAddTD("Post System Checking Initializing");

	LogAddTD("Post Money Checking");

	if(this->m_iPostMoney < 0 || this->m_iPostMoney > MAX_ZEN)
	{
		bRet = SETTINGERROR;
		LogAddTD(CHECKERROR);
	}
	else
		LogAddTD(CHECKOK);

	LogAddTD("Post Level Checking");

	if(this->m_iPostLevel < 0 || this->m_iPostLevel > g_GlobalConfig.m_iMaxLevel) //MAX_CHAR_LEVEL
	{
		bRet = SETTINGERROR;
		LogAddTD(CHECKERROR);
	}
	else
		LogAddTD(CHECKOK);

	return bRet;
}

int CPostSystem::GetState()
{
	if(!m_bPostEnabled)
	{
		return POST_DISABLED;
	}

	if(m_iPostMoney < 0 || m_iPostMoney > MAX_ZEN)
	{
		return POST_MONEYERROR;
	}

	if(m_iPostLevel < 0 || m_iPostLevel > g_GlobalConfig.m_iMaxLevel) //MAX_CHAR_LEVEL
	{
		return POST_LEVELERROR;
	}

	return POST_ENABLED;
}

void CPostSystem::SendErrorMsg(int aIndex)
{
	MsgOutput(aIndex,"Post is disabled...");
}

bool CPostSystem::CheckUser(int aIndex)
{
	if(!gObjIsConnectedGP(aIndex))
		return false;

	if(gObj[aIndex].Level < m_iPostLevel)
	{
		MsgOutput(aIndex,"You need level %d or major for use post command.",m_iPostLevel);
		return false;
	}
	else if(gObj[aIndex].Money < m_iPostMoney)
	{
		MsgOutput(aIndex,"You need %d of zen for use post command.",m_iPostMoney);
		return false;
	}

	return true;
}