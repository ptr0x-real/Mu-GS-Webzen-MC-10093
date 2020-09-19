// RingAttackEvent.h: interface for the CRingAttackEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RINGATTACKEVENT_H__69FC56DD_46A9_4312_8B94_F1CA63A498E3__INCLUDED_)
#define AFX_RINGATTACKEVENT_H__69FC56DD_46A9_4312_8B94_F1CA63A498E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MONSTER_HERD_SYSTEM_20031120

#pragma warning (disable : 4786)

#include <VECTOR>

using namespace std;

#include "MonsterHerd.h"
#include "DSProtocol.h"


enum {
	RINGEVENT_MAP_NUM_ROLANCIA	= 0,	// 로랜시아
	RINGEVENT_MAP_NUM_DEVIAS	= 2,	// 데비아스
	RINGEVENT_MAP_NUM_NORIA		= 3,	// 노리아
};

static INT	g_RingEventMapNum[3] = {	// 인덱스별 맵 번호
	RINGEVENT_MAP_NUM_ROLANCIA,
	RINGEVENT_MAP_NUM_DEVIAS,
	RINGEVENT_MAP_NUM_NORIA
};

static INT	g_RingMapPosition[3][4] = {	// 맵 번호 별 무리가 랜덤하게 등장할 최소X, 최소Y, 최대X, 최대Y 좌표
	66,		44,		191,	197,		// 로랜시아
	111,	14,		232,	125,		// 데비아스
	136,	53,		223,	181,		// 노리아
//	22,		31,		210,	219,		// 로랜시아
//	50,		29,		231,	198,		// 데비아스
//	22,		31,		202,	218,		// 노리아
};

static INT	g_RingMapDestPosition[3][8] = {		// 맵 번호 별 무리가 최종적으로 모일 X, Y 좌표
	133,	79,							// 로랜시아 1
	87,		126,						// 로랜시아 2
	133,	178,						// 로랜시아 3
	180,	126,						// 로랜시아 4
	164,	42,							// 데비아스 1
	221,	85,							// 데비아스 2
	164,	42,							// 데비아스 3
	221,	85,							// 데비아스 4
	160,	75,							// 노리아 1
	160,	75,							// 노리아 2
	152,	117,						// 노리아 3
	209,	133,						// 노리아 4
};

enum {
	RINGEVENT_STATE_NONE		=0,
	RINGEVENT_STATE_CLOSED		=1,
	RINGEVENT_STATE_PLAYING		=2,
};


#define RINGEVENT_MAX_MAP		3		// 반지이벤트가 열리는 최대 맵 수
#define RINGEVENT_MIN_RADIUS	3		// 무리의 최소 반지름
#define RINGEVENT_MAX_RADIUS	6		// 무리의 최대 반지름
#define RINGEVENT_MAX_MOVE		15		// 무리가 한번에 최대로 움직일 수 있는 X, Y 크기

typedef struct {						// 몬스터 정보를 로드하여 가지고 있을 구조체
	INT		m_iType;					// 몬스터 번호
	BOOL	m_bDoRegen;					// 리젠 여부
	BOOL	m_bDoAttackFirst;			// 선공 여부
} RINGMONSTER_DATA, * LPRINGMONSTER_DATA;


typedef struct {
	INT		m_iHour;					// 이벤트가 시작할 시간
	INT		m_iMinute;					// 이벤트가 시작할 분
} RINGMONSTER_EVENT_TIME, * LPRINGMONSTER_EVENT_TIME;


/*==========================================================
	클래스 설명 : MonsterHerd 클래스를 상속받은 반지의 제왕 
		몬스터 무리 클래스
==========================================================*/
class CRingMonsterHerd : public MonsterHerd
{
private:
	BOOL	m_bEventSuccess;

public:
	CRingMonsterHerd();
	virtual ~CRingMonsterHerd();

	virtual BOOL	Start ();
	virtual BOOL	MonsterHerdItemDrop(
		LPOBJECTSTRUCT lpObj
		);
	virtual VOID	MonsterAttackAction(	// 무리 몬스터 중 특정 몬스터가 공격했을 때의 처리
		LPOBJECTSTRUCT lpObj,				// 나
		LPOBJECTSTRUCT lpTargetObj			// 내가 공격한 인간?
		);

	VOID			SendEventGiftWinner (	// 이벤트 서버에 이벤트 당첨자를 보낸다.
		INT iIndex,
		INT iGiftKind						// 상품의 종류 (0:용사의반지드롭, 1:오크죽임)
		);
	UCHAR			GetX() {				// 현재의 중심 X
		return m_iCUR_X;
	}
	UCHAR			GetY() {				// 현재의 중심 Y
		return m_iCUR_Y;
	}
};


/*==========================================================
	클래스 설명 : 위의 CRingMonsterHerd 객체를 각 맵 (로랜시아, 
		노리아, 데비아스) 별로 전체적인 이벤트 관리를 하는 클래스
==========================================================*/
class CRingAttackEvent
{
private:
	CRingMonsterHerd	m_RingMonsterHerd [RINGEVENT_MAX_MAP];		// 세 맵에 각각 활동할 몬스터 무리 객체

	BOOL	m_bDoEvent;								// 이벤트를 할지 않할지 여부
	BOOL	m_bHasData;								// 현재 데이터가 로드되었는가?

	BOOL	m_bMonsterToDest;						// 몬스터들이 목적지로 향하는가 ?

	INT		m_iTIME_MSEC_REMAIN;					// 다음 상태로 가지 전까지 남는 초
	INT		m_iTIME_MIN_OPEN;						// 이벤트가 시작하기 전 공지를 시작할 분
	INT		m_iTIME_MIN_PLAY;						// 이벤트를 진행할 시간
	INT		m_iTIME_MONSTER_TO_DEST;				// 몬스터들이 목적지로 종료 몇분 전에 향할 것인가?
	INT		m_iTIME_TICK_COUNT;						// 현재의 틱카운트 저장
	INT		m_iTIME_NOTIFY_COUNT;					// 공지 카운트

	INT		m_iEVENT_STATE;							// 현재의 이벤트 상태

	INT		m_iMOVE_RAND_SIZE;						// 한번 움직일 때의 최대 크기
	INT		m_iRADIUS_MIN;							// 한번 움직일 때의 최소 반경
	INT		m_iRADIUS_MAX;							// 한번 움직일 때의 최대 반경

	vector<RINGMONSTER_DATA> m_vtMonsterAddData;	// 몬스터 무리에 추가할 몬스터들의 속성정보를 가지고 저장
	CRITICAL_SECTION m_critMonsterAddData;			// 몬스터 무리에 추가할 몬스터들의 속성정보 벡터를 보호하는 임계구역

	vector<RINGMONSTER_EVENT_TIME> m_vtEventTime;	// 몬스터 이벤트가 시작할 시간을 가지고 있음

#ifdef EVENT_MANAGEMENT_20040711
	BOOL	m_bMenualStart;
#endif

public:
	CRingAttackEvent();
	virtual ~CRingAttackEvent();

	VOID	StartEvent			();					// 이벤트를 즉시 시작함
	VOID	StopEvent			();					// 이벤트를 즉시 중지함
	VOID	EnableEvent (BOOL bDoEvent);			// 이벤트를 활성화 할지 여부
	virtual BOOL	Load (LPSTR lpszFileName);		// 반지이벤트의 속성정보를 로드

	// 스케쥴 관리 관련 함수
	VOID	Run					();
	VOID	CheckSync			();					// 특정 다리의 싱크를 맞춘다.
	VOID	SetState			(INT iSTATE);		// 특정 다리의 스케쥴러를 특정 상태로 변경한다.

	VOID	ProcState_None		();
	VOID	ProcState_Closed	();
	VOID	ProcState_Playing	();

	VOID	SetState_None		();
	VOID	SetState_Closed		();
	VOID	SetState_Playing	();

	VOID	Move ();

#ifdef EVENT_MANAGEMENT_20040711
	void	SetMenualStart		(BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	
	void	Start_Menual		();
	void	End_Menual			();
#endif

	void _SendDebugMsg(char *szDebugMsg, ... );
	
};


#endif

#endif // !defined(AFX_RINGATTACKEVENT_H__69FC56DD_46A9_4312_8B94_F1CA63A498E3__INCLUDED_)

