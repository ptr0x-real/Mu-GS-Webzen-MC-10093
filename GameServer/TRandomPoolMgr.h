// TRandomPoolMgr.h: interface for the TRandomPoolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANDOMPOOLMGR_H__DE55B25E_4E9D_45A3_BAD2_748234E8A7F2__INCLUDED_)
#define AFX_TRANDOMPOOLMGR_H__DE55B25E_4E9D_45A3_BAD2_748234E8A7F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

using namespace std;


//#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

struct sValueNode
{
	int index;
	int weight;
};

enum eRandomPoolSelectType
{
	eRANDOMPOOL_BY_RANDOM = 0,
	eRANDOMPOOL_BY_WEIGHT = 1
};

class TRandomPoolMgr  
{
public:
	TRandomPoolMgr();
	virtual ~TRandomPoolMgr();

private:
	map<int, int> m_listPool;
	int			  m_iSumWeight;

public:
	void	InitPool();
	bool	AddValue(int index, int weight);
	int		GetRandomValue( eRandomPoolSelectType eSelectType );
};


//#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#endif // !defined(AFX_TRANDOMPOOLMGR_H__DE55B25E_4E9D_45A3_BAD2_748234E8A7F2__INCLUDED_)
