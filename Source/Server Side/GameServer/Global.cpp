#include "stdafx.h"
#include "Global.h"
#include "LogProc.h"

#include "Include\Readscript.h"

CGlobalConfig g_GlobalConfig;

void CGlobalConfig::Load(const char *filename, const char *filename2)
{
	//Personal ID
	this->m_bPersonalID = GetPrivateProfileInt("GameServerInfo","UsePersonalID", 0, filename) > 0;

	//Skill
	this->m_bCheckSkill = GetPrivateProfileInt("GameServerInfo","CheckSkillLevel", 1, filename) > 0;

	this->m_bDisableTwisting = GetPrivateProfileInt("GameServerInfo","DisableTwistingPVP", 0, filename) > 0;
	this->m_bDisableBlow = GetPrivateProfileInt("GameServerInfo","DisableBlowPVP", 0, filename) > 0;
	this->m_bDisableDecay = GetPrivateProfileInt("GameServerInfo","DisableDecayPVP", 0, filename) > 0;
	this->m_bDisableCombo = GetPrivateProfileInt("GameServerInfo","DisableComboPVP", 0, filename) > 0;

	// Combo Formula
	this->iComboDamage = GetPrivateProfileInt("GameServerInfo","FormulaComboDamage", 0, filename) > 0;

	//CloseCountTime
	this->CloseCountTime = GetPrivateProfileInt("GameServerInfo","CloseCount", 0, filename) > 0;

	this->_Time = GetPrivateProfileInt("GameServerInfo","ItemDurationTime", 0, filename) > 0;

	//Items
	this->m_bCheckItem = GetPrivateProfileInt("GameServerInfo","CheckItem", 1, filename) > 0;

	this->m_bLimitTrade = GetPrivateProfileInt("GameServerInfo","DisableFullTrade", 0, filename) > 0;

	this->m_iLimitOption = GetPrivateProfileInt("GameServerInfo","MinTradeItemExOption", 6, filename);

	this->m_iLimitOption = (this->m_iLimitOption < 1 || this->m_iLimitOption > 6 ? 6 : this->m_iLimitOption);

	//Monsters & War
	this->m_bMonsterKill_War = GetPrivateProfileInt("GameServerInfo", "WarMonsterKill", 0, filename) > 0;
	this->m_bMonsterKill_Duel = GetPrivateProfileInt("GameServerInfo", "DuelMonsterKill", 1, filename) > 0;

	//Checksum
	this->m_bEnableChecksum = GetPrivateProfileInt("GameServerInfo","UseChecksum", 0, filename) > 0;

	//Jewel of Life
	this->m_iLifeRate = GetPrivateProfileInt("GameServerInfo","LifeRateWithoutLuck", 50, filename);

	//Jewel of Soul
	this->m_iSoulRate = GetPrivateProfileInt("GameServerInfo","SoulRateWithoutLuck", 50, filename);
	this->m_iSoulRateWithLuck = GetPrivateProfileInt("GameServerInfo","SoulRateWithLuck", 75, filename);

	//Inner Strength
	this->m_iInnerTime = GetPrivateProfileInt("GameServerInfo","InnerStrengthTime", 10, filename);
	this->m_iInnerVitality = GetPrivateProfileInt("GameServerInfo","InnerStrengthVitality", 100, filename);
	this->m_iInnerEnergy = GetPrivateProfileInt("GameServerInfo","InnerStrengthEnergy", 20, filename);
	this->m_iInnerMaxHP = GetPrivateProfileInt("GameServerInfo","InnerStrengthMaxAddHP", 100, filename);

	//Mana Shield
	this->m_iShieldDexterity = GetPrivateProfileInt("GameServerInfo","ManaShieldDexterity", 50, filename);
	this->m_iShieldEnergy = GetPrivateProfileInt("GameServerInfo","ManaShieldEnergy", 200, filename);
	this->m_iShieldTime = GetPrivateProfileInt("GameServerInfo","ManaShieldTime", 40, filename);
	this->m_iShieldMaxDefense = GetPrivateProfileInt("GameServerInfo","ManaShieldMaxDefense", 100, filename);

	//Chaos Machine Item +10 +11 / MAX Rates
	this->m_iCombination10Rate = GetPrivateProfileInt("GameServerInfo","Combination10Rate", 50, filename);
	this->m_iCombination11Rate = GetPrivateProfileInt("GameServerInfo","Combination11Rate", 45, filename);
	this->m_iCombination12Rate = GetPrivateProfileInt("GameServerInfo","Combination12Rate", 45, filename);
	this->m_iCombination13Rate = GetPrivateProfileInt("GameServerInfo","Combination13Rate", 45, filename);
	this->m_iMaxCombinationRate = GetPrivateProfileInt("GameServerInfo","MaxCombinationRate", 75, filename);

	this->m_iWingCombinationRate = GetPrivateProfileInt("GameServerInfo","MinWingMixRate",0, filename)%101;
	this->m_iThirdWingCombinationRate = GetPrivateProfileInt("GameServerInfo","MinThirdWingMixRate",0, filename)%101;

	//Stats
	this->m_iMax_Stats = GetPrivateProfileInt("GameServerInfo","MaxStats", 32767, filename);

	this->m_iNormal_Points = GetPrivateProfileInt("GameServerInfo","NormalUpPoints", 5, filename);
	this->m_iSpecial_Points = GetPrivateProfileInt("GameServerInfo","SpecialUpPoints", 7, filename);
	this->m_iPowerUP_Points = GetPrivateProfileInt("GameServerInfo","ExtraUpPoints", 1, filename);

	this->m_iMaxLevel = GetPrivateProfileInt("GameServerInfo","MaxLevel",400, filename);


	//Potions
	this->m_iApple_Potion = GetPrivateProfileInt("GameServerInfo","ApplePotion", 10, filename);
	this->m_iSmallHP_Potion = GetPrivateProfileInt("GameServerInfo","HPSmallPotion", 20, filename);
	this->m_iMediumHP_Potion = GetPrivateProfileInt("GameServerInfo","HPMediumPotion", 30, filename);
	this->m_iLargeHP_Potion = GetPrivateProfileInt("GameServerInfo","HPLargePotion", 40, filename);

	this->m_iSmallMP_Potion = GetPrivateProfileInt("GameServerInfo","MPSmallPotion", 20, filename);
	this->m_iMediumMP_Potion = GetPrivateProfileInt("GameServerInfo","MPMediumPotion", 30, filename);
	this->m_iLargeMP_Potion = GetPrivateProfileInt("GameServerInfo","MPLargePotion", 40, filename);

	this->m_iMaxPotionDurability = GetPrivateProfileInt("GameServerInfo","MaxPotionDurability", 255, filename);

	//Global
	this->m_bGlobalNick = GetPrivateProfileInt("GameServerInfo","GlobalNick",0, filename) > 0;

	//Elf Aura
	this->m_fPlusHeal = (float)GetPrivateProfileInt("GameServerInfo","HealAuraRate",100, filename)/100.0f;
	this->m_fPlusDmg = (float)GetPrivateProfileInt("GameServerInfo","DamageAuraRate",100, filename)/100.0f;
	this->m_fPlusDef = (float)GetPrivateProfileInt("GameServerInfo","DefenseAuraRate",100, filename)/100.0f;

	//Make
	this->m_bDisableMake = GetPrivateProfileInt("GameServerInfo","DisableMake", 0, filename) > 0;

	//Feather
	this->m_bFeatherOnlyIcarus = GetPrivateProfileInt("GameServerInfo","DropFeatherOnlyIcarus", 1, filename) > 0;
	this->m_bMonarchOnlyIcarus = GetPrivateProfileInt("GameServerInfo","DropMonarchOnlyIcarus", 0, filename) > 0;
	this->m_bSpiritPotOnlyIcarus = GetPrivateProfileInt("GameServerInfo","DropSpiritPotOnlyIcarus", 0, filename) > 0;
	this->m_bHorsePotOnlyIcarus = GetPrivateProfileInt("GameServerInfo","DropHorsePotOnlyIcarus", 0, filename) > 0;

	//Limit Life
	this->m_bLimitLife = GetPrivateProfileInt("GameServerInfo","LimitLife", 0, filename) > 0;
	this->m_iLimitLife = GetPrivateProfileInt("GameServerInfo","MaxLifeLimit", 150000, filename);

	//Rates
	this->m_iExcellentDrop = GetPrivateProfileInt("GameServerInfo","ExcellentDrop", 2000, filename);
	this->m_iAncientDrop = GetPrivateProfileInt("GameServerInfo","AncientDropRate", 25, filename);

	if(this->m_iAncientDrop > 100)
		this->m_iAncientDrop = 100;
	else if(this->m_iAncientDrop < 0)
		this->m_iAncientDrop = 0;

	this->m_iAncientDrop *= 100;

	//Q & W Command Money
	this->m_iQCommandMoney = GetPrivateProfileInt("GameServerInfo","QCommandMoney", 0, filename);
	this->m_iWCommandMoney = GetPrivateProfileInt("GameServerInfo","WCommandMoney", 0, filename);

	//Client Check
	this->m_bCheckClientSerial = GetPrivateProfileInt("GameServerInfo","CheckClientSerial", 1, filename) > 0;

	//Bless Bug
	this->m_bBlessBug = GetPrivateProfileInt("GameServerInfo","BlessBug", 1, filename) > 0;

	//Miss Rate
	this->m_iMaxMissRate = GetPrivateProfileInt("GameServerInfo","MaxMissRate", 10000, filename);

	//Guild War Score
	this->m_iGuildWarScore = GetPrivateProfileInt("GameServerInfo","GuildWarScore", 1, filename);

	//UDP Port
	this->m_iUDPPort = GetPrivateProfileInt("GameServerInfo", "UDPPort", 60006, filename);

	//Connect Message
	GetPrivateProfileString("GameServerInfo","ConnectMessage","Mirage Continent",this->m_szConnectMessage,sizeof(this->m_szConnectMessage),filename);
	GetPrivateProfileString("GameServerInfo","GuardMessage","Mirage Continent",this->m_szGuardMessage,sizeof(this->m_szGuardMessage),filename);

	//ELF Aura Time
	this->m_iDamageAuraTime = GetPrivateProfileInt("GameServerInfo", "DamageAuraTime", 60, filename);
	this->m_iDefenseAuraTime = GetPrivateProfileInt("GameServerInfo", "DefenseAuraTime", 60, filename);

	this->LoadAuthorityTable("..\\data\\GmSystem.cfg");

	m_bBackSpring = GetPrivateProfileInt("GameServerInfo","BackSpringCheck", 1, filename) > 0;
}

void CGlobalConfig::LoadAuthorityTable(const char *filename)
{
	SMDToken Token;

	SMDFile = fopen(filename,"r");

	if(SMDFile == NULL) return;

	this->m_mAuthorityCode.clear();

	int iSection = -1;

	DWORD dwCommandID, dwCommandAuthority;

	while(true)
	{
		Token = GetToken();

		if(Token == END)
			break;
		else if(Token == NUMBER)
		{
			iSection = TokenNumber;

			if(iSection == 0)
			{
				while(true)
				{
					dwCommandAuthority = 0;

					Token = GetToken();

					if(Token == NAME && strcmp("end",TokenString) == 0)
						break;

					dwCommandID = TokenNumber;

					Token = GetToken();
					if(!TokenNumber)
						dwCommandAuthority = AUTHORITY_DISABLE;

					if(dwCommandAuthority != AUTHORITY_DISABLE)
					{
						Token = GetToken();
						if(TokenNumber)
							dwCommandAuthority |= AUTHORITY_USER;

						Token = GetToken();
						if(TokenNumber)
							dwCommandAuthority |= AUTHORITY_VIP_USER;

						Token = GetToken();
						if(TokenNumber)
							dwCommandAuthority |= AUTHORITY_ADMIN;

						Token = GetToken();
						if(TokenNumber)
							dwCommandAuthority |= AUTHORITY_EVENT_GM;
					}
					else
					{
						Token = GetToken();
						Token = GetToken();
						Token = GetToken();
						Token = GetToken();
					}

					this->m_mAuthorityCode.insert(std::make_pair(dwCommandID, dwCommandAuthority));
				};
			}
		}
	};

	fclose(SMDFile);
}