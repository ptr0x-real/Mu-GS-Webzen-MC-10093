#if !defined(__CRYWOLFALTAR_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFALTAR_DEF__

// 1. > 제단과 요정의 계약을 관리한다.

// 2. > 제단의 상태를 관리한다. - 상태가 변경 됐을 때만 보내 준다.

#include "CrywolfDefine.h"

#define MAX_CRYWOLF_ALTAR_COUNT						5		// 제단의 개수

#define MAX_CONTRACT_COUNT							2		// 제단의 최대 계약 회수
#define CRYWOLF_ALTER_CONTRACTING_TIME				5		// 제단과의 계약 소요 시간
#define CRYWOLF_ALTER_RECONTRACT_TIME				10		// 제단의 재계약 가능 시간

enum CRYWOLF_ALTAR_NUMBER
{
	CRYWOLF_ALTAR1 = 0,
	CRYWOLF_ALTAR2 = 1,
	CRYWOLF_ALTAR3 = 2,
	CRYWOLF_ALTAR4 = 3,
	CRYWOLF_ALTAR5 = 4,
};

enum CRYWOLF_ALTAR_STATE
{
	CRYWOLF_ALTAR_STATE_NONE						= 0,	// 계약 상태 아님.	- 계약을 시도 할수 있음
	CRYWOLF_ALTAR_STATE_CONTRACTED					= 1,	// 계약 상태 중임	- 다른 사제는 계약 할 수 없음
	CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT			= 2,	// 계약을 시도 중임 - 다른 사제는 계약 할 수 없음
	CRYWOLF_ALTAR_STATE_EXCEEDING_CONTRACT_COUNT	= 3,	// 계약 횟수를 초과했음
	CRYWOLF_ALTAR_STATE_OCCUPYING					= 4,	// 제단 점령을 시도 중이다.
	CRYWOLF_ALTAR_STATE_OCCUPIED					= 5,	// 제단이 점령 되었음

};

class CCrywolfAltarInfo
{
public:
	CCrywolfAltarInfo()
	{
		Reset();
	}

	void Reset()
	{
		m_iAltarState				= CRYWOLF_ALTAR_STATE_NONE;

		m_iUserIndex				= _INVALID;
		m_iAltarIndex				= _INVALID;
		m_iContractCount			= _ZERO;
		m_dwAppliedContractTime		= _ZERO;
		m_dwValidContractTime		= _ZERO;
		m_dwLastValidContractTime	= _ZERO;
	}

	int	GetRemainContractCount() 
	{ 
		int iRemainCount = MAX_CONTRACT_COUNT - m_iContractCount;
		return iRemainCount<_ZERO ? _ZERO : iRemainCount;
	}
	
	void SetAltarState( int iState )
	{
		m_iAltarState = iState;
	}
	
	int GetAltarState()
	{
		return m_iAltarState;
	}

	void SetAltarIndex( int iAltarIndex )
	{
		m_iAltarIndex = iAltarIndex;
	}

	int	GetAltarIndex()
	{
		return m_iAltarIndex;
	}
	
	int GetAltarUserIndex()
	{
		return m_iUserIndex;
	}

public:
	int		m_iAltarState;				// 제단의 현재 상태

	int		m_iAltarIndex;				// 제단 인덱스
	int		m_iUserIndex;				// 계약한 유저인덱스

	int		m_iContractCount;			// 계약한 횟수
	int		m_dwAppliedContractTime;	// 계약한 시간
	int		m_dwValidContractTime;		// 계약이 발효된 시간
	int		m_dwLastValidContractTime;	// 마지막으로 계약이 유효 했던 시간
};


class CCrywolfAltar
{

public:
	CCrywolfAltar();
	virtual ~CCrywolfAltar();

public:
	void	CrywolfAltarAct( int iIndex );

public:
	// 계약 조건 체크하는 함수..추가 (요정 체크, 스탯 체크)
	
	// 제단 오브젝트 인덱스 등록
	void	SetAltarObjIndex( int iClass, int iObjIndex );
	void	SetAllAltarObjectIndex();

	// 제단 번호 
	int		GetAlatarNumber( int iClass );

	// 제단 계약 유효 처리
	void	SetAltarValidContract( int iAltarObjIndex, int iClass );
	BOOL	CheckAltarValidContract( int iClass );

	// 제단 상태 얻기
	int		GetAltarState( int iClass );

	// 제단 계약 횟수 얻기
	int		GetAltarRemainContractCount( int iClass );
	
	// 제단 계약 처리
	BOOL	SetAltarUserIndex( int iAltarObjIndex, int iClass, int iUserIndex );
	void	ResetAltarUserIndex( int iAltarObjIndex, int iClass );
	int		GetAltarUserIndex( int iClass );

	// 계약 된 제단 수
	int		GetContractedAltarCount();
	
	// 계약 된 제단의 사제들의 HP 합
	int		GetPriestHPSum();
	int		GetPriestMaxHPSum();

	// 제단 초기화
	void	ResetAllAltar();
	void	ResetAltar( int iClass );
	
	// 제단 뷰 상태 세팅
	void	SetAllAltarViewState( int iAltarState );
	void	SetAltarViewState( int iClass, int iAltarState );
	
public:
	CCrywolfAltarInfo		m_AltarInfo[MAX_CRYWOLF_ALTAR_COUNT];
	
};

extern CCrywolfAltar	g_CrywolfNPC_Altar;

#endif
