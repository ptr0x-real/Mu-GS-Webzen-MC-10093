// EledoradoEvent.h: interface for the CEledoradoEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEDORADOEVENT_H__B4B4C46B_2260_4EB7_B899_D54AE29314DC__INCLUDED_)
#define AFX_ELEDORADOEVENT_H__B4B4C46B_2260_4EB7_B899_D54AE29314DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEledoradoEvent  
{
	BOOL    EventState;
	
	DWORD	GoldGoblenEventStartTime;
	DWORD	TitanEventStartTime;
	DWORD	GoldDerconEventStartTime;
	DWORD	DevilLizardKingEventStartTime;
	DWORD	KanturEventStartTime;	
	
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	DWORD	GoldenRabbitEventStartTime;
	DWORD	GoldenDarkKnightEventStartTime;
	DWORD   GoldenDevilEventStartTime;
	DWORD	GoldenStoneMonsterEventStartTime;
	DWORD	GoldenCrustEventStartTime;
	DWORD	GoldenSatirosEventStartTime;
	DWORD	GoldenTwintailEventStartTime;
	DWORD	GoldenIronKnightEventStartTime;
	DWORD	GoldenNeipinEventStartTime;
	DWORD   GoldenGreatDragonEventStartTime;
#endif

	int		m_BossTitanMapNumber;
	int		m_BossTitanMapX;
	int		m_BossTitanMapY;

	int		m_BossDevilLizardKingMapNumber;
	int		m_BossDevilLizardKingMapX;
	int		m_BossDevilLizardKingMapY;

	int		m_BossKanturMapNumber;
	int		m_BossKanturMapX;
	int		m_BossKanturMapY;
	
#ifdef EVENT_MANAGEMENT_20040711
	BOOL	m_bMenualStart;
#endif

public:
	CEledoradoEvent();
	virtual ~CEledoradoEvent();

	void	Init();
	void	SetEventState(BOOL State);

	void	Run();

	void	RegenGoldGoblen();		// 황금 고블린
	void	RegenTitan();			// 타이탄 
	void	RegenGoldDercon();		// 골드데르콘
	void	RegenDevilLizardKing();	// 데블 리자드킹
	void    RegenKantur();			// 칸투르

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	void	RegenGoldenRabbit();		// 황금토끼
	void	RegenGoldenDarkKnight();	// 황금다크나이트
	void    RegenGoldenDevil();			// 황금데빌 
	void	RegenGoldenStoneMonster();	// 황금돌괴물
	void	RegenGoldenCrust();			// 황금크러스트
	void    RegenGoldenSatiros();		// 황금사티로스
	void	RegenGoldenTwintail();		// 황금트윈테일 
	void	RegenGoldenIronKnight();	// 황금아이언나이트
	void	RegenGoldenNeipin();		// 황금네이핀
	void	RegenGoldenGreatDragon();	// 그레이트황금 드래곤 
#endif

	void	CheckGoldDercon(int MapNumber);

#ifdef EVENT_MANAGEMENT_20040711
	void	SetMenualStart (BOOL bMenualStart)	{	m_bMenualStart = bMenualStart;	}
	
	void	Start_Menual ();
	void	End_Menual ();
#endif
	
public:

	int		m_BossGoldDerconMapNumber[3];	
};

extern CEledoradoEvent gEledoradoEvent;

#endif // !defined(AFX_ELEDORADOEVENT_H__B4B4C46B_2260_4EB7_B899_D54AE29314DC__INCLUDED_)
