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
	주의!!> MonsterHerd 클래스는 가급적 동적 생성,삭제를 하지말고 정적으로 선언하여 사용한다.
*/

#ifdef MONSTER_HERD_SYSTEM_20031120

#include "map"
#include "user.h"
#include "gObjMonster.h"

using namespace std;

#define CHECK_IN_MAP(C_VALUE)		(C_VALUE<0?FALSE:C_VALUE>MAX_MAP-1?FALSE:TRUE)

#define MAX_RADIUS		16


// 몬스터 무리 구성원 개개의 정보를 다룰 구조체
typedef struct _MONSTER_HERD_DATA {
	INT		m_iIndex;		// 사용자 구조체 인덱스
	INT		m_iType;		// 몬스터 타입
	BYTE	m_iX;			// 몬스터 시작 X
	BYTE	m_iY;			// 몬스터 시작 Y
	BOOL	m_bRegen;		// 리젠여부

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
	INT		m_iHerdIndex;			// 무리객체 자신의 ID

	INT		m_iMapNumber;			// 현재 존재하는 맵번호
	INT		m_iCUR_X;				// 현재의 X좌표
	INT		m_iCUR_Y;				// 현재의 Y좌표
	INT		m_iRADIUS;				// 현재의 이동가능 반지름

	BOOL	m_bHasInfo;				// 시작에 필요한 정보들이 존재하는지 확인
	BOOL	m_bHerdActive;			// 이 객체가 살아있는지 여부 (죽었다면 모든 행동관련 메서드는 멈춤)

	map<INT, MONSTER_HERD_DATA>	m_mapMonsterHerd;		// 몬스터 무리를 관리하기 위한 맵
	CRITICAL_SECTION			m_critMonsterHerd;		// 몬스터 무리 관리 벡터를 보호하기 위한 임계구역

public:
	MonsterHerd();
	virtual ~MonsterHerd();

	virtual BOOL	SetTotalInfo (				// 1 . 무리 정보 설정 - 최초 호출
		INT iMapNumber, 
		INT iRadius,
		INT iStartX	=-1,
		INT iStartY	=-1
		);
	virtual BOOL	AddMonster (				// 2 . 몬스터 추가 - 2번째 호출
		INT	iMonsterType,				
		BOOL bRegen = FALSE,
		BOOL bAttackFirst = FALSE		
		);
	virtual VOID	SetRadius (					// 무리가 이동할 반지름을 지정
		INT	iRadius
		);
	virtual VOID	SetPosition (				// 무리가 이동할 좌표를 지정
		UCHAR iTX,
		UCHAR iTY
		);
	virtual BOOL	Start ();
	virtual VOID	Stop ();
	virtual BOOL	CheckInRadius (				// 구성원이 중심점의 범위안에 있는지 체크
		INT iIndex
		);
	virtual BOOL	GetCurrentLocation(			// 무리의 현재 중심좌표를 얻어서 리턴한다.
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	GetRandomLocation(			// 무리 반경내의 랜덤한 좌표를 얻어서 리턴한다.
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	CheckLocation(				// 해당 지역의 속성이 보통인지 확인
		UCHAR & cX,
		UCHAR & cY
		);
	virtual BOOL	MoveHerd (					// 무리를 해당 지점으로 이동시키기 위해 정점 지정
		UCHAR iTX,
		UCHAR iTY
		);
	virtual BOOL	MonsterHerdItemDrop(
		LPOBJECTSTRUCT lpObj
		);
	virtual LPMONSTER_HERD_DATA GetMonsterData(		// 무리안의 해당 몬스터의 정보를 얻는다.
		INT iIndex
		);
	virtual VOID BeenAttacked (					// 무리의 멤버를 공격한 사람에 대한 행동
		LPOBJECTSTRUCT lpObj,					// 나
		LPOBJECTSTRUCT lpTargetObj				// 나를 공격한 인간?
		);
	virtual VOID OrderAttack (					// 특정 인간? 을 공격하라는 지시
		LPOBJECTSTRUCT lpObj,					// 나
		LPOBJECTSTRUCT lpTargetObj,				// 나를 공격한 인간?
		INT iAttackPercent						// 무리원들이 지시에 따를 확률
		);
	virtual VOID MonsterBaseAct(				// 무리 몬스터들의 기본 행동을 정한다.
		LPOBJECTSTRUCT lpObj
		);
	virtual VOID MonsterMoveAction(				// 무리 몬스터들의 움직임 액션을 처리한다.
		LPOBJECTSTRUCT lpObj
		);
	virtual VOID MonsterAttackAction(			// 무리 몬스터 중 특정 몬스터가 공격했을 때의 처리
		LPOBJECTSTRUCT lpObj,					// 나
		LPOBJECTSTRUCT lpTargetObj				// 내가 공격한 인간?
		);
	virtual VOID MonsterDieAction(				// 무리 몬스터 중 특정 몬스터가 죽었을 때의 처리
		LPOBJECTSTRUCT lpObj
		);
	virtual BOOL MonsterRegenAction(			// 무리 몬스터 중 특정 몬스터가 리젠될 때의 처리
		LPOBJECTSTRUCT lpObj
		);
};

#endif

#endif // !defined(AFX_MONSTERHERD_H__E293D5C1_587C_476A_AC2D_BE0484B1D26F__INCLUDED_)
