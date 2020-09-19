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
		EVENT_XMAS							= EVENT_PAYITEM_START,	// ũ�������� �̺�Ʈ ������ ����
		EVENT_PAYITEM_END					= EVENT_XMAS, 
};

//�̺�Ʈ ������ ���� ����
namespace EPAYITEM
{
	enum EERROR
	{
		E_NO_ERROR						= 0x00, 
			E_INVALID_EVENTID, 
			E_ALREADY_SETITEM,				// �̹� ���޵Ǿ���
			E_FAILED_RESULT,				// ����
	};
};

#define IS_PAYITEMEVENT(t)				(EVENT_PAYITEM_START <= t && EVENT_PAYITEM_END >= t)

#endif // EVENTDB_PROTOCOLEX

//----------------------------------------------------------------------------
// [0x01] ��ǰ ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	
	char		AccountId[MAX_IDSTRING];		// ���� ���̵�
	char		GameId[MAX_IDSTRING];			// ĳ���� �̸�
	int			Sequence;						// ���Ӽ��� Ȯ�ο� ����

} CB_PMSG_REQ_PRESENT, * LPCB_PMSG_REQ_PRESENT;

//----------------------------------------------------------------------------
// [0x01] ��ǰ ��û ���
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	

	char		AccountId[MAX_IDSTRING];		// ���� ���̵�
	char		GameId[MAX_IDSTRING];			// ĳ���� �̸�
	int			Sequence;						// ���Ӽ��� Ȯ�ο� ����
	
	char		present;		// �̺�Ʈ ��ǰ ��ȣ 
								// 0 : ����ũ ���� �����
								// 1 : ��Ƽ�� 4
								// 2 : ������ ī�޶�
								// 3 : �ٸ��콺 + Ű����
								// 4 : 256��
								// 5 : ���� ���� 6���� ���� ������
								// 6 : ��ȭ��ǰ��
								// 7 : �� �ӱ���
								// 8 : �� T ����
								// 9 : �� 10�ð� ���� �̿��
								// -1 : �� -_-;;
	char		presentname[MAX_PRESENTNAME];	// �ִ� ��ǰ �̸�
	int			Seq_present;	// ��ǰ �ε���

} CB_PMSG_REQ_PRESENT_RESULT, * LPCB_PMSG_REQ_PRESENT_RESULT;

//----------------------------------------------------------------------------
// [0x02] ��ǰ ��û�� ����
typedef struct
{
	PBMSG_HEAD	h;

	char		AccountID[MAX_IDSTRING];
	char		GameID[MAX_IDSTRING];
	int			servernumber;

	char		present;		// �̺�Ʈ ��ǰ ��ȣ
	int			Seq_present;	// ��ǰ �ε���
} PMSG_PRIZE_INFO, * LPPMSG_PRIZE_INFO;


//----------------------------------------------------------------------------
// [0x03] ��ǰ ���� ���̺� �ʱ�ȭ
typedef struct
{
	PBMSG_HEAD	h;
	int			MaxBoxSize;		// ��ǰ������ �ִ� ũ��
} PMSG_RESET_PRESENTBOX, * LPPMSG_RESET_PRESENTBOX;

//----------------------------------------------------------------------------
// [0x04] ��ǰ ��Ȳ ����
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// ��ǰ ��ȣ
} PMSG_REQ_PRESENTCOUNT, * LPPMSG_REQ_PRESENTCOUNT;


//----------------------------------------------------------------------------
// [0x04] ��ǰ ��Ȳ ���
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// ��ǰ ��ȣ
	int			pcount;			// ��ǰ ����
} PMSG_PRESENTCOUNT_INFO, * LPPMSG_PRESENTCOUNT_INFO;

//----------------------------------------------------------------------------
// [0x05] ��ǰ ��Ȳ ����
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// ��ǰ ��ȣ
	int			pcount;			// ��ǰ ����
} PMSG_EDIT_PRESENTCOUNT, * LPPMSG_EDIT_PRESENTCOUNT;

//----------------------------------------------------------------------------
// [0x05] ��ǰ ��Ȳ �������
typedef struct
{
	PBMSG_HEAD	h;
	char		present;		// ��ǰ ��ȣ
	int			pcount;			// ��ǰ ����
} PMSG_EDIT_PRESENTCOUNT_RESULT, * LPPMSG_EDIT_PRESENTCOUNT_RESULT;



//////////////////////////////////////////////////////////////////////////////
// MU RANKING SERVER PROTOCOL
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// S -> C [0x01] : ���Ӽ������� ��ŷ������ ���������� ������ ������
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
// S -> C [0x02] : ���Ӽ������� ���������� �̺�Ʈ�� ����Ǿ����� �˸�
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	INT ServerCode;
} PMSG_ANS_EVENTCLOSED, * LPPMSG_ANS_EVENTCLOSED;

//----------------------------------------------------------------------------
// S -> C [0x03] : ���Ӽ������� ��ŷ������ ���������� ������ ������
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
} PMSG_ANS_EVENTLIVE, * LPPMSG_ANS_EVENTLIVE;

#ifdef FOR_BLOODCASTLE

/////////////////////////////////////////////////////////////////////////////
// ��ŷ�������� �������� (���� ĳ��)
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// S -> C [0x07] : ���Ӽ������� ��ŷ������ ����ĳ�� ������ ������
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
// C -> S [0x0A] : ���Ӽ������� ��ŷ������ ����ĳ�� ������ ������ (�����ð� ���ؼ�)
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
// C -> S [0x0B] : ���Ӽ������� ��ŷ������ Ư�� ������ ����, �ɸ��� ���Ͽ� ����ĳ���� ���� ����Ƚ���� ��û
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_BLOODCASTLE_ENTERCOUNT, * LPPMSG_REQ_BLOODCASTLE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x0B] : ���Ӽ������� Ư�� ������ ����, �ɸ��� ���� ����ĳ�� ����Ƚ���� ����
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
// ����ĳ�� �Ϸ� ����� ����

//----------------------------------------------------------------------------
// C -> S [0x08] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ����ĳ�� �Ϸ� ����Ƚ�� üũ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	INT		iItemPos;		// ������� ��ġ
	INT		iItemLevel;		// ������� ����
	INT		iBridgeNumber;	// �ٸ���ȣ
} PMSG_REQ_BLOODCASTLEENTERCHECK, * LPPMSG_REQ_BLOODCASTLEENTERCHECK;


//----------------------------------------------------------------------------
// S -> C [0x08] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ����ĳ�� �Ϸ� ����Ƚ�� üũ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	BOOL	bEnterCheck;	// 0 - ����Ұ� / 1 - ���尡��
	INT		iItemPos;		// ������� ��ġ
	INT		iItemLevel;		// ������� ����
	INT		iBridgeNumber;	// �ٸ���ȣ
} PMSG_ANS_BLOODCASTLEENTERCHECK, * LPPMSG_ANS_BLOODCASTLEENTERCHECK;


//----------------------------------------------------------------------------
// C -> S [0x09] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ����ĳ�� �Ϸ� ����Ƚ���� �ø���� ��û
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
// ���������� �Ϸ� ����� ����

//----------------------------------------------------------------------------
// C -> S [0x0F] : ���Ӽ������� ��ŷ������ Ư�� ������ ����, �ɸ��� ���Ͽ� ������������ ���� ����Ƚ���� ��û
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_DEVILSQUARE_ENTERCOUNT, * LPPMSG_REQ_DEVILSQUARE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x0F] : ���Ӽ������� Ư�� ������ ����, �ɸ��� ���� ���������� ����Ƚ���� ����
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
// C -> S [0x10] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ���������� �Ϸ� ����Ƚ�� üũ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	INT		iItemPos;		// �ʴ��� ��ġ
	INT		iItemLevel;		// �ʴ��� ����
	INT		iSquareNumber;	// �����ȣ
} PMSG_REQ_DEVILSQUAREENTERCHECK, * LPPMSG_REQ_DEVILSQUAREENTERCHECK;


//----------------------------------------------------------------------------
// S -> C [0x10] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ���������� �Ϸ� ����Ƚ�� üũ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	BOOL	bEnterCheck;	// 0 - ����Ұ� / 1 - ���尡��
	INT		iItemPos;		// �ʴ��� ��ġ
	INT		iItemLevel;		// �ʴ��� ����
	INT		iSquareNumber;	// �����ȣ
} PMSG_ANS_DEVILSQUAREENTERCHECK, * LPPMSG_ANS_DEVILSQUAREENTERCHECK;


//----------------------------------------------------------------------------
// C -> S [0x11] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ���������� �Ϸ� ����Ƚ���� �ø���� ��û
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
// C -> S [0x0D] : ���Ӽ������� ��ŷ������ ����ĳ�� ������ ������ (�����ð� ���ؼ�)
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

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ����� ���� ��ŷ���� �߰�

// �Ϸ� �������� : 6��
//----------------------------------------------------------------------------
// C -> S [0x12] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ȯ�� ����� �Ϸ� ����Ƚ�� üũ�� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	INT		iItemPos;		// ���� �η縶�� ��ġ
	INT		iItemLevel;		// ���� �η縶�� ����
	INT		iTempleNumber;	// ȯ�� ��� ��ȣ
} PMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK, * LPPMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK;


//----------------------------------------------------------------------------
// S -> C [0x12] : ��ŷ�������� ���Ӽ����� Ư�� ����,����,�ɸ��� ȯ�� ����� �Ϸ� ����Ƚ�� üũ�� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD h;
	CHAR	AccountID [MAX_IDSTRING];
	CHAR	GameID [MAX_IDSTRING];
	INT		ServerCode;
	INT		iObjIndex;		// �������� ����ϴ� ����� �ε���
	BOOL	bEnterCheck;	// 0 - ����Ұ� / 1 - ���尡��
	INT		iItemPos;		// ���� �η縶�� ��ġ
	INT		iItemLevel;		// ���� �η縶�� ����
	INT		iTempleNumber;	// ȯ�� ��� ��ȣ
} PMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK, * LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK;


//----------------------------------------------------------------------------
// C -> S [0x13] : ���Ӽ������� ��ŷ������ Ư�� ����,����,�ɸ��� ȯ�� ��� �Ϸ� ����Ƚ���� �ø���� ��û
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
// C -> S [0x14] : ���Ӽ������� ��ŷ������ Ư�� ������ ����, �ɸ��� ���Ͽ� ȯ������� ���� ����Ƚ���� ��û
//----------------------------------------------------------------------------

typedef struct {
	PBMSG_HEAD h;
	CHAR AccountID [MAX_IDSTRING];
	CHAR GameID [MAX_IDSTRING];
	INT ServerCode;
	INT iObjIndex;
} PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT, * LPPMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT;


//----------------------------------------------------------------------------
// S -> C [0x14] : ���Ӽ������� Ư�� ������ ����, �ɸ��� ���� ȯ������� ����Ƚ���� ����
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
// �̺�ƮĨ �̺�Ʈ
//============================================================================
//----------------------------------------------------------------------------
// GS -> ES [0x01] : �ش� ������ ���� �̺�Ʈ Ĩ ������ �����ȣ Ȯ���� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_VIEW_EC_MN, * LPPMSG_REQ_VIEW_EC_MN;

//----------------------------------------------------------------------------
// ES -> GS [0x01] : �ش� ������ ���� �̺�Ʈ Ĩ ������ �����ȣ�� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	
#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			nEVENT_CHIPS;		// ��������� �̺�Ʈ Ĩ ����
#else
	SHORT		nEVENT_CHIPS;		// ��������� �̺�Ʈ Ĩ ����
#endif // EVENTCHIP_DATASIZE_EXPAND_20090217
	
	INT			iMUTO_NUM;			// ������ �����ȣ - 0�̸� ���� ��Ͼʵ�
} PMSG_ANS_VIEW_EC_MN, * LPPMSG_ANS_VIEW_EC_MN;


//----------------------------------------------------------------------------
// GS -> ES [0x02] : �ش� ������ �̺�Ʈ Ĩ ����� ��û�Ѵ�. (�̹� �ִ� �Ϳ� �߰�)
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		Pos;				// ������ ��ġ
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_REGISTER_EVENTCHIP, * LPPMSG_REQ_REGISTER_EVENTCHIP;


//----------------------------------------------------------------------------
// ES -> GS [0x02] : �ش� ������ �̺�Ʈ Ĩ�� ��� ��û�� ���� ���� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		Pos;				// ������ ��ġ
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	
#ifdef EVENTCHIP_DATASIZE_EXPAND_20090217
	INT			nEVENT_CHIPS;		// ��������� �̺�Ʈ Ĩ ����
#else
	SHORT		nEVENT_CHIPS;		// ��������� �̺�Ʈ Ĩ ����
#endif // EVENTCHIP_DATASIZE_EXPAND_20090217
	
} PMSG_ANS_REGISTER_EVENTCHIP, * LPPMSG_ANS_REGISTER_EVENTCHIP;


//----------------------------------------------------------------------------
// GS -> ES [0x03] : �ش� ������ ���� ��ȣ ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_REGISTER_MUTONUM, * LPPMSG_REQ_REGISTER_MUTONUM;

//----------------------------------------------------------------------------
// ES -> GS [0x03] : �ش� ������ ���� ��ȣ ��Ͽ� ���� ����� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	INT			iMUTO_NUM;			// �����ߴٸ� �ݵ�� �� �ִ�.
} PMSG_ANS_REGISTER_MUTONUM, * LPPMSG_ANS_REGISTER_MUTONUM;


//----------------------------------------------------------------------------
// GS -> ES [0x04] : �ش� ������ �̺�Ʈ Ĩ ������ 0���� �����.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_RESET_EVENTCHIP, * LPPMSG_REQ_RESET_EVENTCHIP;

//----------------------------------------------------------------------------
// ES -> GS [0x04] : �ش� ������ �̺�Ʈ Ĩ ������ 0���� ���� ����� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
} PMSG_ANS_RESET_EVENTCHIP, * LPPMSG_ANS_RESET_EVENTCHIP;

#ifdef FOR_BLOODCASTLE

//----------------------------------------------------------------------------
// GS -> ES [0x05] : �ش� ������ ���� ���� ���� Ȯ���� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_VIEW_STONES, * LPPMSG_REQ_VIEW_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x06] : �ش� ������ ���� �߰� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// ������ ��ġ
	CHAR		szUID [11];			// ����� ����
} PMSG_REQ_REGISTER_STONES, * LPPMSG_REQ_REGISTER_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x07] : �ش� ������ ���� ������ ��� (������ ����)�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	INT			iStoneCount;		// ���ҵ� ���氳��
} PMSG_REQ_DELETE_STONES, * LPPMSG_REQ_DELETE_STONES;


//============================================================================
//============================================================================


//----------------------------------------------------------------------------
// ES -> GS [0x05] : �ش� ������ ���� ���� ������ ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	INT			iStoneCount;
} PMSG_ANS_VIEW_STONES, * LPPMSG_ANS_VIEW_STONES;


//----------------------------------------------------------------------------
// ES -> GS [0x06] : �ش� ������ �����߰� ����� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// ������ ��ġ
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	INT			iStoneCount;		// ��������� ���氳��
} PMSG_ANS_REGISTER_STONES, * LPPMSG_ANS_REGISTER_STONES;


//----------------------------------------------------------------------------
// ES -> GS [0x07] : �ش� ������ ������� ����� ������.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		bSUCCESS;			// 0-���� / 1-�̰�
	INT			iStoneCount;		// ��������� ���氳��
} PMSG_ANS_DELETE_STONES, * LPPMSG_ANS_DELETE_STONES;


#endif


#if defined ( FOR_KOREA ) || defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO ) // �ѱ� ����ȭ 2�ֳ� ��� ���� �̺�Ʈ

//----------------------------------------------------------------------------
// GS -> ES [0x08] : �ش� �ø����� ����Ѵٰ� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	CHAR		SERIAL1[5];			// �ø���1
	CHAR		SERIAL2[5];			// �ø���2
	CHAR		SERIAL3[5];			// �ø���3
	INT			iMEMB_GUID;			// ����� GUID
} PMSG_REQ_2ANIV_SERIAL, * LPPMSG_REQ_2ANIV_SERIAL;


//----------------------------------------------------------------------------
// ES -> GS [0x08] : �ش� �ø����� ��ϰ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	BYTE		btIsRegistered;		// 0:�̸� ������ ������� (����) / 1:�ٸ������ ������� (�Ʒ� �ʵ忡 ��������) / 2:�������Ƚ���� / 3:�������� �ʴ� �ø��� / 4:�˼����¿��� 
	INT			iGiftNumber;		// ��ǰ��ȣ (������ �ؽ�Ʈ ������ �ִٰ� ����)
} PMSG_ANS_2ANIV_SERIAL, * LPPMSG_ANS_2ANIV_SERIAL;

#endif // FOR_KOREA || EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO

#ifdef MONSTER_HERD_SYSTEM_20031120


//----------------------------------------------------------------------------
// GS -> ES [0x10] : �ش� �ø����� ����Ѵٰ� ��û
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	BYTE		btGiftKind;			// ��ǰ�� ���� (0:����ǹ������, 1:��ũ����)
} PMSG_REQ_REG_RINGGIFT, * LPPMSG_REQ_REG_RINGGIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x10] : �ش� �ø����� ��ϰ���� ����
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	BYTE		btIsRegistered;		// 1:���� / 0:���� / 2:�̹� ��ϵ� / 3:���ó��������ʰ�
	BYTE		btGiftSection;		// ��ǰ�� �з� (0:����ǹ������, 1:��ũ����)
	BYTE		btGiftKind;			// ��ǰ�� ���� (1:100�� / 2:10�� / 3:5�� / 4:2��)
} PMSG_ANS_REG_RINGGIFT, * LPPMSG_ANS_REG_RINGGIFT;


#endif


#ifdef EVENT_STONE_OF_FRIENDSHIP_20040206

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/*                     FRIENDSHIP STONE EVENT PROTOCOL                      */ 
//////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
// GS -> ES [0x11] : �ش� ������ ���� �����ǵ� ���� Ȯ���� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
} PMSG_REQ_VIEW_FRIENDSHIP_STONES, * LPPMSG_REQ_VIEW_FRIENDSHIP_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x12] : �ش� ������ �����ǵ� �߰� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	BYTE		iPosition;			// ������ ��ġ
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
} PMSG_REQ_REGISTER_FRIENDSHIP_STONES, * LPPMSG_REQ_REGISTER_FRIENDSHIP_STONES;


//----------------------------------------------------------------------------
// GS -> ES [0x13] : �ش� ������ ���� �����ǵ��� ��� (������ ����)�� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
	INT			iStoneCount;		// ���ҵ� ���氳��
} PMSG_REQ_DELETE_FRIENDSHIP_STONES, * LPPMSG_REQ_DELETE_FRIENDSHIP_STONES;


#endif // EVENT_STONE_OF_FRIENDSHIP_20040206


#ifdef CHAOSCASTLE_SYSTEM_20040408			// ī���� ĳ�� �������� ��ǰ�� ���õ� ��Ŷ ���ǵ�

//----------------------------------------------------------------------------
// GS -> ES [0x15] : ī���� ĳ�� �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
} PMSG_REQ_REG_CC_OFFLINE_GIFT, * LPPMSG_REQ_REG_CC_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x15] : ī���� ĳ�� �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ��� ��û�� ���� ������ �޴´�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
	INT			iResultCode;		// ����ڵ�
	CHAR		szGIFT_NAME[50];	// ��ǰ�̸�
} PMSG_ANS_REG_CC_OFFLINE_GIFT, * LPPMSG_ANS_REG_CC_OFFLINE_GIFT;


#endif


#ifdef DARK_LORD_HEART_EVENT_20040810

//----------------------------------------------------------------------------
// GS -> ES [0x16] : ��ũ�ε��� ���� �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
} PMSG_REQ_REG_DL_OFFLINE_GIFT, * LPPMSG_REQ_REG_DL_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x16] : ��ũ�ε��� ���� �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ��� ��û�� ���� ������ �޴´�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
	INT			iResultCode;		// ����ڵ�
	CHAR		szGIFT_NAME[50];	// ��ǰ�̸�
} PMSG_ANS_REG_DL_OFFLINE_GIFT, * LPPMSG_ANS_REG_DL_OFFLINE_GIFT;

#endif


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804

//----------------------------------------------------------------------------
// GS -> ES [0x17] : ������ �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
} PMSG_REQ_REG_HT_OFFLINE_GIFT, * LPPMSG_REQ_REG_HT_OFFLINE_GIFT;


//----------------------------------------------------------------------------
// ES -> GS [0x17] : ������ �������� �̺�Ʈ - �ش� �ɸ��� ��ǰ ��� ��û�� ���� ������ �޴´�.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	INT			iINDEX;
	CHAR		szUID [11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	CHAR		szNAME [11];		// �ɸ��� �̸�
	INT			iResultCode;		// ����ڵ�
	CHAR		szGIFT_NAME[50];	// ��ǰ�̸�
} PMSG_ANS_REG_HT_OFFLINE_GIFT, * LPPMSG_ANS_REG_HT_OFFLINE_GIFT;

#endif

#ifdef PCBANG_COUPON_EVENT_20060124
//----------------------------------------------------------------------------
// GS -> ES [0x18] : PC�� ���� �̺�Ʈ - ������ ����� �� �ִ��� Ȯ�� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_POSSIBLE_PCBANG_COUPON, *LPPMSG_REQ_POSSIBLE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// ES -> GS [0x18] : PC�� ���� �̺�Ʈ - PC�� ���� ��� Ȯ�� ��û�� ���� ������ ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : ����, 0x01 : ���� �̵��
										//			0x02 : �̹� �߱޵� ����	
										//			0x03 : ����
} PMSG_ANS_POSSIBLE_PCBANG_COUPON, *LPPMSG_ANS_POSSIBLE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// GS -> ES [0x19] : PC�� ���� �̺�Ʈ - PC�� ���� ��� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_USE_PCBANG_COUPON, *LPPMSG_REQ_USE_PCBANG_COUPON;

//----------------------------------------------------------------------------
// ES -> GS [0x19] : PC�� ���� �̺�Ʈ - PC�� ���� ��� ��û�� ���� ������ ������.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : ����
										//			0x01 : ����
} PMSG_ANS_USE_PCBANG_COUPON, *LPPMSG_ANS_USE_PCBANG_COUPON;
#endif // PCBANG_COUPON_EVENT_20060124

#ifdef WHITEANGEL_GET_ITEM_EVENT_20060822

//////////////////////////////////////////////////////////////////////////////
/*                WHITE ANGEL GET ITEM EVENT PROTOCOL                      */ 
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// GS -> ES [0x20] : ȭ��Ʈ���� ���������� �̺�Ʈ - ������ ���� ������� Ȯ��
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_CHECK_WHITEANGEL_GET_ITEM, *LPPMSG_REQ_CHECK_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x20] : ȭ��Ʈ���� ���������� �̺�Ʈ - ������ ���� ������� Ȯ�ο� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : ���� �����
										//			0x01 : ���� ����ڰ� �ƴ�
										//			0x02 : �̹� ���޵Ǿ���
										//			0x03 : ����
} PMSG_ANS_CHECK_WHITEANGEL_GET_ITEM, *LPPMSG_ANS_CHECK_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// GS -> ES [0x21] : ȭ��Ʈ���� ���������� �̺�Ʈ - ������ ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_WHITEANGEL_GET_ITEM, *LPPMSG_REQ_WHITEANGEL_GET_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x21] : ȭ��Ʈ���� ���������� �̺�Ʈ - ������ ���� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : ����
										//			0x01 : ����													
} PMSG_ANS_WHITEANGEL_GET_ITEM, *LPPMSG_ANS_WHITEANGEL_GET_ITEM;

#endif	// WHITEANGEL_GET_ITEM_EVENT_20060822

#ifdef KANTURU_TIMEATTACK_EVENT_20060822
//////////////////////////////////////////////////////////////////////////////
/*                KANTURU TIMEATTACK EVENT PROTOCOL						    */ 
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// GS -> ES [0x22] : ĭ���� Ÿ�Ӿ��� �̺�Ʈ - ĭ���� ������ ���������� Ŭ���� ����� �����.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	CHAR		szNAME[11];				// �ɸ��� �̸�
	WORD		wServerCode;			// �����ڵ�
	CHAR		szBattleID[14];			// ĭ���� ������ ID
	BYTE		btStageNumber;			// �����ܰ� ��ȣ
	WORD		wClearTime;				// Ŭ���� �ð�
	int			iLevel;					// ����
	int			iExp;					// ����ġ	
} PMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT, *LPPMSG_REQ_LOG_KANTURU_TIMEATTACK_EVENT;
#endif	// KANTURU_TIMEATTACK_EVENT_20060822

#ifdef PCBANG_POINT_SYSTEM_20070206		// EventServer Protocol Define
//////////////////////////////////////////////////////////////////////////////
/*                     PCBANG GAGE SYSTME PROTOCOL							*/ 
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// GS -> ES [0x23] : PC�� ����Ʈ �ý��� - ����Ʈ ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	BYTE		btServerType;			// ����Ÿ�� 0:������ 1:�׽�Ʈ����(FOR_NEW_TESTSERVER)	
	CHAR		szUID[11];				// ����� ����
} PMSG_REQ_PCBANG_POINT_INFO, *LPPMSG_REQ_PCBANG_POINT_INFO;

//----------------------------------------------------------------------------
// ES -> GS [0x23] : PC�� ����Ʈ �ý��� - ����Ʈ ������ �ǵ����ش�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	short		nPoint;					// PC�� ����Ʈ
#ifdef PCBANG_KUZIMO
	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
#endif
} PMSG_ANS_PCBANG_POINT_INFO, *LPPMSG_ANS_PCBANG_POINT_INFO;

//----------------------------------------------------------------------------
// GS -> ES [0x24] : PC�� ����Ʈ �ý��� - ����Ʈ�� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����	
	BYTE		btServerType;			// ����Ÿ�� 0:������ 1:�׽�Ʈ����(FOR_NEW_TESTSERVER)	
	short		nNewPoint;				// ����� �� ����Ʈ
#ifdef PCBANG_KUZIMO
	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
#endif
} PMSG_REQ_PCBANG_POINT_NEW_INFO, *LPPMSG_REQ_PCBANG_POINT_NEW_INFO;

//----------------------------------------------------------------------------
// ES -> GS [0x24] : PC�� ����Ʈ �ý��� - ����� ����Ʈ ������ ������� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	BYTE		btResult;				// 0:���� 1:����
} PMSG_ANS_PCBANG_POINT_NEW_INFO, *LPPMSG_ANS_PCBANG_POINT_NEW_INFO;

//----------------------------------------------------------------------------
// GS -> ES [0x25] : PC�� ����Ʈ �ý��� - ��ŷ�̺�Ʈ : ���� ��� ����Ʈ�� ������ ������ �����ϴ� �̺�Ʈ
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	BYTE		btServerType;			// ����Ÿ�� 0:������ 1:�׽�Ʈ����(FOR_NEW_TESTSERVER)	
	short		nUsePoint;				// ��� ����Ʈ
} PMSG_PCBANG_POINT_RANKING_EVENT, *LPPMSG_PCBANG_POINT_RANKING_EVENT;
#endif // PCBANG_POINT_SYSTEM_20070206

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

//----------------------------------------------------------------------------
//GS -> ES [0x26] : ���Ӽ������� ��ŷ������ ȯ�� ��� ������ ����
//----------------------------------------------------------------------------
typedef struct  
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [11];
	CHAR		szGameID [11];
	WORD		wServerCode;
	INT			nTempleNumber;		// ��� ��ȣ
	INT			nClass;				// Ŭ���� 
	INT			nLevel;				// ����
	BYTE		btWin;				// ��/��
	INT			nPCBangGuid;		// PC�� ����
	INT			nRankPoint;			// ��ŷ ����Ʈ
} PMSG_ANS_ILLUSION_TEMPLE_SCORE, * LPPMSG_ANS_ILLUSION_TEMPLE_SCORE;
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_ITEM_GIVE_NPC_ALAN_20070823		// EventServer Protocol Define
//////////////////////////////////////////////////////////////////////////////
/*                     ITEM GIVE NPC ALAN PROTOCOL							*/ 
//////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
// GS -> ES [0x27] : NPC �ٷ� ���������� �̺�Ʈ - ������ ������ �������� Ȯ��
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_CHECK_ALANS_TARGET, *LPPMSG_REQ_CHECK_ALANS_TARGET;

//----------------------------------------------------------------------------
// ES -> GS [0x27] : NPC �ٷ� ���������� �̺�Ʈ - ������ ���� ����/�Ұ��ɿ� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : �̹� ����
										//			0x01 : ������ �൵ ��
										//			0x02 : ����
} PMSG_ANS_CHECK_ALANS_TARGET, *LPPMSG_ANS_CHECK_ALANS_TARGET;

//----------------------------------------------------------------------------
// GS -> ES [0x28] : NPC �ٷ� ���������� �̺�Ʈ - ������ ���� ��û
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
} PMSG_REQ_ALANS_ITEM, *LPPMSG_REQ_ALANS_ITEM;

//----------------------------------------------------------------------------
// ES -> GS [0x21] : NPC �ٷ� ���������� �̺�Ʈ - ������ ���� ��û�� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	WORD		wServerCode;			// �����ڵ�
	BYTE		iResultCode;			// ����ڵ� 0x00 : ����
										//			0x01 : ����													
} PMSG_ANS_ALANS_ITEM, *LPPMSG_ANS_ALANS_ITEM;
#endif	// ADD_ITEM_GIVE_NPC_ALAN_20070823

#ifdef PCBANG_KUZIMO
//----------------------------------------------------------------------------
// GS -> ES [0x29] : PC�� ����Ʈ �ý��� - ����Ʈ ������ ��û�Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	BYTE		btServerType;			// ����Ÿ�� 0:������ 1:�׽�Ʈ����(FOR_NEW_TESTSERVER)	
	CHAR		szUID[11];				// ����� ����
} PMSG_REQ_PCBANG_POINT_INFO2, *LPPMSG_REQ_PCBANG_POINT_INFO2;

//----------------------------------------------------------------------------
// ES -> GS [0x29] : PC�� ����Ʈ �ý��� - ����Ʈ ������ �ǵ����ش�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	short		nPoint;					// PC�� ����Ʈ

	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
} PMSG_ANS_PCBANG_POINT_INFO2, *LPPMSG_ANS_PCBANG_POINT_INFO2;

//----------------------------------------------------------------------------
// GS -> ES [0x30] : PC�� ����Ʈ �ý��� - ����Ʈ�� �����Ѵ�.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����	
	BYTE		btServerType;			// ����Ÿ�� 0:������ 1:�׽�Ʈ����(FOR_NEW_TESTSERVER)	
	short		nNewPoint;				// ����� �� ����Ʈ

	short		nResetYear;
	short		nResetMonth;
	short		nResetDay;
	short		nResetHour;
} PMSG_REQ_PCBANG_POINT_NEW_INFO2, *LPPMSG_REQ_PCBANG_POINT_NEW_INFO2;

//----------------------------------------------------------------------------
// ES -> GS [0x30] : PC�� ����Ʈ �ý��� - ����� ����Ʈ ������ ������� �˸���.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];				// ����� ����
	BYTE		btResult;				// 0:���� 1:����
} PMSG_ANS_PCBANG_POINT_NEW_INFO2, *LPPMSG_ANS_PCBANG_POINT_NEW_INFO2;
#endif


#ifdef ADD_PAYITEM_20081030

//----------------------------------------------------------------------------
// DBTASK_REQ_GET_PAYITEM
// ���������� �̺�Ʈ - ������ ���� ������� Ȯ��
//----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_GET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	short		nEventID;			// �̺�Ʈ ���̵�(0x01: ũ��������)
} PMSG_REQ_GET_PAYITEM, *LPPMSG_REQ_GET_PAYITEM;
typedef struct _tagPMSG_ANS_GET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	short		nEventID;			// �̺�Ʈ ���̵�(0x01: ũ��������)
	BYTE		bFirst;				// ���� ����� ��/��(TRUE/FALSE)
	BYTE		nResult;
} PMSG_ANS_GET_PAYITEM, *LPPMSG_ANS_GET_PAYITEM;

//----------------------------------------------------------------------------
// DBTASK_ANS_SET_PAYITEM
// ���������� �̺�Ʈ - ������ ���� ��û
//----------------------------------------------------------------------------
typedef struct _tagPMSG_REQ_SET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	short		nEventID;			// �̺�Ʈ ���̵�(0x01: ũ��������)
} PMSG_REQ_SET_PAYITEM, *LPPMSG_REQ_SET_PAYITEM;
typedef struct _tagPMSG_ANS_SET_PAYITEM
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szUID[11];			// ����� ����
	WORD		wServerCode;		// �����ڵ�
	short		nEventID;			// �̺�Ʈ ���̵�(0x01: ũ��������)
	BYTE		bFirst;				// ���� ����� ��/��(TRUE/FALSE)
	BYTE		nResult;
} PMSG_ANS_SET_PAYITEM, *LPPMSG_ANS_SET_PAYITEM;

#endif // ADD_PAYITEM_20081030

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029
//----------------------------------------------------------------------------
// GS -> ES [0x33] : ��ϵ� ���� ���� ��û
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
// ES -> GS [0x33] : ��ϵ� ���� ���� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	int			nCurCoinCnt;	// ���� ��ϵ� ������ 
} PMSG_ES_ANS_GET_COIN_COUNT, *LPPMSG__ES_ANS_GET_COIN_COUNT;

//----------------------------------------------------------------------------
// GS -> ES [0x34] : �������
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	BYTE		btPos;						// ���� ��ġ
} PMSG_ES_REQ_REG_COIN, *LPPMSG_ES_REQ_REG_COIN;

//----------------------------------------------------------------------------
// ES -> GS [0x34] : ���� ��� ����
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short		nINDEX;
	CHAR		szAccountID [MAX_IDSTRING+1];
	CHAR		szGameID [MAX_IDSTRING+1];
	WORD		wServerCode;
	BYTE		btResult;					// ���
	int			nCurCoinCnt;				// ���� ��ϵ� ������
	BYTE		btPos;						// ���� ��ġ
} PMSG_ES_ANS_REG_COIN, *LPPMSG_ES_ANS_REG_COIN;

#endif	// UPDATE_LUCKY_COIN_EVENT_20081029


#endif

