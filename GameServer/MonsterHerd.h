// MonsterHerd.h: interface for the MonsterHerd class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4786)


#if !defined(AFX_MONSTERHERD_H__E293D5C1_587C_476A_AC2D_BE0484B1D26F__INCLUDED_)
#define AFX_MONSTERHERD_H__E293D5C1_587C_476A_AC2D_BE0484B1D26F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
	����!!> MonsterHerd Ŭ������ ������ ���� ����,������ �������� �������� �����Ͽ� ����Ѵ�.
*/

#ifdef MONSTER_HERD_SYSTEM_20031120

#include "map"
#include "user.h"
#include "gObjMonster.h"

using namespace std;

#define CHECK_IN_MAP(C_VALUE)		(C_VALUE<0?FALSE:C_VALUE>MAX_MAP-1?FALSE:TRUE)

#define MAX_RADIUS		16


// ���� ���� ������ ������ ������ �ٷ� ����ü
typedef struct _MONSTER_HERD_DATA {
	INT		m_iIndex;		// ����� ����ü �ε���
	INT		m_iType;		// ���� Ÿ��
	BYTE	m_iX;			// ���� ���� X
	BYTE	m_iY;			// ���� ���� Y
	BOOL	m_bRegen;		// ��������

	_MONSTER_HERD_DATA() {
		m_iIndex = -1;
		m_iType = -1;
		m_iX = -1;
		m_iY = -1;
		m_bRegen = FALSE;
	}
} MONSTER_HERD_DATA, * LPMONSTER_HERD_DATA;


class MonsterHerd  
{
protected:
	INT		m_iHerdIndex;			// ������ü �ڽ��� ID

	INT		m_iMapNumber;			// ���� �����ϴ� �ʹ�ȣ
	INT		m_iCUR_X;				// ������ X��ǥ
	INT		m_iCUR_Y;				// ������ Y��ǥ
	INT		m_iRADIUS;				// ������ �̵����� ������

	BOOL	m_bHasInfo;				// ���ۿ� �ʿ��� �������� �����ϴ��� Ȯ��
	BOOL	m_bHerdActive;			// �� ��ü�� ����ִ��� ���� (�׾��ٸ� ��� �ൿ���� �޼���� ����)

	map<INT, MONSTER_HERD_DATA>	m_mapMonsterHerd;		// ���� ������ �����ϱ� ���� ��
	CRITICAL_SECTION			m_critMonsterHerd;		// ���� ���� ���� ���͸� ��ȣ�ϱ� ���� �Ӱ豸��

public:
	MonsterHerd();
	virtual ~MonsterHerd();

	virtual BOOL	SetTotalInfo (				// 1 . ���� ���� ���� - ���� ȣ��
		INT iMapNumber, 
		INT iRadius,
		INT iStartX	=-1,
		INT iStartY	=-1
		);
	virtual BOOL	AddMonster (				// 2 . ���� �߰� - 2��° ȣ��
		INT	iMonsterType,				
		BOOL bRegen = FALSE,
		BOOL bAttackFirst = FALSE		
		);
	virtual VOID	SetRadius (					// ������ �̵��� �������� ����
		INT	iRadius
		);
	virtual VOID	SetPosition (				// ������ �̵��� ��ǥ�� ����
		UCHAR iTX,
		UCHAR iTY
		);
	virtual BOOL	Start ();
	virtual VOID	Stop ();
	virtual BOOL	CheckInRadius (				// �������� �߽����� �����ȿ� �ִ��� üũ
		INT iIndex
		);
	virtual BOOL	GetCurrentLocation(			// ������ ���� �߽���ǥ�� �� �����Ѵ�.
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	GetRandomLocation(			// ���� �ݰ泻�� ������ ��ǥ�� �� �����Ѵ�.
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	CheckLocation(				// �ش� ������ �Ӽ��� �������� Ȯ��
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	MoveHerd (					// ������ �ش� �������� �̵���Ű�� ���� ���� ����
		UCHAR iTX,
		UCHAR iTY
		);
	virtual BOOL	MonsterHerdItemDrop(
		LPOBJECTSTRUCT lpObj
		);
	virtual LPMONSTER_HERD_DATA GetMonsterData(		// �������� �ش� ������ ������ ��´�.
		INT iIndex
		);
	virtual VOID BeenAttacked (					// ������ ����� ������ ����� ���� �ൿ
		LPOBJECTSTRUCT lpObj,					// ��
		LPOBJECTSTRUCT lpTargetObj				// ���� ������ �ΰ�?
		);
	virtual VOID OrderAttack (					// Ư�� �ΰ�? �� �����϶�� ����
		LPOBJECTSTRUCT lpObj,					// ��
		LPOBJECTSTRUCT lpTargetObj,				// ���� ������ �ΰ�?
		INT iAttackPercent						// ���������� ���ÿ� ���� Ȯ��
		);
	virtual VOID MonsterBaseAct(				// ���� ���͵��� �⺻ �ൿ�� ���Ѵ�.
		LPOBJECTSTRUCT lpObj
		);
	virtual VOID MonsterMoveAction(				// ���� ���͵��� ������ �׼��� ó���Ѵ�.
		LPOBJECTSTRUCT lpObj
		);
	virtual VOID MonsterAttackAction(			// ���� ���� �� Ư�� ���Ͱ� �������� ���� ó��
		LPOBJECTSTRUCT lpObj,					// ��
		LPOBJECTSTRUCT lpTargetObj				// ���� ������ �ΰ�?
		);
	virtual VOID MonsterDieAction(				// ���� ���� �� Ư�� ���Ͱ� �׾��� ���� ó��
		LPOBJECTSTRUCT lpObj
		);
	virtual BOOL MonsterRegenAction(			// ���� ���� �� Ư�� ���Ͱ� ������ ���� ó��
		LPOBJECTSTRUCT lpObj
		);
};

#endif

#endif // !defined(AFX_MONSTERHERD_H__E293D5C1_587C_476A_AC2D_BE0484B1D26F__INCLUDED_)
