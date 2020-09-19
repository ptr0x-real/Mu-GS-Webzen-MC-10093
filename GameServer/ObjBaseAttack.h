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
	DT_SUMMONDOT	= 6,	// ȯ�� ���� ������
#endif	
};

#define DT_COMBO		0x80
#define DT_DOUBLEDAMAGE	0x40

// ���� ó���� �⺻ ���� ó������ ���⼭ �Ѵ�
class CObjBaseAttack  
{
public:
	CObjBaseAttack();
	virtual ~CObjBaseAttack();

	// ������ ȭ�� ����
	BOOL	DecreaseArrow(LPOBJECTSTRUCT lpObj);

	// ���� ���� üũ
	BOOL	CheckAttackArea(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

	// PK üũ
	BOOL	PkCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

	// ���׷� üũ	
	int		ResistanceCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill);

	// �̽�(Miss) üũ
#ifdef DARKLORD_WORK
	virtual BOOL	MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss);
#else
	BOOL	MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	BOOL MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int magicsend, int& bAllMiss );
#endif	

	// ����� ������ ��� �´�
	int		GetTargetDefense(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, BYTE& MsgDamage);
	
#ifdef DARKLORD_WORK
	// ��Ƽ���� ü���� ��´�(��Ƽ���� �� ��ȯ)	
	int		GetPartyMemberCount(LPOBJECTSTRUCT lpObj);
#endif
};

#endif // !defined(AFX_OBJBASEATTACK_H__F928A6F8_C53B_4163_B40C_8930B1A3D681__INCLUDED_)
