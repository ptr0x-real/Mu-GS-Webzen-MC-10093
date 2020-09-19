// BuffEffectSlot.h: interface for the CBuffEffectSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFEFFECTSLOT_H__F5BEB4BE_7E88_4E6F_AD72_FF67E3DF2823__INCLUDED_)
#define AFX_BUFFEFFECTSLOT_H__F5BEB4BE_7E88_4E6F_AD72_FF67E3DF2823__INCLUDED_

#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_20070917

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

#include "BuffEffect.h"
#include "BuffScriptLoader.h"

class CBuffEffectSlot  
{
public:
	CBuffEffectSlot();
	virtual ~CBuffEffectSlot();

	int SetEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, BYTE btEffectType1, BYTE btEffectType2, int iEffectValue1, int iEffectValue2, int iDuration );
	bool RemoveEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
	bool CheckUsedEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
	int ClearEffect(LPOBJECTSTRUCT lpObj, eBuffClearType ClearType);
	LPBUFF_EFFECT_DATA GetEffectData(int iBuffIndex);
	int GetBuffClearType(int iBufIndex);

	// 해당 버프와 관련된 변수들을 초기화 한다. (기존 버프 관련 효과 적용을 위함.)
	void RemoveBuffVariable(LPOBJECTSTRUCT lpObj, int iBuffIndex);
private:
};

extern CBuffEffectSlot g_BuffEffectSlot;

extern void gObjCheckBuffEffectList(LPOBJECTSTRUCT lpObj);
// 스킬에 의한 버프 효과
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, BYTE EffectType1, int EffectValue1, BYTE EffectType2, int EffectValue2, int Duration);
// 아이템 사용에 의한 버프 효과
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, int Duration);
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//-->CheckItemOptForGetExpEx 통합
//해외 캐쉬아이템 인장처리
extern void CheckItemOptForGetExp( LPOBJECTSTRUCT lpObj, INT64 &iExp, BOOL bRewardExp = FALSE);
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

extern bool gObjRemoveBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
extern bool gObjClearBuffEffect(LPOBJECTSTRUCT lpObj, eBuffClearType ClearType);
extern bool gObjCheckUsedBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
extern bool gObjCheckPowerfulEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, int EffectValue1, int EffectValue2);
extern int gObjMakeViewportState(LPOBJECTSTRUCT lpObj, BYTE* lpBuffer);
extern int gObjGetTotalValueOfEffect(LPOBJECTSTRUCT lpObj, int EffectType);
extern bool gObjGetValueOfBuffIndex(LPOBJECTSTRUCT lpObj, int iBuffIndex, int* EffectValue1, int* EffectValue2 = NULL);
extern void GCUseBuffEffect(LPOBJECTSTRUCT lpObj, BYTE BuffEffectIndex, BYTE EffectUseOption, WORD OptionType, WORD EffectType, int LeftTime);

// 버프의 지속 시간을 변경
extern bool gObjChangeBuffValidTime(LPOBJECTSTRUCT lpObj, int iBuffIndex, int iTime);

// 버프 효과에 의한 틱당 주어지는 효과 설정 (DOT, 틱당 체워지는 힐 등)
extern void gObjSetActiveEffectAtTick(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
extern void gObjSendBuffList(LPOBJECTSTRUCT lpObj);
#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220

#ifdef ADD_ITEM_ATTACK_XMASEVENT_20081014
BOOL gObjAddBuffEffectWideArea(LPOBJECTSTRUCT lpTargetObj, int nDistance, int &nAffectedCount
							   , int iBuffIndex, BYTE EffectType1, int EffectValue1, BYTE EffectType2, int EffectValue2, int Duration);
#endif // ADD_ITEM_ATTACK_XMASEVENT_20081014

#endif // MODIFY_BUFF_SYSTEM_EXTENTION_20070917


#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
void CheckItemOptForGetExpEx( LPOBJECTSTRUCT lpObj, INT64 &iExp, BOOL bRewardExp = FALSE);
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#endif // !defined(AFX_BUFFEFFECTSLOT_H__F5BEB4BE_7E88_4E6F_AD72_FF67E3DF2823__INCLUDED_)
