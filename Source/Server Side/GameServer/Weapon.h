// Weapon.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPON_H__683A17E4_8061_4CAA_A95B_14A56DC69824__INCLUDED_)
#define AFX_WEAPON_H__683A17E4_8061_4CAA_A95B_14A56DC69824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_NEW_WEAPON_FOR_CASTLE_01_20041116

#include "Include\DEFINE.H"

#include "Common\TSync.h"

#define MAX_WEAPON_DAMAGED_TARGET_INFO	100
#define MAX_WEAPON_CAL_DAMAGE_INFO		50

enum WEAPON_STATE
{
	// 기본 상태
	WEAPON_STATE_NORMAL		= 0x00,
	// 사용 할수 있음
	WEAPON_STATE_USABLE		= 0x01,
	// 사용 중
	WEAPON_STATE_USING		= 0x02,
};

enum WEAPON_TYPE
{
	// 공성측 무기
	WEAPON_TYPE_CASTLE_ATTACKER = 0x01,
	// 수성측 무기
	WEAPON_TYPE_CASTLE_DEFENDER = 0x02,
	// 가디언
	WEAPON_TYPE_CASTLE_GUARDIAN	= 0x03,
};

enum WEAPON_USE_RESULT
{
	// 무기 사용 실패
	WEAPON_USE_FAIL		= 0x00,
	// 무기 사용 성공 - 애니매이션 시작.
	WEAPON_USE_SUCCESS	= 0x01,
};


#define MAX_WEAPON_CS_ATTACKER_TARETINDEX	4
const BYTE WEAPON_CS_ATTACKER[MAX_WEAPON_CS_ATTACKER_TARETINDEX][4] = 
{		
	62,		103,	72,		112,
	88,		104,	124,	111,
	116,	105,	124,	112,
	73,		86,		105,	103
};

// 수성 무기 타겟 범위
#define MAX_WEAPON_CS_DEFENSER_TARETINDEX	3
const BYTE WEAPON_CS_DEFENSER[MAX_WEAPON_CS_ATTACKER_TARETINDEX][4] = 
{		
	61,		88,		93,		108,
	92,		89,		127,	111,
	84,		52,		102,	66
};

typedef struct 
{
	INT		m_iWeaponObjIndex;
	INT		m_iTargetObjIndex;
	INT		m_iDamageDealingTime;

	BOOL	m_IsUsed;

	void RESET()
	{
		m_iWeaponObjIndex		= -1;
		m_iTargetObjIndex		= -1;
		m_iDamageDealingTime	= 0;
		m_IsUsed				= FALSE;
	};

}ST_WEAPON_DAMAGED_TARGET_INFO, *LPST_WEAPON_DAMAGED_TARGET_INFO;

typedef struct 
{
	INT		m_iWeaponObjIndex;
	INT		m_iTargetX;
	INT		m_iTargetY;

	INT		m_iLimitTime;

	BOOL	m_IsUsed;

	void RESET()
	{
		m_iWeaponObjIndex	= -1;
		m_iTargetX			= -1;
		m_iTargetY			= -1;
		m_iLimitTime		= -1;
		m_IsUsed			= FALSE;
	};

}ST_WEAPON_CAL_DAMAGER_INFO, *LPST_WEAPON_CAL_DAMAGER_INFO;


class CWeapon  
{
public:
	CWeapon();
	virtual ~CWeapon();

public:
	BOOL	SetWeaponCalDamageInfo( WORD wObjIndex, BYTE btTargetX, BYTE btTargetY,  INT iDelayTime );
	BOOL	GetWeaponCalDamageInfo( WORD wObjIndex, BYTE &btTargetX, BYTE &btTargetY );

	BOOL	AddWeaponDamagedTargetInfo( INT iWeaponIndex, INT iTargetIndex, INT iDelayTime );

	VOID	WeaponAttackProc();
	VOID	WeaponAct(INT iIndex);

	BOOL	Attack( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, INT iCriticalDamage, INT iActionType = 0);
	
	INT		GetAttackDamage( INT iObjClass );
	BOOL	GetTargetPointXY( INT iObjClass, INT iTargetPointIndex, BYTE& btX, BYTE& btY, BOOL bRandomPos=TRUE);
	

	// 미스체크는 Weapon 용으로 따로 처리 한다
	BOOL	MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, INT iSkill, INT iSkillSuccess, int& bAllMiss );


public:
	ST_WEAPON_DAMAGED_TARGET_INFO	m_WeaponDamagedTargetInfo[MAX_WEAPON_DAMAGED_TARGET_INFO];
	ST_WEAPON_CAL_DAMAGER_INFO		m_WeaponCalDamageInfo[MAX_WEAPON_CAL_DAMAGE_INFO];
 
};

extern CWeapon g_CsNPC_Weapon;

#endif

#endif // !defined(AFX_WEAPON_H__683A17E4_8061_4CAA_A95B_14A56DC69824__INCLUDED_)
