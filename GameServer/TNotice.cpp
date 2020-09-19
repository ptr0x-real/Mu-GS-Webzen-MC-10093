#include "stdafx.h"
#include "TNotice.h"

#include "..\\Include\\Prodef.h"
#include "..\\Common\\winutil.h"

#include "user.h"
#include "protocol.h"
#include "..\\include\\define.h"


void TNotice::MakeNoticeMsg( LPVOID lpNotice, BYTE btType, char *szNoticeMsg )
{
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;
	pNotice->type = btType;

#ifdef UNICODE_MODE_20050718
	char szTempMsg[4096] = {0,};
	memset(pNotice->Notice, 0, sizeof(pNotice->Notice));
	
	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szNoticeMsg, strlen(szNoticeMsg), (WCHAR*)szTempMsg, sizeof(szTempMsg));
	memcpy(pNotice->Notice, szTempMsg, sizeof(pNotice->Notice));
	pNotice->Notice[sizeof(pNotice->Notice)-2] = 0;
	pNotice->Notice[sizeof(pNotice->Notice)-1] = 0;
	PHeadSetB((LPBYTE)pNotice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + (wcslen((WCHAR*)pNotice->Notice)*2+2));
#else
#ifdef MODIFY_SERVER_NOTICE_BUFFERCOPY_20041018
	memcpy(pNotice->Notice, szNoticeMsg, MAX_GUILDNOTICE);
	pNotice->Notice[MAX_GUILDNOTICE-1] = 0;
#else
	wsprintf(pNotice->Notice, szNoticeMsg);								
#endif 
	
#ifdef MODIFY_NOTICE_20040325
	PHeadSetB((LPBYTE)pNotice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + (strlen(pNotice->Notice)+1));
#else
	PHeadSetB((LPBYTE)pNotice, 0x0D, 4+(strlen(pNotice->Notice)+1));
#endif
#endif
	
}

void TNotice::MakeNoticeMsgEx( LPVOID lpNotice, BYTE btType, char *szNoticeMsg, ... )
{	
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;
	pNotice->type = btType;

#ifdef UNICODE_MODE_20050718
	char szTempMsg[4096] = {0,};
	memset(pNotice->Notice, 0, sizeof(pNotice->Notice));

	va_list	pArguments;
	va_start(pArguments, szNoticeMsg);
    vsprintf(szTempMsg, szNoticeMsg, pArguments);
    va_end(pArguments);

	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szTempMsg, strlen(szTempMsg), (WCHAR*)pNotice->Notice, sizeof(pNotice->Notice));
	pNotice->Notice[sizeof(pNotice->Notice)-2] = 0;
	pNotice->Notice[sizeof(pNotice->Notice)-1] = 0;
	PHeadSetB((LPBYTE)pNotice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + (wcslen((WCHAR*)pNotice->Notice)*2+2));
#else
	va_list	pArguments;
	va_start(pArguments, szNoticeMsg);
    vsprintf(pNotice->Notice, szNoticeMsg, pArguments);
    va_end(pArguments);

#ifdef MODIFY_NOTICE_20040325
	PHeadSetB((LPBYTE)pNotice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + (strlen(pNotice->Notice)+1));
#else
	PHeadSetB((LPBYTE)pNotice, 0x0D, 4+(strlen(pNotice->Notice)+1));
#endif
#endif
}


#ifdef UNICODE_MODE_20050718
void TNotice::MakeNoticeMsgUnicode( LPVOID lpNotice, BYTE btType, char *szNoticeMsg, int iSize )
{
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;
	pNotice->type = btType;

	if (iSize > sizeof(pNotice->Notice))
		iSize = sizeof(pNotice->Notice);

	PHeadSetB((LPBYTE)pNotice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(pNotice->Notice) + iSize);
	memcpy(pNotice->Notice, szNoticeMsg, iSize);
	szNoticeMsg[iSize-2]	= '\0';
	szNoticeMsg[iSize-1]	= '\0';
}
#endif


void TNotice::SetNoticeProperty( LPVOID lpNotice, BYTE btType, DWORD dwColor, BYTE btCount, WORD wDelay, BYTE btSpeed)
{
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;

#ifdef MODIFY_NOTICE_20040325
	pNotice->type	 = btType;
	pNotice->dwColor = dwColor;
	pNotice->btCount = btCount;
	pNotice->wDelay	 = wDelay;
	pNotice->btSpeed = btSpeed;
#endif

}

void TNotice::SendNoticeToAllUser( LPVOID lpNotice )
{
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)pNotice, pNotice->h.size);
		}
	}
}

void TNotice::SendNoticeToUser( int aIndex, LPVOID lpNotice )
{
	LPPMSG_NOTICE pNotice = (LPPMSG_NOTICE) lpNotice;
	DataSend(aIndex, (LPBYTE)pNotice, pNotice->h.size);
}

void TNotice::AllSendServerMsg(char *chatmsg)
{
	PMSG_NOTICE	pNotice;
		
	TNotice::MakeNoticeMsg( &pNotice, 0, chatmsg );

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}

}

void TNotice::GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type)
{
	PMSG_NOTICE	pNotice;

	TNotice::MakeNoticeMsg( &pNotice, type, szMsg );

	DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
}

TNotice::TNotice( BYTE btType )
{
#ifdef MODIFY_NOTICE_20040325
	m_Notice.btCount = TNOTICE_COUNT_DEFAULT;
	m_Notice.btSpeed = TNOTICE_SPEED_DEFAULT;
	m_Notice.dwColor = TNOTICE_COLOR_DEFAULT;
	m_Notice.wDelay  = TNOTICE_DELAY_DEFAULT;
#endif
	m_Notice.type	 = btType;
}

void TNotice::SendToAllUser( char *szMsg, ...)
{

#ifdef UNICODE_MODE_20050718
	char szTempMsg[4096] = {0,};
	memset(m_Notice.Notice, 0, sizeof(m_Notice.Notice));

	va_list	pArguments;
	va_start(pArguments, szMsg);
    vsprintf(szTempMsg, szMsg, pArguments);
    va_end(pArguments);

	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szTempMsg, strlen(szTempMsg), (WCHAR*)m_Notice.Notice, sizeof(m_Notice.Notice));
	m_Notice.Notice[sizeof(m_Notice.Notice)-2] = 0;
	m_Notice.Notice[sizeof(m_Notice.Notice)-1] = 0;
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(m_Notice.Notice) + (wcslen((WCHAR*)m_Notice.Notice)*2+2));
#else
	va_list	pArguments;
	va_start(pArguments, szMsg);
    vsprintf(m_Notice.Notice, szMsg, pArguments);
    va_end(pArguments);

#ifdef MODIFY_NOTICE_20040325
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(m_Notice.Notice) + (strlen(m_Notice.Notice)+1));
#else
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, 4+(strlen(m_Notice.Notice)+1));
#endif
#endif

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)&m_Notice, m_Notice.h.size);
		}
	}
}

void TNotice::SendToUser( int aIndex, char *szMsg, ... )
{
#ifdef UNICODE_MODE_20050718
	char szTempMsg[4096] = {0,};
	memset(m_Notice.Notice, 0, sizeof(m_Notice.Notice));

	va_list	pArguments;
	va_start(pArguments, szMsg);
    vsprintf(szTempMsg, szMsg, pArguments);
    va_end(pArguments);

	MultiByteToWideChar(DEFAULT_CODEPAGE, 0, (char*)szTempMsg, strlen(szTempMsg), (WCHAR*)m_Notice.Notice, sizeof(m_Notice.Notice));
	m_Notice.Notice[sizeof(m_Notice.Notice)-2] = 0;
	m_Notice.Notice[sizeof(m_Notice.Notice)-1] = 0;
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(m_Notice.Notice) + (wcslen((WCHAR*)m_Notice.Notice)*2+2));
#else
	va_list	pArguments;
	va_start(pArguments, szMsg);
    vsprintf(m_Notice.Notice, szMsg, pArguments);
    va_end(pArguments);

#ifdef MODIFY_NOTICE_20040325
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, sizeof(PMSG_NOTICE) - sizeof(m_Notice.Notice) + (strlen(m_Notice.Notice)+1));
#else
	PHeadSetB((LPBYTE)&m_Notice, 0x0D, 4+(strlen(m_Notice.Notice)+1));
#endif
#endif

	DataSend(aIndex, (LPBYTE)&m_Notice, m_Notice.h.size);
}


void TNotice::MsgOutputAll(char *message, ...)
{
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, message);
	vsprintf(&szBuffer[0], message, pArguments);
	va_end(pArguments);

	for(int i = OBJECT_MIN; i < MAX_OBJECT; i++)
	{
		if(gObj[i].Connected == 3)
		{
			GCServerMsgStringSend(&szBuffer[0], i, 1);
		}
	}
}