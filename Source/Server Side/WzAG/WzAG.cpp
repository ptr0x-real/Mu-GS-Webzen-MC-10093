//WzAG.cpp	: GameServer 1.00.93

#include "stdafx.h"
#include "WzAG.h"
#include "LogToFile.h"
#include "WzAGConsole.h"

FILE * SMDFile;

char *DataBuffer;
DWORD DataBufferSize;

BYTE szServerKey[19] = {0x29,0x23,0xBE,0x00,0xE1,0x6C,0xD6,0xAE,0x52,0x90,0x49,0xF1,0xF1,0xBB,0xE9,0xEB,0xB3,0xA6,0xDB};

char			sztmpServerName[51];

#define	DATA_PATH	"M:\\"

CLogToFile WZAG_TEST_LOG("WZAG_TEST_LOG", ".\\WZAG_TEST_LOG", TRUE);
char* GetCountryNameByCode(int iCountryCode);
char* GetServerTypeByType(int ServerType);
char* GetEventFlag(int flag);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	
    return TRUE;
}

///////////////////////////////////////////////
//
///////////////////////////////////////////////
WZAG_API void AGConnectSetFS()
{
	MessageBox(NULL, "?", "WZAG_API void AGConnectSetFS()", MB_OK );
}

/////////////////////////////////////////////
//
/////////////////////////////////////////////
WZAG_API void AGGetClientVersion(DWORD Handle, char *szClientVersion, char *szClientSerial)
{
	char tmpszClientVersion[20];
	char tmpszClientSerial[20];

	char *filename = new char[256];

	strcpy(filename, DATA_PATH);
	strcat(filename, "lang\\jpn\\commonloc.cfg");

	GetPrivateProfileString("GameServerInfo", "ClientExeVersion", "", tmpszClientVersion, 20, filename);
	GetPrivateProfileString("GameServerInfo", "ClientExeSerial", "", tmpszClientSerial, 20, filename);

	memcpy(szClientVersion, tmpszClientVersion, 20);
	memcpy(szClientSerial, tmpszClientSerial, 20);

#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGGetClientVersion : (szClientVersion: %s) (szClientSerial: %s)", szClientVersion, szClientSerial);
#endif //_DEBUG
}

/////////////////////////////////////////////
// File Contents
/////////////////////////////////////////////
WZAG_API char *AGGetDataBuffer()
{
#ifdef _DEBUG
	//WZAG_TEST_LOG.Output("[DEBUG] WZAG_API char *AGGetDataBuffer : DataBuffer: \n%s\n", DataBuffer);
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API char *AGGetDataBuffer : Send DataBuffer");
#endif //_DEBUG
	return DataBuffer;
}

/////////////////////////////////////////////
// File Size (could be in DWORD)
/////////////////////////////////////////////
WZAG_API int AGGetDataBufferSize()
{
#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API int AGGetDataBufferSize : DataBufferSize: %d", DataBufferSize);
#endif //_DEBUG
	return DataBufferSize;
}

/////////////////////////////////////////////
// Return by pointer requested server key
/////////////////////////////////////////////
WZAG_API void AGGetKey(HANDLE handle, char *key, int startindex, int len)
{
	memcpy(key, szServerKey, sizeof(szServerKey));
#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGGetKey : (handle: %d) (key: %02X) (startindex: %d) (len: %d)", handle, key, startindex, len);
#endif //_DEBUG
}

/////////////////////////////////////////////
// Tell AuthServer the Application has quit
/////////////////////////////////////////////
WZAG_API void AGReleaseHandle(DWORD handle)
{
	g_WzAGConsole.Destroy();

	//Terminate all buffers
	delete [] DataBuffer;
	DataBufferSize = 0;

#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGReleaseHandle : handle: %d ", handle);
#endif //_DEBUG
}

///////////////////////////////////////////////
// Ask AuthServer for Files to load
///////////////////////////////////////////////
WZAG_API void AGRequestData(DWORD handle, int type)
{
#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGRequestData : (handle: %d) (type: %02X)", handle, type);
#endif //_DEBUG

	char *filename = new char[256];

	switch (type)
	{
	case FILE_ITEM:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\kor\\item(kor).txt");
		break;
	case FILE_ITEMLOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\item(jpn).txt");
		break;
	case FILE_SKILL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\Kor\\skill(Kor).txt");
		break;
		//////////////////////////////////////////////////////////////////////////
	case FILE_SKILLLOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\skill(jpn).txt");
		break;
	case FILE_QUEST:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\Quest(jpn).txt");
		break;
	case FILE_QUESTLOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\Quest(jpn).txt");
		break;
	case FILE_CHECKSUM:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\CheckSum.dat");
		break;
	case FILE_MONSTER:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Monster.txt");
		break;
	case FILE_GATE:	
		strcpy(filename, DATA_PATH);
		strcat(filename, "gate.txt");
		break;
	case FILE_MONSTERSETBASE:
		strcpy(filename, DATA_PATH);
		strcat(filename, "MonsterSetBase.txt");
		break;
	case FILE_NPGGAUTH:
		break;
	case FILE_SHOP0:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop0.txt");
		break;
	case FILE_SHOP1:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop1.txt");
		break;
	case FILE_SHOP2:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop2.txt");
		break;
	case FILE_SHOP3:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop3.txt");
		break;
	case FILE_SHOP4:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop4.txt");
		break;
	case FILE_SHOP5:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop5.txt");
		break;
	case FILE_SHOP6:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop6.txt");
		break;
	case FILE_SHOP7:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop7.txt");
		break;
	case FILE_SHOP8:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop8.txt");
		break;
	case FILE_SHOP9:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop9.txt");
		break;
	case FILE_SHOP10:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop10.txt");
		break;		
	case FILE_SHOP11:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop11.txt");
		break;
	case FILE_SHOP12:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop12.txt");
		break;
	case FILE_MOVEREQ:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\movereq(jpn).txt");
		break;
	case FILE_MOVEREQLOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\movereq(jpn).txt");
		break;
	case FILE_SETITEMOPTION:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\itemsetoption(jpn).txt");
		break;
	case FILE_SETITEMOPTIONLOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\itemsetoption(jpn).txt");
		break;
	case FILE_SETITEMTYPE:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\itemsettype(jpn).txt");
		break;
	case FILE_SETITEMTYPELOCAL:
		strcpy(filename, DATA_PATH);
		strcat(filename, "lang\\jpn\\itemsettype(jpn).txt");
		break;
	case FILE_SHOP13:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop13.txt");
		break;
	case FILE_SHOP14:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop14.txt");
		break;
	case FILE_SHOP15:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop15.txt");
		break;
	case FILE_SHOP16:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop16.txt");
		break;
	case FILE_SHOP17:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop17.txt");
		break;
	case FILE_SHOP18:
		strcpy(filename, DATA_PATH);
		strcat(filename, "Shop18.txt");
		break;
	}
#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGRequestData : !!Attempt Load File: %s!!", filename);
#endif //_DEBUG

	//OpenFile
	if( SMDFile = fopen(filename,"rb") )
	{
		//Set the end position of the file contents (for ftell later)
		fseek(SMDFile, 0, SEEK_END);

		//Store file size at buffer variable
		if( DataBufferSize = ftell(SMDFile) )
		{
			//Set the start position of the file contents (for fread later)
			fseek(SMDFile, 0, SEEK_SET) ;

			//Allocate DataBuffer according to file size (DataBufferSize)
			DataBuffer = new char[DataBufferSize+1];

			//Read whole file contents into DataBuffer
			fread(DataBuffer, DataBufferSize, 1, SMDFile);
		}
		//Close the opened file
		fclose(SMDFile);
	}
}

/////////////////////////////////////////////
// Set ServerInfo at AuthServer
/////////////////////////////////////////////
WZAG_API DWORD AGSetInfo(int iCountryCode, BYTE cPartition, short iServerCode, char* szServerVersion, char* szServerName, int ServerType, void(*callbackfunc)(int, int))
{
	if( DataBuffer )		delete [] DataBuffer;
	if( DataBufferSize )	DataBufferSize = 0;

	//Set WZAG the current ServerName
	strcpy(sztmpServerName, szServerName);

#ifdef _DEBUG
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API DWORD AGSetInfo : (iCountryCode: %d -> %s) (cPartition: %d) (iServerCode: %d) (szServerVersion: %s) (szServerName: %s) (ServerType: %s)", iCountryCode, GetCountryNameByCode(iCountryCode), cPartition, iServerCode, szServerVersion, szServerName, GetServerTypeByType(ServerType));
	g_WzAGConsole.Create();
#endif //_DEBUG
	
	return TRUE;
}

/////////////////////////////////////////////
// Set ServerStatistics at AuthServer
/////////////////////////////////////////////
WZAG_API void AGSetSubInfo(DWORD handle, int gServerMaxUser, int gObjTotalUser, int gMonsterHp, int gItemDropPer, float gAddExperience, int EventFlag)
{
#ifdef _DEBUG
	char * szEvent = GetEventFlag(EventFlag);
	WZAG_TEST_LOG.Output("[DEBUG] WZAG_API void AGSetSubInfo : (handle: %d) (gServerMaxUser: %d) (gObjTotalUser: %d) (gMonsterHp: %d) (gItemDropPer: %d) (gAddExperience: %f) (EventFlag: %s)", handle, gServerMaxUser, gObjTotalUser, gMonsterHp, gItemDropPer, gAddExperience, szEvent);
	delete szEvent;
#endif //_DEBUG
}

char* GetCountryNameByCode(int iCountryCode)
{
	return (iCountryCode==1)?("English"):(iCountryCode==2)?("Japan"):(iCountryCode==3)?("China"):(iCountryCode==4)?("Taiwan"):(iCountryCode==5)?("Thailand"):(iCountryCode==6)?("Phillipines"):(iCountryCode==7)?("Vietnam"):("Korea");
}

char* GetServerTypeByType(int ServerType)
{
	return (ServerType==1)?("TestServer"):(ServerType==2)?("Internal Server"):("MainServer");
}

char* GetEventFlag(int flag)
{
	char * szMsg = new char[512];
	memset(szMsg, 0, 512);

	#define EVENT_ACTIVE(value) ((flag&value)==value)

	if(EVENT_ACTIVE(1))		strcat(szMsg, "| Devil Square Event | ");
	if(EVENT_ACTIVE(2))		strcat(szMsg, "| Blood Castle Event | ");
	if(EVENT_ACTIVE(256))	strcat(szMsg, "| XMas Star Event | ");
	if(EVENT_ACTIVE(512))	strcat(szMsg, "| Fire Cracker Event | ");
	if(EVENT_ACTIVE(1024))	strcat(szMsg, "| Heart Of Love Event | ");
	if(EVENT_ACTIVE(2048))	strcat(szMsg, "| Medal Event | ");
	if(EVENT_ACTIVE(4096))	strcat(szMsg, "| Ring Attack Event | ");
	if(EVENT_ACTIVE(8192))	strcat(szMsg, "| EventChip Event | ");
	if(EVENT_ACTIVE(16384))	strcat(szMsg, "| Eledora Event | ");
	if(EVENT_ACTIVE(65536))	strcat(szMsg, "| NProtect CheckSum | ");

	return szMsg;
}