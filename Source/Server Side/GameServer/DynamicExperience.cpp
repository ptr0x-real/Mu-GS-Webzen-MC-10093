#include "StdAfx.h"
#include "Include\readscript.h"
#include "LogProc.h"

CDynamicExperience _dynamic_experience;

void CDynamicExperience::_loading()
{
	if (gSettings->_dymanic_exp.m_isWork == 0) 
{
		return;
	}
	// ----

	if((SMDFile = fopen("..\\data\\DynamicExperience.cfg", "r")) == NULL) // ".\\FT\\Settings.txt" "./Data/ExpSystem/DynamicExperience.cfg"
	{
		MessageBox(0, "Error on load DynamicExperience.cfg", "Error", MB_ICONSTOP); ExitProcess(0); return;
	}
	// ----
	for (;;) {
		GetToken();
		// ----

		if (strstr(TokenString, "end")) 
		{
			break;
		}
		// ----
		CDynamicExperienceData _data;
		// ----
		_data.m_iReset = TokenNumber; GetToken();
		_data.m_fDivider = atof(TokenString);
		// ----
		_dynamic_exp_data.push_back(_data);
	}
	// ----
	fclose(SMDFile);
}

float CDynamicExperience::_checkCondition(LPOBJECTSTRUCT lpObj)
{
	if (gSettings->_dymanic_exp.m_isWork == 0) 
	{
		return 0;
	}
	// ----
	float _fDivider = 0;
	int _lastReset = 0;

	std::vector<CDynamicExperienceData>::iterator i = _dynamic_exp_data.begin();

	for (; i != _dynamic_exp_data.end(); i ++) 
	{
		int Resets = lpObj->m_iResetNumber; //Manager.CountResets(lpObj->Name);//lpObj->m_iResetNumber

		if (i->m_iReset > _lastReset) 
		{
			if (Resets < i->m_iReset) 
			{
				return _fDivider;
			}
			_lastReset = i->m_iReset;
			_fDivider = i->m_fDivider;
		}
	}
	// ----
	return _fDivider;
}

DWORD CDynamicExperience::_getNewExperience(DWORD m_dwExperience, float m_fDivider)
{
	double _new_exp = (m_dwExperience / 100) * (100.0f - m_fDivider); 
	
	return (DWORD)_new_exp;//nuevo
}