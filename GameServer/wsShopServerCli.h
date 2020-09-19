// wsJoinServerCli.h: interface for the wsJoinServerCli class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSSHOPSERVERCLI_H__10B9CFDA_9936_4932_96B7_97E3492B5164__INCLUDED_)
#define AFX_WSSHOPSERVERCLI_H__10B9CFDA_9936_4932_96B7_97E3492B5164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Common\\MyWinsockBase.h"
#include "PCSdefines.h"
#include "PCSprotocol.h"

#define MAX_JOINSENDBUFSIZE		819200
#define MAX_JOINRECVBUFSIZE		819200

class wsShopServerCli : public MyWinsockBase
{

public:

	BYTE	m_SendBuf[MAX_JOINSENDBUFSIZE];
	int		m_SendBufLen;
	BYTE	m_RecvBuf[MAX_JOINRECVBUFSIZE];
	int		m_RecvBufLen;

public:

	BOOL SetProtocolCore(void (*pc)(DWORD, BYTE*, int));
	void (*wsProtocolCore)(DWORD protoNum, BYTE *aRecv, int aLen);

	int FDWRITE_MsgDataSend();
	int DataRecv();
	BOOL DataSend(char *buf, int len);
	BOOL Connect(char *ip_addr, unsigned short port, DWORD WinMsgNum);
	wsShopServerCli();
	virtual ~wsShopServerCli();
};

#endif // !defined(AFX_WSSHOPSERVERCLI_H__10B9CFDA_9936_4932_96B7_97E3492B5164__INCLUDED_)
