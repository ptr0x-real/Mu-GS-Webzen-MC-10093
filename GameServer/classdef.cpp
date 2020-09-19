// classdef.cpp: implementation of the classdef class.
// ĳ���� �⺻ ������ �ٷ�� Ŭ������.
// ĳ���Ͱ� �����ɶ� �� Ŭ������ ������ �������� ���������.
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
}

classdef::~classdef()
{
}

void classdef::Init()
{
/**********************************************************************************************************************
				        Class * Str * Dex * Vit * Energy * Life * Mana * LevelLife * LevelMana * VitalityToLife * EnergyToMana 
***********************************************************************************************************************/
	SetCharacter( CLASS_WIZARD,  18,   18,  15,     30,     60,    60,        1,		2,	        	1,  		2);
	SetCharacter( CLASS_KNIGHT,  28,   20,  25,     10,    110,    20,        2,	  0.5,				3,			1);
	SetCharacter( CLASS_ELF	  ,  22,   25,  20,     15,     80,    30,        1,	  1.5,    			2,		  1.5);
	SetCharacter(CLASS_MAGUMSA,  26,   26,  26,     26,    110,    60,        1,		1,				2,			2);
}

//////////////////////////////////////////////////////////////////////
// �Էµ� ���ڷ� �⺻ ĳ���͸� �����.
//////////////////////////////////////////////////////////////////////
void classdef::SetCharacter( int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana )
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
	SetEquipment(Class);
}

//////////////////////////////////////////////////////////////////////
// ĳ���� �⺻ ��� ������Ų��.
//////////////////////////////////////////////////////////////////////
void classdef::SetEquipment(int Class)
{
	int def_type=0;
	int right_type=0;

	for( int n=0; n<MAX_EQUIPMENT; n++) 
		DefClass[Class].Equipment[n].m_Type = -1;

	// ������ ���� �⺻ ���⸦ �����Ѵ�.
	switch( Class )
	{
	case CLASS_KNIGHT : //���� (��������)
		def_type   = ItemGetNumberMake(1, 0);	
		break;	
	case CLASS_ELF    : //���� (Ȱ�� ȭ��)
		def_type   = ItemGetNumberMake(4, 0);
		right_type = ItemGetNumberMake(4, 15);	
		break;	
	case CLASS_MAGUMSA: //���˻� (��������/ª����)
		def_type   = ItemGetNumberMake(6, 0);
		right_type = ItemGetNumberMake(0, 1);
		break;	
	}
	if( Class != CLASS_WIZARD )
	{
		if( Class == CLASS_ELF || Class == CLASS_MAGUMSA )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(right_type, 0);
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT ].Convert(def_type, 0);
		}
		else
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(def_type, 0);
		}
	}
}


// ĳ������ �⺻ �ɷ�ġ �� ����?
int classdef::GetDefPoint(int char_class)
{
	if( char_class < 0 || char_class > MAX_CLASSTYPE-1 )  return 0;
	return DefClass[char_class].Strength+DefClass[char_class].Dexterity+DefClass[char_class].Vitality+DefClass[char_class].Energy;
}