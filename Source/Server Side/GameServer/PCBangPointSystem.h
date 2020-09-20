// PCBangPointSystem.h: interface for the PCBangPointSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_)
#define AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef PCBANG_POINT_SYSTEM_20070206
// **PC������Ʈ�ý��� ����������� �Ը� Ŀ���ٸ� ������ �� �ɼ�ó���� �и��Ͽ� ó�� �� ��.(RKFKA)


// ��Ģ�� ���߾� �ε����� �ο��Ѵ�.
enum PCBANG_POINT_RULE_TIME_TYPE
{
	PG_RULE_TYPE_FIRST		= 0,	// ���� ��Ģ �ð�		
	PG_RULE_TYPE_COMMON		= 1,	// �Ϲ� ��Ģ �ð�
};

// PC�� ����Ʈ �ý��� ���� ���
enum PCBANG_POINT_APPLICATION_LEVEL
{
	PP_APPLICATION_OF_NONE				= 0,	// �������� �ʴ´�.
	PP_PCBANG_USER_ONLY					= 1,	// PC�� �������� �����Ѵ�.
	PP_APPLICATION_OF_ALL				= 2,	// ��� ������ ���� �����Ѵ�.
												// (�⺻������ PC�� �������� ����Ǵ� �ý��������� Ȯ�强�� ����Ѵ�.)
};

#ifdef PCBANG_KUZIMO

extern short g_nPCBangPointFirstRuleTime;			// ���� ��Ģ �ð� ����(60�� * 10 = 10��) 
extern short g_nPCBangPointFirstRuleTimePoint;		// ���� ��Ģ �ð��� �������� �� �ο� ����Ʈ
extern short g_nPCBangPointCommonRuleTime;			// �Ϲ� ��Ģ �ð� ����(60�� * 10 = 10��) 
extern short g_nPCBangPointCommonRuleTimePoint;	// �Ϲ� ��Ģ �ð��� �������� �� �ο� ����Ʈ
extern short g_nPCBangMaxPoint;					// �ִ� ���� ������ ����Ʈ

// FeverTime �Ⱓ���� (��� �ο� ����Ʈ * g_nPCBangPointFeverMultipleValue)����Ʈ�� �����ȴ�.
// g_nPCBangPointFeverStartHour�� g_nPCBangPointFeverEndHour�� ������ �������� �ʴ´�.
// g_nPCBangPointFeverStartHour = 4, g_nPCBangPointFeverEndHour = 5��� 4�ú��� �۵��ϸ�, 5�ð� �Ǹ� �۵� �����ȴ�.
extern short g_nPCBangPointFeverStartHour;			// �ð��� 0�ú��� 24�ñ��� ���� �� �ִ�. 0��, 24�ô� �����ϴ�.
extern short g_nPCBangPointFeverEndHour;			//
extern short g_nPCBangPointFeverMultipleValue;

extern short g_nPCBangResetDayOfWeek;				// ���� ������ ���Ѵ�. 0(��), 1(��)... 
extern short g_nPCBangResetHour;		// ���� �ð�. ���� 8�ú��� 10�ñ��� �����̴�. ���ϸ� ���� ���ؼ� 9�÷� �����Ѵ�.

#else

#ifdef MODIFY_PCBANG_POINT_TIME_EVENT_20070709
const short g_nPCBangPointFirstRuleTime			= 900;		// ���� ��Ģ �ð� ����	
const short g_nPCBangPointFirstRuleTimePoint	= 3;		// ���� ��Ģ �ð��� �������� �� �ο� ����Ʈ
const short g_nPCBangPointCommonRuleTime		= 300;		// �Ϲ� ��Ģ �ð� ����
const short g_nPCBangPointCommonRuleTimePoint	= 1;		// �Ϲ� ��Ģ �ð��� �������� �� �ο� ����Ʈ
const short g_nPCBangMaxPoint					= 1008;		// �ִ� ���� ������ ����Ʈ
#else
const short g_nPCBangPointFirstRuleTime			= 1800;		// ���� ��Ģ �ð� ����	
const short g_nPCBangPointFirstRuleTimePoint	= 3;		// ���� ��Ģ �ð��� �������� �� �ο� ����Ʈ
const short g_nPCBangPointCommonRuleTime		= 600;		// �Ϲ� ��Ģ �ð� ����
const short g_nPCBangPointCommonRuleTimePoint	= 1;		// �Ϲ� ��Ģ �ð��� �������� �� �ο� ����Ʈ
const short g_nPCBangMaxPoint					= 1008;		// �ִ� ���� ������ ����Ʈ
#endif	// MODIFY_PCBANG_POINT_TIME_EVENT_20070709

#endif

#ifdef ADD_PCBANG_EXPRATE
extern float g_fPCBangAddExperience;
#endif // ADD_PCBANG_EXPRATE



const short MAX_PCBANG_POINT_ITEM_COUNT			= 6;		// PC�� ����Ʈ ������ ��
typedef struct 
{
	//BOOL	bPCBangPointUser;			// PC�� ����Ʈ �ý��� �����ΰ�? 0:�ƴϴ�/1:�׷���
	BOOL	bIsDataLoadSuccess;			// �̺�Ʈ�����κ��� �����͸� ���������� ���޹޾ҳ�? 0:�ƴϴ�/1:�׷���

	BYTE	btCurrentRule;				// ���� �������� ����Ǵ� ��Ģ
	short	nCurrentAppRuleTime;		// �������� ����ǰ� �ִ� ��Ģ �ð�
	short	nCurrentAppRulePoint;		// �������� ������ ����Ʈ

	int		nPCBangPoint;				// ���� ����Ʈ
	BOOL	bIsSuccessFirstRuleTime;	// ���� ��Ģ �ð��� ����߳�? 0:�ƴϴ�/1:�׷���
	
	DWORD	dwCheckRuleTime;			// ���ð�(������ ��Ģ�ð��� ������ �����Ѵ�.)	

#ifdef PCBANG_KUZIMO
	short	nResetYear;
	short	nResetMonth;
	short	nResetDay;
	short	nResetHour;
#endif
	
	VOID Clear()						// ������ �ʱ�ȭ
	{
		bIsDataLoadSuccess			= FALSE;
		btCurrentRule				= PG_RULE_TYPE_FIRST;	
		nCurrentAppRuleTime			= g_nPCBangPointFirstRuleTime;
		nCurrentAppRulePoint		= g_nPCBangPointFirstRuleTimePoint;
		nPCBangPoint				= 0;			
		bIsSuccessFirstRuleTime		= FALSE;	
		dwCheckRuleTime				= 0;			
#ifdef PCBANG_KUZIMO
		nResetYear					= 2001;
		nResetMonth					= 1;
		nResetDay					= 1;
		nResetHour					= 0;
#endif
	}
} PCBANG_POINT_INFO, *LPPCBANG_POINT_INFO;

// PC�� ����Ʈ�� ������ �ɼ�
typedef struct {
#ifdef MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904	// int������ ���ڸ� ��� Ÿ��Ȯ�� ���� �ʿ�
	int		dwItemType;					// ������ Ÿ��
#else // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	DWORD	dwItemType;					// ������ Ÿ��
#endif // MODIFY_KRIS_MOVE_WAREHOUSE_BUGFIX_20090904
	short	nBuyingPoint;				// ���� ����Ʈ
	short	nConditionPoint;			// ���� ����. �� �̻��� ����Ʈ�� �־�� ������ �� �ִ�.
	BYTE	btAbility;					// �������� ��� 0:�Ϲݾ����� 1:������
	DWORD	dwKeepTime_Sec;				// ���ӽð�(�ʴ���)
	short	nAddExpPercent;				// ����ġ ��·�
	short	nGetItemPercent;			// ������ ȹ�� Ȯ��
	BYTE	btNoMoreExp;				// ����ġ ���� ����
} PCBANG_POINT_ITEM_OPTION, *LPPCBANG_POINT_ITEM_OPTION;


class CPCBangPointSystem  
{
private:
	BOOL	m_bFileDataLoad;
	BOOL	m_bPCBangPointSystemOn;		// �ý��� On/Off
	BOOL	m_bPCBangPointRankingEvent;	// ���� ����Ʈ ��ŷ �̺�Ʈ On/Off

	BYTE	m_btPCBangPointShopNum;		// PC�� ����Ʈ ���� ��ȣ

	PCBANG_POINT_ITEM_OPTION	m_ItemOption[MAX_PCBANG_POINT_ITEM_COUNT + 1];	// PC�� ����Ʈ ���� ������ �ɼ�

	short	m_nBoxOfGreenChaosItemUsingPoint;

public:
	CPCBangPointSystem();
	virtual ~CPCBangPointSystem();

	// Init
	VOID	InitItemOption();									// �ʱ�ȭ

	// Load
	BOOL	LoadData( LPSTR lpszFileName );						// PC�� ����Ʈ �ý��� ���� ������ �д´�.

	// Check
	BOOL	CheckPCBangPointSystemUser( int iIndex );			// PC�� ����Ʈ �ý��� �ش� �����ΰ�?
	BOOL	CheckPCBangPointItem( DWORD dwItemNum );			// PC�� ����Ʈ�� �������ΰ�?	
	BOOL	CheckPCBangPointShopOpenAttr( BYTE btMapNum, short X, short Y );	// PC�� ����Ʈ ������ �� �� �ִ� ��ǥ�ΰ�?

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC	
	VOID	CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp );
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#ifdef ADD_PCBANG_EXPRATE
	BOOL	CheckPCBangAddExperience( LPOBJECTSTRUCT lpObj, INT64 &iExp );
#endif // ADD_PCBANG_EXPRATE

	// Set Info
	VOID	SetPCBangPointUser( int iIndex, BOOL bIsPCBangUser );	// PC�� ���� �����ϱ�

	// Run
	BOOL	Run( LPOBJECTSTRUCT lpObj );						// PC�� ����Ʈ�� �߰��Ѵ�.

	// GS <-> ES
	VOID	GEReqPCBangPointInfo( int iIndex );					// �̺�Ʈ������ PC�� ����Ʈ ������ ��û�Ѵ�.
	VOID	EGRecvPCBangPointInfo( LPBYTE lpMsg );				// �̺�Ʈ �����κ��� ������ �޾Ҵ�.

	VOID	GEReqUpdatePCBangPoint( int iIndex );					// PC�� ����Ʈ�� �����Ѵ�.
	VOID	EGRecvUpdatePCBangPoint( LPBYTE lpMsg );				// PC�� ����Ʈ ���� ��û�� ���� ������ �޾Ҵ�.

#ifdef PCBANG_KUZIMO
	VOID	GEReqPCBangPointInfo2( int iIndex );				
	VOID	EGRecvPCBangPointInfo2( LPBYTE lpMsg );		
	VOID	GEReqUpdatePCBangPoint2( int iIndex );
	VOID	EGRecvUpdatePCBangPoint2( LPBYTE lpMsg );
#endif

	VOID	GEReqUpdateRankingEvent( int iIndex, short nUsePoint );	// Ranking Event

	// Client <-> GS
	VOID	SendPCBangPointInfo( int iIndex );					// Ŭ���̾�Ʈ�� PC�� ����Ʈ�� ������.
	VOID	PCBangPointShopOpen( int iIndex );					// PC�� ����Ʈ ������ ������ ������.
	BOOL	BuyPCBangPointItem( int iIndex, BYTE btShopPos );	// PC�� ����Ʈ �������� �������� ����

#ifdef PCBANG_KUZIMO
	static VOID WeeklyTimerCallBack(MWeeklyTimer *pkMyDate, VOID *pArg);	// �ش� �ָ��� ���� ���ǽÿ� �ݹ�ȴ�.
#endif
};

extern CPCBangPointSystem g_PCBangPointSystem;

#endif // PCBANG_POINT_SYSTEM_20070206

#endif // !defined(AFX_PCBANGPOINTSYSTEM_H__5B026023_5F54_4FF7_A375_08549D5F30D3__INCLUDED_)
