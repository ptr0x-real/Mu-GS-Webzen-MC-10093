// classdef.h: interface for the classdef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSDEF_H__1B6C4ED5_0169_4A49_990F_E7FBB0E9DECF__INCLUDED_)
#define AFX_CLASSDEF_H__1B6C4ED5_0169_4A49_990F_E7FBB0E9DECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\\Common\\zzzItem.h"

//----------------------------------------------------------------------------
// 종족별 기본값
//----------------------------------------------------------------------------
typedef struct
{
	int 	Experience;
	WORD	Strength;
	WORD	Dexterity;
	WORD	Vitality;
	WORD	Energy;
	float   LevelLife;
	float 	Life;
	float 	MaxLife;
	float   LevelMana;	
	float 	Mana;
	float 	MaxMana;
	float   VitalityToLife;
	float 	EnergyToMana;
	CItem	Equipment[MAX_EQUIPMENT+2];

#ifdef DARKLORD_WORK
	WORD	Leadership;		// 통솔 항목 추가
#endif
	
} DEFAULTCLASSTYPE, *LPDEFAULTCLASSTYPE;

class classdef  
{
public:
	DEFAULTCLASSTYPE	DefClass[MAX_CLASSTYPE];
	BOOL	m_BabubBanJiCreate;

public:
	classdef();
	virtual ~classdef();

	void Init();

#ifdef DARKLORD_WORK	
	void SetCharacter( int Class, int Str, int Dex, int Vit, int Energy, 
					   float Life, float Mana, float LevelLife, float LevelMana, 
					   float VitalityToLife, float EnergyToMana, int Leadership = 0);
#else
	void SetCharacter(int Class, int Str, int Dex, int Vit, int Energy, 
						   float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana );
#endif
	void SetEquipment(int Class);
	int  GetDefPoint(int char_class);
	void SetMabubBanjiOption(BOOL flag);
};

#endif // !defined(AFX_CLASSDEF_H__1B6C4ED5_0169_4A49_990F_E7FBB0E9DECF__INCLUDED_)
