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

	// �ش� ������ ���õ� �������� �ʱ�ȭ �Ѵ�. (���� ���� ���� ȿ�� ������ ����.)
	void RemoveBuffVariable(LPOBJECTSTRUCT lpObj, int iBuffIndex);
private:
};

extern CBuffEffectSlot g_BuffEffectSlot;

extern void gObjCheckBuffEffectList(LPOBJECTSTRUCT lpObj);
// ��ų�� ���� ���� ȿ��
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, BYTE EffectType1, int EffectValue1, BYTE EffectType2, int EffectValue2, int Duration);
// ������ ��뿡 ���� ���� ȿ��
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex);
#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
extern bool gObjAddBuffEffect(LPOBJECTSTRUCT lpObj, int iBuffIndex, int Duration);
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//-->CheckItemOptForGetExpEx ����
//�ؿ� ĳ�������� ����ó��
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

// ������ ���� �ð��� ����
extern bool gObjChangeBuffValidTime(LPOBJECTSTRUCT lpObj, int iBuffIndex, int iTime);

// ���� ȿ���� ���� ƽ�� �־����� ȿ�� ���� (DOT, ƽ�� ü������ �� ��)
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
