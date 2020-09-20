	// ObjAttack.h: interface for the CObjAttack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJATTACK_H__B290A6EB_3BD0_408A_9F8C_44225D54DD1B__INCLUDED_)
#define AFX_OBJATTACK_H__B290A6EB_3BD0_408A_9F8C_44225D54DD1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjBaseAttack.h"

class CObjAttack : public CObjBaseAttack
{
public:
	CObjAttack();
	virtual ~CObjAttack();

	// ���� ó��
	BOOL Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int magicsend, BYTE MSBFlag, int AttackDamage,BOOL bCombo = FALSE);

#ifdef DUEL_SYSTEM_20031028
	#ifdef ADD_SKILL_WITH_COMBO
		int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, BOOL bIsOnDuel, CMagicInf *lpMagic);	
	#else
		int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, BOOL bIsOnDuel);	
	#endif
#else
	#if defined( ADD_SKILL_WITH_COMBO ) || GAME_VERSION == G_V_97D
		int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect, CMagicInf *lpMagic);
	#else
		int  GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BYTE & effect);
	#endif
#endif

#ifdef DUEL_SYSTEM_20031028
	int  GetAttackDamageWizard(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect, BOOL bIsOnDuel);
#else
	int  GetAttackDamageWizard(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect);
#endif

#ifdef ADD_SHIELD_POINT_01_20060403
	int  GetShieldDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage);
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	int	GetAttackDamageSummoner(LPOBJECTSTRUCT lpObj, int targetDefense, CMagicInf *lpMagic, BYTE & effect, BOOL bIsOnDuel);
#endif

#ifdef SEASON4_PATCH_GROOVING
	//------>
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24
	//grooving ����Ŀ ���� ���� ���ݷ� �������� �̸� ������ �ش�.
	//���� ��, ��ø�� ���ʷ� �Ѵ�.
	bool GetBuffTypePhysicalIncrease(LPOBJECTSTRUCT lpObj, int &iAttackBerserkerMin, int &iAttackBerserkerMax);
	//<------
#endif
	
};

extern CObjAttack gclassObjAttack;

extern BOOL gObjDenorantSprite(LPOBJECTSTRUCT lpObj);
#ifdef DARKLORD_WORK
extern BOOL gObjDarkHorse(LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
extern BOOL gObjFenrir(LPOBJECTSTRUCT lpObj);
#endif

#endif // !defined(AFX_OBJATTACK_H__B290A6EB_3BD0_408A_9F8C_44225D54DD1B__INCLUDED_)
