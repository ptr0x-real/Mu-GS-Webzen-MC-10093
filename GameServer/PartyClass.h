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
	int     Count;	// ��Ƽ�������� ����ΰ�?
	int		Number[MAX_PARTYUSER];   //UserNumber
	int		DbNumber[MAX_PARTYUSER]; // ���� Ȯ�ο� ��ȣ
	short   m_MaxLevel;
	short   m_MinLevel;
	
#ifdef UPDATE_PK_SYSTEM_20070302	// ��Ƽ�� ���θ� �г�Ƽ ����
	char	m_UserPKLevel[MAX_PARTYUSER];	// ���� ���θ� ���
	char	m_PKPartyLevel;					// ���θ� �г�Ƽ�� �������� ���� m_PK_Level�� ����
	int		m_nPKUserCount;					// ��Ƽ�� ���� ���θ� ��
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

#ifdef UPDATE_PK_SYSTEM_20070302	// ��Ƽ�� ���θ� �г�Ƽ ����
	char GetPKPartyPenalty(int party_number);	// ���θ� �г�Ƽ ����
	void UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char userPKLevel);
	void UpdatePKPartyPanalty(int party_number);	// ���θ� �г�Ƽ ����	
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	void ChangeLeader(int party_number);
#endif
};

extern int GetPartyCount(int party_number);

#endif // !defined(AFX_PARTYCLASS_H__B80109FF_DACE_40A5_9843_D34BFCAC1030__INCLUDED_)
