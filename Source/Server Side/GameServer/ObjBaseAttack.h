// ObjBaseAttack.h: interface for the CObjBaseAttack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJBASEATTACK_H__F928A6F8_C53B_4163_B40C_8930B1A3D681__INCLUDED_)
#define AFX_OBJBASEATTACK_H__F928A6F8_C53B_4163_B40C_8930B1A3D681__INCLUDED_

#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum eDamageType
{
	DT_NONE			= 0,
	DT_PERFECT		= 1,
	DT_EXCELLENT	= 2,
	DT_CRITICAL		= 3,
	DT_MIRROR		= 4,
	DT_POISON		= 5,	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	DT_SUMMONDOT	= 6,	// 환수 지속 데미지
#endif	
};

#define DT_COMBO		0x80
#define DT_DOUBLEDAMAGE	0x40

// 공격 처리시 기본 조건 처리등을 여기서 한다
class CObjBaseAttack  
{
public:
	CObjBaseAttack();
	virtual ~CObjBaseAttack();

	// 요정의 화살 감소
	BOOL	DecreaseArrow(LPOBJECTSTRUCT lpObj);

	// 공격 영역 체크
	BOOL	CheckAttackArea(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

	// PK 체크
	BOOL	PkCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

	// 저항력 체크	
	int		ResistanceCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill);

	// 미스(Miss) 체크
#ifdef DARKLORD_WORK
	virtual BOOL	MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss);
#else
	BOOL	MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	BOOL MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss );
#endif	

	// 대상의 방어력을 얻어 온다
	int		GetTargetDefense(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, BYTE& MsgDamage);
	
#ifdef DARKLORD_WORK
	// 파티원의 체력을 깍는다(파티원의 수 반환)	
	int		GetPartyMemberCount(LPOBJECTSTRUCT lpObj);
#endif
};

#endif // !defined(AFX_OBJBASEATTACK_H__F928A6F8_C53B_4163_B40C_8930B1A3D681__INCLUDED_)
