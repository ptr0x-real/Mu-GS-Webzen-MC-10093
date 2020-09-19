#ifndef __CB_PROTOCOL_H__
#define __CB_PROTOCOL_H__

#include "ProDef.h"

#ifdef EVENTDB_PROTOCOLEX

#define MASK_EVENTDB_PROTOCOLEX			0x30

enum eDBTask
{
	DBTASK_REQ_GET_PAYITEM				= MASK_EVENTDB_PROTOCOLEX | 1, 
		DBTASK_ANS_SET_PAYITEM, 
};

enum ePayItemEventID
{
	EVENT_PAYITEM_START					= 0x01,
		EVENT_XMAS							= EVENT_PAYITEM_START,	// 크리스마스 이벤트 아이템 지급
		EVENT_PAYITEM_END					= EVENT_XMAS, 
};

//이벤트 아이템 지급 에러
namespace EPAYITEM
{
	enum EERROR
	{
		E_NO_ERROR						= 0x00, 
			E_INVALID_EVENTID, 
			E_ALREADY_SETITEM,				// 이미 지급되었음
			E_FAILED_RESULT,				// 에러
	};
};

#define IS_PAYITEMEVENT(t)				(EVENT_PAYITEM_START <= t && EVENT_PAYITEM_END >= t)

#endif // EVENTDB_PROTOCOLEX

//----------------------------------------------------------------------------
// [0x01] 경품 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		AccountId[MAX_IDSTRING];		// 계정 아이디
	char		GameId[MAX_IDSTRING];			// 캐릭터 이름
	int			Sequence;						// 게임서버 확인용 숫자

} CB_PMSG_REQ_PRESENT, * LPCB_PMSG_REQ_PRESENT;

//----------------------------------------------------------------------------
// [0x01] 경품 요청 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	

	char		AccountId[MAX_IDSTRING];		// 계정 아이디
	char		GameId[MAX_IDSTRING];			// 캐릭터 이름
	int			Sequence;						// 게임서버 확인용 숫자
	
	char		present;		// 이벤트 상품 번호 
								// 0 : 히딩크 고향 여행권
								// 1 : 펜티엄 4
								// 2 : 디지털 카메라
								// 3 : 휠마우스 + 키보드
								// 4 : 256램
								// 5 : 게임 잡지 6개월 무료 구독권
								// 6 : 문화상품권
								// 7 : 뮤 머그컵
								// 8 : 뮤 T 셔츠
								// 9 : 뮤 10시간 무료 이용권
								// -1 : 꽝 -_-;;
	char		presentname[MAX_PRESENTNAME];	// 최대 경품 이름
	int			Seq_present;	// 경품 인덱스

} CB_PMSG_REQ_PRESENT_RESULT, * LPCB_PMSG_REQ_PRESENT_RESULT;

//----------------------------------------------------------------------------
// [0x02] 경품 당청자 정보
typedef struct
{
	PBMSG_HEAD	h;

	char		AccountID[MAX_IDSTRING];
	char		GameID[MAX_IDSTRING];
	int			servernumber;

	char		present;		// 이벤트 상품 번호
	int			Seq_present;	// 경품 인덱스
} PMSG_PRIZE_INFO, * LPPMSG_PRIZE_INFO;


//----------------------------------------------------------------------------
// [0x03] 경품 상자 테이블 초기화
typedef struct
{
	PBMSG_HEAD	h;
	int			MaxBoxSize;		// 경품상자의 최대 크기
} PMSG_RESET_PRESENTBOX, * LPPMSG_RESET_PRESENTBOX;

//----------------------------------------------------------------------------
// [0x04] 경품 현황 보기
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// 경품 번호
} PMSG_REQ_PRESENTCOUNT, * LPPMSG_REQ_PRESENTCOUNT;


//----------------------------------------------------------------------------
// [0x04] 경품 현황 결과
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// 경품 번호
	int			pcount;			// 경품 수량
} PMSG_PRESENTCOUNT_INFO, * LPPMSG_PRESENTCOUNT_INFO;

//----------------------------------------------------------------------------
// [0x05] 경품 현황 수정
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// 경품 번호
	int			pcount;			// 경품 수량
} PMSG_EDIT_PRESENTCOUNT, * LPPMSG_EDIT_PRESENTCOUNT;

//----------------------------------------------------------------------------
// [0x05] 경품 현황 수정결과
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// 경품 번호
	int			pcount;			// 경품 수량
} PMSG_EDIT_PRESENTCOUNT_RESULT, * LPPMSG_EDIT_PRESENTCOUNT_RESULT;



//////////////////////////////////////////////////////////////////////////////
// MU RANKING SERVER PROTOCOL
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// S -> C [0x01] : 게임서버에서 랭킹서버에 데빌스퀘어 점수를 보내줌
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT Score;
	INT Class;
	INT SquareNum;
} PMSG_ANS_EVENTUSERSCORE, * LPPMSG_ANS_EVENTUSERSCORE;
//----------------------------------------------------------------------------
// S -> C [0x02] : 게임서버에서 데빌스퀘어 이벤트가 종료되었음을 알림
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	INT ServerCode;
} PMSG_ANS_EVENTCLOSED, * LPPMSG_ANS_EVENTCLOSED;

//----------------------------------------------------------------------------
// S -> C [0x03] : 게임서버에서 랭킹서버에 데빌스퀘어 점수를 보내줌
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
} PMSG_ANS_EVENTLIVE, * LPPMSG_ANS_EVENTLIVE;

#ifdef FOR_BLOODCASTLE

/////////////////////////////////////////////////////////////////////////////
// 랭킹서버와의 프로토콜 (블러드 캐슬)
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// S -> C [0x07] : 게임서버에서 랭킹서버에 블러드캐슬 점수를 보내줌
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT Score;
	INT Class;
	INT BridgeNum;
#ifdef FOR_BLOODCASLTE_PCEVENT
	INT iPCRoomGUID;
#endif
} PMSG_ANS_BLOODCASTLESCORE, * LPPMSG_ANS_BLOODCASTLESCORE;


#ifdef BLOODCASTLE_EVENT_3RD_20040401


//----------------------------------------------------------------------------
// C -> S [0x0A] : 게임서버에서 랭킹서버에 블러드캐슬 점수를 보내줌 (남은시간 합해서)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT Score;
	INT Class;
	INT BridgeNum;
#ifdef FOR_BLOODCASLTE_PCEVENT
	INT iPCRoomGUID;
#endif
	INT iLeftTime;
} PMSG_ANS_BLOODCASTLESCORE_3RD, * LPPMSG_ANS_BLOODCASTLESCORE_3RD;



//----------------------------------------------------------------------------
// C -> S [0x0B] : 게임서버에서 랭킹서버에 특정 서버의 계정, 케릭에 대하여 블러드캐슬의 남은 입장횟수를 요청
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_BLOODCASTLE_ENTERCOUNT, * LPPMSG_REQ_BLOODCASTLE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x0B] : 게임서버에게 특정 서버의 계정, 케릭의 남은 블러드캐슬 입장횟수를 보냄
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
	INT iLeftCount;
} PMSG_ANS_BLOODCASTLE_ENTERCOUNT, * LPPMSG_ANS_BLOODCASTLE_ENTERCOUNT;


#endif


#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
// 블러드캐슬 하루 입장수 제한

//----------------------------------------------------------------------------
// C -> S [0x08] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 블러드캐슬 하루 입장횟수 체크를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	INT		iItemPos;		// 투명망토 위치
	INT		iItemLevel;		// 투명망토 레벨
	INT		iBridgeNumber;	// 다리번호
} PMSG_REQ_BLOODCASTLEENTERCHECK, * LPPMSG_REQ_BLOODCASTLEENTERCHECK;


//----------------------------------------------------------------------------
// S -> C [0x08] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 블러드캐슬 하루 입장횟수 체크를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	BOOL	bEnterCheck;	// 0 - 입장불가 / 1 - 입장가능
	INT		iItemPos;		// 투명망토 위치
	INT		iItemLevel;		// 투명망토 레벨
	INT		iBridgeNumber;	// 다리번호
} PMSG_ANS_BLOODCASTLEENTERCHECK, * LPPMSG_ANS_BLOODCASTLEENTERCHECK;


//----------------------------------------------------------------------------
// C -> S [0x09] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 블러드캐슬 하루 입장횟수를 올리라고 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
} PMSG_REQ_BLOODCASTLECOUNTON, * LPPMSG_REQ_BLOODCASTLECOUNTON;

#endif

#endif


#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
// 데빌스퀘어 하루 입장수 제한

//----------------------------------------------------------------------------
// C -> S [0x0F] : 게임서버에서 랭킹서버에 특정 서버의 계정, 케릭에 대하여 데빌스퀘어의 남은 입장횟수를 요청
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_DEVILSQUARE_ENTERCOUNT, * LPPMSG_REQ_DEVILSQUARE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x0F] : 게임서버에게 특정 서버의 계정, 케릭의 남은 데빌스퀘어 입장횟수를 보냄
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
	INT iLeftCount;
} PMSG_ANS_DEVILSQUARE_ENTERCOUNT, * LPPMSG_ANS_DEVILSQUARE_ENTERCOUNT;


//----------------------------------------------------------------------------
// C -> S [0x10] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 데빌스퀘어 하루 입장횟수 체크를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	INT		iItemPos;		// 초대장 위치
	INT		iItemLevel;		// 초대장 레벨
	INT		iSquareNumber;	// 광장번호
} PMSG_REQ_DEVILSQUAREENTERCHECK, * LPPMSG_REQ_DEVILSQUAREENTERCHECK;


//----------------------------------------------------------------------------
// S -> C [0x10] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 데빌스퀘어 하루 입장횟수 체크를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	BOOL	bEnterCheck;	// 0 - 입장불가 / 1 - 입장가능
	INT		iItemPos;		// 초대장 위치
	INT		iItemLevel;		// 초대장 레벨
	INT		iSquareNumber;	// 광장번호
} PMSG_ANS_DEVILSQUAREENTERCHECK, * LPPMSG_ANS_DEVILSQUAREENTERCHECK;


//----------------------------------------------------------------------------
// C -> S [0x11] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 데빌스퀘어 하루 입장횟수를 올리라고 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
} PMSG_REQ_DEVILSQUARECOUNTON, * LPPMSG_REQ_DEVILSQUARECOUNTON;

#endif


#ifdef BLOODCASTLE_EVENT_5TH_20050531


//----------------------------------------------------------------------------
// C -> S [0x0D] : 게임서버에서 랭킹서버에 블러드캐슬 점수를 보내줌 (남은시간 합해서)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT Score;
	INT Class;
	INT BridgeNum;
	INT iLeftTime;
	INT iAlivePartyCount;
} PMSG_ANS_BLOODCASTLESCORE_5TH, * LPPMSG_ANS_BLOODCASTLESCORE_5TH;


#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영사원 점수 랭킹서버 추가

// 하루 입장제한 : 6번
//----------------------------------------------------------------------------
// C -> S [0x12] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 환영 사원의 하루 입장횟수 체크를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	INT		iItemPos;		// 피의 두루마리 위치
	INT		iItemLevel;		// 피의 두루마리 레벨
	INT		iTempleNumber;	// 환영 사원 번호
} PMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK, * LPPMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK;


//----------------------------------------------------------------------------
// S -> C [0x12] : 랭킹서버에서 게임서버에 특정 계정,서버,케릭의 환영 사원의 하루 입장횟수 체크를 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// 서버에서 사용하는 사용자 인덱스
	BOOL	bEnterCheck;	// 0 - 입장불가 / 1 - 입장가능
	INT		iItemPos;		// 피의 두루마리 위치
	INT		iItemLevel;		// 피의 두루마리 레벨
	INT		iTempleNumber;	// 환영 사원 번호
} PMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK, * LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK;


//----------------------------------------------------------------------------
// C -> S [0x13] : 게임서버에서 랭킹서버에 특정 계정,서버,케릭의 환영 사원 하루 입장횟수를 올리라고 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
} PMSG_REQ_ILLUSION_TEMPLE_INC_COUNT, * LPPMSG_REQ_ILLUSION_TEMPLE_INC_COUNT;

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_SEASON_3_NEW_UI_20071122
//----------------------------------------------------------------------------
// C -> S [0x14] : 게임서버에서 랭킹서버에 특정 서버의 계정, 케릭에 대하여 환영사원의 남은 입장횟수를 요청
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT, * LPPMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x14] : 게임서버에게 특정 서버의 계정, 케릭의 남은 환영사원의 입장횟수를 보냄
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
	INT iLeftCount;
} PMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT, * LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT;

#endif	// ADD_SEASON_3_NEW_UI_20071122


//////////////////////////////////////////////////////////////////////////////
// MU EVENT SERVER PROTOCOL
//////////////////////////////////////////////////////////////////////////////

//============================================================================
// 이벤트칩 이벤트
//============================================================================
//----------------------------------------------------------------------------
// GS -> ES [0x01] : 해당 계정의 현재 이벤트 칩 개수와 뮤토번호 확인을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_VIEW_EC_MN, * LPPMSG_REQ_VIEW_EC_MN;

//----------------------------------------------------------------------------
// ES -> GS [0x01] : 해당 계정의 현재 이벤트 칩 개수와 뮤토번호를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	
#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			nEVENT_CHIPS;		// 현재까지의 이벤트 칩 개수
#else
	SHORT		nEVENT_CHIPS;		// 현재까지의 이벤트 칩 개수
#endif // EVENTCHIP_DATASIZE_EXPAND_20090217
	
	INT			iMUTO_NUM;			// 현재의 뮤토번호 - 0이면 아직 등록않됨
} PMSG_ANS_VIEW_EC_MN, * LPPMSG_ANS_VIEW_EC_MN;


//----------------------------------------------------------------------------
// GS -> ES [0x02] : 해당 계정의 이벤트 칩 등록을 요청한다. (이미 있는 것에 추가)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		Pos;				// 아이템 위치
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_REGISTER_EVENTCHIP, * LPPMSG_REQ_REGISTER_EVENTCHIP;


//----------------------------------------------------------------------------
// ES -> GS [0x02] : 해당 계정의 이벤트 칩을 등록 요청에 대한 답을 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		Pos;				// 아이템 위치
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	
#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			nEVENT_CHIPS;		// 현재까지의 이벤트 칩 개수
#else
	SHORT		nEVENT_CHIPS;		// 현재까지의 이벤트 칩 개수
#endif // EVENTCHIP_DATASIZE_EXPAND_20090217
	
} PMSG_ANS_REGISTER_EVENTCHIP, * LPPMSG_ANS_REGISTER_EVENTCHIP;


//----------------------------------------------------------------------------
// GS -> ES [0x03] : 해당 개정의 뮤토 번호 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_REGISTER_MUTONUM, * LPPMSG_REQ_REGISTER_MUTONUM;

//----------------------------------------------------------------------------
// ES -> GS [0x03] : 해당 개정의 뮤토 번호 등록에 대한 결과를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	INT			iMUTO_NUM;			// 성공했다면 반드시 들어가 있다.
} PMSG_ANS_REGISTER_MUTONUM, * LPPMSG_ANS_REGISTER_MUTONUM;


//----------------------------------------------------------------------------
// GS -> ES [0x04] : 해당 개정의 이벤트 칩 개수를 0으로 만든다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_RESET_EVENTCHIP, * LPPMSG_REQ_RESET_EVENTCHIP;

//----------------------------------------------------------------------------
// ES -> GS [0x04] : 해당 개정의 이벤트 칩 개수를 0으로 만든 결과를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
} PMSG_ANS_RESET_EVENTCHIP, * LPPMSG_ANS_RESET_EVENTCHIP;

#ifdef FOR_BLOODCASTLE

//----------------------------------------------------------------------------
// GS -> ES [0x05] : 해당 계정의 현재 스톤 개수 확인을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_VIEW_STONES, * LPPMSG_REQ_VIEW_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x06] : 해당 계정의 스톤 추가 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// 아이템 위치
	CHAR		szUID [11];			// 사용자 개정
} PMSG_REQ_REGISTER_STONES, * LPPMSG_REQ_REGISTER_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x07] : 해당 계정의 현재 스톤의 사용 (일정량 삭제)을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	INT			iStoneCount;		// 감소될 스톤개수
} PMSG_REQ_DELETE_STONES, * LPPMSG_REQ_DELETE_STONES;


//============================================================================
//============================================================================


//----------------------------------------------------------------------------
// ES -> GS [0x05] : 해당 계정의 현재 스톤 개수를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	INT			iStoneCount;
} PMSG_ANS_VIEW_STONES, * LPPMSG_ANS_VIEW_STONES;


//----------------------------------------------------------------------------
// ES -> GS [0x06] : 해당 계정의 스톤추가 결과를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// 아이템 위치
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	INT			iStoneCount;		// 현재까지의 스톤개수
} PMSG_ANS_REGISTER_STONES, * LPPMSG_ANS_REGISTER_STONES;


//----------------------------------------------------------------------------
// ES -> GS [0x07] : 해당 계정의 스톤삭제 결과를 보낸다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		bSUCCESS;			// 0-실패 / 1-싱공
	INT			iStoneCount;		// 현재까지의 스톤개수
} PMSG_ANS_DELETE_STONES, * LPPMSG_ANS_DELETE_STONES;


#endif


#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO ) // 한국 유료화 2주년 기념 복권 이벤트

//----------------------------------------------------------------------------
// GS -> ES [0x08] : 해당 시리얼을 등록한다고 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	CHAR		SERIAL1[5];			// 시리얼1
	CHAR		SERIAL2[5];			// 시리얼2
	CHAR		SERIAL3[5];			// 시리얼3
	INT			iMEMB_GUID;			// 사용자 GUID
} PMSG_REQ_2ANIV_SERIAL, * LPPMSG_REQ_2ANIV_SERIAL;


//----------------------------------------------------------------------------
// ES -> GS [0x08] : 해당 시리얼의 등록결과를 전달
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	BYTE		btIsRegistered;		// 0:이면 본인이 등록했음 (성공) / 1:다른사람이 등록했음 (아래 필드에 정보없음) / 2:등록제한횟수참 / 3:존재하지 않는 시리얼 / 4:알수없는에러 
	INT			iGiftNumber;		// 상품번호 (서버에 텍스트 가지고 있다가 전송)
} PMSG_ANS_2ANIV_SERIAL, * LPPMSG_ANS_2ANIV_SERIAL;

#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef MONSTER_HERD_SYSTEM_20031120


//----------------------------------------------------------------------------
// GS -> ES [0x10] : 해당 시리얼을 등록한다고 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	BYTE		btGiftKind;			// 상품의 종류 (0:용사의반지드롭, 1:오크죽임)
} PMSG_REQ_REG_RINGGIFT, * LPPMSG_REQ_REG_RINGGIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x10] : 해당 시리얼의 등록결과를 전달
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	BYTE		btIsRegistered;		// 1:성공 / 0:실패 / 2:이미 등록됨 / 3:오늘나갈개수초과
	BYTE		btGiftSection;		// 상품의 분류 (0:용사의반지드롭, 1:오크죽임)
	BYTE		btGiftKind;			// 상품의 종류 (1:100돈 / 2:10돈 / 3:5돈 / 4:2돈)
} PMSG_ANS_REG_RINGGIFT, * LPPMSG_ANS_REG_RINGGIFT;


#endif


#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/*                     FRIENDSHIP STONE EVENT PROTOCOL                      */ 
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
// GS -> ES [0x11] : 해당 계정의 현재 우정의돌 개수 확인을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
} PMSG_REQ_VIEW_FRIENDSHIP_STONES, * LPPMSG_REQ_VIEW_FRIENDSHIP_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x12] : 해당 계정의 우정의돌 추가 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// 아이템 위치
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
} PMSG_REQ_REGISTER_FRIENDSHIP_STONES, * LPPMSG_REQ_REGISTER_FRIENDSHIP_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x13] : 해당 계정의 현재 우정의돌의 사용 (일정량 삭제)을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
	INT			iStoneCount;		// 감소될 스톤개수
} PMSG_REQ_DELETE_FRIENDSHIP_STONES, * LPPMSG_REQ_DELETE_FRIENDSHIP_STONES;


#endif // EVENT_STONE_OF_FRIENDSHIP_20040206


#ifdef CHAOSCASTLE_SYSTEM_20040408			// 카오스 캐슬 오프라인 경품과 관련된 패킷 정의들

//----------------------------------------------------------------------------
// GS -> ES [0x15] : 카오스 캐슬 오프라인 이벤트 - 해당 케릭의 상품 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
} PMSG_REQ_REG_CC_OFFLINE_GIFT, * LPPMSG_REQ_REG_CC_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x15] : 카오스 캐슬 오프라인 이벤트 - 해당 케릭의 상품 등록 요청에 대한 응답을 받는다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
	INT			iResultCode;		// 결과코드
	CHAR		szGIFT_NAME[50];	// 상품이름
} PMSG_ANS_REG_CC_OFFLINE_GIFT, * LPPMSG_ANS_REG_CC_OFFLINE_GIFT;


#endif


#ifdef DARK_LORD_HEART_EVENT_20040810

//----------------------------------------------------------------------------
// GS -> ES [0x16] : 다크로드의 마음 오프라인 이벤트 - 해당 케릭의 상품 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
} PMSG_REQ_REG_DL_OFFLINE_GIFT, * LPPMSG_REQ_REG_DL_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x16] : 다크로드의 마음 오프라인 이벤트 - 해당 케릭의 상품 등록 요청에 대한 응답을 받는다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
	INT			iResultCode;		// 결과코드
	CHAR		szGIFT_NAME[50];	// 상품이름
} PMSG_ANS_REG_DL_OFFLINE_GIFT, * LPPMSG_ANS_REG_DL_OFFLINE_GIFT;

#endif


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804

//----------------------------------------------------------------------------
// GS -> ES [0x17] : 숨겨진 보물상자 이벤트 - 해당 케릭의 상품 등록을 요청한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
} PMSG_REQ_REG_HT_OFFLINE_GIFT, * LPPMSG_REQ_REG_HT_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x17] : 숨겨진 보물상자 이벤트 - 해당 케릭의 상품 등록 요청에 대한 응답을 받는다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// 사용자 개정
	WORD		wServerCode;		// 서버코드
	CHAR		szNAME [11];		// 케릭터 이름
	INT			iResultCode;		// 결과코드
	CHAR		szGIFT_NAME[50];	// 상품이름
} PMSG_ANS_REG_HT_OFFLINE_GIFT, * LPPMSG_ANS_REG_HT_OFFLINE_GIFT;

#endif

#ifdef PCBANG_COUPON_EVENT_20060124
//----------------------------------------------------------------------------
// GS -> ES [0x18] : PC방 쿠폰 이벤트 - 쿠폰을 사용할 수 있는지 확인 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_POSSIBLE_PCBANG_COUPON, *LPPMSG_REQ_POSSIBLE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// ES -> GS [0x18] : PC방 쿠폰 이벤트 - PC방 쿠폰 사용 확인 요청에 대한 응답을 보낸다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 성공, 0x01 : 쿠폰 미등록
										//			0x02 : 이미 발급된 쿠폰	
										//			0x03 : 실패
} PMSG_ANS_POSSIBLE_PCBANG_COUPON, *LPPMSG_ANS_POSSIBLE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// GS -> ES [0x19] : PC방 쿠폰 이벤트 - PC방 쿠폰 사용 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_USE_PCBANG_COUPON, *LPPMSG_REQ_USE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// ES -> GS [0x19] : PC방 쿠폰 이벤트 - PC방 쿠폰 사용 요청에 대한 응답을 보낸다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 성공
										//			0x01 : 실패
} PMSG_ANS_USE_PCBANG_COUPON, *LPPMSG_ANS_USE_PCBANG_COUPON;
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822

//////////////////////////////////////////////////////////////////////////////
/*                WHITE ANGEL GET ITEM EVENT PROTOCOL                      */ 
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// GS -> ES [0x20] : 화이트엔젤 아이템지급 이벤트 - 아이템 지급 대상인지 확인
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_CHECK_WHITEANGEL_GET_ITEM, *LPPMSG_REQ_CHECK_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x20] : 화이트엔젤 아이템지급 이벤트 - 아이템 지급 대상인지 확인에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 지급 대상자
										//			0x01 : 지급 대상자가 아님
										//			0x02 : 이미 지급되었음
										//			0x03 : 에러
} PMSG_ANS_CHECK_WHITEANGEL_GET_ITEM, *LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// GS -> ES [0x21] : 화이트엔젤 아이템지급 이벤트 - 아이템 지급 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_WHITEANGEL_GET_ITEM, *LPPMSG_REQ_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x21] : 화이트엔젤 아이템지급 이벤트 - 아이템 지급 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 실패
										//			0x01 : 성공													
} PMSG_ANS_WHITEANGEL_GET_ITEM, *LPPMSG_ANS_WHITEANGEL_GET_ITEM;

#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
//////////////////////////////////////////////////////////////////////////////
/*                KANTURU TIMEATTACK EVENT PROTOCOL						    */ 
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// GS -> ES [0x22] : 칸투르 타임어택 이벤트 - 칸투르 보스전 세부전투별 클리어 기록을 남긴다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	CHAR		szNAME[11];				// 케릭터 이름
	WORD		wServerCode;			// 서버코드
	CHAR		szBattleID[14];			// 칸투르 보스전 ID
	BYTE		btStageNumber;			// 전투단계 번호
	WORD		wClearTime;				// 클리어 시간
	int			iLevel;					// 레벨
	int			iExp;					// 경험치	
} PMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT, *LPPMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT;
#endif	// KANTURU_TIMEATTACK_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206		// EventServer Protocol Define
//////////////////////////////////////////////////////////////////////////////
/*                     PCBANG GAGE SYSTME PROTOCOL							*/ 
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// GS -> ES [0x23] : PC방 포인트 시스템 - 포인트 정보를 요청한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	BYTE		btServerType;			// 서버타입 0:본서버 1:테스트서버(FOR_NEW_TESTSERVER)	
	CHAR		szUID[11];				// 사용자 계정
} PMSG_REQ_PCBANG_POINT_INFO, *LPPMSG_REQ_PCBANG_POINT_INFO;

//----------------------------------------------------------------------------
// ES -> GS [0x23] : PC방 포인트 시스템 - 포인트 정보를 되돌려준다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	short		nPoint;					// PC방 포인트
#ifdef PCBANG_KUZIMO
	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
#endif
} PMSG_ANS_PCBANG_POINT_INFO, *LPPMSG_ANS_PCBANG_POINT_INFO;

//----------------------------------------------------------------------------
// GS -> ES [0x24] : PC방 포인트 시스템 - 포인트를 변경한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정	
	BYTE		btServerType;			// 서버타입 0:본서버 1:테스트서버(FOR_NEW_TESTSERVER)	
	short		nNewPoint;				// 변경된 후 포인트
#ifdef PCBANG_KUZIMO
	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
#endif
} PMSG_REQ_PCBANG_POINT_NEW_INFO, *LPPMSG_REQ_PCBANG_POINT_NEW_INFO;

//----------------------------------------------------------------------------
// ES -> GS [0x24] : PC방 포인트 시스템 - 변경된 포인트 정보의 결과값을 알린다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	BYTE		btResult;				// 0:실패 1:성공
} PMSG_ANS_PCBANG_POINT_NEW_INFO, *LPPMSG_ANS_PCBANG_POINT_NEW_INFO;

//----------------------------------------------------------------------------
// GS -> ES [0x25] : PC방 포인트 시스템 - 랭킹이벤트 : 누적 사용 포인트로 웹에서 순위를 선정하는 이벤트
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	BYTE		btServerType;			// 서버타입 0:본서버 1:테스트서버(FOR_NEW_TESTSERVER)	
	short		nUsePoint;				// 사용 포인트
} PMSG_PCBANG_POINT_RANKING_EVENT, *LPPMSG_PCBANG_POINT_RANKING_EVENT;
#endif // PCBANG_POINT_SYSTEM_20070206

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

//----------------------------------------------------------------------------
//GS -> ES [0x26] : 게임서버에서 랭킹서버에 환영 사원 점수를 보냄
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [11];
	CHAR		szGameID [11];
	WORD		wServerCode;
	INT			nTempleNumber;		// 사원 번호
	INT			nClass;				// 클래스 
	INT			nLevel;				// 레벨
	BYTE		btWin;				// 승/패
	INT			nPCBangGuid;		// PC방 여부
	INT			nRankPoint;			// 랭킹 포인트
} PMSG_ANS_ILLUSION_TEMPLE_SCORE, * LPPMSG_ANS_ILLUSION_TEMPLE_SCORE;
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823		// EventServer Protocol Define
//////////////////////////////////////////////////////////////////////////////
/*                     ITEM GIVE NPC ALAN PROTOCOL							*/ 
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// GS -> ES [0x27] : NPC 앨런 아이템지급 이벤트 - 아이템 지급이 가능한지 확인
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_CHECK_ALANS_TARGET, *LPPMSG_REQ_CHECK_ALANS_TARGET;

//----------------------------------------------------------------------------
// ES -> GS [0x27] : NPC 앨런 아이템지급 이벤트 - 아이템 지급 가능/불가능에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 이미 줬음
										//			0x01 : 아이템 줘도 됨
										//			0x02 : 에러
} PMSG_ANS_CHECK_ALANS_TARGET, *LPPMSG_ANS_CHECK_ALANS_TARGET;

//----------------------------------------------------------------------------
// GS -> ES [0x28] : NPC 앨런 아이템지급 이벤트 - 아이템 지급 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
} PMSG_REQ_ALANS_ITEM, *LPPMSG_REQ_ALANS_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x21] : NPC 앨런 아이템지급 이벤트 - 아이템 지급 요청에 대한 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	WORD		wServerCode;			// 서버코드
	BYTE		iResultCode;			// 결과코드 0x00 : 실패
										//			0x01 : 성공													
} PMSG_ANS_ALANS_ITEM, *LPPMSG_ANS_ALANS_ITEM;
#endif	// ADD_ITEM_GIVE_NPC_ALAN_20070823

#ifdef PCBANG_KUZIMO
//----------------------------------------------------------------------------
// GS -> ES [0x29] : PC방 포인트 시스템 - 포인트 정보를 요청한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	BYTE		btServerType;			// 서버타입 0:본서버 1:테스트서버(FOR_NEW_TESTSERVER)	
	CHAR		szUID[11];				// 사용자 계정
} PMSG_REQ_PCBANG_POINT_INFO2, *LPPMSG_REQ_PCBANG_POINT_INFO2;

//----------------------------------------------------------------------------
// ES -> GS [0x29] : PC방 포인트 시스템 - 포인트 정보를 되돌려준다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	short		nPoint;					// PC방 포인트

	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
} PMSG_ANS_PCBANG_POINT_INFO2, *LPPMSG_ANS_PCBANG_POINT_INFO2;

//----------------------------------------------------------------------------
// GS -> ES [0x30] : PC방 포인트 시스템 - 포인트를 변경한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정	
	BYTE		btServerType;			// 서버타입 0:본서버 1:테스트서버(FOR_NEW_TESTSERVER)	
	short		nNewPoint;				// 변경된 후 포인트

	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
} PMSG_REQ_PCBANG_POINT_NEW_INFO2, *LPPMSG_REQ_PCBANG_POINT_NEW_INFO2;

//----------------------------------------------------------------------------
// ES -> GS [0x30] : PC방 포인트 시스템 - 변경된 포인트 정보의 결과값을 알린다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// 사용자 계정
	BYTE		btResult;				// 0:실패 1:성공
} PMSG_ANS_PCBANG_POINT_NEW_INFO2, *LPPMSG_ANS_PCBANG_POINT_NEW_INFO2;
#endif


#ifdef ADD_PAYITEM_20081030

//----------------------------------------------------------------------------
// DBTASK_REQ_GET_PAYITEM
// 아이템지급 이벤트 - 아이템 지급 대상인지 확인
//----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_GET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// 사용자 계정
	WORD		wServerCode;		// 서버코드
	short		nEventID;			// 이벤트 아이디(0x01: 크리스마스)
} PMSG_REQ_GET_PAYITEM, *LPPMSG_REQ_GET_PAYITEM;
typedef struct _tagPMSG_ANS_GET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// 사용자 계정
	WORD		wServerCode;		// 서버코드
	short		nEventID;			// 이벤트 아이디(0x01: 크리스마스)
	BYTE		bFirst;				// 최초 사용자 유/무(TRUE/FALSE)
	BYTE		nResult;
} PMSG_ANS_GET_PAYITEM, *LPPMSG_ANS_GET_PAYITEM;

//----------------------------------------------------------------------------
// DBTASK_ANS_SET_PAYITEM
// 아이템지급 이벤트 - 아이템 지급 요청
//----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_SET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// 사용자 계정
	WORD		wServerCode;		// 서버코드
	short		nEventID;			// 이벤트 아이디(0x01: 크리스마스)
} PMSG_REQ_SET_PAYITEM, *LPPMSG_REQ_SET_PAYITEM;
typedef struct _tagPMSG_ANS_SET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// 사용자 계정
	WORD		wServerCode;		// 서버코드
	short		nEventID;			// 이벤트 아이디(0x01: 크리스마스)
	BYTE		bFirst;				// 최초 사용자 유/무(TRUE/FALSE)
	BYTE		nResult;
} PMSG_ANS_SET_PAYITEM, *LPPMSG_ANS_SET_PAYITEM;

#endif // ADD_PAYITEM_20081030

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
//----------------------------------------------------------------------------
// GS -> ES [0x33] : 등록된 동전 개수 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
} PMSG_ES_REQ_GET_COIN_COUNT, *LPPMSG_ES_REQ_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// ES -> GS [0x33] : 등록된 동전 개수 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	int			nCurCoinCnt;	// 현재 등록된 동전수 
} PMSG_ES_ANS_GET_COIN_COUNT, *LPPMSG__ES_ANS_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// GS -> ES [0x34] : 동전등록
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	BYTE		btPos;						// 동전 위치
} PMSG_ES_REQ_REG_COIN, *LPPMSG_ES_REQ_REG_COIN;

//----------------------------------------------------------------------------
// ES -> GS [0x34] : 동전 등록 응답
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	BYTE		btResult;					// 결과
	int			nCurCoinCnt;				// 현재 등록된 동전수
	BYTE		btPos;						// 동전 위치
} PMSG_ES_ANS_REG_COIN, *LPPMSG_ES_ANS_REG_COIN;

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029


#endif

