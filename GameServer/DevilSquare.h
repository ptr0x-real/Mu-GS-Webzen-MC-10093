// DevilSquare.h: interface for the CDevilSquare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVILSQUARE_H__3000047C_3C75_4954_AA6F_D5671B130A52__INCLUDED_)
#define AFX_DEVILSQUARE_H__3000047C_3C75_4954_AA6F_D5671B130A52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		DEVILSQUARE_SEC		1000
#define		DEVILSQUARE_MIN		1000*60

#include "user.h"

#include "DevilSquareGround.h"

#ifdef DEVILSQUARE_EXTEND_20050221

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
// Ư�� �����ȣ�� ���� �ʹ�ȣ�� �˷���
#define		GET_DEVILSQUARE_MAPNUMBER(C_SQRNUM)		(((C_SQRNUM>=0)&&(C_SQRNUM<=3))?MAP_INDEX_DEVILSQUARE:((C_SQRNUM>=4)&&(C_SQRNUM<MAX_DEVILSQUARE))?MAP_INDEX_DEVILSQUARE2:-1)
// Ư�� �ʹ�ȣ�� �������������� Ȯ������
#define		CHECK_DEVILSQUARE(C_DVATTR)				(C_DVATTR==MAP_INDEX_DEVILSQUARE?TRUE:C_DVATTR==MAP_INDEX_DEVILSQUARE2?TRUE:FALSE)
#define		CHECK_DEVILSQUARE_INDEX(C_DVIDX)		(C_DVIDX<0?FALSE:C_DVIDX>(MAX_DEVILSQUARE-1)?FALSE:TRUE)

#else
// Ư�� �����ȣ�� ���� �ʹ�ȣ�� �˷���
#define		GET_DEVILSQUARE_MAPNUMBER(C_SQRNUM)		(((C_SQRNUM>=0)&&(C_SQRNUM<=3))?MAP_INDEX_DEVILSQUARE:((C_SQRNUM>=4)&&(C_SQRNUM<=5))?MAP_INDEX_DEVILSQUARE2:-1)
// Ư�� �ʹ�ȣ�� �������������� Ȯ������
#define		CHECK_DEVILSQUARE(C_DVATTR)				(C_DVATTR==MAP_INDEX_DEVILSQUARE?TRUE:C_DVATTR==MAP_INDEX_DEVILSQUARE2?TRUE:FALSE)
#define		CHECK_DEVILSQUARE_INDEX(C_DVIDX)		(C_DVIDX<0?FALSE:C_DVIDX>5?FALSE:TRUE)
#endif	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010

/*===================================================
//	<< ���������� ���� ���� ����ü >>
// : ������������ �� ���忡 ���� ���� ���ѿ� ���� ������ ������ �ִ�.
===================================================*/
typedef struct _DEVILSQUARE_LEVEL {
	INT iLOWER_BOUND;					// ���Ѽ�
	INT iUPPER_BOUND;					// ���Ѽ�
	INT iLOWER_BOUND_EXCHAR;			// ���Ѽ� - ���˻�, ��ũ�ε�
	INT iUPPER_BOUND_EXCHAR;			// ���Ѽ� - ���˻�, ��ũ�ε�
	INT	iGATE_NUM;						// �̵��� ����Ʈ ��ȣ
} DEVILSQUARE_LEVEL, * LPDEVILSQUARE_LEVEL;

static DEVILSQUARE_LEVEL g_sttDEVILSQUARE_LEVEL[MAX_DEVILSQUARE] = {
	15,		130,		10,		110,			58,
	131,	180,		111,	160,			59,
	181,	230,		161,	210,			60,
	231,	280,		211,	260,			61,
	281,	330,		261,	310,			111,
	331,	MAX_LEVEL,	311,	MAX_LEVEL,		112,
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 7���� ���巹�� �߰�
	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL), MAX_LEVEL, (MAX_LEVEL+MAX_MASTER_LEVEL),	270,
#endif
	};
#endif

enum eDevilSquareState
{
	DevilSquare_CLOSE,		// ������
	DevilSquare_OPEN,		// ��� ���� �ִ�
	DevilSquare_PLAYING		// ������
};

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511
#include <list>
typedef struct __DEVIL_SQARE_STARTTIME
{
	int nHour;
	int nMin;
} DEVIL_SQARE_STARTTIME;
#endif

class CDevilSquare  
{
	eDevilSquareState		m_eState;	

	DWORD					m_iTime;
	int						m_iRemainTime;		// ���� ���� �ð�
	int						m_iremainTimeSec;	// ���� �ð� ��(ī��Ʈ�� ���ؼ�)

	BOOL					m_bSendTimeCount;

	BOOL					m_bQuit;

	BOOL					m_bFlag;

	UINT					m_hThread;

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511
	list<DEVIL_SQARE_STARTTIME>	m_lstStartTime;	// ���۽ð� ���� ����Ʈ
#endif

public:
	int						m_iCloseTime;		// ������ ���� �ִ� �ð�(�д���)
	int						m_iOpenTime;		// ���忡 ��� ���� �ִ� �ð�(�д���)
	int						m_iPlaytime;		// ���忡�� �����ϴ� �ð�

	CDevilSquareGround		m_DevilSquareGround[MAX_DEVILSQUARE];	
#ifdef DEVILSQUARE_EXTEND_20050221				// ��ũ�ε� ���ʽ� ���̺� �߰� + ���尳�� ��ũ��
	int						m_BonusScoreTable[5][MAX_DEVILSQUARE];
#else
	int						m_BonusScoreTable[4][4];
#endif
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	FLOAT					m_fRewardExpRate[MAX_DEVILSQUARE];
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

public:
	CDevilSquare();
	virtual ~CDevilSquare();
	
	void		Init();

	// ������ ���� ����
	void					SetState(eDevilSquareState eState);	
	eDevilSquareState		GetState(){return m_eState;}


	// �Ǹ��� ���� ����
	BOOL		Run();

	// ���� �ð� ��´�
	int			GetRemainTime(){return m_iRemainTime;}

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511
	int			GetNextRemainTime();	// �������� �� ���� ���������� �ð�
#endif

	// ���� ���� �� ����
	void		SetMonster();
	void		ClearMonstr();

	// ���忡���� ���� ���� ó��
	void		gDevilSquareMonsterRegen(LPOBJECTSTRUCT lpObj);
	

	// �Ǹ��� ���忡�� �׾����� ó���� ���ش�
	void		DieProcDevilSquare(LPOBJECTSTRUCT lpObj);

	// ���忡���� ����ġ ó��
	int			gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg);	

	void		gObjExpParty(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int AttackDamage, int MSBFlag);

	// �̺�Ʈ ���� �����й�
	void		gObjMonsterScoreDivision(LPOBJECTSTRUCT lpMonObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int MSBFlag);

	// ��ũ��Ʈ �ε�
	void		Load(char* filename);
	// ���� ��� 
	void		CalcScore();

	void		SetClose();

#ifdef DEVILSQUARE_EXTEND_20050221				
	INT			GetDevilSquareIndex	(INT iGateNumber);
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
	INT			GetUserLevelToEnter (INT iUserIndex, INT & btMoveGate);
#else
	INT			GetUserLevelToEnter (INT iUserIndex, BYTE & btMoveGate);
#endif
#endif

#ifdef MODIFY_SEASON25_PCS_UNIFICATION_20070903
	#if TESTSERVER == 1
	void		SetRemainTime(int iRemainTime);
	#endif
#endif // MODIFY_SEASON25_PCS_UNIFICATION_20070903
	
private:
	void		SetOpen();
	void		SetPlaying();

	void		ProcClose();
	void		ProcOpen();
	void		ProcPlaying();	
	
	// ���� �̺�Ʈ �����Ѵٰ� �˸�
	void		SendEventStartMsg();
	// �Ǹ��� ���� ������ ����
	void		ItemClear();

	void		CheckSync();
	void		SendLiveCall();

	// ������ �ʱ�ȭ �Ѵ�
	void		gObjScoreClear();

	// ���������� ������ �ִ��� Ȯ���Ѵ�
	void		CheckInvalidUser();
};

extern CDevilSquare	g_DevilSquare;

extern void DevilSquareEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);
extern void	DataSendRank(char* pMsg, int size);

#endif // !defined(AFX_DEVILSQUARE_H__3000047C_3C75_4954_AA6F_D5671B130A52__INCLUDED_)
