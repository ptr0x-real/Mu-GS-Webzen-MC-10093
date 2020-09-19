#include "stdafx.h"
#include "CalCharacter.h"
#include "LogProc.h"

bool CBalancer::Load()
{
	// Formulas
	AttackDamageMin_Bow_StrDexDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMin_Bow_StrDexDiv_Elf", 7, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_Bow_StrDexDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMax_Bow_StrDexDiv_Elf", 4, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_NoBow_DexDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMin_NoBow_DexDiv_Elf", 7, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_NoBow_StrDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMin_NoBow_StrDiv_Elf", 14, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_NoBow_DexDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMax_NoBow_DexDiv_Elf", 4, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_NoBow_StrDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMax_NoBow_StrDiv_Elf", 8, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_DexStrDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMin_DexStrDiv_Elf", 7, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_DexStrDiv_Elf = GetPrivateProfileInt("Damage","AttackDamageMax_DexStrDiv_Elf", 4, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_StrDiv_DK = GetPrivateProfileInt("Damage","AttackDamageMin_StrDiv_DK", 6, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_StrDiv_DK = GetPrivateProfileInt("Damage","AttackDamageMax_StrDiv_DK", 4, "..\\data\\CalCharacter.cfg");

	AttackDamageMin_StrDiv_MG = GetPrivateProfileInt("Damage","AttackDamageMin_StrDiv_MG", 6, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_EneDiv_MG = GetPrivateProfileInt("Damage","AttackDamageMin_EneDiv_MG", 12, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_StrDiv_MG = GetPrivateProfileInt("Damage","AttackDamageMax_StrDiv_MG", 4, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_EneDiv_MG = GetPrivateProfileInt("Damage","AttackDamageMax_EneDiv_MG", 8, "..\\data\\CalCharacter.cfg");

	AttackDamageMin_StrDiv_DW = GetPrivateProfileInt("Damage","AttackDamageMin_StrDiv_DW", 8, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_StrDiv_DW = GetPrivateProfileInt("Damage","AttackDamageMax_StrDiv_DW", 4, "..\\data\\CalCharacter.cfg");
	MagicDamageMin = GetPrivateProfileInt("Damage","MagicDamageMin_StrDiv_DW", 9, "..\\data\\CalCharacter.cfg");
	MagicDamageMax = GetPrivateProfileInt("Damage","MagicDamageMax_StrDiv_DW", 4, "..\\data\\CalCharacter.cfg");
	RightDamageMinDW = GetPrivateProfileInt("Damage","RightDamageMin_StrDiv_DW", 2, "..\\data\\CalCharacter.cfg");
	RightDamageMaxDW = GetPrivateProfileInt("Damage","RightDamageMax_StrDiv_DW", 2, "..\\data\\CalCharacter.cfg");

	Defense_Elf = GetPrivateProfileInt("Defense","Defense_Elf", 10, "..\\data\\CalCharacter.cfg");
	Defense_DK = GetPrivateProfileInt("Defense","Defense_DK", 3, "..\\data\\CalCharacter.cfg");
	Defense_DW_MG = GetPrivateProfileInt("Defense","Defense_DW_MG", 4, "..\\data\\CalCharacter.cfg");

	SuccessfulBlocking_Elf = GetPrivateProfileInt("Defense","SuccessfulBlocking_Elf", 4, "..\\data\\CalCharacter.cfg");
	SuccessfulBlocking_DW = GetPrivateProfileInt("Defense","SuccessfulBlocking_DW", 3, "..\\data\\CalCharacter.cfg");
	SuccessfulBlocking_DK = GetPrivateProfileInt("Defense","SuccessfulBlocking_DK", 3, "..\\data\\CalCharacter.cfg");
	SuccessfulBlocking_MG = GetPrivateProfileInt("Defense","SuccessfulBlocking_MG", 3, "..\\data\\CalCharacter.cfg");
	OtherSuccessfulBlocking = GetPrivateProfileInt("Defense","OtherSuccessfulBlocking", 3, "..\\data\\CalCharacter.cfg");

	AttackSpeed_Elf = GetPrivateProfileInt("Speed","AttackSpeed_Elf", 50, "..\\data\\CalCharacter.cfg");
	MagicSpeed_Elf = GetPrivateProfileInt("Speed","MagicSpeed_Elf", 50, "..\\data\\CalCharacter.cfg");
	AttackSpeed_DK_MG = GetPrivateProfileInt("Speed","AttackSpeed_DK_MG", 15, "..\\data\\CalCharacter.cfg");
	MagicSpeed_DK_MG = GetPrivateProfileInt("Speed","MagicSpeed_DK_MG", 20, "..\\data\\CalCharacter.cfg");
	AttackSpeed_DW = GetPrivateProfileInt("Speed","AttackSpeed_DW", 20, "..\\data\\CalCharacter.cfg");
	MagicSpeed_DW = GetPrivateProfileInt("Speed","MagicSpeed_DW", 10, "..\\data\\CalCharacter.cfg");

#ifdef DARKLORD_WORK
	AttackDamageMin_StrDiv_DL = GetPrivateProfileInt("Damage","AttackDamageMin_StrDiv_DL", 7, "..\\data\\CalCharacter.cfg");
	AttackDamageMin_EneDiv_DL = GetPrivateProfileInt("Damage","AttackDamageMin_EneDiv_DL", 14, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_StrDiv_DL = GetPrivateProfileInt("Damage","AttackDamageMax_StrDiv_DL", 5, "..\\data\\CalCharacter.cfg");
	AttackDamageMax_EneDiv_DL = GetPrivateProfileInt("Damage","AttackDamageMax_EneDiv_DL", 10, "..\\data\\CalCharacter.cfg");

	Defense_DL = GetPrivateProfileInt("Defense","Defense_DL", 7, "..\\data\\CalCharacter.cfg");
	SuccessfulBlocking_DL = GetPrivateProfileInt("Defense","SuccessfulBlocking_DL", 7, "..\\data\\CalCharacter.cfg");
	AttackSpeed_DL = GetPrivateProfileInt("Speed","AttackSpeed_DL", 10, "..\\data\\CalCharacter.cfg");
	MagicSpeed_DL = GetPrivateProfileInt("Speed","MagicSpeed_DL", 10, "..\\data\\CalCharacter.cfg");
#endif

	LogAdd("[CalCharacter.cfg] Load() completed");

	return true;
}

CBalancer Balancer;