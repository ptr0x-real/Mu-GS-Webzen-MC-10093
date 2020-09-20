#ifdef ADD_NEW_UNION_01_20041006

#ifndef __TUNION_DEF__
#define __TUNION_DEF__

#include "TUnionInfo.h"

class TUnion
{
public:
	TUnion();
	virtual ~TUnion();

// O P E R A T I O N
public:
	// 길드 관계를 얻는다. : 뷰포트 생성 부분에서 검사하기 위해서 사용
	INT	GetGuildRelationShipCount( INT iGuildNumber, INT iRelationShipType = G_RELATIONSHIP_UNION );
	INT GetGuildRelationShip( INT iGuildNumber, INT iTargetGuildNumber );

	INT	SetGuildUnionMemberList( INT iMasterGuildNumber, CONST INT iCount, CONST INT* iList );
	INT	SetGuildRivalMemberList( INT iMasterGuildNumber, CONST INT iCount, CONST INT* iList );
	
#ifdef ADD_UNION_CHATTING_01_20041201
	INT	GetGuildUnionMemberList( INT iMasterGuildNumber, INT& iCount, INT* iList );
#endif
	
#ifdef ADD_UNION_GET_UNIONNAME_01_20041222
	INT	GetUnionName( INT iMasterGuildNumber, CHAR* szMasterGuildName );
#endif


#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
	INT	AddUnion( INT iMasterGuildNumber, CHAR* szUnionMasterGuildName, BYTE* pUnionMasterGuildMark );
#else
	INT	AddUnion( INT iMasterGuildNumber, CHAR* szUnionMasterGuildName );
#endif

	INT DelUnion( INT iMasterGuildNumber );
	INT	DelAllUnion();

public:
	TUnionInfo* SearchUnion( INT iMasterGuildNumber );

public:
	M_UNION_INFO		m_mpUnion;

private:

	TSync				m_Sync;
	
};

extern TUnion	UnionManager;


#endif

#endif