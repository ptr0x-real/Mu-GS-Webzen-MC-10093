// PartyClass.h: interface for the PartyClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTYCLASS_H__B80109FF_DACE_40A5_9843_D34BFCAC1030__INCLUDED_)
#define AFX_PARTYCLASS_H__B80109FF_DACE_40A5_9843_D34BFCAC1030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\include\\define.h"

#define MAX_PARTY		MAX_OBJECT
#define MAX_PARTYUSER	5

typedef struct
{
	int     Count;	// 파티구성원이 몇명인가?
	int		Number[MAX_PARTYUSER];   //UserNumber
	int		DbNumber[MAX_PARTYUSER]; // 유저 확인용 번호
	short   m_MaxLevel;
	short   m_MinLevel;
	
#ifdef UPDATE_PK_SYSTEM_20070302	// 파티에 살인마 패널티 적용
	char	m_UserPKLevel[MAX_PARTYUSER];	// 유저 살인마 등급
	char	m_PKPartyLevel;					// 살인마 패널티를 적용할지 여부 m_PK_Level와 동일
	int		m_nPKUserCount;					// 파티에 속한 살인마 수
#endif
} PARTY_STRUCT;

class PartyClass  
{
private :
	int m_PartyCount;

public :
	PARTY_STRUCT	m_PartyS[MAX_PARTY];
	char m_szTempPaint[256];
	
public:
	PartyClass();
	virtual ~PartyClass();
	BOOL IsParty(int party_number);
	int  Create(int usernumber, int dbnumber, int level);
	BOOL Destroy(int party_number);
	BOOL Add(int party_number, int usernumber, int dbnumber, int level);
	int  Delete(int party_number, int usernumber, int dbnumber);
	void Delete(int party_number, int index);
	int  GetCount(int party_number);
	BOOL GetIndexUser(int party_number, int index, int & usernumber, int & dbnumber);
	int  GetPartyCount(int party_number);
	int  GetIndex(int party_number, int usernumber, int dbnumber);
	BOOL SetLevel(int party_number, int level);
	BOOL GetLevel(int party_number, int & maxlevel, int & minlevel );
	BOOL Isleader(int party_number, int usernumber, int dbnumber);
	void Paint(int party_number);
	void PartyMemberLifeSend(int party_number);

#ifdef UPDATE_PK_SYSTEM_20070302	// 파티에 살인마 패널티 적용
	char GetPKPartyPenalty(int party_number);	// 살인마 패널티 여부
	void UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char userPKLevel);
	void UpdatePKPartyPanalty(int party_number);	// 살인마 패널티 갱신	
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	void ChangeLeader(int party_number);
#endif
};

extern int GetPartyCount(int party_number);

#endif // !defined(AFX_PARTYCLASS_H__B80109FF_DACE_40A5_9843_D34BFCAC1030__INCLUDED_)
