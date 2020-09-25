// MoveCommand.h: interface for the CMoveCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVECOMMAND_H__91EED516_CA27_47FC_9231_FA4D937BB8AF__INCLUDED_)
#define AFX_MOVECOMMAND_H__91EED516_CA27_47FC_9231_FA4D937BB8AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODIFY_MOVEREQ_MAP_INDEX_LITMIT_20090421
#define MAX_MOVE_COMMAND_COUNT		50	// movereq(kor).txt파일에 맵추가로 인한 증가
#else
	#ifdef ADD_SEASON_3_NEW_MAP_20070910
#define MAX_MOVE_COMMAND_COUNT		40	// movereq(kor).txt파일에 맵추가로 인한 증가
#else
#define MAX_MOVE_COMMAND_COUNT		30
#endif
#endif // MODIFY_MOVEREQ_MAP_INDEX_LITMIT_20090421

typedef struct 
{
	int		Index;
	char	Name[255];			// 이동 할 위치 이름 (자국어)
	int		NeedZen;			// 필요 젠
	int		NeedLevel;			// 가능한 레벨
	int		MapNumber;			// 이동할 게이트 번호
	int		X1, Y1, X2, Y2;
	int		VIP;
	int		Reset;
}MOVE_COMMAND_DATA, * LPMOVE_COMMAND_DATA;

#ifdef DARKLORD_WORK
// 다크로드 파티 스킬 쎠먼에서 사용할것임
typedef struct 
{
	int		MoveLevel;		// 이동 가능 레벨
	int		MapNumber;		// 맵번호
	int		X;				// (X,Y)~(TX,TY)
	int		Y;
	int		TX;
	int		TY;	
}MOVE_MOVE_LEVEL, * LPMOVE_MOVE_LEVEL;		// 이동 레벨 체크할 구조체
#endif

class CMoveCommand  
{
	MOVE_COMMAND_DATA	m_MoveCommandData[MAX_MOVE_COMMAND_COUNT];
#ifdef DARKLORD_WORK
	MOVE_MOVE_LEVEL		m_MoveLevel[MAX_MOVE_COMMAND_COUNT];
#endif
	
private:
	int		FindIndex(char* mapname);	// 이동에 필요한 데이터를 찾는다
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	int		FindIndex(int iMapIndex);	// 스크립트 파일의 인덱스와 MOVE_COMMAND_DATA의 인덱스를 일치 시킨다.
#endif // ADD_MAPMOVE_PROTOCOL_20090327
	
public:
	CMoveCommand();
	virtual ~CMoveCommand();

	void	Init();

	BOOL	Load(char* filename);

#ifdef AUTH_GAMESERVER
	BOOL	Load(char *Buffer, int iSize);
#endif

#ifdef DARKLORD_WORK
	BOOL	LoadMoveLevel(char* filename);
	int		GetMoveLevel(int mapnumber, int x, int y, int Class);
	int		GetMoveLevel(int mapnumber);
	int		CheckMoveLevel(int mapnumber, int x, int y, int Class);
#endif
	
#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
	// 범용적으로 이동을 체크 한다.
	// 0. 주요 제약 조건 체크 - 블러드캐슬, 카오스캐슬 여부
	BOOL	CheckMainToMove( LPOBJECTSTRUCT lpObj );

	// 1. 장착 아이템 체크
	BOOL	CheckEquipmentToMove( LPOBJECTSTRUCT lpObj, INT iTargetMapNumber );

	// 2. 인터페이스 체크
	BOOL	CheckInterfaceToMove( LPOBJECTSTRUCT lpObj );
#endif
	
	// 해당 맵으로 이동 시킨다
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	BOOL	Move(LPOBJECTSTRUCT lpObj, int iMapIndex);
#else
	BOOL	Move(LPOBJECTSTRUCT lpObj, char* mapname);
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#if TESTSERVER == 1		// 테스트 서버용 이동
	BOOL	TestServerMove(LPOBJECTSTRUCT lpObj, char* mapname);	
#endif
#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	BOOL	MoveFreeKalima(LPOBJECTSTRUCT lpObj, char* mapname);	// PC방 이벤트용 칼리마 무료 이동
#endif // PCBANG_FREE_KALIMA_EVENT_20060124	
	BOOL	MoveFree2Kalima(LPOBJECTSTRUCT lpObj); // happycat@20050201

#ifdef ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221
	BOOL	CheckMoveMapBound(int iMapIndex);
#endif // ADD_PCS_MARKITEM_MAP_MOVE_BOUND_20070221
	
#ifdef ADD_NEWPVP_PKFIELD
	LPMOVE_COMMAND_DATA	GetMoveCommandData(int nMapIndex);
#endif // ADD_NEWPVP_PKFIELD

	char* CMoveCommand::GetMapName(int mapnumber); // Get the map name by map number.
};

extern CMoveCommand	gMoveCommand;
#endif // !defined(AFX_MOVECOMMAND_H__91EED516_CA27_47FC_9231_FA4D937BB8AF__INCLUDED_)
