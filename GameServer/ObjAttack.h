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

	// 공격 처리
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
	// 시즌 4.0 추가 스킬  grooving 2008/04/24
	//grooving 버서커 사용시 물리 공격력 증가값을 미리 설정해 준다.
	//계산시 힘, 민첩을 기초로 한다.
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
