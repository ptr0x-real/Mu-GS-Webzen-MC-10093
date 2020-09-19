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


// 카오스 조합 최소 레벨
#define ILLUSION_TEMPLE_CHAOS_MIX_MIN_LEVEL		 10	

// 환영 사원 카오스조합 성공확률
const int g_IllusionTempleChaosRate [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	70,
	70,
	70,
	70,
	70,
	70,
};

// 환영 사원 카오스조합 필요 젠
const int g_IllusionTempleChaosMixZen [MAX_ILLUSION_TEMPLE_COUNT] = 
{
	3000000,
	5000000,
	7000000,
	9000000,
	11000000,
	13000000,
};

// 시작 시간 정보
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
	int		m_nMirageIndex;					// 미라쥬 인덱스

public:
	static	int m_nIllusionTempleOpenTime;
	static	int m_nIllusionTempleReadyTime;
	static	int m_nIllusionTemplePlayTime;
	static	int m_nIllusionTempleEndTime;

	static list <ILLUSION_TEMPLE_STARTTIME>	m_lstIllusionTempleStartTime;	// 시작시간 정보 리스트

public:
	BOOL	IllusionTempleEventInit();

	BOOL	LoadIllusionTempleEventInfo();
	BOOL	LoadIllusionTempleScript(char* filename);

	void	IllusionTempleRun();	

	// 성물을 집었을때 index와 인벤토리의 위치를 저장
	void	SetInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos);
	void	SendRelicsUser(LPOBJECTSTRUCT lpObj);	// 성물을 집은사람을 사용자들에게 전송

	void	SeStatusInfo(BYTE btMapNumber, int nStatusIndex, short wStatusPosNum);		// 석상의 기본정보 저장

	BOOL	EnterIllusionTemple( int aIndex, BYTE btTempleIndex, BYTE btItemPos );		// 이벤트맵 입장
	BOOL	GEReqIllusionTempleEnterCountCheck(int aIndex, BYTE btTempleIndex, BYTE btItemPos, int nItemLevel);	// 입장 횟수 검사 요청
	void	EGAnsIllusionTempleEnterCountCheck(LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK aRecv);				// 입장 횟수 검사 응답
	void	GEReqIllusionTempleIncEnterCount(LPOBJECTSTRUCT lpObj);												// 입장 횟수 증가 요청
	BOOL	LeaveIllusionTemple( int aIndex, BYTE btMapNumber);							// 이벤트 맵에서 나감
	
	BOOL	CheckEnterLevel( int nIndex, int nItemLevel);								// 입장할수 있는 레벨 체크
	
	void	IllusionHolyRelicsStatusAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);	// 석상 행동
	void	HolyRelicsBoxAlliedAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 성물 보관함행동(연합군)
	void	HolyRelicsBoxIllusionAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);		// 성물 보관함행동(환영교)
	void	PlatformMirageAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);								// 미라쥬 행동

	void	DeleteRelicsItem(BYTE btMapNumber, LPOBJECTSTRUCT lpObj);			// 성물을 지움
	void	DropRelicsItem(BYTE btMapNumber, int nIndex);						// 성물을 드랍

	void	SetIllusionTempleState(BYTE btState, BYTE btMapNumber);				// 상태를 변경
	BYTE	GetIllusionTempleState(BYTE btMapNumber);
	
	void	AddNpcPosNum(BYTE btMapNumber, int nClass, int nPosNum);			// 환영 사원 NPC의 위치테이블 번호를 저장
	void	AddMonsterPosNum(BYTE btMapNumber, int nClass, int nPosNum);		// 환영 사원 Monster의 위치테이블 번호를 저장
	void	ResetPosNum();														// NPC, 몬스터의 위치정보를 리셋
	
	BYTE	GetUserTeam(BYTE btMapNumber, int nIndex);

	void	SetStatusRegenTime(BYTE btMapNumber);

	void	IllusionTempleChaosMixItem(LPOBJECTSTRUCT lpObj);					// 피의 두루마리 아이템 조합

	BYTE	IncSkillPoint(int nIndex, BYTE btMapNumber, BYTE btObjectType);		// 스킬 포인트 증가
	BYTE	DecSkillPoint(int nIndex, BYTE btMapNumber, BYTE btDecPoint);		// 스킬 포인트 감소

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
	void	IllusionTempleUseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis);
#else
	void	IllusionTempleUseSkill(int iIndex, BYTE MagicNumber, int wTargetObjIndex, BYTE btDis);
#endif
	void	EventSkillProc(LPOBJECTSTRUCT lpObj);								// 스킬 사용시 처리

	void	IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);						// 사용자가 죽었을 때의 처리
	void	IllusionTempleUserDieRegen(LPOBJECTSTRUCT lpObj);					// 사용자가 죽었다 다시 살아날때의 처리

	void	IncUseSkillCount(int nIndex, BYTE btMapNumber);						// 사용자의 스킬 사용 횟수 처리

	void	SetMirageIndex(int nIndex);											// 미라쥬 인덱스를 저장
	void	SendIllusionTempleUserCount();										// 일정 시간 마다 사용자수 전송

	BOOL	CheckWearingMOPH(int nUserIndex);									// 해당 사용자가 변신반지를 착용하고 있는지 확인한다.
	BOOL	CheckChangeRing(int nType);											// 해당 아이템이 변신잔지인지 검사
	
	BOOL	CheckRelics(int nIndex);											// 캐릭터가 성물을 가지고 있는지 검사

	void	MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);							// 몬스터가 죽었을 경우 아이템 드랍

	void	ReqEventReward(int nIndex);											// 승리 보상 요청

	BYTE	GetRemainTime();													// 제1환영사원의 남은시간 리턴

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
	BOOL	CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber);				// 보호의주문 사용중인지 검사
	BOOL	CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber);				// 속박의주문이 걸린상태인지 검사
#endif

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int		CheckCanEnterLevel(int aIndex);
	int		CheckCharmCount(int aIndex);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	
#ifdef ADD_SEASON_3_NEW_UI_20071122	// 환영사원 입장횟수 요청
	// [0x14] 환영사원의 남은 하루 입장횟수를 요청한다.
	void	EGReqIllusionTempleEnterCount(int iIndex);
	// [0x14] 환영사원의 남은 하루 입장횟수를 받는다.
	void	EGAnsIllusionTempleEnterCount(LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT lpMsg);
#endif	// ADD_SEASON_3_NEW_UI_20071122
	
private:
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	bool	IllusionTempleChaosMix(int nIndex, int nLEVEL, int iCharmOfLuckCount);						// 실질적인 피의 두루마리 조합
#else
	bool	IllusionTempleChaosMix(int nIndex, int nLEVEL);						// 실질적인 피의 두루마리 조합
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int		CheckChaosMixItem(int nIndex);										// 조합을 할수 있는지 확인
};

extern CIllusionTempleEvent g_IllusionTempleEvent;

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#endif // !defined(AFX_ILLUSIONTEMPLEEVENT_H__BFE60AE1_BC06_4FA8_8A19_3E391ABD4560__INCLUDED_)
