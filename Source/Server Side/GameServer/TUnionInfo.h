#ifdef ADD_NEW_UNION_01_20041006

#ifndef __TUNIONINFO_DEF__
#define __TUNIONINFO_DEF__

#include "GuildClass.h"
#include "..\\Common\\TSync.h"

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class TUnionInfo;

typedef map< INT, LPGUILD_INFO  >				M_LPGUILD_INFO;
typedef map< INT, LPGUILD_INFO  >::iterator		MI_LPGUILD_INFO;

typedef vector< LPGUILD_INFO*   >				V_LPGUILD_INFO;
typedef vector< LPGUILD_INFO*   >::iterator		VI_LPGUILD_INFO;

typedef map< INT, TUnionInfo*	>				M_UNION_INFO;
typedef map< INT, TUnionInfo*	>::iterator		MI_UNION_INFO;

typedef vector<INT>	V_INT;
typedef vector<INT>::iterator VI_INT;

class TUnionInfo
{

public:
	TUnionInfo();
	virtual ~TUnionInfo();

public:
	BOOL	IsUnionMember( INT iGuildNumber );
	BOOL	IsRivalMember( INT iGuildNumber );

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
	INT		SetUnionMaster( INT iGuildNumber, CHAR* szGuildName, BYTE* pGuildMark );
#else
	INT		SetUnionMaster( INT iGuildNumber, CHAR* szGuildName );
#endif
	INT		GetUnionMemberCount();
	INT		GetRivalMemberCount();


public:
	void	Clear( INT iRelationShipType=G_RELATIONSHIP_NONE );

// T I M E  S T A M P
private:
	INT		m_iUnionTimeStamp;
	INT		m_iRivalTimeStamp;

public:
	INT		m_iMasterGuild;
	CHAR	m_szMasterGuild[MAX_GUILDNAMESTRING+1];
	
#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
	BYTE	m_pMasterGuildMark[MAX_GUILDMARKSIZE];
#endif

public:
	V_INT	m_vtUnionMember;
	V_INT	m_vtRivalMember;
	
private:
	TSync	m_Sync;


};

#endif

#endif
