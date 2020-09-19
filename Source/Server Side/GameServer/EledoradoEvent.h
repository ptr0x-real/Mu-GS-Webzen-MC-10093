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

	void	RegenGoldGoblen();		// Ȳ�� ���
	void	RegenTitan();			// Ÿ��ź 
	void	RegenGoldDercon();		// ��嵥����
	void	RegenDevilLizardKing();	// ���� ���ڵ�ŷ
	void    RegenKantur();			// ĭ����

#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311
	void	RegenGoldenRabbit();		// Ȳ���䳢
	void	RegenGoldenDarkKnight();	// Ȳ�ݴ�ũ����Ʈ
	void    RegenGoldenDevil();			// Ȳ�ݵ��� 
	void	RegenGoldenStoneMonster();	// Ȳ�ݵ�����
	void	RegenGoldenCrust();			// Ȳ��ũ����Ʈ
	void    RegenGoldenSatiros();		// Ȳ�ݻ�Ƽ�ν�
	void	RegenGoldenTwintail();		// Ȳ��Ʈ������ 
	void	RegenGoldenIronKnight();	// Ȳ�ݾ��̾���Ʈ
	void	RegenGoldenNeipin();		// Ȳ�ݳ�����
	void	RegenGoldenGreatDragon();	// �׷���ƮȲ�� �巡�� 
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
