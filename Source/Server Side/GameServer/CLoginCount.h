// CLoginCount.cpp  CLoginCount.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __CLOGINCOUNT_H__
#define __CLOGINCOUNT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLoginCount
{
	int m_count;

public:


	CLoginCount();
	~CLoginCount();
	
	void Init();
	void Add();
	void Delete();
	int Get();
};

#endif // CCount.H
