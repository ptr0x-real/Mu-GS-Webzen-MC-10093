#if !defined(__TMONSTERGROUP_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERGROUP_DEF__

#define MAX_MONSTER_AI_GROUP		50

#include "TMonsterAIGroupMember.h"

enum MONSTER_AI_GROUP_AI_ORDER
{
	MAG_AI_ORDER_DEFAULT	= 0,
	MAG_AI_ORDER_FIRST		= 1,
	MAG_AI_ORDER_SECOND		= 2,
	MAG_AI_ORDER_THIRD		= 3,
};


class TMonsterAIGroup
{

public:
	TMonsterAIGroup();
	virtual ~TMonsterAIGroup();

public:
	static  void						Init					( int iGroupNumber );
	static  void						InitAll					();
	static	int							LoadData				( LPSTR lpszFileName );
	static	int							DelAllGroupInfo			();
	static	int							DelGroupInstance		( int iGroupNumber );
	
	// Group 의 AI 를 변경 한다.
	static	void						ChangeAIOrder			( int iGroupNumber, int iAIOrder );
	
	static	TMonsterAIGroupMember*		FindGroupMember			( int iGroupNumber, int iGuid );
	static	TMonsterAIGroupMember*		FindGroupMemberToSommon	( int iObjIndex, int iGroupNumber, int iGuid );
	static  TMonsterAIGroupMember*		FindGroupMemberToHeal	( int iObjIndex, int iGroupNumber, int iGuid, int iDistance );

	static	int							FindGroupLeader			( int iGroupNumber );

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	static int							FindGroupMemberObjectIndex	( int iGroupNumber, int iGuid );
#endif

	
public:
	static	BOOL						s_bDataLoad;
	static	TMonsterAIGroupMember		s_MonsterAIGroupMemberArray[MAX_MONSTER_AI_GROUP][MAX_MONSTER_AI_GROUP_MEMBER];
	static	int							s_iMonsterAIGroupMemberCount[MAX_MONSTER_AI_GROUP];



};

#endif