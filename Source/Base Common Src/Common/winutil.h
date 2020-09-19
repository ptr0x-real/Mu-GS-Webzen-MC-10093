#ifndef __WINUTIL_H__
#define __WINUTIL_H__

// ���� �ѵ��� �˻��Ѵ�. max���� ������ 0 ���� ������ 1�̴�.
#define CHECK_LIMIT(C_VALUE, C_MAX) (C_VALUE<0?FALSE:C_VALUE>(C_MAX-1)?FALSE:TRUE)
#define CHECK_LIMIT_NONZERO(C_VALUE, C_MAX) (C_VALUE<=0?FALSE:C_VALUE>(C_MAX-1)?FALSE:TRUE)

#define MAKE_ITEMNUM_WORD(LBYTE, HBYTE)(LBYTE+(HBYTE*256))

// ���� ����Ʈ�� ���� ����Ʈ�� �̿��ؼ� ������ ��ȣ�� ��´�.
#ifdef ITEM_INDEX_EXTEND_20050706
#define MAKE_ITEMNUM_512(LBYTE, MBYTE, HBYTE)((LBYTE+((MBYTE&0x80)<<1)+((HBYTE&0xF0)<<5))%512)
#endif
#define MAKE_ITEMNUM_32(LBYTE, HBYTE)((LBYTE+((HBYTE>>7)*256))%32)

extern int MakeItemNumber(int type, int index);

// Type �� Index�� �Է��Ͽ� ������ ��ȣ�� �����Ѵ�
#define MAKE_ITEMNUM(ITYPE, IINDEX) MakeItemNumber(ITYPE, IINDEX)//((ITYPE<<ITEM_BITSHIFT)+IINDEX)
#define MAKE_ITEMNUM2(ITYPE, IINDEX) ((ITYPE<<ITEM_BITSHIFT)+IINDEX)

// Total Type ���� �� ������ ������ Index ���� ���Ѵ�.
#define MAKE_ITEMNUM_INDEX(ITYPE)(ITYPE%MAX_ITEM_INDEX)


extern void BuxConvert(char *buf, int size);
extern BOOL SQLSyntexCheckConvert(char *SQLString);
extern BOOL SQLSyntexCheck(char *SQLString);
extern BOOL SpaceSyntexCheck(char *string);
extern BOOL StrHangulCheck(char * str);
extern void FileSaveString(char *filename, char * string);
extern BOOL IsFile(char *fileName);
extern void GetTodayString(char * szDate);

extern void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);
extern void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
extern void PHeadSetW(LPBYTE lpBuf, BYTE head, int size);
extern void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
extern void PHeadSetBE(LPBYTE lpBuf, BYTE head, int size);
extern void PHeadSubSetBE(LPBYTE lpBuf, BYTE head, BYTE sub, int size);



class char_ID 
{
	char Name[MAX_IDSTRING+1];
public :
	char_ID(char *szName=NULL)
	{
		memset(Name, 0, MAX_IDSTRING+1);
		if( szName != NULL )
			memcpy(Name, szName, MAX_IDSTRING);
	}

	char* GetBuffer()
	{
		return Name;
	}

	char_ID operator =(char* buffer)
	{
		memcpy(Name, buffer, MAX_IDSTRING);
		return (*this);
	}
	
	int  GetLength()
	{
		return strlen(Name);
	}
};


#endif

