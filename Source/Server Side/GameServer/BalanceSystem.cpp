#include "StdAfx.h"
#include "GameMain.h"
#include "user.h"

cBalanceSystem gBalanceSystem;

cBalanceSystem::cBalanceSystem()
{
	this->Init();
}

cBalanceSystem::~cBalanceSystem()
{

}

void cBalanceSystem::Init()
{
	this->Enable = true;

	this->DKvsDK = 100;
	this->DKvsDW = 100;
	this->DKvsELF = 100;
	this->DKvsMG = 100;
	this->DKvsDL = 100;

	this->DWvsDW = 100;
	this->DWvsDK = 100;
	this->DWvsELF = 100;
	this->DWvsMG = 100;
	this->DWvsDL = 100;

	this->ELFvsELF = 100;
	this->ELFvsDK = 100;
	this->ELFvsDW = 100;
	this->ELFvsMG = 100;
	this->ELFvsDL = 100;

	this->MGvsMG = 100;
	this->MGvsDK = 100;
	this->MGvsDW = 100;
	this->MGvsELF = 100;
	this->MGvsDL = 100;

	this->DLvsDL = 100;
	this->DLvsDK = 100;
	this->DLvsDW = 100;
	this->DLvsELF = 100;
	this->DLvsMG = 100;
}

void cBalanceSystem::ReadConfigs(char * File)
{
	this->Enable = GetPrivateProfileInt("Common", "Enable", 1, File);

	this->DKvsDK = GetPrivateProfileInt("Common", "DKvsDK", 100, File);
	this->DKvsDW = GetPrivateProfileInt("Common", "DKvsDW", 100, File);
	this->DKvsELF = GetPrivateProfileInt("Common", "DKvsELF", 100, File);
	this->DKvsMG = GetPrivateProfileInt("Common", "DKvsMG", 100, File);
	this->DKvsDL = GetPrivateProfileInt("Common", "DKvsDL", 100, File);

	this->DWvsDW = GetPrivateProfileInt("Common", "DWvsDW", 100, File);
	this->DWvsDK = GetPrivateProfileInt("Common", "DWvsDK", 100, File);
	this->DWvsELF = GetPrivateProfileInt("Common", "DWvsELF", 100, File);
	this->DWvsMG = GetPrivateProfileInt("Common", "DWvsMG", 100, File);
	this->DWvsDL = GetPrivateProfileInt("Common", "DWvsDL", 100, File);

	this->ELFvsELF = GetPrivateProfileInt("Common", "ELFvsELF", 100, File);
	this->ELFvsDK = GetPrivateProfileInt("Common", "ELFvsDK", 100, File);
	this->ELFvsDW = GetPrivateProfileInt("Common", "ELFvsDW", 100, File);
	this->ELFvsMG = GetPrivateProfileInt("Common", "ELFvsMG ", 100, File);
	this->ELFvsDL = GetPrivateProfileInt("Common", "ELFvsDL", 100, File);

	this->MGvsMG = GetPrivateProfileInt("Common", "MGvsMG", 100, File);
	this->MGvsDK = GetPrivateProfileInt("Common", "MGvsDK", 100, File);
	this->MGvsDW = GetPrivateProfileInt("Common", "MGvsDW", 100, File);
	this->MGvsELF = GetPrivateProfileInt("Common", "MGvsELF", 100, File);
	this->MGvsDL = GetPrivateProfileInt("Common", "MGvsDL", 100, File);

	this->DLvsDL = GetPrivateProfileInt("Common", "DLvsDL", 100, File);
	this->DLvsDK = GetPrivateProfileInt("Common", "DLvsDK", 100, File);
	this->DLvsDW = GetPrivateProfileInt("Common", "DLvsDW", 100, File);
	this->DLvsELF = GetPrivateProfileInt("Common", "DLvsELF", 100, File);
	this->DLvsMG = GetPrivateProfileInt("Common", "DLvsMG", 100, File);
}

void cBalanceSystem::Load(char * File)
{
	this->Init();

	this->ReadConfigs(File);

	LogAddC(LOGC_BLUE, "Load Balance System Active %d", this->Enable);
}

void cBalanceSystem::Main(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTarget, int & AttackDamage)
{
	if (!this->Enable) return;

	if (lpObj->Type != OBJTYPE_CHARACTER || lpTarget->Type != OBJTYPE_CHARACTER)
	{
		return;
	}

	if (lpObj->Class == CLASS_KNIGHT)
	{
		switch (lpTarget->Class)
		{
		case CLASS_KNIGHT:		AttackDamage = (AttackDamage * this->DKvsDK) / 100; break;
		case CLASS_WIZARD:		AttackDamage = (AttackDamage * this->DKvsDW) / 100; break;
		case CLASS_ELF:			AttackDamage = (AttackDamage * this->DKvsELF) / 100; break;
		case CLASS_MAGUMSA:		AttackDamage = (AttackDamage * this->DKvsMG) / 100; break;
		case CLASS_DARKLORD:	AttackDamage = (AttackDamage * this->DKvsDL) / 100; break;
		default:return;
		}
	}
	else if (lpObj->Class == CLASS_WIZARD)
	{
		switch (lpTarget->Class)
		{
		case CLASS_WIZARD:		AttackDamage = (AttackDamage * this->DWvsDW) / 100; break;
		case CLASS_KNIGHT:		AttackDamage = (AttackDamage * this->DWvsDK) / 100; break;
		case CLASS_ELF:			AttackDamage = (AttackDamage * this->DWvsELF) / 100; break;
		case CLASS_MAGUMSA:		AttackDamage = (AttackDamage * this->DWvsMG) / 100; break;
		case CLASS_DARKLORD:	AttackDamage = (AttackDamage * this->DWvsDL) / 100; break;
		default:return;
		}
	}
	else if (lpObj->Class == CLASS_ELF)
	{
		switch (lpTarget->Class)
		{
		case CLASS_ELF:			AttackDamage = (AttackDamage * this->ELFvsELF) / 100; break;
		case CLASS_KNIGHT:		AttackDamage = (AttackDamage * this->ELFvsDK) / 100; break;
		case CLASS_WIZARD:		AttackDamage = (AttackDamage * this->ELFvsDW) / 100; break;
		case CLASS_MAGUMSA:		AttackDamage = (AttackDamage * this->ELFvsMG) / 100; break;
		case CLASS_DARKLORD:	AttackDamage = (AttackDamage * this->ELFvsDL) / 100; break;
		default:return;
		}
	}
	else if (lpObj->Class == CLASS_MAGUMSA)
	{
		switch (lpTarget->Class)
		{
		case CLASS_MAGUMSA:		AttackDamage = (AttackDamage * this->MGvsMG) / 100; break;
		case CLASS_KNIGHT:		AttackDamage = (AttackDamage * this->MGvsDK) / 100; break;
		case CLASS_WIZARD:		AttackDamage = (AttackDamage * this->MGvsDW) / 100; break;
		case CLASS_ELF:			AttackDamage = (AttackDamage * this->MGvsELF) / 100; break;
		case CLASS_DARKLORD:	AttackDamage = (AttackDamage * this->MGvsDL) / 100; break;
		default:return;
		}
	}
	else if (lpObj->Class == CLASS_DARKLORD)
	{
		switch (lpTarget->Class)
		{
		case CLASS_DARKLORD:	AttackDamage = (AttackDamage * this->DLvsDL) / 100; break;
		case CLASS_KNIGHT:		AttackDamage = (AttackDamage * this->DLvsDK) / 100; break;
		case CLASS_WIZARD:		AttackDamage = (AttackDamage * this->DLvsDW) / 100; break;
		case CLASS_ELF:			AttackDamage = (AttackDamage * this->DLvsELF) / 100; break;
		case CLASS_MAGUMSA:		AttackDamage = (AttackDamage * this->DLvsMG) / 100; break;
		default:return;
		}
	}
}
