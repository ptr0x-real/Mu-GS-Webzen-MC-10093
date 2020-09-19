// SendHackLog.cpp: implementation of the CSendHackLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gamemain.h"
#include "user.h"

#include "SendHackLog.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSendHackLog	gSendHackLog;

CSendHackLog::CSendHackLog()
{

}

CSendHackLog::~CSendHackLog()
{

}


void CSendHackLog::SendSet(char* ip, int port)
{
	m_UdpSocCHackLog.CreateSocket();
	m_UdpSocCHackLog.SendSet(ip, port);
}

void CSendHackLog::Send(int aIndex, BYTE type, char* Context)
{
	PMSG_HACK_LOG	Msg;

	PHeadSetB((LPBYTE)&Msg, 0x01, sizeof( Msg ));	

	Msg.wServer = gGameServerCode;

	memcpy(Msg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy(Msg.Name, gObj[aIndex].Name, MAX_IDSTRING);

	Msg.AccountID[MAX_IDSTRING] = NULL;
	Msg.Name[MAX_IDSTRING] = NULL;
	Msg.btType = type;
	memcpy(Msg.szPacketContext, Context, 50);

	m_UdpSocCHackLog.SendData((LPBYTE)&Msg, sizeof( Msg ));
}

BOOL CSendHackLog::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
	return m_UdpSocCHackLog.SendData(SendData, nSendDataLen);
}