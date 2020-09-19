#ifdef ADD_NEWPVP_PKFIELD

//#define	MASK_NEWPVP				0xAA00
#define	MASK_NEWPVP				0xAA
#define MASK_NEWPVP_SUBCODE		0x00

#define DUEL_CHANNEL_MAX		4
#define DUEL_OBSERVER_MAX		10

enum _MAP_NEWPVP_PROTOCOL
{
	MSG_DUEL_INVITE							= ((MASK_NEWPVP << 8) | MASK_NEWPVP_SUBCODE | 1), 
											// ������û
	MSG_DUEL_ANSWER, 						// ������û ����
	MSG_DUEL_LEAVE, 						// ��������
	
	MSG_DUEL_SCORE_BROADCAST,				// ������ ���� ����
	MSG_DUEL_HP_BROADCAST,					// ������ ���� ����
	
	MSG_DUEL_CHANNELLIST,					// ���� ä��(����) ����Ʈ
	MSG_DUEL_JOINCNANNEL,					// ä��(����) ��û
	MSG_DUEL_JOINCNANNEL_BROADCAST,			// ä��(����) ������ ������
	MSG_DUEL_LEAVECNANNEL,					// ä��(����) ����
	MSG_DUEL_LEAVECNANNEL_BROADCAST,		// ä��(����) ���� ������
	MSG_DUEL_OBSERVERLIST_BROADCAST,		// ������ ����Ʈ(All)
	MSG_DUEL_RESULT_BROADCAST,				// ������� ����
	MSG_DUEL_ROUNDSTART_BROADCAST,			// ���� ����/�� �˸�
};

namespace ENEWPVP
{
	enum EERROR
	{
		E_NO_ERROR				= 0x00,		// ����
		E_FAILED_RESULT,					// ����
		E_INVALID_USER,						// �߸��� ����
		E_NON_PKSERVER,						// NON-PK ����
		E_NOT_DUEL_CSSERVER,				// �������� ����, ���� �Ұ�
		E_CONNECTION_CLOSING,				// ����� ������ ������
		E_NOT_DUEL_MURDERER,				// �����ڴ� ���� ��û ����
		E_ALREADY_DUELLED,					// ������ �̹� ������ ��û�ߴٸ�
		E_INVALID_MAP,						// ���� �Ұ����� �ʿ����� ��û(ī����ĳ��, �̺�Ʈ ��)
		E_LIMIT_PACKETTIME,					// ����� ��Ŷ
		E_INVITE_MYSELF,					// �ڽ����� ���� ��û �Ұ�
		E_INVALID_INDEX,					// �߸��� ����� �ε���
		E_DISCONNECT,						// Ŭ���̾�Ʈ ������
		E_SELFDEFENSE,						// ������� �����
		E_GUILDWAR,							// ���� �߿� ���� �Ұ�
		E_REFUSE_INVATE,					// ������ ���� �ź�
		E_DUEL_MAX,							// �ش� ������ Full(���� ��û)
		E_INVALID_STATUS,					// �߸��� Channel ����(��: ��� �������� ���� ��û)
		E_ALREADY_DUELREQUESTED,			// �̹� ������û�� �޾Ҵٸ�
		E_ALREADY_DUELRESERVED,				// ��밡 ���� ��û���� ��
		E_ALREADY_DUELLING,					// �̹� ������ �ϰ� �ִٸ�
		E_ALREADY_DUELREQUESTED_1,			// �̹� ������û�� �޾Ҵٸ�(����)
		E_ALREADY_DUELRESERVED_1,			// ��밡 ���� ��û���� ��(����)
		E_ALREADY_DUELLING_1,				// ������ �̹� ������ �ϰ� �ִٸ�(����)
		E_INVALID_CHANNELID,				// �߸��� ä�ξ��̵�
		E_FAILED_ENTER,						// ���� ���忡 ����
		E_NOT_EXIST_USER,					// �������� �ʴ� �����
		E_OBSERVER_MAX,						// �ش� ä�ο� �����ڰ� Ǯ�� ���
		E_LIMIT_LEVEL,						// �������ѿ� �ɸ�
		E_NOT_FOUND_MOVEREQUESTDATA,		// 
		E_NOT_ENOUGH_MONEY,					// ���� ���ڸ���
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
	BYTE		NumberH;				// ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;				// ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[MAX_IDSTRING];	// ���� �ɸ��̸�
} PMSG_REQ_DUEL_INVITE, *LPPMSG_REQ_DUEL_INVITE;
typedef struct _tagPMSG_ANS_DUEL_INVITE
{
	PBMSG_HEAD2	h;
	BYTE		nResult;
	BYTE		NumberH;				// ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;				// ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[MAX_IDSTRING];	// ���� �ɸ��̸�
} PMSG_ANS_DUEL_INVITE, *LPPMSG_ANS_DUEL_INVITE;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_ANSWER(0xAC)
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_ANSWER
{
	PBMSG_HEAD2	h;
	BYTE		NumberH;				// ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;				// ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[MAX_IDSTRING];	// ���� �ɸ��̸�
} PMSG_REQ_DUEL_ANSWER, *LPPMSG_REQ_DUEL_ANSWER;
typedef struct _tagPMSG_ANS_DUEL_ANSWER
{
	PBMSG_HEAD2	h;
	BYTE		bDuelOK;				// ����Ȯ�� ��û�� ���� ���� (0:�ƴϿ� / 1:��)
	BYTE		NumberH;				// ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;				// ���� ������� �ε��� (���� ����Ʈ)
	//CHAR		szName[MAX_IDSTRING];	// ���� �ɸ��̸�
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
	BYTE		NumberH;				// ���� ������� �ε��� (���� ����Ʈ)
	BYTE		NumberL;				// ���� ������� �ε��� (���� ����Ʈ)
	CHAR		szName[MAX_IDSTRING];	// ���� �ɸ��̸�
} PMSG_ANS_DUEL_LEAVE, *LPPMSG_ANS_DUEL_LEAVE;




//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_SCORE_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_SCORE_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		NumberH1;				// ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberL1;				// ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberH2;				// ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		NumberL2;				// ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		btDuelScore1;			// �������� - 1��
	BYTE		btDuelScore2;			// �������� - 2��
} PMSG_DUEL_SCORE_BROADCAST, *LPPMSG_DUEL_SCORE_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_HP_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_HP_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		NumberH1;				// ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberL1;				// ���� ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberH2;				// ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		NumberL2;				// ���� ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		nHP1;					// ���� - 1��(%)
	BYTE		nHP2;					// ���� - 2��(%)
	BYTE		nShield1;				// ����1
	BYTE		nShield2;				// ����2
} PMSG_DUEL_HP_BROADCAST, *LPPMSG_DUEL_HP_BROADCAST;




//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_CHANNELLIST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_ANS_DUEL_CHANNELLIST
{
	PBMSG_HEAD2	h;
	struct {
		CHAR	szName1[MAX_IDSTRING];	// �ɸ��̸�
		CHAR	szName2[MAX_IDSTRING];	// �ɸ��̸�
		BYTE	bStart;					// PK ���� ��/��
		BYTE	bWatch;					// �������� ��/��
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
	BYTE		nChannelId;				// ä�� ���̵�
	CHAR		szName1[MAX_IDSTRING];	// �ɸ��̸�
	CHAR		szName2[MAX_IDSTRING];	// �ɸ��̸�
	BYTE		NumberH1;				// ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberL1;				// ������� �ε��� (���� ����Ʈ) - 1��
	BYTE		NumberH2;				// ������� �ε��� (���� ����Ʈ) - 2��
	BYTE		NumberL2;				// ������� �ε��� (���� ����Ʈ) - 2��
} PMSG_ANS_DUEL_JOINCNANNEL, *LPPMSG_ANS_DUEL_JOINCNANNEL;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_JOINCNANNEL_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_JOINCNANNEL_BROADCAST
{
	PBMSG_HEAD2	h;
	CHAR		szName[MAX_IDSTRING];	// ä�ο� ������ �ɸ��̸�
} PMSG_DUEL_JOINCNANNEL_BROADCAST, *LPPMSG_DUEL_JOINCNANNEL_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_LEAVECNANNEL
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_DUEL_LEAVECNANNEL
{
	PBMSG_HEAD2	h;
	BYTE		nChannelId;				// ä�� ���̵�
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
	CHAR		szName[MAX_IDSTRING];	// ä���� ���� �ɸ��̸�
} PMSG_DUEL_LEAVECNANNEL_BROADCAST, *LPPMSG_DUEL_LEAVECNANNEL_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_OBSERVERLIST_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_OBSERVERLIST_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		nCount;
	struct {
		CHAR	szName[MAX_IDSTRING];	// �ɸ��̸�
	} user[DUEL_OBSERVER_MAX];
} PMSG_DUEL_OBSERVERLIST_BROADCAST, *LPPMSG_DUEL_OBSERVERLIST_BROADCAST;


//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_RESULT_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_RESULT_BROADCAST
{
	PBMSG_HEAD2	h;
	CHAR		szWinner[MAX_IDSTRING];	// ���� �ɸ��̸�
	CHAR		szLoser[MAX_IDSTRING];	// ���� �ɸ��̸�
} PMSG_DUEL_RESULT_BROADCAST, *LPPMSG_DUEL_RESULT_BROADCAST;

//-----------------------------------------------------------------------------
//! @brief	MSG_DUEL_ROUNDSTART_BROADCAST
//-----------------------------------------------------------------------------
typedef struct _tagPMSG_DUEL_ROUNDSTART_BROADCAST
{
	PBMSG_HEAD2	h;
	BYTE		nFlag;					// 0: ���� ����, 1: ���� ����
} PMSG_DUEL_ROUNDSTART_BROADCAST, *LPPMSG_DUEL_ROUNDSTART_BROADCAST;

#pragma pack(pop)

#endif // ADD_NEWPVP_PKFIELD