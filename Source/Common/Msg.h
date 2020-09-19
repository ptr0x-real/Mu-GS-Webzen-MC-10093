
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
    BYTE  headcode;     // 헤더코드(구분)
    BYTE  version;      // 버젼
    char  caption[21];  // 제목
    int   count;        // 개수
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
	// 메시지 데이터 메모리를 해제한다.  (클래스 파괴시 호출된다.)
	void lMsgFree();


public:
	char	szDefaultMsg[50];
	
	CMsg();
	~CMsg();
	
	//////////////////////////////////////////////////////////////////////
	// 메시지를 초기화 하며
	// filename 에는 메시지 포멧의 파일 이름을 넣어준다.
	void LoadWTF(char *filename);
	
	//////////////////////////////////////////////////////////////////////
	// 지정된 번호를 찾아 번호에 해당하는 문자열의 포인터를 리턴한다.
	char *Get(int number);
	
	//////////////////////////////////////////////////////////////////////
	// 메시지모두를 출력한다. (아직은 사용안한다)
	void lMsgListPrint();
};

#endif
