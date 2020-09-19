#if !defined(SETTINGS_H)
#define SETTINGS_H

#if (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

class _CRebirth
{
public:
	int iRebirthLevel_1;
	int iRebirthLevel_2;
	int iRebirthLevel_3;
	int iRebirthLevel_4;
	int iRebirthLevel_5;
	//int iRebirthLevel_6;
	int isZen;
	int iStart;
	int iStartForVip;
	int SMBKELF_1;
	int MGDL_1;
	int SMBKELF_2;
	int MGDL_2;
};

class _CDBCon
{
public:
	std::string g_Host;
	std::string g_User;
	std::string g_Password;
	std::string g_Database;
};

/*class _CVipSystem
{
public:
	int iVipDays;
	int iVipCost;
	int iVipExperience;
};*/

/*class _CJewels
{
public:
	int m_iJewelOfLifeSuccess;
	int m_iJewelOfMysticSuccess;
	int m_iJewelOfDragonSuccess;
	int m_iJewelOfLuckSuccess;
	int m_iJewelOfRainbowSuccess;
	int m_iJewelOfEternalSuccess;
	int m_iJewelOfSkillSuccess;
	int m_iJewelOfExcellentSuccess;
	int m_iJewelOfExcellentMax;
	int m_iJewelOfExcellentCanInsertToAncient;
	// ----
	int m_iJOD_ChanceToAdd2Options;
};*/

class _CDynamicExperience
{
public:
	int m_isWork;
};

/*class _CNPCTeleport
{
public:
	int _npc_id;
	int _npc_map;
	int _npc_x;
	int _npc_y;
};*/

/*class _CAncientDrop
{
public:
	int _isEnable;
	int _drop_rate;
	int _drop_level;
};*/

class CSettings
{
public:
	bool Loading();
	// ----
	_CRebirth _Rebirth;
	// ----
	_CDBCon _DBCon;
	// ----
//	_CVipSystem _VipSystem;
	// ----
//	_CJewels _Jewels;
	// ----
	_CDynamicExperience _dymanic_exp;
	// ----
//	_CNPCTeleport _npc_teleport;
	// ----
//	_CAncientDrop _ancient_drop;
	// ----
//	float _satan_dur_down;
//	float _angel_dur_down;
//	float _dino_dur_down;
};

extern CSettings * gSettings;

#endif /* SETTINGS_H */