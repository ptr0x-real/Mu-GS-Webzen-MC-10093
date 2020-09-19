#ifdef ADD_NEWPVP_PKFIELD

//#define	MASK_NEWPVP				0xAA00
#define	MASK_NEWPVP				0xAA
#define MASK_NEWPVP_SUBCODE		0x00

#define DUEL_CHANNEL_MAX		4
#define DUEL_OBSERVER_MAX		10

enum _MAP_NEWPVP_PROTOCOL
{
	MSG_DUEL_INVITE							= ((MASK_NEWPVP << 8) | MASK_NEWPVP_SUBCODE | 1), 
											// 결투요청
	MSG_DUEL_ANSWER, 						// 결투요청 응답
	MSG_DUEL_LEAVE, 						// 결투종료
	
	MSG_DUEL_SCORE_BROADCAST,				// 결투자 점수 정보
	MSG_DUEL_HP_BROADCAST,					// 결투자 생명 정보
	
	MSG_DUEL_CHANNELLIST,					// 결투 채널(관전) 리스트
	MSG_DUEL_JOINCNANNEL,					// 채널(관전) 요청
	MSG_DUEL_JOINCNANNEL_BROADCAST,			// 채널(관전) 입장한 관전자
	MSG_DUEL_LEAVECNANNEL,					// 채널(관전) 종료
	MSG_DUEL_LEAVECNANNEL_BROADCAST,		// 채널(관전) 떠난 관전자
	MSG_DUEL_OBSERVERLIST_BROADCAST,		// 관전자 리스트(All)
	MSG_DUEL_RESULT_BROADCAST,				// 결투결과 정보
	MSG_DUEL_ROUNDSTART_BROADCAST,			// 라운드 시작/끝 알림
};

namespace ENEWPVP
{
	enum EERROR
	{
		E_NO_ERROR				= 0x00,		// 성공
		E_FAILED_RESULT,					// 에러
		E_INVALID_USER,						// 잘못된 유저
		E_NON_PKSERVER,						// NON-PK 서버
		E_NOT_DUEL_CSSERVER,				// 공성전중 결투, 길전 불가
		E_CONNECTION_CLOSING,				// 사용자 게임을 종료중
		E_NOT_DUEL_MURDERER,				// 무법자는 결투 신청 못함
		E_ALREADY_DUELLED,					// 이전에 이미 결투를 신청했다면
		E_INVALID_MAP,						// 결투 불가능한 맵에서의 요청(카오스캐슬, 이벤트 맵)
		E_LIMIT_PACKETTIME,					// 만료된 패킷
		E_INVITE_MYSELF,					// 자신한테 결투 요청 불가
		E_INVALID_INDEX,					// 잘못된 사용자 인덱스
		E_DISCONNECT,						// 클라이언트 종료중
		E_SELFDEFENSE,						// 정당방위 사용자
		E_GUILDWAR,							// 길드워 중엔 결투 불가
		E_REFUSE_INVATE,					// 상대방이 결투 거부
		E_DUEL_MAX,							// 해당 결투장 Full(결투 신청)
		E_INVALID_STATUS,					// 잘못된 Channel 상태(예: 경기 시작전에 관전 요청)
		E_ALREADY_DUELREQUESTED,			// 이미 결투신청을 받았다면
		E_ALREADY_DUELRESERVED,				// 상대가 결투 요청중일 때
		E_ALREADY_DUELLING,					// 이미 결투를 하고 있다면
		E_ALREADY_DUELREQUESTED_1,			// 이미 결투신청을 받았다면(상대방)
		E_ALREADY_DUELRESERVED_1,			// 상대가 결투 요청중일 때(상대방)
		E_ALREADY_DUELLING_1,				// 이전에 이미 결투를 하고 있다면(상대방)
		E_INVALID_CHANNELID,				// 잘못된 채널아이디
		E_FAILED_ENTER,						// 관전 입장에 실패
		E_NOT_EXIST_USER,					// 존재하지 않는 사용자
		E_OBSERVER_MAX,						// 해당 채널에 관전자가 풀인 경우
		E_LIMIT_LEVEL,						// 레벨제한에 걸림
		E_NOT_FOUND_MOVEREQUESTDATA,		// 
		E_NOT_ENOUGH_MONEY,					// 돈이 모자르다
	};
};

#define E_SUCCESSED(r)	(r==0)
#define E_FAILED(r)		(r!=0)




#pragma pack(push,1)
//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_INVITE(0xAA)
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_INVITE
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;				// 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;				// 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[MAX_IDSTRING];	// 상대방 케릭이름
} PMSG_REQ_DUEL_INVITE, *LPPMSG_REQ_DUEL_INVITE;
typedef struct _tagPMSG_ANS_DUEL_INVITE
{
	PBMSG_HEAD2	h;
	BYTE		nResult;
	BYTE		NumberH;				// 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;				// 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[MAX_IDSTRING];	// 상대방 케릭이름
} PMSG_ANS_DUEL_INVITE, *LPPMSG_ANS_DUEL_INVITE;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_ANSWER(0xAC)
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_ANSWER
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;				// 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;				// 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[MAX_IDSTRING];	// 상대방 케릭이름
} PMSG_REQ_DUEL_ANSWER, *LPPMSG_REQ_DUEL_ANSWER;
typedef struct _tagPMSG_ANS_DUEL_ANSWER
{
	PBMSG_HEAD2	h;
	BYTE		bDuelOK;				// 결투확인 요청에 대한 응답 (0:아니요 / 1:예)
	BYTE		NumberH;				// 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;				// 상대방 사용자의 인덱스 (하위 바이트)
	//CHAR		szName[MAX_IDSTRING];	// 상대방 케릭이름
} PMSG_ANS_DUEL_ANSWER, *LPPMSG_ANS_DUEL_ANSWER;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_LEAVE(0xAB)
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_EXIT
{
	PBMSG_HEAD2	h;
} PMSG_REQ_DUEL_LEAVE, *LPPMSG_REQ_DUEL_LEAVE;
typedef struct _tagPMSG_ANS_DUEL_EXIT
{
	PBMSG_HEAD2	h;
	BYTE		nResult;
	BYTE		NumberH;				// 상대방 사용자의 인덱스 (상위 바이트)
	BYTE		NumberL;				// 상대방 사용자의 인덱스 (하위 바이트)
	CHAR		szName[MAX_IDSTRING];	// 상대방 케릭이름
} PMSG_ANS_DUEL_LEAVE, *LPPMSG_ANS_DUEL_LEAVE;




//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_SCORE_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_SCORE_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		NumberH1;				// 상대방 사용자의 인덱스 (상위 바이트) - 1번
	BYTE		NumberL1;				// 상대방 사용자의 인덱스 (하위 바이트) - 1번
	BYTE		NumberH2;				// 상대방 사용자의 인덱스 (상위 바이트) - 2번
	BYTE		NumberL2;				// 상대방 사용자의 인덱스 (하위 바이트) - 2번
	BYTE		btDuelScore1;			// 결투점수 - 1번
	BYTE		btDuelScore2;			// 결투점수 - 2번
} PMSG_DUEL_SCORE_BROADCAST, *LPPMSG_DUEL_SCORE_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_HP_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_HP_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		NumberH1;				// 상대방 사용자의 인덱스 (상위 바이트) - 1번
	BYTE		NumberL1;				// 상대방 사용자의 인덱스 (하위 바이트) - 1번
	BYTE		NumberH2;				// 상대방 사용자의 인덱스 (상위 바이트) - 2번
	BYTE		NumberL2;				// 상대방 사용자의 인덱스 (하위 바이트) - 2번
	BYTE		nHP1;					// 생명 - 1번(%)
	BYTE		nHP2;					// 생명 - 2번(%)
	BYTE		nShield1;				// 쉴드1
	BYTE		nShield2;				// 쉴드2
} PMSG_DUEL_HP_BROADCAST, *LPPMSG_DUEL_HP_BROADCAST;




//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_CHANNELLIST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_ANS_DUEL_CHANNELLIST
{
	PBMSG_HEAD2	h;
	struct {
		CHAR	szName1[MAX_IDSTRING];	// 케릭이름
		CHAR	szName2[MAX_IDSTRING];	// 케릭이름
		BYTE	bStart;					// PK 시작 유/무
		BYTE	bWatch;					// 관전가능 유/무
	} channel[DUEL_CHANNEL_MAX];
} PMSG_ANS_DUEL_CHANNELLIST, *LPPMSG_ANS_DUEL_CHANNELLIST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_JOINCNANNEL
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_JOINCNANNEL
{
	PBMSG_HEAD2	h;
	BYTE		nChannelId;
} PMSG_REQ_DUEL_JOINCNANNEL, *LPPMSG_REQ_DUEL_JOINCNANNEL;
typedef struct _tagPMSG_ANS_DUEL_JOINCNANNEL
{
	PBMSG_HEAD2	h;
	BYTE		nResult;
	BYTE		nChannelId;				// 채널 아이디
	CHAR		szName1[MAX_IDSTRING];	// 케릭이름
	CHAR		szName2[MAX_IDSTRING];	// 케릭이름
	BYTE		NumberH1;				// 사용자의 인덱스 (상위 바이트) - 1번
	BYTE		NumberL1;				// 사용자의 인덱스 (하위 바이트) - 1번
	BYTE		NumberH2;				// 사용자의 인덱스 (상위 바이트) - 2번
	BYTE		NumberL2;				// 사용자의 인덱스 (하위 바이트) - 2번
} PMSG_ANS_DUEL_JOINCNANNEL, *LPPMSG_ANS_DUEL_JOINCNANNEL;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_JOINCNANNEL_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_JOINCNANNEL_BROADCAST
{
	PBMSG_HEAD2	h;
	CHAR		szName[MAX_IDSTRING];	// 채널에 입장한 케릭이름
} PMSG_DUEL_JOINCNANNEL_BROADCAST, *LPPMSG_DUEL_JOINCNANNEL_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_LEAVECNANNEL
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_LEAVECNANNEL
{
	PBMSG_HEAD2	h;
	BYTE		nChannelId;				// 채널 아이디
} PMSG_REQ_DUEL_LEAVECNANNEL, *LPPMSG_REQ_DUEL_LEAVECNANNEL;
typedef struct _tagPMSG_ANS_DUEL_LEAVECNANNEL
{
	PBMSG_HEAD2	h;
	BYTE		nResult;
} PMSG_ANS_DUEL_LEAVECNANNEL, *LPPMSG_ANS_DUEL_LEAVECNANNEL;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_LEAVECNANNEL_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_LEAVECNANNEL_BROADCAST
{
	PBMSG_HEAD2	h;
	CHAR		szName[MAX_IDSTRING];	// 채널을 떠난 케릭이름
} PMSG_DUEL_LEAVECNANNEL_BROADCAST, *LPPMSG_DUEL_LEAVECNANNEL_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_OBSERVERLIST_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_OBSERVERLIST_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		nCount;
	struct {
		CHAR	szName[MAX_IDSTRING];	// 케릭이름
	} user[DUEL_OBSERVER_MAX];
} PMSG_DUEL_OBSERVERLIST_BROADCAST, *LPPMSG_DUEL_OBSERVERLIST_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_RESULT_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_RESULT_BROADCAST
{
	PBMSG_HEAD2	h;
	CHAR		szWinner[MAX_IDSTRING];	// 승자 케릭이름
	CHAR		szLoser[MAX_IDSTRING];	// 패자 케릭이름
} PMSG_DUEL_RESULT_BROADCAST, *LPPMSG_DUEL_RESULT_BROADCAST;

//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_ROUNDSTART_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_ROUNDSTART_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		nFlag;					// 0: 라운드 시작, 1: 라운드 종료
} PMSG_DUEL_ROUNDSTART_BROADCAST, *LPPMSG_DUEL_ROUNDSTART_BROADCAST;

#pragma pack(pop)

#endif // ADD_NEWPVP_PKFIELD