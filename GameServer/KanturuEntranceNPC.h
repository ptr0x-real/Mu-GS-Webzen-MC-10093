// KanturuEntranceNPC.h: interface for the CKanturuEntranceNPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUENTRANCENPC_H__45219BD5_DDB1_47CD_B1BE_8BAD84E6DCE4__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUENTRANCENPC_H__45219BD5_DDB1_47CD_B1BE_8BAD84E6DCE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuEntranceNPC  
{
public:
	CKanturuEntranceNPC();
	virtual ~CKanturuEntranceNPC();

	void	NotifyEntranceInfo( int iIndex );
	void	NotifyResultEnterKanturuBossMap( int iIndex );

};

extern CKanturuEntranceNPC	g_KanturuEntranceNPC;

#endif // !defined(AFX_KANTURUENTRANCENPC_H__45219BD5_DDB1_47CD_B1BE_8BAD84E6DCE4__INCLUDED_)
