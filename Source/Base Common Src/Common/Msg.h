
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MSG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MSG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef __MSGDLL_H__
#define __MSGDLL_H__

#include <stdio.h>

#ifdef MSG_EXPORTS
#define MSG_API __declspec(dllexport)
#else
#define MSG_API __declspec(dllimport)
#endif

struct MSG_STRUCT 
{
	MSG_STRUCT *next;
	int		number;
	BYTE	*msg;
};


#define XORCODE             0xCA
#define TEXTCODEHEADCODE    0xCC
#define TEXTCODEVERSION     0x01

typedef struct
{
    BYTE  headcode;     // ����ڵ�(����)
    BYTE  version;      // ����
    char  caption[21];  // ����
    int   count;        // ����
} TCHeader;



#define MAX_MSG_INDEX		32768



// This class is exported from the Msg.dll
class MSG_API CMsg {

private :
	MSG_STRUCT	* Msghead;
	MSG_STRUCT	* Msgcur;

	MSG_STRUCT	* MsgIndex[MAX_MSG_INDEX];

	TCHeader	LoadHeader;

	int  lMsgListInit();
	int  lMsgListAdd(int number, BYTE *msg);
	
	MSG_STRUCT * lMsgListNew();

	void XorBuffer(char *buf, int len);
	void DataFileLoadVer01(FILE *fp);	

	//////////////////////////////////////////////////////////////////////
	// �޽��� ������ �޸𸮸� �����Ѵ�.  (Ŭ���� �ı��� ȣ��ȴ�.)
	void lMsgFree();


public:
	char	szDefaultMsg[50];
	
	CMsg();
	~CMsg();
	
	//////////////////////////////////////////////////////////////////////
	// �޽����� �ʱ�ȭ �ϸ�
	// filename ���� �޽��� ������ ���� �̸��� �־��ش�.
	void LoadWTF(char *filename);
	
	//////////////////////////////////////////////////////////////////////
	// ������ ��ȣ�� ã�� ��ȣ�� �ش��ϴ� ���ڿ��� �����͸� �����Ѵ�.
	char *Get(int number);
	
	//////////////////////////////////////////////////////////////////////
	// �޽�����θ� ����Ѵ�. (������ �����Ѵ�)
	void lMsgListPrint();
};

#endif
