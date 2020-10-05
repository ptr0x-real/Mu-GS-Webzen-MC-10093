#if !defined(__TMONSTERSKILLMANAGER_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TMONSTERSKILLMANAGER_DEF__

#include "TSkillDefine.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillInfo.h"

#include "VECTOR"
#include "LIST"
#include "ALGORITHM"

#ifdef ADD_RAKLION_20080408
#include "MagicInf.h"
#endif // ADD_RAKLION_20080408

#include "Common\WinUtil.h"

using namespace std;

typedef struct _ST_MONSTER_SKILL_DELAYTIME_INFO
{
	BOOL				bIsUsed;
	INT					iIndex;
	INT					iTargetIndex;
	DWORD				dwDelayTime;

	TMonsterSkillUnit	*lpMonsterSkillUnit;

	void RESET()
	{
		iIndex				= -1;
		iTargetIndex		= -1;
		dwDelayTime			= 0;
		bIsUsed				= FALSE;

		lpMonsterSkillUnit	= NULL;
	};

	BOOL IsValid()
	{
		if( !CHECK_LIMIT(iIndex,MAX_OBJECT) || !CHECK_LIMIT(iTargetIndex,MAX_OBJECT) )
			return FALSE;

		if( lpMonsterSkillUnit == NULL )
			return FALSE;

		return TRUE;
	}

	_ST_MONSTER_SKILL_DELAYTIME_INFO()
	{
		RESET();
	};

}ST_MONSTER_SKILL_DELAYTIME_INFO, *LPST_MONSTER_SKILL_DELAYTIME_INFO;


enum MONSTER_SKILL_UNIT_SLOT_TYPE
{
	MSUS_TYPE_ATTACK0			=	0,		// 공격 0
		
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724		
	MSUS_TYPE_ATTACK_RESTRICTION	=	1,		// 공격 1 - 외부의 제약조건에 따른 공격
	MSUS_TYPE_ATTACK_PENETRATION	=	2,		// 공격 2 - 관통
#endif

	MSUS_TYPE_DEFENSE0			=	10,		// 방어	0

	MSUS_TYPE_HEAL0				=	20,		// 치료	0
	MSUS_TYPE_HEAL_GROUP		=	21,		// 치료 1 - 그룹

	MSUS_TYPE_SPECIAL_SOMMON	=	30,		// 특수 0 - 소환
	
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	MSUS_TYPE_SPECIAL_PUSH			=	31,		// 특수 1 - 밀기	
#endif

};

class TMonsterSkillManager
{

public:
	TMonsterSkillManager();
	~TMonsterSkillManager();

public:
	static	int									LoadData( LPSTR lpszFileName );
	static	void								DelAllSkillManagerInfo();
	
public:
	static	TSync								s_Sync;
	static	BOOL								s_bDataLoad;
	static	TMonsterSkillInfo					s_MonsterSkillInfoArray[MAX_MONSTER_SKILLINFO];
	static	ST_MONSTER_SKILL_DELAYTIME_INFO		s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO];
	
public:
	static	BOOL								CheckMonsterSkill	( int iMonsterClass );
#ifdef ADD_RAKLION_20080408
	static	void								UseMonsterSkill		( int iIndex, int iTargetIndex, int iMonsterSkillUnitType, int iUnitNumber=-1, CMagicInf *lpMagic=NULL);
#else
	static	void								UseMonsterSkill		( int iIndex, int iTargetIndex, int iMonsterSkillUnitType);
#endif // ADD_RAKLION_20080408
	
	static	TMonsterSkillUnit*					FindMonsterSkillUnit( int iIndex, int iMonsterSkillUnitType);

	static	void								MonsterSkillProc	();
	static  BOOL								AddMonsterSkillDelayInfo( INT iIndex, INT iTargetIndex, INT iDelayTime, TMonsterSkillUnit *lpMonsterSkillUnit );
			
public:
	
private:
	
	

};

#endif // __TMONSTERSKILLMANAGER_DEF__