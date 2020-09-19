// ObjCalCharacter.h: interface for the CObjCalCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJCALCHARACTER_H__77C32896_DA7B_41D7_9849_14F0A457DF08__INCLUDED_)
#define AFX_OBJCALCHARACTER_H__77C32896_DA7B_41D7_9849_14F0A457DF08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef UPDATE_GM_FUNCTION_20070228	// GM 캐릭터 인덱스 378
#define  NEW_GM_CHARACTER	378
#endif 

extern void gObjCalCharacter(int aIndex);

#ifdef MODIFY_ITEM_EFFECT_FLOW_20060519
	extern void GObjExtItemApply(LPOBJECTSTRUCT lpObj, int type = 0);  // type - 0 : 값증가 옵션, 1 : %증가 옵션
#else
	extern void GObjExtItemApply(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_ITEM_EFFECT_FLOW_20060519

extern void	gObjCalcSetItemStat(LPOBJECTSTRUCT lpObj);

#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
extern int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg, bool& bSendExp);
#else
extern int gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg);
#endif

extern void gObjMonsterExpDivision(LPOBJECTSTRUCT lpMonObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int MSBFlag);

#endif // !defined(AFX_OBJCALCHARACTER_H__77C32896_DA7B_41D7_9849_14F0A457DF08__INCLUDED_)
