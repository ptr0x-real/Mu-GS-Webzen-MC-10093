// DevilSquareGround.cpp: implementation of the CDevilSquareGround class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevilSquareGround.h"

#include "MonsterSetBase.h"

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "..\\include\\cb_protocol.h"
#include "..\\Include\\public.h"

#include "wzUdp.h"
#include "GameMain.h"

#include "wsJoinServerCli.h"
#include "DevilSquare.h"

#ifdef MU_CRYWOLF_PENALTY_20051215
#include "CrywolfSync.h"
#endif

extern wsJoinServerCli wsRServerCli;		//  랭킹서버
extern CMonsterSetBase	gMSetBase;
extern WzUdp			gUdpSocCRank;		// Rank Server

bool DevilSquareScoreSort(const LPOBJECTSTRUCT& lhs, const LPOBJECTSTRUCT& rhs)
{	
	if( lhs->m_nEventScore == rhs->m_nEventScore )
	{	// 점수가 같으면 레벨이 낮은 사람이 우선
		if( lhs->Level < rhs->Level )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	if( lhs->m_nEventScore > rhs->m_nEventScore )
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDevilSquareGround::CDevilSquareGround()
{
	Init(0);
}

CDevilSquareGround::~CDevilSquareGround()
{
	// 광장 사냥점수 가중치 테이블
	m_BonusScoreTable[CLASS_WIZARD][0] = 0;
	m_BonusScoreTable[CLASS_WIZARD][1] = 0;
	m_BonusScoreTable[CLASS_WIZARD][2] = 0;
	m_BonusScoreTable[CLASS_WIZARD][3] = 170;

	m_BonusScoreTable[CLASS_KNIGHT][0] = 20;
	m_BonusScoreTable[CLASS_KNIGHT][1] = 90;
	m_BonusScoreTable[CLASS_KNIGHT][2] = 120;
	m_BonusScoreTable[CLASS_KNIGHT][3] = 400;

	m_BonusScoreTable[CLASS_ELF][0] = 10;
	m_BonusScoreTable[CLASS_ELF][1] = 10;
	m_BonusScoreTable[CLASS_ELF][2] = 10;
	m_BonusScoreTable[CLASS_ELF][3] = 200;

	m_BonusScoreTable[CLASS_MAGUMSA][0] = 0;
	m_BonusScoreTable[CLASS_MAGUMSA][1] = 0;
	m_BonusScoreTable[CLASS_MAGUMSA][2] = 0;
	m_BonusScoreTable[CLASS_MAGUMSA][3] = 0;

#ifdef DEVILSQUARE_EXTEND_20050221	// 추가 보너스 테이블 추가 (5,6광장 + 다크로드)
	m_BonusScoreTable[CLASS_WIZARD][4] = 170;
	m_BonusScoreTable[CLASS_WIZARD][5] = 170;

	m_BonusScoreTable[CLASS_KNIGHT][4] = 400;
	m_BonusScoreTable[CLASS_KNIGHT][5] = 400;

	m_BonusScoreTable[CLASS_ELF][4] = 200;
	m_BonusScoreTable[CLASS_ELF][5] = 200;

	m_BonusScoreTable[CLASS_MAGUMSA][4] = 0;
	m_BonusScoreTable[CLASS_MAGUMSA][5] = 0;

	m_BonusScoreTable[CLASS_DARKLORD][0] = 0;
	m_BonusScoreTable[CLASS_DARKLORD][1] = 0;
	m_BonusScoreTable[CLASS_DARKLORD][2] = 0;
	m_BonusScoreTable[CLASS_DARKLORD][3] = 0;
	m_BonusScoreTable[CLASS_DARKLORD][4] = 0;
	m_BonusScoreTable[CLASS_DARKLORD][5] = 0;
#endif

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 광장7 추가 보너스 테이블
	m_BonusScoreTable[CLASS_WIZARD][6]		= 170;
	m_BonusScoreTable[CLASS_KNIGHT][6]		= 400;
	m_BonusScoreTable[CLASS_ELF][6]			= 200;
	m_BonusScoreTable[CLASS_MAGUMSA][6]		= 0;
	m_BonusScoreTable[CLASS_DARKLORD][6]	= 0;
#endif
}

void CDevilSquareGround::Init(int Index)
{
	m_dwObjCount = 0;
	m_iIndex = Index;
	memset(m_DevilSquareMonsterInfo, 0xff, sizeof(DevilSquareMonsterInfo)*MAX_DEVILSQUAREMONSTERCOUNT);
	memset(m_DevilSquareBossMonsterInfo, 0xff, sizeof(DevilSquareBossMonsterInfo)*MAX_DEVILSQUAREMONSTERCOUNT);
}

void CDevilSquareGround::Clear()
{
	m_dwObjCount = 0;
}

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
void CDevilSquareGround::Set(WORD type, int starttime, int endtime)
#else
void CDevilSquareGround::Set(BYTE type, int starttime, int endtime)
#endif
{
	for( int i  = 0 ; i < MAX_DEVILSQUAREMONSTERCOUNT; i++)
	{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
		if( m_DevilSquareMonsterInfo[i].m_Type == (WORD)0xffff )
#else
		if( m_DevilSquareMonsterInfo[i].m_Type == 0xff )
#endif
		{
			m_DevilSquareMonsterInfo[i].m_Type = type;
			m_DevilSquareMonsterInfo[i].m_RegenStartTime = starttime;
			m_DevilSquareMonsterInfo[i].m_RegenEndTime = endtime;
#ifdef DEVILSQUARE_EXTEND_20050221
			m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber		= m_iIndex;
#endif			
			return;
		}
	}
}

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
void CDevilSquareGround::SetBoss(WORD type, int starttime, int x, int y, int tx, int ty)
#else
void CDevilSquareGround::SetBoss(BYTE type, int starttime, int x, int y, int tx, int ty)
#endif
{
	for( int i  = 0 ; i < MAX_DEVILSQUAREMONSTERCOUNT; i++)
	{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
		if( m_DevilSquareBossMonsterInfo[i].m_Type == (WORD)0xffff )
#else
		if( m_DevilSquareBossMonsterInfo[i].m_Type == 0xff )
#endif
		{
			m_DevilSquareBossMonsterInfo[i].m_Type = type;
			m_DevilSquareBossMonsterInfo[i].m_RegenStartTime = starttime;
			m_DevilSquareBossMonsterInfo[i].X = x;
			m_DevilSquareBossMonsterInfo[i].Y = y;
			m_DevilSquareBossMonsterInfo[i].TX = tx;
			m_DevilSquareBossMonsterInfo[i].TY = ty;			
#ifdef DEVILSQUARE_EXTEND_20050221
			m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber	= m_iIndex;
#endif			
			return;
		}
	}
}

void CDevilSquareGround::SetBonus(int index, int zen, int exp)
{
	m_Bonus[index][0] = exp;
	m_Bonus[index][1] = zen;
}

#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
void CDevilSquareGround::ApplyBonusRate( FLOAT fRewardExpRate )
{
	for( INT i=0; i<4; ++i )	// 각 랭킹별 보상 경험치에 비율을 적용 시킨다.
		m_Bonus[i][1] = (INT)m_Bonus[i][1] * fRewardExpRate;
}
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

void CDevilSquareGround::IncObjCount()
{
	InterlockedIncrement(&m_dwObjCount);	
}

long CDevilSquareGround::GetObjCount()
{
	return m_dwObjCount;
}

void CDevilSquareGround::RegenBossMonster(int currtime)
{
	for( int i  = 0 ; i < MAX_DEVILSQUAREMONSTERCOUNT; i++)
	{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
		if( m_DevilSquareBossMonsterInfo[i].m_Type != (WORD)0xffff )
#else
		if( m_DevilSquareBossMonsterInfo[i].m_Type != 0xff )
#endif
		{			
			if( currtime == m_DevilSquareBossMonsterInfo[i].m_RegenStartTime )				
			{	
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
				int iMapNumber	= GET_DEVILSQUARE_MAPNUMBER(m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber);
				int n = gObjAddMonster(iMapNumber);					// 데빌스퀘어
#else
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
				int n = gObjAddMonster(MAP_INDEX_DEVILSQUARE);		// 데빌스퀘어
#else
				int n = gObjAdd();		
#endif
#endif
				if( n >= 0 )
				{
					gObjSetMonster(n, m_DevilSquareBossMonsterInfo[i].m_Type);					
					
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
					gObj[n].MapNumber = iMapNumber;
#else
					gObj[n].MapNumber = 9;
#endif
					gMSetBase.GetBoxPosition(gObj[n].MapNumber, 
						m_DevilSquareBossMonsterInfo[i].X, m_DevilSquareBossMonsterInfo[i].Y, 
						m_DevilSquareBossMonsterInfo[i].TX, m_DevilSquareBossMonsterInfo[i].TY, 
						gObj[n].X,gObj[n].Y);

					int BossMap  = gObj[n].MapNumber;
					int BossMapX = gObj[n].X;
					int BossMapY = gObj[n].Y;
					LogAddTD("[DevilSquare] Boss %d %d %d create ", BossMap, BossMapX, BossMapY);
					
					gObj[n].TX = gObj[n].X;
					gObj[n].TY = gObj[n].Y;
					gObj[n].MTX = gObj[n].X;
					gObj[n].MTY = gObj[n].Y;
					gObj[n].StartX = (BYTE)gObj[n].X;
					gObj[n].StartY = (BYTE)gObj[n].Y;

					gObj[n].m_bDevilSquareIndex = m_iIndex;

					gObj[n].DieRegen	= 0;
					
					// 악마의 광장에서 보스 몬스터를 구분하기 위해 위치테이블의 값은 -1로 설정
					gObj[n].m_PosNum = -1;
					gObj[n].Live	= 1;
					gObj[n].m_State	= OBJST_CREATE;	
				}
			}
		}
	}
}

// 악마의 광장에 리젠될 몬스터 타입인지 찾는다
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
BOOL CDevilSquareGround::FindMonsterType(WORD type)
#else
BOOL CDevilSquareGround::FindMonsterType(BYTE type)
#endif
{
	for( int i  = 0 ; i < MAX_DEVILSQUAREMONSTERCOUNT; i++)
	{
		if( m_DevilSquareMonsterInfo[i].m_Type == type )
		{			
			return TRUE;
		}
	}
	return FALSE;
}

// 악마의 광장에 리젠될 몬스터 타입을 얻는다
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
WORD CDevilSquareGround::GetMonsterType(int currtime)
#else
BYTE CDevilSquareGround::GetMonsterType(int currtime)
#endif
{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD	sTemp[MAX_DEVILSQUAREMONSTERCOUNT];
#else
	BYTE	sTemp[MAX_DEVILSQUAREMONSTERCOUNT];
#endif
	int icount = 0;

	for( int i  = 0 ; i < MAX_DEVILSQUAREMONSTERCOUNT; i++)
	{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
		if( m_DevilSquareMonsterInfo[i].m_Type != (WORD)0xffff )
#else
		if( m_DevilSquareMonsterInfo[i].m_Type != 0xff )
#endif
		{			
			if( currtime >= m_DevilSquareMonsterInfo[i].m_RegenStartTime && 
				currtime <= m_DevilSquareMonsterInfo[i].m_RegenEndTime )
			{
				sTemp[icount++] = m_DevilSquareMonsterInfo[i].m_Type;
			}
		}
	}

	if( icount == 0 ) 
		return 0xff;

	return sTemp[rand()%icount];
}

void CDevilSquareGround::ClearScore()
{
	m_DevilSquareRankList.clear();
}

void CDevilSquareGround::SortScore()
{
	sort(m_DevilSquareRankList.begin(), m_DevilSquareRankList.end(),DevilSquareScoreSort); 
}

void CDevilSquareGround::InsertObj(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->Authority != AUTHORITY_ADMIN )
	{	// 운영자는 점수에 포함되지 않는다		

		// 가중치 적용
		if( 
#ifdef DEVILSQUARE_EXTEND_20050221
			lpObj->Class >= 0 && lpObj->Class < 5 && 
			CHECK_DEVILSQUARE_INDEX(lpObj->m_bDevilSquareIndex)
#else
			lpObj->Class >= 0 && lpObj->Class < 4 && 
			lpObj->m_bDevilSquareIndex >= 0 && lpObj->m_bDevilSquareIndex < 4
#endif
			)
		{
			lpObj->m_nEventScore += m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex]/100;
		}
		m_DevilSquareRankList.push_back(lpObj);
	}
}

void CDevilSquareGround::SendScore()
{
	if( m_DevilSquareRankList.size() < 1 )
	{	// 데이터가 없다
		return;
	}

	BYTE count = 1;
	int iUserCount = m_DevilSquareRankList.size();

#ifdef ADD_PCS_MARK_OF_EXP_20070205
	int iExp = 0;
#endif // ADD_PCS_MARK_OF_EXP_20070205
	
	DevilSquareRankList::iterator Itor = m_DevilSquareRankList.begin();
	for( ; Itor != m_DevilSquareRankList.end(); ++Itor)
	{
		memcpy(m_DevilSquareScoreInfoTOP10.Score[count].Name, (*Itor)->Name, 10);		
		
		m_DevilSquareScoreInfoTOP10.Score[count].TotalScore = (*Itor)->m_nEventScore;

		if( iUserCount <= 6 )
		{
#ifdef DEVILSQUARE_EXTEND_20050221
			if( count < 4 )
#else
			if( count < 3 )
#endif
			{
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[count-1][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[count-1][1];
			}
			else
			{
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[3][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[3][1];
			}
		}
		else
		{
			int rate = ((count*100)/iUserCount);
			if( count == 1 )
			{	// 일등 
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[0][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[0][1];
			}
			else if( rate <= 30 )
			{	// 30%내 
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[1][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[1][1];
			}
			else if( rate <= 50 )
			{	// 50%내
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[2][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[2][1];
			}
			else
			{	// 나머지
				m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = m_Bonus[3][0];
				m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = m_Bonus[3][1];
			}
		}
		
		count++;
		if( count >= 10 )
			break;
	}

	m_DevilSquareScoreInfoTOP10.Count = count;

	int	iSize = 3+(sizeof(DevilSquareScoreInfo)*count)+2;
	PHeadSetB((LPBYTE)&m_DevilSquareScoreInfoTOP10, 0x93, iSize);

	count = 1;

	Itor = m_DevilSquareRankList.begin();

	LogAddTD("[DevilSquare] Rank [%d]", m_iIndex);
	for( ; Itor != m_DevilSquareRankList.end(); ++Itor)
	{	
		if( iUserCount <= 6 )
		{
#ifdef DEVILSQUARE_EXTEND_20050221
			if( count < 4 )
#else
			if( count < 3 )
#endif // DEVILSQUARE_EXTEND_20050221
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[count-1][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[count-1][1];
			}
			else
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[3][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[3][1];
			}
		}
		else
		{
			int rate = ((count*100)/iUserCount);
			if( count == 1 )
			{	// 일등 
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[0][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[0][1];
			}
			else if( rate <= 30 )
			{	// 30%내 
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[1][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[1][1];
			}
			else if( rate <= 50 )
			{	// 50%내
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[2][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[2][1];
			}
			else
			{	// 나머지
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_Bonus[3][0];
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_Bonus[3][1];
			}
		}

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
		if (g_bChnPlayTimeLimitOn)
		{
			if( (*Itor)->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
			{
				if ((*Itor)->m_iPlayTimeLimit >= 5*60*60)
				{
					m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iChnPlayTimeLimitLevel2Exp / 100;
					m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iChnPlayTimeLimitLevel2ItemDrop / 100;
				}
				else if ((*Itor)->m_iPlayTimeLimit >= 3*60*60)
				{
					m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iChnPlayTimeLimitLevel1Exp / 100;
					m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iChnPlayTimeLimitLevel1ItemDrop / 100;
				}
			}
		}
	#else
		if (g_bChnPlayTimeLimitOn)
		{
			if ((*Itor)->m_iPlayTimeLimit >= 5*60*60)
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iChnPlayTimeLimitLevel2Exp / 100;
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iChnPlayTimeLimitLevel2ItemDrop / 100;
			}
			else if ((*Itor)->m_iPlayTimeLimit >= 3*60*60)
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iChnPlayTimeLimitLevel1Exp / 100;
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iChnPlayTimeLimitLevel1ItemDrop / 100;
			}
		}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
#endif
		
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 : 데빌스퀘어 보너스 경험치 획득량 조정
		if (g_bVtmPlayTimeLimitOn) 
		{
			if ((*Itor)->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60) 
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iVtmPlayTimeLimitLevel2Exp / 100;
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iVtmPlayTimeLimitLevel2ItemDrop / 100;
			}
			else if ((*Itor)->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
			{
				m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_iVtmPlayTimeLimitLevel1Exp / 100;
				m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen * g_iVtmPlayTimeLimitLevel1ItemDrop / 100;
			}
		}
#endif	

#ifdef MU_CRYWOLF_PENALTY_20051215
		// MVP 실패 : 경험치 획득 감소 
		if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
			&& g_iCrywolfApplyMvpPenalty
		  )
		{
			m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
		}
#endif
		
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef BUGFIX_GAIN_EXP_20071210		
		INT64 iExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;

		#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
		CheckItemOptForGetExpEx( (*Itor), iExp, TRUE );
		#else // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
		//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정		
		#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
		g_PCBangPointSystem.CheckItemOptForGetExp( (*Itor), iExp );
		#else	
			#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		CheckItemOptForGetExp((*Itor), iExp, TRUE);
			#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
		#endif // BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
		//<--
		#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
		
		(*Itor)->Experience += iExp;
	#else
		(*Itor)->Experience += m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
	#endif
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
		if( (*Itor)->m_wExprienceRate == 0 )
		{
			iExp = 0;
		}
		else
		{
			iExp = m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
			(*Itor)->Experience += iExp;
		}
		#else
		if( (*Itor)->m_wExprienceRate > 0 )
		{
			iExp = (int)(m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * ((float)(*Itor)->m_wExprienceRate / 100 ));
			(*Itor)->Experience += iExp;
		}
		#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	#else
		(*Itor)->Experience += m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef BUGFIX_GAIN_EXP_20071210
		bool bSendExp = gObjLevelUp((*Itor), iExp, 0, EVENT_TYPE_DEVILSQUARE );
#else		
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
			bool bSendExp = gObjLevelUp((*Itor), m_DevilSquareScoreInfoTOP10.Score[0].BonusExp, 0, EVENT_TYPE_DEVILSQUARE );
		#else
			bool bSendExp = gObjLevelUp((*Itor), m_DevilSquareScoreInfoTOP10.Score[0].BonusExp);
		#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	#else
		#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
			#ifdef ADD_PCS_MARK_OF_EXP_20070205
			bool bSendExp = false;
			if( (*Itor)->m_wExprienceRate > 0 )
			{
				bSendExp = gObjLevelUp((*Itor), iExp, 0, EVENT_TYPE_DEVILSQUARE );
			}
			#else
			bool bSendExp = gObjLevelUp((*Itor), m_DevilSquareScoreInfoTOP10.Score[0].BonusExp, 0, EVENT_TYPE_DEVILSQUARE );
			#endif // ADD_PCS_MARK_OF_EXP_20070205
		#else
			#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
				#ifdef ADD_PCS_MARK_OF_EXP_20070205
			bool bSendExp = false;
			if( (*Itor)->m_wExprienceRate > 0 )
			{
				bSendExp = gObjLevelUp((*Itor), iExp );
			}
				#else
			bool bSendExp = gObjLevelUp((*Itor), m_DevilSquareScoreInfoTOP10.Score[0].BonusExp);
				#endif // ADD_PCS_MARK_OF_EXP_20070205
			#else
				#ifdef ADD_PCS_MARK_OF_EXP_20070205
			bool bSendExp = false;
			if( (*Itor)->m_wExprienceRate > 0 )
			{
				bSendExp = gObjLevelUp((*Itor), iExp );
			}
				#else
			bool bSendExp = gObjLevelUp((*Itor), m_DevilSquareScoreInfoTOP10.Score[0].BonusExp);
				#endif // ADD_PCS_MARK_OF_EXP_20070205
			#endif
		#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906	
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif	// BUGFIX_GAIN_EXP_20071210

		(*Itor)->m_nEventMoney = m_DevilSquareScoreInfoTOP10.Score[0].BonusZen;
		
#ifdef MODIFY_ZEN_MAX_20040414
		if( !gObjCheckMaxZen( (*Itor)->m_Index, (*Itor)->m_nEventMoney ) )
		{
			int iZEN = MAX_ZEN - (*Itor)->Money;
			(*Itor)->Money += iZEN;
		}
		else
#endif
		(*Itor)->Money += (*Itor)->m_nEventMoney;

		// 돈을 보내고, 
		GCMoneySend((*Itor)->m_Index, (*Itor)->Money);		
		// 경험치를 보내고,
#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
		if (bSendExp == true) {
	//#ifdef ADD_PCS_MARK_OF_EXP_20070205
	#if !defined(MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004) && defined(ADD_PCS_MARK_OF_EXP_20070205)
			if( (*Itor)->m_wExprienceRate > 0 )
			{
				GCKillPlayerExpSend((*Itor)->m_Index, 0xffff, iExp, 0, 0);
			}
	#else
		#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			// 유지의 인장 사용시 보너스 경험치를 클라에 보내는 버그 수정 20081121 (grooving)
			GCKillPlayerExpSend((*Itor)->m_Index, 0xffff, iExp, 0, 0);// 
		#else //MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			GCKillPlayerExpSend((*Itor)->m_Index, 0xffff, m_DevilSquareScoreInfoTOP10.Score[0].BonusExp, 0, 0);
		#endif //MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
	#endif // ADD_PCS_MARK_OF_EXP_20070205
		}
#else
		GCKillPlayerExpSend((*Itor)->m_Index, 0xffff, m_DevilSquareScoreInfoTOP10.Score[0].BonusExp, 0, 0);
#endif


		// 랭킹서버에 정보를 보낸다
		SendRankingInfo((*Itor));

		LogAddTD("Rank :[%d] : [%s][%s][%d][%d][%d]", count, (*Itor)->AccountID, (*Itor)->Name, (*Itor)->m_nEventMoney, (*Itor)->m_nEventExp, (*Itor)->m_nEventScore);
		memcpy(m_DevilSquareScoreInfoTOP10.Score[0].Name, (*Itor)->Name, 10);	
		
		m_DevilSquareScoreInfoTOP10.Score[0].TotalScore = (*Itor)->m_nEventScore;		

//#ifdef ADD_PCS_MARK_OF_EXP_20070205
#if !defined(MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004) && defined(ADD_PCS_MARK_OF_EXP_20070205)
		if( (*Itor)->m_wExprienceRate == 0 )
		{
			m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = 0;
		}
		else
		{
			m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = iExp;
		}
#endif // ADD_PCS_MARK_OF_EXP_20070205

		m_DevilSquareScoreInfoTOP10.MyRank = count++;
		DataSend((*Itor)->m_Index, (LPBYTE)&m_DevilSquareScoreInfoTOP10, iSize);
		
		(*Itor)->m_nEventScore = 0;
		(*Itor)->m_nEventMoney = 0;
		(*Itor)->m_nEventExp = 0;
	}
}

void CDevilSquareGround::SendRankingInfo(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_nEventScore <= 0 )	
		return;

	PMSG_ANS_EVENTUSERSCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x01;
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score	= lpObj->m_nEventScore;
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpObj->Name, MAX_IDSTRING);

//	gUdpSocCRank.SendData((LPBYTE)&pMsg, pMsg.h.size);
	DataSendRank((char*)&pMsg, pMsg.h.size);	
}
