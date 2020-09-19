// IllusionTempleEvent.h: interface for the CIllusionTempleEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILLUSIONTEMPLEEVENT_H__BFE60AE1_BC06_4FA8_8A19_3E391ABD4560__INCLUDED_)
#define AFX_ILLUSIONTEMPLEEVENT_H__BFE60AE1_BC06_4FA8_8A19_3E391ABD4560__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#include "IllusionTempleProcess.h"
#include "..\\Include\\cb_protocol.h"


// ī���� ���� �ּ� ����
#define ILLUSION_TEMPLE_CHAOS_MIX_MIN_LEVEL		 10	

// ȯ�� ��� ī�������� ����Ȯ��
const int g_IllusionTempleChaosRate [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	70,
	70,
	70,
	70,
	70,
	70,
};

// ȯ�� ��� ī�������� �ʿ� ��
const int g_IllusionTempleChaosMixZen [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	3000000,
	5000000,
	7000000,
	9000000,
	11000000,
	13000000,
};

// ���� �ð� ����
typedef struct __ILLUSION_TEMPLE_STARTTIME
{
	int nHour;
	int nMin;
} ILLUSION_TEMPLE_STARTTIME;


class CIllusionTempleEvent  
{
public:
	CIllusionTempleEvent();
	virtual ~CIllusionTempleEvent();

private:
	CIllusionTempleProcess m_cIllusionTempleProc[MAX_ILLUSION_TEMPLE_COUNT];
	int		m_nMirageIndex;					// �̶��� �ε���

public:
	static	int m_nIllusionTempleOpenTime;
	static	int m_nIllusionTempleReadyTime;
	static	int m_nIllusionTemplePlayTime;
	static	int m_nIllusionTempleEndTime;

	static list <ILLUSION_TEMPLE_STARTTIME>	m_lstIllusionTempleStartTime;	// ���۽ð� ���� ����Ʈ

public:
	BOOL	IllusionTempleEventInit();

	BOOL	LoadIllusionTempleEventInfo();
	BOOL	LoadIllusionTempleScript(char* filename);

	void	IllusionTempleRun();	

	// ������ �������� index�� �κ��丮�� ��ġ�� ����
	void	SetInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos);
	void	SendRelicsUser(LPOBJECTSTRUCT lpObj);	// ������ ��������� ����ڵ鿡�� ����

	void	SeStatusInfo(BYTE btMapNumber, int nStatusIndex, short wStatusPosNum);		// ������ �⺻���� ����

	BOOL	EnterIllusionTemple( int aIndex, BYTE btTempleIndex, BYTE btItemPos );		// �̺�Ʈ�� ����
	BOOL	GEReqIllusionTempleEnterCountCheck(int aIndex, BYTE btTempleIndex, BYTE btItemPos, int nItemLevel);	// ���� Ƚ�� �˻� ��û
	void	EGAnsIllusionTempleEnterCountCheck(LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK aRecv);				// ���� Ƚ�� �˻� ����
	void	GEReqIllusionTempleIncEnterCount(LPOBJECTSTRUCT lpObj);												// ���� Ƚ�� ���� ��û
	BOOL	LeaveIllusionTemple( int aIndex, BYTE btMapNumber);							// �̺�Ʈ �ʿ��� ����
	
	BOOL	CheckEnterLevel( int nIndex, int nItemLevel);								// �����Ҽ� �ִ� ���� üũ
	
	void	IllusionHolyRelicsStatusAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// ���� �ൿ
	void	HolyRelicsBoxAlliedAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ���� �������ൿ(���ձ�)
	void	HolyRelicsBoxIllusionAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// ���� �������ൿ(ȯ����)
	void	PlatformMirageAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);								// �̶��� �ൿ

	void	DeleteRelicsItem(BYTE btMapNumber, LPOBJECTSTRUCT lpObj);			// ������ ����
	void	DropRelicsItem(BYTE btMapNumber, int nIndex);						// ������ ���

	void	SetIllusionTempleState(BYTE btState, BYTE btMapNumber);				// ���¸� ����
	BYTE	GetIllusionTempleState(BYTE btMapNumber);
	
	void	AddNpcPosNum(BYTE btMapNumber, int nClass, int nPosNum);			// ȯ�� ��� NPC�� ��ġ���̺� ��ȣ�� ����
	void	AddMonsterPosNum(BYTE btMapNumber, int nClass, int nPosNum);		// ȯ�� ��� Monster�� ��ġ���̺� ��ȣ�� ����
	void	ResetPosNum();														// NPC, ������ ��ġ������ ����
	
	BYTE	GetUserTeam(BYTE btMapNumber, int nIndex);

	void	SetStatusRegenTime(BYTE btMapNumber);

	void	IllusionTempleChaosMixItem(LPOBJECTSTRUCT lpObj);					// ���� �η縶�� ������ ����

	BYTE	IncSkillPoint(int nIndex, BYTE btMapNumber, BYTE btObjectType);		// ��ų ����Ʈ ����
	BYTE	DecSkillPoint(int nIndex, BYTE btMapNumber, BYTE btDecPoint);		// ��ų ����Ʈ ����

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// ��ų��ȣ BYTE -> WORD
	void	IllusionTempleUseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis);
#else
	void	IllusionTempleUseSkill(int iIndex, BYTE MagicNumber, int wTargetObjIndex, BYTE btDis);
#endif
	void	EventSkillProc(LPOBJECTSTRUCT lpObj);								// ��ų ���� ó��

	void	IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);						// ����ڰ� �׾��� ���� ó��
	void	IllusionTempleUserDieRegen(LPOBJECTSTRUCT lpObj);					// ����ڰ� �׾��� �ٽ� ��Ƴ����� ó��

	void	IncUseSkillCount(int nIndex, BYTE btMapNumber);						// ������� ��ų ��� Ƚ�� ó��

	void	SetMirageIndex(int nIndex);											// �̶��� �ε����� ����
	void	SendIllusionTempleUserCount();										// ���� �ð� ���� ����ڼ� ����

	BOOL	CheckWearingMOPH(int nUserIndex);									// �ش� ����ڰ� ���Ź����� �����ϰ� �ִ��� Ȯ���Ѵ�.
	BOOL	CheckChangeRing(int nType);											// �ش� �������� ������������ �˻�
	
	BOOL	CheckRelics(int nIndex);											// ĳ���Ͱ� ������ ������ �ִ��� �˻�

	void	MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);							// ���Ͱ� �׾��� ��� ������ ���

	void	ReqEventReward(int nIndex);											// �¸� ���� ��û

	BYTE	GetRemainTime();													// ��1ȯ������� �����ð� ����

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	BOOL	CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber);				// ��ȣ���ֹ� ��������� �˻�
	BOOL	CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber);				// �ӹ����ֹ��� �ɸ��������� �˻�
#endif

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int		CheckCanEnterLevel(int aIndex);
	int		CheckCharmCount(int aIndex);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	
#ifdef ADD_SEASON_3_NEW_UI_20071122	// ȯ����� ����Ƚ�� ��û
	// [0x14] ȯ������� ���� �Ϸ� ����Ƚ���� ��û�Ѵ�.
	void	EGReqIllusionTempleEnterCount(int iIndex);
	// [0x14] ȯ������� ���� �Ϸ� ����Ƚ���� �޴´�.
	void	EGAnsIllusionTempleEnterCount(LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT lpMsg);
#endif	// ADD_SEASON_3_NEW_UI_20071122
	
private:
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	bool	IllusionTempleChaosMix(int nIndex, int nLEVEL, int iCharmOfLuckCount);						// �������� ���� �η縶�� ����
#else
	bool	IllusionTempleChaosMix(int nIndex, int nLEVEL);						// �������� ���� �η縶�� ����
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int		CheckChaosMixItem(int nIndex);										// ������ �Ҽ� �ִ��� Ȯ��
};

extern CIllusionTempleEvent g_IllusionTempleEvent;

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#endif // !defined(AFX_ILLUSIONTEMPLEEVENT_H__BFE60AE1_BC06_4FA8_8A19_3E391ABD4560__INCLUDED_)
