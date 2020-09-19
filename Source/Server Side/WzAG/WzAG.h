#ifdef WZAG_EXPORTS
#define WZAG_API extern "C" __declspec(dllexport)
#else
#define WZAG_API extern "C" __declspec(dllimport)
#endif

enum DATA_FILE_INDEX
{
	FILE_ITEM = 0x0,
	FILE_ITEMLOCAL = 0x1,
	FILE_SKILL = 0x2,
	FILE_SKILLLOCAL = 0x3,
	FILE_QUEST = 0x4,
	FILE_QUESTLOCAL = 0x5,
	FILE_CHECKSUM = 0x6,
	FILE_MONSTER = 0x7,
	FILE_GATE = 0x8,
	FILE_MONSTERSETBASE = 0x9,
	FILE_NPGGAUTH = 0xa,
	FILE_SHOP0 = 0xb,
	FILE_SHOP1 = 0xc,
	FILE_SHOP2 = 0xd,
	FILE_SHOP3 = 0xe,
	FILE_SHOP4 = 0xf,
	FILE_SHOP5 = 0x10,
	FILE_SHOP6 = 0x11,
	FILE_SHOP7 = 0x12,
	FILE_SHOP8 = 0x13,
	FILE_SHOP9 = 0x14,
	FILE_SHOP10 = 0x15,
	FILE_SHOP11 = 0x16,
	FILE_SHOP12 = 0x17,
	FILE_MOVEREQ = 0x18,
	FILE_MOVEREQLOCAL = 0x19,
	FILE_SETITEMOPTION = 0x1a,
	FILE_SETITEMOPTIONLOCAL = 0x1b,
	FILE_SETITEMTYPE = 0x1c,
	FILE_SETITEMTYPELOCAL = 0x1d,
	FILE_SHOP13 = 0x1e,
	FILE_SHOP14 = 0x1f,
	FILE_SHOP15 = 0x20,
	FILE_SHOP16 = 0x21,
	FILE_SHOP17 = 0x22,
	FILE_SHOP18 = 0x23,
};

WZAG_API void	AGConnectSetFS();
WZAG_API void	AGGetClientVersion(DWORD Handle, char *szClientVersion, char *szClientSerial);
WZAG_API char  *AGGetDataBuffer();
WZAG_API int	AGGetDataBufferSize();
WZAG_API void	AGGetKey(HANDLE handle, char *key, int startindex, int len);
WZAG_API void	AGReleaseHandle(DWORD handle);
WZAG_API void	AGRequestData(DWORD handle, int type);
WZAG_API DWORD	AGSetInfo(int iCountryCode, BYTE cPartition, short iServerCode, char* szServerVersion,  char* szServerName, int ServerType,  void  (*callbackfunc)(int, int));
WZAG_API void	AGSetSubInfo(DWORD handle, int gServerMaxUser, int gObjTotalUser, int gMonsterHp, int gItemDropPer, float gAddExperience, int EventFlag);
