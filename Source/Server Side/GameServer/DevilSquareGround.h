// DevilSquareGround.h: interface for the CDevilSquareGround class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVILSQUAREGROUND_H__4FC6F46A_C499_4AD3_A92A_B67AE8697DB7__INCLUDED_)
#define AFX_DEVILSQUAREGROUND_H__4FC6F46A_C499_4AD3_A92A_B67AE8697DB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <algorithm>
using namespace std;

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "User.h"
#include "Protocol.h"

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
#define		MAX_DEVILSQUARE		7		// �ִ� �Ǹ��� �����
#else
#ifdef DEVILSQUARE_EXTEND_20050221
#define		MAX_DEVILSQUARE		6		// �ִ� �Ǹ��� �����
#else
#define		MAX_DEVILSQUARE		4
#endif
#endif	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010

typedef vector<LPOBJECTSTRUCT>	DevilSquareRankList;

typedef struct 
{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		m_Type;					// ���� Type
#else
	BYTE		m_Type;					// ���� Type
#endif
	int			m_RegenStartTime;		// Regen ���۽ð�
	int	 	    m_RegenEndTime;			// Regen ���ð�
#ifdef DEVILSQUARE_EXTEND_20050221
	int			m_iDevilSquareNumber;	// �����ȣ
#endif
}DevilSquareMonsterInfo, *LPDevilSquareMonsterInfo;

typedef struct 
{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		m_Type;					// ���� Type
#else
	BYTE		m_Type;					// ���� Type
#endif
	int			m_RegenStartTime;		// Regen ���۽ð�
	int	 	    m_RegenEndTime;			// Regen ���ð�
	int			X;
	int			Y;
	int			TX;
	int			TY;
#ifdef DEVILSQUARE_EXTEND_20050221
	int			m_iDevilSquareNumber;	// �����ȣ
#endif
}DevilSquareBossMonsterInfo, *LPDevilSquareBossMonsterInfo;


#define MAX_DEVILSQUAREMONSTERCOUNT			15

#ifdef FOR_JAPAN
	#define MAX_DEVILSQUARE_OBJCOUNT		15	
#else
	#ifdef DEC_MAX_DEVILSQUARE_OBJCOUNT
	#define MAX_DEVILSQUARE_OBJCOUNT		10	
	#else
	#define MAX_DEVILSQUARE_OBJCOUNT		20	// ���������� �ο� ����
	#endif
#endif

class CDevilSquareGround  
{
	// ������ ���� ����
	DevilSquareMonsterInfo			m_DevilSquareMonsterInfo[MAX_DEVILSQUAREMONSTERCOUNT];	
	// ������ ���� ��������
	DevilSquareBossMonsterInfo		m_DevilSquareBossMonsterInfo[MAX_DEVILSQUAREMONSTERCOUNT];	

	// ��������� �ӽ� ���̺�
	DevilSquareRankList			m_DevilSquareRankList;

	// �������� ����� �ӽ����̺�
	PMSG_DEVILSQUARERESULT		m_DevilSquareScoreInfoTOP10;

	// ���� ��ȣ
	int							m_iIndex;
	// ���ʽ� ����
	int							m_Bonus[4][2];

	// ������ �ο���
	long						m_dwObjCount;

#ifdef DEVILSQUARE_EXTEND_20050221				// ��ũ�ε� ���ʽ� ���̺� �߰� + ���尳�� ��ũ��
	int							m_BonusScoreTable[5][MAX_DEVILSQUARE];
#else
	int							m_BonusScoreTable[4][4];
#endif
public:
	CDevilSquareGround();
	virtual ~CDevilSquareGround();

	////////////////////////////////////////////////////////////////////////////////////////////
	// �ʱ�ȭ
	////////////////////////////////////////////////////////////////////////////////////////////
	void		Init(int Index);
	void		Clear();
	// ���ʽ� ������ ����
	void		SetBonus(int index, int zen, int exp);
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	void		ApplyBonusRate( FLOAT fRewardExpRate );
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	// ������ ���� ����
	void		Set(WORD type, int starttime, int endtime);
	// ���� ����(�ѹ��� ����) ����
	void		SetBoss(WORD type, int starttime, int x, int y, int tx, int ty);
#else
	// ������ ���� ����
	void		Set(BYTE type, int starttime, int endtime);
	// ���� ����(�ѹ��� ����) ����
	void		SetBoss(BYTE type, int starttime, int x, int y, int tx, int ty);
#endif


	////////////////////////////////////////////////////////////////////////////////////////////
	// �ο� ����
	////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ī��Ʈ�� ������Ų��
	void		IncObjCount();
	long		GetObjCount();

	////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ����
	////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ���� ����
	void		RegenBossMonster(int currtime);
	// ���� Type�� �����ϴ��� üũ
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	BOOL		FindMonsterType(WORD type);
#else
	BOOL		FindMonsterType(BYTE type);
#endif
	// ���� �ð��� ������ �ɼ� �ִ� Monster Type�� �����ش�
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		GetMonsterType(int currtime);
#else	
	BYTE		GetMonsterType(int currtime);
#endif

	// ��ŷ ������ �����Ѵ�
	void		SendRankingInfo(LPOBJECTSTRUCT lpObj);

	////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ����
	////////////////////////////////////////////////////////////////////////////////////////////
	// ������ Ŭ����
	void		ClearScore();
	// ���� ����
	void		SortScore();
	// ���� ������ ���ؼ� ��ü ����
	void		InsertObj(LPOBJECTSTRUCT lpObj);
	// ������ ��ü���� ���� ����
	void		SendScore();
};

#endif // !defined(AFX_DEVILSQUAREGROUND_H__4FC6F46A_C499_4AD3_A92A_B67AE8697DB7__INCLUDED_)
