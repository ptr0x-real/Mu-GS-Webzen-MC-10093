// classdef.cpp: implementation of the classdef class.
// 캐릭터 기본 정보를 다루는 클래스다.
// 캐릭터가 생성될때 이 클래스의 정보를 바탕으로 만들어진다.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\\Common\\classdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




classdef::classdef()
{
	m_BabubBanJiCreate = 0;
}

classdef::~classdef()
{
}

void classdef::Init()
{
	

/**********************************************************************************************************************
				        Class * Str * Dex * Vit * Energy * Life * Mana * LevelLife * LevelMana * VitalityToLife * EnergyToMana , Leadership
***********************************************************************************************************************/
	SetCharacter( CLASS_WIZARD,  18,   18,  15,     30,     60,    60,        1,		2,	        	2,  		2);
	SetCharacter( CLASS_KNIGHT,  28,   20,  25,     10,    110,    20,        2,	  0.5,				3,			1);
	SetCharacter( CLASS_ELF	  ,  22,   25,  20,     15,     80,    30,        1,	  1.5,    			2,		  1.5);
	SetCharacter(CLASS_MAGUMSA,  26,   26,  26,     26,    110,    60,        1,		1,				2,			2);

#ifdef DARKLORD_WORK
	SetCharacter(CLASS_DARKLORD,  26,   20,  20,    15,     90,    40,        1.5f,		1,				2,			1.5, 25);
#endif
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 기본값 설정
	SetCharacter(CLASS_SUMMONER,  21,   21,  18,     23,     70,    40,        1,		1.5,        	2,  		1.5);
#endif	
}

//////////////////////////////////////////////////////////////////////
// 입력된 인자로 기본 캐릭터를 만든다.
//////////////////////////////////////////////////////////////////////
#ifdef DARKLORD_WORK		// 통솔 항목 추가
void classdef::SetCharacter( int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana, int Leadership)
#else
void classdef::SetCharacter( int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana )
#endif
{
	DefClass[Class].Experience = 0;
	DefClass[Class].Strength   = Str;
	DefClass[Class].Dexterity  = Dex;
	DefClass[Class].Vitality   = Vit;
	DefClass[Class].Energy     = Energy;
	DefClass[Class].Life	   = Life;
	DefClass[Class].MaxLife    = Life;
	DefClass[Class].LevelLife  = LevelLife;
	DefClass[Class].Mana       = Mana;
	DefClass[Class].MaxMana    = Mana;
	DefClass[Class].LevelMana  = LevelMana;
	DefClass[Class].VitalityToLife = VitalityToLife;
	DefClass[Class].EnergyToMana   = EnergyToMana;

#ifdef DARKLORD_WORK	
	DefClass[Class].Leadership = Leadership;
#endif
	SetEquipment(Class);
}

//////////////////////////////////////////////////////////////////////
// 캐릭터 기본 장비를 장착시킨다.
//////////////////////////////////////////////////////////////////////
void classdef::SetEquipment(int Class)
{
	int def_type=0;
	int right_type=0;
	int n;

	for( n=0; n<MAX_EQUIPMENT+1; n++) 
		DefClass[Class].Equipment[n].Clear();

	// 종족에 따라 기본 무기를 세팅한다.
	switch( Class )
	{
	case CLASS_KNIGHT : //흑기사 (작은도끼)
		def_type   = ItemGetNumberMake(1, 0);	
		break;	
	case CLASS_ELF    : //요정 (활과 화살)
		def_type   = ItemGetNumberMake(4, 0);
		right_type = ItemGetNumberMake(4, 15);	
		break;	

#ifdef DARKLORD_WORK
	case CLASS_DARKLORD: //다크로드 (원형방패/짧은검)
		def_type   = ItemGetNumberMake(6, 0);
		right_type = ItemGetNumberMake(0, 1);
		break;	
#endif
	case CLASS_MAGUMSA: //마검사 (원형방패/짧은검)

		def_type   = ItemGetNumberMake(6, 0);
		right_type = ItemGetNumberMake(0, 1);
		break;	
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 소환술사 생성시 기본 아이템
	case CLASS_SUMMONER:	// 소환술사 (작은도끼)
		def_type = ItemGetNumberMake(1, 0);
		break;	
#endif
	}
	
	if( Class != CLASS_WIZARD )
	{
#ifdef DARKLORD_WORK
		if( Class == CLASS_DARKLORD )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(right_type);
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT ].Convert(def_type);
		}
		else
#endif
		if( Class == CLASS_ELF || Class == CLASS_MAGUMSA )

		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(right_type);
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT ].Convert(def_type);
		}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 소환술사 생성시 기본 아이템
		else if( Class == CLASS_SUMMONER )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(def_type);
		}
#endif
		else
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(def_type);
		}
	}
	
	if( m_BabubBanJiCreate == 1 )
	{
 		// 이벤트로 용사의 반지를 추가해준다.
		def_type   = ItemGetNumberMake(13, 20);	
		DefClass[Class].Equipment[MAX_EQUIPMENT].m_Level = 1;
		DefClass[Class].Equipment[MAX_EQUIPMENT].Convert(def_type);
		
		// 이벤트로 전사의 반지를 추가해준다.
		def_type   = ItemGetNumberMake(13, 20);	
		DefClass[Class].Equipment[MAX_EQUIPMENT+1].m_Level = 2;
		DefClass[Class].Equipment[MAX_EQUIPMENT+1].Convert(def_type);
	}
}

// 캐릭터의 기본 능력치 총 합은?
int classdef::GetDefPoint(int char_class)
{
	if( char_class < 0 || char_class > MAX_CLASSTYPE-1 )  return 0;
#ifdef DARKLORD_WORK
	return DefClass[char_class].Strength+DefClass[char_class].Dexterity+DefClass[char_class].Vitality+DefClass[char_class].Energy+DefClass[char_class].Leadership;
#else
	return DefClass[char_class].Strength+DefClass[char_class].Dexterity+DefClass[char_class].Vitality+DefClass[char_class].Energy;
#endif
}

void classdef::SetMabubBanjiOption(BOOL flag)
{
	m_BabubBanJiCreate = flag;
}
