
#include "stdafx.h"

#ifdef ADD_NEW_UNION_01_20041006

#include "TUnion.h"

TUnion UnionManager;

TUnion::TUnion()
{

}

TUnion::~TUnion()
{

}

INT	TUnion::DelAllUnion()
{
	m_Sync.Lock();

	MI_UNION_INFO _PosItor;

	for( _PosItor=m_mpUnion.begin(); _PosItor!=m_mpUnion.end(); ++_PosItor )
	{
		delete (TUnionInfo*)_PosItor->second;
	}

	m_mpUnion.clear();

	m_Sync.Unlock();
	
	return -1;
}

INT TUnion::GetGuildRelationShip( INT iGuildNumber, INT iTargetGuildNumber )
{
	// ���Ͽ� ã��
	TUnionInfo* pUnionInfo = SearchUnion( iGuildNumber );

	if( pUnionInfo == NULL ) 
		return G_RELATIONSHIP_NONE;

	// ���� ����Ʈ �˻�
	if( pUnionInfo->IsUnionMember(iTargetGuildNumber) )
			return G_RELATIONSHIP_UNION;

	// ���̹� ����Ʈ �˻�
	if( pUnionInfo->IsRivalMember(iTargetGuildNumber) )
		return G_RELATIONSHIP_RIVAL;

	return G_RELATIONSHIP_NONE;
}

INT	TUnion::GetGuildRelationShipCount( INT iGuildNumber, INT iRelationShipType )
{
	TUnionInfo* pUnionInfo = SearchUnion( iGuildNumber );
	
	if( pUnionInfo == NULL ) 
		return 0;

	if( iRelationShipType == G_RELATIONSHIP_UNION )
	{
		return pUnionInfo->GetUnionMemberCount();
	}
	else if( iRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		return pUnionInfo->GetRivalMemberCount();
	}

	return 0;
}

TUnionInfo* TUnion::SearchUnion( INT iMasterGuildNumber )
{
	m_Sync.Lock();

	MI_UNION_INFO _Itor = m_mpUnion.find( iMasterGuildNumber );

	if( _Itor != m_mpUnion.end() )
	{	
		m_Sync.Unlock();
		return (*_Itor).second;
	}

	m_Sync.Unlock();
	return NULL;
}

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
INT	TUnion::AddUnion( INT iMasterGuildNumber, CHAR* szUnionMasterGuildName, BYTE* pUnionMasterGuildMark )
#else
INT	TUnion::AddUnion( INT iMasterGuildNumber, CHAR* szUnionMasterGuildName )
#endif
{
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );

	if( pUnionInfo == NULL )
	{
		m_Sync.Lock();

		pUnionInfo = new TUnionInfo();

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
		pUnionInfo->SetUnionMaster( iMasterGuildNumber, szUnionMasterGuildName, pUnionMasterGuildMark );
#else
		pUnionInfo->SetUnionMaster( iMasterGuildNumber, szUnionMasterGuildName );
#endif
		
		m_mpUnion[iMasterGuildNumber] = pUnionInfo;

		m_Sync.Unlock();

		return 1;
	}
	else
	{
		// Official Union �� ���� �ƴ� ��� ���Ͽ� �̸� ĳ�� ������ �Ʒ� ���� ó����.
		// Official Union �� �ƴ� ��� szUnionMasterGuildName�� '\0' �� �ȴ�.
		m_Sync.Lock();

		pUnionInfo->SetUnionMaster( iMasterGuildNumber, szUnionMasterGuildName );

		m_Sync.Unlock();
	}
	

	return 0;
}

INT TUnion::DelUnion( INT iMasterGuildNumber )
{
	// ��� ��ü �� ��� ���踦 ������ ��� �Ѵ�..
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );

	if( pUnionInfo!=NULL )
	{
		m_Sync.Lock();

		m_mpUnion.erase( iMasterGuildNumber );
		delete pUnionInfo;	

		m_Sync.Unlock();

		return 1;
	}

	return -1;
}

INT	TUnion::SetGuildUnionMemberList( INT iMasterGuildNumber, CONST INT iCount, CONST INT* iList )
{
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );

	if( pUnionInfo != NULL )
	{
		pUnionInfo->Clear( G_RELATIONSHIP_UNION );

		m_Sync.Lock();

		for( INT i=0; i<iCount; i++ )
			pUnionInfo->m_vtUnionMember.push_back( iList[i] );

		m_Sync.Unlock();

		return 1;
	}

	return -1;
}

INT	TUnion::SetGuildRivalMemberList( INT iMasterGuildNumber, CONST INT iCount, CONST INT* iList )
{
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );

	if( pUnionInfo != NULL )
	{
		pUnionInfo->Clear( G_RELATIONSHIP_RIVAL );
		
		m_Sync.Lock();

		for( INT i=0; i<iCount; i++ )
			pUnionInfo->m_vtRivalMember.push_back( iList[i] );

		m_Sync.Unlock();

		return 1;
	}

	return -1;
}

#ifdef ADD_UNION_CHATTING_01_20041201
INT	TUnion::GetGuildUnionMemberList( INT iMasterGuildNumber, INT& iCount, INT* iList )
{
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );

	if( pUnionInfo != NULL )
	{
		m_Sync.Lock();

		VI_INT _BItor = pUnionInfo->m_vtUnionMember.begin();
		VI_INT _EItor = pUnionInfo->m_vtUnionMember.end();

		for( ; _BItor!=_EItor; ++_BItor )
		{
			iList[iCount] = (INT)*_BItor;
			iCount++;
		}

		m_Sync.Unlock();

		if( iCount > 0 && iCount < MAX_RELATIONSHIP_COUNT )
			return 1;
	}

	return -1;
}
#endif

#ifdef ADD_UNION_GET_UNIONNAME_01_20041222
INT TUnion::GetUnionName( INT iMasterGuildNumber, CHAR* szMasterGuildName )
{
	TUnionInfo* pUnionInfo = SearchUnion( iMasterGuildNumber );
	
	if( pUnionInfo != NULL )
	{
		m_Sync.Lock();

		CopyMemory( szMasterGuildName, pUnionInfo->m_szMasterGuild, MAX_GUILDNAMESTRING );

		m_Sync.Unlock();

		return 1;
	}

	return -1;
}
#endif


#endif
