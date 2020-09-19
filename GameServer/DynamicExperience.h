#if !defined(DYNAMIC_EXPERIENCE_H)
#define DYNAMIC_EXPERIENCE_H

#if (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

//#include "User.h"

class CDynamicExperienceData
{
public:
	int m_iReset;
	float m_fDivider;
};

class CDynamicExperience
{
public:
	void _loading();
	// ----
	float _checkCondition(LPOBJECTSTRUCT lpObj);
	// ----
	DWORD _getNewExperience(DWORD m_dwExperience, float m_fDivider);
private:
	std::vector<CDynamicExperienceData> _dynamic_exp_data;
};

extern CDynamicExperience _dynamic_experience;

#endif /* DYNAMIC_EXPERIENCE_H */