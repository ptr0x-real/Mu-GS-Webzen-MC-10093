#if !defined(__TMONSTERAIRULEINFO_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIRULEINFO_DEF__

#include "TSkillDefine.h"

#define		MAX_MONSTER_AI_RULEINFO				200
#define		MAX_MONSTER_CLASS					500

#define		MAX_MONSTERAI_RULEDESC				100

enum MONSTER_AI_RULEINFO_CONDITION
{
	MAR_CONDITION_SPECIFIC_DATE = 1,
	MAR_CONDITION_CRYWOLF_START	= 71,
	MAR_CONDITION_CRYWOLF_END	= 72,
};

class TMonsterAIRuleInfo
{
	
public:
	TMonsterAIRuleInfo();

public:
	void	Reset();
	BOOL	IsValid();

public:
	BOOL	CheckConditionSpecificDate();
	BOOL	CheckConditionCrywolfStart();
	BOOL	CheckConditionCrywolfEnd();

public:
//	static 

public:
	int		m_iRuleNumber;
	int		m_iMonsterClass;
	int		m_iMonsterAIUnit;

	int		m_iRuleCondition;

	int		m_iWaitTime;		
	int		m_iContinuanceTime;

	int		m_iMonth;
	int		m_iDay;
	int		m_iWeekDay;
	int		m_iHour;
	int		m_iMinute;

	int		m_iAppliedTime;
	int		m_iApplyingTime;

	char	m_szRuleDesc[MAX_MONSTERAI_RULEDESC];
	
};


#endif