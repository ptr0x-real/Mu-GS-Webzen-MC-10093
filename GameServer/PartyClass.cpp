// PartyClass.cpp: implementation of the PartyClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "user.h"
#include "..\\Include\\prodef.h"
#include "Protocol.h"
#include "PartyClass.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PartyClass::PartyClass()
{

}

PartyClass::~PartyClass()
{
	
}


// 파티에 접근 가능한지 체크해본다.
BOOL PartyClass::IsParty(int party_number)
{
	if( party_number < 0 || 
		party_number > MAX_PARTY ) return FALSE;
	
	if( m_PartyS[party_number].Count == 0 )	// 파티가 없는거라면??
		return FALSE;

	return TRUE;
}

// 파티의 리더가 맞는지 검사한다.
BOOL PartyClass::Isleader(int party_number, int usernumber, int dbnumber)
{
	if( IsParty(party_number) == FALSE ) return FALSE;

	if( (m_PartyS[party_number].Number[0] == usernumber) &&
		(m_PartyS[party_number]. DbNumber[0] == dbnumber) )
	{
		return TRUE;
	}
	return FALSE;
}

// 파티의 인원수를 리턴한다.
int PartyClass::GetCount(int party_number)
{
	if( IsParty(party_number) == FALSE )
		return 0;

	return m_PartyS[party_number].Count;
}

BOOL PartyClass::GetIndexUser(int party_number, int index, int & usernumber, int & dbnumber)
{
	if( IsParty(party_number) == FALSE ) return FALSE;
	if( index < 0 || index > MAX_PARTYUSER-1 ) return FALSE;

	usernumber = m_PartyS[party_number].Number[index];
	dbnumber   = m_PartyS[party_number].DbNumber[index];

	return TRUE;
}

int PartyClass::GetIndex(int party_number, int usernumber, int dbnumber)
{
	if( IsParty(party_number) == FALSE ) return FALSE;
	
	for( int n=0; n<MAX_PARTYUSER; n++)
	{
		if( m_PartyS[party_number].Number[n] == usernumber ||
			m_PartyS[party_number].DbNumber[n] == dbnumber )
		{
			return n;
		}
	}	
	return -1;
}



/*BOOL PartyClass::GetIndexUser(int party_number, int index, int & usernumber, int & dbnumber)
{
	if( IsParty(party_number) == FALSE ) return FALSE;
	if( index < 0 || index > MAX_PARTYUSER-1 ) return FALSE;

	usernumber = m_PartyS[party_number].Number[index];
	dbnumber   = m_PartyS[party_number].DbNumber[index];
}*/

// 파티를 생성한다.
int PartyClass::Create(int usernumber, int dbnumber, int level)
{
	int n=m_PartyCount;
	int loopcount=0;

	while(1)
	{
		if( m_PartyS[n].Count == 0 )
		{
			m_PartyS[n].Count = 1;
			m_PartyS[n].Number[0] = usernumber;
			m_PartyS[n].DbNumber[0] = dbnumber;
			for( int i=1; i<MAX_PARTYUSER; i++)
			{
				m_PartyS[n].DbNumber[i] = -1;
				m_PartyS[n].Number[i]   = -1;
			}
			m_PartyCount++;
			m_PartyS[n].m_MaxLevel = 0;
			m_PartyS[n].m_MinLevel = 0;
			SetLevel(n, level);
			if( m_PartyCount > MAX_PARTY-1 ) m_PartyCount = 0;

#ifdef UPDATE_PK_SYSTEM_20070302	// 파티 생성시 파티 리더가 살인마인지 확인
			m_PartyS[n].m_UserPKLevel[0] = gObj[usernumber].m_PK_Level;
			if(m_PartyS[n].m_UserPKLevel[0] >= PK_LEVEL_DEFAULT+2)
			{
				m_PartyS[n].m_PKPartyLevel		= m_PartyS[n].m_UserPKLevel[0];
				m_PartyS[n].m_nPKUserCount		= 1;
			}
			else
			{
				m_PartyS[n].m_PKPartyLevel		= PK_LEVEL_DEFAULT;
				m_PartyS[n].m_nPKUserCount		= 0;
			}
#endif
			return n;
		}
		n++;
		if( n > MAX_PARTY-1 ) n = 0;
		loopcount++;
		if( loopcount > MAX_PARTY-1 ) break;
	}

	return -1;
}

BOOL PartyClass::SetLevel(int party_number, int level)
{
	if(m_PartyS[party_number].m_MaxLevel < level )
		m_PartyS[party_number].m_MaxLevel = level;

	if( m_PartyS[party_number].m_MinLevel == 0 )
		m_PartyS[party_number].m_MinLevel = level;

	if(m_PartyS[party_number].m_MinLevel > level )
		m_PartyS[party_number].m_MinLevel = level;

	//LogAdd("파티 최대 최소 레벨 : %d %d %d", m_PartyS[party_number].m_MaxLevel, m_PartyS[party_number].m_MinLevel, level);
	return TRUE;
}


BOOL  PartyClass::GetLevel(int party_number, int & maxlevel, int & minlevel )
{
	if( IsParty(party_number) == FALSE ) return 0;

	maxlevel = m_PartyS[party_number].m_MaxLevel;
	minlevel = m_PartyS[party_number].m_MinLevel;
	return TRUE;
}

BOOL PartyClass::Destroy(int party_number)
{
	if( IsParty(party_number) == FALSE ) return FALSE;

	/*if( usernumber == m_PartyS[party_number].Number[0] &&
		dbnumber   == m_PartyS[party_number].DbNumber[0] )
	{
	*/
		m_PartyS[party_number].Count = 0;
		m_PartyS[party_number].Number[0] = -1;
		m_PartyS[party_number].DbNumber[0] = -1;
		m_PartyCount--;
//		return TRUE;
//	}
	return FALSE;
}

// 파티에 사용자를 추가한다.
int PartyClass::Add(int party_number, int usernumber, int dbnumber, int level)
{
	int i;
	if( IsParty(party_number) == FALSE ) return -1;

	// 파티가 꽉찼다면 
	if( m_PartyS[party_number].Count >= MAX_PARTY ) 
		return -1;


	// 같은 캐릭터가 있는지..
	for( i=0; i<MAX_PARTYUSER; i++)
	{
		if( m_PartyS[party_number].Number[i] >= 0 )
		{
			if( m_PartyS[party_number].Number[i] == usernumber &&
				m_PartyS[party_number].DbNumber[i] == dbnumber )
			{
				return -2;
			}
		}
	}
	for( i=0; i<MAX_PARTYUSER; i++)
	{
		if( m_PartyS[party_number].Number[i] == -1 )
		{
			m_PartyS[party_number].Number[i]   = usernumber;
			m_PartyS[party_number].DbNumber[i] = dbnumber;
			m_PartyS[party_number].Count++;
			SetLevel(party_number, level);

#ifdef UPDATE_PK_SYSTEM_20070302	// 파티원 추가시 살인마인지 확인
			m_PartyS[party_number].m_UserPKLevel[i] = gObj[usernumber].m_PK_Level;
			if(m_PartyS[party_number].m_UserPKLevel[i] >= PK_LEVEL_DEFAULT+2)
			{
				if(m_PartyS[party_number].m_PKPartyLevel < m_PartyS[party_number].m_UserPKLevel[i])
				{
					m_PartyS[party_number].m_PKPartyLevel = m_PartyS[party_number].m_UserPKLevel[i];
				}
				m_PartyS[party_number].m_nPKUserCount++;
			}
#endif
			return i;
		}
	}
	return -1;
}

// 파티에서 사용자를 삭제한다.
int PartyClass::Delete(int party_number, int usernumber, int dbnumber)
{
	if( IsParty(party_number) == FALSE ) return -1;

	for( int i=0; i<MAX_PARTYUSER; i++)
	{
		if( m_PartyS[party_number].Number[i] == usernumber && 
			m_PartyS[party_number].DbNumber[i] == dbnumber )
		{
			m_PartyS[party_number].Number[i] = -1;
#ifdef PARTYMEMBER_LIFELIST_FIX_20040531
			m_PartyS[party_number].DbNumber[i] = -1;
#endif
			m_PartyS[party_number].Count--;

#ifdef UPDATE_PK_SYSTEM_20070302
			m_PartyS[party_number].m_UserPKLevel[i] = PK_LEVEL_DEFAULT;
#endif
			return i;
		}
	}
	
	return -1;
}

// 파티에서 사용자를 삭제한다.
void PartyClass::Delete(int party_number, int index)
{
	if( IsParty(party_number) == FALSE ) return;
	if( index < 0 || index > MAX_PARTYUSER-1 ) return;

	if( m_PartyS[party_number].Number[index] >= 0 )
	{
		m_PartyS[party_number].Number[index] = -1;
#ifdef PARTYMEMBER_LIFELIST_FIX_20040531
		m_PartyS[party_number].DbNumber[index] = -1;
#endif
		m_PartyS[party_number].Count--;

#ifdef UPDATE_PK_SYSTEM_20070302	
		m_PartyS[party_number].m_UserPKLevel[index] = PK_LEVEL_DEFAULT;
#endif
	}
}

int PartyClass::GetPartyCount(int party_number)
{
	if( IsParty(party_number) == FALSE ) return -1;

	return m_PartyS[party_number].Count;
}

void PartyClass::Paint(int party_number)
{
#ifdef FOR_CHINA
	return;
#endif
	
#ifdef FOR_TAIWAN
	return;
#endif
	
	if( IsParty(party_number) == FALSE ) return;

	int usern=0;
	
	
	wsprintf(m_szTempPaint, "PartyMember(%d) :",m_PartyS[party_number].Count);

	for( int i=0; i<MAX_PARTYUSER; i++)
	{
		if( m_PartyS[party_number].Number[i] && m_PartyS[party_number].DbNumber[i] )
		{
			usern = m_PartyS[party_number].Number[i];
#ifdef PARTY_ADD_LOGFIX_20050801
			if (!gObjIsConnected(usern))
				continue;
#endif
			strcat(m_szTempPaint,"[");
			strcat(m_szTempPaint, gObj[usern].AccountID);
			strcat(m_szTempPaint,"][");
			strcat(m_szTempPaint, gObj[usern].Name);
			strcat(m_szTempPaint,"], ");
		}
	}
	LogAddTD(m_szTempPaint);
}

// 파티원 모두에게 파티원들의 생명을 알린다.
void PartyClass::PartyMemberLifeSend(int party_number)
{
	if( IsParty(party_number) == FALSE ) return;

	PMSG_DEFAULT_COUNT		pCount;
	PMSG_PARTYLIFEALL		pList;
	int usern;
	BYTE sendbuf[512];
	int lOfs=0;
	int tlife=0;
	
	PHeadSetB((LPBYTE)&pCount, 0x44, sizeof( pCount ));
	
	pCount.Count = m_PartyS[party_number].Count;

	lOfs += sizeof(pCount);

	for( int i=0; i<MAX_PARTYUSER; i++)
	{
#ifdef PARTYMEMBER_LIFELIST_FIX_20040531
		if( m_PartyS[party_number].Number[i] >= 0 && m_PartyS[party_number].DbNumber[i] >= 0 )
#else
		if( m_PartyS[party_number].Number[i] && m_PartyS[party_number].DbNumber[i] )
#endif		
		{
			usern = m_PartyS[party_number].Number[i];
#ifdef PARTYMEMBER_LIFELIST_FIX_20040531
			if( gObjIsConnected(usern) )
#else
			if( usern >= 0 )
#endif
			{
				pList.Number  = (i<<4)&0xF0;
				tlife		  = ((BYTE)(gObj[usern].Life/((gObj[usern].MaxLife+gObj[usern].AddLife)/10.0f)))&0x0F;
				pList.Number |= tlife;
				//pList.LifeAndMaxLife |= ((gObj[usern].Life/(gObj[usern].Life/100))&0xF0<<4);
				//LogAdd("%s %d %d %d", gObj[usern].Name, tlife, gObj[usern].Life, gObj[usern].MaxLife);
				memcpy(sendbuf+lOfs, (char*)&pList, sizeof( pList) );
				lOfs += sizeof( pList );
			}
#ifdef PARTYMEMBER_LIFELIST_FIX_20040531
			else {
				// 접속해 있지 않다면 이 사용자는 파티 리스트에서 제외시킨다.
				m_PartyS[party_number].Number[i]	= -1;
				m_PartyS[party_number].DbNumber[i]	= -1;
				m_PartyS[party_number].Count--;
			}
#endif
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));

	for( int i=0; i<MAX_PARTYUSER; i++)
	{			
		usern = m_PartyS[party_number].Number[i];
		if( usern >= 0 )
		{
			DataSend(usern , (LPBYTE)sendbuf, lOfs);
		}
	}
}

#ifdef UPDATE_PK_SYSTEM_20070302	// 파티에 살인마 패널티 적용
char PartyClass::GetPKPartyPenalty(int party_number)	// 살인마 패널티 여부
{
	return m_PartyS[party_number].m_PKPartyLevel;
}

void PartyClass::UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char userPKLevel)
{
	if( IsParty(party_number) == FALSE ) return;

	for( int i=0; i<MAX_PARTYUSER; i++)
	{
		if( m_PartyS[party_number].Number[i] == usernumber && 
			m_PartyS[party_number].DbNumber[i] == dbnumber )
		{
			m_PartyS[party_number].m_UserPKLevel[i] = userPKLevel;
			return;
		}
	}
}

void PartyClass::UpdatePKPartyPanalty(int party_number)	// 살인마 패널티 갱신
{
	int PKUserCount			= 0;
	int usern				= 0;
	for( int i=0; i<MAX_PARTYUSER; i++)
	{
		usern = m_PartyS[party_number].Number[i];
		if( usern >= 0 )
		{
			if(m_PartyS[party_number].m_UserPKLevel[i] >= PK_LEVEL_DEFAULT+2)
			{
				if(m_PartyS[party_number].m_PKPartyLevel < m_PartyS[party_number].m_UserPKLevel[i])
				{
					m_PartyS[party_number].m_PKPartyLevel = m_PartyS[party_number].m_UserPKLevel[i];
				}
				PKUserCount++;
			}				
		}
	}

	m_PartyS[party_number].m_nPKUserCount = PKUserCount;

	if(PKUserCount == 0)
	{
		m_PartyS[party_number].m_PKPartyLevel = PK_LEVEL_DEFAULT;
	}
}
#endif	// UPDATE_PK_SYSTEM_20070302

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
void PartyClass::ChangeLeader(int party_number)
{
	int usern	= 0;
	for( int i=0; i<MAX_PARTYUSER; i++)
	{
		usern = m_PartyS[party_number].Number[i];
		if( usern >= 0 )
		{
			m_PartyS[party_number].Number[0] = m_PartyS[party_number].Number[i];
			m_PartyS[party_number].Number[i] = -1;

			m_PartyS[party_number].DbNumber[0] = m_PartyS[party_number].DbNumber[i];
			m_PartyS[party_number].DbNumber[i] = -1;

			m_PartyS[party_number].m_UserPKLevel[0] = m_PartyS[party_number].m_UserPKLevel[i];
			m_PartyS[party_number].m_UserPKLevel[i] = PK_LEVEL_DEFAULT;
			
			break;
		}
	}
}
#endif