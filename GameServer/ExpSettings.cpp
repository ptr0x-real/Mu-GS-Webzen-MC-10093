#include "StdAfx.h"
#include "LogProc.h"

CSettings * gSettings;

bool CSettings::Loading()
{
	CNSIniFile gIniFile("..\\data\\DynamicSettings.ini", "DynamicExp"); // "./Data/ExpSystem/SettingsDynamic.ini"  ".\\FT\\Settings.txt"
	LogAdd("[DynamicSettings.ini] - [DynamicExperience.cfg] Load() completed");
	// ----
	gIniFile.changeSection("DynamicExperience");
	// ----
	_dymanic_exp.m_isWork = gIniFile.getInt("Enabled");
	// ----
		return true;
}
