// ConMember.h: interface for the CConMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONMEMBER_H__69F6AFDF_3B57_4270_85A5_C2923B65C76F__INCLUDED_)
#define AFX_CONMEMBER_H__69F6AFDF_3B57_4270_85A5_C2923B65C76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#ifdef CONNECT_MEMBER_COUNT_INFINITE_20040823
#include "map"					// set에 버그가 있어서 부득이 하게 map을 사용
#include "string"
using namespace std;
#else
#define MAX_CONNECTMEMBER		60
#endif

class CConMember  
{
public:
	CConMember();
	virtual ~CConMember();

#ifdef CONNECT_MEMBER_COUNT_INFINITE_20040823
	map <string, int>	m_szAccount;		// value의 형인 int는 dummy값
#else
	char szAccount[MAX_CONNECTMEMBER][MAX_IDSTRING+1];
#endif	


	void Init();
	BOOL IsMember(char *AccountID);
	void Load(char *filename);



};


extern CConMember	ConMember;

#endif // !defined(AFX_CONMEMBER_H__69F6AFDF_3B57_4270_85A5_C2923B65C76F__INCLUDED_)
