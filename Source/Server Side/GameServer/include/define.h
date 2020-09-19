//----------------------------------------------------------------------------
// MU ����������Ʈ ���ݿ� ���� ���ǵǾ� ���Ǿ����� �͵�..
//----------------------------------------------------------------------------
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include ".\Public.h"

//----------------------------------------------------------------------------
// �����Ͻÿ� ���Ǵ� ��
//#define _DEBUG_PROTOCOL						// ���������� ����� �ÿ� ��� 
#define LOG_PRINT			1					// �α׸� ����Ұ��ΰ�?
#define SPEEDHACK_CHECK		1					// ���ǵ����� üũ�Ұ��ΰ�?

#define TESTSERVER	 		0					// Service:0  �׽�Ʈ �����ΰ�..
#define LOCALCONNECT		0					// Service:0  �系������ ���� �����ϰ�..
#define DATANOSAVE			0					// Service:0  �����Ͱ� ������� �ʰ� �Ѵ�.
 
#define DBGUILDLOAD			1					// Service:1, DB������ �����ؼ� ��� ������ ������ ���°�..

#define LICENCIA 1

/*enum GAME_VERSION_ENUM
{
	G_V_97D,
	G_V_99B,
	G_V_S1,
	G_V_S2,
	G_V_S2_2,
	G_V_S3_E1,
	G_V_S3_E2,
	G_V_S4_5,
	G_V_S4_6, //S4.5.1
};*/

#define G_V_97D		0
#define G_V_97_99	1
#define G_V_99B		2
#define G_V_S1		3
#define G_V_S1_512	4
#define G_V_S2		5
#define G_V_S2_2	6
#define G_V_S3_E1	7
#define G_V_S3_E2	8
#define G_V_S4_5	9
#define G_V_S4_6	10

#define GAME_VERSION	G_V_99B//G_V_S4_6

// GXDev
#define OBJ_STARTUSERINDEX (MAX_OBJECT - MAX_USER)
#define PLAYER_EMPTY  0
#define PLAYER_CONNECTED 1
#define PLAYER_LOGGED 2
#define PLAYER_PLAYING 3
#define MAKE_NUMBERW(x,y)  ( (WORD)(((BYTE)((y)&0xFF)) |   ((BYTE)((x)&0xFF)<<8 ))  )
#define CFG_QUERYSHOP   "..\\data\\QueryShop.ini"
#define MAX_VIEWPORT_MONSTER 20
#define MAX_PARTY_DISTANCE_EFFECT	10
#define CFG_COMMAND        "..\\data\\NewsCommand.ini"


//--
#define PICKUP_SYSTEM
#define MAX_CHAT_LEN 60
#define MAX_ITEM_INFO	7
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )
#define MAX_PICKUP_ITEMS 20
//--
#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )
#define MAX_ACCOUNT_LEN 10//CRISTIAN
#define OBJECT_MIN					6400
// **************************
// Item Limits
// **************************
#define MAX_SUBTYPE_ITEMS 512

// ********************
// MACRO for get Item
// ********************
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))
//#define ADD_ANTI_HACK_01_20051027 //SpeedHack FIX! - BAD

#if GAME_VERSION >= G_V_S4_6
//#define KOREAN_S4_6
//#define GLOBAL_S4_6
#define JAPAN_S4_6
#endif

#if GAME_VERSION >= G_V_99B
#define EVENT_STONE
#define EVENT_RENA_20040909
#define EVENT_LOTTO
#endif

#define UPDATE_VERSION 1

/*static char *GetGameServerVersion()
{
	static char static_GameVersion[60] = {0,};

	if(static_GameVersion[0] == '\0')
	{
		sprintf(static_GameVersion, 60, "%d.%02d.%02d", (TESTSERVER == 1 ? 0 : 1), GAME_VERSION, UPDATE_VERSION);
	}

	return static_GameVersion;
}*/

#define GAMESEVER_VERSION	"1.00.92"//GetGameServerVersion()			// ���Ӽ��� ����

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TESTSERVER
	#undef TESTSERVER
#endif // TESTSERVER

// �系 �׽�Ʈ���� ����
#ifdef _RTEST
	#define TESTSERVER	 		1					// Service:0  �׽�Ʈ �����ΰ�..

	#define LOCAL_DATATEST							// �� ������ ����Ÿ ������ �̿� üũ��&������ �׽�Ʈ�� �Ѵ�.
#else
	#define TESTSERVER	 		0					// Service:0  �׽�Ʈ �����ΰ�..

	#define LOCAL_DATATEST							// �� ������ ����Ÿ ������ �̿� üũ��&������ �׽�Ʈ�� �Ѵ�.

//	#ifdef FOR_CHECKSUM_SERVER			// ���Ӽ��� ������ ���ϰ� ���� ����Ÿ������ �̿�
//		#define LOCAL_DATATEST
//	#else	// FOR_CHECKSUM_SERVER		// ���Ӽ��� ������ ���
// ���Ӽ��� ���� (foguy)
//		#define AUTH_GAMESERVER
//		#define AUTH_GAMESERVER_CHECKKEY
//	#endif	// FOR_CHECKSUM_SERVER
#endif // _RTEST

// �������� ����
#ifdef _CS
	#define GAMESERVER_TO_CASTLESIEGESERVER			// ������ ���뼭���� ����
	#define GAMESERVER_TO_CRYWOLFSERVER				// ũ���̿��� ���뼭���� ����
#endif


//������ NProtect���� ����
#if defined(FOR_KOREA)
#if GAME_VERSION >= G_V_S3_E1
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2.5 ��� (hnine)
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
#elif GAME_VERSION >= G_V_S2
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#elif GAME_VERSION >= G_V_S1
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_15_20050411
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
#endif

#elif defined(FOR_JAPAN)
	// ** �Ϻ� ������ �� ���Ӱ��� 2.0���� ����� ������Ʈ�� �߰��� ���·� �������� �ؾ� ��.
	// �߰� ���� : ggsrvdll.lib, ggsrvlib.lib

	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#elif defined(FOR_TAIWAN)
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2.5 ��� (hnine)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

#elif defined(FOR_CHINA)
	//�߱��� NPROTECT ������
#elif defined(FOR_VIETNAM)
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123     
#elif defined(FOR_USA)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
#elif defined(FOR_PHILIPPINE)
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
#else
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 1.5 ��� (u4us)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_15_20050411
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 1.0 ��� (u4us)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
#endif

//#ifndef FOR_KOREA
	// [������Ʈ] �ؿ� �κ�����ȭ on/off (hnine)
#if GAME_VERSION >= G_V_S2_2
#ifndef KOREAN_S4_6
	#define ADD_PARTIALY_CHARGE_ON
#endif
#endif
//#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#if 0
#define GAMESERVER_TO_CASTLESIEGESERVER			// ������ ���뼭���� ����

#define GAMESERVER_TO_CRYWOLFSERVER				// ũ���̿��� ���뼭���� ����

// ���Ӽ��� ���� (foguy)
//#define AUTH_GAMESERVER
//#define AUTH_GAMESERVER_CHECKKEY

// ##UPDATE-SET-LEVEL200-20041207 (apple)
#define FOR_NEW_TESTSERVER						// �׽�Ʈ(����) ������...
#define FOR_NEW_LIVESEVER						// �ż�(����) ������...

#define FOR_CHECKSUM_SERVER						// üũ�� ������������� (GSAuth ������, ���õ���̺� ���� ���)

// #define FOR_TEMP_SERVER							// ü�輭�� �����ÿ��� ����Ѵ�. �ӽ� PC�� ����Ʈ ���̺� �����ȴ�. ���� FOR_NEW_TESTSERVER�� ������ �ʿ��� ���̴�.

#ifdef FOR_TEMP_SERVER
	//[������Ʈ] ���� 4 ������Ʈ(***NPC ü�� ������***) - ���� �ʱ�ȭ (grooving)
	#define EXPERIENCE_SERVER_NPC_STAT_RESET
#endif 

// ��ۿ�
//#define FOR_ONAIR								// ��ۿ� (�����޽��� ��� �ʵ�)

#endif // 0

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �ȵǴ� �����ε�
//#define LOCAL_DATATEST							// �� ������ ����Ÿ ������ �̿� üũ��&������ �׽�Ʈ�� �Ѵ�.
//#define CHECKSUM_REMOVE							// üũ�� üũ�� �����Ѵ� -> 2003.09.10 Ŭ���̾�Ʈ ��ġ�� ����
//#define PRINT_MAGICBOOK_ITEMINSERT_LOG		// ������ ������ ������� �α����Ͽ� ���
//#define MAKE_ITEM_STOP						// "/�����" ��� ���� -> �׽�Ʈ ����������
//#define CS_MAKE_ITEM_POTION					// �ູ�ǹ���, ��ȥ�ǹ���, ������������� �������� ���� �� ���� (/����� �ʵ�)
//#define MUTE_EVENT_MESSAGE					// ���� ����
#define ADD_MINIDUMP_20081222					// �������� ���� �Ѵ�.(grooving)
#define MUTEX_REMOVE							// ���ؽ� üũ�� �����Ѵ�
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �׼���
#if TESTSERVER == 1
#define	MUTE_EVENT_MESSAGE						// ���� ����
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
//#define CHECK_CRT_DBG (happycat) - �����ϸ� ���� ����!

// ##UPDATE-SET-LEVEL400-20041207 (apple)
//#define FOR_CASTLE_TESTSERVER					// ĳ�� �׽�Ʈ ������..


// �ѱ� ����
#define FOR_KOREA

// �Ϻ� ����
//#define FOR_JAPAN

// �߱� ����
//#define FOR_CHINA

// �븸 ����
//#define FOR_TAIWAN

// �±� ����
//#define FOR_THAILAND

// �۷ι� ����
//#define FOR_GLOBAL

// �ʸ��� ����
//#define FOR_PHILIPPINE

// ��Ʈ�� ����
//#define FOR_VIETNAM

// �̱� K2 ����
//#define FOR_USA


///////////////////////////////////////////////////////////////////////////////
// N-PROTECT ����
///////////////////////////////////////////////////////////////////////////////
#ifdef FOR_KOREA
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2.5 ��� (hnine)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
//	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411     
#endif // FOR_KOREA

#ifdef FOR_JAPAN
	// ** �Ϻ� ������ �� ���Ӱ��� 2.0���� ����� ������Ʈ�� �߰��� ���·� �������� �ؾ� ��.
	// �߰� ���� : ggsrvdll.lib, ggsrvlib.lib

	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
	#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411     
#endif // FOR_JAPAN

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	// Ŭ���̾�Ʈ ���� �˰��� ��� �� ���ϸ� (u4us)
	#define FP_DLL_GAME_GAURD_AUTHENTICATION         "..\\lib\\"
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifndef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#ifndef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

//�߱��� NPROTECT ������
#ifndef FOR_CHINA
		// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 1.5 ��� (u4us)
		#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_15_20050411
		// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 1.0 ��� (u4us)
		#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
#endif // FOR_CHINA

	#endif
#endif

#endif // 0

///////////////////////////////////////////////////////////////////////////////
// ������ �߰� �� ���� �ڵ�
///////////////////////////////////////////////////////////////////////////////

#define	WEBZEN_PUBLIC_IPADDRESS	"218.234.76.254"	// �ѱ� ���� IP�ּ� (b4nfter)

#ifdef FOR_KOREA
	// [���׼���] PC�� ���� ��� �� ������ ���� ��� ����� ���� ó���� �����ϵ��� ���� (hnine)
	#define MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801

	#define MODIFY_CONNNECT_HACK_ATTACK_20090409
	#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		#define TEMPLOG_PRINT		0	// 1 �̸� �α� ������ ���� ����,��� �Ѵ�.
	#endif

	// [��ġ����] PC�� ����Ʈ �ý��� ���θ�� - PC�� ����Ʈ �ð� ����(RKFKA) 20070821 ����
	//#define MODIFY_PCBANG_POINT_TIME_EVENT_20070709
	// [������Ʈ] PC�� ����Ʈ �ý��� (RKFKA)
#if GAME_VERSION >= G_V_S4_5
	#define PCBANG_POINT_SYSTEM_20070206

	// [���׼���] ����� ���� �����ð� ��� ���� ���� ���� (hnine)
	#define MODIFY_BILLING_RESTTIME_BUGFIX_20070212

	// [���׼���] ���� �޽����� 3�и��� ��µǴ� ���� ���� (hnine)
	#define MODIFY_BILLING_MESSAGE_BUGFIX_20070208

	// [���׼���] PC�� ���պ��� ���� ���� ���� �����ڿ� ���� PC�� ���� ����
	//#define MODIFY_PCBANG_EXPIRE_USER_ADVANTAGE_REMOVE_20070120

	// PC�� ���պ���, PC�� ����� Ȯ�ΰ� PC�� ���� �����ð��� ���� ����
	#define PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103

	// PC�� ���� Ȯ�ΰ�, PC�� �⺻ ������ �и��Ѵ�. (apple_hnine)
	#define PCBANG_ADVANTAGE_EXPAND_20060919

	// << 20060918 PC�� ���� ���� ������,���� ���� �ڸ�Ʈ (apple_hnine) >>
	// 
	// Q) �⺻ PC�� �����̶�? - ü�»��, 150������ �߰�����ġ, Į���������̵�, �߰���, ��ź��ȣ�� ����������
	//
	// D E F I N E  
	//		[PCBANG_ADVANTAGE] - PC�� ���������� ���������� ���� ����. �⺻ PC�� ���� ����
	//		[PCBANG_COUPON_EVENT_20060124] - PC�� �����̺�Ʈ�� PCBANG_FREE_KALIMA_EVENT_20060124�� ���� ����Ǿ� ��.
	//		[PCBANG_FREE_KALIMA_EVENT_20060124] - PC�� ���������� ������ Į���� ���� �̵� ��Ŵ. �⺻ PC�� ���� ����
	//
	// V A R I A B. 
	//		m_bPCBangUser, m_bPCBangCouponUser
	//
	// D E S C R I.
	//		m_bPCBangUser, m_bPCBangCouponUser �� ���� ��� PC�� ���������� ��Ÿ����. (20060918 ������)
	//		
	//		PCBANG_ADVANTAGE �� ��� ������������ ���������� �޴� �Լ��� �ʼ��� �̵� �Լ��� Ȯ���� ���� PC�� ���������� Ȯ���Ѵ�.
	//		����!!> PC�� ���� ���� �� �ݵ�� ���Ǿ�� �Ѵ�.
	//		����!!> ���� �������� �������� ó�� �Լ��� Ȯ�� �Ǿ����Ƿ� PC�� ���������� �����ϰ� ���� PC�� ������ �ְ� ���� ���� ��쿡��
	//				m_PCBangUser ��  FALSE�� �ϸ� �ȵȴ�. �� �̵� �� �� ���� ���� ���� PC�� ���������� üũ�ϱ� ����.
	//
	//		PCBANG_FREE_KALIMA_EVENT_20060124 �� PCBANG_COUPON_EVENT_20060124 �� PCBANG_ADVANTAGE ���� ������ ���� PC�� ������ ������� �ʴ´�.
	//		����!!> m_bPCBangUser �� ������ �ִ� �κ� ������ if�� FALSE ������ �����ν� PC�� ������ �۵� ���� �ʰ� ��.
	//		����!!> PC�� ����� Ű�� PC�� �⺻ ������ �����ϰ� ���� ������ PCBANG_ADVANTAGE, PCBANG_FREE_KALIMA_EVENT_20060124 �� �Ҵ�.
	//		����!!> PC�� ������ɱ��� �����Ϸ��� PCBANG_ADVANTAGE, PCBANG_FREE_KALIMA_EVENT_20060124, PCBANG_COUPON_EVENT_20060124 �� �Ҵ�.

	#define			PCBANG_ADVANTAGE		// happycat@20050201 (PC�� ����)

	#define			HAPPY_POUCH				// happycat@20050201 (���ָӴ�) - ���� �� �̺�Ʈ
	#define			HAPPY_CAT
	#define			BUGFIX_CAT
	#define			HAPPY_CAT_USE_EXCEPTIONHANDLER
#endif

#if GAME_VERSION >= G_V_S2
	#define EVENT_SERVER_OPEN_COMMEMORATION_20050510
#endif
	// ���� ĳ���� NPC��ȭ ON/OFF
	//#define BLOOCASTLE_NPC_TALK_20031215

#if GAME_VERSION >= G_V_S2_2
	// �������� ����� �����Ҷ� PC�� Guid���� (����ĳ�� 2���̺�Ʈ(PC��) �۾��� ����
	#define ADD_PROTOCOL_PCBANG_GUID

	// �Ǿ��� ��� �̳��� ó���� ���� ������ 
	#define PCBANG_DEFAULTER
	// �Ǿ��� ��� �̳��� ó���� ���� ������ ��
#endif
	// ä�� �α� ����� ���� ������
	//#define CHAT_LOG
	// ä�� �α� ����� ���� ������ ��

	// ���ӷα� ������ ���� ������
	#define LOGINSERVER_MULOG	
	// ���ӷα� ������ ���� ������ ��	

	// ���ο� ����ó��
#if GAME_VERSION < G_V_99B
	//#define NEW_ATTACK_PROC	
#endif
	// ���ο� ����ó�� ��

	// ��ũ��Ʈ ��ȣȭ �۾�..
	//#define SCRIPT_DECODE_WORK
	// ��ũ��Ʈ ��ȣȭ �۾� ��
	
	// ������ ���� ����
	//#define DURABILITY_DOWN_CHANGE					// ������ ���� ��� ����
	//#define REPAIR_ITEM_CHANGE						// ������ ���� ����	

	// ���� ������ ��� �α����� �߰� (b4nfter)
	#define MONSTER_ITEMDROP_LOG_20040309		
#if GAME_VERSION >= G_V_99B
	// ����ĳ�� �Ϸ� ����� ���� (b4nfter)
	#define BLOODCASTLE_CHECK_ENTERCOUNT_20040304	
#endif
	// ���� ĳ�� PC�� �̺�Ʈ (�ѱ���)
#if GAME_VERSION >= G_V_S4_5
	#define FOR_BLOODCASLTE_PCEVENT		
#endif
#endif


#ifdef FOR_JAPAN						// �Ϻ���
	// ���� �Ҳɳ��� �̺�Ʈ(pelero)
	#define ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

	// �ؿ� PC�� �ű������� ����(PC�� ����Ʈ ����) 2008-04-01 by riverstyx
	#define ADD_FOREIGN_PCBANG_ADVANTAGE
	#ifdef ADD_FOREIGN_PCBANG_ADVANTAGE

		// [������Ʈ] PC�� ������ ����  2008-04-01 by riverstyx
		#define ADD_PCBANG_BUFF_MARK4

		// [������Ʈ] ����4 PC�� ����Ʈ ���� ����
		#define PCBANG_KUZIMO

		// [���׼���] �Ǿ�������Ʈ���� ����� ó��(goni)
		#define MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218

		// [������Ʈ] PC�� ����Ʈ �ý��� (RKFKA)
		#define PCBANG_POINT_SYSTEM_20070206
		// �������� ����� �����Ҷ� PC�� Guid���� (����ĳ�� 2���̺�Ʈ(PC��) �۾��� ����
		#define ADD_PROTOCOL_PCBANG_GUID

		// PC�� ���� Ȯ�ΰ�, PC�� �⺻ ������ �и��Ѵ�. (apple_hnine)
		#define PCBANG_ADVANTAGE_EXPAND_20060919
		#define PCBANG_ADVANTAGE		// happycat@20050201 (PC�� ����)
	#endif // ADD_FOREIGN_PCBANG_ADVANTAGE

	// Admin, EventGm ���� ���� ��� 20081215 (grooving)
	#define MODIFIY_AUTHORITY_INTERGRATION_20081215

	// ���&���� �����۹� ������ �۾� by natinda
//	#define MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04 

	// [������Ʈ] Ȳ�ݱü�(����) �̺�Ʈ ��� ���� Ȯ��(grooving)
	// �Ϻ��� �켱 �߰� ���� �� ������ ����(Ŭ���̾�Ʈ �� ���� �ʿ�)
	#define EVENTCHIP_DATASIZE_EXPAND_20090217

	// [������Ʈ] ���� �̺�Ʈ ������ ����Ʈ-���� �����̾� �������Ǹ�(grooving)
	#define COUPON_EVENT_ITEMLIST_20081022
	// [������Ʈ] Ȳ�ݱü� �̺�Ʈ ������ ī����ī�� �̴� �߰� (hnine)

	#define MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_2_20070408

	// [������Ʈ] Ȳ�ݱü� �̺�Ʈ ������ ����Ʈ �߰� (hnine)
	#define MODIFY_GOLDEN_ARCHER_EVENT_ITEMLIST_20061109

	// [������Ʈ] �Ϻ� ���� ��ӵ��� �ʴ� ���� 2�� ���� (hnine)
	#define MODIFY_MAGIC_SCROLL_DROPBUG_FIX_02_20070319

	// [������Ʈ] �Ϻ� ���� ��ӵ��� �ʴ� ���� ���� (hnine)
	#define MODIFY_MAGIC_SCROLL_DROPBUG_FIX_20070207

	// [���׼���] ������ �� ���� ���¿��� �� �̵� �� �̻��� ��ǥ�� �̵��Ǵ� ���� ���� (hnine)
	#define MODIFY_MAPMOVE_SET_REGEN_POSITION_20060905

	// �� ������ ����(RKFKA) 2007.12.11
	// �Ϻ� ��û���� ����ġ ��� �α׿� ���� �ε����� �̺�Ʈ Ÿ�� �߰�(hnine)
	//#define MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

	// [���׼���] MatchDB �� �߸��� ĳ���� ������ ������Ʈ �Ǵ� ���� ���� (apple_hnine)
	#define MODIFY_MATCHDB_STRANGE_UPDATING_20060822

	// [���׼���] ������ ĳ���Ͱ� MatchDB�� ������Ʈ �Ǵ� ���� ���� (hnine)
	#define MODIFY_MATCHDB_UPDATE_DELETED_CHARACTER_20060810

	// [���׼���] ĳ���ͼ��� â���� �̵� �� MatchDB ������Ʈ �ȵǴ� ���� ���� (apple_hnine)
	#define MODIFY_UPDATE_MATCHDB_AT_CHARACTERLIST_VIEW_20060725

	// MuLog ��� �� ����
	#define LOGINSERVER_MULOG

	// �Ϻ� ��û���� ����ĳ�� ��ŷ����� �ѱ������� ���� ��
//	#define BLOODCASTLE_EVENT_3RD_20040401				// ����ĳ�� 3�� �̺�Ʈ (ĳ�� �����ð� ����) (b4nfter)
//	#define BLOODCASTLE_EVENT_4TH_20040531				// ����ĳ�� 4�� �̺�Ʈ (ĳ�� �����ð� ����) (b4nfter)

//	#define JAPAN_CHAOSMIX_BUG_20040701					// �Ϻ� ī�������� ���׼���	-> �ѱ� �̹� ���� ��

	#define JAPAN_DESTROYKING_EVENT_20040625			// �Ϻ� �ı��� �̺�Ʈ

//	#define JAPAN_NPC_NEWTALK							// �Ϻ� NPC���ο� ����..
	#define MODIFY_JAPAN_MONSTER_VALUE					// �Ϻ� ���� ��ġ ����

	#define JAPAN_ROCFEATHER_CBMIX_BLOCK_20040303		// �Ϻ� �Ϲ� ī���� ���� �� ��ũ�� ������ ���� ���� �ʵǵ���

	#define JAPAN_1ST_ANIVERSARY_BOX_20040531			// �Ϻ� 1�ֳ� ��� ���� ����

	#define EVENT_RENA_20040909
	#define EVENT_LOTTO 
#endif


#ifdef FOR_CHINA						// �߱���

	#define MODIFY_CONNNECT_HACK_ATTACK_20090409
	#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		#define TEMPLOG_PRINT		0	// 1 �̸� �α� ������ ���� ����,��� �Ѵ�.
	#endif

	// [���׼���] �߱� ���� ���� ���� ��ŷ ���� (hnine)
	#define MODIFY_DENY_USER_DISCONNNECT_HACK_20080323

	// [���׼���] �߱� �Ƿε� �ý��� �����ð� ���� ���� (hnine)
	#define MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_02_20080218

	// [������Ʈ] 3������ ���� Ȯ�� ���� - �߱� ���� (hnine)
	#define MODIFY_THIRD_WING_MIX_RATE_20080116

	// �߱� PACKET_POSITION �������� �ڵ� ���� (hnine)
	#define MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126

	// [������Ʈ] �߱� �Ƿε� �ý��� Ȯ�� (hnine)
	#define MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509

	// �ؿܽ���1�� ��ġ : �߱� �Ƿε� �ý��� �޽��� ����� g_bChnPlayTimeLimitOn �ɼǿ� ���� On/Off �Ѵ�
	#define MODIFY_PLAYTIME_LIMIT_MESSAGE_ONOFF_20060427

	// �ؿܽ���1�� ��ġ : �߱����� ���ý� �����긦 �Ѵ�.
	#define MUTEX_REMOVE

	#define	CHN_PLAYTIME_LIMIT_SYSTEM_20050818			// �߱� �Ƿε� �ý��� ���� (b4nfter)
	#define MODIFY_HACKLOG_OF_ATTACK_20050714			// ���� �� �α� ���� (b4nfter)
	#define MODIFY_HACKLOG_OF_ATTACK_20050518           // ���� �� �α� ���� (u4us)
//	#define MU_CHN_HACKLOG3_MODIFY_20041117             // ���� �� �α� ���� (foguy)
//	#define MU_CHN_HACKLOG_MODIFY_20040930				// �ٷα� ���� ����
	#define MU_CHN_OFFLINE_GIFT_EVENT_20040908			// �������� ��ǰ �̺�Ʈ ��� (�� ���̸� ��ǰ ����)
	#define MU_CHN_THANKS_COOKIE_EVENT_20040908			// �߱� ������ ���� �̺�Ʈ
	#define MU_CHN_THANKS_RING_EVENT_20040908			// �߱� ������ ������ ���� �̺�Ʈ	
	#define SMS_BLOCK									// SMS Block
	#define CHINA_BUG_RETOUCH
	#define CHINA_GM_EVENT
	#define CHECK_ATTACK_AREA
	#define PACKET_CHANGE_CHINA_V3
	#define CHINA_GUILD_POINTER_REF_ERROR
	#define BLOOD_DEVILSQUARE_TRADE_BUG_FIX
	#define CHAOS_MIX_TRANSACTION_BUXFIX_20040610

	#define EVENT_RENA_20040909
#endif

#ifdef FOR_TAIWAN
	//������ ���� ���� ����(riverstyx)
	//#define MODIFY_RECONNECTSHOPSERVER_20080807

	// �븸 GM ��� ���� �̺�Ʈ �߰� (hnine)
	#define EVENT_SERVER_OPEN_COMMEMORATION_20050510

	// �븸 GM ��� ���� �̺�Ʈ�� ���� GM��ɾ� �߰�
	#define MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef _FREE
	// [������Ʈ] �븸 ���Ἥ�� ���� (hnine)
	#define MODIFY_TAIWAN_FREE_SERVER_SETTING_20080214
#endif // _FREE

	// �븸 PACKET_POSITION �������� �ڵ� ���� (hnine)
	#define MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126

	// 20051206 �븸 PC�� �̺�Ʈ �۾� (apple)
	// defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN) <- �̰͵� �������
	// ADD_PROTOCOL_PCBANG_GUID ���� �Ǹ� �ȵǱ� ������ ����!

	// �ؿܽ���1�� ��ġ : �븸 ���� PC�� ���� ����
	#define PCBANG_ADVANTAGE
	// �ؿܽ���1�� ��ġ : �븸�� ���ؽ� ������ �ɼ��� ���ش�.
	#define MUTEX_REMOVE
#endif

#ifdef FOR_THAILAND
	// �޴� �̺�Ʈ �� �������� �� ���� (b4nfter)
	#define MODIFY_MEDAL_EVENT_ZEN_20050829
	// �ʼ����̵� �� �������� ���� ���� (b4nfter)
	#define MAP_SERVER_ADDINFO_ADULT_20050610
	// �ų� �̺�Ʈ �ݼ� �׸� ������ ��ӽ� 2548 �� ��� (u4us)
//	#define EVENT_SONGKRAN_2005_20050316                           
	// ���� �̺�Ʈ ���� ������ ��� ó�� ���� (u4us)
	#define MODIFY_DROP_ITEM_OF_FIRE_CRACKER_EVENT_20050316        
	// ���� ���� �������� ���� ���ʽ� ����ġ �ο� (u4us)
	#define BONUS_EXPERIENCE_POINT_20050303       

	#define THA_RENA_EVENT_20040909
#endif // FOR_THAILAND

#ifndef FOR_THAILAND		
	// ���������� �ο� ���� 
	#define DEC_MAX_DEVILSQUARE_OBJCOUNT	// �±��� ���������� �����ο��� 20���̴�.
#endif

#ifdef FOR_VIETNAM

	// �ɼ� ���ε� �� CS�� ���� �������� ���� ����(musang33)
	#define MODIFY_FAIL_CONNECT_CS_AFTER_OPTIONRELOAD_20091008

	// ��ȣ�� ���� ���� �� ������ ���� ���� �� �׿��� ���� ����Ʈ ���� �ȵǴ� ���� ����(musang33)
	#define MODIFY_DUAL_AFFECT_BUFFTYPE_CHARM_GUARDIAN_20091007

	// ������ ��ȣ���� ����ÿ��� �渰�� ��ų������ ������ ������ ���� �Ǵ� ���� ����(musang33)
	#define MODIFY_NOT_EFFECT_FENRIRSKILL_IF_CHARM_PROTECTITEM_20090924

	// ������ �������� ���� �ǸŰ��� ���� �ȵǴ� ���� ����(musang33)
	#define MODIFY_SELLITEM_WITHOUT_DURABILITY_20090914

	// ��Ű�罺 ��, ĭ���� ���������� �����, �Ҵ� ���Ź��� ���� �Ǵ� ���� ����(musang33)
	#define MODIFY_WEARING_PENDA_SNOWMAN_IN_ICARUS_AND_CANTUR_20090914

	// �������պ��� ����� 1�� ���� ���� �� ��ũ�� 3�� ���� �����Ǵ� ���� ����(musang33)
	#define MODIFY_WINGCHAM_DARKLOAD3RD_WING_20090914

	// �� �̵��� ĳ���Ͱ� �и��� ���� ����(pelero)
	#define MODIFY_MOVE_MAP_MOON_WALKING_BUGFIX_20090908

	// �Ƿε� �ý��� (apple)		-- ���� �ҽ� �߰�
	#define VTM_PLAYTIME_LIMIT_SYSTEM_20060626

	// �޼��� ���۽� ���ڿ� ���� ��� ���� ���� (RKFKA)		-- ���� �ҽ� �߰�
	//#define MODIFY_VIETNAM_MEMCPY_BUG_20060502
	// ** ��Ʈ�� ������ �� ���Ӱ��� 2.0���� ����� ������Ʈ�� �߰��� ���·� �������� �ؾ� ��.
	// �߰� ���� : ggsrvdll.lib, ggsrvlib.lib

	// ��Ʈ�� ���Ͻ� �� ���� ��� (apple)

	// message.wtf �� ��ɾ� ó�� � �����ڵ�ȭ�� ����ȴ�.
	//#define	UNICODE_MODE_20050718
	// �ڵ������� - ��Ʈ�� (1258)
	//#define	DEFAULT_CODEPAGE			1258
	// Ŭ���̾�Ʈ ���� ������ ���̺귯�� 2 ��� (u4us)
	//#define NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411     
#endif

#ifdef FOR_GLOBAL
	// �۷ι� �ٷα� ��� Ƚ�� ����� �ѹ� (u4us)
	#define MODIFY_LOG_OF_HACK_20050607
#endif

//#ifdef USA
#if GAME_VERSION >= G_V_S4_6
	// �̱� ���� �׽�Ʈ�� ���� �����ͷε� �˻�.
//	#define	TEST_LOADING_STEP_20060211
			
	// 1) ���� ���� �� ���� ĸ(���� 60 ����) ������ ���� �۾�
	// 2) ���� ������ �����ϴ� ����, ĳ����, ���� ���� ���̺��� ���Ἲ�� ���� �۾�
	// 3) ĳ���� ���� �� ���������� �˷� �ش� ���̺��� �������ش�. (apple)

	// [������Ʈ] ĳ���� ����Ʈ �ߺ���û����(goni)
	#define UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

	// [���׼���] �߱� ���� ���� ���� 
	#define MODIFY_CONNNECT_HACK_ATTACK_20090409
	#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		#define TEMPLOG_PRINT		0	// 1 �̸� �α� ������ ���� ����,��� �Ѵ�.
	#endif

	#define NOTIFY_CHARACTER_DELETE_FROM_GS_TO_JS_20060322

	// MuLog ��� �� ����
	#define LOGINSERVER_MULOG

	// �н����� ���� 20����Ʈ�� ����
	#define PW_KUZIMO
#ifdef GLOBAL_S4_6
	#define UPDATE_USERPASSWORD_LENGTH_20090424
#endif

	// [������Ʈ] ���պ��� �߰�(goni)
	#define UPDATE_UNIFICATIONBILL_20090203

	//#define MODIFY_HACKLOG_OF_ATTACK_20050714			// ���� �� �α� ���� (b4nfter)
	//#define MODIFY_HACKLOG_OF_ATTACK_20050518           // ���� �� �α� ���� (u4us)
	//#define CHECK_ATTACK_AREA

	// [������Ʈ] ȭ��Ʈ���� ������ ���� �̺�Ʈ (RKFKA)
	#define WHITEANGEL_GET_ITEM_EVENT_20060822

#endif

#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	#define ADD_ITEM_OF_FRIENDSHIP_EVENT_20040729		           
#endif

///////////////////////////////////////////////////////////////////////////////
// �̺�Ʈ ���� ���� �ڵ�
//
// ����: �ΰ� �̻��� �̺�Ʈ ����� ���� �߻� Ȯ�� ���
///////////////////////////////////////////////////////////////////////////////

// EVENT_SERVER_OPEN_COMMEMORATION_20050510     // ���� ���� ��� �̺�Ʈ (u4us)
// EVENT_RENA_20040909                          // ���� �̺�Ʈ (u4us)
// EVENT_LOTTO                                  // ���� �̺�Ʈ (u4us)    
                                              
// EVENT_STONE_OF_FRIENDSHIP_20040206                       
// EVENT_RENA_FOR_CASTLE_SIEGE_20041116                     
// EVENT_FRIENDSHIP_20040204


///////////////////////////////////////////////////////////////////////////////

#if GAME_VERSION >= G_V_99B
#define DELETE_DARKLORD_PET_SERIAL_ZERO_20050617		// �Ϻ� ������ġ
#define DARKLORD_PET_INFO_FORCE_TO_SET_20050531			// �������� ��ũ�ε� ���� �� ��� �⺻������ ������ ����
#endif

#define APPLY_TESTSERVER					// �׽�Ʈ �������� ������ �Ǵ� ���� ���� ���ΰ͵�

// �׽�Ʈ ������ ����Ǵ� �͵��� ���⿡ �߰�!!!!!
#ifdef APPLY_TESTSERVER

#if TESTSERVER == 1
	// [������Ʈ] 3�� ������ �Ͽ� ������ ������ ��츸 MasterLevelSystem ������ �ε��ϰ� �����ϵ��� ���� 2008-12-12 by riverstyx
	// ���� �����ͷ����� �ƴ������� MasterLevelSystem�� ������ �����ϰ� �ε��ϴ� ���ʿ��� ������ �����
	//#define MODIFY_3RDCHANGEUP_MASTERLEVEL
#endif

#ifndef FOR_KOREA
	// EventItembag ��ȣ ��ġ�� ����(pelero)
	#define MODIFY_FIRECRACKER_EVENTITEMBAG_NUMBER_CHANGE_20090930

	// �ؿ� �κ�����ȭ 7�� ������Ʈ (pelero)	
	#define ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
		// ����Ʈ SD ȸ�� ���� �߰�(pelero)
		#define ADD_ELITE_SD_POTION_20090918
		// ��Ƽ�� ����ġ ���� ��ũ�� �߰�(pelero)
		#define ADD_PARTY_EXP_INCREASE_SCROLL_20090916
		// ���� ������ �߰�(pelero)
		#define ADD_AURA_CASH_ITEM_20090915
		// ������ �� �߰�(pelero)
		#define ADD_UNICORN_PET_20090915
		// ġ�� ���� ���� �߰�(pelero)
		#define ADD_CRITICAL_WIZARD_RING_20090915
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_07_20090915

	// ũ������ â�� �� ���� ����(pelero)
	#define MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904

	// ĳ�� �������� ���ݷ�, ����, ����, ������ ���������ִ� ������ ����
	// ������ ����Ʈ�� ��������Ʈ �ߺ� ����Ǵ� ���� (musang33)
	#define MODIFY_CASHITEM_EFFECT_REITERATION_APPLY_20090903

	// �����ÿ� ������Ʈ ��񿡵� ��� �ɼ��� ���� �� �ֵ��� ����(pelero)
	#define MODIFY_GAMBLING_ATTACH_LUCK_OPTION_20090901

	// ȯ�� ��� ���� �� �� �̵��� ������ ���� ����(pelero)
	#define MODIFY_ILLUSIONTEMPLE_MOVEGATE_ITEM_DELETE_BUGFIX_20090826

	// �����忡�� ��ũ���Ǹ��� �������ؾ� ���Ǵ� ����(pelero)
	#define MODIFY_PKFIELD_DARK_SPRIT_BUGFIX_20090804

	// ���� �̺�Ʈ ���� NPC�� ���(pelero)
	#define MODIFY_CHEREEY_NPC_REGISTER_NPC_GROUP_20090803

	// ��ȣ ������ ��ȣ ������ ���� �ߺ����� ������� ���ϵ��� ó��(pelero)
	#define MODIFY_CHARM_GUARDIAN_CHARM_PROTECTITEM_NON_DUPLICATION_20090720

	// ������ ���� ���� ī��Ʈ �߿� �Ǵٽ� ���� ���� ��û�� ī��Ʈ�� �ʱ�ȭ �Ǵ� ����(pelero)
	#define MODIFY_NEWPVP_ENDING_COUNTDOWN_BUGFIX_20090715

	// ��ī�������� ������� �������� ������ �켱 ���� �ǵ��� ����(pelero)
	#define MODIFY_PCS_MARKS_GLORYOFGLADIATOR_SEQUENCE_20090715

	// ������ ����ÿ� �ʱ�ȭ ����(pelero)
	#define MODIFY_NEWPVP_GAME_CLOSING_BUGFIX_20070715

	// ����� ���� ����, �Ҵ� ���� ���� ���� ����(pelero)
	#define MODIFY_SNOW_MAN_PANDA_POLYMORPH_RING_BUGFIX_20090715

	// ���� �Ҳɳ��� �̺�Ʈ(pelero)
	#define ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

	// Ȳ�� ���� �̺�Ʈ ������ 20090311 (natinda)
	//#define ADD_GOLDEN_EVENT_RENEWAL_20090311

	// ���&���� �����۹� ������ �۾� by natinda
	//#define MODIFY_KUNDUN_EVENTBAG_RENEWAL_2009_02_04
#endif // FOR_KOREA

	// [���� ����] PathFind ���׼���(goni)
	#define MODIFY_PATHFIND_BUGFIX_20090609
#if GAME_VERSION >= G_V_S4_6
    // [���� ����] ī���� ĳ�� ���� ���� ���� by natinda
	#define MODIFY_CHAOSCASTLE_REWARD_20090608     
#endif

#if defined( FOR_KOREA ) && GAME_VERSION >= G_V_S4_6// ������ ���� ������ ����
    // [���� ����] �ڵ� ������ ���� ���� ����
	#define MODIFY_AUTOREPAIR_CALC_20090608
#endif // FOR_KOREA
 
    // [���� ����] ��ü ������� ���� ���� (natinda)
#if GAME_VERSION >= G_V_S4_6
	#define MODIFY_CALREPAIRRATE_20090528
#endif
  
#if GAME_VERSION >= G_V_S4_6
	// [������Ʈ] EventItemBag Ȯ��_addoption  ���� ���� �ɼ��� �־���.(goni)
	#define UPDATE_PROBABILITYITEMBAG_ADDOPTION_VALUE_20090526

	// �����̾������� �߰�(�Ҵ����Ź���, �Ҵ���) 2009-04-30 by riverstyx
	// #define PREMIUMITEM_20090106 Ȱ��ȭ �Ǿ��־�� ��
	#define ADD_PREMIUMITEM_PANDA

	// [���� ����] ���� ��ų�� ��ȭ�� �� ���⸦ ���� �����ϸ�
	// ��ųƮ���� ������������ ǥ�� �Ǵ� ����(pelero)
	#define MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_2ND_20090514

	// [��������] �̺�Ʈ�� ���Խ� ��ų ������ ���쵵�� ����(pelero)
	#define ADD_EVENT_MAP_BUFF_EFFECT_DELETE_20090421
#endif

	// [���׼���] ���Ǵ�Ƽ ���ο� ������ +2ȭ���� �������� ����(pelero)
	#define MODIFY_INFINITY_ARROW_USE_MANA_BUGFIX_20090416

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ���� ȯ����� �����(�η縶��/���ǵη縶��)�� �������� 0�Ǿ ���� �ȵǴ� ���� ����
	// ������ �߻����� TW_0074(riverstyx) - ������ �Űܿ� 20090416
	#define MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

	// [���׼���] ȯ�� ������� ����Ʈ�� ��ũ ���� ����(pelero)
	#define MODIFY_LIGHTNINGSHOCK_ILLUSIONTEMPLE_BUGFIX_20090414

	// [���׼���] ����� ���� ���� ���â���� â��� ������ ���� ��Ǯ���� ����(pelero)
	#define MODIFY_SNOW_MAN_POLYMORPH_RING_BUGFIX_20090414
#endif

	// [���׼���] ������ ������ ������뺸�� �������� ���ƾ� �����Ǵ� ����(pelero)
	// �������� ������� ���� ���� �����ǵ��� ����
	#define	MODIFY_ITEM_REPAIR_HIRE_BUGFIX_20090415

	// [���׼���] â���� ������ ã�� �� ���Ẹ�� �������� ���ƾ� ã������ ����(pelero)
	// �������� ����� ���� ���� ã�������� ����
	#define	MODIFY_WAREHOUSE_HIRE_BUGFIX_20090413

#if GAME_VERSION >= G_V_S4_6
	// [���׼���] �Ǿ������ �κ�����ȭ ���� ó�� ���� ���� 2008-04-08 by riverstyx
	// ���������� ���еǾ� ó���Ǵ� �Ǿ���, �ؿ� �κ� ����ȭ ������ ���� ó�� ������ �ϳ��� ��ģ��.
	// CheckItemOptForGetExpEx <- (CheckItemOptForGetExp, CPCBangPointSystem::CheckItemOptForGetExp, CNewPVP::CheckItemOptForGetExp) ����
	// ����ȭ�Ǹ� �Ʒ� �������� ������ ����
	// ���� ���� ó�� �������� ����
	// MODIFY_MARKITEM_20080506 - ����
	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404 - ����
	// MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318 - ���� ó�� �������� ����
	// BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205 - ���� ó�� �������� ����
	#define MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#endif


	// ���
	//#define MODIFY_LUA_SCRIPT_20090430

	// [���׼���] ������ �� ������ ��������ε��� PKī��Ʈ�� �ö󰡴� ���� ���� 2009-04-30 by riverstyx
	#define MODIFY_CASTLESIEGE_STATENOTIFY_JOINGUILD_BUGFIX

	// [������Ʈ] ��ŷ �� ��� ����� ��ü ������ �����ϵ��� ������������ ���� ��� �߰� (hnine)
	#define MODIFY_HACKTOOL_BLOCK_ALL_SESSION_20090422
#if GAME_VERSION >= G_V_S3_E1
	// [���׼���] 3������ ���հ��� ���׼���(goni)
	#define MODIFY_THIRD_WING_EXITEMMONEY_BUGFIX_20090423
#endif
	// [���׼���] Movereq.txt ������ �ִ� �� �ε��� ���� ��� 40 -> 50 (hnine)
	#define MODIFY_MOVEREQ_MAP_INDEX_LITMIT_20090421

#ifdef FOR_KOREA	// ����� ������ ����
	// [������Ʈ] ���̵��� ä�� �޽����� �ƴ� �� �̵� ��û �������ݷ� ���� (hnine)
#if GAME_VERSION >= G_V_S4_6
	//#define ADD_MAPMOVE_PROTOCOL_20090327 //TEST MOVE
#endif
	#define USE_MAPMOVE_KEY_GENERATOR_SERVER_SIDE
#endif	// FOR_KOREA

	// [������Ʈ] ��ŷ �� ��� ����� ���� �������� ����ǵ��� �ӽ� ���� (hnine)
	#define MODIFY_HACKING_TOOL_BLOCK_SINGLE_SERVER_20090325

	// [���׼���] �巡�� ���� �̺�Ʈ ������ ���� �༮�� ������ �ǸŽ� 4���� �Ѵ� �ݾ����� �ȸ��� ����. by natinda 20090410
	#define MODIFY_JEWELOFBLESS_SELL_BUGFIX
	
	// [������Ʈ] ���պ��� �߰�(goni)
	#define UPDATE_UNIFICATIONBILL_20090203

	// [���׼���] �Ŀ� ������ �Ÿ� ���� ���� �׽�Ʈ �� ����(grooving)
	#define MLS_POWERSLASH_DISTANCE_20090216
#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] ���̷κ� �� ��ŷ �� ��� ��� �߰� (hnine)
	#define ADD_HACKING_TOOL_BLOCK_20090311

	// [���׼���] ũ�ν� �ǵ� ������Ʈ �ɼ� ����(pelero)
	#define MODIFY_CROSS_SHIELD_EX_OPTION_APPLY_20090326
#endif

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ���ɰ��� �������� �Ǹ� ���ݿ� ���� ����(pelero)
	#define MODIFY_CHERRYBLOSSOM_SELL_MONEY_BUGFIX_20090325
#endif

	// [���׼���] ĳ���� ����Ʈ�� �������(��ϵ��� �ʰų� ���������� ���� ������ ���޾������) empty packet�� ������ �ʵ��� �Ѵ�. by natinda 20090218
	#define MODIFY_BUGFIX_CASHSHOP_OPEN_INTERFACE

	// [������Ʈ] �̺�Ʈ ������ �߱� �α׿� �߱޵� ������ ���� �߰� 2009-03-18 by riverstyx
	#define ADD_COUPON_EVENTITEM_LOG

	// [���׼���] Teleport �� Ÿ�� ��ǥ Y�� OBJECTSTRUCT.TX �� �Է��ϴ� ���׼��� 2009-03-12 by riverstyx
	#define MODIFY_TELEPORT_TARGETPOSITION_BUGFIX

	// [���׼���] ī����ī�� ���� - ���λ��� ���½� ī���� ���� �Ұ� 2009-03-12 by riverstyx
	#define MODIFY_PCS_CHAOSMIX_PSHOP_BUGFIX

#if GAME_VERSION >= G_V_S4_6
	// [������Ʈ] ��ī����, PK Field 2009-01-08 by riverstyx
	#define	ADD_NEWPVP_PKFIELD
	
	// [������Ʈ] �׺� �ý��� ������Ʈ(pelero)
	#define ADD_GAMBLING_20090120
#endif

	#ifdef ADD_NEWPVP_PKFIELD
		// PKField �۾��߿� ��Ʈ Life ���� �߰� �켱 PKField �ݿ��ÿ� ������ �����̴�.
		// [���׼���] Life �ִ� �� ���ҽ� ���� ���ҵ� �� ����.������ ȿ�� ���뿡�� �и� 2009-02-02 by riverstyx
		// ��������
		//#define MODIFY_LIFE_CHANGEMAXVALUE

		// [���׼���] Life, Shield ������� 100%(Life > MaxLife + AddLife) �Ѿ�� ���׼���(NewPVP������ ���) 2009-04-17 by riverstyx
		#define MODIFY_LIFE_CHANGEMAXVALUE_BUGFIX
		// [������Ʈ] ���ɻ��� ��ī���������� ����ǵ��� ����(pelero)
		#define MODIFY_CHERRYBLOSSOM_DROP_VULCANUS_20090324
	#endif // ADD_NEWPVP_PKFIELD

	#ifdef ADD_GAMBLING_20090120
		// [������Ʈ] �׺� �ý��� ����
		#define MODIFY_GAMBLING_COMPLEMENT_20090513
		// [������Ʈ] �׺� ���࿩�� ��ũ��Ʈ ����
		#define ADD_IS_ENABLED_GAMBLING_20090504
		// [���׼���] �׺� �ý��� 1�� ���� ����(pelero)
		#define MODIFY_GAMBLING_BUGFIX_1_20090323
		// [������Ʈ] ��Ʈ����Ŀ ���� ó�� �߰�(pelero)
		#define ADD_STRIKER_SCEPTER_20090317
		// [������Ʈ] ������� �߰�(pelero)
		#define ADD_AERINBOW_20090309
	#endif // ADD_GAMBLING_20090120

#if GAME_VERSION >= G_V_S2_2
	// ���� �����̾� ������(ī����ī��) �߰�
	#define ADD_PCS_LOTTERY_SYSTEM_20070117
	#define MODIFY_PCS_CHAOSMIX_REFACTORING_20070903
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [���׼���] �����ͷ��� ����ġ ������ ���������� �о���� ����(pelero)
	#define MODIFY_MASTER_LEVEL_ADD_EXPERIENCE_BUGFIX_20090304
#endif

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] �Ǿ��� �̿��� ����ġ(Rate) ���� 2009-02-19 by riverstyx
	#define UPDATE_PCBANGPOINTSYSTEM_BUGFIX
#endif

#if GAME_VERSION >= G_V_S4_6
	// [������Ʈ] ��������峪 ���������ձ������ �Ǵ�(goni)
	#define UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
#endif

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �Ǿ��� �̿��� ����ġ(Rate) ���� 2009-02-16 by riverstyx
	#define ADD_PCBANG_EXPRATE
#endif

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ������� ���� ī���� ���� ���� ���׼���(goni)
	#define MODIFY_CHAOS_ITEM_COPY_BUGFIX_20090211

	// [���׼���] �������� ���� ���ε� �ɼ� ��� ����� �������� �ٺ��� �� ä�� ����(pelero)
	#define MODIFY_SELUPAN_REGEN_MONSTER_RELOADING_BUGFIX_20090128
#endif

	// [������Ʈ] GM ��� ��ȭ 2008-12-16 by riverstyx
	#define GMCOMMAND_EX
	#if defined( GMCOMMAND_EX ) && GAME_VERSION >= G_V_S4_5
		#define GMCOMMAND_EX_1ST_BUGFIX
	#endif // GMCOMMAND_EX

#if GAME_VERSION >= G_V_99B
	// [������Ʈ] GM ��� ��ȭ 2009-01-14 by riverstyx
	#define MODIFY_SETITEM_LEADERSHIP_BUGFIX
#endif

	// [������Ʈ] ��ũ�ε庸��_ī��ƽ���̾� ����(goni)
	#define UPDATE_CHAOTICDESAIR_SKILL_20090109

#if GAME_VERSION >= G_V_99B
	// [������Ʈ] ��ũ�ε� ����(��ũ���Ǹ�, ��ũȣ�� �κ���������)(goni)
	#define UPDATE_DARKROAD_PET_ITEMREPAIR_20090109
#endif

#if GAME_VERSION >= G_V_S4_6
	// [������Ʈ] ���� �����̾� �������Ǹ�(grooving)
	#define PREMIUMITEM_20090106
#endif 

	#ifdef PREMIUMITEM_20090106
		#define EVENT_LOTTO
		// [������Ʈ] ���� �̺�Ʈ ������ ����Ʈ(grooving)
		#define COUPON_EVENT_ITEMLIST_20081022
		// [������Ʈ] ī���� ����. ī�������� ���н� �Ҹ�Ǵ� ������ ��ȣ(grooving)
		#define	ADD_CHAOSMIXCHARM_ITEM_20080702
	#endif

#if GAME_VERSION >= G_V_S4_5
	// �������� ������
	// [������Ʈ] ��ȣ ������. ���� ��ȣõ�� ������ ���׷��̵� ����. //grooving
	#define	ADD_SAFEGUARD_ITEM_20080702

	// ��ƾ�� ����ƽ����� �ٺ��� ���·� �����Ǵ� ���� ����(pelero)
	#define MODIFY_REGEN_SELUPAN_SUMMON_MONSTER_IN_DEVIAS_BUGFIX_20081205

	// [������Ʈ] �ؿ� ũ�������� �̺�Ʈ �� �߰� (riverstyx)
	#define UPDATE_XMASEVENT_MAP_20080930
#endif

	#ifdef UPDATE_XMASEVENT_MAP_20080930
		// [������Ʈ] �ؿ� ũ��������  (grooving)
		#define ADD_XMASEVENT_PET_20080930

		// [������Ʈ] �ؿ� ũ�������� ������ ���� ���� (natinda)
		#define ADD_GIFTITEM_XMASEVENT_20081030

		// [������Ʈ] �ؿ� ũ�������� ��ŸŬ�ν� ����
		#define ADD_ITEM_ATTACK_XMASEVENT_20081014

		// [������Ʈ] �ؿ� ũ�������� �̺�Ʈ Npc �߰�
		#define ADD_NPC_XMASEVENT_20080930

		// [������Ʈ] �ؿ� ũ�������� �̺�Ʈ ���� �߰�
		#define ADD_BUFF_XMASEVENT_20080930

		// [������Ʈ] �ؿ� ũ�������� ���� ���� �߰�
		#define ADD_XMASEVENT_POLYMORPH_RING_20080930

		// [������Ʈ] �ؿ� ũ�������� ������ ���� ó�� �߰� (riverstyx)
		#define ADD_PAYITEM_20081030
		// [������Ʈ] EventDB Ȯ�� (riverstyx)
		#define EVENTDB_PROTOCOLEX

		// [���׼���] 15���� �̸� ����� ��Ÿ�� ���� ���� �������� ���� 2008-12-17 by riverstyx
		#define MODIFY_SANTACLAUSVILLAGE_VISITLEVEL

		// [���׼���] ��Ÿ�� ��ȭ ���µ� +30 ����(��ȯ����-���濡 ���ַ��߰�) �ǵ��� ���� 2008-12-17 by riverstyx
		#define MDOFIY_BUFF_POWER_UP

		// [������Ʈ] ��Ÿ�� ���� NPC ���� ���� ���Ϸ� ���� 2008-12-17 by riverstyx
		#define ADD_XMASEVENT_LUCKNUMBER

		// [���׼���] ��Ÿ�� ���� �ʴ����� �����Ǵ� ���� ���� 2008-12-18 by riverstyx
		#define MODIFY_SANTACLAUSVILLAGE_INVITATION

		// [���׼���] �絹����, ����� ���Ź��� ��ü���� ���� 2008-12-19 by riverstyx
		#define MODIFY_XMASEVENTITEM_REPAIR
	#endif // UPDATE_XMASEVENT_MAP_20080930

	// [���׼���] �������׼���(goni)
	#define MODIFY_BUGFIX_20081119

	//[���׼���] ������� ���������� ���� �������Ե� �������� ����Ǵ� ����(pelero)
	#define MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121

#if GAME_VERSION >= G_V_S4_6
	// [���׼���] ���� ������ ��ũ���ðŰ� ���˹�Ʈ�ν����캸�� �������� ���� ����(pelero)
	#define MODIFY_DARKSTINGER_BUGFIX_20081119

	// [���׼���] ���ǵ� �� �ӽ� ó�� (riverstyx)
	#define MODIFY_SPEEDHACK_20080605
#endif

#if GAME_VERSION >= G_V_99B
	// [���׼���] �̺�Ʈ�ʿ��� �̺�Ʈ�� ����Ǹ� �����̵� ��ų ��� ���ϰ�..(pelero)
	#define MODIFY_DONT_TELEPORT_EVNET_ENDING_IN_EVENTMAP_20081118
#endif

	// [������Ʈ] ���� +3 ȭ�� �߰�(pelero)
	#define UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �ż� �߰��� ����� ������ ���� �����ϰ� ���� �߰� 2008-11-13 by riverstyx
	#define ADD_NEWSERVER_20081113
#endif

	#ifdef ADD_NEWSERVER_20081113
		// [������Ʈ] �ż� ī����ĳ�� ���� ��� �� ��ӵǴ� ���� ���� �߰� 2008-11-13 by riverstyx
#ifndef MODIFY_CHAOSCASTLE_REWARD_20090608
		#define ADD_NEWSERVER_CHAOSCASTLE_MONSTERITEMS_20081113
#endif
		// [������Ʈ] �ż� �̺�Ʈ Off
		#define ADD_NEWSERVER_EVENTOFF_20081114
	#endif // ADD_NEWSERVER_20081113

#if GAME_VERSION >= G_V_S4_6
	// [������Ʈ] ���ȭ 7�ֳ� �̺�Ʈ(����� ����)(goni)
	#define UPDATE_LUCKY_COIN_EVENT_20081029
#endif

	// �κ����� �����Ұ�� ������ �����Ҷ� ���� 3���� �ΰ�(goni)
	#define UPDATE_INVEN_REPAIREMONEY_INC_20081112

#if GAME_VERSION >= G_V_S4_5
    //[���׼���] ������ ������ ��ȭ�� ������ �Ÿ� ��ų�� ����� ��� ��Ƽ������ �ٷ� �Ա��� ƨ��� ���� 2008-11-07 by natinda
	#define MODIFY_RECALLSKIL_BUGFIX_20081107
#endif

	// [��ŷ] NPC�� �������� ���������� ��û ����(�ŷ�, ģ���߰�, ����߰�, ��Ƽ�ʴ�) - 2008-11-06 by riverstyx
	#define MODIFY_REQUESTNPC_HACK

	// [���׼���] �������� ���׼��� - riverstyx
	// ������ �ŷ����¿��� ����������(����) ��û �� �ŷ� ��Ҹ� ��û�Ͽ� ����
	#define MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

	// [���׼���] ���̵��ý� �̵� ��ġ ���׼���(goni)
	#define MODIFY_MAPMOVE_POSITION_SYNC_BUGFIX_200981024

#if GAME_VERSION >= G_V_S4_5
    // [��ġ����] ����� ������ ������ ������ ����(natinda)
	#define MODIFY_DEMON_AND_SPRITE_DURABILITY_20081023
#endif

	// [���׼���] ���̾ũ�� ��ȭ�� ���÷��� �������� ���� �ȵǴ� ����(pelero)
	#define MODIFY_STRENGTHEN_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081017

	// [���׼���] �������� �� ī�����ڽ� ������ �ȳ������� ���׼���(goni)
	#define MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_200801021

	// [���׼���] ������ �һ����� ��ȭ ���� ����� ü�� ����Ʈ�� �������� �̻������� �����Ͽ� ������ �ر� (�Ϻ�/�߱�) �������� �����ؾ� �� ������ ����..(natinda)
	#define MODIFY_CHAIN_LIGHTNING_BUGFIX_20081022

#ifdef CHAOS_MIX_UPGRADE
	// [���׼���] ������� ī���� ���ս� �κ��丮�� Ǯ�� ��� �ö� ���� ������ ������ �������� ���ϴ� ���� (natinda)
	#define MODIFY_MIXSYSTEM_BUGFIX_20081016
#endif

	// [���׼���] ���˽�ų ���Ϲ� ����(goni)
	#define MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
	
	// [���׼���] GetBuffData ���ϰ� NULL �˻�(goni)
	#define MODIFY_GETBUFFDATA_RETURNVALUE_NULLCHECK_BUGFIX_20080923

	// [������Ʈ] ȯ����� ��ŷ���� SendData ����(goni)
	#define UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925

#if GAME_VERSION >= G_V_S3_E2
	// ���̺� �׷� 4�� ���׼���
	#define ADD_LIVE_GROUP_4TH_BUGFIX_20080908
#endif

	#ifdef ADD_LIVE_GROUP_4TH_BUGFIX_20080908
		// [������Ʈ] ��ī�罺 �������� ��ũ�ε� �Ÿ� ��ų�� ����(3�� ����, ��ȯ�����, ��ũȣ��)��
		// �����ߴµ��� ��ȯ�� �ȵǴ� ����(pelero) - WOPS��ȣ:30852
		#define MODIFY_RECALL_PARTY_SKILL_SUMMONER_BUGFIX_20080829
		// [������Ʈ] ����� ���� ������ �ִ� ��Ȳ���� ���� ��������
		// ������ �Ұ����� ������ ����� ����� ���� ���� ȿ���� 2��� 
		// �����ϴ� ����(pelero) - WOPS��ȣ:30867 
		#define MODIFY_INCREASE_LIFE_AND_SETITEM_BUGFIX_20080829
		// [������Ʈ] ������ ���� ĳ������ ��� ������Ʈ �ɼ��� ���ݷ�����+����/20 �ɼ���
		// ���� ���Ŀ��� ����+�����ͷ���/20�� ������ �ȵǴ� ����(pelero) - WOPS��ȣ:30853
		#define MODIFY_MASTER_LEVEL_ATTACK20UP_BUGFIX_20080828
		// [������Ʈ] ��ũ���Ǹ����� ���ݽ� ������Ʈ������, ����������
		// ������� �ʴ� ���� (pelero) - WOPS��ȣ:28829, 26208
		//#define MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
		// [������Ʈ] �渶���� �����Ʈ ������ �����̵� ��ų�� ����Ͽ���
		// �����Ʈ ���ӽð��� �����Ǵ� ����(pelero) - WOPS��ȣ:29010
		#define MODIFY_TELEPORT_USING_HELLBURST_BUGFIX_20080822
		// [������Ʈ] ȯ��������� Į���� ������ �����Ǵ� ����(pelero) - WOPS��ȣ:27964
		#define MODIFY_ILLUSION_TEMPLE_USE_KALIMA_GATE_BUGFIX_20080821
		// [������Ʈ] ��ȯ������ ü�� ����Ʈ�� ��ų�� �ν�ƮŸ�� ������ ���Ǵ� ����(pelero) - WOPS��ȣ:22175
		#define MODIFY_CHAIN_LIGHTNING_USE_TRAP_BUGFIX_20080820
		// [������Ʈ] ����Ʈ�ذ� ���� ������ ũ�������� ���Ź����� �Բ� ������ ���
		// ����Ʈ�ذ� ���� ������ �������� �����ϴ� ����, ���� (13, 10)�� ���Ź���(0~6)��
		// ������ ���� �������� 2������ �������� �����ϰ� �ִ� ���� ����(pelero) - WOPS��ȣ:20886
		#define MODIFY_CHANGE_RING_DURABILITY_BUGFIX_20080820
	#endif // ADD_LIVE_GROUP_4TH_BUGFIX_20080908

	// [���׼���] �����ͽ�ų �Ÿ�����(goni)
	#define MODIFY_MASTER_SKILL_DISTANCE_BUGFIX_20080909

	// [���׼���] �÷��� ���ս� ���� �������� �Ϲ� ������ �������� ���� (hnine)
	#define MODIFY_PLUS_ITEM_MIX_WING_SET_NORMAL_ITEM_20080916

	// [���׼���] �õ彺�Ǿ� ���� ���Ŀ� ������ �̵� �Ұ� ���� ���� (hnine)
	#define MODIFY_SOCKET_CHAOSMIX_ITEM_MOVE_BUGFIX_20080910

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	// [���׼���] �����ͽ�ų ���� ������ ����(goni)
	#define BUGFIX_MASTERSKILL_INC_DEFENSE_BUGFIX_20080917
#endif

	// [������Ʈ] ���Ű��� ����(goni)
	#define UPDATE_FRUITPOINT_SUCCESSRATE_FIX_200809017

	// [������Ʈ] ��Ŭ���� ���������� �α����� ��� ������ ��������� �̵�(pelero)
	#define MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_2ND_20080917

	// [������Ʈ] ��Ŭ���� ������ ���� �� ���� �ٷ� �����ϵ��� ����(pelero)
	#define MODIFY_BATTLE_OF_SELUPAN_FAIL_DELETE_IMMEDIATELY_20080917

	// [������Ʈ] ��Ŭ���� ������ ���� ����Ʈ���� ���� ������ ����� �ȵǴ� ����(pelero)
	#define MODIFY_RAKLION_BATTLE_USER_DELETE_BUGFIX_20080916

	// [������Ʈ] R����Ű ������ ����� �ȵǴ� ����(pelero)
	#define MODIFY_R_HOTKEY_SAVE_BUGFIX_20080910

	// [������Ʈ] ũ���̿������� ��Ŭ�������� �̵����� �ʴ� ����(pelero)
	#define MODIFY_CRYWOLF_MOVE_RAKLION_BUGFIX_20080910

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ����4 �űԽ�ų ���� (goni)
	#define MODIFY_SEASON4_NEWSKILL_20080909
#endif

#if GAME_VERSION >= G_V_S3_E2//G_V_S3_E1
	// ���̺� �׷� 3�� ���׼���
	#define ADD_LIVE_GROUP_3TH_BUGFIX_20080908
#endif

	#ifdef ADD_LIVE_GROUP_3TH_BUGFIX_20080908
		// [������Ʈ] �˹�Ʈ�ν� ���� ���� �������� ���� �ʴ� ����(pelero) - WOPS��ȣ:28574
		#define MODIFY_ALBATROS_BOW_DURABILITY_BUGFIX_20080813
		// [������Ʈ] ������ �����ϰ� ó������ â�� �����
		// â������ �������� �ʴ� ���� ����(pelero) - WOPS��ȣ:27340
		#define MODIFY_NEW_ACCOUNT_WAREHOUSE_ZEN_BUGFIX_20080812
		// [������Ʈ] ��簡 ������ ��ų�� ��Ƽ�� ��ų(ȸ��������, ��ξ�, �г��� �ϰ�)��
		// ��� ���¿��� �޺��� ����� ��� 2��° ��ų�� 3��° ��ų�� ���Ƶ�
		// �޺��� �ߵ� �Ǵ� ���� (pelero) - WOPS��ȣ:27647
		#define MODIFY_MASTER_LEVEL_COMBO_SKILL_BUGFIX_20080805
		// [������Ʈ] �����ͷ��� ��Ƽ�� ��ų�� ������ ��ų�� ���Կ�
		// �����ϰ� �������ϸ� ���µǴ� ���� (pelero) - WOPS��ȣ:27127
		#define MODIFY_MASTER_SKILL_HOTKEY_BUGFIX_20080804
		// [������Ʈ] �������� 0�� ������ �Ҵ�Ʈ�� �����ϰ� �־ ȿ����
		// ����Ǵ� ����(������� �ʴ� ����) (pelero) - WOPS��ȣ:26919
		#define MODIFY_BATTLE_OF_NIGHTMARE_MOONSTONE_PENDANT_BUGFIX_20080731
		// [������Ʈ] ȯ�� ��������� ������������ ���� ������ �����Ǵ� ����(pelero) - WOPS��ȣ:26834
		#define MODIFY_SHADOW_PHANTOM_BUFF_IN_ILLUSION_TEMPLE_BUGFIX_20080731
		// [������Ʈ] ��ī�罺���� �渱�� �����ߴµ��� ������ �������� �ʴ� ����(pelero)
		// ��ũȣ���� ���������� ��ī�罺�� �̵����� ���ϴ� ����
		#define MODIFY_SKYLAND_FENRIR_AND_WING_BUGFIX_20080730
		// [������Ʈ]�����̷��� ��ø�� ���� �ɼ��� ������� �ʴ� ����(pelero) - WOPS��ȣ:26206
		#define MODIFY_STAFF_DECREASE_DEXTERITY_BUGFIX_20080729
		// [������Ʈ]�̵��Ұ� ��ų�� ���� ���¿��� ��ų�� ���� �и����� ����(pelero) - WOPS��ȣ:29691
		#define MODIFY_BACK_SPRING_SKILL_BUGFIX_20080716
	#endif // ADD_LIVE_GROUP_3TH_BUGFIX_20080908

	// [���׼���] �÷��� ������ ���տ��� ���� �������� 380���������� ����Ǵ� ���׸� ���� (hnine)
	#define MODIFY_PLUSITEMMIX_SOCKET_ITEM_DIVISION_20080905

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	// [������Ʈ] ���ݽ� ���������̸� ������ �������� �ʰ� ��������� �̵���Ų��(pelero)
	// ��Ŭ���� �̵� ���� �����Ͽ� ������
	#define MODIFY_SAFEZONE_CHECK_ATTACK_20080903
#endif

	// [������Ʈ] ��� ī�������� ������ ���� ����Ȯ�� ����(goni)
	#define UPDATE_CHAOSMIX_LEVELUP_RATE_20080904

	// ��ģ��Ŷ ó��(kuzimo)
	#define HACKTOOL_KUZIMO

	// [���׼���] ü�ζ���Ʈ�� ���׼���(goni)
	#define MODIFY_CHAINLIGHTNING_BUGFIX_20080902

	// [���׼���] ���� ��ġ�� �۾�����(goni)
	#define MODIFY_MONSTER_ATTACK_BUGFIX_20080902

	// [ ���׼���] ���Ǿ� �巴 ���׼���(goni)
	//#define MODIFY_SPEAR_DROP_BUGFIX_20080901
#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] ����4 �߰� �ü��뷱��(goni)
	#define UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

	// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ����� ��Ŭ���� ������ ���� (hnine)
	#define MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825

	// [������Ʈ] ���� 4 ������Ʈ - �α��߰� 2�� (hnine)
	#define ADD_SOCKET_OPTION_LOG_02_20080821		

	// [������Ʈ] ���� 4 ������Ʈ - �ű� ���� �ɼ� �߰� (hnine)
	#define MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

	// [������Ʈ] ���� 4 ������Ʈ - �������� �ǸŵǴ� ���� �����ۿ� ���� ���� ���� �߰� (hnine)
	#define MODIFY_SEASON4_SHOP_SOCKET_ITEM_MAKE_SLOT_20080808
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	// [������Ʈ] �����ͽ�ų �߰� �۾�(goni)
	#define UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
#endif

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �̺�Ʈ�� ����� ������� ���� ����
	#define UPDATE_EVENT_MAP_MIXiTEM_VALUE_200800808
#endif

	// [������Ʈ] �� ����� ���(goni)
	#define UPDATE_ZEN_DROP_DOWN_20080806

#if GAME_VERSION >= G_V_S4_5
	//[���׼���] ���⽺ų��ȭ�� ��ų �����ܼ���(goni)
	#define MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807

	// [������Ʈ] ��ųŰ ����� Ȯ��(goni)
	#define UPDATE_SKILLKEY_EXPAND_20080804

	// [���׼���] ���� 4 ������Ʈ - ���׼��� 2�� (hnine)
	#define MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804

	// [���׼���] �޺���ų �����ͱ���ü�� ��ų�� Ȯ��(goni)
	#define MODIFY_COMBOSKILLDATA_SKILLNUMBER_20080801
#endif

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] �����ӽ� �߰������ ���빮�� ����(goni)
	#define MODIFY_ADDLIFE_BUGFIX_20080730
#endif

	// [������Ʈ] ���� 4 ������Ʈ - ���� ������ �ý��� ���� (hnine)
	#define MODIFY_SOCKET_OPTION_SYSTEM_20080725

	// [���׼���] �ʼ����̵��� �����κ� ���� ����(goni)
	#define MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723

	// [������Ʈ]���� ����, ���� ���� �۾�(�ִ�HP �� ����) (pelero)
	#define MODIFY_GATE_AND_STATUE_INCREASE_MAX_HP_20080718

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ���� 4 ������Ʈ - ���׼��� 1�� (hnine)
	#define MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722

	// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ���� ���� 5�� (hnine)
	#define MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715

	// [������Ʈ] ���� 4 ������Ʈ - ���� ������ �α� �߰� (hnine)
	#define ADD_SOCKET_OPTION_LOG_20080715

	// [������Ʈ] skill(kor).txt ��ũ��Ʈ Ȯ�� - ��ų Ÿ��(riverstyx)
	// �ϴ� �ؿܸ� ���� ������ �����ؾ���
	#define ADD_EXPAND_SKILL_SCRIPT_FOR_SKILLTYPE_20080515

	// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ���� ���� 4�� (hnine)
	#define MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#endif

	// [���׼���] Ÿ������,��ȯ���ε��� üũ(goni)
	#define MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711

	// [���׼���] ��ų ���� �䱸���������� ����(riverstyx)
	#define MODIFY_SKILL_REQUIREENERGY_20080423

#if GAME_VERSION >= G_V_99B
	// [���׼���] ���� ���ӽ� ��ũ ���Ǹ� ��ų �ʱ�ȭ �ȵǴ� ����(pelero) - WOPS��ȣ:20295
	#define MODIFY_DARK_SPRIT_SKILL_INITIALIZE_BUGFIX_20080702
#endif

	// [���׼���] ���˻簡 ���̿� ��Ʈ ������ ���� �� �������� �ö� ���¿��� ��������� ����
	// ��Ʈ �������� ���� ������� ��� ���� ������ �ƴ� ���¿����� ��������� �����Ǵ� ����(pelero) - WOPS��ȣ:20674
	#define MODIFY_BLOOD_ATTACK_AND_ADD_STRENGTH_BUGFIX_20080701

	// [���׼���] ȸ�������� ��ȭ ��ų ����� ��ų ������ �پ��� ����(pelero) - WOPS��ȣ:26969
	#define MODIFY_ML_STRENGTHEN_WHEEL_RANGE_BUGFIX_20080630

	// [���׼���] ĭ���� ������ ��ũȣ�� ����(pelero) - WOPS��ȣ:20444
	#define MODIFY_KANTURU_ENTERANCE_DARKHORSE_BUGFIX_20080627

#if GAME_VERSION >= G_V_S4_5
	// [���׼���] ��ũ�ε� ���� ����� ���� ����(pelero) - WOPS��ȣ:19787
	#define MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	// [���׼���] ������ �������� �Ǹ��� ���忡�� ��Ƽ�� ����ġ �й� ���� ����(pelero) - WOPS��ȣ:29466
	#define MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
#endif

	// [���׼���] 3�� ���� �ɼ� 5% Ȯ���� ����� ���� ȸ�� ���� ����(pelero) - WOPS��ȣ:28215
	#define MODIFY_THIRD_WING_OPTION_5_PERCENT_REFILL_ALL_BUGFIX_20080624

	// [������Ʈ] GM ��ũ�� �ִ� Ȯ�� (hnine)
	#define MODIFY_MAX_GMCMD_EXTEND_20080630

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] ���� 4 ������Ʈ - ���� 4 ������Ʈ �߰� ��ġ�� �ش� ���������� On/off
	#define ADD_SEASON_4_UPDATE_20080617
#endif

	#ifdef ADD_SEASON_4_UPDATE_20080617
		// [���׼���] ����ĳ�� ���� �ε��� ����(goni)
		#define MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709

		// [������Ʈ] ���� 4 ������Ʈ - ������ ���׷��̵�/���� ���� (hnine)
		#define MODIFY_ITEM_UPGRADE_MIX_RATE_FIX_20080627
		// [������Ʈ] ���� 4 ������Ʈ - ���̳ʽ� ���� ���� ���� (hnine)
		#define MODIFY_MINUS_FRUIT_POINT_CALC_20080627

		// [������Ʈ] ����4 ����ĳ�� ������ ����(goni)
		#define UPDATE_BLOODECASTLE_SCADULE_20080624
		// [������Ʈ] ����4 �űԽ�ų �߰�(goni)
		#define ADD_SEASON4_NEW_SKILL_20080522
		// [������Ʈ] ����4 �ű�ĳ���� ȯ����ų(Pollution)�߰�(goni)
		#define UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
		// [������Ʈ] ���� 4 ������Ʈ - ���� ������ �ý��� (hnien)
		#define ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
		// [������Ʈ] ���� 4 ������Ʈ - ������ ���� ������ �κ� Ȯ�� (hnine)
		#define ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ���� NPC (hnine)
		#define ADD_ITEM_SOCKET_OPTION_MIX_20080415
		// [������Ʈ] ���� 4 ������Ʈ - ī���� ���� �������� Ȯ�� (hnine)
		#define MODIFY_CHAOSMIX_PROTOCOL_EXTEND_20080526
		// [������Ʈ] ��Ŭ���� ��ƾ, ���̾� ����Ʈ ������ ��ǥ ����(pelero)
		#define MODIFY_RAKLION_COOLUTIN_AND_IRON_KNIGHT_REGEN_BUGFIX_20080828
		// [������Ʈ] ��ȭ�� ��� �� ������ ó��(pelero)
		#define MODIFY_RAKLION_BOSS_ZONE_BANISH_USER_20080818
		// [������Ʈ] �������� �̵��ϸ鼭 ��ų�� ����ϴ� ����(pelero)
		#define MODIFY_RAKLION_SELUPAN_MOVING_USE_SKILL_BUGFIX_20080813
		// [������Ʈ]��Ȥ �������� ������ �ʾƼ� ��Ŭ���� �������� ��ü �������� ���ߴ� ����(pelero)
		#define MODIFY_RAKLION_SELUPAN_NO_REGEN_BUGFIX_20080730
		// [������Ʈ]������ ���� ��ų Ÿ�� ����(pelero)
		#define MODIFY_RAKLION_SELUPAN_FIRST_SKILL_BUGFIX_20080721
		// [������Ʈ] ����4 �̺�Ʈ�� �α� �߰�(pelero)
		#define UPDATE_SEASON_4_EVENT_LOG_200800715
		// [������Ʈ] ����4 ������Ʈ - ��Ŭ���� ������ ���� ���� 2��(pelero)
		#define MODIFY_RAKLION_BUGFIX_2ND_20080710
		// [������Ʈ] ����4 ������Ʈ - ��Ŭ���� ������ ���� ����(pelero)
		#define MODIFY_RAKLION_BUGFIX_20080707
		// [������Ʈ] �űԸ� ��Ŭ���� �߰�(pelero)
		#define	ADD_RAKLION_20080408
		// [������Ʈ] ��Ŭ���¿��� ����ϴ� ��������(pelero)
		#define RAKLION_PROTOCOL_20080625
		// [������Ʈ] ������ ���� ���� ��ų ���� �߰� TSkillDefine.h (pelero)
		#define UPDATE_MONSTER_SKILLUNIT_SLOT_EXTEND_20080514
		// ����4 ����ȭ �Ŀ� ���� �ٶ��ϴ�.
		#define SEASON4_PATCH_GROOVING
	#endif // ADD_SEASON_4_UPDATE_20080617

	#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
		// [���׼���] �õ� ���� ���н� �������� ������ 0~2 �ٿ�ǵ��� ���� (hnine)
		#define MODIFY_SEED_EXTRACT_FAIL_RESULT_20080903

		// [���׼���] �õ�� ���Ǿ��� ��� Ȯ���� ���� �����ϵ��� ���� (hnine)
		#define MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	#endif

	#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
		// [���׼���] ���� 4 ������Ʈ - ���� ������ ��� ��� ���� (hnine)
		#define MODIFY_SOCKET_ITEM_DROP_SYSTEM_FIX_20080828
	#endif

	#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
		// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ���� ���� 3�� (hnine)
		#define MODIFY_ITEM_SOCKET_OPTION_BUGFIX_03_20080704

		// [������Ʈ] ���� 4 ������Ʈ - ���� ������ ���� ���� 2�� (hnine)
		#define MODIFY_ITEM_SOCKET_OPTION_BUGFIX_02_20080630
	#endif

	// [���׼���] OBJECTSTRUCT�����ÿ� ���� ���ɼ��� �־� �����.�׽�Ʈ ����.
	#define MEM2_KUZIMO

	// [���׼���] LNK4084 ��� �޽��� ���Ÿ� ���� OBJECTSTRUCT gObj[MAX_OBJECT]�� Heap�޸� �̵�. ��Ÿ ���� ����.
	#define MEM_KUZIMO

    
#ifdef FOR_KOREA
	// [������Ʈ] ����4 PC�� ����Ʈ ���� ����
#ifdef PCBANG_POINT_SYSTEM_20070206
	#define PCBANG_KUZIMO
#endif
	// [�����۾�] �н����� ���� 12bytes�� ����(kuzimo)
	//#define PW_KUZIMO
#endif // FOR_KOREA

	// [���׼���] �Ƿ� ���׼���(goni)
	#define MODIFY_EVIL_BUGFIX_20080618

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	// [���׼���] ���� ų �� SD, ���� ��ø ȸ�� ���׼���(goni)
	#define MODIFY_SKILL_ML_MONSTER_KILL_SD_BUGFIX_20080514
#endif

	// [���׼���] AddMonster ����(goni)
	#define MODIFY_ADDMONSTER_BUGFIX_20080423

	//[������Ʈ] ��Ʈ������ ��� �α׼��� (goni)
	#define UPDATE_SET_ITEM_DROP_LOG_20080420

	// [���׼���] �������� ���� �Ұ� ���� ���� �α� (hnine)
	//#define MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] ĳ���� ����(goni)
	#define UPDATE_CHANGE_CHARACTERNAME_20080410
#endif

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] NPC ���� �̺�Ʈ ������ �� ��Ʈ����(goni)
	#define UPDATE_NPC_REO_NEW_EVENTITEMBACK_20080408
#endif

	//[���׼���] ������ ��ġ�� ���� ����(goni)
	#define MODIFY_ITEMDURABILITY_BUGFIX_20080408

	// [���׼���] ��ȯ���� ��ũ�Ͻ�, �̳ʺ��̼� PVP ����ð� ����(goni)
	#define MODIFY_SUMMONER_WEAKENESS_ENERVATION_PVPEFFECTTIME_BUGFIX_20080404

	// [���׼���] ���� ���ε�Ͻð� ���¼���(goni)
	#define MODIFY_CASTLE_CROWN_ACCESS_TIME_CHECK_BUGFIX_20080403

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �̺�Ʈ ������ �鿡 ��Ʈ������ �߰�(goni)
	#define UPDATE_PROBABILITYiTEMBAG_ADD_SETITEM_20080329

	// [������Ʈ] ItemSetOption(kor)�� ��ȯ���� Ŭ���� Ȯ��(goni)
	#define UPDATE_SETOPTION_SCRIPT_ADD_CLASS_20080321

	// [������Ʈ] ���ɳ��� �̺�Ʈ(riverstyx)
	#define ADD_EVENT_CHERRYBLOSSOM_20080312

	// [������Ʈ] �� �̺�Ʈ�� ���� ����ġ ���� �߰�(pelero)
	#define UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	// [������Ʈ] ���˻�, ��ũ�ε�, ��ȯ���� ��Ƽ ��Ʈ ����ġ �߰�(pelero)
	#define UPDATE_ADD_NEW_PARTY_SET_EXP_20080325

	// [������Ʈ] ������ �ø��� Ȯ�� (hnine)
	#define MODIFY_ITEM_SERIAL_EXPAND_20080227
#endif

	// [������Ʈ] ������ �ø��� �ߺ�üũ �ɼ� ���ε� �����ϵ��� ���� (hnine)
	#define MODIFY_SERIAL_CHECK_OPTION_RELOAD_20080212

	//[���׼���] ��ų����Ʈ ������ ����(goni)
	#define MODIFY_SKLL_LIST_SIZE_BUFFIX_20080221

	// [������Ʈ] ���������Ұ��� �ִ뷹�� ��ũ��Ʈ�� ����(goni)
	#define UPDATE_SHADOW_PHANTOM_MAX_LAVEL_LOAD_SCRIPT_20080220

	// [������Ʈ] "/��û off"�� ������� �α�/���� ��û������ �źε� �߰�(goni)
	#define UPDATE_REQUEST_OFF_ADD_RELATIONSHIP_20080219

#if GAME_VERSION >= G_V_S4_5
//�ؿܷ� ����� ������ ������ �߻��Ͽ� ���� ������������ �߰��� riverstyx
#ifdef FOR_KOREA
	// [���׼���] �Ǿ�������Ʈ���� ����� ó��(goni)
	#define MODIFY_PCBANGPOINTSHOP_CLOSEWINDOWS_BUGFIX_20080218
#endif // FOR_KOREA
#endif

//////////////////////////////////////////////////////////////////////////
// ����3plus �ű�ĳ���� ��ų�߰�
//////////////////////////////////////////////////////////////////////////
#if GAME_VERSION >= G_V_S2
	// 380 ������ �� �ȶ�������(goni)
	#define MODIFY_380_EXCELENT_ITEM_DISABLE_20080212
#endif

	// [���׼���] ������ų ������ ����(goni)
	#define MODIFY_SLEEP_SKILL_SUCCESSRATE_BUGFIX_20080125

	// [������Ʈ] �α��߰�(goni)
	#define UPDATE_ADD_LOG_20080117

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �ɼ� Ű �����߰�(goni)
	#define UPDATE_OPTIONKEY_LEVEL_20080116

	// [������Ʈ] �ű�ĳ���� �űԽ�ų �߰�(goni)
	#define UPDATE_SUMMONER_NEW_SKILL_20080111
#endif

	// [���׼���] �ű�ĳ���� summoner 3������ ���׼���(goni)
	#define MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �űԸ� ����� �� �߰�(goni)
	#define UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
#endif
//////////////////////////////////////////////////////////////////////////

	// [���׼���] ������ NPC ���� ���� �ɰ�� ���� ����� ���� ����(goni)
	#define MODIFY_MIRAGE_NPC_ACT_CLOSESHOP_BUGFIX_20080123

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] �����ͷ��� ���� �̺�Ʈ�� ����� ��� ���Ȯ�� ����(goni)
	#define UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
#endif

	// [���׼���] ���� ������ ���� ��(goni)
//	#define MODIFY_ADDMONSTER_ADDLOCK_20080117

	// [���׼���] ���� �ý��� Ȯ�� 7�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_07_20080109

	// [���׼���] �̵����� ���׼���(goni)
	#define MODIFY_MOVEMAP_BUGFIX_20080109

#ifdef DUEL_SYSTEM_20031028
	//[���׼���] ������ PK�� ������ �ݻ�� �ڽ��� ������� �󵥹��� PK���� �ȵǰ� ����(goni)
	#define MOIDFY_PK_DAMAGE_REFLECT_NOPK_BUGFIX_20081202
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [���׼���] ���� �ý��� Ȯ�� 6�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_06_20080102
#endif

	// [���׼���] ������� �ð� �� �ִ� �ο��� ����(goni)
	#define MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
#if GAME_VERSION >= G_V_S3_E2
	// [���׼���] �ű�ĳ���� ���׼���_1(goni)
	#define MODIFY_SUMMONER_BUGFIX_1_20071221
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	// [���׼���] ���� �ý��� Ȯ�� 5�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_05_20071220

	// [���׼���] ���� �ý��� Ȯ�� 4�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_04_20071216
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [���׼���] ������ ���� ��ũ�� ����� ��µ��� �ʴ� ���� ���� (hnine)
	#define MODIFY_CASTLESIEGE_MARK_DISPLAY_20071216

	// [���׼���] �ű�ĳ���� ��� ������ ���� ���׼���(goni)
	#define MODIFY_SUMMONER_ITEM_DURABILITY_DOWN_BUGFIX_20071213
#endif

	// [������Ʈ] �ؿ� �κ�����ȭ ��ũ�� ���������� ���� (hnine)
	#define MODIFY_INVEN_CLEAR_MACRO_20071212

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	// [������Ʈ] ���� �ý��� Ȯ�� 3�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_03_20071211
#endif

	// [�α��߰�] �Ϻ� ��û���� ����ġ ��� �α׿� ���� �ε����� �̺�Ʈ Ÿ�� �߰�(hnine)
	#define MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

	// [���׼���] Ŭ���̾�Ʈ ���ӽ� ����� ��������(goni)
	#define MODIFY_JOIN_ERROR_BUGFIX_20071211

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	// [������Ʈ] ���� �ý��� Ȯ�� 2�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_02_20071204
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] �ű�ĳ���� 3������ ���ջ���( ������ ���� ) (goni)
	#define ADD_SUMMONER_THIRD_WING_MIX_20071203

	// [������Ʈ] �ű�ĳ���� 2��, 3������ (������ ����, ������ ����) (goni)
	#define ADD_SUMMONER_SECOND_THIRD_WING_20071130
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	// [������Ʈ] ���� �ý��� Ȯ�� 1�� ���� (hnine)
	#define MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] �ű� UI �߰�_����Ű, ����Ƚ��(goni)
	#define ADD_SEASON_3_NEW_UI_20071122

	// [���׼���] ����Ʈ ���� �������� ���� ���� �߰� ���� �������� ���� (hnine)
	#define MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
#endif

	// [���׼���] ī���� ���ս� ���� ������ �ʰ��� ��� Ȯ���� ���̳ʽ� ���� ������ ���� ���� (hnine)
	#define MODIFY_CHAOSMIX_MONEY_OVER_MAX_FIX_RATE_20071114

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] ���� 3 �ű� ĳ���� �߰�(goni)
	#define ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#endif

	// [���׼���] �κ��丮���� ���� ��ġ�� ������ �̵��� ������ ������� ����(goni)
	#define MODIFY_INVENTORY_ITEM_MOVE_BUGFIX_20071113

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] ����� ȣ�� ��ӷ��� ����(goni)
	#define UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026
#endif

	// [���׼���] ��Ƽ�� ���漳�� ���� ���׼���(goni)
	#define MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022

	// [������Ʈ] Ÿ��Ʋ�ٿ� ���������߰�(goni)
	#define UPDATE_TITLEBAR_VERSION_INFO_120071004

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] �����ͷ��� �ý���(RKFKA)
	#define MASTER_LEVEL_SYSTEM_20070912	

	// [������Ʈ] ���� �ý��� ���� ���� (hnine)
	#define MODIFY_BUFF_SYSTEM_EXTENTION_20070917

	// [������Ʈ] ���� �ý��� Ȯ�忡 ���� ���� ��ų ���� ȿ�� ���� (hnine)
	#define MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] ItemAddOption���� ī�װ� ���� �ε� �ϵ��� ���� (hnine)
	#define MODIFY_ITEMADDOPTION_LOAD_CATEGORY_20070712
#endif

#if GAME_VERSION >= G_V_S3_E2
	// [������Ʈ] ���� 3 �ű� �� �������� �߰�(goni)
	#define ADD_SEASON_3_NEW_MAP_20070910

	// [���׼���] ������ ����Ŭ ���� (RKFKA)
	#define MODIFY_CASTLE_SIEGE_CYCLE_RESET_20071009
#endif

	// [������Ʈ] �ؿ� �κ�����ȭ on/off (hnine)
	//#define ADD_PARTIALY_CHARGE_ON

	// [������Ʈ] �ؿ� �κ�����ȭ ������ ���� (hnine)
	#ifdef ADD_PARTIALY_CHARGE_ON
		// [���׼���] ���� ���� ó�� ���� ���� (hnine)
		//#define MODIFY_IMPROVE_POTION_CALC_PROCEDURE_20071023
		
		// [������Ʈ] �κ�����ȭ : ȯ����� ���� ����ġ�� ����� ������ ȿ�� ���� (hnine)
		#define MODIFY_SET_MARK_EFFECT_AT_ILLUSIONTEMPLE_20071018
		// [������Ʈ] �κ�����ȭ : ���� �����۰� �ؿ� ������ �� �ε����� ��ġ�� �κ� ���� (hnine)
		// �̵��� ����(13,52)->(13,59), ���˺�(13,53)->(13,60)
		#define MODIFY_PCS_OVERLAP_ITEM_BUFF_INDEX_20071011

		// [������Ʈ] �κ�����ȭ : ����� ������ ����Ȯ�� ������ ȯ����� ����ǰ� 3������ �߰� (hnine)
		#define MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

		// [������Ʈ] �κ�����ȭ : ȯ����� ���� ����� �߰� (hnine)
		#define ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

		// �ؿ� �κ�����ȭ 1�� ������Ʈ (hnine)
		#define ADD_PARTIALY_CHARGE_SYSTEM_01_20070110
		// �ؿ� �κ�����ȭ 2�� ������Ʈ (hnine)
		#define ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
#if GAME_VERSION >= G_V_S3_E2
		// �ؿ� �κ�����ȭ 3�� ������Ʈ (riverstyx, hnine)
		#define ADD_PARTIALY_CHARGE_SYSTEM_03_20080407
		// �ؿ� �κ�����ȭ 4�� ������Ʈ (riverstyx)
		#define ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
		// �ؿ� �κ�����ȭ 5�� ������Ʈ (grooving)
		#define ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
		// �ؿ� �κ�����ȭ 6�� ������Ʈ (natinda)	
		#define ADD_PARTIALY_CHARGE_SYSTEM_06_20090129
		// �ؿ� �κ�����ȭ 7�� ������Ʈ (pelero)	
		//#define ADD_PARTIALY_CHARGE_SYSTEM_07_20090915
#endif

		// �ؿ� �κ�����ȭ 2���� 1�� ������Ʈ�� �ʿ���.
		#if defined ( ADD_PARTIALY_CHARGE_SYSTEM_02_20070423 ) && !defined ( ADD_PARTIALY_CHARGE_SYSTEM_01_20070110 )
			#define ADD_PARTIALY_CHARGE_SYSTEM_01_20070110
		#endif 
		// [������Ʈ] ���� ���� 2.5�� �ؿ� �κ�����ȭ ���ս� �߻� ���� ���� (hnine)
		#define MODIFY_SEASON25_PCS_UNIFICATION_20070903
		// [������Ʈ] �ؿܺκ�����ȭ ���� ���� �����丵 �߰� (hnine)
		#define MODIFY_PCS_CHAOSMIX_REFACTORING_20070903

		// [���׼���] ȯ����� �������ġ ���� ���� ���� (hnine)
		// #define MODIFY_REMOVE_MARK_BENEFIT_ILLUSIONTEMPLE_REWARD_20071116

		// [������Ʈ] ȯ����� �޽��� ���ö���¡ (hnine)
		// #define MODIFY_ILLUSIONTEMPLE_MESSAGE_LOCALIZING_20071104
		// -> MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417

		// [������Ʈ] ���� ���� ���ս� ����� ���� ��� ���� (hnine)
		#define MODIFY_LUCKYCHARM_BLOCK_USE_CASTLEPOTION_20071105

#if GAME_VERSION >= G_V_S3_E2
		// [���׼���] �ؿ� ���� ĳ�� MasterLevel ���� ���� ����(riverstyx)
		#define MODIFY_MASTER_LEVEL_SYSTEM_20080414

		// [���׼���] �ؿ� �̵���� ���� ���� (riverstyx)
		#define MODIFY_MOVECOMMAND_20080415

		// [������Ʈ] ȯ����� ��� �޽��� ������Ʈ (riverstyx)
		// �ϴ� �ؿܸ� ����
		#define MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417

		// [������Ʈ] �����ͷ����� 0���� ������ �� �ֵ��� ����(riverstyx)
		#define MODIFY_MASTER_LEVEL_SYSTEM_COMMAND_20080417

		// [���׼���] 10���� ���� ����ġ �� �� �ս� ���� ����(riverstyx)
		// ������ �ִ� ���� �ϴ� �ؿܸ� ����
		#define MODIFY_MINLEVEL_EXPRATE_20080630

		// [������Ʈ] ������ �������� �� �ȵ��� ����(riverstyx)
		// Ŭ���̾�Ʈ ���׷� Ȯ�ε� ������ ���� ���� 20081024 (riverstyx)
		//#define ADD_NOT_SELL_EQUIPITEM_20080625

		// [���׼���] �Ǹ��� ���� Ȯ��� �ɿ� ���� 30���� �޽��� �Ȱ��� ���� ���� (riverstyx)
		// ������ �ִ� ���� �ϴ� �ؿܸ� ����
		#define MODIFY_DEVILSQUARE_CLOSE_MSG_20080717

		// �Ǹ��� ���� ���� ������ ��� ���� - �ؿܸ� ����(grooving)
		#define DEVILSQARE_COMBINE_ITEM_DROP_20090114

		// �����౸ �̺�Ʈ �Ϲ� ���� ��ɾ�� ���� �Ҵ� ���� ���� - �ؿܸ� ����(grooving) 
		#define BATTLESOCCERWAR_COMMAND_BUGFIX_20090310

		// [���׼���] ��Ʈ ������ ���� ����(riverstyx)
		//#define MODIFY_SETITEM_BUGFIX_20080731

		// [���׼���] �������� ���� ����ڰ� ���弭��(����� MAX)�� �̵��� �߻��ϴ� ���� ���� ���� 
		// ������ �߻�����(riverstyx)
		#define MODIFY_MOVECOMMAND_BUGFIX_20080806

		// [���׼���] ������ ������ ������ üũ (riverstyx)
		// �ϴ� EQUIPMENT_WEAPON_RIGHT, EQUIPMENT_WEAPON_LEFT üũ - ������ �ִ� ���� �ϴ� �ؿܸ� ����
		#define MODIFY_EQUIPITEM_CHECKDURABILITY_20080811

		// [���׼���] ������ ����/�Ǹ� �� ����(60) �Ǹ�(18) ���ѿ� �ɸ���� ó����(riverstyx)
		#define MODIFY_SELLBUYITEM_20080811

		// [���׼���] ���� ��Ʈ ������ ���� �� �������� �ö� ���¿��� ���̽��ַο츦 ����
		// ��Ʈ �������� ���� ���̽��ַο� ��� ���� ������ �ƴ� ���¿����� �����Ǵ� ����(riverstyx)
		#define MODIFY_SKILL_ICEARROW_AND_ADD_DEXTERITY_BUGFIX_20080820
#endif

		// �ؿ� �κ�����ȭ�� �� �ִ� ���¿����� �����Ǹ� �ȵǴ� �����ε�
	#else
		#ifdef FOR_KOREA
		#if GAME_VERSION >= G_V_S2_2
			// [������Ʈ] �α� �߰� 6�� (hnine)
			#define EXTEND_LOG_SYSTEM_06_20070308
		#endif
		//////////////////////////////////////////////////////////////////////////
		//	
		// ** �������� �����ϴ� NPC ����(RKFKA)
		// ��ũ(PC�� Ȱ��ȭ): PC�濡�� ���� / EventDB�� ��ϵ� ����	/ �� ��			/ �����۹鿡�� ���� ����
		// ����(ȭ��Ʈ����) : �����ϸ� ����	/ EventDB�� ��ϵ� ����	/ �� ��			/ �����۹鿡�� ���� ����
		// �ٷ�(�����̺�Ʈ)	: �����ϸ� ����	/ ��� ����				/ �Ϸ翡 �� ��	/ �����۹鿡�� ���� ����
		//
		//////////////////////////////////////////////////////////////////////////	
#if GAME_VERSION >= G_V_S3_E2
		// [������Ʈ] ���������� NPC ����� �ٷ� �߰�(RKFKA)
		#define ADD_ITEM_GIVE_NPC_ALAN_20070823
#endif
		#endif // FOR_KOREA
	#endif // ADD_PARTIALY_CHARGE_ON

#if GAME_VERSION >= G_V_S4_5
	// �ؿ� �κκ�����ȭ �����۱��� ��û��� �߰�(goni)
	#define UPDATE_BUY_CASHITEM_ERROR_CODE_20090410
#endif

	// �ؿ� �κ�����ȭ 5�� ������Ʈ (grooving)
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_05_20080930
	// [������Ʈ] 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
	#define ADD_TELEPORT_CHARM_ITEM_20080930
	#endif //ADD_PARTIALY_CHARGE_SYSTEM_05_20080930

	// �κ�����ȭ 4�� ������Ʈ
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_04_20080723
		// [������Ʈ] ī���� ����. ī�������� ���н� �Ҹ�Ǵ� ������ ��ȣ //grooving
		#define	ADD_CHAOSMIXCHARM_ITEM_20080702

		// [������Ʈ] 4�� ������ �߰�(ġ���� ����, �ż��� ����) riverstyx
		#define ADD_CASHITEM_4RD_MARK_20080723

		// [������Ʈ] 4�� ������ �߰�(������ ��ũ��, ��ȭ�� ��ũ��) riverstyx
		#define ADD_CASHITEM_4RD_SCROLL_20080723
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_04_20080723

	// �κ�����ȭ 3�� ������Ʈ
	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_03_20080407
		// [������Ʈ] ī����ī�� ���� �ý��� Ȯ�� (hnine)
		#define MODIFY_EXPEND_CHAOSCARD_SYSTEM_20080326

		// [������Ʈ] ��ȯ���� ĳ���� ī�� (riverstyx)
		#define ADD_CHARCARD_SUMMONER_20080313

		// [������Ʈ] ���� ������ ĳ����ó�� 
		#define	MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

		#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		// [���׼���] ���� ������(���) ��븸�� �� �ɷ�ġ ����� �ٽ� �ϵ��� ����(riverstyx)
		#define MODIFY_BUFFITEM_MEDICINE_20080424
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

		// [���׼���] ���� ������(���) ��Ʈ ������ �ɷ�ġ ��� �κ� ����(riverstyx)
		#define MODIFY_BUFFITEM_MEDICINE_SETITEM_20080424

		// [���׼���] ���� ������(����) ������ ���� ���� ���� ĳ�� ����ġ�� �����Ǵ� ���� ����(riverstyx)
		#define MODIFY_MARKITEM_20080506

		// [���׼���] ���� ������(���) ��Ʈ ������ �ɷ�ġ ��� �κ� ����(riverstyx)
		#define MODIFY_BUFFITEM_MEDICINE_SETITEM_20080709
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_03_20080407

	// [������Ʈ] ���� ������ �߰� (hnine)
	#define ADD_PCS_MARK_OF_EXP_20070205

	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_02_20070423
		// [���׼���] Į���� ���� ����� ��� ���� �κ� ó�� �߰� (hnine)
		#define MODIFY_KALIMA_FREETICKET_USE_RESULT_20070830

		// [���׼���] ���¿��� ��� �ڵ� �߰� (hnine)
		#define MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
		// [���׼���] ������ �̺�Ʈ�� �ɼ� �ε� ������ �ٲ� (hnine)
		#define MODIFY_ELDORADO_EVENT_OPTION_LOAD_SEQUEANCE_20070821

		// [���׼���] ItemAddOption�� �߰�ȿ���� Add������ �����Ű���� ���� (hnine)
		#define MODIFY_ITEMADDOPTION_VARIABLE_20070725
		// [������Ʈ] Ȯ�� ���� ���� ���� ������ �⺻ ���� ������ ��� �⺻ ���� ������ �ʱ�ȭ �ǵ��� ���� (hnine)
		#define MODIFY_PCS_FRUITS_EX_DECREASE_POINT_SET_DEFAULT_20070806
		// [���׼���] ĳ�� ������ �̵� �ɼǿ� ��� ������ Ƽ�� 6 �߰� (hnine)
		#define MODIFY_ADD_CASHITEM_MOVE_CHECK_ITEM_20070821
		// [���׼���] ItemAddOption�� �ɼ��� �ҷ��� �����۰� �κ�����ȭ �������� ȿ���� �и��ؼ� �����Ų��.
		#define MODIFY_ITEMADDOPTION_SET_EFFECT_20070801
		// [������Ʈ] ������ ���Ž� �������� �ִ� ��ġ�� ������ŭ �����ֵ��� ���� (hnine)
		#define MODIFY_OVERLAP_ITEM_DURATION_20070730
		// [������Ʈ] ItemAddOption���� ī�װ� ���� �ε� �ϵ��� ���� (hnine)
		#define MODIFY_ITEMADDOPTION_LOAD_CATEGORY_20070712
		// [������Ʈ] �Ⱓ�� ������ ���� Ȯ�� (hnine)
		#define MODIFY_PERIODITEM_SLOT_EXTEND_20070423
		// [������Ʈ] ĳ�� ������ ����Ʈ �켱 ���� �߰� (hnine)
		#define ADD_CASH_ITEMLIST_RATIO_20070430
		// [������Ʈ] ����Ʈ ���� (Ǯ ȸ�� ����) �߰� (hnine)
		#define ADD_FULL_RECOVERY_POTION_20070719
		// [������Ʈ] ���� �ý��� Ȯ�� (hnine)
		#define ADD_PCS_FRUITS_POINT_EXTENTION_20070430
		// [������Ʈ] ���˺� �߰� (hnine)
		#define ADD_PCS_INDULGENCE_20070504
		// [������Ʈ] 100% ȸ�� ���� �߰� (hnine)
		#define ADD_PCS_FULL_RECOVERY_POTION_20070508
		// [������Ʈ] �� �̵� ������ Ȯ���ϴ� ���� ���� (hnine)
		#define MODIFY_MAPMOVEBOUNCE_CHECK_OPTION_20070521
		// [������Ʈ] ��ȭ�� ��� �߰� (hnine)
		#define ADD_PCS_IMPROVE_POTION_20070718
		// [������Ʈ] ����Ʈ ��ũ�� �߰� (hnine)
		#define ADD_ELITE_SCROLL_20070521
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_02_20070423

	#ifdef ADD_PARTIALY_CHARGE_SYSTEM_01_20070110
		// [���׼���] ���� ������ �� ���·� ī���� ī�� ������ �� �� ����. (hnine)
		#define MODIFY_CHAOSCARD_MIX_BLOCK_PSHOP_USE_20070713

		// [���׼���] ����ĳ�� ���� ����� ���� ���� ���� (hnine)
		#define MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627

		// [���׼���] �Ⱓ�� ������ ������ ���� ũ��Ƽ�ü��� �߰� (hnine)
		//#define MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619

		// [���׼���] �Ⱓ�� ������ ���� �� DB�� �̹� ��� ���� �������� �ִ��� Ȯ�� �Ѵ�. (hnine) - �ӽ� ����
		//#define MODIFY_CHECK_PERIOD_ITEM_LIST_AT_ITEMBUY_20070619

#if GAME_VERSION >= G_V_S2_2
		#ifndef FOR_JAPAN
			// [������Ʈ] ���� ȿ���� �ٸ� �������Ե� �����ش�. (hnine)
			#define MODIFY_PERIODITEM_EFFECT_ADD_VIEWPORT_20070601
		#endif // FOR_JAPAN

		// [���׼���] ���� ĳ�� ���� ����� ���� �޽��� ���� ���� (hnine)
		#define MODIFY_FREE_BLOODCASTLE_ENTER_CHECK_20070531

		// [���׼���] ���� ȿ�� ũ���̿��� ���� ����ġ ���� (hnine)
		#define MODIFY_MARKITEM_EFFECT_USE_CRYWOLF_REWARD_20070502

		// [���׼���] ������ ���� ȿ���� ���� ���� �ð��� ������� ���� ���� (hnine)
		#define MODIFY_PERIODITEM_EFFECT_DURING_CLOSETIME_20070416
#endif

		// [���׼���] �÷��� ������ ���ս� �α� ���� (hnine)
		#define MODIFY_PLUSITEM_CHAOSMIX_LOG_BUGFIX_20070330

#if GAME_VERSION >= G_V_S2_2
		// [���׼���] �̺�Ʈ �ʿ����� ���� ������ ȿ�� ���� - ������ ���� ���� (hnine)
		#define MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328

		// [���׼���] ����� ���� ��� ������ ���� ���� �߰� (hnine)
		#define MODIFY_LUCKYCHARM_ADD_WINGMIX_20070328
#endif

		// [���׼���] ���� ����� ī���� ���� ������ ����Ʈ ���� (hnine)
		#define MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328

		// [���׼���] �κ� ����ȭ �Ⱓ�� �ý��� 1�� ���� (hnine)
		#define MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

#if GAME_VERSION >= G_V_S2_2
		// [���׼���] �� �� ������ �ູ ���� ���� ƨ��� ���� ���� (hnine)
		#define MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030
		// [������Ʈ] �ҷ��� ���� �̺�Ʈ �۾� (hnine)
		#define HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
#endif

		// [������Ʈ] ĳ�� �� �ý��� �߰� (hnine)
		#define ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
		// [������Ʈ] ĳ�� ������ �Ⱓ�� �ý��� �߰� (hnine)
		#define ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
		// [������Ʈ] ���� ������ (hnine)
		#define ADD_PCS_LOTTERY_SYSTEM_20070117
		// [������Ʈ] ���� �������� �� �̵� ���� (hnine)
		#define ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221
		// [������Ʈ] �̺�Ʈ �� ���� �����
		#define ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		// [������Ʈ] ���շ� ��� ������
		#define ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
		// [������Ʈ] ī���� ���� Ȯ�� ����
		#define MODIFY_CHAOSMIX_RATE_FIX_20070304
		// 2�� ������Ʈ ���� ������
	#endif // ADD_PARTIALY_CHARGE_SYSTEM_01_20070110

	#if GAME_VERSION >= G_V_S2_2
		// �κ� ����ȭ ������ �߰��� ���� �ʿ��� �����ε�.
		// [������Ʈ] ĳ�� ������ ȿ�� ������ ���� ItemAddOption.h �߰�
		#define ITEM_ADD_OPTION_20061019
	#endif

	// [������Ʈ] ������ �ɼ� �߰�(commonserver.cfg) (RKFKA)
	#define ADD_ZEN_OPTION_20070903

#if GAME_VERSION >= G_V_S3_E1
// [������Ʈ] �̺�Ʈ ��_ȯ�� ���(goni)
	#define ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	// [���׼���] ���ǵη縶�� ������� ����ǥ�� ���� ���� (goni)
	#define MODIFY_ILLUSIONTEMPLE_ITEMVALUE_BUFGIX_20070904 
#endif

	// [���׼���] ������ ���� ���ݽ� ���� �Ȱɸ��� ���� ���� (goni)
	#define MODIFY_SELFDEFENSE_20070904

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] �߼� ���� �̺�Ʈ(RKFKA)
	#define CHUSEOK_MONSTER_EVENT_20070823

	// [������Ʈ] ���� ���� ����� ���н� �����ð� ���� �г�Ƽ ���� (RKFKA)
	#define MODIFY_CASTLE_CROWN_ACCESS_TIME_20070828
#endif

	// [������Ʈ] ������ ���� ���� �� �⺻���� ����� �Լ� ���� (RKFKA)
	#define MODIFY_EVENTITEMBAG_BASIC_SET_20070823

	// [���׼���[ 8 �� ���׼���(goni)
	#define MODIFY_0708_BUFFIX_20070820

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	// [���׼���] ȯ����� ���׼���_6 (goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_6_200708013
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] �α� �߰� 8�� (hnine)
	#define EXTEND_LOG_SYSTEM_08_20070810
#endif

	// [���׼���] �÷��Ƕ� ���� �ݾ� ����Ȯ�� ���� (RKFKA)
	#define MODIFY_CASTLE_HUNTZONE_TAX_BUGFIX_20070806

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	// [���׼���] ȯ����� ���׼���_5(goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_5_20070807

	// [���׼���] ȯ����� ���׼���_4(goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
#endif

	// [���׼���] PK �ý��� ���� ���׼���_������ PK ���� ��Ƽ �г�Ƽ ����(goni)
	#define MODIFY_PK_SYSTEM_BUGFIX_2_20070731

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	// [���׼���] ȯ����� ���׼���_3, MAX_OBJ����(goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [�α׼���] 3������ �α׼���(RKFKA)
	#define MODIFY_LOG_THIRD_CHANGEUP_20070801

	// [�α��߰�] ȯ����� �α��߰�(goni)
	#define ADD_LOG_ILLUSION_TEMPLE_1_20070727
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	// [���׼���] ȯ����� ���׼���_2(goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724

	// [���׼���] ȯ����� ���׼���_1(goni)
	#define MODIFY_ILLUSIONTEMPLE_BUGFIX_1_20070723
#endif

	// [���׼���] �ִ� ����� ���� �˻��� ���׼���(goni)
	#define MODIFY_MAXHITUSER_BUG_FIX_20070711

#if GAME_VERSION >= G_V_S3_E1
	// ȯ����� ��ũ��Ʈ ����( �������ġ ������, �ּ� Skill ��� Ƚ�� )(goni)
	#define UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [���׼���] ���� ĳ�� ���� �ð� ���� ���� ����(goni)
	#define MODIFY_BLOODECASTLE_SCADULE_BUGFIX_20070710
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] ȯ����� ��Ȱ��ȭ�� ����(goni)
	#define MODIFY_ILLUSION_TEMPLE_SCEDULE_20070703

	// [������Ʈ] ȯ�� �渱 �߰�(goni)
	#define ADD_ILLUSION_FENRIR_20070703
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525
	// [���׼���] ����Ʈ ���� ųī��Ʈ ���� �ʱ�ȭ ���� ���� (RKFKA)
	#define MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
#endif

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] ���� ĳ�� ���� �ð���(¦�� Ȧ��) ��ũ��Ʈ�� ����(goni)
	#define UPDATE_BLOODECASTLE_SCADULE_20070702

	// [������Ʈ] ȯ����� ������ MAX_MAP ����(goni)
	#define UPDATE_BEFORE_ILLUSION_TEMPLE_MAX_MAP_20070702

	// [���׼���] ȯ����� ���׼���(goni)
	#define MODIFY_ILLUSION_TEMPLE_20070629

	// [������Ʈ]400���� �޼��� ��Ƽ���α� �߰�(goni)
	#define ADD_LOG_400LEVELUP_PARTY_20070628

	// [������Ʈ] 3�� ���� �߰� (RKFKA)
	#define ADD_THIRD_WING_20070525
	// [������Ʈ] 3�� ���� �ý��� (RKFKA)
	#define THIRD_CHANGEUP_SYSTEM_20070507

	// [���׼���] ����Ʈ �ý��� ���׼��� �� ���� (RKFKA)
	#define MODIFY_QUEST_SYSTEM_20070525

	// [������Ʈ] �Ǹ��� ���� ���۽ð� ������ �߰�(goni)
	#define UPDATE_DEVIL_SQUARE_START_TIME_20070511
#endif

	// [������Ʈ] ���� �ý��� ���� (RKFKA)
	#define MODIFY_MIX_SYSTEM_20070518	

	// [���׼���] ������������ �����ڸ��� ����Ǵ� ����(�����ð� ���Ŀ� ����Ǿ�� ��) (RKFKA)
	#define MODIFY_LIFESTONE_BUGFIX_20070426


#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] Shop14 �߰�-> WzVersionManager, WzAuthGs, WzFsGate ���� ����(RKFKA)
	#define	ADD_SHOP_14_20070308
#endif

	// [���׼���] ���˼��� ��ǰ ��Ͼ����� ������� ����(goni)
	#define MODIFY_VALUABLE_ITME_BUGFIX_20070828

	// [���׼���] ���, ����, ������ ���� �ŷ� ���׼���(goni)
	#define MODIFY_MAGICIARINGRING_TRADE_BUGFIX_20070328

	// [���׼���] PK �ý��� ���� ���� ����_2�ܰ� �̻� ���θ��� ��Ƽ���� ĳ���� �׿������ ���θ��� �Ǵºκ� ����(goni)
	#define MODIFY_PK_SYSTEM_BUGFIX_20070323

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] �α� �߰� 7�� (hnine)
	#define EXTEND_LOG_SYSTEM_07_20070320

	// [������Ʈ] ���� ĳ�� ���� ���� �α� �߰�(goni)
	#define ADD_LOG_BLOODCASTLE_DOOR_20070321
#endif

	// [���׼���] ī�� ������ ���� ĳ���� ������ ���� õ��� ��ȭ�� �޽��� ����(goni)
	#define MODIFY_BLOODCASTLE_PK_USER_ENTER_MESSAGE_20070321

	// [���׼���] ������� ����� ���� ���� �޽��� �߰�(goni)
	#define MODIFY_SELFDEFENSE_CLOSE_20070321

	// [���׼���] ��Ƽ ����ġ ȹ�� ���� 2�� ����(goni)
	#define MODIFY_PARTY_PLAY_EXPERIENCE_2_20070320

	// [���׼���] ī����ĳ�� �������뿡���� ���尡������ ����(goni)
	#define MODIFY_ENTER_CHOSCASTLE_BUGFIX_20070316

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] ���� ���� ����� ����Ʈ ������ ������� �߰�(goni)
	#define UPDATE_VIEWPORTCREATE_CHANGE_ADD_CHARSET_20070314

	// [������Ʈ] ��ǰ ���� ������ �߰� �� ����(goni)
	#define UPDATE_VALUABLE_ITEM_20070307

	// [���׼���] �Ҿ����ǥ�� �̵��� ������ ������� ���׼���(goni)
	#define MODIFY_MOVE_KUNDUN_MARK_20070307

	// [������Ʈ] �����Ǹ� NPC(goni)
	#define ADD_FIRECRACKER_NPC_20070306

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] PK �ý��� ����(goni)
	#define UPDATE_PK_SYSTEM_20070302
#endif

	// [������Ʈ] ������ ���� ���� ����(goni)
	#define UPDATE_SHADOW_PHANTOM_20070302

	// [������Ʈ] GM ��� �߰�: GM ���������߰�, GM ��ȯ���� �߰�, GM ��ũ Ȱ��ȭ, GM �ܼ� ��� �߰�, GM ĳ���� ���Ź��� �߰�(goni)
	#define UPDATE_GM_FUNCTION_20070228

	// [������Ʈ] �̺�Ʈ ������ ���� ��ӿ�����Ʈ ������ �α� �߰�(goni)
	#define ADD_LOG_EVENT_DROP_EXITEM_20070308
#endif

	// [���� ����] ���� �� �ؿ� �������� ���� ����(goni)
	#define MODIFY_BOX_DROP_POSITION_20070308

	// [���׼���] ������ ���۽� �÷��� ���� �ִ� ��� ������ �η����� �ո������� ������.(RKFKA)
	#define MODIFY_CASTLEHUNTZONE_USER_BUGFIX_20070223

	// [�ҽ�����] ����������� ��ŷ������ ������ �ֱ����� �޼��� ����(RKFKA)
	#define DELETE_DEVILSQURE_MESSAGE_TO_RANKINGSERVER_20070223

#if GAME_VERSION >= G_V_S3_E1
	// [���� ����] ���Ź��� �ŷ� ����(goni)
	#define MODEFY_CHAGNE_RING_TRADE_INVENTORY_20070213

	// [���� ����] �Ҿ���� ������ �������� ��ĥ�� ǥ���� ������� �ʴ� ����(goni)
	#define MODIFY_CREATE_LOSTMAP_ITEM_20070209

	// [���� ����] ��Ƽ�� �ٸ��������� ��ɽ� ����ġ ȹ�� ����_������ 200�̻�(goni)
	#define MODIFY_PARTY_PLAY_EXPERIENCE_20070208
	
	// [������Ʈ] �߷�Ÿ�ε��� ���� ��ӿ�����Ʈ ������ �α� �߰�(goni)
	#define ADD_LOG_VALENTINES_DAY_DROP_EXITEM_20070212

	// [���� ����] �߷�Ÿ�ε��� ���� �� �ؿ� �������� ���� ����(goni)
	#define MODIFY_VALENTINES_DAY_BOX_DROP_20070202

	// [������Ʈ] �������ָӴ� �߰�(goni)
	#define UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131

	// [������Ʈ] ī����ĳ�� �ý��ۺ��� - �ð��� ���� ������ 1�� ������ ���Ͱ� �־ ����(RKFKA)->���� ����
	//#define MODIFY_CHAOSCASTLE_SYSTEM_CHANGE_SUCCESS_FLOW_20070129

	// [������Ʈ] ���ں��ް� �ĸ��, ������ �߰�(goni)
	#define UPDATE_QUARTERMASTER_NPC_20070122
#endif

	// [������Ʈ] Ȯ�� �� �̺�Ʈ ������ ���� �߰�(goni)
	#define UPDATE_EVENT_ITEM_BAG_EXTEND_20070131

	// [������Ʈ] ī����ĳ������ ���� ���� ���� �Ǵ��ϴ� ��ƾ ����(RKFKA)
	#define MODIFY_CHOSCASTLE_MONSTER_COUNT_RECAL_20070119

	// [���׼���] ���� ��ų �Ƿ� �����Ǵ����� ���� (RKFKA) -> ���뺸��
	//#define MODIFY_SKILL_EVEIL_AT_SERVER_20070117

#if GAME_VERSION >= G_V_S3_E1
	// [������Ʈ] ���, ����(������ ��)�� ���� �߰�/����(goni)
	#define UPDATE_LOWLEVEL_SUPPORT_RING_20070119

	// [��������] ī����ĳ������ ���Ͱ� ���� ������ ��� ������ ��ǥ�� �α׷� �����.(RKFKA)
	#define ADD_LOG_CHAOSCASTLE_LEFT_MONSTER_ATTR_20070118

	// [������Ʈ] ����/���ݷ���� ��ų ����� �������� ������ ���ڸ� �������� �ʴ´�.(RKFKA)
	#define MODIFY_ELF_DEFENCE_ATTACK_VALUE_UP_SKILL_20070110
#endif

	// [���׼���] �˹�Ʈ�ν� ���� ���� ���� (goni)
	#define MODIFY_ALBATROS_BOW_BUGFIX_20070105

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ũ���������� �� ������ �߰� (RKFKA)
	#define CHRISTMAS_STAR_20061211

	// [������Ʈ] ũ�������� �̺�Ʈ ��Ÿũ�ν� ���Ź��� (hnine)
	#define CHRISTMAS_SANTA_POLYMORPH_RING_20061117

	// [������Ʈ] ������ �������� ������ ����� ������ ǥ���Ѵ�. (RKFKA)
	#define ADD_CASTLE_TOTAL_GUILD_SCORE_20061123

	// [������Ʈ] �α� �߰� 5�� (hnine)
	#define EXTEND_LOG_SYSTEM_05_20060823

	// [���׼���] ����� ȣ���� ��ӵǴ� ���� ���� (hnine)
	#define MODIFY_BLOCK_PUMPKIN_OF_LUCK_DROP_20061120
#endif

	// [���׼���] ī����ĳ�� ���� ���� 2�� (RKFKA)
	#define MODIFY_CHAOSCASTLE_BUGFIX2_20061026

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] �� �� ������ �ູ ���� ���� ƨ��� ���� ���� (hnine)
	#define MODIFY_JOL_BLESS_POTION_USE_EFFECT_20061030

	// [���׼���] ��ȭ�� ���� �ɼ��� �ʱⰪ�� FF�� ����Ǵ� ���� ���� (hnine)
	#define MODIFY_HARMONY_OPTION_INIT_VALUE_FIX_20061019

	// [������Ʈ] �ҷ��� ���� �̺�Ʈ �۾� (hnine)
	#define HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017

	// [������Ʈ] �ҷ��� ���Ź��� �۾� (hnine)
	#define HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
#endif

#if GAME_VERSION >= G_V_99B
	// ī���� ĳ��
	#define CHAOSCASTLE_SYSTEM_20040408						// ī���� ĳ�� �̺�Ʈ	
	#define CHAOSCASTLE_TIME_BUGFIX_20040508				// ī���� ĳ�� �ð� ���� ����

	// ������ ���� �̺�Ʈ -> ���� ������ �̺�Ʈ
	#define RING_EVENT_ITEMBOX_20031124

	// �� ģ�� �̺�Ʈ ����  (b4nfter)
	//#define FRIENDSHIP_STONE_EXCHANGE_20040206			// �����ǵ� ȯ�� �� üũ
	#define FRIENDSHIP_EVENT_RING_BOX_20040211				// �����ǹ��� �̺�Ʈ
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
	// [���׼���] ī����ĳ�� ���� ���� (RKFKA)
	#define MODIFY_CHAOSCASTLE_BUGFIX_20060919
#endif

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ī����ĳ�� ���� ������ �����Ѵ�. (hnine)
	#define MODIFY_DELETE_ZETTO_EVENT_20061121
#endif

#if GAME_VERSION >= G_V_S4_5
	// [������Ʈ] PC�� �̳��� �޽����� �ֱ������� ����ϵ��� ���� (hnine)
	#define MODIFY_PCBANG_DEFAULTER_MESSAGE_20061011
#endif

	// [���׼���] ĭ���� ����Ʈ�޾��� ����,���� �ʱ�ȭ ���� ����(RKFKA)
	#define MODIFY_KANTURU_NIGHTMARE_INIT_BUGFIX_20061010

#ifdef NEW_SKILL_FORSKYLAND
	// [���׼���] ���� ���̽��ַο콺ų. �̽��� �� ��� ���̽����� �ȵǵ��� ����. ( hjlim )
	#define MODIFY_ELF_ICEARROWSKILL_BUGFIX_20060914
#endif

	// [���׼���] ��ũ�ε� - �����ũ ��ų. �ʿ信���� ���� ( hjlim )
	#define MODIFY_DARKLORD_EARTHQUAKE_REQENERGY_BUGFIX_20060914

	// [���׼���] 380 ������ ��ӽ� 380�ɼ��� ������� ���� ���� (hnine)
	#define MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919

	// [��������] ī����ĳ��, ����ĳ�� ���� ���� �α� �߰�(RKFKA)
	#define ADD_LOG_FOR_CASTLE_BUG_20060911

	// [������Ʈ] �ؿ� ���� 2 ��ġ�� ���� ���� ( PC�� ���� �̺�Ʈ�� ������ ó���� ���� ������)
	#ifndef FOR_KOREA
		#define MODIFY_SEASON_02_20060908
	#endif

	#if !defined( MODIFY_SEASON_02_20060908 ) && GAME_VERSION >= G_V_S4_5
		// [������Ʈ] PC�� Ȱ��ȭ �̺�Ʈ (RKFKA) - PC�� �����̺�Ʈ�� �״�� ����ϵ� ������ ���ó���� ����Ǿ���.
		#define PCBANG_REVITALIZATION_EVENT_20060830

		// [������Ʈ] ȭ��Ʈ���� ������ ���� �̺�Ʈ (RKFKA)
		#define WHITEANGEL_GET_ITEM_EVENT_20060822

		// [���׼���] PC�� �̺�Ʈ ī����ĳ�� ���� ���� - ���� ����(RKFKA)
		#define MODIFY_PCBANG_EVENT_FREE_CHAOSCASTLE_20060331

		// [������Ʈ] PC�� �̺�Ʈ - ī����ĳ�� �������� (RKFKA)
		#define PCBANG_EVENT_FREE_CHAOSCASTLE_20060307

		// [������Ʈ] PC�� ���� �̺�Ʈ - �������� �̺�Ʈ (hnine)
		#define PCBANG_COUPON_EVENT_20060124

		// [������Ʈ] PC�� ���� �̺�Ʈ - Į���� �����̵� �̺�Ʈ (hnine)
		#define PCBANG_FREE_KALIMA_EVENT_20060124
	#endif

	// [��������] ĳ�� ���� ���� ���� �α� �߰� (RKFKA)
	#define ADD_LOG_CASTLE_MONSTER_BUG_FIX_20060911

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// [���׼���] ������ ���Ź����� ���� Ŭ���̾�Ʈ���� ������ ������ - ųī��Ʈ ������ �����ش�.
	#define MODIFY_KILL_COUNT_BUGFIX_20060908
#endif

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ����Ʈ�ذ����纯�Ź��� �߰�(RKFKA)
	#define ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830
#endif

	// [���׼���] AI Automata ������ ���� ���� ���� ( hjlim )
	#define MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] ũ���̿��� ���� ���ʹ� ����ġ�� ���� �ʴ´�. - �߰���, ��ũ����, �߸���Ÿ (hnine) 
	#define MODIFY_NON_EXP_IN_CRYWOLF_MONSTER_20060530

	// [���׼���] �� �̵��� Į���� ����Ʈ äũ �κ� ���� (hnine)
	#define MODIFY_MAP_MOVE_LEVEL_MESSAGE_SEND_20060830

	// [���׼���] �α� �ý��� Ȯ�� (2,3��) ���� ���� (hnine)
	#define MODIFY_LOG_SYSTEM_2_3_20060828

	// [���׼���] ���̾ũ����ũ�� ��� �����ϰ� ���� - ����2 ���� (apple)
	#define	MODIFY_ITEM_DROP_FIRESCREAMSCROLL_20060830
#endif

	// [���׼���] ������ ��� ���� ���� (apple)
	// 1) Type �� �����Ǿ� �ִ� Index �� �ִ밪�� �ش�Ǵ� ������ ��� �� �Ǵ� ���� ����
	// 2) ������ ��� ���� Index �ִ� ���� ����
	#define MODIFY_ITEM_DROP_MAXINDEX_20060830

	// [���׼���] ũ���̿��� HeroList �α� ���� ( hjlim )
	#define MODIFY_CRYWOLF_HEROLIST_LOG_BUGFIX_20060828

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] �α� �ý��� Ȯ�� - 3�� (hnine)
	#define EXTEND_LOG_SYSTEM_04_20060823
#endif

	// [������Ʈ] ĭ���� ������ Ÿ�Ӿ��� �̺�Ʈ (RKFKA)	-	20061018 �̺�Ʈ ����� ����(RKFKA)
	//#define KANTURU_TIMEATTACK_EVENT_20060822
#if GAME_VERSION >= G_V_99B
	// [���׼���] �� ���� �̵� �� ĳ���� ���� Flag ����(�߱����� �� ��� ����) ( hjlim )
	#define MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
#endif

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ��ȭ�Ǻ��� �޽��� WTF �߰� ( hjlim )
	#define ADD_JEWEL_OF_HARMONY_WTFMESSAGE_20060818

	// [������Ʈ] �α� �ý��� Ȯ�� - 2�� (hnine)
	#define EXTEND_LOG_SYSTEM_03_20060816

	// [���׼���] 380 �߰� �ɼ� ��ũ��Ʈ Ÿ�� Ȯ�� ( hjlim )
	#define MODIFY_380ITEM_SCRIPTTYPE_EXTEND_20060811

	//[������Ʈ] PC�� ���� ü�輭������ ���ӽ� �̵� ���� �� ���� (hnine)
	#define MODIFY_KANTURU_MAPNUMBER_CHECK_20060807

	// [������Ʈ] Ÿ���� ������ Ʈ���� ���� PMSG_ACTIONRESULT�� Ÿ�� �ε��� �߰�(hnine)
	#define ADD_TRAP_OBJECT_EXTEND_20060731

	// [������Ʈ] ���� AI & ��ų ���� ( hjlim_RKFKA )
	#define	MODIFY_NEW_MONSTER_AI_01_2006724 
	#define MODIFY_NEW_MONSTER_SKILL_01_20060724
#endif

	// [���׼���] 350 ������ 2�� ���� ����. �űԾ������߰� �� ���� ( hjlim )
	#define MODIFY_350ITEM_BUGFIX2_20060823

	// [���׼���] ���� �б� ��ų �⺻�� ���� / �� ������ ���� ��� ó�� ���� ( hjlim )
	#define MODIFY_MONSTERSKILL_SPRING_POISON_BUGFIX_20060817

	//[���׼���] 350 ������ �߰� ���� ���� ( hjlim )
	#define MODIFY_350ITEM_BUGFIX_20060811

	// [���׼���] ���̵� �ּ� ���� üũ ���� ( hjlim )
	#define MODIFY_CHECK_MOVELEVEL_20060731

#if !defined(GAMESERVER_TO_CASTLESIEGESERVER) && GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ĭ���� ������ �߰�(RKFKA)
	#define ADD_KANTURU_20060627
#endif


	//[���׼���] ��ȭ�Ǻ���/380������ DB���� ���� ���� ����.�ſ� �߿�. ( hjlim )
	#define MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ��ũ�ε� ��ų ���̾� ��ũ�� �߰�(RKFKA)
	#define ADD_SKILL_DARKLOAD_FIRESCREAM_20060601

	// [������Ʈ] ���̳ʽ� ���� ���� ���� ( hjlim )
	#define MODIFY_MINUS_FRUIT_POINT_20060622
#endif

	// [���׼���] ���� ���� �� �Ӹ� ĳ�ÿ��� ĳ���͸� �������� �ʴ� ���� ( hjlim )
	#define MODIFY_WHISPER_CASH_BUGFIX_20060704

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] 380 ������ �߰� �ɼ� �ý��� ( hjlim )
	#define ADD_380ITEM_NEWOPTION_20060711

	// [������Ʈ] ��ȭ�� ���� �ý��� (hjlim)
	#define ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
#endif

#if GAME_VERSION >= G_V_99B
	// [���׼���] ���λ��� ���� �� ��������->�η��þ� �̵� �� 
	//				RegenMapNumber �ʱ�ȭ ���ؼ� ����� ��ġ ���� ����(apple)
	//				!!����!! gObjMoveGate()���� RegenMapNumber �� �ʱ�ȭ �ȴ�
	#define MODIFY_MOVEGATE_CHECK_PERSONALSHOP_OPEN_20060704
#endif

	// [���׼���][ExDB] ���� �ٸ� �������� ģ�� �߰� �� �������� ���� ���� ( hjlim )
	#define MODIFY_RE_ADDFRIEND_BUG_20060623

	// [���׼���][ExDB] ĳ���� ���� �� ����� ��� ģ�� ����ó�� �����ʴ� ���� ( hjlim )
	#define MODIFY_CHARCLOSE_BUG_20060623

	// [���׼���] ���������� �ʱ�ȭ ���� ����� ( hjlim )
	#define MODIFY_SWELLLIFE_BUG_FIXAGIAN_20060628

	// [���׼���] �ͽ��÷��� ���̵� �߰� ���� �ݿ� ���� �ʴ� ���� ( hjlim )
	#define MODIFY_EXPLOSION_BLADE_WEAPON_MAGICDAMAGE_BUG_20060623

	// [���׼���] ������Ʈ ������Ż ���� ���� �ɼ� ���� (hjlim)
	#define	MODIFY_EXCELLENT_SHILED_BUG_20060615

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] ����, ���� ���׷��̵� �� �� ��ȣ������ ������ �� ���׷��̵带 �����Ѵ�. (RKFKA)
	#define MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
#endif

	// [���׼���] �ֹι�ȣ�� ����� üũ���� �ʴ� ���� - USA �����ڵ尡 �߰��Ǹ鼭 �ش� ó���� ������(RKFKA)
	#define MODIFY_JOOMINCHECK_FOR_USA

	// [���׼���] ��ȣ���� & ������ ���� ȸ�� - �ִ� HP,MP,AG ���� (hjlim)
	#define MODIFY_GUARDIANSTATUE_LIFESTONE_REFILLBUG_FIX_20060609

	// [���׼���] ���� ��å ��ų - ���� ��ų Ÿ�� ���� ���� ( hjlim )
	#define MODIFY_CLOAKING_TARGET_BUGFIX_20060608

	// [���׼���] ���ձ�� ���� �� ����� ���� (hjlim)
	#define MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529

	// [���׼���] ���� ���� �̵��� ����Ʈ ���� ����� (hjlim)
	#define MODIFY_WIZARD_TELEPORT_VIEWPORT_20060526

	// [���׼���] ���� ���� �� ��ũ���Ǹ� Ÿ�� ���� (hjlim)
	#define MODIFY_DARKSPIRTI_TARGETRELEASE_ON_DUEL_20060524

	// [���׼���] ������Ʈ ������ ����� ������ ���� (hjlim)
	#define MODIFY_EXCELLENT_DROP_DURATION_BUG_20060524

	// [������Ʈ] ���Ӱ��� 2.5�������� �� ���� �̵��ÿ��� ������ ���� �ʴ´�. (hnine)
	#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
		#define MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
	#endif

	// [������Ʈ] ī���� ĳ�������� ���� �ı� ����Ʈ�� ������� �ʴ´�. (hnine)
	#define MODIFY_NO_SHOW_SHIELDBROKE_EFFECT_IN_CHAOSCASTLE_20060523

	// [�������׼���] �� ����Ʈ ��ų ���� �ð� ���� ���� (hnine)
	#define MODIFY_HELLBURST_SKILL_TIME_20060522

	// [���׼���] ������ �ִ� ������ ��� ���� (0527����) (hjlim)
	//#define MODIFY_CALC_DURABILITY_20060523

	// [���׼���] ���� ���׷� 100% ���� ���� (hjlim)
	#define MODIFY_MONSTER_RESISTANCE_IMMUNE_20060523

	// [���׼���] ��� ���� ����Ʈ ���� (hjlim)
	#define MODIFY_KUNDUN_VIEWPORT_20060523

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] �渱 ������Ʈ �ɼ� ����(������/�漺���� ����) (hjlim)
	// MODIFY_DESTRUCTION_FENRIR_GET_MONEY_20060515 ����
	#define MODIFY_FENRIR_EXOPTION_BUG_20060522
#endif

	// [���׼���] �η�����/ũ���̿��� �ʿ��� �渱 ���ݽ� ��� ������ ���� ���� �ʴ� ���� (hjlim)
	#define MODIFY_CASTLEMAP_FENRIR_DURATION_20060522

	// [���׼���] ����/���� �ִ� �� ���ҽ� ���� ���ҵ� �� ����.������ ȿ�� ���뿡�� �и� ( hjlim )
	#define MODIFY_SHIELD_MANA_CHANGEMAXVALUE_20060526

	// [���׼���] ������ ȿ�� ���� ���� ����. ������Ʈ������/��Ʈ������ ( hjlim )
	//#define MODIFY_ITEM_EFFECT_FLOW_20060519
	//#define MODIFY_ITEM_EFFECT_FLOW_20060519_2

	// [���׼���] �������� ü������ �ɼ� ����ȵǴ� ���� �� ������ ���̴� ����(0527�����) (hjlim)
	#define MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018

#if GAME_VERSION >= G_V_S2_2
	// [��ų����] ���Ǵ�Ƽ �ַο� ��ų ��� ���� (RKFKA)
	#define ADD_INFINITY_ARROW_CANCEL_20060516
#endif

#ifdef MAP_SERVER_WORK_20041030
	// [���׼���] �� �̵��� ������ ������� ���� ������ ���� ���� ���� (hnine)
	#define MODIFY_ITEM_DUPLICATION_USING_ITEMDROP_20060518
#endif

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] ����� �ȵǴ� ���� ������ �߰� (hnine)
	#define ADD_NON_DROP_VALUABLE_ITEM_20060518
#endif

	// [���׼���] �Ϻ� ���������� �ʱ�ȭ ���� ���� (hnine)
	#define MODIFY_SWELLLIFE_INIT_BUGFIX_20060411

	// [���׼���] �ڷ���Ʈ �� �ܻ� ���� ���� (����) (hnine)
	//#define MODIFY_AFTERIMAGE_BUGFIX_20060215

	// [���׼���] �Ϻ� ���� ���� ���� �ð� 3�� ���� ���� (hnine)
	#define MODIFY_SIEGE_END_DATE_BUG_FIX_20060116

#if GAME_VERSION >= G_V_S2_2
	// [���׼���] ���� ������ �� �̵��ÿ� ������ �� ���̴� ���� ���� (hnine)
	#define MODIFY_MOVEMAP_RESET_VIEWPORT_BUGFIX_20060511
#endif

	// [���׼���] �ູ�� ����, ��ȥ�� ������ ��ĥ�� �ִ� �ִ� �� ���� 160 ->250 (RKFKA)
	#define MODIFY_SIEGE_POTION_OVERLAP_COUNT_20060511

	// [���׼���] �ؿ� �����Ͻ� ���� �κ� ���� (HAPPY_CAT���� ���� ���� ����) (hnine)
	// �ؿ� ������ �ÿ��� �� ���� ��
	#ifndef FOR_KOREA
		#define MODIFY_SEASON_01_20060317
	#endif

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] �α� �ý��� Ȯ�� (hnine)
	#define EXTEND_LOG_SYSTEM_02_20060516
#endif

	// [�������׼���] ī���� ĳ������ ���� ������ �̻� ���� (hnine)
	#define MODIFY_SHIELDDAMAGE_BUGFIX_IN_CHAOSCASTLE_20060516

	// [�������׼���] �η��þ� ���� �̵� ����  (hjlim)
	#define MODIFY_FORCEMOVE_TO_LORENCIA_20060515

#if GAME_VERSION >= G_V_S2_2
	// [�������׼���] ���˻� �����̳� ��Ʈ ����� ��Ʈ ���� �߰� (hjlim)
	#define MODIFY_MAGUMSA_VOLCANOSET_DEFENSE_20060515

	// [�������׼���] �����Ǹ��� ���׷��̵�� ������ ��� ���� (hjlim)
	#define MODIFY_UPGRADE_DARKLORDMANTLE_20060515
#endif

	// [�������׼���] ������ ���� ���� ���� ��ǥ �缳�� ���� (RKFKA)
	#define MODIFY_LOGINUSER_SET_REGEN_COORD_AT_CASTLE_MAP_20060511

	// [�������׼���] ���� ����� ��� ��� ��ü �Ұ� (RKFKA)
	#define MODIFY_CASTLE_OWNER_GUILD_DESTROY_LIMIT_20060418

	// [�������׼���] ������ NPC �ʱ�ȭ ���� ���� (RKFKA)
	#define MODIFY_CASTLE_NPC_INITIALIZE_BUG_20060406

	// [���׼���] �����ÿ� ������ �ڷ���Ʈ ��ų �� �ִ� ���� ���� (hnine)
	#define MODIFY_TELEPORT_SKILL_USING_PARTY_MEMBER_20060504

	// [���׼��� ��Ʈ��] - �޼��� ���۽� ���ڿ� ���� ��� ���� ���� (RKFKA)
	#define MODIFY_VIETNAM_MEMCPY_BUG_20060502

	// [������Ʈ] ���� ������ ���� ���� (hjlim)
	#define UPDATE_SHIELD_CALCULATE_GAGE_20060428

#ifdef ADD_SHIELD_POINT_01_20060403
	// [����߰�] ���� �ý��� �޺� ���� �ɼ� & ���� ���� Ȯ�� (hjlim)
	#define UPDATE_SHIELD_OPTION_20060427
#endif
	
	#ifdef FOR_ONAIR
		// [����߰�] ��ۿ� ���� ���� ��ũ�� �߰� (hnine)
		#define ADD_MACRO_FOR_SETITEM_20060424
	#endif

	// [���׼���] ���̵� ��ȣ �ӷ� â���� Connect �� ���� ��� ������ ���� �ذ� (apple)
	#define MODIFY_CONNECTTIME_CHECK_20060424

#if GAME_VERSION >= G_V_S2_2
	// [����߰�] ���Ǵ�Ƽ �ַο� ��ų ���� �߰� MP �Ҹ� ���� ��ũ�� (RKFKA)
	#define ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
	// [������Ʈ] CSkillAdditionInfo �߰� (RKFKA)
	#define ADD_SKILL_ADDITION_INFO_CLASS_20060407
	// [������Ʈ] ����� ��ų '���Ǵ�Ƽ �ַο�' �߰� (RKFKA)
	#define ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406

	// [������Ʈ] Į���� NPC ���̶� �߰� (RKFKA)
	#define ADD_SHOP_KALIMA_NPC_REIRA_20060403
#endif

	// [������Ʈ] ī������ �̺�Ʈ( ī����ĳ��, ����������, ����ĳ��) ���� ���� (RKFKA)
	#define ADD_PK_USER_ENTER_LIMIT_EVENT_MAP_20060411

#if GAME_VERSION >= G_V_S2_2
	// [���溸��] ������ ������ ������ ���� (RKFKA)
	#define ADD_CRYWOLF_SUCCESS_REWARD_20060404		
	// [���溸��] ���ܿ� ����ϴ� ���� ĳ������ ���� ���� 350 -> 260 ���� (RKFKA)
	#define MODIFY_CRYWOLF_ELF_ALTAR_LIMIT_LEVEL_CHANGE_20060404
	// [���溸��] ũ���̿��� ���� ��ũ����, �߰��� ������ ���	(RKFKA)
	#define ADD_CRYWOLF_MONSTER_ITEMDROP_20060404
	
	// ���� �ý��� ����� �޽��� ��� (����) : �系 ���߼��������� Ȱ��ȭ.
	//#define SHIELD_POINT_DEBUGLOG

	// [������Ʈ] ���� �ý���(����Ʈ) �߰� (hnine)
	#define ADD_SHIELD_POINT_01_20060403
#endif

	// [���׼���] ��ų ���� ���� üũ �߰� ���� (hjlim)
	#define MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421

	// [���׼���] ��ų ���� ���� üũ ���� �� ���� (hjlim)
	#define MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	
	// [���׼���] ��ų ���� ���� üũ (hjlim)
	#define MODIFY_SKILL_CHECK_DISTANCE_20060413

	// [����߰�] ���Ӽ��� ���� �߰� (apple)
	// �׽�Ʈ/���� ��������, ���� ��������� ���� ���� ���� �����ֱ�
	#define ADD_SERVER_INFO_DISPLAY_EXTEND_01_20060313

	// �� ���� ���������� �޾Ҵ��� üũ �Ѵ�. (Server Dev.)
	#define	BLOCK_INVALID_CHARACTER_DELETE_01_20060327

	// [���׼���] ��������(���귯��) �̺�Ʈ�� ������������ ������� ���� (apple)
	#define REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319

	// [��ɼ���] ũ���̿��� ���� ü���϶� �ڵ�ȭ & ��� �ڻ� ���� �ɼ�ȭ (apple)
	#define MODIFY_CRYWOLF_BENEFIT_01_20060315

	// [���װ���] ī����ĳ��, ����ĳ�� ���� ���� �α� ( RKFKA )
	#define ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314

	// [����߰�] FSGate ���� �� ������ Option Reload ���� (apple)
//	#define ADD_SERVER_FUNC_OPTION_RELOAD_CHECK_01_20060313

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ȭ��Ʈ���� �������� �̺�Ʈ ( RKFKA )
	#define WHITEDAY_CANDY_BOX_EVENT_20060222

	// [�α��߰�] ��� ������ ��� �α� �߰�( RKFKA )
	// ADD_KUNDUN_ITEM_DROP_LOG_20060309
#endif

	// [������Ʈ] ���ö���¡ ������(.wtf) �߰�
	// 0.99.60 - 0.99.94
	#define MODIFY_LOCALIZING_WTF_20060309

	// [���׼���] Į���� �̵� ���� ���� (hnine)
	#define MODIFY_KALIMAMOVE_BUGFIX_20060221

	// [���׼���] �������� ���� ������ ���� ���� (hnine)
	#define MODIFY_MAGICIANRING_DURATION_BUGFIX_20060221

#ifdef ADD_KUNDUN_CONTROL_20040923
	// ��� �̺�Ʈ ���� �α� ���� ���ϸ� ����(�ӽ�) (hnine)
	#define MODIFY_KUNDUN_EVENT_LOG_FILENAME_20060220
#endif

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] �α� �ý��� Ȯ�� 20060202 (hnine)
	#define EXTEND_LOG_SYSTEM_20060202

	// [���׼���] ��� �������� Į������ �ƴ� �ʿ��� ����Ǵ� ���� ���� (RKFKA)
	#define MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
#endif

	// [������Ʈ] ����ġ ��� ��� �� ��Ƽ/��Ʈ ����ġ ���� (hnine)
	#define MODIFY_GETTING_EXP_RATE_20060214
	// ����ġ ���� ���� ���� ���� MODIFY_GETTING_EXP_RATE_20060213 ���� (hnine)
	// MODIFY_GETTING_EXP_RATE_20060213 (RKFKA)
	// [������Ʈ] ����ġ ȹ�� ���� (apple)
	//#define MODIFY_GETTING_EXP_RATE_20051116

	// [�α׼���] 1) ��� ���� �α� ���� KALIMA(6) -> KALIMA(7)	
	//				2) 6Į������ �ִ� ��� �������� �α� �κ� �ּ�ó�� (RKFKA)
	// MODIFY_KUNDUN_LOG_20060201

	// [���װ���] ��ȿ���� ���� �������� ���� �η��þ� �̵� ���� �α� (apple)
	#define ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131

	// [���װ���] ����ġ ȹ�� ��, ��ǥ ���� �߰� (apple)
	#define ADD_LOG_GETTING_EXP_MAP_X_Y_INFO_20060131

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] �߷�Ÿ�ε��� ���� �̺�Ʈ (RKFKA)
	#define VALENTINES_DAY_BOX_EVENT_20060124

	// [���׼���] �渱 ������ ���� �Ǵ� ���� ���� (apple)
	#define	MODIFY_FENRIR_ZEN_REPAIR_20060124
#endif

#ifdef PERSONAL_SHOP_20040113
	// [���׼���] �ʼ��� �̵� �� ���λ��� ������ ���� �߻� (apple)
	#define MODIFY_ITEM_DUPLICATION_IN_MAPSERVER_MOVING_20051219
#endif

	// [���׼���] ���� ���࿡ �ٸ� ������ SOMMON ���� �ʴ´�. (apple)
	#define MODIFY_DARKLORD_RECALL_IN_CASTLESIEGE_TIME_20051219

	// [���׼���] ��ȣ������ �ı� ���� �ʾ�����, ���� ����� �� �� ����. (apple)
	#define	MODIFY_CASTLE_CROWN_ACCESS_BY_CHECKING_STATUE_20051219

#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] ũ�������� �������� �̺�Ʈ (RKFKA)
	#define CHRISTMAS_RIBBONBOX_EVENT_20051212	
#endif

	// [������Ʈ] ������ ���� ���� ���� (hnine)
	#define UPDATE_LOWLEVEL_SUPPORT_1_20051215				
	
	// [���׼���] ���� ���� �ƴϸ� NPC�� �̾߱� �Ҽ� ����. (apple)
	#define MODIFY_NPCTALK_REQUIREMENT_20051212
	
#if GAME_VERSION >= G_V_S2_2
	// [������Ʈ] �ű� ���� AI �߰� (apple)
	#define ADD_NEW_MONSTER_SYSTEM_02_20050725	// Monster AI

// �١� ũ���̿��� (apple)
#ifdef GAMESERVER_TO_CRYWOLFSERVER
	#define CRYWOLF_PRECEDENCE_PATCH_20050413
	#define CRYWOLF_COMMON_PATCH_20050413
	#define CRYWOLF_ONLY_PATCH_20050413
#else
	#define CRYWOLF_PRECEDENCE_PATCH_20050413
	#define CRYWOLF_COMMON_PATCH_20050413
#endif

	// [������Ʈ] �ű� Ż�� �渱(Fenrir) �߰� (apple)
	#define	ADD_ITEM_FENRIR_01_20051110
#endif

	// [��Ƽ��ŷ] �ø��� 0 ���� ���� �� �ŷ��� ���� ó�� (apple)
	#define ANTIHACKING_SERIAL_0_ITEM_20051202

	// [���׼���] ���� ������ ���� ������ ��� Rand() -> GetLargeRand()�� ���� (apple)
	#define MODIFY_MONSTER_RANDOM_POS_CREATION_20051202

	// [���׼���] �ʻ󿡼� ĳ������ �������� ���� ���� ���� ���� (apple)
	#define MODIFY_CHARACTER_DELETE_IN_MAP_20051121

	// [���׼���] (�߱�) ������� 117���� ä�ø޼��� ���� �� �� ���� ���� (apple)
	#define PREVENT_117_CHATTING_MSG_BUG_20051114

	// [�α��߰�] ������ ����, ���� ��¥ DB ������Ʈ ���� �α� �߰� (apple)
	#define ADD_CASTLESIEGE_DATE_UPDATING_LOG_20051103

	// [���׼���] ��ũ�ε� ��ȯ 5�� ī��Ʈ �� ���ϸ��� ���� ���� ���� ���� (apple)
	#define MODIFY_DARKLORD_RECALL_BUGFIX_20051103 

	// [���׼���] ������������ ���Ź��� ����� ��&���� ������ ������ (apple)
	#define MODIFY_CASTLESIEGE_VIEWPORT_BY_CANGE_RING_20051103

	// [���׼���] ������������ ���Ź����� �������� �� �ڽ��� ����Ʈ�� â/���� ������ ������.. (apple)
	#define MODIFY_VIEWPORT_REMOVE_CHANGE_RING_20051013

	// [���ϼ���] �η� ���� �̺�Ʈ �޼����� wtf ���Ϸ� ���� (apple)
	#define MODIFY_CASTLEDEEP_EVENT_MESSAGE_20051013

	// [��������] ���� ��� �� �հ� ����ġ ����� ������ �˻��ϰ� ���� ���� �Ǵ� (apple)
	#define MODIFY_CASTLE_CROWN_ACCESS_ALLOWED_BY_CROWNSWITCH_20051102

	// [��������] �������� �� ��ȣ������ ���� ��� ������Ű�� ��ƾ ���� (apple)
	#define REMOVE_CASTLESIEGE_AUTO_REVIVE_GUARDIANSTATUE_20051101

	// [��������] ��ũȣ���� �����ϰ� ���� ��� ���� ��� �����ʹ� �з����� �ʴ´�. (apple)
	#define MODIFY_CASTLESIEGE_GUILDMASTER_BACKSPRING_20051018

	// [��������] �հ� ����ġ ����� ������ �˸���. (apple)
	#define MODIFY_CASTLESIEGE_NOTIFY_CROWNSWITCH_USER_20051017

	// [��������] ���� ���� ���� ��� ���� 5->10���� ����, ���� �ð� Ȯ�� (apple)
	#define MODIFY_CASTLE_POTION_20051017

	// [��������] ���� ��� �ð� ���� �ý��� (apple)
	#define MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014

	// [��������] ���� ��尣 ������ ���� �ĺ� �ý��� (apple)
	#define MODIFY_CASTLESIEGE_IDENTIFICATION_20051012




	//----------------------------------------------------------------------------------------------------------
	//
	//	�� OLD Style..
	//
	//----------------------------------------------------------------------------------------------------------


	// T_CurCharName �� ĳ���� ������ ���� �ʴ´� (apple)
	#define REMOVE_CONNECTED_CHARACTER_NAME_INFO_20051004

	// �¸Ŵ��� ��ü���� �߻� �� �α� ��� (apple)
	#define	ADD_LOG_GM_TOOL_NOTICE_20051004

	// ���� �����̻� ���� : ���̽����ο�� ���� �������� ���ߴ� ��ų ����! (apple)
	#define MODIFY_MONSTER_ACT_BY_ICEARROW_20050914

#if GAME_VERSION >= G_V_S2_2
	// ������ �������� �̺�Ʈ (b4nfter)
	#define HIDDEN_TREASUREBOX_EVENT_20050804

	// �ѱ� ���� IP�ּ� ���� (b4nfter)
	#define MODIFY_WEBZEN_PUBLIC_IPADDRESS_20050822

	// ���ο������ �� Ȱ, ���� ��ġ ���� ���� (apple)
	#define MODIFY_APPLYING_BOW_01_20050825

	// �ű� ������ �߰� (apple)
	#define ADD_NEW_ITEM_01_20050823
	
	// ���̳ʽ� ���� ���� �߰� (apple)
	#define ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
#endif

#if GAME_VERSION >= G_V_99B
	// item.txt ���� Ȯ�� (apple)
	// < �ʿ�䱸ġ Ȯ�� >
	//	��,��ø,������ -> ����,��,��ø,������,ü��,���
	#define EXPAND_ITEMSCRIPT_01_20050808
#endif

	// �ַο������ ȭ�� +2 �ۿ�� ������ ���� (apple)
	#define MODIFY_ARROWVIPER_2PERCENT_DAMAGE_20050816

#if GAME_VERSION >= G_V_99B
	// ExDB ģ�� ��� ON/OFF �߰� (apple)
	#define ADD_FRIEND_FUNCTION_ON_OFF_20050811

	// �Ķ� ���ָӴ� ��� �� '�̵� Į����' ��� ��� ���� ����
	#define MODIFY_KALIMAMOVE_COMMAND_FOR_BLUEPOUCH_20050811

	// �������ε��� Ȯ������ ���� �������Ʈ ���� (apple)
	// ���� ������ ���� ���´�.
	// �������� ��Ʈ������ ����� ����
	#define ADD_RANDOM_RANGE_OF_ITEMINDEX_20050808

#if GAME_VERSION >= G_V_S1_512
	// ���� Į���� �߰� (b4nfter)
	#define HIDDEN_KALIMA_20050706
#endif

	// ��Ƽ�� ���� �� �α� ���� ���� (b4nfter)
	#define	PARTY_ADD_LOGFIX_20050801

	// ����� ����ġ�� ���� �ʴ´� (apple)
	#define DISABLE_GETTING_EXP_FROM_KUNDUN_20070801
#endif

	// ���Ӽ��� �Ķ���� �߰� (DataServerIp2, DataServerPort2, ExDbIp, ExDbPort) (b4nfter)
	#define ADD_GAMESERVER_PARAMETER_20050728

#if GAME_VERSION >= G_V_S1_512
	// ������ �ε��� Ȯ�� (b4nfter)
	#define ITEM_INDEX_EXTEND_20050706

	// DBũ�� Ȯ�� (b4nfter)
	#define ITEM_DBSIZE_EXTEND_20050706

	// ���̴� �� �߰� (apple)
	#define ADD_NEW_MAP_AIDA_20050617

	// ���ο� ���� �ý��� �߰� (apple)
	#define ADD_NEW_MONSTER_SYSTEM_01_20050714	// Monster SKILL

	// EXDB���� ���밡 �ִ� ���¿����� �ٸ� ���븦 ���� ���ϵ��� ��! (apple)
	#define MODIFY_EXDB_RIVAL_CHECK
#endif

	// ���� ��� ��� wtf �� ����(apple)
	#define MODIFY_CASTLESIEGE_GM_MESSAGE_20050722

	// ��Ʋ��Ŀ�� �̿��� PK �ȵǰ� �ϴ� ���� ���� (apple)
	#define MODIFY_BATTLESOCCER_PKCHECK_20050714

	// ��ũ�ε� �ִ� �������� ���� ���� ����(apple)
	#define MODIFY_DARKLORD_CREATION_MAXSTAT_USING_FRUIT_20050712

	// �ູ, ��ȥ�� ���� ���� ���� ��� ���� ����� ����. (apple)
	#define MODIFY_BLESS_SOUL_POTION_CREATION_BUG_20050711

#if GAME_VERSION >= G_V_S1_512
	// ��õ���� ���� ���� �߰�. item.txt ���� �ʿ�(apple)
	#define	ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706

	// ��� ä�� / ���� ä���� �����׷������ �����ϰ� �Ѵ�. (apple)
	// GS <-> ExDB <-> GS ���� ������ ��ȯ
	#define ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706

	// �����,��Ʋ��Ŀ�� �ڽ��� ��忡 �Ǵ� ���� ����. (������ ���� ��_��) (apple)
	#define MODIFY_GUILDWAR_REQUEST_PROCESS_20050704

	// WhatsUp�� Console ȭ�� �󿡼� ������ �̻��� üũ�Ѵ�. (apple)
//	#define ADD_SERVER_ALERT_USING_WHATSUP_01_20050628

	// ĳ���� ��å ��ų ����. skill.txt ���� �ʿ�(apple)
	#define ADD_NEW_SKILL_FOR_CASTLE_02_20050531		
	
	// ���� ĳ��, ī���� ĳ��, ���忡���� SHADOW PAHTOM�� ����� ���ش�(apple)
	#define MODIFY_NPC_SHADOW_PAHTOM_20050628

	// ������ ������ ��� �ִ� ���� ���� (u4us)
	#define MODIFY_DROP_MAX_ITEM_LEVEL_6_OF_MONSTER_20050624
	// ���� �� ����� ���� 5 �̻� ���� ��� ���� (u4us)
	#define MODIFY_DROP_PREVENT_OF_RING_N_NECKLACE_LV_5_OVER_20050623
	// ĳ���� ���ݷ� & ���� ��� ���� NPC Shadow Phantom �߰� (u4us)
	#define ADD_NPC_SHADOW_PHANTOM_20050616
	// ĳ���� �ڵ� ȸ�� �ý��� (u4us)
	#define CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	// ��� ������ ���� 0 - 1 ���� ��� ȸ�� (u4us)
	#define IMMEDIATELY_HP_MP_AG_RECUPERATION_20050614
#endif

	// [�ҽ�����] Shop15�� ���� �߰� ���� -> ADD_SHOP_15_20050613 ����(RKFKA)
	// ������ �ʿ� �����߰� -> WzVersionManager, WzAuthGs, WzFsGate ���� ���� (b4nfter)
//	#define	ADD_SHOP_15_20050613

	// �ְ� ������ �Ǹ� ���̻� ����ġ�� ���� ���Ѵ�.
	#define	MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613

	// �������� ����� ������ ����� ���� �����ϴ� ���� �ǵ��� ���� (apple)
	#define MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610

	// �̺�Ʈ ���(GM) ���� �߰� (u4us)
	#define ADD_AUTHORITY_CODE_OF_EVENT_GM_20050602	

#if GAME_VERSION >= G_V_S1_512
	// ���������� Ȯ�� -> ���� 4������ 6�� (b4nfter)
	#define DEVILSQUARE_EXTEND_20050221
#endif

	// [�������׼��� 1��] ��ũ�ε� PET ���� ����ġ ���� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_DARKLORD_PET_EXP_01_20050603
	// [�������׼��� 1��] ������ ǥ�� �ǸŰ��� ���� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_MARKOFLORD_SELL_VALUE_01_20050531
	// [�������׼��� 1��] ĳ��Ÿ���� ���� ��� ����ġ �϶� ���� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_DEAD_BY_CANNONTOWER_01_20050531
	// [�������׼��� 1��] ���������� ��Ƽ �� ���� ���� �ǵ��� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_SWELLLIFE_ON_PARTY_01_20050531
	// [�������׼��� 1��] ��������, �����ǹ��� �ڻ� ���� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_AMULETRING_WRONG_OPTION_01_20050531
	// [�������׼��� 1��] ������ ���� +10~13 ���� ���� ����. (apple)
	#define MODIFY_ACCUMULATED_BUG_SLEEVEOFLORD_01_20050531

	
	// ������ ������ ������ MAXHIT ������ �Ѵ�. (apple)
	// ������ ������ ������ LastHit ������ �Ѵ�. (apple)
	#define MODIFY_ITEM_LOOTING_FROM_EROHIM_20050610
	#define MODIFY_ITEM_LOOTING_FROM_EROHIM_20050530

	// ���� ĳ�������� ��Ȱ�� �ڽ��� ����Ʈ ���� ���� (u4us)
	#define BUGFIX_UPDATE_VIEWPORT_INFO_FOR_SELF_20050510

#if GAME_VERSION >= G_V_S1_512
	// �ʼ��� ���� �߰�����
	#define	MAP_SERVER_ADDINFO_20050509
#endif

	// ���� ������� �������� �������� ����ġ�� ���� �ʴ´�. (apple)
	// ����ġ ���׷� ���� ���� ��.
	#define DISABLE_GETTING_EXP_FROM_EROHIM_20050509

	// NPC ��ȭâ ���� (Ŭ���̾�Ʈ ��û) (b4nfter)
	#define DISABLE_NPC_MESSAGE_20050506

#ifdef DARKLORD_WORK
	// ��ũ�ε��� ���� ���� ������ �ǸŰ��� ���� ���� (u4us)
	#define BUGFIX_SELL_PRICE_OF_DARKLOAD_PETS_20050310            
#endif

	// [��������] ���� ���� �ٸ� ��忡 ���� ���� �Ҽ� ����. (apple)
	#define MODIFY_UNION_JOIN_CASTLE_OWNER_GUILD_20050502

	// ī���� ���� ���� & ���� �߰� (u4us)
	#define ADD_FRUIT_N_WING_OF_CHAOS_MIX_20050502    

#if GAME_VERSION >= G_V_S1_512
	// ���� ���� �ý��� (b4nfter)
	#define JEWEL_MIX_SYSTEM_20050309

	// [��������] ���� ��帶���� ��� ���� ���� ��� �߰� (apple)
	#define ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418

	// [��������] ���� ��� �� ���� (7->3) (apple)
	#define MODIFY_MAX_UNION_MEMBER_01_20050418
#endif

	// ���λ��� ������ ���� ��ġ
	#define	BUGFIX_PERSONALSHOP_ZENCOPY_20050426

	// � ĳ�� ��ɾ�ó�� �α� �߰� ����. (apple)
	// ��� ��� ���� ���� �߰�.
	#define ADD_LOG_GM_COMMAND_20050426

	// ���� �� �̺�ƮĨ ���� �õ� ���� �� �α� ��� (u4us)
	#define BUGFIX_EVENT_CHIP_DUPLICATE_PREVENT_20050422  

	// MaxUser �� Commonserver.cfg���� ���� ��� �߰�. (apple)
	// Commonserver.cfg �߰� ���� :
	// 1) NumberOfMaxUser = 500		
	// 2) NumberOfCastleSiegeMaxUser = 1500
	#define ADD_CONTROL_MAXUSER_20050420
     
	// X �ֹε�Ϲ�ȣ üũ�� ���� (u4us)
	#define BUGFIX_X_RESIDENT_REGISTRATION_OF_USER_20050419            

	// ���� ���̵� ũ��Ƽ�� ������ ó�� �߰� (u4us)
	#define ADD_CRITICAL_DAMAGE_OF_DEATH_BLADE_20050419 

	// Send Buffer Ȯ��. (apple)
	#define EXPAND_MAX_SENDBUFFER_20050413
	
	// #LOGADD_CASTLESIEGE_20050404 (b4nfter)

	// ����̸� ���� (b4nfter)
	#define	GUILDNAME_MEMSET_20050410

	// �������� ���� �� ����� ���� ���� ���� (b4nfter)
	#define ITEM_GET_BUGFIX_20050406

	// ��ü ���� �� ���ִ� ��ġ�� �Ӽ��� �������� (b4nfter)
	#define	CLEAR_STANDATTR_OBJDEL_20050325

	// PK�� �����Ӱ� �� �� �ֵ��� ���� (b4nfter) -> ServerInfo.dat �� PkLimitFree = 1 �� �Ǿ�� ����
	#define	ADD_PK_LIMIT_FREE_OPTION_20050324

	// ���λ��� �κ��丮 �ʱ�ȭ �� �޸� ħ������ �߻����� �������� (b4nfter)
	#define	PSHOP_INVENTORY_CLEAR_BUGFIX_20050321

	// ���� �� �κ��丮�� ������� ���� ���� (b4nfter)
	#define INVENTORY_CLEAR_BUGFIX_20050226

	// ��� �̺�Ʈ ���ῡ ���� ���� ���� (apple)
	// 1) ������ ��� 5->2���� ����
	// 2) ��� HP �޼��� ����.
	#define	CHANGE_KUNDUN_SETTING_01_20050221

	// Ư�� ���� �׾��ٴ� ���� Q ó���� �켱���Ѵ�. (apple)
	#define ADD_QPROCESSING_STATE_OF_DEAD_20050221

	// DB������ �����Ϳ��� �������� ���� ���� üũ��� 
	// �ܼ��� �迭 ù�ڸ��� 0xFF ��� ���� ������ �Ҷ� 9��Ʈ ������Ÿ�� ���Ŀ��� 0xFF (-> 7/31) �������� ������� ��� ���� �߻�
	#define INVENTORY_EMPTYSLOT_CHECK_20050217

	// ���� ĳ��, ���������� ���� ���� ����
	// �ѱ����� ��Ȱ��ȭ �Ǿ� �ִ����� Ȱ��ȭ ��Ŵ. (20050216) (apple)
	#define BLOOD_DEVILSQUARE_TRADE_BUG_FIX

	// ĳ���� ���� â���� ĳ���� ������ ������ �˼� �����Ƿ� �ش� �ڵ� ����(apple)
	#define DEL_CODE_LIMIT_OF_DEL_CHARACTER_LEVEL_20050215
	
	// ģ�� ����Ʈ + ���� ����Ʈ ��û ���� ����.
	// �ʼ��� ����� ����.	(b4nfter_apple)
	#define MODIFY_ORDER_REQUEST_FRIENDLIST_20050205

#ifdef PERSONAL_SHOP_20040113
	// ���� 5 ������ �ɸ��͵��� ��� ���� ���� �� ���λ��� ������ �ʱ�ȭ �ϰ� �����Ѵ�.
	#define PERSONAL_SHOP_DELETE_UNDER_LV5_20050203
#endif

	// EXDB DB ��ü ����ȭ�� ���� ����. (apple)
	// EXDB Server���� �����.
	// ExDB Server : 0.40.G�� �����.
	#define MODIFY_EXDB_SYNC_01_20050128		

#if GAME_VERSION >= G_V_S1_512
	// ĳ���� ����Ʈ ������ ��� ������ �߰��Ѵ�. (apple)
	// Dataserver���� ����ȴ�. : 0.55.09T ���� ����
	// ������ Dataserver�������� ����!!
	#define ADD_CHARACTERLIST_GUILD_STATUS_01_20050126
#endif

	// ī���� ���ս� �˾� �޼��� ���� �ʵ��� �޼��� ���� (apple)
	#define REMOVE_CHAOSMIX_NPC_MESSAGE_20050125

	#define ADD_OPTION_RELOAD_ITEM_TO_JOINSERVER
	
	// �������� ���� �����ü, ���ղ��� ���Ѽ���. (apple)
	// ExDB Server�� �����.
	// ExDB Server : 0.40.F �̻� ����
	#define EXDB_APPLY_CASTLEUPDATE_01_20050111

	// �̺�Ʈ �Ŵ��� (��ü �̺�Ʈ�� Ư�� �ð����� �����Ͽ� ���������� �����Ű�� ���� ��� ���) (b4nfter_apple)
	#define EVENT_MANAGEMENT_20040711

#if GAME_VERSION >= G_V_S1_512
	// ĳ�������� Class ���� ������ (apple)
	#define ADD_CLASSINFO_ON_CHARCTER_CREATION_20041224

	// ���� ��ũ ��� (apple) - EXDB ���� �ʿ�
//	#define ADD_UNION_GET_UNIONMARK_01_20041222
	
	// ���� �̸� ��� (apple)
	#define ADD_UNION_GET_UNIONNAME_01_20041222
#endif

	// �������� ���� �� ���μ��� ���ᰡ �ƴ� 1�� ���ᰡ �ǵ��� ����
//	#define	MODIFY_AUTHSERVER_CLOSE_20041207

	// �׼���Ʈ ������ ������ ���� (apple)
	#define MODIFY_EXCELLENT_ITEM_DUR_01_20041202
	
	// ���� ������ ��� �α׿� ���� �̸��� �߰��Ѵ�. (b4nfter)
	#define	MONSTER_ITEMDROP_LOG_ADD_NAME_20041202

#if GAME_VERSION >= G_V_S1_512
	// �ŷ� �� ���� ���� �ʱ�ȭ ����. (apple)
	#define MODIFY_NEW_UNION_02_20041201	

	// ���� ä�� ��� (apple)
	#define ADD_UNION_CHATTING_01_20041201
#endif

#if GAME_VERSION >= G_V_99B
	// �Ŵ������� ���Ϻ����� (foguy)
	#define ADD_MANAGER_SENDMAIL

	// ������ �ٸ� ���� ���� �Ҽ� �ִ� �� ���� (apple)
	#define ADD_NAMECHECK_DISMISS_GUILDMEMBER_20041129

	// ������ ī����ĳ��, ����ĳ��, �Ǹ��Ǳ��忡�� �� �� ����. (apple)
	#define ADD_MAPCHECK_OPEN_KALIMAGATE_20041129
#endif

	// ī����ĳ�� ����ð� ǥ�� ���� ���� (apple)
	#define MODIFY_OPENTIME_CHAOS_CASTLE_20041129
	
	// ��ũ�� ���� ���� ���� (apple)	
	#define MODIFY_PRICE_FEATHE_OF_LOCH_20041129

#if GAME_VERSION >= G_V_S1_512
	// �ʼ��� ���� �۾� (b4nfter)
	#define MAP_SERVER_WORK_20041030				
#endif

	// üũ�� ������ ������ ĸ�ǹٿ� ������ �����ش�.(apple)
	#define ADD_DISPLAY_CHECKSUM_INFO_20041127

	// GM ��� User State ��ũ�ε� �߰� (apple)
	// 20041127 message_kor.wtf ����Ǿ�� �մϴ�~! 
//	#define MODIFY_GMCOMMAND_USERSTAT_20041127

	// �븸 �����н�����(â���й�ȣ) ���� (apple)
	#define MODIFY_GAMEPASSWORD_20041126

	// ��å �Ӹ� ���� ����. (apple)
	#define MODIFY_ASSIGN_GUILD_STATUS_20041123

	// ������ ���� ������ ���� ��� ����. (apple)
	// DataServer, ExDB Server�� �����.
	// !!���� - Commonserver.cfg�� CountryCode �� �߰� �Ǿ���.
	// !!���� - �ؿ� ���չ����� �����. 
	// !!���� - DataServer : 0.55.07 ExDB Server : 0.40.B
	#define MODIFY_LOAD_DATAFILE_FOR_DBSERVER_20041117

	// ��ũȣ��, ���Ǹ� ���� ���� (apple)
	#define MODIFY_DARKLORD_PETITEM_CHAOS_MIX_20041117

#ifdef DARKLORD_WORK
	// ��ũ�ε� ���� ���� ���� (apple)
	#define MODIFY_DARKLORD_ACQUIRING_EXTRA_POINT_20041117
#endif

#ifdef CASTLE_SIEGE_COMMON_PATCH_20041018
	// ���� �غ� ���� �̺�Ʈ(�������� ��� �Ͻ��� �̺�Ʈ) (apple)
	#define ADD_RENAEVENT_FOR_CASTLE_SIEGE_20041116
#endif

#ifdef DARKLORD_WORK
	// ���� ���� ����� �� ��� �� �ִ� �������� üũ.. (apple)
	#define ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
#endif

#if GAME_VERSION >= G_V_S1
	// ������ �۾� (b4nfter)
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
	// �������� ���
#define CASTLE_SIEGE_COMMON_PATCH_20041018		// ������ ������ �Ϲ� �������� ����Ǵ� ��ġ�κ�
#define CASTLE_SIEGE_ONLY_PATCH_20041018		// ������ ������ �ʿ��� ��ġ�κ�
#else
	// �Ϲݼ��� ���
#define CASTLE_SIEGE_COMMON_PATCH_20041018		// ������ ������ �Ϲ� �������� ����Ǵ� ��ġ�κ�
#endif
#endif

#if GAME_VERSION >= G_V_S1
	// New Guild System	(apple)
	// �ش� ���������� �����ϱ� ���ؼ��� ExDB�� 40.C �̻� �������� ���� �ؾ� ��.
	// [C   <-> GS] 0xE0 ~ ���� ���.
	// [EDS <-> GS] 0xE0 ~ ���� ���
	#define MODIFY_NEW_UNION_01_20041122		// �ŷ� �� ��� ���� �ʱ�ȭ ����.

	#define ADD_NEW_UNION_02_20041118	// ���� ����
	#define ADD_NEW_UNION_01_20041006	// ����
	#define ADD_NEW_GUILD_02_20040922	// �뺴
	#define ADD_NEW_GUILD_01_20040913	// ��å
#endif

	// ���� ��ʸ�Ʈ �̺�Ʈ
//	#define WORLD_TOURNAMENT_EVENT_SETTING
	
	// ���� �����ɼ� ���� ����(������ �����ɼ��� ���� �Ǵµ� ������ ���� ����)
	#define SLEEVEOFLORD_OPTION_BUG_PATCH

	// �߱� ī���� ������ �̿��� ���� ���� ����
	#define CHAOS_BOX_ITEMCOPY_FIX_20041112_FOR_CHS

	// �渶���� �ڷ���Ʈ �� ����Ʈ�� ���̵��� �ɸ��� �����ִ� ���� ���� (b4nfter)
	#define BUGFIX_WIZARD_TELEPORT_MAGIC_20041018

	// ���� ������ ���ۺ����� ���� (b4nfter)
//	#define MODIFY_SERVER_NOTICE_BUFFERCOPY_20041018

	// WzJoinServerCli.cpp ���� Send() �� ���۰� ������ ���̴� ���� ���� (�׵��� DB���� ���� ������ ���� ������ �������� ������) (b4nfter)
//	#define	BUGFIX_WSJOINCLIENT_SENDLENGTH_20041011		

	// GS -> DS ������ �������� LIVE��ȣ ����
//	#define GS_DS_LIVECHECK_20041008

#if GAME_VERSION >= G_V_99B
	// (1) ���Ӽ��� ���׷��̵� (apple)
	// : ���� OBJ �淮ȭ
	#define UPGRADE_GAMESERVER_01_20041006

	// ���� �縮���� ���� ���� (b4nfter)
	#define BUGFIX_KALIMAGATE_VANISHING_20041006		// Į���� ����Ʈ ������� ���� ����

	// #BUGFIX_DARKSPRITE_DINORANT_DAMAGE_20041005		// ��ũ���Ǹ� ���� �� ����Ʈ �����ϸ� �߰� �������� ���� ����

	// ��� �� ���� �� ����͸��� ���� ��ɾ��߰� ��(apple)
	#define	ADD_KUNDUN_CONTROL_20040923

	// ���� HP ȸ�� (��� �̺�Ʈ�� ���� �߰�) (apple)
	// HP�� 20%�����϶� ���� ���� x ���� ��ȯ!
	#define ADD_ABILLITY_KUNDUN_01_20040920
#endif

#ifdef _NEW_EXDB_
	// ��� �ı� �� ����� ���� �ʵǴ� �� ���� (b4nfter)
	#define BUGFIX_GUILD_DESTROY_WAR_BLOCK_20040922
#endif

	// �޺� �������� PVP�� 50% ������ ���� �ǵ��� ����
	#define MODIFY_CC_PVP_COMBO_DAMAGE_LOWER_50_20040917

	// ���˻� �ƽ�����Ʈ ����. ���� ���� (apple)
	#define MODIFY_10_11_12_13_SET_DEFENSE_20040916

#ifdef ADD_KUNDUN_CONTROL_20040923
	// ����� ���� ���� ���� �� �α� �߰� (apple)
	#define MODIFY_KUNDUN_REWARD_01_20040915
#endif

	// Ư�� ���͸� �׿��� �� �������� �������� �������� ����Ѵ�. (apple)
	#define ADD_MONSTER_DIE_REWARD_ITEMS_20040915 

	// ���� ��ü ���� ���� (apple)
	// ������ ����ϴ� CGBeattackRecv ������� 4���̻��� Ÿ�ٿ��� ������ ���� ���ϴ°� ����.
	#define MODIFY_MONSTER_ENTIRE_ATTACK_20040915

	// Ư�� ���� �̸��� ���� 3������ ��� ó���� ���� ��尡��, ������ ����. (apple)
	// ExDB BUG FIX�� �߰�. ExDB 40.B �̻� �������� ���� �ؾ� ��.
	#define MODIFY_GUILDMEMBER_NAME_LIMIT_200040914

	// DSProtocol �� EDSProtocol �� ȥ���� �ִ� �ڵ� ���� (apple)
	// DSProtocol �ּ�ó��. -> ���� ���.
	#define	BLOCK_PREVIOUS_DSPROTOCOL_FOR_GUILD_20040913

#if GAME_VERSION >= G_V_99B
	// ��� Number 0x36(DB��������) �κ� ���� : WORD -> INT (apple)
	// ExDB DeadLock �ذ�
	// �ش� ���������� �����ϱ� ���ؼ��� ExDB�� 40.A���� ���� �ؾ� ��.
	#define	MODIFY_GUILD_VIEWPORT_03_20040913
#endif

	// ��ũ���Ǹ� "Ÿ������" ���� ���� Ÿ�� �������� ���� (apple)
	#define MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906

	// ����� ǥ�� ��� -> �� ǥ�� ��޺��� ó���ϵ��� ���� (apple)
	#define MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906

	// ��ũ�ε� ��Ƽ�� SOMMON ��ų �̵� ���� ���� (apple)
	// DARKLORD_WORK <-- ON �Ǿ� �־�� ��.
	#define MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831

#if GAME_VERSION >= G_V_99B
	// ���� ��� ����Ʈ �ý��� ���� (apple) 
	// ���� GuildNumber�� WORD�� �ν��ߴ� �κ�.(OLD ����Ʈ ���� ���� ���� ����)
	#define	MODIFY_GUILD_VIEWPORT_02_20040830
#endif

	// ������ ������� ���� ���� (b4nfter)
	#define ITEM_DUPLICATE_PREVENT_PATCH_BUGFIX_20040825

	// ConnectMember.txt�� �������� ������ (b4nfter)
	#define CONNECT_MEMBER_COUNT_INFINITE_20040823

#if GAME_VERSION >= G_V_99B
	// ĳ�����̵�..
	#define CHARACTER_MOVE_20040810
#endif

#if GAME_VERSION >= G_V_99B
	// ��ų�� ����/���� �Ӽ� �� (b4nfter)
	#define SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812

	// ��ũ�ε��� ���� �̺�Ʈ (b4nfter)
	#define DARK_LORD_HEART_EVENT_20040810
#endif

	// ����ĳ���� ��õ���� ���� ����� ���� �ϵ��� ���� (b4nfter)
	#define BLOODCASTLE_ITEMDROP_DIRECTLY_20040728

	// �ŷ� ���� ���� (â������ �κп� ����)
	#define	TRADE_BUG_FIX_CLOSE_WINDOWS_20040727

	// ���ο� ������ NPC_A Type �߰� (apple)
	//#define ADD_NPC_TYPE_A_01_20040708

	// ������ ���� ���� ��ġ (b4nfter)
	#define ITEM_DUPLICATE_PREVENT_PATCH_20040719

	// ���������� �Ϸ� ����� ���� (b4nfter)
	//#define DEVILSQUARE_CHECK_ENTERCOUNT_20040715
#if GAME_VERSION >= G_V_S1
	// Guild Viewport �۾� (apple)
	#define MODIFY_GUILD_VIEWPORT_01_20040701
#endif

#if GAME_VERSION >= G_V_99B
	// ���� ���� �� ���� (apple)
	#define APPLY_DURATION_MAGIC_HACK_CLEANER_20040504

	// �űԸ� - Į���� (b4nfter)
	#define ADD_NEW_MAP_KALIMA_20040518

	// ���Ӽ������� ���Ӽ����� �����ִ� �߰����� ������ [0xF4][0x07]
	//#define GAMESERVER_INFO_EXTENDED_20040625

	// ���� ���� ������ (apple)
	//#define ADD_PROPERTY_ITEM_SYSTEM_01_20040609 

	// PVP �ý��� ���� 1�� (apple)
	//#define ADD_PVP_SYSTEM_01_20040517	

	// ���� Ÿ�� BYTE -> WORD �� Ȯ�� (b4nfter)
	#define ENHANCE_MONSTER_TYPE_TO_WORD_20040618
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
	// ��� �׾��� ��� ��Ʈ ������ ���(apple) 
	#define ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
#endif

	// ���� ���� �����(foguy)
	#define	HP_POTION_VALUE_CHANGE_20040618
	
#if GAME_VERSION >= G_V_99B
	// ExDB ��� �۾� �����
	#define	EX_DB_GUILD_REWORK_RETOUCH
	// GuildClass �����۾�
	#define MODIFY_GUILD_CLASS_20040608
#endif

	// �׾��� �� ���, ������ ���� �ʶ������� �Ѵ� (b4nfter)
	#define USER_DIE_NOT_DROP_HERORING_20040330

	// ����ĳ�� ����� ���� 6 ���������� 7���忡 ������ �� �ְ� �Ѵ�. (b4nfter)
	//#define USER_ENTER_BLOODCASTLE7_WITH6_20040330

	// ������ ��ź, ������ �����ϰ� �����ϴ� �ڽ��� ü�°��ҷ� ���� �� ������ ī���� ����� ���� (b4nfter)
	#define USER_DIE_MAKE_OPPOSIT_PK_20040329

	// ĳ���� ����� ������ ������ ������ (b4nfter)
	#define USER_DIE_ITEMGET_BUG_PATCH_20040322

	// �׾��� �� ���� �����ϴ� ���� ���� (b4nfter) 
	#define USER_DIE_ZEN_BUG_PATCH_20040319
	// �׾��� �� �� �����ϴ� ��ġ ���� (b4nfter)
	#define USER_DIE_ZEN_MODIFY_20040318

	//////////////////////////////////////////////////////////////////
	// 2004�� 2�� 5�� ���� ��ġ ����
	// ������ ������� ��ġ (b4nfter)
	#define ITEMCOPY_BUGFIX_20040112	
#if GAME_VERSION >= G_V_99B
	// ��ģ�� ���� (saemsong)
	#define __FRIEND_WORK__	
#endif

#if GAME_VERSION >= G_V_S1
	// ��� ����Ʈ ���� (foguy)
	#define GUILD_VIEWPORT_MODIFY	
#endif

	// ���� ĳ�� ���������� ����� ������ ���� ���� ���� (foguy)
	#define BLOOD_DEVILUSQARE_TRADE_BUG_FIX
	// ĳ���� ����� ����ġ �ս� ���� (foguy)
	#define USER_DIE_EXPERIENCE_MODIFY
	// �̵� ��ɾ� ��ũ��Ʈ �߰� (foguy)
	#define	ADD_MOVE_COMMAND_SCRIPT
#if GAME_VERSION >= G_V_S1
	// ��Ƽ������ �ڽ��� ���� ������ ������ �ش� (foguy)
	#define	SEND_GETITEMINFO_FOR_MYPARTY
#endif
	// ��ũ�� ���� ��ī�罺������ �������� ����
	#define	FEATHEROFLOKE_ITEMDROP_CHECK
	//////////////////////////////////////////////////////////////////


	// ��� ������ ���� ���� ����
	#define CHINA_GUILD_POINTER_REF_ERROR

	// ī���� ���� -> ������ ���� �� �κ��丮 RollBack / Commit �ϵ��� ����
	#define CHAOS_MIX_TRANSACTION_BUXFIX_20040610

	// DbVersion �� 0 �� ����ڵ��� �������� �� ���λ��� ������ �ʱ�ȭ
	#define PERSONAL_SHOP_DBVER_0_FIX_200406016

	// ���λ��� �������� ���� ������ �����ϰ� ��� �ȱ� �� ��������� üũ
	#define PERSONAL_SHOP_CHECK_COPYITEM_200406016

	// ���� ĳ��, ���������� ���� ���� ����
	//#define BLOOD_DEVILSQUARE_TRADE_BUG_FIX

	// ī���� ĳ�� ���� �� ����� ��û���� �ȵǰ� ����
	#define	CHAOSCASTLE_DUAL_GUILDWAR_CHANGE_20040614
	// ���� ��ų �ϳ��� �ø��� �ϳ��� ���͸� �°� ����
	#define	CHECK_PENETRATION_SKILL

	//  ��Ƽ�� ������ ���� ���� ������� ���� �߰� 
	#define SEND_GETITEMINFO_FOR_MYPARTY_ADD_LEVEL

#if GAME_VERSION >= G_V_99B
	#define GUILD_REWORK		// ��� �ٽ� �۾��� ����(SaemSong)
#endif

	// ���� ������ ���� -> ���ϵ��� ���� (b4nfter)
	#define	ITEMDELETE_MODIFY_20040604

	// ���� ĳ������ ���� ���� �� ���� ���� �ο��鵵 ����ǵ��� ���� (b4nfter)
	//#define	BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	
	// ��Ƽ ��� ����� ����Ʈ �߿� ��� ���� �� ������ ����� ������ �� �� ���̴� ��. (b4nfter)
	#define PARTYMEMBER_LIFELIST_FIX_20040531


	// ���� �̵� ���� ���� Ŭ���̾�Ʈ�� ���� (NeedLevel = ((NeedLevel/3)*2) -> NeedLevel = ((NeedLevel*2)/3);
	#define MODIFY_MAGUMSA_MOVE_LEVEL_20040517

	// 2�� ���� ī���� ����Ȯ��(-454%) ���� ���� (apple)
	#define MODIFY_WINGMIX_454BUG_20040512

	// �ŷ��� �̿��� ������ ���� ���� ����(foguy)(���� �߱��� ������)
	//#define	TRADE_ITEM_COPY_BUG_FIX_20040511


	// ��Ʈ �����ۿ� ���� Ŭ����
	#define	SETITEM_BUFFER_CLEAR

	// ������ ������ ������ ä�ﲲ ���� ������, �װ͵� ä��� ���� ������ ���δ�
	#define	USE_PREV_HP_POTION

	//#define FOR_ANTI_HACK						// ���׽�Ʈ(������)
	// õ�� 1�� 
	#define MONSTER_SKILL						// ���� ��ų
	//#define RENA_CHANGE
	#define CHANGE_VIEWSTATE					// ViewStat BYTE���� WORD�� Ȯ��

	#define NEW_SKILL_FORSKYLAND				// õ���� ���� ���ο� ��ų

	#define SERVERCODE_EXPAND					// ���� �ڵ� Ȯ��

#if GAME_VERSION >= G_V_S1
	#define GAMESERVER_DIVISION					// ���� �����۾�
#endif
	// õ�� 1�� ��
	
	// õ�� 2�� 
	// ���� üũ�� 
	//#define MULTI_CHECKSUM
	// ���� üũ�� ��

	#define CHANGE_MAGICATTACKPACKET			// ���� ������Ŷ�� ������, ���� ���� ��ȣ ���, ��ų��ȣ�� �ް� ����

	#define EXP_CAL_CHANGE						// ����ġ ��� ��� ����
	// õ�� �߰� ������ ���� ������
	#define NEW_FORSKYLAND3		
	// õ�� �߰� ������ ���� ������ ��

	#define VER_CHATWINDOW_OPTION				// ä�ÿɼ� ����

	#define NEW_FORSKYLAND2						// õ�� 2���� ����...

	#define APPLY_PACKETCHECKSUM				// üũ�� üũ

	#define HACK_LOG
	// õ�� 2�� ��

	#define CHANGE_CLIENTHACKCHECK_MSG			// ��üũ ���� �޽��� ����						

	#define FOR_STONE_EXCHANGE					// ���� ȯ��
	#define FOR_LOTTO_EVENT						// ���ȭ 2�ֳ� ��� ���� �̺�Ʈ ����

#if GAME_VERSION >= G_V_99B
	// ��ų�߰� (2003.10.21)
	#define ADD_SKILL_20031022
	// ��ų�߰� ��

	// ������ �߰�
	#define ADD_ITEM
	// ������ �߰� ��

	// ���� ���� ������ ��� ����
	#define ITEMDROP_RINGOFTRANSFORM
	// ���� ���� ������ ��� ���� ��

	// ��ų �߰�
	#define	ADD_SKILL_WITH_COMBO	

	// 0x1E��Ŷ Ȯ�� (���� ���ݽ� ���� ���ݴ��)
	#define EXPAND_ATTACK_PACKET_0X1E	
#endif

	// Ʈ���̵带 �̿��� ��ų ���� ����
	#define TRADE_SKILLBUG_FIX	

	// ī���� �Ϲݳ��� ������ ���� ���� ����
	#define	CHAOS_MIX_WING_ITEMLEVEL_FIX

	// ĳ���� �ǽð� ��踦 ���ؼ� �ѽð� �������� ������ �����Ҷ�.. �ǽð� ������ ����϶�� DB������ �˸�
	#define ITEMSAVE_MODIFY_FORSTATICS

	// WhatsUp�� ���� ��Ʈ
	#define WHATSUP_DUMMY_SERVER	

	// ���� ���� ����
	#define WING_MIX_MODIFY

	// â�� ������ ���� ���� ����
	#define WAREHOUSE_ITEMCOPY_BUG_FIX

	// �����۹� ���׷��̵�
	#define EVENT_ITEMBAG_UPGRADE

	/////////////////////////////////////////////////////////
	// �׼��� ���� ���� �͵�
	/////////////////////////////////////////////////////////

	// ���˻� ���� = ��ø / 5 �� ���� �Ǿ� ���� �ʾ���.. (apple)
	// ���� ������ -> ���̳ʽ� ��ġ.
	//#define MODIFY_CALC_MAGUMSA_DEFENSE_20040511

	// M: ����̺� ���� üũ, ��������޼��� �� ���μ��� ���� (apple)
	#define APPLY_SERVER_UTIL_20040504

	// ������ �ݻ翡 ���� ȭ�� ������ ����. ���� ȭ�� ���� ���� (apple)
	#define MODIFY_ARROW_DECREASE_ELF_20040503

	// �α� ���� ���� 1�� Check & Modify (apple)
	#define CHECK_LOG_INFINITE_20040426

	// ��Ʈ ��� ������ ���� ���� (apple)
	#define MODIFY_10_11_12_13_SET_DEFENSE_20040421

	// � ĳ�� ��ɾ�ó�� �α� (apple)
	// [IP][AccountID][Name][Command]
	#define ADD_LOG_GM_COMMAND_20040420

	// ���� ��ġ�� ���� ���� (apple)
	#define ADD_HP_POTION_LEVEL1_FIX_20040420

#if GAME_VERSION >= G_V_99B
	// �������� ���� (apple)
	// Speed : 25 Default
	// Delay : 0
	// Type  : 10-15
	#define MODIFY_NOTICE_20040325
#endif

	// ĳ������ �ִ� ���� ZEN MAX ���� : MAX 20�� (apple)
	#define MODIFY_ZEN_MAX_20040414

	// ������ ���� 40�� �ʰ��� 40������ ���� (apple)
	// ������ ���� ������ : ��� ���� �ִ� 30��(foguy)_�ؿ� ������ ����ϴ°� ��� ��� ���.
	// ������ ���� ������ : ��� ���� �ִ� 20��(apple)
	#define MODIFY_ITEM_PRICE_MAXFIX

	// ������ ���� ���� ������ (�ִ� 4������ ����(������ ���� �������� ���� ���� ��� ������ ���� ����)
	#define MODIFY_ITEM_REPAIRE_MAXMONEY

	// �ù̽� ó�� ���� (apple)
	#define MODIFY_SUCCESSBLOCKING_ALLMISS_20040408
	
	// ��ɾ� ó�� �߰� (����â, �������ε���, ���ȼ�����..) (apple)
	#define JS_ADD_TEST_COMMAND

#if GAME_VERSION >= G_V_99B
	// 12, 13 ������ �߰� && �����߰� (apple)
	// CHAOS_MIX_UPGRADE ���� ���� ��.!
	// Viewport ���� �����۷��� ��ȯ �Լ� ���� : ���� 0~7 -> ���� 0~6
	#define ITEM_12_13_20040401

	// ���� �Ծ����� �ٸ� ������� ȿ�� �����ֱ� (apple)
	//#define ADD_EFFECT_POTION_20040401

	// ������ ���� �� �ٸ� ������� ȿ�� �����ֱ�
	// Effect ������ �Լ�, �������� ���⼭ ���Ե� (apple)
	#define ADD_EFFECT_LEVELUP_20040401
#endif

#if GAME_VERSION >= G_V_99B
	// ī���� ���� Ÿ�Ժ��� ���� ���׷��� (foguy)
	// +12, +13�� ���� ��ġ �Ǿ���� (apple)
	#define CHAOS_MIX_UPGRADE	
#endif
	// HP ���� ���� +1 �߰� (apple)
	#define ADD_HP_POTION_LEVEL1_20040401

	// �̺�Ʈ ���� �ɼ� ���ε� -> commonserver.cfg �� �����ʹ� �״�� �ְ� �д� ��θ� ReadCommonServerInfo() �� �и� (b4nfter)
	#define EVENT_OPTION_RELOAD_20040305

	// �̺�Ʈ ���� �ɼ� ���ε� -> commonserver.cfg �� �����ʹ� �״�� �ְ� �̺�Ʈ ���� ���ӿɼ� ���ε� (b4nfter)
	#define ETC_OPTION_RELOAD_20040308

	// ����ڰ� ���Ḧ ���������� �α� ���� (foguy)
	#define	ADD_LOG_USER_CLOSE_SET
	
#if GAME_VERSION >= G_V_99B
	// �̵�, ����, ��Ŷ ����(foguy)
	#define PACKET_CHANGE
#endif

	// ���� ���� ó�� �ð� ����
	#define CHANGE_GUILD_WAR_END_DELAYTIME	

	// ���Ӽ��� ���� -> ���Ӽ��� �� ���� - ���Ӽ����� �л걸���� �ٲ� ���� �߰� (b4nfter)
//	#define GAMESERVER_INFO_TO_CSS_20040317

	// ��Ʈ ������ �ɼ� ����(foguy)	
// [20060821 �ڵ� ����]
//	#define ADD_SET_ITEM_OPTION


	/////////////////////////////////////////////////////////
	// ����� �Ǿ��µ� �Ⱦ��°�
	// ���� <-> ���Ӽ��� �������� TCP (b4nfter)
	// #define GAMESERVER_INFO_TCP_20031203
	// ��Ȱ �����ý��� (b4nfter)
	// #define	HERO_SYSTEM_MODIFY
	// ��Ȱ �����ý��� ��
	/////////////////////////////////////////////////////////
#endif


///////////////////////////////////////////////////////////////////
//
//	�� �����ͷ��� �ý��� ��
//
///////////////////////////////////////////////////////////////////
#ifdef MASTER_LEVEL_SYSTEM_20070912
	// �����ͷ��� ���� �ý���
	#define MASTER_LEVEL_UP_SYSTEM_20070912				

	// �����ͷ��� ������ �̺�Ʈ�� �߰�
	#define ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// ����������7 �߰�
	#define ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 �߰�
	#define ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010		// ī����ĳ��7 �߰�
	#define ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010	// ȯ�����6 ��� (��Ȱ��ȭ �� ������ Ȱ��ȭ)

	// ��ɾ�
	#define ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122		// �����ͷ��� ���� �׽�Ʈ�� ��ɾ� �߰�

	// �����ͷ��� ��ų �ý���	
	#define EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122	// skill(kor).txt ��ũ��Ʈ Ȯ��
	#define MASTER_LEVEL_SKILL_SYSTEM_20071122				// �����ͷ��� ��ų �ý���

//�ؿܷ� ����� ������ ������ �߻��Ͽ� �ӽ÷� ���� ������������ �߰��� - riverstyx
#ifdef PCBANG_POINT_SYSTEM_20070206
	// PC�� ������ ���� �����ý��� ����
	#define BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
	
	// PC�� �����ؼ� �ؿ� ���� ���� ����.(20081106-grooving)
	// [���׼���] ȯ����� �������ġ ǳ���� ���� ����(goni)
	#define MODIFY_EVENTMAP_REWARD_BUGFIX_20080404
#endif // PCBANG_POINT_SYSTEM_20070206

	// [���׼���] �������� ���� �� ����ġ�� �߰��� Ŭ���̾�Ʈ�� ���۵Ǵ� ���� ����
	// ������ �ִ� Ŭ���̾�Ʈ ���׸� �������� ó����.
	// �����ͷ��� �ý��� �����Ͽ� ����ġ ��� �κ��� INT64�� ����.
	#define BUGFIX_GAIN_EXP_20071210

	// [���׼���] ��ų �⺻ �䱸������ �� ���� (RKFKA)
	#define MODIFY_BASIC_SKILL_REQENG_20080102 

	// [�ڵ����] DS�� ������ ��û �ι� �ϴ� �� ����
	#define DELETE_GD_REQUEST_20080110

	// ���� 3+ �����ͷ��� ��ų �߰�
	#define ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107			// �нú� ��ų �߰�
	#define ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��Ƽ�꽺ų �ý��� �� ��Ƽ�꽺ų �߰�
	
	#ifdef  ADD_SEASON_4_UPDATE_20080617
		// [������Ʈ] �����ͷ��� 4��ũ �߰�(goni)
		#define UPDATE_MASTER_LEVEL_4RANK_20080428
	#endif	// ADD_SEASON_4_UPDATE_20080617

	// [�׽�Ʈ �޼���]
	//#define TEST_MSG_MLS_ACTIVE_20080131
#endif

///////////////////////////////////////////////////////////////////
//
//	�� PC�� �̺�Ʈ ��
//
///////////////////////////////////////////////////////////////////
#ifdef PCBANG_REVITALIZATION_EVENT_20060830
	#define ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830		// ����Ʈ�ذ����纯�Ź����� �߰��Ǿ� �־�� �Ѵ�.
	#define PCBANG_COUPON_EVENT_20060124						// PC�� �����̺�Ʈ�� �⺻���� ����Ѵ�.
#endif

///////////////////////////////////////////////////////////////////
//
//	�� ĭ���� ��
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
#if GAME_VERSION >= G_V_S2_2
	#define KANTURU_MOONSTONEPENDANT_20060804		// ������ ���Ʈ ó�� �߰� (RKFKA)
	#define ADD_NEW_MAP_KANTURU_BOSS_20060627		// ĭ���� ������ �߰� (RKFKA)		
	#define ADD_NEW_MAP_KANTURU_COMMON_20060523		// ĭ���� �Ϲ� �� �߰� (RKFKA)		
	#define KANTURU_PROTOCOL_20060705				// ĭ���� �������� �߰� (RKFKA)
#endif

///////////////////////////////////////////////////////////////////
#ifdef ADD_KANTURU_20060627

	//#define KANTURU_TEST_200607119					// ĭ���� �׽�Ʈ�� ������ - �۾��� ������ ��� �����Ѵ�. (RKFKA)
	#define KANTURU_BOSSMONSTER_EXP_NONE_20060809	// ĭ���� �������ʹ� ����ġ�� ���� �ʴ´�.
	#define KANTURU_SPECIAL_ITEMDROP_20060808		// ĭ���� ��ȭ�Ǻ���/������ ���
	#define KANTURU_MONSTER_ITEMDROP_20060807		// ĭ���� ���� ���߼�, ����Ʈ�޾� ������ ��� (RKFKA)
	#define ADD_KANTURU_CONDITION_20060725			// ĭ���� ��Ÿ �۾����� ���� (hnine)
	#define KANTURU_GM_COMMAND_20060709				// ĭ���� ��ڿ� ��ɾ� �߰� (RKFKA)	
	#define ADD_NEW_NPC_KANTURU_20060612			// ĭ���� NPC�߰� (RKFKA)
#endif
//-----------------------------------------------------------------




///////////////////////////////////////////////////////////////////
//
//	�� ũ���̿��� ��
//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
#ifdef CRYWOLF_PRECEDENCE_PATCH_20050413
//-----------------------------------------------------------------

	#ifndef CASTLE_SIEGE_COMMON_PATCH_20041018
		#define CASTLE_SIEGE_COMMON_PATCH_20041018
	#endif 

	#ifndef DEVILSQUARE_EXTEND_20050221
		#if TESTSERVER == 1
			#define DEVILSQUARE_EXTEND_20050221			
		#endif
	#endif

	#ifndef ADD_NEW_MAP_AIDA_20050617
		#define ADD_NEW_MAP_AIDA_20050617
	#endif

//-----------------------------------------------------------------
#endif
///////////////////////////////////////////////////////////////////

 
///////////////////////////////////////////////////////////////////
#ifdef CRYWOLF_COMMON_PATCH_20050413
//-----------------------------------------------------------------
#define ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414		// ũ���̿��� 1�� ������ �߰�
#define ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414		// ũ���̿��� 2�� ������ �߰�

#define	MU_CRYWOLF_SERVER_SYNC_20050505				// ũ���̿��� ���� ���� �� ����ȭ �۾�
#define MU_CRYWOLF_CL_PROTOCOL_20050505				// ũ���̿��� - GS <-> CL ��������
#define MU_CRYWOLF_DS_PROTOCOL_20050505				// ũ���̿��� - GS <-> DS ��������

#define MU_CRYWOLF_BENEFIT_20051215					// ũ���̿��� MVP �¸� ����
#define MU_CRYWOLF_PENALTY_20051215					// ũ���̿��� MVP �й� �г�Ƽ

#define CRYWOLF_MVP_REWARD_RANK_20051215			// ũ���̿��� MVP ����� ���� ��ũ �� ����

//-----------------------------------------------------------------
#endif
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#ifdef CRYWOLF_ONLY_PATCH_20050413
//-----------------------------------------------------------------

#define	CRYWOLF_MAIN_SCHEDULER_20050505				// ũ���̿��� ������ �����͸� �����ϴ� �����췯

#define CRYWOLF_MONSTER_WORK_20050914				// ũ���̿��� MONSTER ó��
#define	CRYWOLF_NPC_WORK_20050912					// ũ���̿��� NPC ó��
#define CRYWOLF_MAP_SETTING_20050915				// ũ���̿��� ���� ���¿� ���� �� �Ӽ� ����

#define CRYWOLF_GM_COMMAND_20050525					// ũ���̿��� ��� ��ɾ�

#define CRYWOLF_MVP_DISABLE_USER_ITEMDROP_20051217	// ũ���̿��� MVP �ÿ��� �׾��� �� ������ ������� ����
#define CRYWOLF_MVP_DISABLE_USER_ZENDOWN_20060116	// ũ���̿��� MVP �ÿ��� �׾��� �� ���� ���� ���� ����

#define CRYWOLF_MODIFY_REGEN_POS_ON_MVP_20060116	// ũ���̿��� MVP �ÿ� �׾��� �� ���� ��ġ


//-----------------------------------------------------------------
#endif
///////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////
//
//	�� ������ ��
//
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#ifdef  CASTLE_SIEGE_COMMON_PATCH_20041018			// ������ ������ �Ϲ� �������� ����Ǵ� ��ġ�κ�
//-----------------------------------------------------------------
//#define MODIFY_MARKOFLORD_OVERLAPPED_20050530		// ������ ǥ���� �κ��丮 �󿡼� ���������� ����
#define	ADD_NEW_MAP_CASTLE_20041115					// ������ �� �߰�
#define	ADD_NEW_NPC_CASTLE_20041115					// ������ ���� NPC �߰�
#define MAP_SERVER_WORK_20041030					// �ʼ��� ���� �۾�

#define	MU_CASTLESIEGE_SERVER_SYNC_20041130			// ������ ������ ���� ����ȭ�� ���ߴ� �Ϸ��� �۾���
#define MU_CASTLESIEGE_CL_PROTOCOL_20041122			// �� ������ - �� Ŭ���̾�Ʈ ���� ��������, ��� �κ�
#define MU_CASTLESIEGE_DS_PROTOCOL_20041105			// �� ������ - ������ ���� ���� ��������, ��� �κ�
#define	MU_CASTLESIEGE_TAX_SYSTEM_20041219			// �� ���� �ý��� ���� ����
#define MU_CASTLESIEGE_CS_MARKS_20041214			// ������ ǥ�� ������ ���� �۾���

#define MODIFY_CASTLE_PK_01_20050309				// ���� ��尡 ������ PK ���� ��쿡 ���� ó��

#define ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301	// ���������� ��ȣ ���� ����, ��������� ����.
#define ADD_NEW_CASTLE_FLAG_01_20041227				// Castle ��� �� ��� ����
#define ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214	// ���� �뺴 NPC �߰�
#define ADD_NEW_ITEM_FOR_CASTLE_01_20041116			// ITEM �߰�
#define ADD_NEW_SKILL_FOR_CASTLE_01_20041116		// ĳ���� ��ų �߰�

#define ADD_NEW_WEAPON_FOR_CASTLE_01_20041116		// ���� ���� �߰�
#define ADD_NEW_VIEWPORT_SYSTEM_01_20041201			// ���� ���� ����Ʈ �ý���
#define ADD_NEW_MAP_CASTLE_HUNTZONE_20041115		// ����� �� �߰�
#define EXPAND_INTERFACE_BYTE_TO_DWORD_20041209		// m_IfState BYTE -> DWORD �� Ȯ��
#define EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122	// ViewSkillState WORD -> INT �� Ȯ��
#define EXPAND_MAX_MAGICLIST_20_TO_60_20041126		// MagicList Ȯ�� 20�� -> 60���� Ȯ��
//-----------------------------------------------------------------
#endif //CASTLE_SIEGE_COMMON_PATCH_20041018


#define MAP_SERVER_ADD_MONSTER_20041216				// �ʼ��� ���� ���� ���� �߰� �Լ� ���� (������ ������ �ִ� �ʸ� �߰��ǵ���)

// ���������� �����ڵ� �߰� (����)
#define JOINSERVER_USING_SERVERCODE
// ���������� �����ڵ� �߰� ��
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
#ifdef  CASTLE_SIEGE_ONLY_PATCH_20041018			// ������ ������ �ʿ��� ��ġ�κ�
//-----------------------------------------------------------------
#define	DISABLE_GETTING_EXP_FROM_GUARDTOWER_20070801	// ����Ÿ�� �ı� �� ����ġ�� ���� ���ϰ� �Ѵ�.
#define CASTLE_NPC_DONT_DROP_ITEM_20050517			// ������ NPC���� �������� ������ �ʴ´�.
#define	CASTLEDEEP_EVENT_20050413					// �η����� ���� �̺�Ʈ (b4nfter)
#define	CASTLE_SPECIAL_ITEMMIX_20050425				// �������� Ư�� ������ ���� (b4nfter)
#define	MU_CASTLESIEGE_START_NOTIFY_20050418		// ������ ���� �� N�� ���� �� ������ ������ �˶� ����
#define	MU_CASTLESIEGE_STATE_SEC_VIEW_20050413		// ������ ���� �����ð� ǥ��
//#define	CASTLE_NPC_TEST_STATE_20050118				// ������ NPC�� �׽�Ʈ�� �ӽü�ġ ����
#define CASTLE_SAVE_PCROOM_USERLIST_20050105		// PC�� �̺�Ʈ ���� - ������ �����ڵ��� PC�� ���� ����
//#define	CASTLE_TEST_SCHEDULER_20041218				// �׽�Ʈ �����췯�� ����Ѵ�. (��� �ܰ�� 5�о�)
#define CASTLE_GM_COMMAND_20041222					// ������ ��� ��ɾ�
//#define	CASTLE_POTION_NPC_20041222					// ������ ���� �����Ĵ� NPC ó��
#define	CASTLE_MAIN_SCHEDULER_20041111				// �������� ������ �����͸� �����ϴ� �����췯
#define	CASTLE_NPC_GATE_WORK_20041206				// ���� ó���� ���õ� �۾���
#define CASTLE_NPC_GUARDIANSTATUE_WORK_20041207		// ���� ó���� ���õ� �۾���
#define CASTLE_NPC_LIFESTONE_WORK_20041208			// ���������� (����) ó���� ���õ� �۾���
#define CASTLE_NPC_CROWN_WORK_20041210				// �հ� ó���� ���õ� �۾���
#define CASTLE_NPC_CROWN_SWITCH_WORK_20041210		// �հ� ����ġ ó���� ���õ� �۾���
#define CASTLE_NPC_CANNON_TOWER_20041223			// ĳ��Ÿ�� ó���� ���õ� �۾���
#define MU_CASTLESIEGE_JOINSIDE_VIEWPORT_20041209	// ������ �� ��/�� ������ �Ҽ� ���¸� ����Ʈ�� �߰��Ͽ� ǥ����
#define MU_CASTLESIEGE_LOG_20050107					// ������ ���� �α� ����
#define ADD_CASTLE_HUNTZONE_MONSTER_01_20050111		// ����� ����. Ư�� BOSS �� ó��
#define ADD_CASTLE_HUNTZONE_NPC_01_20050111			// ����� ������ ��.
#define ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303	// ���� ����Ϳ��� ���� �߰�.(1.�������� ����)
#define ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314	// ���� ����Ϳ��� ��Ʈ������ ��� �߰�.(2.�÷��Ƕ�������)
#define MODIFY_RANGEATTACK_CHECK_20050113			// ���� ��ų ���ݹ�� ���� (�����ʿ��� ������ ���� �� ����� �µ���)
#define MODIFY_UPDATING_VIEWPORT_20050118			// ���� ���� ��� ĳ���͸� �ٽ� �׸���� ����� Ŭ���̾�Ʈ���� ���� �ʴ´�.

#define MODIFY_ANS_GIVEUP_20050413					// ���� ���� �Ⱓ���� ��������Ⱓ�� �ƴҶ� Ŭ���̾�Ʈ�� �����ִ� ���� ó�� ���� (apple)
//-----------------------------------------------------------------
#endif //CASTLE_SIEGE_ONLY_PATCH_20041018
///////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////
//
//	�� �̺�Ʈ ��	- Old Style...
//
///////////////////////////////////////////////////////////////////
// Blood Castle��
#define FOR_BLOODCASTLE						// ���� ĳ������
#define FOR_BLOODCASTLE2					// ���� ĳ���� �߰� �۾�(foguy)
#define FOR_BLOODCASTLE3					// ���� ĳ���� �߰� �۾�(b4nfter - 2003.10.21 ��ġ)
#if GAME_VERSION >= G_V_99B
#define BLOODCASTLE_EXTEND_20040314			// ����ĳ�� 7���� Ȯ��	(�����Ǵ� ���ҽ� gate.txt, MonsterSetBase.txt, BloodCastle.dat)
#define BLOODCASTLE_EVENT_3RD_20040401		// ����ĳ�� 3�� �̺�Ʈ (ĳ�� �����ð� ����) (b4nfter)
#define BLOODCASTLE_EVENT_4TH_20040531		// ����ĳ�� 4�� �̺�Ʈ (ĳ�� �����ð� ����) (b4nfter)
#define BLOODCASTLE_EVENT_5TH_20050531		// ����ĳ�� 5���̺�Ʈ. ��Ƽ�߽����� ���� (b4nfter)
#endif

// �������� �����̺�Ʈ
#define ELEGORADO_EVENT						
///////////////////////////////////////////////////////////////////


#if GAME_VERSION >= G_V_99B
///////////////////////////////////////////////////////////////////
//
//	�� �⺻ �ý��� ��	- Old Style...
//
///////////////////////////////////////////////////////////////////
// ���� �ý���
#define	DUEL_SYSTEM_20031028
#define	DUEL_SYSTEM_FIX_20031224	

// 2�� ����Ʈ
#define SECOND_QUEST

// ���� ���� �ý���
#define MONSTER_HERD_SYSTEM_20031120

// ��ũ�ε� �۾�	(������ ��ũ��Ʈ Ȯ�嵵 ���� ��)
#define	DARKLORD_WORK

// ���λ��� �߰� (b4nfter)
#define PERSONAL_SHOP_20040113
#define PERSONAL_SHOP_BUG_FIX
#define ADD_SHOP_11_12	// ���� �߰�
#endif

// ��� ���� ����
#define _NEW_EXDB_

#define QUEST_BUG_FIX	// ����Ʈ ���� ����

///////////////////////////////////////////////////////////////////


#ifdef MAP_SERVER_WORK_20041030						// �ʼ��� ���� �۾� ���� �߰� ��ġ
//#define MAP_SERVER_ADD_MONSTER_20041216				// �ʼ��� ���� ���� ���� �߰� �Լ� ���� (������ ������ �ִ� �ʸ� �߰��ǵ���)
#endif //MAP_SERVER_WORK_20041030


#define LOLONG(l)									((DWORD)((UINT64)(l) & 0xFFFFFFFF))
#define HILONG(l)									((DWORD)((UINT64)(l) >> 32))
#define MAKELONGLONG(a, b)							((INT64)(((DWORD)((UINT64)(a) & 0xFFFFFFFF)) | ((INT64)((DWORD)((UINT64)(b) & 0xFFFFFFFF))) << 32))

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
// D E F I N E : ���� ������ �߰�
#ifdef MODIFY_SIEGE_POTION_OVERLAP_COUNT_20060511
#define MAX_SPECIAL_POTION_OVERLAP_COUNT	250
#else
#define MAX_SPECIAL_POTION_OVERLAP_COUNT	160
#endif

// ���� MODIFY_CASTLE_POTION_20051017
// BLESS:60 -> 120, SOUL:30 -> 60
#define CASTLE_BLESSPOTION_ACTIVETIME		120
#define CASTLE_SOULPOTION_ACTIVETIME		60


// D E F I N E : ��ȥ�� ���� AG ȸ����
#define CASTLE_SOULPOTION_AG_REFILL_POINT		8

// D E F I N E : ��ȥ�� ���� ���׷� ������(%)
#define CASTLE_SOULPOTION_ADD_RESISTANCE	50


#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// D E F I N E : ���� ��ų �߰� 
enum SKILL_LIST_TYPE
{
	// �⺻
	SKILL_LIST_TYPE_DEFALT	= 0x00,
	// ��ų �߰� 
	SKILL_LIST_TYPE_ADD		= 0x01,
	// ��ų ���� 
	SKILL_LIST_TYPE_DEL		= 0x02,

};

enum KILLCOUNT_OPTION
{
	KILLCOUNT_RESET		= 0x00,
	KILLCOUNT_INC		= 0x01,
	KILLCOUNT_DEC		= 0x02,	
};

#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
// D E F I N E : ����� NPC �߰�

#define CASTLE_HUNTZONE_GATE_01		106

enum CASTLE_HUNTZONE_NPCTALK_RESULT
{
	CASTLE_HUNTZONE_ENTRANCE_DISABLE			= 0x00,
	CASTLE_HUNTZONE_ENTRANCE_ENABLE				= 0x01,
	CASTLE_HUNTZONE_ENTRANCE_ENABLE_TO_MEMBER	= 0x02,
	CASTLE_HUNTZONE_ENTRANCE_ENABLE_TO_OWNER	= 0x03,
};

enum CASTLE_HUNTZONE_SETTING
{
	CASTLE_HUNTZONE_SETTING_FAIL				= 0x00,	
	CASTLE_HUNTZONE_SETTING_SUCCESS				= 0x01,

};

enum CASTLE_HUNTZONE_USAGE
{
	CASTLE_HUNTZONE_USAGE_DISABLE				= 0x00,	
	CASTLE_HUNTZONE_USAGE_ENABLE				= 0x01,

};
#endif


///////////////////////////////////////////////////////////////////
//
//	ģ�� �ý���
//
///////////////////////////////////////////////////////////////////////
#ifdef __FRIEND_WORK__
	#define MAX_MEMO_SUBJECT	32
	#define MAX_MEMO			1000
	#define MAX_DATE			30
	
	#define FRIENDSERVER_ONLINE				0	// ģ�� ���� �¶��� ����
	#define FRIENDSERVER_OFFLINE			1	// ģ�� ���� �������� ����
	#define FRIENDSERVER_ONLINEREQUEST		2	// ģ�� ������ ���� ��û ��

#endif

//////////////////////////////////////////////////////////////////////////
//
//	���� ����
//
//////////////////////////////////////////////////////////////////////////
#ifdef UPDATE_UNIFICATIONBILL_20090203
enum
{
	BILL_TYPE_NONE					= 0,				// ����
		BILL_TYPE_PERSONAL_AMOUNT		= 1,				// ���� ����
		BILL_TYPE_PCBANG_AMOUNT			= 2,				// PC�� ����
		BILL_TYPE_PCBANG_QUANTITY		= 3,				// PC�� ����
		BILL_TYPE_PERSONAL_QUANTITY		= 4,				// ���� ����
		BILL_TYPE_COLLOR				= 5,				// �÷� �����(����)
};
#define MAX_LOGIN_COUNT			6
#else	// UPDATE_UNIFICATIONBILL_20090203
#define MAX_LOGIN_COUNT			3

#define CERTIFYTYPE_ACCOUNT		0			// ��������
#define CERTIFYTYPE_IP			1			// IP ����

#define BILLTYPE_JUNGYANG		0			// ���� 
#define BILLTYPE_JUNGACK		1			// ���� 
#define BILLTYPE_JONGYANG		2			// ����
#define BILLTYPE_TIME			3			// �ð���		
#define BILLTYPE_DATE			4			// ��¥��
#define BILLTYPE_NOCHARGE		5			// ����
#endif	// UPDATE_UNIFICATIONBILL_20090203
//----------------------------------------------------------------------------
// ���������� �����ϴ� ���α׷��� Ÿ��
#define ST_GAMESERVER				1			// ���Ӽ��� ��ȣ

#define DEFMAPSERVERPORT			55900		// Map Server port number
#define DEFJOINSERVERPORT			55970		// Join Server port number
#define DEFDATASERVERPORT			55960		// Game Data Server Port number
#ifdef ADD_GAMESERVER_PARAMETER_20050728
#define DEFEXDBSERVERPORT			55906		// ExDB Server Port number
#endif

//----------------------------------------------------------------------------
// ���������� ���Ǵ� ���ǵ�..
#define MAX_JOOMINNUMBERSTR			13		// �ֹε�Ϲ�ȣ ��Ʈ�� ũ�� -�� �A ũ���̴�.
#define MAX_QUESTIONSTR				30		// ���� ��Ʈ�� ũ��
#define MAX_ANSWERSTR				30		// �亯 ��Ʈ�� ũ��
#define MAX_PHONENUMBERSTR			20		// ��ȭ��ȣ ��Ʈ�� ũ��
#define MAX_EMAILSTR				50		// e-mail ��Ʈ�� ũ��


//////////////////////////////////////////////////////////////////////////
//
//	���� Ÿ�� ����
//
//////////////////////////////////////////////////////////////////////////
// Server Type
#define ST_JOINSERVER				0		// ���� ���� ����
#define ST_GAMESERVER				1		// ���� ����
#define MAX_CHAT					60		// ��ȭ �ִ� ũ��
#define MAX_GUILDNOTICE				60		// �ִ� ��� ���� ����


#ifdef GAMESERVER_INFO_TO_CSS_20040317		// ���Ӽ����л� - ��ũ�μ���
#define MAX_CS_COUNT				32		// �ִ� ���Ӽ����� ����
#endif

//////////////////////////////////////////////////////////////////////////
//
//	�ý��� �⺻ ����
//
//////////////////////////////////////////////////////////////////////////
#define MAX_ROADPATH				15		// ����ڰ� �̵��ϴ� PATH �ִ� ũ��
#define MAX_PATHDIR					8		// �̵��� ���Ǵ� ���� ����
#define MAX_IDSTRING				10		// ���̵� ũ��
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
#define MAX_PASSWORD_LENGTH			20
#define MAX_JOOMINNUMBER_LENGTH		20
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
#define MAX_PASSWORD_LENGTH			12
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424
#define DEFAULT_MONEY				0		// �ʱ� �־����� �� �׼�

#ifdef GAME_VERSION >= G_V_S4_6
#define MAX_LEVEL					400			// ���� �ִ� ũ��
#else
#define MAX_LEVEL					1000		// ���� �ִ� ũ��
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// �����ͷ��� �ִ� ũ��
	#define MAX_MASTER_LEVEL			200
#endif

#ifdef ITEM_12_13_20040401
	#define MAX_ITEMLEVEL			15		// ���� �ִ� ũ��
#else
	#define MAX_ITEMLEVEL			15		// ���� �ִ� ũ��
#endif

#define MAX_PRESENTNAME				50		// �ִ� ��ǰ �̸�

#ifdef MODIFY_ZEN_MAX_20040414
	#define MAX_ZEN					2000000000
#else
	#define MAX_ZEN					2000000000
#endif

#define MAXVIEWPORTOBJECT			75		// ����Ʈ �ִ� OBJECT �� 
#ifdef GAMESERVER_TO_CASTLESIEGESERVER		// ������ ���뼭�� �϶��� ���� ����Ʈ ��ȭ
#define MAX_MONVIEWPORTOBJECT		75		// ������ �信�� �ִ� 50 ���� ���� ǥ�� (������ ���뼭�� �� ����)
#else
#define MAX_MONVIEWPORTOBJECT		20		// ������ �信�� �ִ� 20 ���� ���� ǥ��
#endif

#define MAXUSERCHARACTER			5		// ������ ������ �ִ� �ִ� ĳ���� ��

#define MAX_GUILDNAMESTRING			8		// ��� �̸� ��Ʈ�� ����

#define MAX_GUILDMARKSIZE			32		// ��� ��ũ ���� ũ��

#define MAX_DBEQUIPMENT				120		// (12x10) �����ͺ��̽� �б�&����Ǵ� ������� ũ��

#ifdef PERSONAL_SHOP_20040113
#define MAX_PSHOP_TEXT				36		// ���λ��� �ؽ�Ʈ�� �ִ�ũ��

#ifdef ITEM_DBSIZE_EXTEND_20050706
#define MAX_DBINVENTORY				1728	// (8x8x10)+120+(8x4x10) �����ͺ��̽� �б�&����Ǵ� �κ��丮������ ũ��
#else
#define MAX_DBINVENTORY				1080	// (8x8x10)+120+(8x4x10) �����ͺ��̽� �б�&����Ǵ� �κ��丮������ ũ��
#endif

#else
#define MAX_DBINVENTORY				760		// (8x8x10)+120 �����ͺ��̽� �б�&����Ǵ� �κ��丮������ ũ��
#endif

#ifdef ITEM_DBSIZE_EXTEND_20050706
#define MAX_ITEMDBBYTE				16		// �������� DB�� ����Ǵ� ����Ʈ ��
#define MAX_ITEMDBBYTE_V02			10		// �������� DB�� ����Ǵ� ����Ʈ ��
#else
#define MAX_ITEMDBBYTE				10		// �������� DB�� ����Ǵ� ����Ʈ ��
#endif

#define MAX_ITEMDBBYTE_V00			7		// �������� DB�� ����Ǵ� ����Ʈ ��

#ifdef ITEM_INDEX_EXTEND_20050706
#define ITEM_INDEX_NOT_EXIST		511		// 9��Ʈ ������ �ε��� (4��Ʈ ������ Ÿ��) �� ��� �������� �ʴ� ���ʷ� �������� �ε���
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	#define ITEM_BUFFER_SIZE			12			// ������ ��Ŷ ���� ũ�� 
#else
	#ifdef ITEM_INDEX_EXTEND_20050706
	#define ITEM_BUFFER_SIZE			7			// ������ ��Ŷ ���� ũ�� 
	#elif GAME_VERSION >= G_V_99B
	#define ITEM_BUFFER_SIZE			5			// ������ ��Ŷ ���� ũ�� 
	#else
	#define ITEM_BUFFER_SIZE			4
	#endif
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#define MONEY_BUFFER_SIZE			4			// �� ��Ŷ ���� ũ��

#ifdef ITEM_DBSIZE_EXTEND_20050706
#define MAX_WAREHOUSEDBSIZE			1920
#else
#define MAX_WAREHOUSEDBSIZE			1200
#endif

#define MAX_WAREHOUSEITEMS			(8*15)

#define MAX_CHAOSBOXSIZE			8*4
#define MAX_CHAOSBOXITEMS			8*4


#define MAX_EQUIPMENT				12
#define MAX_INVENTORYMAP			8*8
#define MAX_INVENTORY				(MAX_EQUIPMENT + MAX_INVENTORYMAP)		// (8x8)+12 �κ��丮 �ִ� ����
#define MAX_TRADEITEM				32				// (8x4) 

#ifdef PERSONAL_SHOP_20040113		
#define MAX_PSHOPITEM				8*4		// (8x4) ���λ���â
#define MAX_PSHOPITEMMAP			8*4		// (8x4) ���λ���â ��
#define MAX_INVENTORY_EXTEND		(MAX_INVENTORY + MAX_PSHOPITEM)		// (8x8)+12+(8*4) DB���� �� �κ��丮 �ִ� ����
#endif

#define MAX_DBMAGIC					(MAX_MAGIC*3)	// �����ͺ��̽��� ����Ǵ� ���� ������ ũ��

#ifdef ITEM_DBSIZE_EXTEND_20050706
#define ITEM_BITSHIFT				9
#else
#define ITEM_BITSHIFT				5
#endif

#ifdef ADD_NOT_SELL_EQUIPITEM_20080625
#define IS_EQUIPITEM(pos)			(pos >= EQUIPMENT_WEAPON_RIGHT && pos < MAX_EQUIPMENT) ? TRUE : FALSE
#endif // ADD_NOT_SELL_EQUIPITEM_20080625

//////////////////////////////////////////////////////////////////////////
//
//	ĳ���Ϳ� ���õ� ���ǵ�
//
//////////////////////////////////////////////////////////////////////////
#define PK_MINIMUM_LEVEL			5		// ����ڳ��� ���ϼ� �ִ� �ּ� ���� 

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ä�� ���� ���� Ȯ��
#define MAX_CLASSTYPE				6		// �ִ� ���� ����
#else
#ifdef DARKLORD_WORK
#define MAX_CLASSTYPE				5		// �ִ� ���� ����
#else
#define MAX_CLASSTYPE				4		// �ִ� ���� ����
#endif
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#define MAX_PREVIEWCHARSET			9		// ĳ���� ��Ʈ �ִ� ũ��( �̸������ ���� )

#define MAX_RESISTANCE				7		// ��Ʈ ���������� ���� 7�� ����


#ifdef GAMESERVER_TO_CASTLESIEGESERVER		// ������ ���뼭���� ����� ������Ʈ�� ������ �ø��� ���� ������ ���δ�.
#if TESTSERVER == 1
#define MAX_OBJECT					7400	// ���Ӽ����� �ִ� ������Ʈ(����) ��
#define MAX_MONSTER					5800
#define MAX_CALLMONSTER				600
#define MAX_USER					1000
#else
#define MAX_OBJECT					4000	// ���Ӽ����� �ִ� ������Ʈ(����) ��
#define MAX_MONSTER					1500
#define MAX_CALLMONSTER				1000
#define MAX_USER					1500
#endif	// TESTSERVER

#else

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
#define MAX_OBJECT					9000	// ���Ӽ����� �ִ� ������Ʈ(����) ��
#define MAX_MONSTER					7400
#define MAX_CALLMONSTER				600
#define MAX_USER					1000
#else
#ifdef CHAOSCASTLE_SYSTEM_20040408
#define MAX_OBJECT					7400	// ���Ӽ����� �ִ� ������Ʈ(����) ��
#define MAX_MONSTER					5800
#define MAX_CALLMONSTER				600
#define MAX_USER					1000
#else
#define MAX_OBJECT					5650	// ���Ӽ����� �ִ� ������Ʈ(����) ��
#define MAX_MONSTER					4800
#define MAX_CALLMONSTER				600
#define MAX_USER					250
#endif	// CHAOSCASTLE_SYSTEM_20040408
#endif	// MODIFY_ILLUSIONTEMPLE_BUGFIX_3_20070731
#endif	// GAMESERVER_TO_CASTLESIEGESERVER

#define ALLOC_USEROBJECTSTART		(MAX_MONSTER+MAX_CALLMONSTER)	// ���Ӽ����� �ִ� ������Ʈ(����) ��

#define OBJTYPE_CHARACTER			1		// ������Ʈ Ÿ���� ����ĳ�����̴�.
#define OBJTYPE_MONSTER				2		// ������Ʈ Ÿ���� �����̴�.
#define OBJTYPE_NPC					3		// ������Ʈ Ÿ����  NPC �̴�.
#define OBJTYPE_ITEM				5		// �������̴�.

//----------------------------------------------------------------------------
// Killer Type ( �׿���
#define KT_CHAR						0		// �÷��̾����� �׾���.
#define KT_MONSTER					1		// �������� �׾���.
#define KT_GUILDWARCHAR				2		// ��������� �÷��̾ �׿���.

#ifdef DUEL_SYSTEM_20031028
#define KT_DUELCHAR					3		// ���� �߿� �÷��̾ �׿���.
#endif

//////////////////////////////////////////////////////////////////////////
//
// ���ӿ� ���Ǵ� ���ǵ�
//
//////////////////////////////////////////////////////////////////////////
#define MAX_LEVELUPPOINT			5		// ĳ���ʹ� ������ ������ ����Ʈ

#ifdef ADD_GAMBLING_20090120			// �׺� �������� �߰�
	#define MAX_SHOP						20
#else // ADD_GAMBLING_20090120
#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� �������� �߰�
	#define MAX_SHOP						18
#else
#ifdef ADD_SHOP_14_20070308
	#define MAX_SHOP						15
#else
#ifdef ADD_FIRECRACKER_NPC_20070306		// ���� ���� �߰�
	#define MAX_SHOP						14		// �ִ� ���� ����
#else	// ADD_FIRECRACKER_NPC_20070306
	#ifdef ADD_SHOP_11_12
		#define MAX_SHOP					13		// �ִ� ���� ����
	#else
		#define MAX_SHOP					12		// �ִ� ���� ����
	#endif
#endif	// ADD_FIRECRACKER_NPC_20070306
#endif	// ADD_SHOP_14_20070308
#endif	// ADD_SEASON_3_NEW_MAP_20070910 
#endif	// ADD_GAMBLING_20090120

#define PK_LEVEL_DEFAULT			3		// PK Level �� �⺻ ��

#ifdef HERO_SYSTEM_MODIFY
#define PK_LEVEL_3_ITEMDROP			2		// PK Level 3 �̻��� ������ �����
#define PK_LEVEL_2_ITEMDROP			4		// PK Level 2 �̻��� ������ �����
#define PK_LEVEL_1_ITEMDROP			8		// PK Level 1 �̻��� ������ �����

#define PK_LEVEL_3_ITEMERASE		8		// PK Level 3 �̻��� ������ �Ҹ���
#define PK_LEVEL_2_ITEMERASE		16		// PK Level 2 �̻��� ������ �Ҹ���
#define PK_LEVEL_1_ITEMERASE		32		// PK Level 1 �̻��� ������ �Ҹ���
#endif

// ����Ʈ���� ���Ǵ� ���ǵ�
#define MAX_QUEST					200
#define MAX_DBQUEST					50
#define MAX_QUEST_SUBINFO			10
#define MAX_QUEST_CONDITION			10

#define APPLE_DROPRATE				2000	// �ɸ��� ������ 20 ������ �� ��� ��ӷ� (n / 10000)

//----------------------------------------------------------------------------
#define PARTY_LENGTH				130

//----------------------------------------------------------------------------
#ifdef ADD_ITEM_FENRIR_01_20051110
#define MAX_OVERLAP_FENRIR_MIX_STUFF_01		20	// ���� ����
#define MAX_OVERLAP_FENRIR_MIX_STUFF_02		20	// ������ ��ȣ
#define MAX_OVERLAP_FENRIR_MIX_STUFF_03		10	// �ͼ��� ����

#ifdef ADD_PCS_FULL_RECOVERY_POTION_20070508
#define MAX_OVERLAP_FULL_RECOVERY_POTION	100 // 100% ȸ������ ��ġ�� ����
#endif // ADD_PCS_FULL_RECOVERY_POTION_20070508

#define MIN_FENRIR_EQUIP_LEVEL				300	// 300���� ���� ���� ����
#define MAX_FENRIR_DAMAGE_INC_LEVEL			5	// 5���� ���� ��ų������ 1% ����

#endif // ADD_ITEM_FENRIR_01_2005

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#define MAX_USER_FOR_GAMEGUARD_AUTH		1500
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123


#ifdef UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828	// ����, ��ũ�� ������ ��� ���� 
#ifdef MODIFY_SPEAR_DROP_BUGFIX_20080901
	#define MAX_BALLTABLE_SIZE	22
#else	// MODIFY_SPEAR_DROP_BUGFIX_20080901
	#define MAX_BALLTABLE_SIZE	22
#endif	// MODIFY_SPEAR_DROP_BUGFIX_20080901
#else	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828
#ifdef MODIFY_ITEM_DROP_FIRESCREAMSCROLL_20060830
	#define MAX_BALLTABLE_SIZE	17
#else  
#ifdef MODIFY_ITEM_DROP_MAXINDEX_20060830
	#define MAX_BALLTABLE_SIZE	16
#endif // MODIFY_ITEM_DROP_MAXINDEX_20060830
#endif // MODIFY_ITEM_DROP_FIRESCREAMSCROLL_20060830
#endif	// UPDATE_SEASON4_MARKET_CONDITIONS_BALANCE_20080828

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
enum EXP_GETTING_EVENT_TYPE
{
	EVENT_TYPE_NONE				= 0,
	EVENT_TYPE_PARTY			= 1,
	EVENT_TYPE_DEVILSQUARE		= 2,
	EVENT_TYPE_CHAOSCASTLE		= 3,
	EVENT_TYPE_BLOODCASTLE		= 4,
	EVENT_TYPE_CRYWOLF			= 5
};
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
#define MAX_OVERLAP_JACK_O_LANTERN_POTION	3	// �ҷ��� ���� �̺�Ʈ ���� �������� ����
#endif

#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
#define PLAYTIME_LIMIT_VERIFY_ADULT		0
#define PLAYTIME_LIMIT_VERIFY_KID		1
#define PLAYTIME_LIMIT_VERIFY_ERROR		2
#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509

#ifdef ADD_FULL_RECOVERY_POTION_20070719
#define MAX_OVERLAP_ELITE_POTION			50	// ����Ʈ ������ �ִ� �������� ����
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef ADD_PCS_IMPROVE_POTION_20070718
#define MAX_OVERLAP_IMPROVE_POTION			3	// ����� �ִ� �������� ����
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
enum _enumMAXOVERLAP
{
	MAX_OVERLAP_CHERRYBLOSSOM_ITEM_DEFAULT = 3,	
	MAX_OVERLAP_CHERRYBLOSSOM_WHITE	=	10, 
	MAX_OVERLAP_CHERRYBLOSSOM_RED =		30, 
#ifdef FOR_KOREA
	MAX_OVERLAP_CHERRYBLOSSOM_GOLD =	255, 
#else	// FOR_KOREA
	MAX_OVERLAP_CHERRYBLOSSOM_GOLD =	50, 
#endif	// FOR_KOREA
};
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� ���� ��ġ�� �ִ� ����
#define MAX_OVERLAP_LUCKY_COIN			255
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029	


//////////////////////////////////////////////////////////////////////////
//
//	����Ʈ ���� ����
//
//////////////////////////////////////////////////////////////////////////
// �ڽ��� ������ ���߿��� �������� ����Ʈ���� �ȴ�.
#define Max3(v1, v2, v3) (max((max(v1, v2)),v3))

//----------------------------------------------------------------------------
// ������ ����Ʈ �ȿ� �ִ� �ٸ� ������ ���� ����
#define STVP_NONE		0	// ����� �ȵǾ��ִ�.
#define STVP_CREATE		1	// Create �Ǿ�� �Ѵ�.
#define STVP_PLAYING	2	// Ȱ�� ���̴�.
#define STVP_DESTROY	3	// Destroy �Ǿ�� �Ѵ�.

typedef struct 
{
	char  state;	// ����Ʈ������ ����
	short number;	// ����Ʈ�� ��ϵ� ������Ʈ�� ���� ��ȣ
	BYTE  type;		// ��ϵ� ������Ʈ�� ���� (STVP_??????) ����
	short index;	// ��ϵ� ������Ʈ�� ���° ����� �Ǿ�����..
	int   dis;		// �Ÿ�.
} VIEWPORT_PLAYER_STRUCT;


// �� ȭ�鿡 �������� ����� ����Ʈ
typedef struct 
{
	char  state;		// ����Ʈ������ ����
	short number;		// ����Ʈ�� ��ϵ� ������Ʈ�� ���� ��ȣ
	BYTE  type;			// ��ϵ� ������Ʈ�� ���� (STVP_??????) ����
	short index;		// ��ϵ� ������Ʈ�� ���° ����� �Ǿ�����..
	int   dis;			// �Ÿ�.
//	short HitDamage;	// Ÿ�� ������ (�������� ��ŭ �¾Ҵ�)
} VIEWPORT_STRUCT;


#ifdef PERSONAL_SHOP_20040113
// �� ȭ�鿡 �������� ���λ��� ����Ʈ (���)
typedef struct 
{
	bool	bUsing;		// 0:������ / 1:�����
	short	iIndex;		// ����ڰ�ü �迭�� �ε���
} PSHOP_VIEWPORT_STRUCT;
#endif


#define MAX_HITDAMAGE	40

typedef struct
{
	short number;		// Ÿ���� ���� ���� ��ȣ
	int   HitDamage;	// Ÿ�� ������ (�������� ��ŭ �¾Ҵ�)
	DWORD LastHitTime;	// ������ Ÿ�� �ð�..

} HITDAMAGE_STRUCT;		// ���� Ÿ�� �������� ����ϱ� ����

//////////////////////////////////////////////////////////////////////////
//
//	UI Type
//
//////////////////////////////////////////////////////////////////////////
#define I_TRADE						1		// ��ȯ ������
#define I_PARTY						2		// ��Ƽ ������
#define I_SHOP						3
#define I_GUILD						4		// ��� ���� �������̽�
#define I_GUILDCREATE				5		// ��� ���� �������̽�
#define I_WAREHOUSE					6		// â��
#define I_CHAOSBOX					7		// ī���� �ڽ�
#define I_INVENTORY					8		// �κ��丮��
#define I_EVENTCHIP					9		// �̺�ƮĨ
#define I_QUEST						10		// ����Ʈ
#define I_SERVERDIVISION			11		// ��������
#define I_ETCWINDOW					12		// ��������
#define I_DARK_TRAINER				13		// ��ũȣ�� Ʈ���̳�

#ifdef PERSONAL_SHOP_20040113				// ���λ��� �������̽� �ڵ� (I_SHOP�� ���� ���� �;����� ����� �ʱ�ȭ�� �κ��� �ٸ� ����� ���� ��)
#define I_PSHOP_OPEN				12		// ���λ��� - ������ �� �� ��� (�湮�� ���� I_SHOP)
#endif

#ifdef ADD_NEW_UNION_01_20041006
#define I_UNION						14		// ����, ���̹� ���� �������̽�.
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#define I_CS_MACHINE_ATTACK			15		// ���� ����
#define I_CS_MACHINE_DEFENSE		16		// ���� ����
#endif

#ifdef ADD_CASTLE_HUNTZONE_NPC_01_20050111
#define I_CS_HUNTZONE_GUARD			17		// ����� ������	
#endif

#ifdef PCBANG_POINT_SYSTEM_20070206			// ���� �ε���
#define I_PCBANG_POINT_SHOP			18		// PC�� ����Ʈ ����
#endif

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	#ifdef ADD_PARTIALY_CHARGE_ON
	#define I_CASHSHOP_INTERFACE		19
	#else
	#define I_CASHSHOP_INTERFACE		18
	#endif // ADD_PARTIALY_CHARGE_ON
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020
enum _eInterfaceState
{
	I_JEWEL		= 20,
};

#define	IS_MIXJEWEL(u, t)		(u == 1 && t == I_JEWEL) ? TRUE : FALSE

#endif // MODIFY_JEWELMIXSYSTEM_BUGFIX_20081020

#ifdef PERSONAL_SHOP_20040113
// ������ �̵� (0x24) �� ���Ǵ� ��ġ ��ũ��
#define ITEMMOVE_INVENTORY			0		// �̵���ġ - �κ��丮
#define ITEMMOVE_TRADEWND			1		// �̵���ġ - �ŷ�â
#define ITEMMOVE_WAREHOUSE			2		// �̵���ġ - â��
#define ITEMMOVE_CHAOSBOX			3		// �̵���ġ - ī��������
#define ITEMMOVE_PSHOPWND			4		// �̵���ġ - ���λ���
#else
#define ITEMMOVE_INVENTORY			0		// �̵���ġ - �κ��丮
#define ITEMMOVE_TRADEWND			1		// �̵���ġ - �ŷ�â
#define ITEMMOVE_WAREHOUSE			2		// �̵���ġ - â��
#define ITEMMOVE_CHAOSBOX			3		// �̵���ġ - ī��������
#define ITEMMOVE_PSHOPWND			4		// �̵���ġ - ���λ���
#endif

#ifdef DARKLORD_WORK
#define ITEMMOVE_DARK_TRAINER			5	// �̵� ��ġ - ��ũ�ε�(���û�)
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#define	ITEMMOVE_JEWELOFHARMONY_PURITY	6	// �̵� ��ġ - ��ȭ�Ǻ��� ���� ( NPC )
	#define ITEMMOVE_JEWELOFHARMONY_SMELT	7	// �̵� ��ġ - ���ü� ( NPC )
	#define ITEMMOVE_JEWELOFHARMONY_RESTORE 8	// �̵� ��ġ - ȯ�� ( NPC )
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
#define ITEMMOVE_LOTTERY_MIX	9
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
#define ITEMMOVE_CHERRYBLOSSOM	10
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
#define ITEMMOVE_SOCKET_ITEM_EXTRACT	11
#define ITEMMOVE_SOCKET_ITEM_COMPOSIT	12
#define ITEMMOVE_SOCKET_ITEM_EQUIP		13
#define ITEMMOVE_SOCKET_ITEM_REMOVE		14
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415

#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
#define	CASTLE_ITEM_MIX_MAXCOUNT		1	// ���� ���� �� ������ �ִ� ���հ��� Ƚ��
#endif

//////////////////////////////////////////////////////////////////////////
//
// �� ���� ����
//
//////////////////////////////////////////////////////////////////////////
#define MAX_MAPITEM					300

#ifdef ADD_NEWPVP_PKFIELD
#define MAX_MAP						65		// ũ�������� ��
#else

#ifdef UPDATE_XMASEVENT_MAP_20080930
#define MAX_MAP						63		// ũ�������� ��
#else

#ifdef ADD_RAKLION_20080408
#define MAX_MAP						59		// ����ȣ�� ��
#else
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
#define MAX_MAP						57		//	�����ͷ��� ���� ����Ǵ�
#else
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	
#define MAX_MAP						54		// �����ͷ��� ������ ī����ĳ��
#else
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#define MAX_MAP						53		// �����ͷ��� ������ ����ĳ��
#else
#ifdef ADD_SEASON_3_NEW_MAP_20070910
#define MAX_MAP						52		// ���� 3 �ű� ��
#else
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#define	MAX_MAP						51		// �ű� �̺�Ʈ ��
#else
#ifdef UPDATE_BEFORE_ILLUSION_TEMPLE_MAX_MAP_20070702
#define	MAX_MAP						43		// �ű� �̺�Ʈ ������ �ִ� �� �� �� ����
#else
#ifdef UPDATE_GM_FUNCTION_20070228
#define MAX_MAP						41		// GM ��ȯ ���� �߰�
#else
#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
#define MAX_MAP						40		// ĭ���� ������ �߰�
#else
#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
#define MAX_MAP						39		// ĭ���� �Ϲ� �� �߰� (2)
#else
#ifdef HIDDEN_KALIMA_20050706
#define MAX_MAP						37		// ���� Į���� �߰�
#else
#ifdef ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
#define MAX_MAP						36		// ũ���̿��� 2������ �߰� �Ǿ���.
#else
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
#define MAX_MAP						35		// ũ���̿��� 1������ �߰� �Ǿ���.
#else
#ifdef ADD_NEW_MAP_AIDA_20050617
#define MAX_MAP						34		// ���̴� ���� �߰� �Ǿ���.
#else
#ifdef DEVILSQUARE_EXTEND_20050221			// ���������� ���߰�
#define MAX_MAP						33		// ���������� ���� Ȯ�� �Ǿ���.
#else
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
#define MAX_MAP						32		// �� ����� ���� �߰� �Ǿ���.
#else
#ifdef ADD_NEW_MAP_CASTLE_20041115
#define MAX_MAP						31		// �� ���� �߰��Ǿ���.
#else
#ifdef ADD_NEW_MAP_KALIMA_20040518
#define MAX_MAP						30		// �űԸ� Į������ �߰��Ǿ���.
#else
#ifdef CHAOSCASTLE_SYSTEM_20040408 
#define MAX_MAP						24		// ī���� ĳ���� �߰��Ǿ���.
#else
#ifdef BLOODCASTLE_EXTEND_20040314			// ����ĳ�� Ȯ������ ���� ���� �߰��Ǿ���.
#define MAX_MAP						18
#else
#ifdef FOR_BLOODCASTLE
#define MAX_MAP						17
#else
	#ifdef NEW_SKILL_FORSKYLAND
		#define MAX_MAP						11
	#else
		#define MAX_MAP						10
	#endif
#endif		// FOR_BLOODCASTLE
#endif		// BLOODCASTLE_EXTEND_20040314
#endif		// CHAOSCASTLE_SYSTEM_20040408
#endif		// ADD_NEW_MAP_KALIMA_20040518
#endif		// ADD_NEW_MAP_CASTLE_20041115
#endif		// ADD_NEW_MAP_CASTLE_HUNTZONE_20041115
#endif		// DEVILSQUARE_EXTEND_20050221
#endif		// ADD_NEW_MAP_AIDA_20050617
#endif		// ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
#endif		// ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
#endif		// HIDDEN_KALIMA_20050706
#endif		// ADD_NEW_MAP_KANTURU_COMMON_20060523
#endif		// ADD_NEW_MAP_KANTURU_BOSS_20060627
#endif		// UPDATE_GM_FUNCTION_20070228
#endif		// UPDATE_BEFORE_ILLUSION_TEMPLE_MAX_MAP_20070702
#endif		// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#endif		// ADD_SEASON_3_NEW_MAP_20070910
#endif		// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif		// ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
#endif		// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
#endif		// ADD_RAKLION_20080408

#endif		// UPDATE_XMASEVENT_MAP_20080930
#endif		// ADD_NEWPVP_PKFIELD

enum MAP_INDEX
{
	MAP_INDEX_RORENCIA	=	0,
	MAP_INDEX_DUNGEON	=	1,
	MAP_INDEX_DEVIAS	=	2,
	MAP_INDEX_NORIA		=	3,
	MAP_INDEX_LOSTTOWER =	4,
	MAP_INDEX_RESERVED	=	5,
	MAP_INDEX_BATTLESOCCER	= 6,
	MAP_INDEX_ATHLANSE	=	7,
	MAP_INDEX_TARKAN	=	8,
	MAP_INDEX_DEVILSQUARE	= 9,
	MAP_INDEX_ICARUS	=	10,

// ���� ĳ��
	MAP_INDEX_BLOODCASTLE1	= 11,
	MAP_INDEX_BLOODCASTLE2	= 12,
	MAP_INDEX_BLOODCASTLE3	= 13,
	MAP_INDEX_BLOODCASTLE4	= 14,
	MAP_INDEX_BLOODCASTLE5	= 15,
	MAP_INDEX_BLOODCASTLE6	= 16,
#ifdef BLOODCASTLE_EXTEND_20040314
	MAP_INDEX_BLOODCASTLE7	= 17,
#endif


#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �� �ε���
	MAP_INDEX_CHAOSCASTLE1	= 18,
	MAP_INDEX_CHAOSCASTLE2	= 19,
	MAP_INDEX_CHAOSCASTLE3	= 20,
	MAP_INDEX_CHAOSCASTLE4	= 21,
	MAP_INDEX_CHAOSCASTLE5	= 22,
	MAP_INDEX_CHAOSCASTLE6	= 23,
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518		// Į���� �� �ε���
	MAP_INDEX_KALIMA1		= 24,
	MAP_INDEX_KALIMA2		= 25,
	MAP_INDEX_KALIMA3		= 26,
	MAP_INDEX_KALIMA4		= 27,
	MAP_INDEX_KALIMA5		= 28,
	MAP_INDEX_KALIMA6		= 29,
#endif

#ifdef CASTLE_SIEGE_COMMON_PATCH_20041018		// �� �� �ε���
	MAP_INDEX_CASTLESIEGE		= 30,
#endif
	
#ifdef ADD_NEW_MAP_CASTLE_HUNTZONE_20041115		// �� �����
	MAP_INDEX_CASTLEHUNTZONE	= 31,
#endif
	
#ifdef DEVILSQUARE_EXTEND_20050221				// ���������� Ȯ��
	MAP_INDEX_DEVILSQUARE2		= 32,
#endif
#ifdef ADD_NEW_MAP_AIDA_20050617
	MAP_INDEX_AIDA					= 33,		// ���̴� ��
#endif
#ifdef ADD_NEW_MAP_CRYWOLF_FIRSTZONE_20050414
	MAP_INDEX_CRYWOLF_FIRSTZONE		= 34,		// ũ���̿��� 1����
#endif

#ifdef ADD_NEW_MAP_CRYWOLF_SECONDZONE_20050414
	MAP_INDEX_CRYWOLF_SECONDZONE	= 35,		// ũ���̿��� 2����
#endif

#ifdef HIDDEN_KALIMA_20050706
	MAP_INDEX_KALIMA7				= 36,		// ���� Į����
#endif

#ifdef ADD_NEW_MAP_KANTURU_COMMON_20060523
	MAP_INDEX_KANTURU1				= 37,		// ĭ���� 1����
	MAP_INDEX_KANTURU2				= 38,		// ĭ���� 2���� 	
#endif

#ifdef ADD_NEW_MAP_KANTURU_BOSS_20060627
	MAP_INDEX_KANTURU_BOSS			= 39,		// ĭ���� ������	
#endif

#ifdef UPDATE_GM_FUNCTION_20070228
	MAP_INDEX_GM_MAP				= 40,		// GM ��ȯ ����
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� �� �ε��� �߰�
	MAP_INDEX_3RD_CHANGEUP_QUEST			= 41,		// 3�� ���� ����Ʈ ��
	MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST		= 42,		// 3�� ���� ����Ʈ ������
#endif

#ifdef ADD_SEASON_3_NEW_MAP_20070910
	MAP_INDEX_ILLUSION_TEMPLE1				= 45,		// �� 1 ȯ�� ��� �ʹ�ȣ
	MAP_INDEX_ILLUSION_TEMPLE2				= 46,		// �� 2 ȯ�� ��� �ʹ�ȣ
	MAP_INDEX_ILLUSION_TEMPLE3				= 47,		// �� 3 ȯ�� ��� �ʹ�ȣ
	MAP_INDEX_ILLUSION_TEMPLE4				= 48,		// �� 4 ȯ�� ��� �ʹ�ȣ
	MAP_INDEX_ILLUSION_TEMPLE5				= 49,		// �� 5 ȯ�� ��� �ʹ�ȣ
	MAP_INDEX_ILLUSION_TEMPLE6				= 50,		// �� 6 ȯ�� ��� �ʹ�ȣ

	MAP_INDEX_ELBELAND						= 51,		// �ű�ĳ���͸� '��������'
#endif
	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
	MAP_INDEX_BLOODCASTLE8			= 52,	// ����ĳ��8
#endif
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010
	MAP_INDEX_CHAOSCASTLE7			= 53,	// ī����ĳ��7	
#endif
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
	MAP_INDEX_SWAMP_OF_CALMNESS		= 56,	// ����Ǵ�
#endif
#ifdef ADD_RAKLION_20080408
	MAP_INDEX_RAKLION_FIELD			= 57,	// ���� ȣ�� �ʵ�
	MAP_INDEX_RAKLION_BOSS			= 58,	// ���� ȣ�� ���� ��
#endif // ADD_RAKLION_20080408
#ifdef UPDATE_XMASEVENT_MAP_20080930
	MAP_INDEX_XMASEVENT				= 62, //ũ�������� �̺�Ʈ ��
#endif // UPDATE_XMASEVENT_MAP_20080930
#ifdef ADD_NEWPVP_PKFIELD
	MAP_INDEX_VULCANUS				= 63, // ��ī��������
	MAP_INDEX_PKFIELD				= 64, // PK �ʵ�
#endif // ADD_NEWPVP_PKFIELD
};


//////////////////////////////////////////////////////////////////////////
//
//	������ ��Ŷ 
//
//////////////////////////////////////////////////////////////////////////

// 0xDx ����� ����!!!!! ~_~
#ifdef PACKET_CHANGE
	#if GAME_VERSION == G_V_99B
		#define CHINESE_PACKET
		#define PACKET_CHANGE_CHINA_V3
	#elif GAME_VERSION == G_V_S2_2
		#define JAPAN_PACKET
	#elif defined(JAPAN_S4_6)
		#define JAPAN_PACKET
	#elif GAME_VERSION == G_V_S4_6 || GAME_VERSION == G_V_S1_512 || GAME_VERSION == G_V_S3_E1
		#define KOREAN_PACKET
	#endif
	#if GAME_VERSION == G_V_S4_5
		#define JAPAN_PACKET
	#endif

	#ifdef KOREAN_PACKET
		#define PACKET_MOVE				0xD3 
		#define PACKET_POSITION			0xDF 
		#define PACKET_MAGIC_ATTACK		0x10 
		#define PACKET_ATTACK			0xD7
	#endif

	#ifdef JAPAN_PACKET
		#define PACKET_MOVE				0X1D 
		#define PACKET_POSITION			0XD6 
		#define PACKET_MAGIC_ATTACK		0XD7 
		#define PACKET_ATTACK			0XDC
	#endif

	#ifdef CHINESE_PACKET
		#ifdef PACKET_CHANGE_CHINA_V3
			#define PACKET_MOVE				0XD7
#ifdef MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126
			#define PACKET_POSITION			0XD2
#else
			#define PACKET_POSITION			0XD0 
#endif // MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126 
			#define PACKET_MAGIC_ATTACK		0X1D 
			#define PACKET_ATTACK			0XD9
		#else
			#define PACKET_MOVE				0XDD 
			#define PACKET_POSITION			0XD3 
			#define PACKET_MAGIC_ATTACK		0XD9 
			#define PACKET_ATTACK			0XDE
		#endif
	#endif


	#ifdef TAIWAN_PACKET
		#define PACKET_MOVE				0XD6 
		#define PACKET_POSITION			0XDF 
#ifdef MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126
		#define PACKET_MAGIC_ATTACK		0XD2
#else
		#define PACKET_MAGIC_ATTACK		0XD0 
#endif // MODIFY_PACKET_POSITION_PROTOCOL_CODE_20071126
		#define PACKET_ATTACK			0XDD
	#endif

	#ifdef THAILAND_PACKET
		#define PACKET_MOVE				0X11 
		#define PACKET_POSITION			0XD4 
		#define PACKET_MAGIC_ATTACK		0XD9 
		#define PACKET_ATTACK			0XD6
	#endif

	#ifdef GLOBAL_PACKET
		#define PACKET_MOVE				0XD4 
		#define PACKET_POSITION			0X15 
		#define PACKET_MAGIC_ATTACK		0XDB 
		#define PACKET_ATTACK			0X11
	#endif

	#ifdef FOR_PHILIPPINE
		#define PACKET_MOVE				0XDD 
		#define PACKET_POSITION			0XDF 
		#define PACKET_MAGIC_ATTACK		0X11 
		#define PACKET_ATTACK			0XD6
	#endif

	#ifdef FOR_VIETNAM		// ��Ʈ��
		#define PACKET_MOVE				0xD9 
		#define PACKET_POSITION			0xDC 
		#define PACKET_MAGIC_ATTACK		0x1D 
		#define PACKET_ATTACK			0x15
	#endif

	#ifdef FOR_USA
		#define PACKET_MOVE				0XD4 
		#define PACKET_POSITION			0X15 
		#define PACKET_MAGIC_ATTACK		0XDB 
		#define PACKET_ATTACK			0X11
	#endif

#else
	#define PACKET_MOVE				0x10 
	#define PACKET_POSITION			0x11 
	#define PACKET_MAGIC_ATTACK		0x1D 
	#define PACKET_ATTACK			0x15
#endif

//////////////////////////////////////////////////////////////////////////
//
//	����
//
//////////////////////////////////////////////////////////////////////////
#define AUTH_CHECK_00			((char)0x29)
#define AUTH_CHECK_01			((char)0x23)
#define AUTH_CHECK_02			((char)0xBE)
#define AUTH_CHECK_03			((char)0x84)
#define AUTH_CHECK_04			((char)0xE1)
#define AUTH_CHECK_05			((char)0x6C)
#define AUTH_CHECK_06			((char)0xD6)
#define AUTH_CHECK_07			((char)0xAE)
#define AUTH_CHECK_08			((char)0x52)
#define AUTH_CHECK_09			((char)0x90)
#define AUTH_CHECK_10			((char)0x49)
#define AUTH_CHECK_11			((char)0xF1)
#define AUTH_CHECK_12			((char)0xF1)
#define AUTH_CHECK_13			((char)0xBB)
#define AUTH_CHECK_14			((char)0xE9)
#define AUTH_CHECK_15			((char)0xEB)
#define AUTH_CHECK_16			((char)0xB3)
#define AUTH_CHECK_17			((char)0xA6)
#define AUTH_CHECK_18			((char)0xDB)
#define AUTH_CHECK_19			((char)0x00)


//////////////////////////////////////////////////////////////////////////
//
//	���� ����
//
//////////////////////////////////////////////////////////////////////////
enum CHAOS_TYPE
{
	CHAOS_TYPE_DEFAULT		=	1,
	CHAOS_TYPE_DEVILSQUARE	=	2,
	CHAOS_TYPE_UPGRADE_10	=	3,
	CHAOS_TYPE_UPGRADE_11	=	4,
#ifdef ITEM_12_13_20040401
	CHAOS_TYPE_UPGRADE_12	=	22,
	CHAOS_TYPE_UPGRADE_13   =	23,
#endif
	CHAOS_TYPE_DINORANT		=	5,
	CHAOS_TYPE_FRUIT		=	6,
	CHAOS_TYPE_SECOND_WING	=	7,
	CHAOS_TYPE_BLOODCATLE	=	8,
	CHAOS_TYPE_FIRST_WING	=	11,
#ifdef ITEM_12_13_20040401
	CHAOS_TYPE_SETITEM		=	12,
#endif
#ifdef DARKLORD_WORK
	CHAOS_TYPE_DARKHORSE	=	13,
	CHAOS_TYPE_DARKSPIRIT	=	14,
	CHAOS_TYPE_CLOAK		=	24,
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	CHAOS_TYPE_BLESS_POTION =	15, 
	CHAOS_TYPE_SOUL_POTION	=	16,
	CHAOS_TYPE_LIFE_STONE	=	17,
#endif
	
#ifdef CASTLE_SPECIAL_ITEMMIX_20050425
	CHAOS_TYPE_CASTLE_ITEM	=	18,			// ���� �������� Ư���ϰ� ���� ������ ������
#endif

#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// ������ �������� �̺�Ʈ - ��������
	CHAOS_TYPE_HT_BOX		=	20,
#endif
	
#ifdef ADD_ITEM_FENRIR_01_20051110
	CHAOS_TYPE_FENRIR_01	=	25,			// �渱 ���� 1�ܰ� - ���Ǹ� ����
	CHAOS_TYPE_FENRIR_02	=	26,			// �渱 ���� 2�ܰ� - �η��� ���Ǹ�
	CHAOS_TYPE_FENRIR_03	=	27,			// �渱 ���� 3�ܰ� - �渱�� ���Ǹ�
	CHAOS_TYPE_FENRIR_04	=	28,			// �渱 ���� 4�ܰ� - �渱�� ���Ǹ� ���׷��̵�
#endif
	
#ifdef ADD_SHIELD_POINT_01_20060403
	CHAOS_TYPE_COMPOUNDPOTION_LV1	= 30,
	CHAOS_TYPE_COMPOUNTPOTION_LV2	= 31,
	CHAOS_TYPE_COMPOUNTPOTION_LV3	= 32,
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	CHAOS_TYPE_JEWELOFHARMONY_PURITY			= 33,	// ��ȭ�Ǻ��� ���� ����
	CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM  = 34,	// ���ü� ����
	CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM		= 35,	// ��ȭ������ ȯ��
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	
#ifdef ADD_380ITEM_NEWOPTION_20060711
	CHAOS_TYPE_380_OPTIONITEM					= 36,	// 380 ������ ����
#endif // ADD_380ITEM_NEWOPTION_20060711
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ���� �η縶�� ������ ���� Ÿ��
	CHAOS_TYPE_ILLUSION_TEMPLE_ITEM				= 37,	// ���� �η縶�� ������ ����
#endif

#ifdef ADD_THIRD_WING_20070525	// ���� ���� ��ȣ
	CHAOS_TYPE_THIRD_WING_1		= 38,		// 3�� ���� ���� - 1��	
	CHAOS_TYPE_THIRD_WING_2		= 39,		// 3�� ���� ���� - 2��	
#endif	

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	CHAOS_TYPE_LOTTERY_MIX		= 40,
/*
ADD_PCS_LOTTERY_SYSTEM_20070117 ����� �ӽ÷� ó�� 
	#ifdef ADD_PARTIALY_CHARGE_ON
	CHAOS_TYPE_LOTTERY_MIX		= 40,
	#else
	CHAOS_TYPE_LOTTERY_MIX		= 37,
	#endif // ADD_PARTIALY_CHARGE_ON
*/
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	CHAOS_TYPE_CHERRY_MIX		= 41, 
#endif // ADD_EVENT_CHERRY_20080312

#ifdef ADD_ITEM_SOCKET_OPTION_MIX_20080415
	CHAOS_TYPE_SEED_EXTRACT		= 42,
	CHAOS_TYPE_SEED_COMPOSIT	= 43,
	CHAOS_TYPE_SEED_EQUIP		= 44,
	CHAOS_TYPE_SEED_REMOVE		= 45,
#endif // ADD_ITEM_SOCKET_OPTION_MIX_20080415
};

//////////////////////////////////////////////////////////////////////////
//
//	�ɼ� ����
//
//////////////////////////////////////////////////////////////////////////
#ifdef EVENT_OPTION_RELOAD_20040305					// ���� �̺�Ʈ �ɼǸ� �ٽ� �б� ����

enum MU_EVENT_TYPE {
	MU_EVENT_ALL					= 0,			// ��� �̺�Ʈ�� �ɼ��� ����
	
	MU_EVENT_DEVILSQUARE			= 1,			// ����������
	MU_EVENT_BLOODCASTLE			= 2,			// ����ĳ��

	MU_EVENT_ATTACKEVENTTROOP		= 3,			// ���� ����
	MU_EVENT_GOLDENTROOP			= 4,			// Ȳ�� ����
	MU_EVENT_WHITEMAGETROOP			= 5,			// ��������� ����

	MU_EVENT_LOVEPANGPANG			= 6,			// �������� �̺�Ʈ (���Ϳ��Լ� �Ϸ� ���������� �������Ʈ (14,12) ��ӵ�, ��ȣ��� �̺�Ʈ)
	MU_EVENT_FIRECRACKER			= 7,			// ���� �̺�Ʈ (���� ���� �� ���� (14,11) ��ӵ� -> �ѱ��� ��Ʋ�� ���ϵ� Ư��������)
	MU_EVENT_MEDALION				= 8,			// �޴� �̺�Ʈ (�¿��� �� ������ ���� ���޴�, �ݸ޴� ���� ���´�.)
	MU_EVENT_XMASSTAR				= 9,			// ��ź�Ǻ� �̺�Ʈ (���Ϳ��Լ� ��ź�Ǻ��� ��ӵ�)
	MU_EVENT_HEARTOFLOVE			= 10,			// �������Ʈ �̺�Ʈ (���Ϳ��Լ� �������Ʈ�� ��ӵ�)

	MU_EVENT_SAY_HAPPYNEWYEAR		= 11,			// NPC�� �����λ縦 �Ѵ�.
	MU_EVENT_SAY_MERRYXMAS			= 12,			// NPC�� �޸� ũ����������� ���Ѵ�.

#ifdef CHAOSCASTLE_SYSTEM_20040408 
	MU_EVENT_CHAOSCASTLE			= 13,			// ī����ĳ��
#endif
	
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	MU_EVENT_CHRISTMAS_RIBBONBOX	= 14,			// ũ�������� �������� �̺�Ʈ( ���Ϳ��Լ� ����,�ʷ�,�Ķ� �������ڰ� ��ӵ� )
#endif

#ifdef VALENTINES_DAY_BOX_EVENT_20060124
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX	= 15,		// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ( ���Ϳ��Լ� �м�,����,�Ķ� ���ݸ����ڰ� ��ӵ� )
#endif

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	MU_EVENT_WHITEDAY_CANDYBOX		= 16,			// ȭ��Ʈ���� �������� �̺�Ʈ( ���Ϳ��Լ� ������,��Ȳ,��û �������ڰ� ��ӵ� )
#endif

};

#endif


#ifdef ETC_OPTION_RELOAD_20040308					// ���� �ɼ� �� �̺�Ʈ ���� �͵鸸 ���ε�

enum MU_ETC_TYPE 
{
	MU_ETC_ALL						= 0,			// ��� ���ӿ��� �ɼ� ���ε�
	MU_ETC_CREATECHARACTER			= 1,			// �ɸ��� ����
	MU_ETC_GUILD					= 2,			// ��� ����
	MU_ETC_TRADE					= 3,			// �ŷ�
	MU_ETC_USECHAOSBOX				= 4,			// ī�����ڽ� ���
	MU_ETC_PERSONALSHOP				= 5,			// ���λ���
	MU_ETC_PKITEMDROP				= 6,			// PK�� ������ ��ӿ���
	MU_ETC_ITEMDROPRATE				= 7,			// ������ ���Ȯ��
	MU_ETC_SPEEDHACK				= 8,			// ���ǵ���
	MU_ETC_GAMEGUARD				= 9,			// ���Ӱ��� üũ�� üũ����
};

#endif


enum DATA_FILE_INDEX				// !!! . �� ��ȣ�� WzVersionManager ���� GetPrivateProfileString() ���� ���� �� �������ִ� ���� ID��ȣ�� ���ƾ� ��
{
	FILE_ITEM			= 0,		// Item(kor).txt
	FILE_ITEMLOCAL		= 1,		// ������ Item.txt
	FILE_SKILL			= 2,		// Skill(kor).txt
	FILE_SKILLLOCAL		= 3,		// ������ skill.txt
	FILE_QUEST			= 4,		// Quest(kor).txt
	FILE_QUESTLOCAL		= 5,		// ������ Quest.txt
	FILE_CHECKSUM		= 6,		// Checksum.dat
	FILE_MONSTER		= 7,		// Monster.txt
	FILE_GATE			= 8,		// Gate.txt
	FILE_MONSTERSETBASE	= 9,		// MonsterSetBase.txt
	FILE_NPGGAUTH		= 10,		// CSAuth.dat
	FILE_SHOP0			= 11,		// Shop0.txt
	FILE_SHOP1			= 12,		// Shop1.txt
	FILE_SHOP2			= 13,		// Shop2.txt
	FILE_SHOP3			= 14,		// Shop3.txt
	FILE_SHOP4			= 15,		// Shop4.txt
	FILE_SHOP5			= 16,		// Shop5.txt
	FILE_SHOP6			= 17,		// Shop6.txt
	FILE_SHOP7			= 18,		// Shop7.txt
	FILE_SHOP8			= 19,		// Shop8.txt
	FILE_SHOP9			= 20,		// Shop9.txt
	FILE_SHOP10			= 21,		// Shop10.txt
	FILE_SHOP11			= 22,		// Shop11.txt
	FILE_SHOP12			= 23,		// Shop12.txt

	FILE_MOVEREQ		= 24,		// movereq(local).txt
	FILE_MOVEREQLOCAL	= 25,		// ������ movereq.txt
	FILE_SETITEMOPTION	= 26,		// itemsetoption(Kor).txt
	FILE_SETITEMOPTIONLOCAL	= 27,	// (������) itemsetoption.txt
	FILE_SETITEMTYPE	= 28,		// itemsettype(Kor).txt
	FILE_SETITEMTYPELOCAL	= 29,	// (������) itemsettype.txt	

	// ������ SHOP�̳� �ٸ� �����͸� �߰��� ���� ���� ��û��ȣ�� �и��� �ʵ��� �ڷ� ���� �Ѵ�.
#ifdef ADD_FIRECRACKER_NPC_20070306
	FILE_SHOP13			= 30,		// Shop13.txt
#endif
#ifdef ADD_SHOP_14_20070308	
	FILE_SHOP14			= 31,		// Shop14.txt
#endif
#ifdef ADD_SEASON_3_NEW_MAP_20070910	// �������� �������
	FILE_SHOP15			= 32,		// Shop15.txt
	FILE_SHOP16			= 33,		// Shop16.txt
	FILE_SHOP17			= 34,		// Shop17.txt
#endif
#ifdef ADD_GAMBLING_20090120		// �׺� ����
	FILE_SHOP18			= 35,		// Shop18.txt
#endif // ADD_GAMBLING_20090120
};


// ���� �̺�Ʈ
#define EVENTMASK_DevilSquareEvent			0x00000001		// ����������
#define EVENTMASK_BloodCastleEvent			0x00000002		// ����ĳ��

// ���� �̺�Ʈ
#define EVENTMASK_XMasEvent					0X00000100		// ��ź�Ǻ�
#define EVENTMASK_FireCrackerEvent			0X00000200		// ����
#define EVENTMASK_HeartOfLoveEvent			0X00000400		// ����� ��Ʈ
#define EVENTMASK_MedalEvent				0X00000800		// �޴��̺�Ʈ
#define EVENTMASK_RingAttackEvent			0X00001000		// �����̺�Ʈ
#define EVENTMASK_EventChipEvent			0X00002000		// �����̺�Ʈ
#define EVENTMASK_IsEledoradoEvent			0X00004000		// Ȳ�������̺�Ʈ

// ��Ÿ ����
#define EVENTMASK_UseNPGGChecksum			0X00010000	// ���Ӱ��� ��� 



///////////////////////////////////////////////////////////////////
//
//	���� �ý��� (apple)
//
///////////////////////////////////////////////////////////////////////
#ifdef ADD_NEW_GUILD_01_20040913

#define MIN_UNION_MASTER_GUILD_MEMBERCOUNT	20
#define MAX_RELATIONSHIP_COUNT				100 
#define INVALID_USER_INDEX					-1

#ifdef MODIFY_MAX_UNION_MEMBER_01_20050418

	#ifdef FOR_KOREA
		#define MAX_UNION_MEMBER_COUNT				3
		#define MAX_TESTSERVER_UNION_MEMBER_COUNT	3
	#else
		#define MAX_UNION_MEMBER_COUNT				5
		#define MAX_TESTSERVER_UNION_MEMBER_COUNT	5
	#endif

#else

#define MAX_UNION_MEMBER_COUNT				7
#define MAX_TESTSERVER_UNION_MEMBER_COUNT	12

#endif


enum GUILD_NOTIFICATION
{
	G_NOTIFICATION_SET				= 0x01,
	G_NOTIFICATION_UNION_BREAKOFF	= 0x10

};

enum GUILD_OPERATION_ERRORCODE
{
	G_ERROR_NONE = 0

	// 0x10~ Common Result

};

enum GUILD_RELATION_OPERATION
{
	G_RELATION_OPERATION_JOIN		= 1,
	G_RELATION_OPERATION_BREAKOFF	= 2
};

enum GUILD_RELATIONSHIP
{
	G_RELATIONSHIP_NONE			= 0,
	G_RELATIONSHIP_UNION		= 0x01,
	G_RELATIONSHIP_RIVAL		= 0x02,
	G_RELATIONSHIP_UNIONMASTER	= 0x04,
	G_RELATIONSHIP_RIVALMASTER	= 0x08,

};

enum GUILD_TYPE
{
	G_TYPE_NONE			= -1,
	G_TYPE_COMMON		= 0,
	G_TYPE_GUARD		= 1		// ��ȣ ���.
//	G_TYPE_MERCENARY	= 1,
//	G_TYPE_CLAN			= 2
};

enum GUILD_STATUS
{
	G_NONE			= -1,
	G_PERSON		= 0,		// �Ϲ�
	G_MASTER		= 128,		// ��帶����
	G_SUB_MASTER	= 64,		// �α�帶����
	G_BATTLE_MASTER = 32		// ��Ʋ������
};

enum GUILD_REQ_COMMON_RESULT
{
	// ��尡 ����
	GUILD_ANS_NOTEXIST_GUILD			= 0x10,
	// ���������� ���Ͽ� ���ձ���� ���ѵ˴ϴ�.
	GUILD_ANS_UNIONFAIL_BY_CASTLE		= 0x10,
	// ���� ����
	GUILD_ANS_NOTEXIST_PERMISSION		= 0x11,

	// �Ӹ� �� �� �ִ� ����� ����
	GUILD_ANS_NOTEXIST_EXTRA_STATUS		= 0x12,
	// �Ӹ� �� �� �ִ� Ÿ���� ����
	GUILD_ANS_NOTEXIST_EXTRA_TYPE		= 0x13,			
	
	// ���� �� & �� 
	GUILD_ANS_EXIST_RELATIONSHIP_UNION	= 0x15,
	GUILD_ANS_EXIST_RELATIONSHIP_RIVAL	= 0x16,

	// ���� �� & ��
	GUILD_ANS_EXIST_UNION				= 0x17,
	GUILD_ANS_EXIST_RIVAL				= 0x18,
	GUILD_ANS_NOTEXIST_UNION			= 0x19,
	GUILD_ANS_NOTEXIST_RIVAL			= 0x1A,

	// ���� �����Ͱ� �ƴ�
	GUIDL_ANS_NOT_UNION_MASTER			= 0x1B,

	// ���� ��尡 �ƴ�
	GUILD_ANS_NOT_GUILD_RIVAL			= 0x1C,
	
	// ������ ���� ������ ����� ������ ������ ����.
	GUILD_ANS_CANNOT_BE_UNION_MASTER	= 0x1D,

	// �ִ� ��� ���� ���� �ʰ� ��
	GUILD_ANS_EXCEED_MAX_UNION_MEMBER	= 0x1E,

	// ��û�� ��ҵǾ����ϴ�.
	GUILD_ANS_CANCLE_REQUEST			= 0x20

};

enum EXDB_DATA_TRANSFER_TYPE
{
	EX_SEND_P2P			 = 0x01,		// ��û ��������
	EX_SEND_BROADCASTING = 0x00			// ��ε�ĳ����
};

enum EXDB_RESULT
{
	EX_RESULT_SUCCESS			= 0x01,
	EX_RESULT_FAIL				= 0x00,
	EX_RESULT_FAIL_FOR_CASTLE	= 0x10,	// GUILD_ANS_UNIONFAIL_BY_CASTLE �� ����.
};
#endif // ADD_NEW_GUILD_01_20040913

//////////////////////////////////////////////////////////////////////////
//
//	���� ���� ���� ����
//
//////////////////////////////////////////////////////////////////////////
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

enum FRUIT_ITEM_TYPE
{
	FRUIT_ENERGY		= 0,
	FRUIT_VITALITY		= 1,
	FRUIT_DEXTERITY		= 2,
	FRUIT_STRENGTH		= 3,
	FRUIT_LEADERSHIP	= 4
};

enum STAT_FRUIT_USING_RESULT
{
	STAT_PLUS_SUCCESS			= 0x00,	// ���Ȼ��� ����
	STAT_PLUS_FAILED			= 0x01, // ���Ȼ��� ����
	STAT_PLUS_PREVENTED			= 0x02,	// ���Ÿ� Stat Plus�� ��� �Ұ�

	STAT_MINUS_SUCCESS			= 0x03,	// ���Ȱ��� ����
	STAT_MINUS_FAILED			= 0x04,	// ���Ȱ��� ����
	STAT_MINUS_PREVENTED		= 0x05,	// ���Ÿ� Stat Minus�� ��� �Ұ�

#ifdef MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	STAT_MINUS_EX_SUCCESS		= 0x06,	// ���¿��� ��� �ڵ� ����
	STAT_MINUS_EX_FAILED		= 0x07, // ���¿��� ��� �ڵ� ����
	STAT_MINUS_EX_PREVENTED		= 0x08,	// ���¿��� ��� �Ұ�
#endif // MODIFY_RESET_FRUITS_RESULT_TYPE_20070821
	
	STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT = 0x10,	// �������� �����ؼ� ���Ұ���

	STAT_PLUS_PREVENTED_BY_MAX		= 0x21,	// ���� �ִ�ġ���� ��� ����
	STAT_MINUS_PREVENTED_BY_MAX		= 0x25,	// ���� �ִ�ġ���� ��� ����
	STAT_MINUS_PREVENTED_BY_DEFAULT	= 0x26,	// �⺻�� ���Ϸδ� ���� �� �� ����
};

#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
enum MAP_MOVE_RESULT
{
	MAPMOVE_SUCCESS					= 0,				// �̵� ����
		MAPMOVE_FAILED,										// �̵� ���� (�̵� �Ұ����� �ٸ� ���)
		MAPMOVE_FAILED_TELEPORT,							// �̵� ���� (�ڷ���Ʈ ��)
		MAPMOVE_FAILED_PSHOP_OPEN,							// �̵� ���� (���λ����� �����ִ� ���)
		MAPMOVE_FAILED_RECALLED,							// �̵� ���� (��ȯ�Ǵ� ����)
		MAPMOVE_FAILED_NOT_ENOUGH_EQUIP,					// �̵� ���� (�ش� ������ �̵��ϱ� ���� ��� ����)
		MAPMOVE_FAILED_WEARING_EQUIP,						// �̵� ���� (�ش� �ʿ� ����� ������� ���ϴ� ��� ����)
		MAPMOVE_FAILED_MURDERER,							// �̵� ���� (���θ� �� ��� �� �̵� �Ұ�)
		MAPMOVE_FAILED_NOT_ENOUGH_LEVEL,					// �̵� ���� (���� ����)
		MAPMOVE_FAILED_NOT_ENOUGH_ZEN,						// �̵� ���� (���� �� ����)
		MAPMOVE_FAILED_EVENT_END,							// �̵� ���� (�ش� ���� �̺�Ʈ�� �̹� ����Ǿ���)
		MAPMOVE_FAILED_HACKTOOL_BLOCK,						// �̵� ���� (���� ����ڷ� �� �̵��� ���ܵǾ���)
		MAPMOVE_FAILED_CHECKSUM_ERROR,						// �̵� ���� (����ڰ� ���� üũ�� ���� ���� �ʴ´�.)
		MAPMOVE_FORCED_EVENT_END		= 20,				// ���� �̵� (�̺�Ʈ ���ῡ ���� ���� �̵�)
		MAPMOVE_FORCED_GM									// ���� �̵� (GM�� ���� ���� �̵�)
};

#endif // ADD_MAPMOVE_PROTOCOL_20090327

#endif


