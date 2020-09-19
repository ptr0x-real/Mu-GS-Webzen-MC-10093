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
// 특정 광장번호에 따른 맵번호를 알려줌
#define		GET_DEVILSQUARE_MAPNUMBER(C_SQRNUM)		(((C_SQRNUM>=0)&&(C_SQRNUM<=3))?MAP_INDEX_DEVILSQUARE:((C_SQRNUM>=4)&&(C_SQRNUM<MAX_DEVILSQUARE))?MAP_INDEX_DEVILSQUARE2:-1)
// 특정 맵번호가 데빌스퀘어인지 확인해줌
#define		CHECK_DEVILSQUARE(C_DVATTR)				(C_DVATTR==MAP_INDEX_DEVILSQUARE?TRUE:C_DVATTR==MAP_INDEX_DEVILSQUARE2?TRUE:FALSE)
#define		CHECK_DEVILSQUARE_INDEX(C_DVIDX)		(C_DVIDX<0?FALSE:C_DVIDX>(MAX_DEVILSQUARE-1)?FALSE:TRUE)

#else
// 특정 광장번호에 따른 맵번호를 알려줌
#define		GET_DEVILSQUARE_MAPNUMBER(C_SQRNUM)		(((C_SQRNUM>=0)&&(C_SQRNUM<=3))?MAP_INDEX_DEVILSQUARE:((C_SQRNUM>=4)&&(C_SQRNUM<=5))?MAP_INDEX_DEVILSQUARE2:-1)
// 특정 맵번호가 데빌스퀘어인지 확인해줌
#define		CHECK_DEVILSQUARE(C_DVATTR)				(C_DVATTR==MAP_INDEX_DEVILSQUARE?TRUE:C_DVATTR==MAP_INDEX_DEVILSQUARE2?TRUE:FALSE)
#define		CHECK_DEVILSQUARE_INDEX(C_DVIDX)		(C_DVIDX<0?FALSE:C_DVIDX>5?FALSE:TRUE)
#endif	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010

/*===================================================
//	<< 데빌스퀘어 레벨 제한 구조체 >>
// : 데빌스퀘어의 각 광장에 대한 레벨 제한에 대한 정보를 가지고 있다.
===================================================*/
typedef struct _DEVILSQUARE_LEVEL {
	INT iLOWER_BOUND;					// 하한선
	INT iUPPER_BOUND;					// 상한선
	INT iLOWER_BOUND_EXCHAR;			// 하한선 - 마검사, 다크로드
	INT iUPPER_BOUND_EXCHAR;			// 상한선 - 마검사, 다크로드
	INT	iGATE_NUM;						// 이동할 게이트 번호
} DEVILSQUARE_LEVEL, * LPDEVILSQUARE_LEVEL;

static DEVILSQUARE_LEVEL g_sttDEVILSQUARE_LEVEL[MAX_DEVILSQUARE] = {
	15,		130,		10,		110,			58,
	131,	180,		111,	160,			59,
	181,	230,		161,	210,			60,
	231,	280,		211,	260,			61,
	281,	330,		261,	310,			111,
	331,	MAX_LEVEL,	311,	MAX_LEVEL,		112,
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 7광장 입장레벨 추가
	MAX_LEVEL,	(MAX_LEVEL+MAX_MASTER_LEVEL), MAX_LEVEL, (MAX_LEVEL+MAX_MASTER_LEVEL),	270,
#endif
	};
#endif

enum eDevilSquareState
{
	DevilSquare_CLOSE,		// 닫혔다
	DevilSquare_OPEN,		// 들어 갈수 있다
	DevilSquare_PLAYING		// 끝났다
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
	int						m_iRemainTime;		// 현재 남은 시간
	int						m_iremainTimeSec;	// 남은 시간 초(카운트를 위해서)

	BOOL					m_bSendTimeCount;

	BOOL					m_bQuit;

	BOOL					m_bFlag;

	UINT					m_hThread;

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511
	list<DEVIL_SQARE_STARTTIME>	m_lstStartTime;	// 시작시간 정보 리스트
#endif

public:
	int						m_iCloseTime;		// 광장이 닫혀 있는 시간(분단위)
	int						m_iOpenTime;		// 광장에 들어 갈수 있는 시간(분단위)
	int						m_iPlaytime;		// 광장에서 게임하는 시간

	CDevilSquareGround		m_DevilSquareGround[MAX_DEVILSQUARE];	
#ifdef DEVILSQUARE_EXTEND_20050221				// 다크로드 보너스 테이블 추가 + 광장개수 매크로
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

	// 광장의 현재 상태
	void					SetState(eDevilSquareState eState);	
	eDevilSquareState		GetState(){return m_eState;}


	// 악마의 광장 동작
	BOOL		Run();

	// 남은 시간 얻는다
	int			GetRemainTime(){return m_iRemainTime;}

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511
	int			GetNextRemainTime();	// 진행중일 때 다음 열릴때까지 시간
#endif

	// 몬스터 설정 및 정리
	void		SetMonster();
	void		ClearMonstr();

	// 광장에서의 몬스터 리젠 처리
	void		gDevilSquareMonsterRegen(LPOBJECTSTRUCT lpObj);
	

	// 악마의 광장에서 죽었을때 처리를 해준다
	void		DieProcDevilSquare(LPOBJECTSTRUCT lpObj);

	// 광장에서의 경험치 처리
	int			gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg);	

	void		gObjExpParty(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int AttackDamage, int MSBFlag);

	// 이벤트 몬스터 점수분배
	void		gObjMonsterScoreDivision(LPOBJECTSTRUCT lpMonObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int MSBFlag);

	// 스크립트 로드
	void		Load(char* filename);
	// 점수 계산 
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
	
	// 광장 이벤트 시작한다고 알림
	void		SendEventStartMsg();
	// 악마의 광장 아이템 정리
	void		ItemClear();

	void		CheckSync();
	void		SendLiveCall();

	// 점수를 초기화 한다
	void		gObjScoreClear();

	// 비정상적인 유저가 있는지 확인한다
	void		CheckInvalidUser();
};

extern CDevilSquare	g_DevilSquare;

extern void DevilSquareEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);
extern void	DataSendRank(char* pMsg, int size);

#endif // !defined(AFX_DEVILSQUARE_H__3000047C_3C75_4954_AA6F_D5671B130A52__INCLUDED_)
