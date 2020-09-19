// KanturuMaya.h: interface for the CKanturuMaya class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUMAYA_H__56039896_6802_410A_9376_A78A1444CD31__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUMAYA_H__56039896_6802_410A_9376_A78A1444CD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 마야는 전투에 직접 참여하지 않고 광역 스킬을 사용하기 위한 데이터로만 사용한다.
// 마야는 실제 마야전이 이루어 지는 맵에 위치하지 않고 멀리 떨어진 곳에 있다.

class CKanturuMaya  
{
private:
	int					m_iMayaObjIndex;		// 마야의 Object Index

	BOOL				m_bMonsterKillCount;	// 몬스터와의 전투중 10마리 죽을 때마다 스킬 사용

	int					m_iMayaSkillTime;		// 마야가 사용하는 스킬(브로큰 샤워) 시간
	int					m_iIceStormCount;		// 아이스스톰을 몇 번 사용했는가? ( 통계를 위해 )

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
