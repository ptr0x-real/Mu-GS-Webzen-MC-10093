// CashItemPeriodSystem.h: interface for the CCashItemPeriodSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHITEMPERIODSYSTEM_H__73C2CF26_7712_4B8B_B02E_A8721C1CB76C__INCLUDED_)
#define AFX_CASHITEMPERIODSYSTEM_H__73C2CF26_7712_4B8B_B02E_A8721C1CB76C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#define MAX_ITEMPERIOD_INFO			MAX_USER
#define MAX_ITEMPERIOD_DATE_LENGTH	20

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
#define MAX_PERIODITEM_DATEINFO		10

#define PERIOD_ITEM_CLEAR_ALL		-1

#define ITEM_EFFECTDATE_USE_FILE	-1
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#define PERIOD_MINUTE				( 60 * 1000 )		// 1 분
#define PERIODITEM_CHECK_TIME		( 10 * 1000 )		// 10초

// for DataServer Protocol
//----------------------------------------------------------------------------
// GD [0xD0] 기간제 아이템 사용 여부 요청 (0x01)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;
}PMSG_REQ_PERIODITEM_INQUIRY, *LPPMSG_REQ_PERIODITEM_INQUIRY;

//----------------------------------------------------------------------------
// DG [0xD0] 기간제 아이템 사용 여부 응답 (0x02)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	BYTE			btResult;
}PMSG_ANS_PERIODITEM_INQUIRY, *LPPMSG_ANS_PERIODITEM_INQUIRY;

//----------------------------------------------------------------------------
// GD [0xD0] 기간제 아이템 추가 요청 (0x03)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;

	int				iItemCode;
	int				iItemPeriodDate;

	BYTE			btItemEffectType1;
	BYTE			btItemEffectType2;

	char			chUserID[MAX_IDSTRING+1];
	char			chExpireDate[MAX_ITEMPERIOD_DATE_LENGTH];

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	BYTE			btEffectCategory;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
}PMSG_REQ_PERIODITEM_INSERT, *LPPMSG_REQ_PERIODITEM_INSERT;

//----------------------------------------------------------------------------
// DG [0xD0] 기간제 아이템 추가 응답 (0x04)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;

	int				iItemCode;
	int				iItemPeriodDate;
	BYTE			btItemEffectType1;
	BYTE			btItemEffectType2;
	BYTE			btResult;
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	BYTE			btEffectCategory;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
}PMSG_ANS_PERIODITEM_INSERT, *LPPMSG_ANS_PERIODITEM_INSERT;

//----------------------------------------------------------------------------
// GD [0xD0] 기간제 아이템 리스트 요청 (0x05)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;
	char			chCharacterName[MAX_IDSTRING+1];
}PMSG_REQ_PERIODITEM_LIST, *LPPMSG_REQ_PERIODITEM_LIST;

//----------------------------------------------------------------------------
// DG [0xD0] 기간제 아이템 리스트 응답 (0x06)
//----------------------------------------------------------------------------
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
typedef struct
{
	BYTE	btUsedInfo;
	int		iItemCode;
	BYTE	btEffectCategory;
	BYTE	btEffectType1;
	BYTE	btEffectType2;
	DWORD	dwItemUsedTime;
	DWORD	dwItemLeftTime;
}PERIODITEM_DATA_EXT;

typedef struct
{
	PBMSG_HEAD2			head;

	int					iUserIndex;
	int					iUserGuid;
	BYTE				btItemUseCount;
	char				btResult;		// 0이상 : 성공, -1 : 아이템이 1개 이상, -2 : 사용기간 만료, -3 : DB 오류

	PERIODITEM_DATA_EXT	Date[MAX_PERIODITEM_DATEINFO];
}PMSG_ANS_PERIODITEM_LIST, *LPPMSG_ANS_PERIODITEM_LIST;
#else
typedef struct
{
	PBMSG_HEAD2		head;

	int				wUserIndex;
	int				iItemCode;
	BYTE			btEffectType1;
	BYTE			btEffectType2;
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	int				iLeftTime;
	int				iUserGuid;
#else
	char			chPeriodItemEndDate[MAX_ITEMPERIOD_DATE_LENGTH];
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	char			btResult;		// 0이상 : 성공, -1 : 아이템이 1개 이상, -2 : 사용기간 만료, -3 : DB 오류
}PMSG_ANS_PERIODITEM_LIST, *LPPMSG_ANS_PERIODITEM_LIST;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

//----------------------------------------------------------------------------
// GD [0xD0] 기간제 아이템 업데이트 요청 (0x07)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;
	int				iItemCode;
	int				iUsedTime;
	int				iLeftTime;

	char			chCharacterName[MAX_IDSTRING+1];
}PMSG_REQ_PERIODITEM_UPDATE, *LPPMSG_REQ_PERIODITEM_UPDATE;

//----------------------------------------------------------------------------
// DG [0xD0] 기간제 아이템 업데이트 응답 (0x08)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iItemCode;
	BYTE			btResult;			// 결과 : 0x00(성공), 0x01(잘못된 아이템 코드), 0x02(존재하지 않는 사용자), 0x03(사용기간 만료), 0x04(기타)

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	BYTE			btEffectCategory;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
}PMSG_ANS_PERIODITEM_UPDATE, *LPPMSG_ANS_PERIODITEM_UPDATE;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
//----------------------------------------------------------------------------
// GD [0xD0] 기간제 아이템 삭제 요청 (0x09)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;

	char			chCharacterName[MAX_IDSTRING+1];
}PMSG_REQ_PERIODITEM_DELETE, *LPPMSG_REQ_PERIODITEM_DELETE;

//----------------------------------------------------------------------------
// DG [0xD0] 기간제 아이템 삭제 응답 (0x0A)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD2		head;

	int				iUserIndex;
	int				iUserGuid;
	BYTE			btResult;
}PMSG_ANS_PERIODITEM_DELETE, *LPPMSG_ANS_PERIODITEM_DELETE;
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

// for client protocol
typedef struct
{
	PBMSG_HEAD2		head;

	BYTE			btResult;
}PMSG_ANS_PERIODITEM_INSERT_RESULT, *LPPMSG_ANS_PERIODITEM_INSERT_RESULT;

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423

typedef struct
{
	BYTE		btUsedInfo;				// 아이템 사용 여부 (0:사용안함, 1:사용)
	BYTE		btEffectCategory;		// 아이템 효과 카테고리
	BYTE		btEffectType1;			// 아이템 사용 효과 1
	BYTE		btEffectType2;			// 아이템 사용 효과 2
	int			iItemCode;				// 아이템 코드
	DWORD		dwItemUsedTime;			// 아이템 사용 시간
	DWORD		dwItemLeftTime;			// 아이템 사용 남은 시간
}ITEMPERIOD_DATEINFO, *LPITEMPERIOD_DATEINFO;

typedef struct
{
	BYTE		btUsedInfo;				// 기간제 아이템 정보 사용 여부 (0:사용안함, 1:사용)

	BYTE		btDateInfoUseCount;		// DateInfo 사용 개수
	WORD		wUserIndex;				// 사용자 서버 인덱스
	int			iUserGuid;				// 사용자 Guid
	DWORD		dwItemCheckTickCount;	// 사용자 시간 채크 틱 카운트

	char		chCharacterName[MAX_IDSTRING+1];	// 사용자 캐릭터 이름

	ITEMPERIOD_DATEINFO	DateInfo[MAX_PERIODITEM_DATEINFO];		// 아이템 효과 슬롯 (160)
	// 171 바이트
}ITEMPERIOD_INFO, *LPITEMPERIOD_INFO;

#else
typedef struct
{
	BYTE	btUsedInfo;
	int		iUserIndex;
	int		iUserGuid;
	char	chCharacterName[MAX_IDSTRING+1];
	int		iItemCode;
	BYTE	btEffectType1;				// 아이템 사용 효과 1
	BYTE	btEffectType2;				// 아이템 사용 효과 2
	int		iItemPeriodUsedTime;		// 아이템 사용 기간 (단위:분)
	int		iItemPeriodLeftTime;		// 아이템 남은 사용 기간 (단위:분)
	char	chItemPeriodBuyDate[MAX_ITEMPERIOD_DATE_LENGTH];	// 아이템 구입 시기 (날짜)
	char	chItemPeriodEndDate[MAX_ITEMPERIOD_DATE_LENGTH];	// 아이템 만료 시기 (날짜)

	DWORD	dwItemCheckTickCount;
	// 69 바이트
}ITEMPERIOD_INFO, *LPITEMPERIOD_INFO;
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

class CCashItemPeriodSystem  
{
public:
	CCashItemPeriodSystem();
	virtual ~CCashItemPeriodSystem();
	void Initialize();
	void GetUserPeriodItemList(LPOBJECTSTRUCT lpObj);
	BOOL SetPeriodItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode, BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod);
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	BOOL ClearPeriodItemEffect(LPOBJECTSTRUCT lpObj, int iItemCode);
	bool CheckItemUsedInfo(LPOBJECTSTRUCT lpObj, int iItemCode);
	LPITEMPERIOD_INFO GetPeriodInfo(LPOBJECTSTRUCT lpObj);
#else
	BOOL ClearPeriodItemEffect(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

	// for DataServer
	void GDReqPeriodItemList(LPOBJECTSTRUCT lpObj);
	void GDReqPeriodItemInsert(LPOBJECTSTRUCT lpObj, int iItemCode, int iItemEffectDate);
	void GDReqPeriodItemInquiry(LPOBJECTSTRUCT lpObj);
#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	void GDReqPeriodItemUpdate(LPOBJECTSTRUCT lpObj, int iDateInfoIndex);
#else
	void GDReqPeriodItemUpdate(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	void DGAnsPeriodItemList(LPPMSG_ANS_PERIODITEM_LIST aRecv);
	void DGAnsPeriodItemInsert(LPPMSG_ANS_PERIODITEM_INSERT aRecv);
	void DGAnsPeriodItemInquiry(LPPMSG_ANS_PERIODITEM_INQUIRY aRecv);
	void DGAnsPeriodItemUpdate(LPPMSG_ANS_PERIODITEM_UPDATE aRecv);
#ifdef MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326
	void GDReqPeriodItemDelete(LPOBJECTSTRUCT lpObj, char* pchCharacterName);
	void DGAnsPeriodItemDelete(LPPMSG_ANS_PERIODITEM_DELETE aRecv);
#endif // MODIFY_PERIOD_ITEM_SYSTEM_BUGFIX_20070326

	// for client
	void GCSendPeriodItemInserResult(LPOBJECTSTRUCT lpObj, BYTE btResult);

	// for time
	int CalcItemUseLeftTime(char* pchBuyDate);
	CTime CalcItemUseEndTime(int iLeftTime);
	void ConvertStringToTime(const char* pchDate, CTime* lpTime);
	void ConvertTimeToString(const CTime* lpTime, char* pchDate);

	// for Thread
	static DWORD WINAPI PeriodCheckThread(LPVOID lpPeriodInfo);

#ifdef MODIFY_PERIODITEM_SLOT_EXTEND_20070423
	BOOL CheckMapBoundFreeItemUse(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_PERIODITEM_SLOT_EXTEND_20070423

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
private:
	BOOL SetPeriodItemBuffEffect(LPOBJECTSTRUCT lpObj, int iItemCode, int iItemUsePeriod);
	int GetBuffType(int iItemCode);
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	
private:
	int iItemPeriodInfoCount;
	ITEMPERIOD_INFO	ItemPeriodInfo[MAX_ITEMPERIOD_INFO];

	HANDLE hThreadHandle;

#ifdef MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
	CRITICAL_SECTION csPeriodItemSystem;
#endif // MODIFY_PERIOD_ITEM_SYSTEM_CRITICALSECTION_20070619
};

extern CCashItemPeriodSystem g_CashItemPeriodSystem;
extern BOOL g_bUsePeriodCheckThread;
extern BOOL g_bRunningPeriodCheckThread;

#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#endif // !defined(AFX_CASHITEMPERIODSYSTEM_H__73C2CF26_7712_4B8B_B02E_A8721C1CB76C__INCLUDED_)
