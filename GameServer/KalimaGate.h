// KalimaGate.h: interface for the CKalimaGate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_)
#define AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEW_MAP_KALIMA_20040518

#include "user.h"
#include "gObjMonster.h"

#define KALIMAGATE_SUMMON_FIELDSIZE		5				// 사용자 주변에 마석을 소환하는 좌표범위 (N x N)

#ifdef HIDDEN_KALIMA_20050706
#define MAX_KALIMAGATE_COUNT			7				// 마석 레벨 범위
#else
#define MAX_KALIMAGATE_COUNT			6				// 마석 레벨 범위
#endif

//#define MAX_KALIMAGATE_STONE_OVERLAP	20				// 마석의 최대 겹쳐지는 수 
// ### 20040906 (apple) 
//#define MAX_KALIMAGATE_STONE_OVERLAP	10				// 마석의 최대 겹쳐지는 수 (10개로 수정 - apple)
#define MAX_KALIMAGATE_STONE_OVERLAP	5				// 마석의 최대 겹쳐지는 수 (5개로 수정 - b4nfter)
//#define MAX_KALIMAGATE_LIFE_TICKTERM	300000			// 마석이 살아 있는 시간
// ### 20040906 (apple) 
#define MAX_KALIMAGATE_LIFE_TICKTERM	180000			// 마석이 살아 있는 시간

#ifdef HIDDEN_KALIMA_20050706
#define MAX_KALIMAGATE_LIFE_TICKTERM2	60000			// 마석이 살아 있는 시간 (특별 마석)
#endif

#define MAX_KALIMAGATE_ENTER_COUNT		5				// 마석을 통해 입장할 수 있는 최대 횟수


#define GATE_INDEX_KALIMA_1				88				// 칼리마 1
#define GATE_INDEX_KALIMA_2				89				// 칼리마 2
#define GATE_INDEX_KALIMA_3				90				// 칼리마 3
#define GATE_INDEX_KALIMA_4				91				// 칼리마 4
#define GATE_INDEX_KALIMA_5				92				// 칼리마 5
#define GATE_INDEX_KALIMA_6				93				// 칼리마 6
#ifdef HIDDEN_KALIMA_20050706
#define GATE_INDEX_KALIMA_7				116				// 칼리마 7 (히든 칼리마)
#endif

#define MONSTER_INDEX_KALIMA_1			152				// 마석 인덱스 1
#define MONSTER_INDEX_KALIMA_2			153				// 마석 인덱스 2
#define MONSTER_INDEX_KALIMA_3			154				// 마석 인덱스 3
#define MONSTER_INDEX_KALIMA_4			155				// 마석 인덱스 4
#define MONSTER_INDEX_KALIMA_5			156				// 마석 인덱스 5
#define MONSTER_INDEX_KALIMA_6			157				// 마석 인덱스 6
#ifdef HIDDEN_KALIMA_20050706
#define MONSTER_INDEX_KALIMA_7			158				// 마석 인덱스 7
#endif

typedef struct _KALIMAGATE_LEVEL {
	INT iLOWER_BOUND;									// 하한선
	INT iUPPER_BOUND;									// 상한선
	INT iLOWER_BOUND_MAGUMSA;							// 하한선 - 마검사
	INT iUPPER_BOUND_MAGUMSA;							// 상한선 - 마검사
} KALIMAGATE_LEVEL, * LPKALIMAGATE_LEVEL;

/*
static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	349,		280,	319,
	350,	MAX_LEVEL,	320,	MAX_LEVEL,
	};
*/

// ### 20040906 (apple) 

#ifdef HIDDEN_KALIMA_20050706
static int g_iKalimaGateGateIndex[MAX_KALIMAGATE_COUNT] = {
	88,													// 칼리마 1
	89,													// 칼리마 2
	90,													// 칼리마 3
	91,													// 칼리마 4
	92,													// 칼리마 5
	93,													// 칼리마 6
	116,												// 칼리마 7
};

static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {		// "GameMain.cpp" 의 MapMinUserLevel[] 의 데이터도 함께 수정되어야 함
	40,		130,		20,		110,					// 칼리마 1
	131,	180,		111,	160,					// 칼리마 2
	181,	230,		161,	210,					// 칼리마 3
	231,	280,		211,	260,					// 칼리마 4
	281,	330,		261,	310,					// 칼리마 5
#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	331,	349,		311,	349,					// 칼리마 6
	350,	MAX_LEVEL,	350,	MAX_LEVEL,				// 칼리마 7
#else
	331,	379,		311,	379,					// 칼리마 6
	380,	MAX_LEVEL,	380,	MAX_LEVEL,				// 칼리마 7
#endif
	};
#else
static KALIMAGATE_LEVEL	g_sttKALIMAGATE_LEVEL[MAX_KALIMAGATE_COUNT] = {		// "GameMain.cpp" 의 MapMinUserLevel[] 의 데이터도 함께 수정되어야 함
	15,		49,			15,		29,
	50,		119,		30,		99,
	120,	179,		100,	159,
	180,	239,		160,	219,
	240,	299,		220,	279,
	300,	MAX_LEVEL,	280,	MAX_LEVEL,
	};
#endif


class CKalimaGate  
{
	CRITICAL_SECTION	m_critKalimaGate;

public:
					CKalimaGate				();
	virtual			~CKalimaGate			();
#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	BOOL	CreateKalimaGate		(			// 특정 사용자의 마석을 소환. 마석 레벨 체크 (apple)
		INT		iIndex,
		BYTE	btLevel, 
		UCHAR	cTX, 
		UCHAR	cTY 
		);
#else
	BOOL	CreateKalimaGate		(			// 특정 사용자의 마석을 소환
		INT		iIndex,
		UCHAR	cTX, 
		UCHAR	cTY 
		);
#endif

	VOID	KalimaGateAct			(			// 마석의 행동방식
		INT		iIndex
		);

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
	INT		CheckOverlapKundunMark	(			// 쿤둔의 표식을 인벤토리에 겹칠 수 있는지 확인한다. 
		INT		iIndex,
		BYTE	btLevel
		);
#else
	INT		CheckOverlapKundunMark	(			// 쿤둔의 표식을 인벤토리에 겹칠 수 있는지 확인한다. 
		INT		iIndex
		);
#endif

	BOOL	DeleteKalimaGate		(			// 칼리마 게이트를 제거한다.
		INT		iKalimaGateIndex,
		INT		iCallOwnerIndex
		);
	BOOL	DeleteKalimaGate		(			// 칼리마 게이트를 제거한다.
		INT		iCallOwnerIndex
		);

#ifdef HIDDEN_KALIMA_20050706					// 다음 맵으로 보내는 칼리마 게이트의 생성, 동작 함수
	BOOL	CreateKalimaGate2		(			// 다음 맵으로 보내는 칼리마 게이트의 생성
		INT		iIndex,
		INT		iMonMapNumber,
		UCHAR	cTX, 
		UCHAR	cTY 
		);

	VOID	KalimaGateAct2			(			// 다음 맵으로 보내는 칼리마 게이트의 동작
		INT		iIndex
		);
#endif

#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
	INT		GetKalimaGateLevel2		(
		INT		iIndex
		);
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110

private:

	BOOL	GetRandomLocation		(			// 사용자 반경내의 랜덤한 좌표를 얻어서 리턴한다.
		INT		iMapNumber,
		UCHAR &	cX,
		UCHAR & cY
		);
	INT		GetKalimaGateLevel		(			// 사용자가 사용 가능한 마석의 레벨을 얻는다.
		INT		iIndex
		);
};


extern CKalimaGate	g_KalimaGate;


#endif


#endif // !defined(AFX_KALIMAGATE_H__4E0C037D_DA6B_4CED_8C8B_DEE36ED82342__INCLUDED_)
