#if !defined(__TSKILLDEFINE_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_01_20050714)
#define __TSKILLDEFINE_DEF__

#define _SUCCESS						TRUE
#define _FAIL							FALSE

#define _ALL							0
#define _ZERO							0
#define _NONE							-1


#define _INVALID						-1
#define _RANDOM							-1
#define _NOTUSING						-1
#define _REPEAT							-1

#define _PERCENT						0
#define _CONSTANT						1

#define AVOID_ZERO(x)					( x<=0?1:x )

#define MAX_MONSTER_SKILL_SUCCESS_RATE	100

#define MAX_SKILLELEMENTNAME			20
#define MAX_SKILLUNITNAME				20

#define MAX_MONSTER_SKILL_UNIT			200		
#define MAX_MONSTER_SKILLELEMENT_SLOT	5

#ifdef UPDATE_MONSTER_SKILLUNIT_SLOT_EXTEND_20080514
#define MAX_MONSTER_SKILLUNIT_SLOT		10
#else
#define MAX_MONSTER_SKILLUNIT_SLOT		5
#endif // UPDATE_MONSTER_SKILLUNIT_SLOT_EXTEND_20080514

#define MAX_MONSTER_SKILLINFO			500		// 몬스터 종류 만큼 필요하다. 즉 MONSTER INDEX의 최대값!

#define MAX_MONSTER_SKILL_DELAY_INFO	3000




#endif // __TSKILLDEFINE_DEF__