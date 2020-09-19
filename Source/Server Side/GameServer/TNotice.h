#ifndef __TNOTICE_DEF__
#define __TNOTICE_DEF__

#include "..\Include\ProDef.h"
#include "user.h"
#include "protocol.h"

inline DWORD _ARGB(BYTE a, BYTE r, BYTE g, BYTE b) { return (a << 24) + (b << 16) + (g << 8) + (r); }

#define TNOTICE_PRIORITY_LV1	10
#define TNOTICE_PRIORITY_LV2	11
#define TNOTICE_PRIORITY_LV3	12

#define TNOTICE_COLOR_DEFAULT	_ARGB ( 255, 255, 200, 80 )
#define TNOTICE_COLOR_RED		_ARGB ( 255, 254, 81,  81 )
#define TNOTICE_COLOR_BLUE		_ARGB ( 255, 128, 149, 196)

#define TNOTICE_SPEED_DEFAULT	20
#define TNOTICE_DELAY_DEFAULT	0
#define TNOTICE_COUNT_DEFAULT	1

// 0 : �ϴ� ��������
// 1 : ���� ���� �޽���
// 2 : ��� ����
// 10 : �����̵� ����
// 11 : �����̵� ����
// 12 : �����̵� ����
#define TNOTICE_TYPE_BOTTOM		0
#define TNOTICE_TYPE_LEFTTOP	1
#define TNOTICE_TYPE_GUILD		2
#define TNOTICE_TYPE_SLIDE_LV1	10
#define TNOTICE_TYPE_SLIDE_LV2	11
#define TNOTICE_TYPE_SLIDE_LV3	12

class TNotice
{

// S T A T I C 
public:
	static void MakeNoticeMsg( LPVOID lpNotice, BYTE btType, char *szNoticeMsg );
	static void MakeNoticeMsgEx( LPVOID lpNotice, BYTE btType, char *szNoticeMsg, ... );

#ifdef UNICODE_MODE_20050718
	static void MakeNoticeMsgUnicode( LPVOID lpNotice, BYTE btType, char *szNoticeMsg, int iSize);
#endif

	static void SetNoticeProperty(	LPVOID lpNotice, BYTE btType, 
									DWORD dwColor=TNOTICE_COLOR_DEFAULT , BYTE btCount=TNOTICE_COUNT_DEFAULT, 
									WORD wDelay=TNOTICE_DELAY_DEFAULT, BYTE btSpeed=TNOTICE_SPEED_DEFAULT     );
	
	static void SendNoticeToUser( int aIndex, LPVOID lpNotice );
	static void SendNoticeToAllUser( LPVOID lpNotice );
	
public:
	static void AllSendServerMsg(char *chatmsg);
	static void GCServerMsgStringSend(char *szMsg, int aIndex, BYTE type);
	

// N O R M A L
public:
	
	// ���� ������ �Ϲ������� TNOTICE_TYPE_LEFTTOP Ÿ���� ���´�.
	// SendToAllUser�� ��� �� ��� �����ڿ��� TNOTICE_TYPE_LEFTTOP ���ڸ� �־��ָ� �ȴ�.
	TNotice( BYTE btType = TNOTICE_TYPE_BOTTOM );

#ifdef MODIFY_NOTICE_20040325
	void SetSpeed( BYTE btSpeed ){ m_Notice.btSpeed = btSpeed;	}
	void SetColor( DWORD dwColor){ m_Notice.dwColor = dwColor;	}
	void SetCount( BYTE btCount ){ m_Notice.btCount = btCount;	}
	void SetDelay( WORD wDelay ) { m_Notice.wDelay  = wDelay;	}
#endif
	void SetType( BYTE btType )	 { m_Notice.type    = btType;	}

	void SendToUser( int aIndex, char *szMsg, ... );
	void SendToAllUser( char *szMsg, ...);
	void MsgOutputAll(char *message, ...);

private:
	PMSG_NOTICE m_Notice;

};

#endif