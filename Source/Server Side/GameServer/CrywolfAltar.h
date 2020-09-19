#if !defined(__CRYWOLFALTAR_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFALTAR_DEF__

// 1. > ���ܰ� ������ ����� �����Ѵ�.

// 2. > ������ ���¸� �����Ѵ�. - ���°� ���� ���� ���� ���� �ش�.

#include "CrywolfDefine.h"

#define MAX_CRYWOLF_ALTAR_COUNT						5		// ������ ����

#define MAX_CONTRACT_COUNT							2		// ������ �ִ� ��� ȸ��
#define CRYWOLF_ALTER_CONTRACTING_TIME				5		// ���ܰ��� ��� �ҿ� �ð�
#define CRYWOLF_ALTER_RECONTRACT_TIME				10		// ������ ���� ���� �ð�

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
	CRYWOLF_ALTAR_STATE_NONE						= 0,	// ��� ���� �ƴ�.	- ����� �õ� �Ҽ� ����
	CRYWOLF_ALTAR_STATE_CONTRACTED					= 1,	// ��� ���� ����	- �ٸ� ������ ��� �� �� ����
	CRYWOLF_ALTAR_STATE_APPLYING_CONTRACT			= 2,	// ����� �õ� ���� - �ٸ� ������ ��� �� �� ����
	CRYWOLF_ALTAR_STATE_EXCEEDING_CONTRACT_COUNT	= 3,	// ��� Ƚ���� �ʰ�����
	CRYWOLF_ALTAR_STATE_OCCUPYING					= 4,	// ���� ������ �õ� ���̴�.
	CRYWOLF_ALTAR_STATE_OCCUPIED					= 5,	// ������ ���� �Ǿ���

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
	int		m_iAltarState;				// ������ ���� ����

	int		m_iAltarIndex;				// ���� �ε���
	int		m_iUserIndex;				// ����� �����ε���

	int		m_iContractCount;			// ����� Ƚ��
	int		m_dwAppliedContractTime;	// ����� �ð�
	int		m_dwValidContractTime;		// ����� ��ȿ�� �ð�
	int		m_dwLastValidContractTime;	// ���������� ����� ��ȿ �ߴ� �ð�
};


class CCrywolfAltar
{

public:
	CCrywolfAltar();
	virtual ~CCrywolfAltar();

public:
	void	CrywolfAltarAct( int iIndex );

public:
	// ��� ���� üũ�ϴ� �Լ�..�߰� (���� üũ, ���� üũ)
	
	// ���� ������Ʈ �ε��� ���
	void	SetAltarObjIndex( int iClass, int iObjIndex );
	void	SetAllAltarObjectIndex();

	// ���� ��ȣ 
	int		GetAlatarNumber( int iClass );

	// ���� ��� ��ȿ ó��
	void	SetAltarValidContract( int iAltarObjIndex, int iClass );
	BOOL	CheckAltarValidContract( int iClass );

	// ���� ���� ���
	int		GetAltarState( int iClass );

	// ���� ��� Ƚ�� ���
	int		GetAltarRemainContractCount( int iClass );
	
	// ���� ��� ó��
	BOOL	SetAltarUserIndex( int iAltarObjIndex, int iClass, int iUserIndex );
	void	ResetAltarUserIndex( int iAltarObjIndex, int iClass );
	int		GetAltarUserIndex( int iClass );

	// ��� �� ���� ��
	int		GetContractedAltarCount();
	
	// ��� �� ������ �������� HP ��
	int		GetPriestHPSum();
	int		GetPriestMaxHPSum();

	// ���� �ʱ�ȭ
	void	ResetAllAltar();
	void	ResetAltar( int iClass );
	
	// ���� �� ���� ����
	void	SetAllAltarViewState( int iAltarState );
	void	SetAltarViewState( int iClass, int iAltarState );
	
public:
	CCrywolfAltarInfo		m_AltarInfo[MAX_CRYWOLF_ALTAR_COUNT];
	
};

extern CCrywolfAltar	g_CrywolfNPC_Altar;

#endif
