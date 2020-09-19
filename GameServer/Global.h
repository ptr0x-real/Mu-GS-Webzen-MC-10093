#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

class CGlobalConfig
{
public:
	void Load(const char *filename, const char *filename2);
	void LoadAuthorityTable(const char *filename);

	bool CheckCommand(DWORD command_code)
	{
		return (this->m_mAuthorityCode.find(command_code) != this->m_mAuthorityCode.end());
	}

	DWORD GetCommandAuthority(DWORD command_code)
	{
		return this->m_mAuthorityCode.find(command_code)->second;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------
	//                                            FUNCIONES ARRIBA - CONFIGURACIONES ABAJO                                             //
	//-----------------------------------------------------------------------------------------------------------------------------------

public:
	BOOL m_bPersonalID;

	BOOL m_bCheckSkill, m_bCheckItem, m_bDisableMake, m_bFeatherOnlyIcarus, m_bMonarchOnlyIcarus, m_bSpiritPotOnlyIcarus, m_bHorsePotOnlyIcarus;

	BOOL m_bDisableTwisting, m_bDisableBlow, m_bDisableDecay, m_bDisableCombo;

	BOOL m_bMonsterKill_War, m_bMonsterKill_Duel;

	BOOL m_bEnableChecksum;

	BOOL m_bLimitTrade;

	BOOL m_bGlobalNick;

	BOOL m_bCheckClientSerial;

	BOOL m_bLimitLife;

	bool m_bBlessBug;

	bool m_bBackSpring;

	int m_iSoulRate, m_iSoulRateWithLuck;

	int m_iLifeRate;

	int m_iInnerTime, m_iInnerVitality, m_iInnerEnergy, m_iInnerMaxHP;

	int m_iShieldTime, m_iShieldDexterity, m_iShieldEnergy, m_iShieldMaxDefense;

	int m_iCombination10Rate, m_iCombination11Rate, m_iCombination12Rate, m_iCombination13Rate, m_iMaxCombinationRate;

	int m_iWingCombinationRate, m_iThirdWingCombinationRate;

	int m_iMax_Stats;

	int m_iApple_Potion, m_iSmallHP_Potion, m_iMediumHP_Potion, m_iLargeHP_Potion;
	int m_iSmallMP_Potion, m_iMediumMP_Potion, m_iLargeMP_Potion;

	int m_iMaxPotionDurability;

	int m_iNormal_Points, m_iSpecial_Points, m_iPowerUP_Points;

	int m_iMaxLevel;

	int m_iLimitOption;

	int m_iLimitLife;

	int m_iExcellentDrop, m_iAncientDrop;

	int m_iQCommandMoney, m_iWCommandMoney;

	int m_iMaxMissRate;

	int m_iGuildWarScore;

	int m_iUDPPort;

	float m_fPlusDef, m_fPlusDmg, m_fPlusHeal;

	char m_szConnectMessage[512];
	char m_szGuardMessage[256];

	int m_iDamageAuraTime, m_iDefenseAuraTime;

	int iComboDamage;

	int CloseCountTime;

	int _Time;

	char OFFTRADESUMA[200];//CRISTIAN
	char OFFTRADERESTA[200];//CRISTIAN

private:
	std::map<DWORD,DWORD> m_mAuthorityCode;
};

extern CGlobalConfig g_GlobalConfig;

#endif