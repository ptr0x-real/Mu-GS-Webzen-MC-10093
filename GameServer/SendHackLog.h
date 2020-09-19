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

// 0x01 : Game Server�� ���� UDP Data�� Receive �޴´�. 
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wServer;							// ��������
	char		AccountID[MAX_IDSTRING+1];			// ���� 
	char		Name[MAX_IDSTRING+1];				// ĳ���͸�  
	BYTE		btType;								// ��ŷ���� 
	char		szPacketContext[50];				// ��Ŷ ���� 
} PMSG_HACK_LOG, * LPPMSG_HACK_LOG;



// 0x02 : Game Server�� ���� ä�� �޽����� �޴´�
typedef struct
{
	PBMSG_HEAD	h;
	WORD		wServer;						// ��������
	char		AccountID[MAX_IDSTRING+1];		// ���� 
	char		Name[MAX_IDSTRING+1];			// ĳ���͸�  

	BYTE		btType;							// ����( 4bit:�̸�ī��Ʈ, 4bit:���� )
	char		szChatMsg[MAX_CHAT+1];			// ��Ŷ ���� 

	char		szTargetName[5][(MAX_IDSTRING+1)];	// ���
} CHAT_LOG_DATA, * LPCHAT_LOG_DATA;
//	char		szTargetName[(MAX_IDSTRING+1)*5];	// ���

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
