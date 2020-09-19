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
	RINGEVENT_MAP_NUM_ROLANCIA	= 0,	// �η��þ�
	RINGEVENT_MAP_NUM_DEVIAS	= 2,	// ����ƽ�
	RINGEVENT_MAP_NUM_NORIA		= 3,	// �븮��
};

static INT	g_RingEventMapNum[3] = {	// �ε����� �� ��ȣ
	RINGEVENT_MAP_NUM_ROLANCIA,
	RINGEVENT_MAP_NUM_DEVIAS,
	RINGEVENT_MAP_NUM_NORIA
};

static INT	g_RingMapPosition[3][4] = {	// �� ��ȣ �� ������ �����ϰ� ������ �ּ�X, �ּ�Y, �ִ�X, �ִ�Y ��ǥ
	66,		44,		191,	197,		// �η��þ�
	111,	14,		232,	125,		// ����ƽ�
	136,	53,		223,	181,		// �븮��
//	22,		31,		210,	219,		// �η��þ�
//	50,		29,		231,	198,		// ����ƽ�
//	22,		31,		202,	218,		// �븮��
};

static INT	g_RingMapDestPosition[3][8] = {		// �� ��ȣ �� ������ ���������� ���� X, Y ��ǥ
	133,	79,							// �η��þ� 1
	87,		126,						// �η��þ� 2
	133,	178,						// �η��þ� 3
	180,	126,						// �η��þ� 4
	164,	42,							// ����ƽ� 1
	221,	85,							// ����ƽ� 2
	164,	42,							// ����ƽ� 3
	221,	85,							// ����ƽ� 4
	160,	75,							// �븮�� 1
	160,	75,							// �븮�� 2
	152,	117,						// �븮�� 3
	209,	133,						// �븮�� 4
};

enum {
	RINGEVENT_STATE_NONE		=0,
	RINGEVENT_STATE_CLOSED		=1,
	RINGEVENT_STATE_PLAYING		=2,
};


#define RINGEVENT_MAX_MAP		3		// �����̺�Ʈ�� ������ �ִ� �� ��
#define RINGEVENT_MIN_RADIUS	3		// ������ �ּ� ������
#define RINGEVENT_MAX_RADIUS	6		// ������ �ִ� ������
#define RINGEVENT_MAX_MOVE		15		// ������ �ѹ��� �ִ�� ������ �� �ִ� X, Y ũ��

typedef struct {						// ���� ������ �ε��Ͽ� ������ ���� ����ü
	INT		m_iType;					// ���� ��ȣ
	BOOL	m_bDoRegen;					// ���� ����
	BOOL	m_bDoAttackFirst;			// ���� ����
} RINGMONSTER_DATA, * LPRINGMONSTER_DATA;


typedef struct {
	INT		m_iHour;					// �̺�Ʈ�� ������ �ð�
	INT		m_iMinute;					// �̺�Ʈ�� ������ ��
} RINGMONSTER_EVENT_TIME, * LPRINGMONSTER_EVENT_TIME;


/*==========================================================
	Ŭ���� ���� : MonsterHerd Ŭ������ ��ӹ��� ������ ���� 
		���� ���� Ŭ����
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
	virtual VOID	MonsterAttackAction(	// ���� ���� �� Ư�� ���Ͱ� �������� ���� ó��
		LPOBJECTSTRUCT lpObj,				// ��
		LPOBJECTSTRUCT lpTargetObj			// ���� ������ �ΰ�?
		);

	VOID			SendEventGiftWinner (	// �̺�Ʈ ������ �̺�Ʈ ��÷�ڸ� ������.
		INT iIndex,
		INT iGiftKind						// ��ǰ�� ���� (0:����ǹ������, 1:��ũ����)
		);
	UCHAR			GetX() {				// ������ �߽� X
		return m_iCUR_X;
	}
	UCHAR			GetY() {				// ������ �߽� Y
		return m_iCUR_Y;
	}
};


/*==========================================================
	Ŭ���� ���� : ���� CRingMonsterHerd ��ü�� �� �� (�η��þ�, 
		�븮��, ����ƽ�) ���� ��ü���� �̺�Ʈ ������ �ϴ� Ŭ����
==========================================================*/
class CRingAttackEvent
{
private:
	CRingMonsterHerd	m_RingMonsterHerd [RINGEVENT_MAX_MAP];		// �� �ʿ� ���� Ȱ���� ���� ���� ��ü

	BOOL	m_bDoEvent;								// �̺�Ʈ�� ���� ������ ����
	BOOL	m_bHasData;								// ���� �����Ͱ� �ε�Ǿ��°�?

	BOOL	m_bMonsterToDest;						// ���͵��� �������� ���ϴ°� ?

	INT		m_iTIME_MSEC_REMAIN;					// ���� ���·� ���� ������ ���� ��
	INT		m_iTIME_MIN_OPEN;						// �̺�Ʈ�� �����ϱ� �� ������ ������ ��
	INT		m_iTIME_MIN_PLAY;						// �̺�Ʈ�� ������ �ð�
	INT		m_iTIME_MONSTER_TO_DEST;				// ���͵��� �������� ���� ��� ���� ���� ���ΰ�?
	INT		m_iTIME_TICK_COUNT;						// ������ ƽī��Ʈ ����
	INT		m_iTIME_NOTIFY_COUNT;					// ���� ī��Ʈ

	INT		m_iEVENT_STATE;							// ������ �̺�Ʈ ����

	INT		m_iMOVE_RAND_SIZE;						// �ѹ� ������ ���� �ִ� ũ��
	INT		m_iRADIUS_MIN;							// �ѹ� ������ ���� �ּ� �ݰ�
	INT		m_iRADIUS_MAX;							// �ѹ� ������ ���� �ִ� �ݰ�

	vector<RINGMONSTER_DATA> m_vtMonsterAddData;	// ���� ������ �߰��� ���͵��� �Ӽ������� ������ ����
	CRITICAL_SECTION m_critMonsterAddData;			// ���� ������ �߰��� ���͵��� �Ӽ����� ���͸� ��ȣ�ϴ� �Ӱ豸��

	vector<RINGMONSTER_EVENT_TIME> m_vtEventTime;	// ���� �̺�Ʈ�� ������ �ð��� ������ ����

#ifdef EVENT_MANAGEMENT_20040711
	BOOL	m_bMenualStart;
#endif

public:
	CRingAttackEvent();
	virtual ~CRingAttackEvent();

	VOID	StartEvent			();					// �̺�Ʈ�� ��� ������
	VOID	StopEvent			();					// �̺�Ʈ�� ��� ������
	VOID	EnableEvent (BOOL bDoEvent);			// �̺�Ʈ�� Ȱ��ȭ ���� ����
	virtual BOOL	Load (LPSTR lpszFileName);		// �����̺�Ʈ�� �Ӽ������� �ε�

	// ������ ���� ���� �Լ�
	VOID	Run					();
	VOID	CheckSync			();					// Ư�� �ٸ��� ��ũ�� �����.
	VOID	SetState			(INT iSTATE);		// Ư�� �ٸ��� �����췯�� Ư�� ���·� �����Ѵ�.

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

