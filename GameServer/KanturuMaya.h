// KanturuMaya.h: interface for the CKanturuMaya class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUMAYA_H__56039896_6802_410A_9376_A78A1444CD31__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUMAYA_H__56039896_6802_410A_9376_A78A1444CD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���ߴ� ������ ���� �������� �ʰ� ���� ��ų�� ����ϱ� ���� �����ͷθ� ����Ѵ�.
// ���ߴ� ���� �������� �̷�� ���� �ʿ� ��ġ���� �ʰ� �ָ� ������ ���� �ִ�.

class CKanturuMaya  
{
private:
	int					m_iMayaObjIndex;		// ������ Object Index

	BOOL				m_bMonsterKillCount;	// ���Ϳ��� ������ 10���� ���� ������ ��ų ���

	int					m_iMayaSkillTime;		// ���߰� ����ϴ� ��ų(���ū ����) �ð�
	int					m_iIceStormCount;		// ���̽������� �� �� ����ߴ°�? ( ��踦 ���� )

public:
	CKanturuMaya();
	virtual ~CKanturuMaya();

	// INITIALIZE
	void	Init();

	// MAYA ACT
	void	KanturuMayaAct_IceStorm( int iSkillUsingRate );
	void	KanturuMayaAct_Hands();

	// MAYA DATA
	void	SetMayaObjIndex( int iIndex )	{ m_iMayaObjIndex  = iIndex; }
	int		GetMayaObjIndex()				{ return m_iMayaObjIndex; }

};

#endif // !defined(AFX_KANTURUMAYA_H__56039896_6802_410A_9376_A78A1444CD31__INCLUDED_)
