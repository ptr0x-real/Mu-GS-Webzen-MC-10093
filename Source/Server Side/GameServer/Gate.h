// Gate.h: interface for the CGate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_)
#define AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// Gate�� �ִ� �迭���� �ø�
#define MAX_GATE	512
#else
#define MAX_GATE	256
#endif

class CGate  
{

public:
	
	//------------------------------------------------------------------------
	BYTE m_This[MAX_GATE];			// �ڽ� ��ȣ
	BYTE m_Flag[MAX_GATE];			// flag : 0 : ������ �ִ°�, 1 : �������� �ִ°�, 2 : ���� ������ ��� ����
	BYTE m_MapNumber[MAX_GATE];		// �� ��ȣ
	BYTE m_Sx[MAX_GATE];			// ���� X ��ǥ
	BYTE m_Sy[MAX_GATE];			// ���� Y ��ǥ
	BYTE m_Ex[MAX_GATE];			// ��   X ��ǥ
	BYTE m_Ey[MAX_GATE];			// ��   Y ��ǥ
#ifdef ADD_SEASON_3_NEW_MAP_20070910
	WORD m_TargetGate[MAX_GATE];	// �̵��Ϸ��� gate number
#else
	BYTE m_TargetGate[MAX_GATE];	// �̵��Ϸ��� gate number
#endif
	BYTE m_Dir[MAX_GATE];
	
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// Gate��ũ��Ʈ ������ �������Ѱ��� short�� �ø�
	short m_Level[MAX_GATE];			// ���� ����
#else	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
	BYTE m_Level[MAX_GATE];			// ���� ����
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
	
	//------------------------------------------------------------------------
	CGate();
	virtual ~CGate();
	void Init();
	void Load(char *filename="..\\data\\gate.txt");
	
#ifdef AUTH_GAMESERVER
	void Load(char *Buffer, int iSize);
#endif

#ifdef SCRIPT_DECODE_WORK
	void LoadDecode(char *filename);
#endif

	BOOL IsGate(int GateNumber);
	int  GetGate(int mgt, short &x, short &y, BYTE & MapNumber, BYTE & dir, short & Level);
	int  GetLevel(int GateNumber);
	
	// �̵� ������ ����Ʈ�ȿ� �ִ°�?
	BOOL	IsInGate(int aIndex, int GateNumber);
	
#ifdef MAP_SERVER_WORK_20041030		// Ư�� ����Ʈ��ȣ�� �̵� �������� üũ
	BOOL CheckGateLevel(int aIndex, int GateNumber);
#endif
};


extern CGate	gGateC;


#endif // !defined(AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_)
