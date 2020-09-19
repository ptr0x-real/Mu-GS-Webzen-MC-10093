// GuildClass.cpp: implementation of the CGuildClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <winbase.h>
#include "GuildClass.h"

#include "Gamemain.h"
#include "..\Common\winutil.h"

BOOL	gGuildNoticeStringCheck(char* notice);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildClass::CGuildClass()
{
	head = NULL;
	tail = NULL;
}

CGuildClass::~CGuildClass()
{
	AllDelete();
}

void CGuildClass::Init()
{
#ifdef MODIFY_GUILD_CLASS_20040608
	m_GuildMap.clear();
#endif
	
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	m_GuildNumberMap.clear();
#endif
	
}

LPGUILD_INFO CGuildClass::AddGuild(int number, char * guildname, LPBYTE mark, char * mastername, int score )
{
	LPGUILD_INFO	pNewNode;
	LPGUILD_INFO    pSearchGuild;

	pSearchGuild = SearchGuild(guildname);
	if( pSearchGuild != NULL )
	{
#ifdef GUILD_REWORK
		return pSearchGuild;
#else
		return NULL;
#endif
	}
	pNewNode = (LPGUILD_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(GUILD_INFO));
	if( pNewNode )
	{
		memset(pNewNode->Name, 0, MAX_GUILDNAMESTRING+1);
		strcpy(pNewNode->Name, guildname);
		strcpy(pNewNode->Names[0], mastername);
		if( mark != NULL )
		{
			memcpy(pNewNode->Mark, mark, MAX_GUILDMARKBUFFER);
		}
		pNewNode->Number		= number;
		pNewNode->Count			= 0;
		pNewNode->TotalCount	= 0;
		pNewNode->PlayScore		= 0;
		pNewNode->TotalScore	= score;
		pNewNode->next			= NULL;
		pNewNode->WarDeclareState = 0;
		pNewNode->WarState		  = 0;

		memset(pNewNode->Notice, 0, MAX_GUILDNOTICE);

#ifdef ADD_NEW_UNION_01_20041006
		pNewNode->iGuildUnion	= G_RELATIONSHIP_NONE;							
		pNewNode->iGuildRival	= G_RELATIONSHIP_NONE;							
#endif
		for( int n=0; n<MAX_GUILD; n++)
		{
			pNewNode->Use[n]	= 0;
			pNewNode->Index[n]	= -1;
			pNewNode->pServer[n]= -1;

#ifdef ADD_NEW_GUILD_01_20040913
			// 0x36���� ��������� �ٽ� ���� ������ �渶�� ���� ó���� ���� ���ʿ� ����.
			// ��� G_NONE
			pNewNode->GuildStatus[n] = G_NONE;
#endif
		}
		
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
		pNewNode->lpLifeStone = NULL;	
#endif
		
		BuildMemberTotal(pNewNode);
		AddTail(pNewNode);
		return pNewNode;
	}
	else 
	{
	}
	return NULL;
}

void CGuildClass::AddTail(LPGUILD_INFO pNewNode)
{
	pNewNode->back	= tail;
	pNewNode->next  = NULL;

	if(head == NULL)
		head = pNewNode;
	else
		tail->next= pNewNode;
	
	tail = pNewNode;

#ifdef MODIFY_GUILD_CLASS_20040608
	m_GuildMap[pNewNode->Name] = pNewNode;
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	m_GuildNumberMap[pNewNode->Number] = pNewNode;
#endif
	
	//printf("%s %p \n", pNewNode->Name, tail);
}

void CGuildClass::Print()
{
	LPGUILD_INFO	pNode;

	pNode = head;
	while(pNode)
	{
		//printf("%s  %s\n",pNode->Name, pNode->Names[0]);
		pNode = pNode->next;
	}
}

void CGuildClass::PrintGuild(char * guildname)
{
	LPGUILD_INFO	pNode;
	pNode = SearchGuild(guildname);
	if( pNode == NULL ) return;

	//printf("search : %s\n",pNode->Name);
}

void CGuildClass::AllDelete()
{
	LPGUILD_INFO	pNode, pTemp;

	pNode = head;
	while(pNode)
	{
		pTemp = pNode;
		//printf("delete %p\n", pTemp);
		pNode = pNode->next;
		HeapFree(GetProcessHeap(), 0, pTemp);
	}
	
#ifdef ADD_NEW_UNION_01_20041006
	head = NULL;
	tail = NULL;
#endif

#ifdef MODIFY_GUILD_CLASS_20040608
	m_GuildMap.clear();
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	m_GuildNumberMap.clear();
#endif
	
}

// ��带 �����Ѵ�.
int CGuildClass::DeleteGuild(char * guildname, char *commader)
{
	LPGUILD_INFO	pNode;
	LPGUILD_INFO	pprev, pnext;
	
	pNode = SearchGuild(guildname);


	if( pNode == NULL )
	{
		return FALSE;
	}	

#ifdef MODIFY_GUILD_CLASS_20040608
	MapGuildNameItor Itor = m_GuildMap.find(guildname);	
	if( Itor != m_GuildMap.end() )
	{			
		m_GuildMap.erase(Itor);
	}
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	MapGuildNumberItor _Itor = m_GuildNumberMap.find(pNode->Number);
	if( _Itor != m_GuildNumberMap.end() )
	{
		m_GuildNumberMap.erase(_Itor);
	}
#endif
	
	pprev = pNode->back;
	pnext = pNode->next;
	if( pprev == NULL && pnext == NULL )
	{
	}
	else if( pprev == NULL && pnext != NULL)
	{
		pnext->back = NULL;
		head = pnext;
	}
	else if( pprev != NULL && pnext == NULL)
	{
		pprev->next = NULL;
		tail = pprev;
	}
	else if( pprev != NULL && pnext != NULL)
	{
		pprev->next = pnext;
		pnext->back = pprev;
	}

	//printf("%s %p \n", pNode->Name, pNode);
	HeapFree(GetProcessHeap(), 0, pNode);
	if( pprev == NULL && pnext == NULL )
	{
		head = NULL;
		tail = NULL;
	}
	return TRUE;
}

// ��� �̸����� �˻��ؼ� �����Ѵ�.
LPGUILD_INFO CGuildClass::SearchGuild(char *guildname)
{
#ifdef MODIFY_GUILD_CLASS_20040608
	MapGuildNameItor Itor = m_GuildMap.find(guildname);	
	if( Itor == m_GuildMap.end() )
	{	
		return NULL;
	}
	return (*Itor).second;	
#else
	LPGUILD_INFO	pNode;

	pNode = head;
	while(pNode)	
	{
		if( pNode->Name[0] == guildname[0] )
		{
			if( pNode->Name[1] == guildname[1] )
			{
				if( strcmp(pNode->Name, guildname) == 0 )
				{
					return pNode;
				}
			}
		}
		pNode = pNode->next;
	}
	return NULL;
#endif
}


// ����ȣ�� �˻��ؼ� �����Ѵ�.
LPGUILD_INFO CGuildClass::SearchGuild_Number(int number)
{
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	MapGuildNumberItor Itor = m_GuildNumberMap.find(number);	
	if( Itor == m_GuildNumberMap.end() )
	{	
		return NULL;
	}
	return (*Itor).second;
#else
	LPGUILD_INFO	pNode;

	pNode = head;
	while(pNode)	
	{
		if( pNode->Number == number )
		{
			return pNode;
		}
		pNode = pNode->next;
	}
	return NULL;
#endif
	
}


LPGUILD_INFO CGuildClass::SearchGuild_NumberAndId(int number, char *name)
{
	LPGUILD_INFO	pNode = SearchGuild_Number(number);
	if( pNode != NULL )
	{
		for( int n=0; n<MAX_GUILD; n++)
		{
			if( strcmp(pNode->Names[n], name) == 0 )
			{
				return pNode;
			}
		}
	}
	return NULL;
}

BOOL CGuildClass::ConnectUser(LPGUILD_INFO	lpNode, char *guild_name, char *player_name, int aIndex, int pServer)
{
	if( lpNode == NULL ) return FALSE;

	if( strcmp(guild_name, lpNode->Name) != 0 ) return FALSE;

	for( int n=0; n<MAX_GUILD; n++)
	{
		if( lpNode->Use[n] == 1 )
		{
			if( strcmp(lpNode->Names[n], player_name) == 0 )
			{
				lpNode->Index[n] = aIndex;
				lpNode->pServer[n] = pServer;					
				// �渶�� ������ ������ ����� �ٽ� �����Ѵ�.
				if( n == 0 )
				{
					lpNode->PlayScore		= 0;
					lpNode->WarDeclareState = 0;
					lpNode->WarState		= 0;
				}
				return TRUE;
			}
		}
	}
	AddMember(lpNode, player_name, aIndex, pServer);
	return FALSE;
}

#ifdef ADD_NEW_GUILD_01_20040913
LPGUILD_INFO CGuildClass::AddMember(char* guild_name, char *player_name, int aIndex, int totalcount, int iGuildStatus, int pServer)
#else
LPGUILD_INFO CGuildClass::AddMember(char* guild_name, char *player_name, int aIndex, int totalcount, int pServer)
#endif
{
	LPGUILD_INFO	pNode = SearchGuild(guild_name);
	int				blank=-1;

	if( pNode == NULL )
		return NULL;

	BuildMemberTotal(pNode);
	if( strcmp(player_name, pNode->Names[0]) == 0 )
	{
		blank = 0;
	}
	else
	{
		for( int n=1; n<MAX_GUILD; n++)
		{
			if( pNode->Use[n] == 1 )
			{
				if( strcmp(pNode->Names[n], player_name) == 0 )
				{
					if( aIndex != -1 )
					{
						pNode->Use[n]		= 1;
						pNode->Index[n]		= aIndex;
						pNode->pServer[n]	= pServer;

#ifdef ADD_NEW_GUILD_01_20040913
						// �渶�̹Ƿ� �渶 ��å SET
						pNode->GuildStatus[n] = G_MASTER;
#endif	
					}
					BuildMemberTotal(pNode);
					return NULL;
				}
			}
			else {
				if( blank < 0 ) blank = n;
			}
		}
	}

	if( blank < 0 ) return NULL;
	
	strcpy(pNode->Names[blank], player_name);
	pNode->Use[blank] = 1;
	pNode->pServer[blank] = pServer;

#ifdef ADD_NEW_GUILD_01_20040913
	// ���� ���� ���¸� ��Ÿ���ų� ���� ����Ʈ�� ���� ����ϹǷ� ��å �� ���¸� �����ؾ� �Ѵ�.
	pNode->GuildStatus[blank] = iGuildStatus;	// ��� ��å �� ���� 
#endif	
	
	if( aIndex != -1 )
	{
		pNode->Index[blank] = aIndex;
		pNode->Count++;
	}
	if( totalcount > 0 )
	{
		pNode->TotalCount = totalcount;
	}
	//LogAdd("%s��� %s������ %d�ε��� ���� %d", pNode->Name, pNode->Names[0], pNode->Index[blank], pNode->TotalCount);
	BuildMemberTotal(pNode);
	return pNode;	
}

// ������ ConnectUser �κп����� ȣ��ȴ�. (�������� ó��)
LPGUILD_INFO CGuildClass::AddMember(LPGUILD_INFO pNode, char *player_name, int aIndex, int totalcount, int pServer)
{
	int				blank=-1;

	if( pNode == NULL )
		return NULL;
	
	if( strcmp(player_name, pNode->Names[0]) == 0 )
	{
		blank = 0;
	}
	else
	{
		for( int n=1; n<MAX_GUILD; n++)
		{
			if( pNode->Use[n] == 1 )
			{
				if( strcmp(pNode->Names[n], player_name) == 0 )
				{
					if( aIndex != -1 )
					{
						pNode->Use[n]	  = 1;
						pNode->Index[n]   = aIndex;
						pNode->pServer[n] = pServer;

#ifdef ADD_NEW_GUILD_01_20040913
	// ���� ���� ó�� �κ��̹Ƿ� ��åó�� �ʿ� ����
#endif
					}
					BuildMemberTotal(pNode);
					return NULL;
				}
			}
			else {
				if( blank < 0 ) blank = n;
			}
		}
	}

	if( blank < 0 ) return NULL;
	
	strcpy(pNode->Names[blank], player_name);
	pNode->Use[blank] = 1;
	pNode->pServer[blank] = pServer;

#ifdef ADD_NEW_GUILD_01_20040913
	// ���� ���� ó�� �κ��̹Ƿ� ��åó�� �ʿ� ����
#endif
	
	if( aIndex != -1 )
	{
		pNode->Index[blank] = aIndex;
		pNode->Count++;
	}
	if( totalcount > 0 )
	{
		pNode->TotalCount = totalcount;
	}
	//LogAdd("%s��� %s������ %d�ε��� ���� %d", pNode->Name, pNode->Names[0], aIndex, pNode->TotalCount);
	BuildMemberTotal(pNode);
	return pNode;
}

#ifdef ADD_NEW_GUILD_01_20040913

// ��峻 ��å�� ��´�.
INT	CGuildClass::GetGuildMemberStatus(char* szGuildName, char* szMemberName)
{
	INT iGuildStatus = G_PERSON;

	LPGUILD_INFO	pNode = SearchGuild(szGuildName);
	int				iKey  = -1;

	// 0. ��尡 ����.
	if( pNode == NULL )
		return G_NONE;

	// 1. ��� �˻�
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( strcmp(pNode->Names[n], szMemberName) == 0 )
			{
				iKey = n;
			}
		}
	}

	if( iKey < 0 ) return G_NONE;

	iGuildStatus = pNode->GuildStatus[iKey];

	return iGuildStatus;
}

// ��峻 ��å�� �����Ѵ�.
BOOL CGuildClass::SetGuildMemberStatus(char* szGuildName, char* szMemberName, INT iGuildStatus )
{
	LPGUILD_INFO	pNode = SearchGuild(szGuildName);
	int				iKey  = -1;

	// 0. ��尡 ����.
	if( pNode == NULL )
		return G_NONE;

	// 1. ��� �˻�
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( strcmp(pNode->Names[n], szMemberName) == 0 )
			{
				iKey = n;
			}
		}
	}

	if( iKey < 0 ) return FALSE;

	pNode->GuildStatus[iKey] = iGuildStatus;

	// ���� ������ �����ϸ�.. �ǽð� ����
	if( pNode->Index[iKey] > 0 )
	{
		if( gObjIsConnectedGP(pNode->Index[iKey]) == FALSE ) 
		{
			LogAddTD("@SetGuildMemberStatus() error-L2 : Index %s %d", __FILE__, __LINE__);
			return FALSE;
		}

		if(!CHECK_LIMIT(pNode->Index[iKey], MAX_OBJECT)) 
			return FALSE;

		LPOBJECTSTRUCT lpObj = &gObj[pNode->Index[iKey]];
		
		if( strcmp(lpObj->Name, szMemberName) == 0 )
		{
			lpObj->GuildStatus = iGuildStatus;

			// ����Ʈ�󿡼� �ٷ� ����
			GCGuildViewportNowPaint( lpObj->m_Index, lpObj->GuildName, NULL, FALSE);
		}
	}

	return TRUE;
}
#endif //## ADD_NEW_GUILD_01_20040913

#ifdef ADD_NEW_GUILD_02_20040922

INT CGuildClass::GetGuildType(char* szGuildName)
{
	INT iGuildType = G_TYPE_COMMON;
	
	LPGUILD_INFO	pNode = SearchGuild(szGuildName);
	
	// 0. ��尡 ����.
	if( pNode == NULL )
		return G_TYPE_NONE;
		
	iGuildType = pNode->btGuildType;
	
	return iGuildType;
}

BOOL CGuildClass::SetGuildType(char* szGuildName, int iGuildType )
{
	LPGUILD_INFO	pNode = SearchGuild(szGuildName);
	
	// 0. ��尡 ����.
	if( pNode == NULL )
		return FALSE;
	
	pNode->btGuildType = iGuildType;

	// ���� ���� �����鿡�� ��� Type�� ����Ǿ����� �ǽð����� �˸���.
	// ���� ������ �����ϸ�.. �ǽð� ����
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( pNode->Index[n] > 0 )
			{
				if( gObjIsConnectedGP(pNode->Index[n]) == FALSE ) 
				{
					LogAddTD("SetGuildType() error-L2 : Index %s %d", __FILE__, __LINE__);
					continue;
				}
				
				if(!CHECK_LIMIT(pNode->Index[n], MAX_OBJECT)) 
					continue;
				
				LPOBJECTSTRUCT lpObj = &gObj[pNode->Index[n]];				
				GCGuildViewportNowPaint( lpObj->m_Index, lpObj->GuildName, NULL, FALSE);
			}

		}

	}

	return TRUE;
}

#endif //## ADD_NEW_GUILD_02_20040922

BOOL CGuildClass::BuildMemberTotal(LPGUILD_INFO lpNode)
{	
	if( lpNode == NULL ) return FALSE;

	lpNode->TotalCount = 0;
	lpNode->Count = 0;
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( lpNode->Use[n] == 1 )
		{
			lpNode->TotalCount++;
			if( lpNode->Index[n] )
			{
				lpNode->Count++;
			}			
		}
	}
	return TRUE;
}

BOOL CGuildClass::DelMember(char *guild_name, char *player_name)
{
	LPGUILD_INFO	pNode = SearchGuild(guild_name);

	if( pNode == NULL ) return FALSE;
	
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( strcmp(pNode->Names[n], player_name) == 0 )
			{
				pNode->Use[n] = 0;
				pNode->Count--;
				BuildMemberTotal(pNode);
				return TRUE;
			}
		}		
	}
	return FALSE;
}

BOOL CGuildClass::CloseMember(char *guild_name, char *player_name)
{
	LPGUILD_INFO	pNode = SearchGuild(guild_name);

	if( pNode == NULL ) return FALSE;

	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( strcmp(pNode->Names[n], player_name) == 0 )
			{
				pNode->Index[n] = -1;
				pNode->Count--;	
				pNode->pServer[n] = -1;
				BuildMemberTotal(pNode);
				return TRUE;
			}
		}		
	}
	return FALSE;
}


BOOL CGuildClass::SetServer(char *guild_name, char *player_name, int pServer)
{
	LPGUILD_INFO	pNode = SearchGuild(guild_name);

	if( pNode == NULL ) return FALSE;

	for( int n=0; n<MAX_GUILD; n++)
	{
		if( pNode->Use[n] == 1 )
		{
			if( strcmp(pNode->Names[n], player_name) == 0 )
			{
				pNode->pServer[n] = pServer;
				return TRUE;
			}
		}		
	}
	return FALSE;
}

BOOL	gGuildNoticeStringCheck(char* notice)
{	// ����üũ
	int len = strlen(notice);	

	for( int i = 0; i < len; i++ )
	{
		if( notice[i] & 0x80 )
		{	// �ι���Ʈ ���ڿ� üũ 
			if( gLanguage == 2 )
			{	// �Ϻ��� ��� ���������̽� ������ �Ǿ�� �ȴ�
				// ���� �����̽��� 0x8140
				if( (BYTE)notice[i] != 0x81 )
				{					
					return TRUE;
				}
				else
				{
					if( notice[i+1] != 0x40 )
					{
						return TRUE;
					}
				}
				i++;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			if( notice[i] != 0x20 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

