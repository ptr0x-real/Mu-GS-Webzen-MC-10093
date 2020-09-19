
#include "Stdafx.h"

#ifdef ADD_NEW_UNION_01_20041006

#include "TUnionInfo.h"

TUnionInfo::TUnionInfo()
{
	m_iUnionTimeStamp = 0;
	m_iRivalTimeStamp = 0;

	m_iMasterGuild	  = 0;
	
	ZeroMemory( m_szMasterGuild, MAX_GUILDNAMESTRING+1 );

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222	
	ZeroMemory( m_pMasterGuildMark, MAX_GUILDMARKSIZE );
#endif

}

TUnionInfo::~TUnionInfo()
{
	Clear();
}

INT	TUnionInfo::GetUnionMemberCount()
{
	INT iCount = 0;
	m_Sync.Lock();
	
	iCount = m_vtUnionMember.size();

	m_Sync.Unlock();

	return iCount;
}

INT	TUnionInfo::GetRivalMemberCount()
{
	INT iCount = 0;
	
	m_Sync.Lock();
	
	iCount = m_vtRivalMember.size();

	m_Sync.Unlock();

	return iCount;
}

BOOL TUnionInfo::IsUnionMember( INT iGuildNumber )
{
	m_Sync.Lock();

	VI_INT _Itor = find( m_vtUnionMember.begin(), m_vtUnionMember.end(), iGuildNumber );

	if( _Itor != m_vtUnionMember.end() )
	{
		m_Sync.Unlock();
		return TRUE;
	}

	m_Sync.Unlock();
	return FALSE;
}

BOOL TUnionInfo::IsRivalMember( INT iGuildNumber )
{
	m_Sync.Lock();

	VI_INT _Itor = find( m_vtRivalMember.begin(), m_vtRivalMember.end(), iGuildNumber );

	if( _Itor != m_vtRivalMember.end() )
	{
		m_Sync.Unlock();
		return TRUE;
	}

	m_Sync.Unlock();
	return FALSE;
}


#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
INT	TUnionInfo::SetUnionMaster( INT iGuildNumber, CHAR* szGuildName, BYTE* pGuildMark )
#else
INT	TUnionInfo::SetUnionMaster( INT iGuildNumber, CHAR* szGuildName )
#endif
{
	m_Sync.Lock();

	m_iMasterGuild = iGuildNumber;
	CopyMemory( m_szMasterGuild, szGuildName, MAX_GUILDNAMESTRING );

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
	CopyMemory( m_pMasterGuildMark, pGuildMark, MAX_GUILDMARKSIZE );
#endif

	m_Sync.Unlock();
	
	return 1;
}

VOID TUnionInfo::Clear( INT iRelationShipType )
{
	m_Sync.Lock();

	if( iRelationShipType == G_RELATIONSHIP_NONE )
	{
		m_vtRivalMember.clear();
		m_vtUnionMember.clear();
	}
	else
	if( iRelationShipType == G_RELATIONSHIP_UNION )
	{
		m_vtUnionMember.clear();
	}
	else
	if( iRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		m_vtRivalMember.clear();
	}

	m_Sync.Unlock();
}

#endif