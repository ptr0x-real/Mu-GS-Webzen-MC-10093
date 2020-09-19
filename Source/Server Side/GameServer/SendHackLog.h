// SendHackLog.h: interface for the CSendHackLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDHACKLOG_H__F9D65B97_9DDF_4E11_A5FF_C3009BBC2CA2__INCLUDED_)
#define AFX_SENDHACKLOG_H__F9D65B97_9DDF_4E11_A5FF_C3009BBC2CA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Common\\zzzItem.h"
#include "..\\Common\\Classdef.h"

#include "WzUdp.h"

// 0x01 : Game Server로 부터 UDP Data를 Receive 받는다. 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wServer;							// 서버정보
	char		AccountID[MAX_IDSTRING+1];			// 계정 
	char		Name[MAX_IDSTRING+1];				// 캐릭터명  
	BYTE		btType;								// 해킹유형 
	char		szPacketContext[50];				// 패킷 정보 
} PMSG_HACK_LOG, * LPPMSG_HACK_LOG;



// 0x02 : Game Server로 부터 채팅 메시지를 받는다
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wServer;						// 서버정보
	char		AccountID[MAX_IDSTRING+1];		// 계정 
	char		Name[MAX_IDSTRING+1];			// 캐릭터명  

	BYTE		btType;							// 유형( 4bit:이름카운트, 4bit:유형 )
	char		szChatMsg[MAX_CHAT+1];			// 패킷 정보 

	char		szTargetName[5][(MAX_IDSTRING+1)];	// 대상
} CHAT_LOG_DATA, * LPCHAT_LOG_DATA;
//	char		szTargetName[(MAX_IDSTRING+1)*5];	// 대상

class CSendHackLog  
{
	WzUdp			m_UdpSocCHackLog;		// Rank Server
public:
	CSendHackLog();
	virtual ~CSendHackLog();

	void			SendSet(char* ip, int port);
	void			Send(int aIndex, BYTE type, char* Context);

	BOOL			SendData(LPBYTE SendData, DWORD nSendDataLen);
};

extern CSendHackLog	gSendHackLog;

#endif // !defined(AFX_SENDHACKLOG_H__F9D65B97_9DDF_4E11_A5FF_C3009BBC2CA2__INCLUDED_)
