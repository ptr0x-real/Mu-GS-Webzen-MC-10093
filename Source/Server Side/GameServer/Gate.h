// Gate.h: interface for the CGate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_)
#define AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// Gate의 최대 배열수를 늘림
#define MAX_GATE	512
#else
#define MAX_GATE	256
#endif

class CGate  
{

public:
	
	//------------------------------------------------------------------------
	BYTE m_This[MAX_GATE];			// 자신 번호
	BYTE m_Flag[MAX_GATE];			// flag : 0 : 들어갈수만 있는곳, 1 : 나갈수만 있는곳, 2 : 들어가고 나가고 모두 가능
	BYTE m_MapNumber[MAX_GATE];		// 맵 번호
	BYTE m_Sx[MAX_GATE];			// 시작 X 좌표
	BYTE m_Sy[MAX_GATE];			// 시작 Y 좌표
	BYTE m_Ex[MAX_GATE];			// 끝   X 좌표
	BYTE m_Ey[MAX_GATE];			// 끝   Y 좌표
#ifdef ADD_SEASON_3_NEW_MAP_20070910
	WORD m_TargetGate[MAX_GATE];	// 이동하려는 gate number
#else
	BYTE m_TargetGate[MAX_GATE];	// 이동하려는 gate number
#endif
	BYTE m_Dir[MAX_GATE];
	
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// Gate스크립트 파일의 레벨제한값을 short로 늘림
	short m_Level[MAX_GATE];			// 레벨 제한
#else	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
	BYTE m_Level[MAX_GATE];			// 레벨 제한
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
	
	// 이동 가능한 게이트안에 있는가?
	BOOL	IsInGate(int aIndex, int GateNumber);
	
#ifdef MAP_SERVER_WORK_20041030		// 특정 게이트번호로 이동 가능한지 체크
	BOOL CheckGateLevel(int aIndex, int GateNumber);
#endif
};


extern CGate	gGateC;


#endif // !defined(AFX_GATE_H__3802B257_1BDE_48D3_88E9_C0FA94F5B4AC__INCLUDED_)
