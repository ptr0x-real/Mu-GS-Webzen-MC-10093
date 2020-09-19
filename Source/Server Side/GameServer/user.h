#ifndef __USER_H__
#define __USER_H__

#include <winsock2.h>

#include "Common\zzzItem.h"
#include "Magicinf.h"
#include "HitC.h"
#include "wsGameServer.h"
#include "MapItem.h"
#include "giocp.h"
#include "GuildClass.h"
#include "CLoginCount.h"

#include "EngagePVP.h"

#ifdef PCBANG_KUZIMO
#include "MDate.h"
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	#include "CSAuth.h"

#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
//	#include "Include\ggsrv.h"		
	#include "ggsrv.h"						// ���� ��Ʈ�� ���� (b4nfter)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	#include "ggsrv25.h"						// ���� ��Ʈ�� ���� (hnine)
#endif // NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123

#ifdef ADD_SKILL_WITH_COMBO
#include "ComboAttack.h"
#endif

//#ifdef EVENT_LOTTO
#define ITEMMAKE_INVENTORY_1CELL	235		// DB�������� �ø������ 1ĭ¥�� �������� �κ��� �߰�		
//#endif // EVENT_LOTTO

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#define ITEMMAKE_INVENTORY_CASHITEM	236		// ĳ�� ������ ������ �ø��� �߱��� ���� �ε���
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117f

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// ����� �������� �������� �����Ͽ� �κ��� �߰�
#define ITEMMAKE_INVENTORY_COINITEM 237		
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029

#ifdef ADD_GAMBLING_20090120
#define ITEMMAKE_INVENTORY_GAMBLINGITEM 234
#endif // ADD_GAMBLING_20090120

#ifdef MONSTER_HERD_SYSTEM_20031120
class MonsterHerd;							// OBJECTSTRUCT ����ü�� MonsterHerd * ����� ���� ���� �������� ����
#endif

#ifdef DARKLORD_WORK
#include "SkillDelay.h"
#endif


#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
#include "TDurMagicKeyChecker.h"
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillElementInfo.h"
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
#include "TMonsterAI.h"
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#include "JewelOfHarmonySystem.h"
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#include "ItemSystemFor380.h"
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef PCBANG_POINT_SYSTEM_20070206		// ���� ����
#include "PCBangPointSystem.h"
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// "QuestInfo.h"
#include "QuestInfo.h"
#endif

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
#include "BuffEffectSlot.h"
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
#include "MasterSkillSystem.h"
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415


#ifdef HACKTOOL_KUZIMO
#include "PacketStatistics.h"
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#include "HackToolBlock.h"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

// ����ĳ���� ������ CtlCode �� 0x06���� �����ϸ� ����Կ��� ���� ����ĳ���Ͱ� �ȴ� (apple)
// ����ĳ���� ������ CtlCode �� 0x0E���� �����ϸ� ����Կ�+���� ����ĳ���Ͱ� �ȴ� (apple)

// ����
// gObj->Authority
#define AUTHORITY_USER					0x00000001		// �Ϲ� ���� 
#define AUTHORITY_ADMIN					0x00000002		// ������ 
#define AUTHORITY_VIP_USER				0x00000004		// ���Ӽ��� ������
//#define AUTHORITY_GM					0x00000008		// ���� ������
// #define AUTHORITY_SUBGM					0x00000010		// ���� ���� ������
//#ifdef CHINA_GM_EVENT
#define AUTHORITY_EVENT_GM				0x00000020		// ���� ���� ������
#define AUTHORITY_SUPER_ADMIN			0x00000040		// ���� ������.
#define AUTHORITY_DISABLE				0x00000080
//#endif

// ���� �ڵ�
// ��ɾ �޾����� ����� �Է��� �ڿ� ���� ������ �ִ��� üũ�ϱ� ����
/*
#define AUTHORITY_CODE_NOTICE			0x00000001		// ���� �����Ѱ�?
#define AUTHORITY_CODE_ACCOUNTBLOCK		0x00000002		// ���� �� �����Ѱ�?
#define AUTHORITY_CODE_DISCONNECT		0x00000004		// ���� ���� ����
#define AUTHORITY_CODE_SETPOSITION		0x00000008		// ��ġ �̵�
#define AUTHORITY_CODE_COPYCHAR			0x00000010		// ĳ���� ����&����
#define AUTHORITY_CODE_CHATBAN			0x00000020		// ä�� ���� ��Ų��

#define	AUTHORITY_CODE_FULLCONTROL		0xFFFFFFFF		// ��� ��� ����
*/
// ó���ڵ�
// gObj->Penalty Chracter = CtlCode
// ĳ���Ϳ� �̷��� ó�� �ڵ尡 �ִٸ� �� �ش� ��ɿ� ������ ������.
#define CTLCODE_ACCOUNTBLOCK			0x00000001		// ���� ��
#define CTLCODE_CHATBAN					0x00000002		// ä�� ����
#define CTLCODE_ITEMDONTTOUCH			0x00000004		// �����ۿ� �մ��� ���ϰ� �Ѵ�.
#define CTLCODE_ACCOUNTITEMBLOCK		0x00000008		// ���� ������ �� (���� �ȿ����� ������ �̵��� ����)

#ifdef CHINA_GM_EVENT
#define CTLCODE_EVENT_GM				0x00000010		// ���� ���� ������
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
#define CTLCODE_EVENT_GM				0x00000010		// ���� ���� ������
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef JAPAN_DESTROYKING_EVENT_20040625
#define CTLCODE_EVENT_GM				0x00000010		// ���� ���� ������
#endif
// 

#define CTLCODE_TRANSFER_SERVER			0x00000080		// �������� ĳ����

#define OPTION_TRADE					0x00000001		// �ŷ� ���� 1�϶��� �ŷ� �� �� �ִ�.

#ifdef DUEL_SYSTEM_20031028
#define OPTION_DUEL						0x00000002		// ���� ���� 1�϶��� ���� �� �� �ִ�.
#endif

// ĳ���� ����..
// gObj[].m_State
#define OBJST_NONE				0x0000		// 
#define OBJST_CREATE			0x0001		// ĳ���� ����
#define OBJST_PLAYING			0x0002		// ������ �������̴�.
#define OBJST_DYING				0x0004		// �׾�� �ִ�.
#define OBJST_DIECMD			0x0008		// �׿���!!
#define OBJST_DIED				0x0010		// �׾���.
#define OBJST_DELCMD			0x0020		// �����̵��� ���� ����Ʈ���� ����


// Client Service State ( Ŭ���̾�Ʈ�� ���� ���¸� ��Ÿ����.)
#define CSS_LOGIN				1		// ������ ���� �ȵǾ�����..  ������Ʈ�� �Ҵ��
#define CSS_LOGINOK				2		// ������ ���´�
#define CSS_CHARACTERVIEW		2		// ĳ���� ����Ʈ ���¿� �ִ�.^
#define CSS_GAMEPLAYING			3		// ���� �÷��� �� �̴�.

#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
#define CSS_LOGINWAIT			-1		// ���� ��û�� �� �� ��� ����
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801

// ���� ���� ��Ʈ���� ���
#define CL_EXIT					0		// ���ӿ��� ����
#define CL_BACK					1		// �������� ���ư���
#define CL_SERVERLIST			2		// ���� �������� ����

// �޽��� ó�� ����
#define MSG_BEATTACK			2		// ���ݴ��ߴ�.
#define MSG_FILLENERGY			3		// �������� ä���.
#define MSG_GUILDWAREND			4		// ������� ������ �˸���.
#define MSG_BSSTART				5		// ��Ʋ��Ŀ ���� ����
#define MSG_BSPAUSE				6		// ��Ʋ��Ŀ ���� ��� ����
#define MSG_BSSTOP				7		// ��Ʋ��Ŀ ���� �ߴ�
#define MSG_REFLECT				10		// ���� �ݻ�

#define MSG_LIFECHECK			11		// ����üũ

// _MODIFY_ADD_THIRD_WING_20070623
#ifdef ADD_THIRD_WING_20070525
#define	MSG_ATTACK_TURNING		12		// ���ݷ� ������
#define MSG_FULL_LIFE			13		// �������� ���� ȸ��
#define MSG_FULL_MANA			14		// �������� ���� ȸ��
#endif

#define MSG_BILLSEND			1000	// ���� ������ ������.

#define MSG_ATTACKPROC_ENTIREMAGIC		50		// ��ü ����

#ifdef DARKLORD_WORK
#define MSG_ATTACKPROC_DARKSPRITATTACK	51		// ��ũ���Ǹ� ����
#endif

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
#define MSG_ATTACKPROC_WEAPONATTACK		52		// ���� ���� ����
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
#define MSG_ATTACKPROC_FENRIR			53		// �渱 ����
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// MSG_ATTACKPROC_FIRESCREAM
#define MSG_ATTACKPROC_FIRESCREAM		54		// ���̾� ��ũ�� ���������� ���� ����
#endif

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	// RKFKA 20060728 ��ȣ ����
	#define MSG_ATTACK_NORMAL			55		// �������ִ� �Ϲ� ���� ����
	#define	MSG_MONSTER_SKILL_POISON	56		// �������ִ� ��ų ����
	#define	MSG_MONSTER_SKILL_SPRING	57		// �������ִ� �б� ����
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// LighingOrb�� ���� Splash Damage �޽���
#define MSG_ATTACK_LIGHTNINGORB			58
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// pullution�� ���� �޽���
#define MSG_SKILL_PULLUTION1			59
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425

// ���� ����
#define	CHANGEUP_MASK			0x07	// Class���� 2,3,4bit�� ����Ѵ�

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 3�� ���� ĳ���� ��ȣ ����
#define GRAND_MASTER		3		// �׷��帶���� (���� 3������)
#define BLADE_MASTER		19		// ���̵帶���� (��� 3������)
#define HIGH_ELF			35		// ���̿���	(���� 3������)
#define DUEL_MASTER			50		// ��󸶽��� (���˻� 3������)
#define LORD_EMPEROR		66		// �ε忥�۷� (��ũ�ε� 3������)
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 2,3������ ��ȣ����
#define BLOODY_SUMMONER		81		// ���� ���ӳ�
#define DIMENSION_MASTER	83		// ���� ������ (��ȯ���� 3������)
#endif

typedef struct tagActionState
{
	DWORD Rest:1;		// ����.
	DWORD Attack:1;		// �����ϴ�.
	DWORD Move:1;		// �����̴�.
	DWORD Escape:1;		// ����ġ��.
	DWORD Emotion:4;	// ����, 0:����, 1:���, 2:������, 4:�������Ҵ�..
	DWORD EmotionCount:8;// ���� ī���ͷμ� 1Tick�� �ѹ��� �����Ѵ�. 0�� ��������� �����̵ȴ�.
} ACTION_STATUS, *LPACTION_STATUS;


#ifdef EXPAND_INTERFACE_BYTE_TO_DWORD_20041209
typedef struct tagInterfaceState
{
	DWORD  use:2;		// ���Ǿ�������..
	DWORD  state:4;		// 0: ��û, 1: ������
	DWORD  type:10;		// 1: �κ��丮, 2:��Ƽ, 3:����
	
} INTERFACE_STATUS, *LPINTERFACE_STATUS;
#else
typedef struct tagInterfaceState
{
	BYTE  use:2;		// ���Ǿ�������..
	BYTE  state:2;		// 0: ��û, 1: ������
	BYTE  type:4;		// 1: �κ��丮, 2:��Ƽ, 3:����
	
} INTERFACE_STATUS, *LPINTERFACE_STATUS;
#endif

typedef struct
{
	int		MsgCode;	// �޽��� �ڵ�
	int		SendUser;	// �޽����� ���� ����
	DWORD	MsgTime;	// �޽��� �����ð�
	int		SubCode;	// ����� ���Ǵ� �ڵ�
} MessageStateMachine;


typedef struct
{
	int		MsgCode;	// �޽��� �ڵ�
	int		SendUser;	// �޽����� ���� ����
	DWORD	MsgTime;	// �޽��� �����ð�
	int		SubCode;	// ����� ���Ǵ� �ڵ�
	int		SubCode2;	// �����ڵ� 2
} ExMessageStateMachine;


extern CLoginCount	    gLCount[];

//----------------------------------------------------------------------------
// ���� Ŭ���� 
//----------------------------------------------------------------------------
class BILL_CLASS
{

private:

#ifdef UPDATE_UNIFICATIONBILL_20090203
	BYTE btDeductType;
	DWORD dwRestPoint;
	DWORD dwRestTime;
#else	// UPDATE_UNIFICATIONBILL_20090203	
	char cCertifyType;
	BYTE PayCode;
	int  EndTime;
#endif	// UPDATE_UNIFICATIONBILL_20090203

	char EndsDays[13];

public:

	BILL_CLASS()
	{
		Init();
	};
	
	void Init()
	{
#ifdef UPDATE_UNIFICATIONBILL_20090203
		btDeductType	= BILL_TYPE_NONE;
		dwRestPoint		= 0;
		dwRestTime		= 0;
#else	// UPDATE_UNIFICATIONBILL_20090203
		cCertifyType	= -1;
		PayCode			= 0;
		EndTime			= 0;
#endif	// UPDATE_UNIFICATIONBILL_20090203

		EndsDays[0]		= '\0';
	};

	// ������ �����Ѵ�.
#ifdef UPDATE_UNIFICATIONBILL_20090203
	BOOL SetBill(BYTE deductType, DWORD rest_point, DWORD rest_time, char *ends_days)
	{
		BOOL bRet=FALSE;
		
		if( btDeductType == BILL_TYPE_NONE ) bRet = TRUE;
		btDeductType		= deductType;
		
		dwRestPoint			= rest_point;
		dwRestTime			= rest_time;
		strcpy(EndsDays, ends_days);
		
		return bRet;
	};
#else	// UPDATE_UNIFICATIONBILL_20090203
	BOOL SetBill(BYTE certify_type, BYTE pay, DWORD end_time, char *ends_days)
	{
		BOOL bRet=FALSE;
		
		if( cCertifyType == -1 ) bRet = TRUE;
		cCertifyType	= certify_type;
		PayCode			= pay;
		EndTime			= end_time;
		strcpy(EndsDays, ends_days);
		return bRet;
	};
#endif	// UPDATE_UNIFICATIONBILL_20090203
	
	// �ð��� ����Ǿ���?
	BOOL IsEndTime()
	{
#ifdef UPDATE_UNIFICATIONBILL_20090203
		if( btDeductType == BILL_TYPE_NONE             )	return FALSE;
		if( btDeductType == BILL_TYPE_PCBANG_QUANTITY )		return FALSE;
		if( btDeductType == BILL_TYPE_PERSONAL_QUANTITY )   return FALSE;
		
		// �ð��� �ٵǾ��ٸ�.
		if( dwRestTime < 0 )								return TRUE;
#else	// UPDATE_UNIFICATIONBILL_20090203
		if( cCertifyType == -1             )    return FALSE;
		if( cCertifyType == CERTIFYTYPE_IP )    return FALSE;
		if( PayCode      == BILLTYPE_JUNGYANG ) return FALSE;
		
		// �ð��� �ٵǾ��ٸ�.
		if( EndTime < 0 )						return TRUE;
#endif	// UPDATE_UNIFICATIONBILL_20090203	
		
		return FALSE;
	};

#ifndef UPDATE_UNIFICATIONBILL_20090203
	BYTE  GetPayCode()  { return PayCode; };
#endif	// UPDATE_UNIFICATIONBILL_20090203

	char* GetEndsDays() { return EndsDays; };

#ifdef UPDATE_UNIFICATIONBILL_20090203
	DWORD	GetRestTime()	{ return dwRestTime; }
	DWORD	GetRestPoint()	{ return dwRestPoint; }
	BYTE	GetDeductType()	{ return btDeductType; }
#else	// UPDATE_UNIFICATIONBILL_20090203
	int   GetEndTime()  { return EndTime; };
	int   GetCertify()  { return cCertifyType; };
#endif	// UPDATE_UNIFICATIONBILL_20090203
} ;
//----------------------------------------------------------------------------


#define MAXMESSAGE				10		// �Ϲ� �޽��� 
#define MAXATTACKMESSAGE		50		// ����ó�� �޽���

extern MessageStateMachine	gSMMsg[][MAXMESSAGE];	// �޽���

#ifdef ADD_SKILL_WITH_COMBO
extern ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// �޽���
#elif defined(NEW_ATTACK_PROC)
extern ExMessageStateMachine	gSMAttackProcMsg[MAX_USER][MAXATTACKMESSAGE];	// �޽���
#endif

extern BILL_CLASS	m_ObjBill[];
//PICKUP
struct PICK_UP_ITEMS
{
	int Type;
	int Index;
	int Item;

};

#ifdef MEM2_KUZIMO
	class ZeroMemBase
	{
	public:	
		void	*operator new[] (size_t);
		void	operator delete[] (void*);
	};

	class OBJECTSTRUCT : public ZeroMemBase
	{
	public:
#else
	typedef struct OBJECTSTRUCT
	{
#endif
	int		m_Index;					// �ڽ��� �迭��ȣ

	//-----------------------------------------------------------------------
	// ���� ���� �� ����	
	int		Connected;					// ���ӵǾ� �ֳ�?	1�̸� ���ӵǾ���.. 2�̸� ������ �޾Ұ� 3�̸� ���� ������..

	char	LoginMsgSnd;			// ���Ӹ޽��� ���� �Ķ�� 1, �����޽����� �ޱ� 
	char	LoginMsgCount;			// ������ ��� ����ó�? ���� 3���̸� ������ �����Ų��.
	
	char	CloseCount;				// ���� ����� ī����..  1�̸� ���� ��Ŵ (�ð� ī���ʹ� CloseCount-1)
	char	CloseType;				// ���� Ÿ��
	
#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	BOOL	bEnableDelCharacter; 	// ĳ���� ������ �����Ѱ�?
#endif

	//-----------------------------------------------------------------------
	// SOCKET && NETWORK
	PER_SOCKET_CONTEXT *PerSocketContext;		
	SOCKET				m_socket;
	char				Ip_addr[16];				// ip�ּ�

	int  UserNumber;				// �������������� ���� ��ȣ
	int  DBNumber;					// DB�� ����Ǿ� �ִ� ������ ���� ��ȣ(Guid)	

#ifdef ADD_PROTOCOL_PCBANG_GUID	
	int		PcBangGuid;				// PC�� Guid (0�̸� �Ǿ��� ����� �ƴ�)
#endif	
	BYTE Magumsa;					// ���˻縦 ����� �ִ� �����ΰ�?

	//-----------------------------------------------------------------------
	// Tick �� �ð�����
	DWORD AutoSaveTime;				// �ڵ� ���� �ð�
	DWORD ConnectCheckTime;			// ����Ǿ����� üũ�ϴ� Ÿ��
	DWORD CheckTick;				// Tick ����ȭ�� ���Ǳ� ���� ���ӽÿ� Ŭ���̾�Ʈ Tick�� ����
	BYTE  CheckSpeedHack;
	DWORD CheckTick2;
	BYTE  CheckTickCount;			// Tick ����ȭ�� ���Ǵ� ī��Ʈ��

#if defined ( MODIFY_HACKLOG_OF_ATTACK_20050518 ) || defined ( MU_CHN_HACKLOG3_MODIFY_20041117 )
	DWORD	dwPlayTime;
#endif
	
#ifdef ITEMSAVE_MODIFY_FORSTATICS
	DWORD SaveTimeForStatics;		// ĳ���� ��踦 ���ؼ� ���� �ð��� ������ �ִٰ� �˸��� ���� �ð�(1�ð�����)
#endif

	int		iPingTime;
	BYTE	m_TimeCount;			// ������ ���Ҹ� ���� Ÿ�̸�
	DWORD	m_dwPKTimer;			// PK���� ������ ���� Ÿ�̸�
	
	//-----------------------------------------------------------------------
	// üũ��
	short CheckSumTableNum;
	DWORD CheckSumTime;

	//------------------------------------------------------------------------
	// 
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD	Type;						// Ÿ��: 0�̸� ����, 1�̸� ����
#else
	BYTE	Type;						// Ÿ��: 0�̸� ����, 1�̸� ����
#endif
	BYTE	Live;
	
	//-----------------------------------------------------------------------
	// ĳ���� ����
	char	CharacterPos;				// ���� �������� ĳ���� ��ȣ(5����)

	char	AccountID[MAX_IDSTRING+1];	// ���� ���̵�
	char	Name[MAX_IDSTRING+1];		// ĳ���� �̸� 
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424
	char	LastJoominNumber[MAX_JOOMINNUMBER_LENGTH+1];		// �ֹι�ȣ
#else
	char	LastJoominNumber[14];		// �ֹι�ȣ
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

#ifdef SECOND_QUEST
	BYTE	PlusStatQuestClear;			// ����Ʈ�� Ŭ���� �Ǹ�, �������� +1 �����߰�
	BYTE	ComboSkillquestClear;		// �޺� ��ų ����Ʈ Ŭ����
#endif

#ifdef ADD_SKILL_WITH_COMBO
	ComboSkillData	comboSkill;
#endif

	//------------------------------------------------------------------------
	// �⺻ ����
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD	Class;				// ���� - ���ʹ� �������� ����
#else
	BYTE	Class;				// ���� - ���ʹ� �������� ����
#endif
	BYTE    DbClass;			// �������� ����

#ifdef NEW_SKILL_FORSKYLAND
	BYTE	ChangeUP;			// ��������
#else
	BYTE    Skin;				// ��Ų (ĳ���Ͱ� �԰��ִ� �⺻ �� ����)
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507
	BYTE	ThirdChangeUp;		// 3�� ��������
#endif	

	short   Level;				// ����
	int     LevelUpPoint;		// ������ ����Ʈ

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	int		iFruitPoint;		// ���� ����Ʈ
#endif

	DWORD	Experience;			// ����ġ
	DWORD	NextExp;			// ���� ����ġ
	
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ĳ���� �⺻ ���� ����
	short	m_nMasterLevel;					// �����ͷ���	

	INT64	m_i64MasterLevelExp;			// �����ͷ��� ����ġ
	INT64	m_i64NextMasterLevelExp;		// ���� �����ͷ��� ����ġ

	int		m_iMasterLevelPoint;			// �����ͷ��� ����Ʈ

	BOOL	m_bMasterLevelDBLoad;			// �����ͷ��� ������ ���������� �ҷ��Դ��� ����
#endif

	int		Money;				// ��
	WORD	Strength;			// ��
	WORD	Dexterity;			// ��ø�� 
	WORD	Vitality;			// �ǰ�
	WORD	Energy;				// ������
	float	Life;				// ���� ����
	float	MaxLife;			// ���� �ִ� ����
	
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int		m_iScriptMaxLife;	// Monster.txt �� ��ũ���� �� �ִ� ����
#endif
	
	float   FillLife;			// ä���� �ϴ� ����
	float   FillLifeMax;		// ä���� �ϴ� ����
	float	Mana;				// ���� ����
	float	MaxMana;			// ���� �ִ� ������
	
#ifdef DARKLORD_WORK	
	WORD	Leadership;			// ���
	WORD	AddLeadership;		// �߰� ���
	WORD	ChatLitmitTime;		// ä�� ���� �ð�
	BYTE	ChatLimitTimeSec;	// ä�� ���� �ð�(��)
#endif

#ifdef ADD_HP_POTION_LEVEL1_20040401
	BYTE	FillLifeCount;		// ä���� �ϴ� ���� <- ������� ä����̳�?
#endif

	WORD	AddStrength;			// �߰���
	WORD	AddDexterity;			// �߰���ø�� 
	WORD	AddVitality;			// �߰�ü��
	WORD	AddEnergy;				// �߰�������

#ifdef NEW_FORSKYLAND2
	int		BP;					// AG �Ŀ�
	int		MaxBP;				// �ִ�AG�Ŀ�
	int		AddBP;				// �߰� AG�Ŀ�
#endif

	float   VitalityToLife;
	float   EnergyToMana;
	
	char	m_PK_Count;
	char	m_PK_Level;
	int     m_PK_Time;

	short	X;
	short	Y;

	BYTE	Dir;
	BYTE	MapNumber;

	int		AddLife;			// �߰� ����
	int		AddMana;			// �߰� ������
	
#ifdef ADD_SHIELD_POINT_01_20060403
	int		iShield;
	int		iMaxShield;
	int		iAddShield;

	int		iFillShieldMax;
	int		iFillShield;
	int		iFillShieldCount;

	DWORD	dwShieldAutoRefillTimer;
#endif

	//------------------------------------------------------------------------
	// ������Ʈ �ɼ�
	BYTE	DamageMinus;
	BYTE	DamageReflect;

	short 	MonsterDieGetMoney;
	BYTE	MonsterDieGetLife;
	BYTE	MonsterDieGetMana;
	

	//------------------------------------------------------------------------
	// �̵� ����
	BYTE	StartX, StartY;	// ó�� ���� ��ǥX,Y

	short	m_OldX;	// ���� X ��ǥ
	short	m_OldY;	// ���� Y ��ǥ
	
	short	TX;	// ��� X
	short	TY;	// ��� Y
	short	MTX;	// �̵��Ϸ��� ���� ������ X
	short	MTY;	// �̵��Ϸ��� ���� ������ Y

	int		PathCount;
	int		PathCur;
	char	PathStartEnd;			// 0�̸� �̵� �Ϸ�, 1�̸� �̵� ����
	short	PathOri[MAX_ROADPATH];// �н�����
	short	PathX[MAX_ROADPATH];
	short	PathY[MAX_ROADPATH];
	char	PathDir[MAX_ROADPATH];
	DWORD	PathTime;

	char				m_MoveGateNumber;	// �̵��Ϸ��� ����Ʈ ��ȣ
	
	//------------->
	//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
	#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
	short	m_siDieRegenX;			// ��Ȱ ���� X
	short	m_siDieRegenY;			// ��Ȱ ���� Y
	short	m_siDieRegenMapIndex;	// ��Ȱ ���� Map
	
	short	m_siMoveX;				// �̵� ���� X
	short	m_siMoveY;				// �̵� ���� Y
	short	m_siMoveMapIndex;		// �̵� ���� Map
	#endif // ADD_TELEPORT_CHARM_ITEM_20080930
	//<-------------
	
	//------------------------------------------------------------------------
	// ����� ����
	DWORD	Authority;	// ����
// by kuzimo
//	DWORD	AuthorityCode;
	DWORD	Penalty;		// ó��
	BYTE	m_cAccountItemBlock;	// ���� �����ۺ��ΰ�?	

	//------------------------------------------------------------------------
	// ���� �ڵ�
	ACTION_STATUS		m_ActState;			// �׼� ���� 
	BYTE				m_ActionNumber;		// ���� �ൿ ��ȣ 
	
	DWORD				m_State;			// ĳ������ ���� ���� ����
	char				m_StateSub;			// ĳ������ ���� ���� ���� ����

	BYTE				m_Rest;				// ���� �޽� ���̶��..

	char				m_ViewState;		// ĳ������ �������� ���� ����

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	BYTE				m_BuffEffectCount;
	EFFECTLIST			m_BuffEffectList[MAX_BUFF_SLOT_INDEX];
#else
	#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	INT					m_ViewSkillState;
	#else
		#ifdef CHANGE_VIEWSTATE	
		WORD				m_ViewSkillState;
		#else
		BYTE				m_ViewSkillState;
		#endif
	#endif
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917
	
	DWORD				m_LastMoveTime;		// ���̵��̳� Ÿ����Ż �̿�� ������ �̿��ѽð�

	DWORD				m_LastAttackTime;			// ���ݽ� ������ �ð�.

#ifdef __FRIEND_WORK__
	BYTE				m_FriendServerOnline;	// ģ�� ���� ��/�������� 0�̸� ģ�������� �α׿��� ��û�Ѵ�.
#endif
	//------------------------------------------------------------------------
	// �� üũ
	int		m_DetectSpeedHackTime;	// ���ǵ��� üũ �ð�
	DWORD	m_SumLastAttackTime;	// ���ݼӵ��� �̻��� �ð��� ��
	DWORD	m_DetectCount;			// ���ݼӵ��� �̻��ϰ� ���� ��	
	int		m_DetectedHackKickCount;			// ���ݼӵ��� �̻��ϰ� ���� ��	
	int		m_SpeedHackPenalty;		// ���ݼӵ��� �̻��ϰ� ���� ��

	BYTE	m_AttackSpeedHackDetectedCount;
	
	DWORD	m_PacketCheckTime;		// ��Ŷ�� ���� �ð�
	BYTE	m_ShopTime;				// �������� ������ ����ִ� �ִ� �ð�

	DWORD	m_TotalAttackTime;		// �� ���� �ð�
	int		m_TotalAttackCount;		// �� ���� ī��Ʈ

	DWORD	TeleportTime;
	char	Teleport;
	
	char	KillerType;				// ���� ���ΰ���?? ���� ����.. �������.. ���������..

	//------------------------------------------------------------------------
	// ����
	char    DieRegen;				// �׾��ٰ� �ٽ� ��Ƴ���.

	char	RegenOk;
	BYTE	RegenMapNumber;
	BYTE	RegenMapX;
	BYTE	RegenMapY;	
	DWORD	RegenTime;
	DWORD   MaxRegenTime;			// 

	//------------------------------------------------------------------------	
	short	m_PosNum;				// ��ġ���̺� ��ȣ

	
	DWORD	LifeRefillTimer;		// ����� �ٽ� ä���� �ð�..
	DWORD	CurActionTime;			// ���� �ൿ �ð�
	DWORD	NextActionTime;			// ���� �׼� �ð�.
	DWORD	DelayActionTime;		// �׼����� �ð� 
	char	DelayLevel;				// ���� ���� 

#ifdef ADD_SKILL_WITH_COMBO
	char 	m_PoisonType;			// �ߵ��϶� �ߵ� Ÿ��(��ų��ȣ)
	char 	m_IceType;				// ������� �ߵ� Ÿ��(��ų��ȣ)
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
	char	m_ImmuneToMagicCount;		// �������� ���� �ð�
	char	m_ImmuneToHarmCount;		// �������� ���� �ð�
	char	m_iMonsterBattleDelay;		// ���Ͱ� ���� �� ��� �ӹ��Ÿ� �� �����̵Ǵ� �ð� (���ݵ����� ������ ����)
	char	m_cKalimaGateExist;			// ���� ������ ������ �����ϴ��� Ȯ���Ѵ�. (������)
	int		m_iKalimaGateIndex;			// ���� ������ ������ �ε���
	char	m_cKalimaGateEnterCount;
#endif

	OBJECTSTRUCT *lpAttackObj;		// ���� ������ ���� ������Ʈ

	short	m_SkillNumber;			// ��ų��ȣ	(-1 �̸� ��ų�� �������� �ʴ°�!)
	DWORD	m_SkillTime;			// ��ų����� �ð�

#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
	bool	m_bAttackerKilled;		// ���� �׾��� �� ������ �ϴ� �׾����� ����
#endif

	char    m_ManaFillCount;		// ������ ������ ä�� �ð�
	char    m_LifeFillCount;		// ������ ������ ä�� �ð�
	

#ifdef MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221	// ������� �ִ� �ο� ����
#define MAXSELFDEFENSE	7		// �ִ� 7�� ���� ��������� �ǽ��Ҽ� �ִ�. 
#else	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221
#define MAXSELFDEFENSE	5		// �ִ� 5�� ���� ��������� �ǽ��Ҽ� �ִ�. 
#endif	// MODIFY_SELFDEFENSETIME_MAXSELFDEFIENSE_20071221

	//------------------------------------------------------------------------	
	//�������
	int		SelfDefense[MAXSELFDEFENSE];		// ������� ���
	DWORD   SelfDefenseTime[MAXSELFDEFENSE];	// ������� �ð�
	DWORD   MySelfDefenseTime;					// ���� ������ ������� �ð�

	

	//-----------------------------------------------------------------------
	// ��, ����/���� ���� �ɷ�ġ ����
	char	m_Drink;			// ���� �������� �ɷ�

	int		m_SkillAttack2;		// ���ݷ� ���� ��ų2 (����� ����)
	char	m_SkillAttackTime2;	// 	

	//-----------------------------------------------------------------------
	// PARTY
	int     PartyNumber;		// ����ڰ� �����ִ� ��Ƽ ��ȣ : Default : -1
	int		PartyTargetUser;	// ��Ƽ�� �ΰ��� �ϴ� ������ ��ȣ (Ȯ�ο�)  : Default : -1

	//-----------------------------------------------------------------------
	// GUILD
	int		GuildNumber;		// ����ڰ� �����ִ� ��� ��ȣ
	LPGUILD_INFO lpGuild;		// ����ڰ� �����ִ� ����� ������
	char	GuildName[MAX_IDSTRING+1];

#ifdef ADD_NEW_GUILD_01_20040913
	INT		GuildStatus;		// ��� ��å �� ���� 
#endif

#ifdef ADD_NEW_UNION_01_20041006
	INT		iGuildUnionTimeStamp;	// Union �� Rival ���� ������ ���� TimeStamp.
#endif
	
	//-----------------------------------------------------------------------
	// ��ȯ
	int		m_RecallMon;		// ��ȯ��
	
	int		m_Change;			// ����

	//-----------------------------------------------------------------------		
	short	TargetNumber;		// � ��� ����� �Ǵ� ��ȣ( ������ȣ)
	short	TargetShopNumber;
	
	short	ShopNumber;			// NPC�� ���� ��ȣ�� ���δ�.
	
#ifdef FOR_BLOODCASTLE
	short	LastAttackerID;		// ���� ������ ������ ����� �ε��� (������ ��츸)
#endif


	//-----------------------------------------------------------------------		
	// ���ݷ�/����/��ø���� ��������
	int		m_AttackDamageMin;	// �ּ� ���ݷ�(������ ��� ���ʿ� ����
	int		m_AttackDamageMax;	// �ִ� ���ݷ�(������� ���, �ǽð����� ����)

	int		m_MagicDamageMin;	// �ּ� ���� ���ݷ�(�ǽð�)
	int		m_MagicDamageMax;	// �ִ� ���� ���ݷ�

	int		m_AttackDamageLeft;		// �޼� ���ݷ�(�ǽð����� ����)
	int		m_AttackDamageRight;	// ������ ���ݷ� 	

	int		m_AttackDamageMaxLeft;	// �ּ� �޼� ���ݷ�(��� ����� ���)
	int		m_AttackDamageMinLeft;	// �ִ� �޼� ���ݷ�

	int		m_AttackDamageMaxRight;	// �ּ� ������ ���ݷ�(��� ����� ���)
	int		m_AttackDamageMinRight;	// �ִ� ������ ���ݷ�

	int		m_AttackRating;			// ���ݷ�(?)

	int		m_AttackSpeed;			// ���ݼӵ�
	int		m_MagicSpeed;			// �����ӵ�
	
	int		m_Defense;				// ����
	int		m_MagicDefense;			// ��������
	
	int		m_SuccessfulBlocking;	// ������

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���ַ� �߰�
	int		m_Curse;				// ���ַ�
	int		m_CurseDamgeMin;		// �ּ� ���ַ�
	int		m_CurseDamgeMax;		// �ִ� ���ַ�
#endif

	short	m_MoveSpeed;		// �̵� �ӵ�
	short   m_MoveRange;		// �̵� ����
	short   m_AttackRange;		// ���� ����
	short   m_AttackType;		// ���� ����

	short   m_ViewRange;
	short   m_Attribute;		// �Ӽ�

	short	m_ItemRate;			// ������ ��� Ȯ��(����)
	short   m_MoneyRate;		// �� ��� Ȯ��(����)

	int		m_CriticalDamage;	// ũ��Ƽ�� ������ Ȯ��(�������� ����� ����)
	int		m_ExcelentDamage;

	//-----------------------------------------------------------------------		
	// ����/��ų ����
	CMagicInf*	m_lpMagicBack;
	CMagicInf*	Magic;

	char		MagicCount;
	BYTE		UseMagicNumber;	// ���� ������ ���� ��ȣ
	DWORD		UseMagicTime;	// ������ ������ ������ �ð�
	char		UseMagicCount;	// �ѹ� ������ Ÿ�� ����
	
	short       OSAttackSerial;
	BYTE		SASCount;
	DWORD		SkillAttackTime;

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// MASTERLEVEL_PASSIVE_SKILL_OPTION m_MPSkillOpt
	MASTERLEVEL_PASSIVE_SKILL_OPTION	m_MPSkillOpt;
#endif
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ҿ�ٸ��� �Ҹ� ���� ��ġ ���� ����
	int			m_nSoulBarrierDefence;
	short		m_nSoulBarrierManaRate;
#endif

	//------------------------------------------------------------------------
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE		CharSet[MAX_PREVIEWCHARSET+9];		// �ڽ��� ��� ������ �̸� ����� �ξ ����Ʈ ���� �ٸ� ����ڿ��� ������.
#else
#ifdef DARKLORD_WORK
	BYTE		CharSet[MAX_PREVIEWCHARSET+4];		// �ڽ��� ��� ������ �̸� ����� �ξ ����Ʈ ���� �ٸ� ����ڿ��� ������.
#else
		BYTE	CharSet[MAX_PREVIEWCHARSET+3];		// �ڽ��� ��� ������ �̸� ����� �ξ ����Ʈ ���� �ٸ� ����ڿ��� ������.
#endif
#endif

	BYTE	m_Resistance[MAX_RESISTANCE];		// ������ ���׷� 
	
	BYTE	m_AddResistance[MAX_RESISTANCE];		

	//------------------------------------------------------------------------
	// ViewPort
	int FrustrumX[4];
	int FrustrumY[4];
	
	VIEWPORT_STRUCT			VpPlayer[MAXVIEWPORTOBJECT];
	VIEWPORT_PLAYER_STRUCT	VpPlayer2[MAXVIEWPORTOBJECT];	// ���� ���� �ִ� ������?
	int  VPCount;		// ����Ʈ�� ���� �ִ� ����� ����
	int  VPCount2;


	//------------------------------------------------------------------------
	// ����ġ �й� �� ������ ���� �켱���� ���� �ڽ��� ������ Obj�� ���� ����
	HITDAMAGE_STRUCT		sHD[MAX_HITDAMAGE];
	short	sHDCount;
	
	//------------------------------------------------------------------------
	// �������̽� ���� (�κ�, â��, ī�����ڽ� ��)
	INTERFACE_STATUS	m_IfState;	
	DWORD				m_InterfaceTime;		// �������̽� ��û �ð�

	// �κ��丮
	CItem * pInventory;				// ������ ���
	BYTE  * pInventoryMap;
	char  * pInventoryCount;
	char    pTransaction;			// �κ��丮 ó�� ���� 1=���, 0=������

	CItem*  Inventory1;				// ������ ���
	BYTE*	InventoryMap1;			// �κ��丮 ���� üũ�� ����
	char    InventoryCount1;

	CItem*  Inventory2;				// ������ ���
	BYTE*	InventoryMap2;			// �κ��丮 ���� üũ�� ����
	char    InventoryCount2;

	// �ŷ�â
	CItem*	Trade;					// ��ȯâ ������ ���
	BYTE *	TradeMap;				// ��ȯâ üũ�� ����
	int 	TradeMoney;				// ��ȯ�� ���� �׼�
	BYTE	TradeOk;				// ��ȯ OK ��ư ����

	// â��
	CItem*  pWarehouse;				// ������� â��
	BYTE *  pWarehouseMap;			// 
	char    WarehouseCount;			// ����: â�� �ִ� �������� ����
									// �����: â���� ������ ����� �ߴ����� ����... (��,��)
	short	WarehousePW;			// â�� ��й�ȣ 0 �̸� ��� ����, �� �̻��̸� ��ݻ���
	BYTE	WarehouseLock;			// â���� ��� ���� 0�̸� ����Ҽ� �ְ� 1�̸� ����Ҽ� ����.
	BYTE    WarehouseUnfailLock;	// â�� ����� ���� �Ҷ� ��й�ȣ ������ ī��Ʈ
	int		WarehouseMoney;			// â�� �ִ� �ݾ�
	BOOL	WarehouseSave;			// â�� ��� �߾�����..
	
	// ī����â
	CItem*  pChaosBox;				// ī�����ڽ�
	BYTE *  pChaosBoxMap;			// 
	int		ChaosMoney;				// �ͽ��� �ʿ��� �ݾ�
	int		ChaosSuccessRate;		// ���� Ȯ�� 
	int		ChaosLock;				// ChaosLock �� 1�̸� ��� �̵��� �Ҽ�����.
	
	//------------------------------------------------------------------------
	DWORD	m_Option;				// ���� �ɼ�

	//------------------------------------------------------------------------
	// �̺�Ʈ�� 
	// �Ǹ��� ����
	int		m_nEventScore;			// �̺�Ʈ�� ����
	int		m_nEventExp;			// �̺�Ʈ�� ����ġ
	int		m_nEventMoney;			// �̺�Ʈ�� ��	
	BYTE	m_bDevilSquareIndex;	// ���� �ε���
	BYTE	m_bDevilSquareAuth;		// �Ǹ��� ���� ����(?)	

#ifdef FOR_BLOODCASTLE
	// ����ĳ��
	char	m_cBloodCastleIndex;	// ���� ĳ���� �ٸ���ȣ (0 ~ 6)
	char	m_cBloodCastleSubIndex;	// ���� ĳ���� �ٸ��� ���° �濡 ���� �ִ°� (����� ��츸)
	int		m_iBloodCastleEXP;		// ���� ĳ������ ���� ����ġ (����� ��츸)
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	bool	m_bBloodCastleComplete;	// �ش� ����ڰ� ����ĳ���� �Ϸ� (����� �̰ų� ����ڿ� ���� ������ �� ��õ��� ��ȭ) �ߴ��� Ȯ��
#endif
	
#ifdef CHAOSCASTLE_SYSTEM_20040408
	// ī���� ĳ��
	char	m_cChaosCastleIndex;	// ī���� ĳ���� �ٸ���ȣ (0 ~ 7)
	char	m_cChaosCastleSubIndex;	// ī���� ĳ���� ���° �濡 ���� �ִ°� (����� ��츸)
	int		m_iChaosCastleBlowTime;	// ī���� ĳ������ ���������� ���߷� �и� �ð� (Ŭ���̾�Ʈ�ʿ��� ����ġ��� �̴°� ����)
	char	m_cKillUserCount;		// ī���� ĳ������ PK�� �� Ƚ��
	char	m_cKillMonsterCount;	// ī���� ĳ������ ���͸� ���� Ƚ��
#endif

#ifdef DUEL_SYSTEM_20031028
	//------------------------------------------------------------------------
	// ����
	int		m_iDuelUserReserved;	// ���� ������ ���� (���� ��û)
	int		m_iDuelUserRequested;	// ������ ������ ��û�� ����
	int		m_iDuelUser;			// ���� �����ϰ� �ִ� ����
	BYTE	m_btDuelScore;			// ���� ���� �� ����

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	DWORD	m_iDuelTimer;			// ���� �ð�	
#endif	
#endif
#ifdef DUEL_SYSTEM_FIX_20031224
	int		m_iDuelTickCount;		// ���� �� �������� ���� �ð��� üũ	
#endif


#ifdef PERSONAL_SHOP_20040113	
	//------------------------------------------------------------------------
	// ���λ���
	bool	m_bPShopOpen;							// ���λ����� �����ߴ����� ����
	bool	m_bPShopTransaction;					// ���λ����� ��ǰ ��ȯ ���� Ʈ����� (true�� ������ �ŷ� �Ұ�)
	bool	m_bPShopItemChange;						// ���λ����� ��ǰ�� �ȷ��� ��� �������� ����� ��� (�Ĵ� ��� ���忡��)
	bool	m_bPShopRedrawAbs;						// ���λ����� ����Ʈ�� ������ �ٽ� �׷���
	CHAR	m_szPShopText[MAX_PSHOP_TEXT];			// ���λ����� �̸�

	bool	m_bPShopWantDeal;						// �ٸ� ����� ���λ��� â�� ���� �ִ°� ����
	int		m_iPShopDealerIndex;					// ���� ���� �ִ� ���� ������ �ε���
	CHAR	m_szPShopDealerName[MAX_IDSTRING];		// ���� ���� �ִ� ���� ������ �ɸ���

	CRITICAL_SECTION	m_critPShopTrade;			// ���λ����� �����̵��� ����� ���� ����ȭ�� ���� ���Ǵ� CRITICAL_SECTION

	INT		m_iVpPShopPlayer[MAXVIEWPORTOBJECT];	// ���λ����� ����Ʈ
	WORD	m_wVpPShopPlayerCount;					// ���λ����� ����Ʈ�ȿ� ���� ������ �ִ� �������� ��
#endif
	

	//------------------------------------------------------------------------
	// ��Ÿ
	BYTE	IsInBattleGround;		// �����౸ ���ΰ�?
	BYTE	HaveWeaponInHand;		// �տ� ���⸦ ��� �ִ°�?

	// �����̺�Ʈ(20030509)
	short	EventChipCount;				// �̺�Ʈ Ĩ�� ��
	int		MutoNumber;					// �����ȣ
	int		UseEventServer;

	BYTE	LoadWareHouseInfo;		// â�������� �о��°�?

#ifdef FOR_STONE_EXCHANGE
	INT		iStoneCount;			// ������ ����
#endif

	// ����Ʈ
#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ �Ϸ����� ���� ų�� ó�� �߰�(DB����)	
	BOOL	m_bQuestMonKillDBLoad;		// DB�� �ش� ĳ������ ������ ���� ���� �ҷ��´�.
	int		m_iQuestMonKillDBIndex;		// DB�� ����� ���� ����Ʈ �ε��� - ����ų���� �Ϸ����� ���	
	QUEST_INFO_MOSNTER_KILLCOUNT m_QuestMonKillInfo[MAX_QUEST_MONSTER_KILL_INFO];
#endif
	
#ifdef NEW_SKILL_FORSKYLAND
	BYTE	m_Quest[MAX_DBQUEST];
	BYTE	m_SendQuestInfo;

	int		m_SkyBossMonSheildLinkIndex;	// ������ �� �ε���
	int		m_SkyBossMonSheild;				// ���� ����ϴ� �ð�
	int		m_SkyBossMonSheildTime;			// ���� �������� �ð�
#endif

#ifdef NEW_FORSKYLAND2
	int		m_MaxLifePower;					// �ִ� �����

	int		m_PacketChecksumTime;
	int		m_CheckLifeTime;
#endif


#ifdef GAMESERVER_DIVISION
	BYTE	m_MoveOtherServer;				// ������ ������Ű�� �ִ°�?	
#endif

#ifdef LOGINSERVER_MULOG
	char	BackName[MAX_IDSTRING+1];		// ĳ���� �̸� ���
#endif

	char	m_BossGoldDerconMapNumber;
	BYTE	m_InWebzen;

	char	m_LastTeleportTime;				// �̵��� ����, ���� �ð� ��������� ���� �ڷ� �ȹи��� �����ϱ� ���� ��

	BYTE	m_ClientHackLogCount;

#ifdef FOR_THAILAND
	BYTE	m_AuthAdult;
	BYTE	m_DisplayTeenLogOutMsg;
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	int		m_iVerifyType;					// �߱� �Ƿε� ���� ���� Ÿ�� (0 : ����, 1 : �̼�����, 2 : ����)
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	INT		m_iPlayTimeLimit;				// �߱� �Ƿε� - ���ӽð� (5�ð� ������ �������� 0%)
	DWORD	m_dwPlayTick;					// ���ӽð��� ����ϱ� ���� ƽ ī��Ʈ
	INT		m_iPlayNoticeTick;				// ���� �߰��� ���� ���ӽð��� �������ֱ� ���� ƽī��Ʈ
#endif
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	INT		m_iPlayTimeLimit;				// (Sec) ��Ʈ�� �Ƿε�- �������ӽð� (5�ð� ������ �������� 0%)
	INT		m_iPlayRestTime;				// (Sec) ��Ʈ�� �Ƿε�- �����޽Ľð�
	DWORD	m_dwPlayTick;					// (ms)	 ���ӽð��� ����ϱ� ���� ƽ ī��Ʈ
	INT		m_iPlayNoticeTick;				// (ms)  ���� �߰��� ���� ���ӽð��� �������ֱ� ���� ƽī��Ʈ
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
	BOOL	m_bIsInMonsterHerd;				// Ư�� ���� ������ ���ϴ��� ?
	BOOL	m_bIsMonsterAttackFirst;		// ���� ������ �ƴ��� ?
	MonsterHerd * m_lpMonsterHerd;			// ���� ���� ���� ���� Ŭ������ ������, ������ GUID�ε� Ȱ�� (�ݵ�� �ٸ� ������ �޾Ƴ��� ����.)
#endif

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_10_20050411
	CCSAuth	NPggCSAuth;	
	DWORD	m_NPggCheckSum;
#endif
	
#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_20_20050411
	CCSAuth2 NPggCSAuth;
	bool m_bSentGGAuth;
#endif 

#ifdef NPROTECT_GAME_GAURD_FOR_SERVER_VERSION_25_20060123
	CCSAuth25 NPggCSAuth;
	bool m_bSentGGAuth;
	bool m_bSentGGAuthAgain;
	#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
		bool m_bCheckGGAuth;
#ifdef MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723	// �ʼ��� �̵��� ������ �ð�
		DWORD m_dwNPGGMapSvrMoveTime;
#endif	// MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
	#endif
#endif

	DWORD	m_NPggCheckSumSendTime;
	
	int		fSkillFrustrumX[4];
	int		fSkillFrustrumY[4];

#ifdef ADD_SKILL_WITH_COMBO
	BYTE	SkillHellFire2State;
	BYTE	SkillHellFire2Count;
	DWORD	SkillHellFire2Time;
#endif
	
#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	BYTE	m_ReqWarehouseOpen;	
#endif

//#ifdef CHECK_ATTACK_AREA                  // define ������ �����ؾ� ������ �ٷα� ó���� ������ ���� (u4us)
	int		m_NotAttackAreaCount;			// ���� �ȵǴ� �������� ���� �õ��� Ƚ��
//#endif

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	int     m_NotAttackAreaCountFor10Min;   // �ֱ� 10 �е��� �����Ǵ� ���� ���� ���� Ƚ��
#endif

//#if defined ( MODIFY_HACKLOG_OF_ATTACK_20050518 ) || defined ( MU_CHN_HACKLOG3_MODIFY_20041117 )
//	DWORD	dwPlayTime;
//#endif

	short	SetOpAddSkillAttack;				// ��ų ���ݷ� ���� v

	short	SetOpAddExDamage;					// ������Ʈ ������ ���� v
	short	SetOpAddExDamageSuccessRate;		// ������Ʈ ������ ����	v
	short	SetOpAddCriticalDamage;				// ũ��Ƽ�� ������ ���� v
	short	SetOpAddCriticalDamageSuccessRate;	// ũ��Ƽ�� ������ Ȯ�� ���� v

	short	SetOpIncAGValue;					// AG������ ���� v
	short	SetOpAddDamage;						// ���������� v
	short	SetOpAddMinAttackDamage;			// �ּ� ���ݷ� ���� v
	short	SetOpAddMaxAttackDamage;			// �ִ� ���ݷ� ���� v

	short	SetOpAddAttackDamage;				// �߰� ���ݷ� v
	short	SetOpAddDefence;					// �߰� ���� v
	short	SetOpAddDefenceRate;				// �߰� ����(%) v
	short	SetOpAddMagicPower;					// �߰� ���� v

	BYTE	SetOpIgnoreDefense;					// ����� ���� Ȯ�� v
	BYTE	SetOpDoubleDamage;					// ���� ������ v
	BYTE	SetOpTwoHandSwordImproveDamage;		// ��հ� ���� ������ 15% ���� v
	BYTE	SetOpImproveSuccessAttackRate;		// ���� ������ ���� v
	BYTE	SetOpReflectionDamage;				// 100% ������ �ݻ� Ȯ�� 5% ���� v
	BYTE	SetOpImproveSheldDefence;			// ���� ������ ���� 5%���� v
	BYTE	SetOpDecreaseAG;					// �Һ� AG 10% ���� v
	BYTE	SetOpImproveItemDropRate;			// ������ ��� Ȯ�� 5% ����	 v

	BYTE	IsFullSetItem;



#ifdef TEST_SETITEM_CLEAR
	BYTE	ClearInfoForSetItem;
#endif

#ifdef APPLY_DURATION_MAGIC_HACK_CLEANER_20040504
	TDurMagicKeyChecker DurMagicKeyChecker;
#endif

#ifdef DARKLORD_WORK	
	WORD	SkillRecallParty_Time;
	BYTE	SkillRecallParty_MapNumber;
	BYTE	SkillRecallParty_X;
	BYTE	SkillRecallParty_Y;
#endif
	
#ifdef CHAOS_MIX_TRANSACTION_BUXFIX_20040610
	bool	bIsChaosMixCompleted;			// ī���� ���� �߿� ������ �Ϸ�Ǿ����� (1) / �̿Ϸ� ���� (0) �� ���°�
#endif
	
#ifdef DARKLORD_WORK
	BYTE	SkillLongSpearChange;
	CSkillDelay	SkillDelay;
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
	INT		iObjectSecTimer;
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
	BYTE	bWaitBlockState;
#endif
	
#ifdef MAP_SERVER_WORK_20041030				// �ʼ��� �̵��� �ʿ��� ����� ������
	bool	m_bMapSvrMoveQuit;				// �ʼ����� �̵��ϱ� ���� ���� �����̾��°� ? 
	bool	m_bMapSvrMoveReq;				// �ʼ��� �̵� ��û�� �߾��°� ?
	DWORD	m_dwMapSvrQuitTick;				// �ʼ��� �̵� ���� ƽ ī��Ʈ
	SHORT	m_sPrevMapSvrCode;				// ���� �ʼ��� �ڵ�
	SHORT	m_sDestMapNumber;				// �̵��� �ʹ�ȣ	(�̵��� �����ʿ��� ���� �� ����)
	BYTE	m_btDestX;						// �̵��� X��ǥ		(�̵��� �����ʿ��� ���� �� ����)
	BYTE	m_btDestY;						// �̵��� Y��ǥ		(�̵��� �����ʿ��� ���� �� ����)
#endif
#ifdef	CASTLE_MAIN_SCHEDULER_20041111
	// ������ NPC�鿡 �ʿ��� ������
	union {
		struct {
			BYTE	m_btCsNpcExistVal1;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 1 (1 ~ 255)
			BYTE	m_btCsNpcExistVal2;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 2 (0 ~ 255)
			BYTE	m_btCsNpcExistVal3;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 3 (0 ~ 255)
			BYTE	m_btCsNpcExistVal4;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� - 4 (0 ~ 255)
		};
		INT			m_iCsNpcExistVal;		// NPC�� ��ȿ���� Ȯ���ϱ� ���� �� (0�̸� ��ȿ)
	};
	BYTE	m_btCsNpcType;
	BYTE	m_btCsGateOpen;					// ������ ��� -> ���� ����Ʈ�� ������������ Ȯ�� (�ʱⰪ 1:����)
	INT		m_iCsGateLeverLinkIndex;		// ����,����-�������ε���
	// NPC ���׷��̵�
	BYTE	m_btCsNpcDfLevel;				// ����,����-����
	BYTE	m_btCsNpcRgLevel;				// ����-ȸ����
	// ������ �� �� ������� ���� �Ҽ� ���θ� Ȯ���ϱ� ���� ��
	BYTE	m_btCsJoinSide;					// ������ ��� �ҼӰ� (0:���þƴ�/1:������/2�̻�:������) / ���������� - ��ȯ���� ��� �ҼӰ�
	bool	m_bCsGuildInvolved;				// ������ ���� ���õ� ����� ������� Ȯ�� (0:����ھƴ�/1:�����)
#endif
	
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619
	bool	m_bIsCastleNPCUpgradeCompleted;	// ���� ���� NPC�� ���׷��̵� �ߴ°�? ( 0:���ߴ� 1:�ߴ� )
											// Ŭ���̾�Ʈ ��������� �ش� ������ ���� �� ������ ������ rollback �Ǵ°��� ���´�.
#endif	

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116
	// ������ ���� ����
	BYTE	m_btWeaponState;
	INT		m_iWeaponUser;
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// ��ų�� ����ϱ� ���� �ʿ��� ���� ī����
	BYTE	m_btKillCount;
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	INT		m_iAccumulatedDamage;			// ����, ���� ���ݽ� ������ ����
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
	BYTE	m_btLifeStoneCount;				// ����� ���������� ����
	BYTE	m_btCreationState;				// ���� ����.
	INT		m_iCreatedActivationTime;		// ���� �� �ð�.
	
#endif
	
#ifdef MODIFY_CASTLESIEGE_CALC_CROWN_ACCESSTIME_20051014
	INT		m_iAccumulatedCrownAccessTime;	// ���� ��� ���� �ð�

#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	// ���� ��ų������Ʈ�� ȿ�� ���� �� ����
	TMonsterSkillElementInfo	m_MonsterSkillElementInfo;
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// ���� AI
	int		m_iBasicAI;
	int		m_iCurrentAI;
	
	// ���� AI ���� -> ����ü�� �ٲܰ��ΰ�?
	int		m_iCurrentAIState;

	// ���� AI ���� �ð�
	int		m_iLastAIRunTime;
	
	// ���� Group ��ȣ
	int		m_iGroupNumber;
	int		m_iSubGroupNumber;

	int		m_iGroupMemberGuid;
	int		m_iRegenType;

//	int		m_iLastAIPathSpot;

	TMonsterAIAgro	m_Agro;
	#ifdef MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
		int		m_iLastAutomataRuntime;	// ���� Automata ���� �ð�
		int		m_iLastAutomataDelay;	// ���� ���� Automata�� Delay ��
	#endif // MODIFY_MONSTERAI_AUTOMATA_DELAY_BUGFIX_20060907
#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725


#ifdef CRYWOLF_MVP_REWARD_RANK_20051215
	int		m_iCrywolfMVPScore;
#endif

#ifdef PCBANG_ADVANTAGE
	BOOL	m_bPCBangUser;				// happycat@20050201 (PC�� ����)		
#endif

#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	BOOL	m_bPCBangCouponUser;
#endif
	
#ifdef PCBANG_EVENT_FREE_CHAOSCASTLE_20060307
	BOOL	m_bPCBangFreeChaosCastleUser;	// PC�� ī����ĳ�� ���� ���� �̺�Ʈ(RKFKA)
#endif

#ifdef HAPPY_POUCH // happycat@20050201 (���ָӴ�) - ���� �� �̺�Ʈ	
	LONG	m_lFreeKalimaTime;			//Second
	LONG	m_lLifeIncTime;
	BYTE	m_btLifeInc;
#endif
//#ifdef HAPPY_CAT
	DWORD	m_dwLastCheckTick;			// ���ʹ� ������ �뵵�� ��� (�η������̺�Ʈ - b4nfter)
//#endif
	
#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
	int m_iAutoRecuperationTime;
#endif	

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	#ifdef NEW_SKILL_FORSKYLAND
	int		m_SkillAddLife;			// ����� ���� ��ų
	int		m_SkillAddLifeTime;		// ����� ���� ��ų���� �ð�

	int		m_SkillHarden;			// ��ȭ ��ų
	int		m_SkillHardenTime;		// ��ȭ ��ų ���� �ð�	

	int		m_SkillMagumReduceDefense;		// ���� ���� ����
	int		m_SkillMagumReduceDefenseTime;	// ���� ���� ���� ���ӽð�
	#endif

	#ifdef NEW_FORSKYLAND2
	int		m_WizardSkillDefense;			// ���� ���� ����
	int		m_WizardSkillDefenseTime;
	#endif // NEW_FORSKYLAND2

	#ifdef DARKLORD_WORK
	WORD	SkillAddCriticalDamageTime;
	WORD	SkillAddCriticalDamage;
	#endif // DARKLORD_WORK

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// STUN
	INT		m_iSkillStunTime;				// ���� ���� �ð�
	
	// BRAND OF SKILL
	INT		m_iSkillBrandOfSkillTime;		// �귣����꽺ų ���ӽð�

	// Invisible
	INT		m_iSkillInvisibleTime;			// �κ����� ���ӽð�

	// Mana Swell
	INT		m_iSkillManaSwellTime;			// �������� ���ӽð�
	INT		m_iSkillManaSwell;				// ����������
	#endif

	#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
	// Bless Potion
	INT		m_iPotionBlessTime;				// �ູ�� ���� ���� �ð�

	// Spirit Potion
	INT		m_iPotionSoulTime;				// ��ȥ�� ���� ���� �ð� 
	#endif // ADD_NEW_ITEM_FOR_CASTLE_01_20041116

	char 	m_PoisonBeattackCount;	// 0 �̸� �ߵ����°� �ƴϰ� �� �̻��̸� �ߵ� 
	char    m_ColdBeattackCount;	// 0�̸�  �������� ���� ���°� �ƴϰ� �� �̻��̸� ���� ������ ���� �����̴�.

	int 	m_SkillDefense;		// ���� ���� ��ų(��������)
	DWORD	m_SkillDefenseTime;	

	int		m_SkillAttack;		// ���ݷ� ���� ��ų(��������)
	DWORD	m_SkillAttackTime;	// 

	#ifdef ADD_NPC_SHADOW_PHANTOM_20050616
	int m_iSkillNPCHelpTime;
	int m_iSkillNPCDefense;
	int m_iSkillNPCAttack;
	#endif

	#ifdef ADD_PCS_MARK_OF_EXP_20070205
	WORD	m_wExprienceRate;
	WORD	m_wItemDropRate;
	BYTE	m_btMoveMapBound;
	#endif // ADD_PCS_MARK_OF_EXP_20070205

	#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// ����� ���Ǵ�Ƽ �ַο� ��ų ���ӽð�
	int m_iMuseElfInfinityArrowSkillTime;
	#endif

	#ifdef PCBANG_POINT_SYSTEM_20070206
	// PC�� ����Ʈ ������ ��� ����
	int		m_iPCBangPointItemTime;				// ������ ��� �����ð�(Sec)
					
	// PC�� ����Ʈ �������� �������� �� ��Ÿ���� ȿ��
	int		m_iPCBangPointItemAddExpPer;			// �߰� ȹ�� ����ġ Ȯ��
	BYTE	m_bPCBangPointNoMoreExp;				// TRUE : ����ġ�� �������� �ʴ´� / FALSE
	int		m_iPCBangPointItemGiveItemPer;		// ������ ȹ�� �߰� Ȯ��
	#endif // PCBANG_POINT_SYSTEM_20070206
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	WORD m_wExcellentDropRate;
#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
	WORD m_wAncientDropRate;
#endif
	
#ifdef ADD_ANTI_HACK_01_20051027
	// PACKET_ATTACKET ��Ŷ�� ���� �ð� üũ
	DWORD	m_dwLastPacketAttackTime;
#endif
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417
	int m_iSkillDistanceErrorCount;	// ��ų �Ÿ� üũ �̻��� ��� 
#endif	//MODIFY_SKILL_CHECK_DISTANCE_UPDATE_20060417

#ifdef MODIFY_SKILL_CHECK_DISTANCE_UPDATE_02_20060421
	DWORD m_dwSkillDistanceErrorTick;	// ��ų �Ÿ� �� ����� üũ ���� Ÿ��ƽ
#endif
	
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ������ ���� ��ȭ ������ �ɼ� ����
	JEWELOFHARMONY_ITEM_EFFECT	m_JewelOfHarmonyEffect;
#endif	// ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;	// 380 �߰� �ɼ� ����
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_NEW_NPC_KANTURU_20060612
	BOOL	m_bKanturuEntranceByNPC;	// ĭ���� ���� NPC�� ���� ĭ���� ������ �ʿ� ������ �����̴�.
#endif
	
#ifdef ITEM_ADD_OPTION_20061019
	WORD	m_wItemEffectType;
	int		m_iItemEffectValidTime;
#endif // ITEM_ADD_OPTION_20061019

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	WORD	m_wCashPoint;
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef PCBANG_POINT_SYSTEM_20070206			// PCBANG_POINT_INFO ����
	BOOL				m_bPCBangPointUser;		// PC�� ����Ʈ �ý��� ��� �����ΰ�? 0:�ƴϴ�/1:�׷���	
	PCBANG_POINT_INFO	m_PCBangPointInfo;		// PC�� ����Ʈ ����
#ifdef PCBANG_KUZIMO
	MWeeklyTimer		m_kWeeklyTimer;
#endif
#endif	// PCBANG_POINT_SYSTEM_20070206

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ����� ���� �ε��� �߰�
	int		m_iIllusionTempleIndex;
#endif

#ifdef MODIFY_0708_BUFFIX_20070820	// ��ų Ű���� �޾Ҵ��� ����
	BOOL	m_bSkillKeyRecv;		
#endif

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	int		m_iPeriodItemEffectIndex;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	
#ifdef ADD_CHARCARD_SUMMONER_20080313
	BYTE	m_bCharCardSummoner;
#endif // ADD_CHARCARD_SUMMONER_20080313

#ifdef MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	int		m_UserKillRecvCount;
#endif // MODIFY_DENY_USER_DISCONNNECT_HACK_20080323

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	SOCKET_OPTION_LIST	m_SeedOptionList[MAX_SEEDSPHERE_OPTION_DATA];
	SOCKET_OPTION_LIST	m_BonusOptionList[MAX_BONUS_OPTION_DATA];
	SOCKET_OPTION_LIST	m_SetOptionList[MAX_SET_OPTION_DATA];
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
	
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	WORD	m_btRefillHPSocketOption;
	WORD	m_btRefillMPSocketOption;
#else
	BYTE	m_btRefillHPSocketOption;
	BYTE	m_btRefillMPSocketOption;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
	
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	WORD	m_wSocketOptionMonsterDieGetHp;
	WORD	m_wSocketOptionMonsterDieGetMana;
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_02_20080804
	
#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	BYTE	m_btAGReduceRate;
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

#ifdef HACKTOOL_KUZIMO
	PacketStatistics	m_kRecvPacketStatistics;
#endif
	
#ifdef GMCOMMAND_EX
	BYTE	m_bGMSummon;
#endif // GMCOMMAND_EX

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	DWORD	m_dwUseHacktoolList[MAX_HACKTOOL_LIST];
#endif // ADD_HACKING_TOOL_BLOCK_20090311
	
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	DWORD	m_dwMapMoveKeyValue;
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	BOOL	m_bRequestPaket;		// ��Ŷ�� ���´��� ����
	int		m_nRequestPacketCnt;	// ���� ��Ŷ�� ��� �Ӵ��� ����
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

	BYTE	m_AccountType;
	INT64	m_EndTime;
	time_t	m_JoinTime;
	DWORD	m_dwRequestVip;
	bool	m_bRequestVip;
	bool	m_bVIPReload;

	bool	m_bResetState;
	INT		m_iResetNumber;
	bool	m_bReqUpdateReset;

	bool	m_bShell;

	PVP_USER_STATE	m_stateEngagePVP;

	bool	m_bBuffHelper;

	//offafk
	bool OffAfk;
	bool Offtrade;
	//DevGamesX 01/05/2018 - OffExp
	int DbOffExp;
	bool OffExp;
	int OffExpTick;
	int OffExpSpeed;

	int OffTradeZen;
	int OffTradeCredits;
	int BotStore;
	int m_ConnectBonusTime;

	//PICKUP
	PICK_UP_ITEMS m_pickup[20];
	int PickUpZen;
	
#ifdef MEM2_KUZIMO
	};

	typedef OBJECTSTRUCT* LPOBJECTSTRUCT;
#else
	} OBJECTSTRUCT, *LPOBJECTSTRUCT;
#endif

// OBJECTSTRUCT, *LPOBJECTSTRUCT;
/*
inline BOOL CheckAuthority(DWORD dwMyAuthority, DWORD dwCmdAuthority)
{
	return(((dwMyAuthority & dwCmdAuthority) == dwMyAuthority)? true : false);
}
*/
inline bool CheckAuthority(DWORD dwComposeAuthority, OBJECTSTRUCT *pMyObject)	// dwComposeAuthority�� ���ռӼ��̴�. pMyObject->Authority�� �������� ����.
{
	return ((pMyObject->Authority & AUTHORITY_DISABLE) ? false : ((pMyObject->Authority & dwComposeAuthority) ? true : false));
	//return(((pMyObject->Authority & dwComposeAuthority) == pMyObject->Authority)? true : false);
}


#ifdef MEM_KUZIMO
extern OBJECTSTRUCT *gObj;
#else
extern OBJECTSTRUCT	gObj[];
#endif

extern short RoadPathTable[];

extern int gServerMaxUser;
extern int gObjTotalUser;

#ifdef ADD_SKILL_WITH_COMBO
extern void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2 = 0);
extern void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#elif defined(NEW_ATTACK_PROC)
extern void gObjAddAttackProcMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2 = 0);
extern void gObjStateAttackProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
#endif

extern void  gObjInit();
extern void  gObjEnd();
extern short gObjMemFree(int index);
extern short gObjSometimeClose(int index);

extern void gObjDestroy(SOCKET aSocket, int client);
extern short gObjAdd(SOCKET aSocket,char *ip, int aIndex);
extern short gObjAddCallMon();
extern short gObjAddSearch(SOCKET aSocket,char *ip);

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
extern short gObjAddMonster(int iMapNumber);
#else
extern short gObjAdd();
#endif

extern short gObjDel(int socket);
extern int gObjGameClose(int aIndex);
extern int gObjGetSocket(SOCKET socket);
extern void gObjBillRequest(LPOBJECTSTRUCT lpObj);
extern void gObjUserKill(int aIndex);
extern void gObjViewportClose(LPOBJECTSTRUCT lpObj);
extern BOOL gObjJoominCheck(int aIndex, char *szInJN);

extern BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int iw, int ih);		// 

#ifdef PERSONAL_SHOP_20040113
extern BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
#endif

//#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
// ADD_PARTIALY_CHARGE_CASH_SHOP_20070117 ���� ���� �������� ��� �����ϵ��� 
// ADD_PARTIALY_CHARGE_CASH_SHOP_20070117 ������
extern BOOL CheckInventoryEmptySpace(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
//#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
extern BOOL CheckInventoryEmptySpace_(LPOBJECTSTRUCT lpObj, int iItemHeight, int iItemWidth);
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

extern char *gObjGetAccountId(int aIndex);
extern int   gObjGetIndex(char *szId);
extern BOOL  gObjUserIdConnectCheck(char *szId, int index);
extern BOOL gObjIsConnectedGP(int aIndex);
extern BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj);
extern BOOL gObjIsConnected(LPOBJECTSTRUCT lpObj, int dbnumber);
extern BOOL gObjIsConnected(int aIndex);
#ifdef ADD_NEWPVP_PKFIELD
extern BOOL gObjIsConnectedEx(int aIndex);
#endif // ADD_NEWPVP_PKFIELD
extern BOOL gObjIsAccontConnect(int aIndex, char *accountid);
extern BOOL gObjIsGamePlaing(LPOBJECTSTRUCT lpObj);

#ifdef ADD_LOG_CHECK_INVLIDE_X_Y_POSITION_20060131
#define DBGNAME_Default					0
#define DBGNAME_PMoveProc				1
#define DBGNAME_RecvPositionSetProc		2
#define DBGNAME_gObjSecondProc			3
extern BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj, int iDbgName = DBGNAME_Default );
#else
extern BOOL gObjCheckXYMapTile(LPOBJECTSTRUCT lpObj);
#endif

extern void gObjTimeCheckSelfDefense(LPOBJECTSTRUCT lpObj);


#ifdef ADD_PROTOCOL_PCBANG_GUID	

	#ifdef PCBANG_ADVANTAGE //happycat@20050201 (PC�� ����)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser);
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber);
	#endif//PCBANG_ADVANTAGE

#else
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, int aPCBangGuid, char *JoominNumber, BYTE	bPCBangUser);
	#else
		BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char *JoominNumber);
	#endif
#endif


extern BOOL  gObjSetCharacter(BYTE *lpdata, int aIndex);
extern BOOL  gObjSetMonster(int aIndex, int MonsterClass);
extern BOOL  gObjSetPosMonster(int aIndex, int PosTableNum);

extern void gObjViewportListCreate(short aIndex);
extern void gObjViewportListDestroy(short aIndex);
extern void gObjViewportListProtocol(short aIndex);
extern void gObjSetState();
extern void gObjSecondProc();


#ifdef PERSONAL_SHOP_20040113	// ���λ��� ���� ���θ� üũ�ϰ� ���λ����� ����Ʈ�� �߰��Ѵ�.
// ���λ����� ����Ʈ ����Ʈ�� �籸�� �Ѵ�. -> �߰��� ���λ����� ���� ��Ŷ�� ������.
extern void		PShop_ViewportListRegenarate(short aIndex);
// �ش� �ε����� ���λ����� ������� Ȯ���Ѵ�. (true - ����� / false - ������ �ִ�.)
extern bool		PShop_CheckInventoryEmpty(short aIndex);
#endif

#ifdef ADD_NEW_UNION_01_20041006
extern BOOL gObjCheckRival( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );
extern INT	gObjGetRelationShip( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );
extern INT	gObjGetGuildUnionNumber( LPOBJECTSTRUCT lpObj );
extern void gObjGetGuildUnionName( LPOBJECTSTRUCT lpObj, CHAR* szUnionName, INT iUnionNameLen = MAX_GUILDNAMESTRING);
extern void gObjNotifyUpdateUnionV1( LPOBJECTSTRUCT lpObj );
extern void gObjNotifyUpdateUnionV2( LPOBJECTSTRUCT lpObj );
extern void gObjUnionUpdateProc( INT aIndex );
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
// Kill Count ����
extern void gObjSetKillCount( INT aIndex, INT iOption = KILLCOUNT_INC);

#endif

#ifdef ADD_NEW_VIEWPORT_SYSTEM_01_20041201
extern void gObjNotifyUseWeaponV1( LPOBJECTSTRUCT lpOwnerObj, LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY  );
extern void gObjNotifyUseWeaponDamage( LPOBJECTSTRUCT lpWeaponObj, INT iTargetX, INT iTargetY );

#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
extern void gObjUseBlessAndSoulPotion( INT aIndex, INT iItemLevel );

#endif


extern void gObjStateSetCreate(int aIndex);

extern void gObjViewportPaint(HWND hWnd, short aIndex);

extern void ObjectMapJoinPositionSend(short aIndex);

extern void gObjViewportListProtocol(short aIndex);


extern int  gObjGetUserNumber(int aIndex);
extern int  gObjGetNumber(int aIndex, int & UN, int & DBN);


#ifdef BUGFIX_GAIN_EXP_20071210
extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, INT64 &addexp, int iMonsterType, int iEventType);
#else
	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp, int iMonsterType, int iEventType);
	#else
		#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		extern bool gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp);
		#else
		extern void gObjLevelUp(LPOBJECTSTRUCT lpObj, int addexp);
		#endif
	#endif
#endif
extern int  gObjLevelUpPointAdd(BYTE type, LPOBJECTSTRUCT lpObj);

extern BYTE LevelSmallConvert(int aIndex, int inventoryindex);
extern BYTE LevelSmallConvert(int level);
extern BYTE gObjInventoryInsertItem(int aIndex, int item_num);
extern BYTE gObjInventoryInsertItem(int aIndex, CItem item);
extern BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item);
#ifdef PERSONAL_SHOP_20040113		
extern BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item);	// ���� ���� �κ����� (���λ��� ���� ����) �� �������� �������� �� �� ���
#endif
extern BYTE gObjInventoryInsertItemTemp(LPOBJECTSTRUCT lpObj, CMapItem *Item);
extern BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item);
extern BYTE gObjShopBuyInventoryInsertItemSerial(int aIndex, CItem item);

#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
extern BYTE gObjShopBuyInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur);
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117

#ifdef ADD_HP_POTION_LEVEL1_FIX_20040420
	extern BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur, int nLevel);	
#else
	extern BOOL gObjSearchItem(LPOBJECTSTRUCT lpObj, int item_type, int add_dur);
#endif

#ifdef MODIFY_OVERLAP_ITEM_DURATION_20070730
extern BOOL gObjOverlapItemDuration(LPOBJECTSTRUCT lpObj, int iItemCode, int iLevel, int iAddDuration, int iMaxDuration);
#endif // MODIFY_OVERLAP_ITEM_DURATION_20070730

extern BOOL gObjSearchItemMinus(LPOBJECTSTRUCT lpObj, int pos, int m_dur);

//#ifdef EVENT_LOTTO
extern BYTE gObjInventoryInsertItem(LPOBJECTSTRUCT lpObj, int type, int index, int level, int iSerial, int iDur);
//#endif

extern BYTE gObjInventoryDeleteItem(int aIndex, int itempos);
extern void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte);
extern void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
extern void gObjMakePreviewCharSet(int aIndex);

#ifdef PERSONAL_SHOP_20040113		
extern void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte);
extern void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
#endif

extern void gObjAddMsgSend(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int SubCode=0);
extern void gObjAddMsgSendDelay(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode=0);

#ifdef ADD_QPROCESSING_STATE_OF_DEAD_20050221
// ������ �޽��� ť�� ������Ʈ�� �߰��Ѵ�.
extern void gObjAddMsgSendDelayInSpecificQPos( LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition = 0 );
#endif

extern void InitFrustrum();
extern void CreateFrustrum(int x,int y, int aIndex);

extern void gObjAllDel();

extern BOOL gObjAttack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage=0, BOOL bCombo = FALSE);

extern void gObjMsgProc(LPOBJECTSTRUCT lpObj);
extern BOOL gObjBackSpring(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
extern BYTE GetPathPacketDirPos(int px, int py);
extern int  gObjCalDistance(LPOBJECTSTRUCT lpObj1, LPOBJECTSTRUCT lpObj2);


extern void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y);
extern int  gObjMagicManaUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic);

#ifdef NEW_FORSKYLAND2
extern int  gObjMagicBPUse(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic);
#endif

extern void GetMoveArea(short &x, short &y, BYTE MapNumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2, BYTE & dir);

extern BOOL gObjPosMagicAttack(LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, BYTE x, BYTE y);
extern BOOL gObjMovePlayer(int aIndex, BYTE mapnumber, BYTE x1, BYTE y1, BYTE x2, BYTE y2);
extern BOOL gObjMoveGate(int aIndex, int gatenumber);
extern void gObjMoveDataLoadingOK(int aIndex);
extern void gObjTeleport(int aIndex, int map, int x, int y);

extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE Type, BYTE Index, BYTE Level, BYTE & SkillNumber);

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// gObjMagicAdd - Skill Type ����
extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, INT aSkill, BYTE Level);
#else
extern int gObjMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif

extern CMagicInf *gObjGetMagic(LPOBJECTSTRUCT lpObj, int mIndex);
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
extern CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, int skillnumber);
#else
extern CMagicInf *gObjGetMagicSearch(LPOBJECTSTRUCT lpObj, BYTE skillnumber);
#endif


extern void MonsterAndMsgProc();
extern void MoveMonsterProc();


extern void gObjTradeCancel(int aIndex);
extern void gObjTradeOkButton(int aIndex);

extern BYTE gObjInventoryMoveItem(int aIndex, BYTE source, BYTE target, int & durSsend, int & durTsend, BYTE sFlag, BYTE tFlag, LPBYTE siteminfo );
extern void gObjInventory2MoveItem(int aIndex, BYTE source, BYTE target);
extern BYTE gObjTradeInventoryMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);
extern BYTE gObjTradeTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);
extern BYTE gObjInventoryTradeMove(LPOBJECTSTRUCT lpObj, BYTE source, BYTE target);

extern int gObjInventoryTrans(int aIndex);
extern int gObjInventoryCommit(int aIndex);

extern BOOL gObjItemLevelUp(LPOBJECTSTRUCT lpObj, int source, int target);
extern BOOL gObjItemRandomLevelUp(LPOBJECTSTRUCT lpObj, int source, int target);
extern BOOL gObjItemRandomOption3Up(LPOBJECTSTRUCT lpObj, int source, int target);


extern void gObjUseDrink(LPOBJECTSTRUCT lpObj, int level = 0);
extern void gObjAbilityReSet(LPOBJECTSTRUCT lpObj);

extern DWORD gGetItemNumber();
extern void gPlusItemNumber();


extern void gObjAllDisconnect();
extern void gObjAllLogOut();
extern void gObjCloseSet(int aIndex, int Flag);

extern BOOL gObjCheckViewport(int aIndex, int x, int y);
extern BOOL gObjPositionCheck(LPOBJECTSTRUCT lpObj);
extern BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis);

extern int  gObjGuildMasterCapacityTest(LPOBJECTSTRUCT lpObj);

#ifdef _NEW_EXDB_
//------------------------------------------------------------------------------------
// ���..
extern void gObjGuildWarEndSend(LPOBJECTSTRUCT lpObj, BYTE Result1, BYTE Result2);
extern void gObjGuildWarEndSend(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, BYTE Result1, BYTE Result2);
extern int  gObjGuildWarProc(LPGUILD_INFO lpGuild1, LPGUILD_INFO lpGuild2, int score);
extern void gObjGuildWarEnd(LPGUILD_INFO lpGuild, LPGUILD_INFO lpTargetGuild);
//------------------------------------------------------------------------------------
#endif

extern BOOL gObjGetPkTime(LPOBJECTSTRUCT lpObj, int & hour, int & min);
extern BOOL gObjInventorySearchSerialNumber(LPOBJECTSTRUCT lpObj, DWORD serial);
extern void gObjSetInventory1Pointer(LPOBJECTSTRUCT lpObj);

extern void gObjStatTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjMagicTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjItemTextSave(LPOBJECTSTRUCT lpObj);
extern void gObjWarehouseTextSave(LPOBJECTSTRUCT lpObj);

extern BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source);
extern void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte);

extern BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
extern void gObjMonsterCallKill(int aIndex);
extern BOOL gObjBackSpring2(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int count);


// �ɼ� ���� �κ�
extern void gObjSetTradeOption(int aIndex, int option);
#ifdef DUEL_SYSTEM_20031028
extern void gObjSetDuelOption(int aIndex, int option);
extern bool IsDuelEnable(int aIndex);
extern bool IsOnDuel (int aIndex1, int aIndex2);
#endif

//#HAPPY_CAT
extern void	gObjClearViewportOfMine(LPOBJECTSTRUCT lpObj);
//HAPPY_CAT#

extern void gObjClearViewport(LPOBJECTSTRUCT lpObj);
extern void gObjViewportListProtocolDestroy(LPOBJECTSTRUCT lpObj);
extern void gObjViewportListProtocolCreate(LPOBJECTSTRUCT lpObj);

extern int GetMapMoveLevel(LPOBJECTSTRUCT lpObj, int mapnumber, int max_over);

extern BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source);
extern void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte);

extern BOOL gObjCanItemTouch(LPOBJECTSTRUCT lpObj, int type);
extern void ItemIsBufExOption(LPBYTE buf, CItem * lpItem);

extern BOOL	gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y);
extern BOOL	gObjCheckAttackArea(int aIndex, int TarObjIndex);

extern bool gObjIsSelfDefense (LPOBJECTSTRUCT lpObj, int aTargetIndex);


#ifdef DUEL_SYSTEM_20031028
// Ư�� ����ڰ� ���������� üũ
extern BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj);
// ���� ���������� üũ
extern BOOL gObjDuelCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
// �������� �ʱ�ȭ
extern VOID gObjResetDuel(LPOBJECTSTRUCT lpObj);
// ������ ����Ǿ��ٴ� ��Ŷ�� ������
extern VOID gObjSendDuelEnd(LPOBJECTSTRUCT lpObj);
#endif

#ifdef DUEL_SYSTEM_FIX_20031224
extern VOID gObjCheckAllUserDuelStop ();		// 10�ʿ� �ѹ��� �������ε� 1�гѰ� �ο��� ���� ����� �ִ��� Ȯ�� -> ������ ��������
#endif

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
extern VOID	gObjSendTimeView(int aIndex);
#endif

//------------->
//20080930 �ؿ� 5�� �κ�����ȭ ��Ȱ, �̵� ���� (grooving)
#ifdef ADD_TELEPORT_CHARM_ITEM_20080930
extern	BOOL	gIsMoveCharmEnable(int iMapNum);
#endif //ADD_TELEPORT_CHARM_ITEM_20080930
//<-------------
// �Ǹ��� ���� �ʴ���� �ִ��� Ȯ��
extern BOOL	gUserFindDevilSquareInvitation(int aIndex);
// �Ǹ��� ����, ���� �ִ��� Ȯ��
extern BOOL	gUserFindDevilSquareKeyEyes(int aIndex);

// Ư�� ���� �ο����� �������� �̵� 
extern void	gObjBatchMoveGate(BYTE mapnumber, BYTE targetmap);

extern void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);

extern LPOBJECTSTRUCT	 gObjFind(char* targetcharname);

// ������ �̺�ƮĨ�� ã�´�
extern int		gObjFind10EventChip(int aIndex);
extern int		gObjDelete10EventChip(int aIndex);

// ��ġ�� ������ ������Ų��
void gObjSetPosition(int aIndex, int x, int y);

// �κ��丮���� Ư�� ������ ���� ��´�

#ifdef SECOND_QUEST
extern int		gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel = -1);
#else
extern int		gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex);
#endif
extern int		gObjGetItemCountInIventory(int aIndex, int itemnum);

extern int		gObjGetManaItemPos(int aIndex);

// �κ��丮���� Ư�� �����۸� ����ŭ ���� �Ѵ�
extern void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);

// ��������Ʈ�� �ø��� �ִ���
extern int	gObjCheckStatPoint(int aIndex);

// ���Ÿ� ����Ѵ�
extern void	gObjUseCircle(int aIndex, int pos);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
//  - 0) ���Ȼ���
extern void gObjUsePlusStatFruit(int aIndex, int pos);
//  - 1) ���� ����
extern void gObjUseMinusStatFruit(int aIndex, int pos);
#endif

#ifdef ADD_PCS_FRUITS_POINT_EXTENTION_20070430
extern void gObjUseExMinusFruit(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_FRUITS_POINT_EXTENTION_20070430

#ifdef ADD_PCS_IMPROVE_POTION_20070718
extern void gObjUseImprovePotion(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_IMPROVE_POTION_20070718

#ifdef ADD_PCS_INDULGENCE_20070504
extern void gObjUseIndulgence(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_PCS_INDULGENCE_20070504

#ifdef ADD_FULL_RECOVERY_POTION_20070719
extern void gObjUseFullRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos);
#endif // ADD_FULL_RECOVERY_POTION_20070719

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
void gObjUseRecoveryPotion(LPOBJECTSTRUCT lpObj, int iPos, double dRate = 1);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

#ifdef NEW_FORSKYLAND2
extern void	gObjSetBP(int aIndex);
#endif

extern void gObjCalcMaxLifePower(int aIndex);

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
extern void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
#else
extern void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint);
#endif

extern void gObjPenaltyDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
extern void gObjWeaponDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int type);
extern void gObjArmorRandomDurDown(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpAttackObj);
#ifdef _NEW_EXDB_
extern BOOL gObjTargetGuildWarCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
#endif
extern void gObjCheckSelfDefense(LPOBJECTSTRUCT lpObj, int aTargetIndex);

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
extern void gObjWeaponDurDownInCastle( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iDecValue );

#endif



extern BOOL gObjAttackQ(LPOBJECTSTRUCT lpObj);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage=0);
#else

#ifdef ADD_SHIELD_POINT_01_20060403
	#ifdef ADD_SKILL_WITH_COMBO
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill, int iShieldDamage=0);
	#else
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, int iShieldDamage=0);
	#endif
#else
	#ifdef ADD_SKILL_WITH_COMBO
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, BYTE Skill);
	#else
	extern void gObjLifeCheck(LPOBJECTSTRUCT lpTargetObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage);
	#endif
#endif // ADD_SHIELD_POINT_01_20060403
#endif // ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107


extern BOOL retResistance(LPOBJECTSTRUCT lpObj, int Resistance_Type);

// ��ų ���׷�
extern BOOL retCalcSkillResistance(LPOBJECTSTRUCT lpTargetObj, BOOL isDouble = FALSE);

extern void gObjSpriteDamage(LPOBJECTSTRUCT lpObj, int damage);
extern BOOL gObjSatanSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjAngelSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjWingSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjUniriaSprite(LPOBJECTSTRUCT lpObj);

#ifdef	ADD_SAFEGUARD_ITEM_20080702
extern BOOL gObjDaemonSprite(LPOBJECTSTRUCT lpObj);
extern BOOL gObjSafeGuardSprite(LPOBJECTSTRUCT lpObj);
#endif // ADD_SAFEGUARD_ITEM_20080702

extern void gObjNextExpCal(LPOBJECTSTRUCT lpObj);

#ifdef NEW_FORSKYLAND2
extern void	gObjDelayLifeCheck(int aIndex);
#endif


void SkillFrustrum(BYTE bangle, int aIndex);
BOOL SkillTestFrustrum(int x,int y, int aIndex);

#ifdef RING_EVENT_ITEMBOX_20031124		// �����̺�Ʈ - ������ ����߿��� �ش� �������� ������ �ľ�
int	 gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
#endif

#ifdef MODIFY_ZEN_MAX_20040414
// MAX_ZEN�� �Ѿ��°�?
extern BOOL gObjCheckMaxZen( int aIndex, int nAddZen );
#endif

#ifdef CHINA_GM_EVENT
extern BOOL	gGMEventRunning;
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
extern BOOL	gGMEventRunning;
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

extern BOOL gObjIsItemPut(LPOBJECTSTRUCT lpObj, CItem *lpItem, int pos);

#if GAME_VERSION >= G_V_99B
extern void MakeRandomSetItem(int aIndex);
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
extern BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, WORD aSkill);	// ������ ����Ҷ� ������ üũ�� �Ѵ�
#else
extern BOOL	gObjMagicEnergyCheck(LPOBJECTSTRUCT lpObj, BYTE aSkill);	// ������ ����Ҷ� ������ üũ�� �Ѵ�
#endif


#ifdef DARKLORD_WORK	
extern void	gObjRecall(int aIndex, int mapnumber, int x, int y);
extern void	gObjSetExpPetItem(int aIndex, int exp);
#endif

#ifdef ITEM_DUPLICATE_PREVENT_PATCH_20040719		// ������ ���� ������ �Լ���
extern bool gObjFixInventoryPointer(int aIndex);
#endif

#ifdef ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907
// Reward Type = 1 : ��п� ���� ����
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
extern void MakeRewardSetItem(int aIndex, UCHAR cDropX=0, UCHAR cDropY=0, int iRewardType=0, int iMapnumber = 0);
#else
extern void MakeRewardSetItem(int aIndex, UCHAR cDropX=0, UCHAR cDropY=0, int iRewardType=0);
#endif	// #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
#endif

#if TESTSERVER == 1
void	DebugOutput(int aIndex, char* msg, ...);
#endif
void	MsgOutput(int aIndex, char* msg, ...);

#ifdef CHARACTER_AUTO_RECUPERATION_SYSTEM_20050614
extern void gProcessAutoRecuperation(LPOBJECTSTRUCT lpObj);
#endif

#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518
// Ư������ cX, cY ��ó ���� �߿� ���ڸ��� �ϳ� ã�Ƴ���.
extern BOOL	gObjGetRandomFreeLocation (	INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount);
// �ش� �ɸ� (�����)�� ������ ������������ ��������.
extern INT	gObjCheckAttackTypeMagic ( INT iClass, INT iSkill);
#endif

#ifdef CASTLEDEEP_EVENT_20050413
extern BOOL gObjGetRandomFreeArea (INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iSX, INT iSY, INT iDX, INT iDY, INT iLoopCount);
#endif

#ifdef ADD_MONSTER_DIE_REWARD_ITEMS_20040915
extern BOOL gObjGetRandomItemDropLocation ( INT	iMapNumber,	UCHAR & cX,	UCHAR & cY, INT iRangeX, INT iRangeY, INT iLoopCount);
#endif

#ifdef	HAPPY_CAT
extern void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick);	// happycat@20050201
extern void	gObjReady4Relife(LPOBJECTSTRUCT lpObj);							// happycat@20050201
#endif //HAPPY_CAT

#ifdef MODIFY_SEASON_01_20060317
	#ifndef HAPPY_CAT
	extern void	gObjCheckTimeOutValue(LPOBJECTSTRUCT lpObj, DWORD & rNowTick);	// happycat@20050201
	extern void	gObjReady4Relife(LPOBJECTSTRUCT lpObj);							// happycat@20050201
	#endif // HAPPY_CAT
#endif // MODIFY_SEASON_01_20060317

#ifdef PCBANG_ADVANTAGE
extern BOOL	IsOKPCBangBenefit(LPOBJECTSTRUCT lpObj);						// happycat@20050201
#endif

// PC�� ���� ���θ� �� �� ���� ���ǿ� ������� �Ǵ��Ѵ�.
#ifdef PCBANG_ADVANTAGE_EXPAND_20060919
extern BOOL	IsOKPCBangBenefitAll(LPOBJECTSTRUCT lpObj);
#endif

#ifdef FOR_TAIWAN
// �븸�� �ֹ� ��ȣ üũ�� Ʋ����.
extern BOOL gObjTaiwanJoominCheck(int aIndex, char *szInJN);
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
extern int gObjCheckOverlapItemUsingDur( int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel );
extern int gObjOverlapItemUsingDur( CItem *lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel );
#endif

#ifdef ANTIHACKING_SERIAL_0_ITEM_20051202
// Ư�� ������ �ø��� 0 �� ���� ��� FALSE �� ��ȯ�Ѵ�.
extern BOOL gObjCheckInventorySerial0Item(LPOBJECTSTRUCT lpObj);

extern BOOL gObjCheckSerial0ItemList( CItem *lpItem );

#endif // ANTIHACKING_SERIAL_0_ITEM_20051202

// ��ų ���� ��, ������ ���� ��ġ���� Ȯ��
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413

extern BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);

#endif //MODIFY_SKILL_CHECK_DISTANCE_20060413

#ifdef MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328
extern void gObjSaveChaosBoxItemList(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_SAVE_CHAOSBOX_ITEMLIST_AT_LOGOUT_20070328

#ifdef EXPERIENCE_SERVER_NPC_STAT_RESET
extern void gStatReset( int iIndex );
#endif //EXPERIENCE_SERVER_NPC_STAT_RESET

#ifdef ADD_GAMBLING_20090120
extern BOOL gObjGamblingInventoryCheck( int aIndex, int iWidth, int iHeight );
#endif // ADD_GAMBLING_20090120

extern void gObjMagicAddEnergyCheckOnOff(int flag);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
extern int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, WORD aSkill, BYTE Level);
#else
extern int gObjWeaponMagicAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
#endif