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
#define		MAX_DEVILSQUARE		7		// 최대 악마의 광장수
#else
#ifdef DEVILSQUARE_EXTEND_20050221
#define		MAX_DEVILSQUARE		6		// 최대 악마의 광장수
#else
#define		MAX_DEVILSQUARE		4
#endif
#endif	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010

typedef vector<LPOBJECTSTRUCT>	DevilSquareRankList;

typedef struct 
{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		m_Type;					// 몬스터 Type
#else
	BYTE		m_Type;					// 몬스터 Type
#endif
	int			m_RegenStartTime;		// Regen 시작시간
	int	 	    m_RegenEndTime;			// Regen 끝시간
#ifdef DEVILSQUARE_EXTEND_20050221
	int			m_iDevilSquareNumber;	// 광장번호
#endif
}DevilSquareMonsterInfo, *LPDevilSquareMonsterInfo;

typedef struct 
{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		m_Type;					// 몬스터 Type
#else
	BYTE		m_Type;					// 몬스터 Type
#endif
	int			m_RegenStartTime;		// Regen 시작시간
	int	 	    m_RegenEndTime;			// Regen 끝시간
	int			X;
	int			Y;
	int			TX;
	int			TY;
#ifdef DEVILSQUARE_EXTEND_20050221
	int			m_iDevilSquareNumber;	// 광장번호
#endif
}DevilSquareBossMonsterInfo, *LPDevilSquareBossMonsterInfo;


#define MAX_DEVILSQUAREMONSTERCOUNT			15

#ifdef FOR_JAPAN
	#define MAX_DEVILSQUARE_OBJCOUNT		15	
#else
	#ifdef DEC_MAX_DEVILSQUARE_OBJCOUNT
	#define MAX_DEVILSQUARE_OBJCOUNT		10	
	#else
	#define MAX_DEVILSQUARE_OBJCOUNT		20	// 데빌스퀘어 인원 제한
	#endif
#endif

class CDevilSquareGround  
{
	// 리젠될 몬스터 정보
	DevilSquareMonsterInfo			m_DevilSquareMonsterInfo[MAX_DEVILSQUAREMONSTERCOUNT];	
	// 리젠될 보스 몬스터정보
	DevilSquareBossMonsterInfo		m_DevilSquareBossMonsterInfo[MAX_DEVILSQUAREMONSTERCOUNT];	

	// 순위집계용 임시 테이블
	DevilSquareRankList			m_DevilSquareRankList;

	// 순위집계 저장용 임시테이블
	PMSG_DEVILSQUARERESULT		m_DevilSquareScoreInfoTOP10;

	// 광장 번호
	int							m_iIndex;
	// 보너스 점수
	int							m_Bonus[4][2];

	// 광장의 인원수
	long						m_dwObjCount;

#ifdef DEVILSQUARE_EXTEND_20050221				// 다크로드 보너스 테이블 추가 + 광장개수 매크로
	int							m_BonusScoreTable[5][MAX_DEVILSQUARE];
#else
	int							m_BonusScoreTable[4][4];
#endif
public:
	CDevilSquareGround();
	virtual ~CDevilSquareGround();

	////////////////////////////////////////////////////////////////////////////////////////////
	// 초기화
	////////////////////////////////////////////////////////////////////////////////////////////
	void		Init(int Index);
	void		Clear();
	// 보너스 점수를 설정
	void		SetBonus(int index, int zen, int exp);
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	void		ApplyBonusRate( FLOAT fRewardExpRate );
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	// 리젠될 몬스터 설정
	void		Set(WORD type, int starttime, int endtime);
	// 보스 몬스터(한번만 등장) 설정
	void		SetBoss(WORD type, int starttime, int x, int y, int tx, int ty);
#else
	// 리젠될 몬스터 설정
	void		Set(BYTE type, int starttime, int endtime);
	// 보스 몬스터(한번만 등장) 설정
	void		SetBoss(BYTE type, int starttime, int x, int y, int tx, int ty);
#endif


	////////////////////////////////////////////////////////////////////////////////////////////
	// 인원 제한
	////////////////////////////////////////////////////////////////////////////////////////////
	// 유저 카운트를 증가시킨다
	void		IncObjCount();
	long		GetObjCount();

	////////////////////////////////////////////////////////////////////////////////////////////
	// 게임 진행
	////////////////////////////////////////////////////////////////////////////////////////////
	// 보스 몬스터 리젠
	void		RegenBossMonster(int currtime);
	// 같은 Type이 존재하는지 체크
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	BOOL		FindMonsterType(WORD type);
#else
	BOOL		FindMonsterType(BYTE type);
#endif
	// 현재 시간에 리스폰 될수 있는 Monster Type을 돌려준다
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD		GetMonsterType(int currtime);
#else	
	BYTE		GetMonsterType(int currtime);
#endif

	// 랭킹 정보를 전송한다
	void		SendRankingInfo(LPOBJECTSTRUCT lpObj);

	////////////////////////////////////////////////////////////////////////////////////////////
	// 순위 집계
	////////////////////////////////////////////////////////////////////////////////////////////
	// 점수를 클리어
	void		ClearScore();
	// 점수 정렬
	void		SortScore();
	// 정수 정렬을 위해서 객체 삽입
	void		InsertObj(LPOBJECTSTRUCT lpObj);
	// 각각의 객체에게 점수 전송
	void		SendScore();
};

#endif // !defined(AFX_DEVILSQUAREGROUND_H__4FC6F46A_C499_4AD3_A92A_B67AE8697DB7__INCLUDED_)
