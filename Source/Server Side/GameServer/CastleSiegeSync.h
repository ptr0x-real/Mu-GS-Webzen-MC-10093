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

#define	MAX_CS_SYNC_MARKS_OVERLAP			255					// ������ ǥ���� ��ĥ �� �ִ� �ִ� ����
#define MAX_CS_SYNC_MONEY_SEC				(3*60)				// ���ʸ��� �� ������ ����ȭ �� ������ üũ


// ������ ����
enum {
	CASTLESIEGE_SYNC_STATE_NONE				= -1,				// ���¾���
	CASTLESIEGE_SYNC_STATE_IDLE_1			= 0,				// ���ޱⰣ - 1
	CASTLESIEGE_SYNC_STATE_REGSIEGE			= 1,				// ��û�Ⱓ (�����ֱ� ����)
	CASTLESIEGE_SYNC_STATE_IDLE_2			= 2,				// ���ޱⰣ - 1
	CASTLESIEGE_SYNC_STATE_REGMARK			= 3,				// ���� ��ϱⰣ
	CASTLESIEGE_SYNC_STATE_IDLE_3			= 4,				// ���ޱⰣ - 2
	CASTLESIEGE_SYNC_STATE_NOTIFY			= 5,				// ��ǥ�Ⱓ
	CASTLESIEGE_SYNC_STATE_READYSIEGE		= 6,				// �غ�Ⱓ
	CASTLESIEGE_SYNC_STATE_STARTSIEGE		= 7,				// ��������
	CASTLESIEGE_SYNC_STATE_ENDSIEGE			= 8,				// �����Ⱓ
	CASTLESIEGE_SYNC_STATE_ENDCYCLE			= 9,				// �����ֱ� ����
};


class CCastleSiegeSync  
{
	INT		m_iCurCastleState;				// ������ ������ ���°��� ����
	INT		m_iCurTaxRateChaos;				// ������ ������(ī��������)�� ����
	INT		m_iCurTaxRateStore;				// ������ ������(����)�� ����
	INT		m_iCurTaxHuntZone;				// ������ ������(����� �����)�� ����
	LONG	m_lCastleTributeMoney;			// ���� ������ ��
	CHAR	m_szCastleOwnerGuild[16];		// ���� ���� ������ ���(����)�� �̸�

	INT		m_iCsTributeMoneyTimer;			// ���� ������ ������ �ð��� üũ�ϱ� ���� Ÿ�̸�

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

	VOID	AdjustTributeMoney	();								// ������� ���� �������� DB�� ����
	
	BOOL	CheckCastleOwnerMember			(INT iIndex);		// �ش� ����ڰ� ���� ������ ����� �Ҽ����� Ȯ���Ѵ�.
	BOOL	CheckCastleOwnerUnionMember		(					// Ư�� ����ڰ� �ڽ��� ���� ��忬���� �Ͽ����� Ȯ���Ѵ�.
			INT iIndex
			);
	LPSTR	GetCastleOwnerGuild	()			{	return m_szCastleOwnerGuild;		}

	INT		CheckOverlapCsMarks	(INT iIndex);					// Ư�� ������� �κ��丮�� ������ ǥ���� ��ĥ �� �ִٸ� ���° ĭ���� �˷��ش�.
};


extern CCastleSiegeSync	g_CastleSiegeSync;


#endif


#endif // !defined(AFX_CASTLESIEGESYNC_H__93EF915B_CDEF_420E_8637_6C03F640ECDA__INCLUDED_)
