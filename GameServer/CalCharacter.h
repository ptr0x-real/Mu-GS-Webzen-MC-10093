#pragma once

#include "user.h"

class CBalancer
{

public:
	bool Load();

public:
	//Formulas
	int AttackDamageMin_Bow_StrDexDiv_Elf, AttackDamageMax_Bow_StrDexDiv_Elf;
	int AttackDamageMin_NoBow_DexDiv_Elf, AttackDamageMin_NoBow_StrDiv_Elf;
	int AttackDamageMax_NoBow_DexDiv_Elf, AttackDamageMax_NoBow_StrDiv_Elf;
	int AttackDamageMin_DexStrDiv_Elf, AttackDamageMax_DexStrDiv_Elf;
	int AttackDamageMin_StrDiv_DK, AttackDamageMax_StrDiv_DK;
	int AttackDamageMin_StrDiv_MG, AttackDamageMin_EneDiv_MG, AttackDamageMax_StrDiv_MG, AttackDamageMax_EneDiv_MG;
	int AttackDamageMin_StrDiv_DW, AttackDamageMax_StrDiv_DW;
	int MagicDamageMin, MagicDamageMax;

	int Defense_Elf, Defense_DK, Defense_DW_MG;

	int SuccessfulBlocking_Elf;
	int SuccessfulBlocking_DW;
	int SuccessfulBlocking_MG;
	int SuccessfulBlocking_DK;
	int OtherSuccessfulBlocking;

	int RightDamageMinDW;
	int RightDamageMaxDW;

	int AttackSpeed_Elf, MagicSpeed_Elf;
	int AttackSpeed_DK_MG, MagicSpeed_DK_MG;
	int AttackSpeed_DW, MagicSpeed_DW;

	int AttackDefenseMagicDuel;

#ifdef DARKLORD_WORK
	int AttackDamageMin_StrDiv_DL, AttackDamageMin_EneDiv_DL;
	int AttackDamageMax_StrDiv_DL, AttackDamageMax_EneDiv_DL;
	int Defense_DL;
	int SuccessfulBlocking_DL;
	int AttackSpeed_DL, MagicSpeed_DL;
#endif
};

extern CBalancer Balancer;