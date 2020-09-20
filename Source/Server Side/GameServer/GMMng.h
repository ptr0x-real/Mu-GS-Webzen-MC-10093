// GMMng.h: interface for the CGMMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_)
#define AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#ifdef GMCOMMAND_EX
#define MASK_GMCMDEX				0x01F4	//500
#endif // GMCOMMAND_EX

#ifdef MODIFY_MAX_GMCMD_EXTEND_20080630
	#if TESTSERVER == 1
		#define MAX_GMCOMMAND		500
	#else
		#define MAX_GMCOMMAND		450
	#endif // TESTSERVER == 1
#else
	#if TESTSERVER == 1 
		#ifdef JS_ADD_TEST_COMMAND
			// 20060719 100 -> 150 ���� (apple)
			#define MAX_GMCOMMAND		150
		#endif
	#else
		#ifdef ADD_MACRO_FOR_SETITEM_20060424
			// 20060719 60 -> 100 ���� (apple)
			#define MAX_GMCOMMAND		100
		#else
			// 20060719 50 -> 100 ���� (apple)
			#define MAX_GMCOMMAND		100
		#endif
	#endif
#endif // MODIFY_MAX_GMCMD_EXTEND_20080630

#define GMCMD_DISCONNECT			100		// ���� ���� ����
#define GMCMD_SETPOSITION			101		// ���� ��ġ �̵�  
#define GMCMD_COPYCHAR				102		// ĳ���� ����&����
#define GMCMD_ACCOUNTBLOCK			103		// ���� ��
#define GMCMD_CHATBAN				104		// ä�� ����
#define GMCMD_ACCOUNTBLOCKFREE		105		// ���� �� ����
#define GMCMD_CHATBANFREE			106		// ä�� ���� ����
#define GMCMD_ITEMCREATE			107		// ������ ����
#define GMCMD_GUILDSETPOSITION		108		// ��� ���� ��ġ �̵�  
#define GMCMD_GUILDWARSTART			109		// ���� ������ �˸���.
#define GMCMD_GUILDWARSTOP			110		// ���� ��� �ߴ��� �˸���.
#define GMCMD_GUILDWAREND			111		// ���� ���Ḧ �˸���.
#define GMCMD_GUILDDISCONNECT		112		// ��� ���Ḧ �˸���.
#define GMCMD_SETPOSITION2			113

#define UMCMD_GUILDWAR				200		// ��� ����
#define UMCMD_BILLCHECK				201		// �����ð� Ȯ��
#define UMCMD_BATTLESOCCERWAR		202		// �����౸ 
#define UMCMD_TRADEOPTION			203		// ��û on/off

#define GMCMD_USERSTAT				214		// ������Ȳ
#define GMCMD_USERTRAKING			215		// ����
#define GMCMD_USERWATCHNG			216		// ����

#define GMCMD_FIRECRACK				217		// ����

#define GMCMD_STATRESET				218		// ���� ����
#define GMCMD_QUESTRESET			219		// ���� ����


#define GMCMD_MANYMANYBLESS			230		// ���â
#define GMCMD_MANYMANYSOUL			231		// ����â
#define GMCMD_MAKESETITEM			232		// ����â

#define GMCMD_MAKERANDOMSETITEM		233		// ������Ʈ�����

#define GMCMD_CHANGEPOS				220		// ��ǥ �̵�

#ifdef CHINA_GM_EVENT	
#define GMCMD_START_GMEVENT			221
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
#define GMCMD_START_GMEVENT			221
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906

#ifdef JS_ADD_TEST_COMMAND
#define GMCMD_MANYMANYCHAOS			301
#define GMCMD_MANYMANYLIFE			302
#define GMCMD_MANYMANYCREATE		303
#define GMCMD_MANYMANYGEM			304
#define GMCMD_MODIFYSTAT			305
#define GMCMD_INFORMITEMINDEX		308
#define GMCMD_NOTICEALL				309
#define GMCMD_RESETZEN				310
#define GMCMD_MANYMANYITEM			311

#define GMCMD_CREATEPROPERTYITEM	313
#define GMCMD_SETPKLEVEL			314
#define GMCMD_SETPKPOINT			315

#define GMCMD_CREATEITEMNAME		318
#define GMCMD_NOTIFYMONSTERHP		319
#define GMCMD_SET1004				330		// PK ���� �ʱ�ȭ
#define GMCMD_MODIFYSTAT_ENG		339		// ���ȼ���.������.
#define GMCMD_CLASS_UP				341		// ����
#define GMCMD_FENRIR_DURSMALL_DEF	342		// �渱 ������ ����ġ �⺻ DurSmall
#define GMCMD_FENRIR_DURSMALL_ELF	343		// �Ҹ� ������ ����ġ ���� DurSmall
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
#define GMCMD_SET_SHIELD_POINT		349		// ���� ��ġ ����
#define GMCMD_POINT_DISPLAY			350		// ���� ��ġ ǥ��
#endif

#ifdef ITEM_ADD_OPTION_20061019
#define GMCMD_ITEM_EFFECT_CLEAR		351		// ������ ȿ�� �ʱ�ȭ
#define GMCMD_ITEM_EFFECT_SET		352		// ������ ȿ�� ����
#endif


#define GMCMD_SETLEVEL				312		// ��������

#ifdef DARKLORD_WORK
#define GMCMD_DARKSPIRIT			316		// ��ũ���Ǹ� ���
#define GMCMD_DARKSPIRIT_LEVEL		317		// ��ũ���Ǹ� ���
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
#define GMCMD_NOTIFYKUNDUNHP		320		// ��� �Ǻ���
#define GMCMD_SETKUNDUNHP			321		// ��� �Ǽ���
#define GMCMD_SETKUNDUNREFILLHP		322		// ��� ��ȸ��������
#define GMCMD_SETKUNDUNREFILLHPSEC	323		// ��� �ʴ���ȸ��������
#define GMCMD_SETKUNDUNREFILLHPTIME	324		// ��� ��ȸ�� �ð� ����
#endif

#ifdef CASTLE_GM_COMMAND_20041222	// ��� �������� ����߰�
#define GMCMD_CS_CHANGEOWNERGUILD			331		// "/�����������"
#define GMCMD_CS_SETSTATE_REGSIEGE			332		// "/�����������"
#define GMCMD_CS_SETSTATE_REGMARK			333		// "/����������"
#define GMCMD_CS_SETSTATE_NOTIFY			334		// "/��������"
#define GMCMD_CS_SETSTATE_STARTSIEGE		335		// "/��������"
#define GMCMD_CS_SETSTATE_ENDSIEGE			336		// "/��������"
#define GMCMD_CS_SETSTATE_GETOWNERGUILD		337		// "/�����������"
#define GMCMD_CS_SETSTATE_GETCASTLESTATE	338		// "/�����������"
#define GMCMD_CS_CHANGECSSIDE				340		// "/������ü"
#endif

#ifdef CRYWOLF_GM_COMMAND_20050525	// ũ���̿��� ��ɾ�
#define GMCMD_CRYWOLF_READY		345	// "/ũ���̿����غ�"
#define	GMCMD_CRYWOLF_START		346	// "/ũ���̿�������"
#define GMCMD_CRYWOLF_END		347 // "/ũ���̿�������"
#define GMCMD_CRYWOLF_NOTIFY_2	348 // "/ũ���̿�������"
#endif

#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410	// ���Ǵ�Ƽ �ַο� ���� ��ɾ�	
#define GMCMD_GETINFINITYARROW_MP		360	// "/�������Ǵ��¾ַο�"
#define GMCMD_SETINFINITYARROW_MP_0		361	// "/���Ǵ�Ƽ�ַο�+0"
#define GMCMD_SETINFINITYARROW_MP_1		362	// "/���Ǵ�Ƽ�ַο�+1"
#define GMCMD_SETINFINITYARROW_MP_2		363	// "/���Ǵ�Ƽ�ַο�+2"
#define GMCMD_INFINITYARROW_SETTIME		364	// "/����Ƽ�Ͼַο�ð�"
#endif

#ifdef MODIFY_LIFEANDMANA_BUG_LEVELUP_RELOGIN_20065018
#define GMCMD_LEVELUP_TEST	365
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ���� ��ɾ�
#define GMCMD_FIRE_SCREAM				366	// "/���߰Ÿ�"
#endif

	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530	// ��ȭ���� �ɼ� �׽�Ʈ
		#define GMCMD_MAKE_STRENGTHENITEM 367
	#endif

	#ifdef ADD_380ITEM_NEWOPTION_20060711	// 380 ������ �ɼ� �׽�Ʈ
		#define GMCMD_MAKE_380OPTIONITEM 368
	#endif

#ifdef KANTURU_GM_COMMAND_20060709
#define GMCMD_KANTURU_STANBY						369	// "/ĭ����������"
#define GMCMD_KANTURU_BATTLEOFMAYA					370	// "/ĭ����������"
#define GMCMD_KANTURU_MAYA_MONSTER1					371	// "/���߸���1"
#define GMCMD_KANTURU_MAYA1							372	// "/����1"
#define GMCMD_KANTURU_MAYA_MONSTER2					373	// "/���߸���2"
#define GMCMD_KANTURU_MAYA2							374	// "/����2"
#define GMCMD_KANTURU_MAYA_MONSTER3					375	// "/���߸���3"
#define GMCMD_KANTURU_MAYA3							376	// "/����3"
#define GMCMD_KANTURU_MAYA3_SUCCESS					377	// "/����3����"
#define GMCMD_KANTURU_MAYA_SUCCESS					378	// "/���߼���"
#define GMCMD_KANTURU_MAYA_FAIL						379	// "/���߽���"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE				380	// "/ĭ��������Ʈ�޾���"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE_SUCCESS		381	// "/����Ʈ�޾��"
#define GMCMD_KANTURU_BATTLEOFNIGHTMARE_FAIL		382	// "/����Ʈ�޾��"
#define GMCMD_KANTURU_TOWEROFREFINEMENT				383	// "/ĭ����������ž"
#define GMCMD_KANTURU_TOWER_OPEN					384	// "/������ž����"
#define GMCMD_KANTURU_TOWER_CLOSE					385	// "/������ž�ݱ�"
#define GMCMD_KANTURU_END							386	// "/ĭ��������"
#define GMCMD_KANTURU_NEXT_STATE					387	// "/ĭ������������"
#define GMCMD_KANTURU_NEXT_DETAIL_STATE				388	// "/ĭ�����������λ���"
#define GMCMD_KANTURU_CURRENT_STATE					389	// "/ĭ�����������"
#endif	// #ifdef KANTURU_GM_COMMAND_20060709

#ifdef UPDATE_GM_FUNCTION_20070228		// "/����" "/��������" �ε��� ����
#define GMCMD_INVISIBLE								390 // "/����"
#define GMCMD_REMOVE_INVISIBLE						391 // "/��������"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ��� �̺�Ʈ ��ũ��
#define GMCMD_ILLUSION_TEMPLE_READY					392	// �̺�Ʈ �غ�
#define GMCMD_ILLUSION_TEMPLE_START					393	// �̺�Ʈ ����
#define GMCMD_ILLUSION_TEMPLE_END					394	// �̺�Ʈ ����
#define GMCMD_ILLUSION_TEMPLE_RESET					395 // �̺�Ʈ ���� ����
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// /����3 ��ɾ� �߰�
#define GMCMD_THIRD_CHANGE_UP						396	// "/����3"
#define GMCMD_CHANGE_UP_ALL_RESET					397	// "/��������"
#define GMCMD_QUEST_ITEM							398 // "/���� (����)"
#define GMCMD_MANYMANY_ITEM							399	// "/�ۿ�â (Ÿ��) (�ε���) (����)"
#define GMCMD_SET_QUEST								400	// "/������ (����)"
#define GMCMD_SET_ZEN								401	// "/�� (����)"
#endif

#ifdef ADD_PCS_LOTTERY_SYSTEM_20070117
	#define GMCMD_LOTTERY_ITEM_CREATE				402
#endif // ADD_PCS_LOTTERY_SYSTEM_20070117

#ifdef MODIFY_INVEN_CLEAR_MACRO_20071212
	#define GMCMD_INVENTORY_ITEM_DELETE				403
	#define GMCMD_INVENTORY_ITEM_ALL_DELETE			404
	#define GMCMD_DEVILSQUARE_SET_TIME				405
#endif // MODIFY_INVEN_CLEAR_MACRO_20071212

#ifdef ADD_MASTER_LEVEL_SYSTEM_COMMAND_20071122
#define GMCMD_MASTERLEVEL_LEVELUP					406	// "/masterlevel (����)"
#define GMCMD_MASTERLEVEL_POINT						407	// "/mlpoint (����)"
#define GMCMD_MASTERLEVEL_SKILL_RESET				408	// "/ms_reset"
#endif

#ifdef ADD_RAKLION_20080408
#define GMCMD_RAKLION_IDLE							409 // "/��Ŭ���´��"
#define GMCMD_RAKLION_NOTIFY_1						410 // "/��Ŭ���°���1"
#define GMCMD_RAKLION_STANDBY						411 // "/��Ŭ�����غ�"
#define GMCMD_RAKLION_NOTIFY_2						412 // "/��Ŭ���°���2"
#define GMCMD_RAKLION_READY							413 // "/��Ŭ���°���2"
#define GMCMD_RAKLION_START_BATTLE					414 // "/��Ŭ���½���"
#define GMCMD_RAKLION_NOTIFY_3						415 // "/��Ŭ���°���3"
#define GMCMD_RAKLION_CLOSE_DOOR					416 // "/��Ŭ�������"
#define GMCMD_RAKLION_ALL_USER_DIE					417 // "/��Ŭ���½���"
#define GMCMD_RAKLION_NOTIFY_4						418 // "/��Ŭ���°���4"
#define GMCMD_RAKLION_END							419 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_1						420 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_2						421 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_3						422 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_4						423 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_5						424 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_6						425 // "/��Ŭ��������"
#define GMCMD_RAKLION_SELUPAN_7						426 // "/��Ŭ��������"
#define GMCMD_RAKLION_FIRST_SKILL					427 // "/���������ʽ�ų"
#define GMCMD_RAKLION_POISON_ATTACK					428 // "/�����ǵ�����"
#define GMCMD_RAKLION_ICE_STORM						429 // "/�����ǳñ���ǳ"
#define GMCMD_RAKLION_ICE_STRIKE					430	// "/�����ǳñ����"
#define GMCMD_RAKLION_SUMMON_MONSTER				431 // "/�����Ǽ�ȯ"
#define GMCMD_RAKLION_HEAL							432 // "/��������"
#define GMCMD_RAKLION_FREEZE						433 // "/�����Ǿ󸮱�"
#define GMCMD_RAKLION_TELEPORT_RANDOM				434 // "/�����Ǽ����̵�"
#define GMCMD_RAKLION_INVINCIBILITY					435 // "/�����ǹ���ȭ"
#endif // ADD_RAKLION_20080408

#ifdef ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415
#define GMCMD_SET_SOCKET_OPTION						436	// "/setsocket (��������ġ) �õ��ε��� ���Ǿ�� ������ġ
#define GMCMD_CLEAR_SOCKET_OPTION					437	// "/clearsocket (��������ġ) ������ġ

#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710
#define GMCMD_MAKE_SOCKET_SLOT						438	// "/makesocketslot (��������ġ) ���Ͻ��Լ�
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_04_20080710

#ifdef MODIFY_SPEEDHACK_20080605
#define GMCMD_ABILITY_DEBUG							439 // "/abilitydebug (1,0)"
#endif // MODIFY_SPEEDHACK_20080605
#endif // ADD_ITEM_SOCKET_OPTION_SYSTEM_20080415

#if TESTSERVER == 1 

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
#define GMCMD_XMASEVENT_START						440	// "/xmaseventstart (1,0)"
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#ifdef ADD_GIFTITEM_XMASEVENT_20081030
#define GMCMD_XMASEVENT_SETVISITORCNT				441
#define GMCMD_XMASEVENT_SETSANTAMODE				442
#endif //ADD_GIFTITEM_XMASEVENT_20081030

#endif //TESTSERVER

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
#define GMCMD_SETINFINITYARROW_MP_3					443 // "/���Ǵ�Ƽ�ַο�+3"
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_NEWPVP_PKFIELD
#define GMCMD_NEWPVP_OBSERVER						444	// "��������"
#define GMCMD_NEWPVP_GLORYOFGLADIATOR				445	// "/�˸�"
#endif // ADD_NEWPVP_PKFIELD

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
#define GMCMD_ADD_HACKTOOL_MOVE_BLOCK				446	// "/��������߰�"
#define GMCMD_MODIFY_HACKTOOL_MOVE_BLOCK			447 // "/������ϼ���"
#define GMCMD_REMOVE_HACKTOOL_MOVE_BLOCK			448 // "/������ϻ���"
#define GMCMD_STATISTICS_HACKTOOL					449	// "/�������"
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef PCBANG_POINT_SYSTEM_20070206
#define GMCMD_ADD_PCBANG_POINT						450 // "/addpcbangpoint 10"
#define GMCMD_RESET_PCBANG_POINT					451 // "/resetpcbangpoint"
#endif // PCBANG_POINT_SYSTEM_20070206

#define GMCMD_TIME									452
#define GMCMD_PKCLEAR								453
#define GMCMD_ONLINE								454
#define GMCMD_ADDSTR								455
#define GMCMD_ADDDEX								456
#define GMCMD_ADDVIT								457
#define GMCMD_ADDENE								458
#ifdef DARKLORD_WORK
#define GMCMD_ADDCOM								459
#endif
#define GMCMD_JOINPVP								460
#define GMCMD_RESETCOMMAND							461
#define GMCMD_VIPINFO								462
#define GMCMD_LIFE_POTION							463
#define GMCMD_MANA_POTION							464
#define GMCMD_BAN_USER								465
#define GMCMD_OFFAFK                                466
#define GMCMD_OFFTRADE                              467
#define GMCMD_OFFEXP                                468
#define GMCMD_DUELINFO                              469
#define GMCMD_ATTACK                                470
#define GMCMD_NOATTACK                              471
#define GMCMD_OFFCASH                               472
#define GMCMD_BOTSTORE                              473

#ifdef GMCOMMAND_EX
enum EnumCheatCode
{
	GMCMD_GETABILITY				= MASK_GMCMDEX | 1,		// "/���� [ĳ���͸�]"
	GMCMD_PARTYINFO,										// "/��Ƽ [ĳ���͸�]"
	GMCMD_SUMMONMONSTER,									// "/��ȯ [���͸�]"
	GMCMD_CLEARMONSTER,										// "/��������"
	GMCMD_CLEARITEM,										// "/����������"
	GMCMD_CLEARINVEN,										// "/�κ�����"
	GMCMD_SUMMONCHARACTER,									// "/ĳ���ͼ�ȯ [ĳ���͸�]"
};
#endif // GMCOMMAND_EX

#define MAX_MANAGER					5

class CGMCommand
{

public :

	char	szCmd[MAX_GMCOMMAND][20];
	int		nCmdCode[MAX_GMCOMMAND];
	DWORD	dwAuthority[MAX_GMCOMMAND];
	int  count;

	CGMCommand() 
	{ 
		Init();
	}

	void Init()
	{
		for( int n=0; n<MAX_GMCOMMAND; n++) nCmdCode[0] = -1; 
		count = 0;	
	}

	void Add(char * command_str, int command_code, DWORD dwInAuthority)
	{
		strcpy(szCmd[count], command_str);
		nCmdCode[count] = command_code;

		// Admin, EventGm ���� ���� ��� 20081215 (grooving)
#ifdef MODIFIY_AUTHORITY_INTERGRATION_20081215
		dwAuthority[count] = dwInAuthority | AUTHORITY_ADMIN | AUTHORITY_EVENT_GM | AUTHORITY_SUPER_ADMIN;
#else // MODIFIY_AUTHORITY_INTERGRATION_20081215
		dwAuthority[count] = dwInAuthority | AUTHORITY_SUPER_ADMIN;
#endif // MODIFIY_AUTHORITY_INTERGRATION_20081215

		if(g_GlobalConfig.CheckCommand(command_code) == true)
		{
			dwAuthority[count] = g_GlobalConfig.GetCommandAuthority(command_code);
		}

		if( count < MAX_GMCOMMAND-1 ) count++;
	}
};

enum INCREASE_STAT_ENUM
{
	IS_STRENGTH,
	IS_DEXTERITY,
	IS_VITALITY,
	IS_ENERGY,
	IS_COMMAND
};

class CGMMng
{
public:
	
	CGMCommand	cCommand;
	

	CGMMng();
	virtual ~CGMMng();
	void Init();
	int GetCmd(char* szCmd);
	bool SearchCmd(char* szCmd, int &iOutCmd, DWORD &iOutAuthority);
	int ManagementProc(LPOBJECTSTRUCT lpObj, char* szCmd, int aIndex);
	char* GetTokenString();
	int   GetTokenNumber();
	int   GetTokenNumberEx();

	////////////////////////////////////////////////////////////////////////////
	
#ifdef ADD_MACRO_INIFINITY_ARROW_CONTROL_20060410
	void GetInfinityArrowMPConsumption( LPOBJECTSTRUCT lpObj );
	void ControlInfinityArrowMPConsumption0( LPOBJECTSTRUCT lpObj, int iValue );
	void ControlInfinityArrowMPConsumption1( LPOBJECTSTRUCT lpObj, int iValue );
	void ControlInfinityArrowMPConsumption2( LPOBJECTSTRUCT lpObj, int iValue );
	void SetInfinityArrowTime( LPOBJECTSTRUCT lpObj, int iValue );
#endif

#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
	void ControlInfinityArrowMPConsumption3( LPOBJECTSTRUCT lpObj, int iValue );
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	void ControlFireScreamDoubleAttackDistance( LPOBJECTSTRUCT lpObj, int iValue );
#endif

	////////////////////////////////////////////////////////////////////////////
	char szManagerName[MAX_MANAGER][MAX_IDSTRING+1];
	int  ManagerIndex[MAX_MANAGER];

	int	 WatchTargetIndex;

	void ManagerInit();
	int  ManagerAdd(char *name, int aIndex);
	void ManagerDel(char *name);
	void ManagerSendData(char *szMsg, int size);
	void BattleInfoSend(char *Name1, BYTE score1, char *Name2, BYTE score2);

	void	DataSend(LPBYTE szMsg, int size);
	////////////////////////////////////////////////////////////////////////////
#ifdef GMCOMMAND_EX
private:
	void	_GetUserAbility(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);
	void	_GetPartyInfo(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);
	void	_SummonMonster(LPOBJECTSTRUCT lpObj, LPCSTR lpszName, int nCount);
	void	_ClearMonster(LPOBJECTSTRUCT lpObj, int nDistance);
	void	_ClearItem(LPOBJECTSTRUCT lpObj, int nDistance);
	void	_ClearInven(LPOBJECTSTRUCT lpObj);
	void	_SummonCharacter(LPOBJECTSTRUCT lpObj, LPCSTR lpszName);

	int		_GetObjectType(WORD wClass);
#endif // GMCOMMAND_EX

	void	IncreaseStat(LPOBJECTSTRUCT lpObj, int Points, INCREASE_STAT_ENUM type);

public:
	void	ResetViewport(LPOBJECTSTRUCT lpObj);
};


extern CGMMng cManager;

#endif // !defined(AFX_GMMNG_H__53E6F8F0_D7DE_48E7_A7AD_E75FB75257D4__INCLUDED_)

