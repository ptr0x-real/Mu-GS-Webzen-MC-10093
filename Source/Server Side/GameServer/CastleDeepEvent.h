// CastleDeepEvent.h: interface for the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_)
#define AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push)

#pragma warning (disable : 4786)

#include <map>
#include <vector>

using namespace std;

#ifdef CASTLEDEEP_EVENT_20050413


#define	CASTLEDEEP_NOTIFYTIME			5
#define	CASTLEDEEP_MAXSPAWNCHECKCOUNT	100				// �η����� ���� �̺�Ʈ ���� �� ������ ���� �� �ִ� Ƚ��
#define	CASTLEDEEP_ENDTICKCOUNT			(1000*60*10)
#define	CASTLEDEEP_DELTICKCOUNT			(1000*60*10)

enum {
	CASTLEDEEPEVENT_STATE_NONE			= 0,
	CASTLEDEEPEVENT_STATE_CLOSED		= 1,
	CASTLEDEEPEVENT_STATE_PLAYING		= 2,
};

// �η����� ���� �̺�Ʈ�� �����ϴ� ���� ���� (Ÿ��, ����)
typedef struct {
	INT		m_iMonsterGroup;			// ���� �׷�
	INT		m_iMonsterType;				// ���� Ÿ��
	INT		m_iMonsterNumber;			// ���� ������
	INT		m_iSX;
	INT		m_iSY;
	INT		m_iDX;
	INT		m_iDY;
} CASTLEDEEP_MONSTERINFO, * LPCASTLEDEEP_MONSTERINFO;

// �η����� ���� �̺�Ʈ�� ����Ǳ� ���� �������� ����
typedef struct {
	INT		m_iHour;					// �̺�Ʈ�� ������ �ð�
	INT		m_iMinute;					// �̺�Ʈ�� ������ ��
} CASTLEDEEP_EVENTTIME, * LPCASTLEDEEP_EVENTTIME;

// �η����� ���� �̺�Ʈ�� Ÿ�Կ� ���� �� �׷��� ������ �ð��� ����
typedef struct {
	INT		m_iMonsterGroup;			// ���� �׷�
	INT		m_iMonsterSpawnMinute;		// ���Ͱ� �����ϴ� ��
} CASTLEDEEP_SPAWNTIME, * LPCASTLEDEEP_SPAWNTIME;

typedef map<int, vector<CASTLEDEEP_MONSTERINFO> >				TP_CASTLEDEEP_MONSTERINFO;
typedef map<int, vector<CASTLEDEEP_SPAWNTIME> >					TP_CASTLEDEEP_SPAWNTIME;
typedef vector<CASTLEDEEP_EVENTTIME>							TP_CASTLEDEEP_EVENTTIME;
typedef vector<int>												TP_CASTLEDEEP_ASSULTTYPE;
typedef map<int,int>											TP_CASTLEDEEP_RUNCHECK;

typedef map<int, vector<CASTLEDEEP_MONSTERINFO> >::iterator		TP_CASTLEDEEP_MONSTERINFO_ITR;
typedef map<int, vector<CASTLEDEEP_SPAWNTIME> >::iterator		TP_CASTLEDEEP_SPAWNTIME_ITR;
typedef vector<CASTLEDEEP_EVENTTIME>::iterator					TP_CASTLEDEEP_EVENTTIME_ITR;
typedef vector<int>::iterator									TP_CASTLEDEEP_ASSULTTYPE_ITR;
typedef map<int,int>::iterator									TP_CASTLEDEEP_RUNCHECK_ITR;


class CCastleDeepEvent  
{
	BOOL	m_bDoEvent;									// �̺�Ʈ�� ���� ������ ����
	BOOL	m_bHasData;									// ���� �����Ͱ� �ε�Ǿ��°�?

	BOOL	m_bCheckEnd;								// �̺�Ʈ ���Ḧ �� �� �ִ��� üũ (������ ���� ���� �� N�� �� �޽����� ��������)
	DWORD	m_uCheckEndTick;							// ���� Ȯ���� ���� ƽī��Ʈ

	INT		m_iTIME_MSEC_REMAIN;						// ���� ���·� ���� ������ ���� ��
	INT		m_iTIME_MIN_PLAY;							// �̺�Ʈ�� ������ �ð�
	INT		m_iTIME_TICK_COUNT;							// ������ ƽī��Ʈ ����
	INT		m_iTIME_NOTIFY_COUNT;						// ���� ī��Ʈ
	
	INT		m_iEVENT_STATE;								// ������ �̺�Ʈ ����

	INT		m_iCUR_ASSULT_TYPE;							// ������ ����Ÿ��
	INT		m_iCUR_SPAWN_STEP;							// ������ �׷캰 ���� ���� ����

	TP_CASTLEDEEP_MONSTERINFO		m_mapMonsterInfo;	// �� ����Ÿ��, �׷쿡 ���� �������� ����Ʈ
	TP_CASTLEDEEP_SPAWNTIME			m_mapSpawnTime;		// �� ����Ÿ��, �׷쿡 ���� ���� �����ð� ����Ʈ
	TP_CASTLEDEEP_EVENTTIME			m_vtEventTime;		// ���� �̺�Ʈ�� ������ �ð� ����Ʈ
	TP_CASTLEDEEP_ASSULTTYPE		m_vtAssultType;		// ����Ÿ��
	TP_CASTLEDEEP_ASSULTTYPE		m_vtAssultTypeRate;	// ����Ÿ���� ����Ȯ�� (1/100)

	TP_CASTLEDEEP_RUNCHECK			m_mapRunCheck;		// �̺�Ʈ ���� �߿� üũ�ϰ� �Ǵ� �κ� (�׷�/�ð�)
	
	CRITICAL_SECTION				m_critEventData;	// �̺�Ʈ ������ ������ ��ȣ�ϴ� �Ӱ豸��

public:
	CCastleDeepEvent			();
	virtual ~CCastleDeepEvent	();

	VOID	StartEvent			();						// �̺�Ʈ�� ��� ������
	VOID	StopEvent			();						// �̺�Ʈ�� ��� ������
	VOID	EnableEvent			(BOOL bDoEvent);		// �̺�Ʈ�� Ȱ��ȭ ���� ����
	BOOL	Load				(LPSTR lpszFileName);	// �����̺�Ʈ�� �Ӽ������� �ε�

	// ������ ���� ���� �Լ�
	VOID	Run					();
	VOID	CheckSync			();						// Ư�� �ٸ��� ��ũ�� �����.
	VOID	SetState			(INT iSTATE);			// Ư�� �ٸ��� �����췯�� Ư�� ���·� �����Ѵ�.

	VOID	ProcState_None		();
	VOID	ProcState_Closed	();
	VOID	ProcState_Playing	();

	VOID	SetState_None		();
	VOID	SetState_Closed		();
	VOID	SetState_Playing	();

	// �̺�Ʈ ���� ���� �Լ�
	VOID	ClearMonster		();						// �̺�Ʈ ���͵��� ����
	VOID	AddMonster			(						// Ư�� ����Ÿ��, �׷��� ���͵��� �߰��Ѵ�.
		INT iAssultType, 
		INT iGroup
		);
};


extern CCastleDeepEvent	g_CastleDeepEvent;

#pragma warning(pop)

#endif

#endif // !defined(AFX_CASTLEDEEPEVENT_H__C1287797_9DCE_4E63_B1A6_D426FED40097__INCLUDED_)
