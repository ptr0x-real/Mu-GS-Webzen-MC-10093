// CastleSiegeSync.h: interface for the CCastleSiegeSync class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLESIEGESYNC_H__93EF915B_CDEF_420E_8637_6C03F640ECDA__INCLUDED_)
#define AFX_CASTLESIEGESYNC_H__93EF915B_CDEF_420E_8637_6C03F640ECDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130


#include "CastleSiege.H"

#define	MAX_CS_SYNC_MARKS_OVERLAP			255					// 성주의 표식을 겹칠 수 있는 최대 개수
#define MAX_CS_SYNC_MONEY_SEC				(3*60)				// 몇초마다 성 세금을 동기화 할 것인지 체크


// 공성전 상태
enum {
	CASTLESIEGE_SYNC_STATE_NONE				= -1,				// 상태없음
	CASTLESIEGE_SYNC_STATE_IDLE_1			= 0,				// 유휴기간 - 1
	CASTLESIEGE_SYNC_STATE_REGSIEGE			= 1,				// 신청기간 (공성주기 시작)
	CASTLESIEGE_SYNC_STATE_IDLE_2			= 2,				// 유휴기간 - 1
	CASTLESIEGE_SYNC_STATE_REGMARK			= 3,				// 문장 등록기간
	CASTLESIEGE_SYNC_STATE_IDLE_3			= 4,				// 유휴기간 - 2
	CASTLESIEGE_SYNC_STATE_NOTIFY			= 5,				// 발표기간
	CASTLESIEGE_SYNC_STATE_READYSIEGE		= 6,				// 준비기간
	CASTLESIEGE_SYNC_STATE_STARTSIEGE		= 7,				// 공성시작
	CASTLESIEGE_SYNC_STATE_ENDSIEGE			= 8,				// 휴전기간
	CASTLESIEGE_SYNC_STATE_ENDCYCLE			= 9,				// 공성주기 종료
};


class CCastleSiegeSync  
{
	INT		m_iCurCastleState;				// 현재의 공성전 상태값을 가짐
	INT		m_iCurTaxRateChaos;				// 현재의 세율값(카오스조합)을 가짐
	INT		m_iCurTaxRateStore;				// 현재의 세율값(상점)을 가짐
	INT		m_iCurTaxHuntZone;				// 현재의 세율값(사냥터 입장료)을 가짐
	LONG	m_lCastleTributeMoney;			// 성에 공납할 돈
	CHAR	m_szCastleOwnerGuild[16];		// 현재 성을 차지한 길드(연합)의 이름

	INT		m_iCsTributeMoneyTimer;			// 성에 세금을 공납할 시간을 체크하기 위한 타이머

public:
	CCastleSiegeSync();
	virtual ~CCastleSiegeSync();
	
	VOID	Clear			();

	VOID	SetCastleState	(
			INT iCastleState
			)								{	m_iCurCastleState	= iCastleState;	}
	VOID	SetTaxRateChaos (
			INT iTaxRate
			)								{	m_iCurTaxRateChaos	= iTaxRate;		}
	VOID	SetTaxRateStore (
			INT iTaxRate
			)								{	m_iCurTaxRateStore	= iTaxRate;		}
	VOID	SetTaxHuntZone	(
			INT iTaxRate
			)								{	m_iCurTaxHuntZone	= iTaxRate;		}
	VOID	SetCastleOwnerGuild (
			LPSTR lpszGuildName
			);

	INT		GetCastleState	()				{	return	m_iCurCastleState;			}
	INT		GetTaxRateChaos (INT iIndex);
	INT		GetTaxRateStore (INT iIndex);
	INT		GetTaxHuntZone	(INT iIndex, BOOL bCheckOwnerGuild = TRUE);

	VOID	AddTributeMoney		(INT iMoney);
	VOID	ResetTributeMoney	();
	INT		GetTributeMoney		()			{	return m_lCastleTributeMoney;		}

	VOID	AdjustTributeMoney	();								// 현재까지 모은 공납금을 DB에 적용
	
	BOOL	CheckCastleOwnerMember			(INT iIndex);		// 해당 사용자가 성을 차지한 길드의 소속인지 확인한다.
	BOOL	CheckCastleOwnerUnionMember		(					// 특정 사용자가 자신이 속한 길드연합의 일원인지 확인한다.
			INT iIndex
			);
	LPSTR	GetCastleOwnerGuild	()			{	return m_szCastleOwnerGuild;		}

	INT		CheckOverlapCsMarks	(INT iIndex);					// 특정 사용자의 인벤토리에 성주의 표식을 겹칠 수 있다면 몇번째 칸인지 알려준다.
};


extern CCastleSiegeSync	g_CastleSiegeSync;


#endif


#endif // !defined(AFX_CASTLESIEGESYNC_H__93EF915B_CDEF_420E_8637_6C03F640ECDA__INCLUDED_)
