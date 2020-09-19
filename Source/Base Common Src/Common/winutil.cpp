#include "stdafx.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\Prodef.h"
#include "..\\include\\ItemDef.h"
#include <stdio.h>
#include <time.h>

int MakeItemNumber(int type, int index)
{
	if(type < 0 || type > MAX_ITEM_TYPE || index < 0 || index > MAX_ITEM_INDEX)
	{
		return 0;
	}

	return (type<<ITEM_BITSHIFT)+index;
}

#define MAX_BUXCCODE				3

static BYTE	bBuxCode[MAX_BUXCCODE] = {0xFC, 0xCF, 0xAB};

void BuxConvert(char *buf, int size)
{
	for( int n=0; n<size; n++) buf[n] ^= (char)bBuxCode[n%MAX_BUXCCODE];
}

BOOL SQLSyntexCheck(char *SQLString)
{
	char *temp;
	temp = strchr( SQLString, '\'');
	if( temp == NULL ) 
	{
		temp = strchr( SQLString, 0x20);
		if( temp == NULL ) return TRUE;
		return FALSE;
	}
	return FALSE;
}

BOOL  SQLSyntexCheckConvert(char *SQLString)
{
	char tempsql[256];
	char *temp;
	char *temp2 = SQLString;
	int	 lOfs=0, tOfs=0, sOfs=0;

	if( SQLString == NULL )
		return FALSE;
	
	while(1)
	{
		temp = strchr( (temp2), '\'');
		if( temp) 
		{
			if( temp-(temp2) )
			{
				memcpy(tempsql+tOfs, temp2, temp-(temp2));
			}
			lOfs = temp-(temp2+sOfs);
			tOfs += lOfs;
			temp2 += lOfs;
			tempsql[tOfs] ='\0';
			tOfs += 2;
			temp2 += 1;
			strcat(tempsql,"''");
		}
		else
		{
			strcpy(tempsql+tOfs, temp2);
			break;
		}
	}

	strcpy(SQLString, tempsql);
//	printf("%s\n",tempsql);
	return TRUE;
}

BOOL SpaceSyntexCheck(char *string)
{
	char *temp;
	temp = strchr( string, 0x20);
	if( temp == NULL ) return TRUE;
	return FALSE;
}

// �ѱ����� üũ�ؼ� �ѱ��϶��� �ϼ��� ���ڸ� �ִ´�.
BOOL StrHangulCheck(char * str)
{
	BYTE code1, code2, code;
	int i=0, n=0;
	char sztemp[256]="";
	BOOL result = TRUE;
	int  count=255;

	while( count-- )
	{
		code1 = *(str+i);
		i++;
		if( code1 == '\0' ) break;
		if( code1 < 128 ) {
			sztemp[n] = code1; n++;
			continue;
		}
		code2 = *(str+i);
		i++;
		code = (code2)|(code1)<<8;
		if( code > 0 ) {
			sztemp[n] = code1; n++;
			sztemp[n] = code2; n++;
		}
		else result = FALSE;
		if( code2 == '\0' ) break;
	}
	strcpy( str, sztemp);
	return TRUE;
}

void FileSaveString(char *filename, char * string)
{
	FILE * fp;

	fp = fopen(filename, "at");
	if( fp == NULL ) return;
	fprintf(fp,string);
	fprintf(fp,"\n");
	fclose(fp);
}

BOOL IsFile(char *fileName)
{
	FILE * fp;
	fp = fopen(fileName, "rb");
	if( fp == NULL ) return FALSE;
	fclose(fp);
	return TRUE;
}


void GetTodayString(char * szDate)
{
	//char		szTemp[50];
	struct tm * today;
	time_t		ltime;
	
	time( &ltime );
	today = localtime( &ltime );
	
	today->tm_year += 1900;
	
	wsprintf(szDate, "%02d%02d%02d",today->tm_year, today->tm_mon+1,today->tm_mday);

}

void PHeadSetB(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] = PMHC_BYTE;
	lpBuf[1] = (BYTE)size;
	lpBuf[2] = head;
}

void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] = PMHC_BYTE;
	lpBuf[1] = (BYTE)size;
	lpBuf[2] = head;
	lpBuf[3] = sub;
}

void PHeadSetW(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] = PMHC_WORD;
	lpBuf[1] = HIBYTE(size);
	lpBuf[2] = LOBYTE(size);
	lpBuf[3] = head;
}

void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] = PMHC_WORD;
	lpBuf[1] = HIBYTE(size);
	lpBuf[2] = LOBYTE(size);
	lpBuf[3] = head;
	lpBuf[4] = sub;
}

void PHeadSetBE(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] = PMHCE_BYTE;
	lpBuf[1] = (BYTE)size;
	lpBuf[2] = head;
}

void PHeadSubSetBE(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] = PMHCE_BYTE;
	lpBuf[1] = (BYTE)size;
	lpBuf[2] = head;
	lpBuf[3] = sub;
}
