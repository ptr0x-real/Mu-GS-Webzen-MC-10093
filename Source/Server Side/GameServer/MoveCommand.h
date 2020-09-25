// MoveCommand.h: interface for the CMoveCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVECOMMAND_H__91EED516_CA27_47FC_9231_FA4D937BB8AF__INCLUDED_)
#define AFX_MOVECOMMAND_H__91EED516_CA27_47FC_9231_FA4D937BB8AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODIFY_MOVEREQ_MAP_INDEX_LITMIT_20090421
#define MAX_MOVE_COMMAND_COUNT		50	// movereq(kor).txt���Ͽ� ���߰��� ���� ����
#else
	#ifdef ADD_SEASON_3_NEW_MAP_20070910
#define MAX_MOVE_COMMAND_COUNT		40	// movereq(kor).txt���Ͽ� ���߰��� ���� ����
#else
#define MAX_MOVE_COMMAND_COUNT		30
#endif
#endif // MODIFY_MOVEREQ_MAP_INDEX_LITMIT_20090421

typedef struct 
{
	int		Index;
	char	Name[255];			// �̵� �� ��ġ �̸� (�ڱ���)
	int		NeedZen;			// �ʿ� ��
	int		NeedLevel;			// ������ ����
	int		MapNumber;			// �̵��� ����Ʈ ��ȣ
	int		X1, Y1, X2, Y2;
	int		VIP;
	int		Reset;
}MOVE_COMMAND_DATA, * LPMOVE_COMMAND_DATA;

#ifdef DARKLORD_WORK
// ��ũ�ε� ��Ƽ ��ų �Ǹտ��� ����Ұ���
typedef struct 
{
	int		MoveLevel;		// �̵� ���� ����
	int		MapNumber;		// �ʹ�ȣ
	int		X;				// (X,Y)~(TX,TY)
	int		Y;
	int		TX;
	int		TY;	
}MOVE_MOVE_LEVEL, * LPMOVE_MOVE_LEVEL;		// �̵� ���� üũ�� ����ü
#endif

class CMoveCommand  
{
	MOVE_COMMAND_DATA	m_MoveCommandData[MAX_MOVE_COMMAND_COUNT];
#ifdef DARKLORD_WORK
	MOVE_MOVE_LEVEL		m_MoveLevel[MAX_MOVE_COMMAND_COUNT];
#endif
	
private:
	int		FindIndex(char* mapname);	// �̵��� �ʿ��� �����͸� ã�´�
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	int		FindIndex(int iMapIndex);	// ��ũ��Ʈ ������ �ε����� MOVE_COMMAND_DATA�� �ε����� ��ġ ��Ų��.
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
	// ���������� �̵��� üũ �Ѵ�.
	// 0. �ֿ� ���� ���� üũ - ����ĳ��, ī����ĳ�� ����
	BOOL	CheckMainToMove( LPOBJECTSTRUCT lpObj );

	// 1. ���� ������ üũ
	BOOL	CheckEquipmentToMove( LPOBJECTSTRUCT lpObj, INT iTargetMapNumber );

	// 2. �������̽� üũ
	BOOL	CheckInterfaceToMove( LPOBJECTSTRUCT lpObj );
#endif
	
	// �ش� ������ �̵� ��Ų��
#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	BOOL	Move(LPOBJECTSTRUCT lpObj, int iMapIndex);
#else
	BOOL	Move(LPOBJECTSTRUCT lpObj, char* mapname);
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#if TESTSERVER == 1		// �׽�Ʈ ������ �̵�
	BOOL	TestServerMove(LPOBJECTSTRUCT lpObj, char* mapname);	
#endif
#ifdef PCBANG_FREE_KALIMA_EVENT_20060124
	BOOL	MoveFreeKalima(LPOBJECTSTRUCT lpObj, char* mapname);	// PC�� �̺�Ʈ�� Į���� ���� �̵�
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
