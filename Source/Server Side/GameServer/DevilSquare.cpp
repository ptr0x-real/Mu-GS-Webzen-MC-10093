// DevilSquare.cpp: implementation of the CDevilSquare class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevilSquare.h"

#include <stdio.h>
#include <TIME.H>
#include "process.h"

#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "Include\GMProtocol.h"
#include "Include\Public.h"
#include "Include\cb_protocol.h"
#include "Include\public.h"
#include "Include\Readscript.h"

#include "User.h"
#include "protocol.h"

#include "wsJoinServerCli.h"

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#include "MonsterSetBase.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "wzUdp.h"

#include "MapClass.h"
#include "GameServer.h"

#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
#include "Event.h"
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
#include "CrywolfSync.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

extern wsJoinServerCli wsRServerCli;		//  랭킹서버


extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern MapClass			*MapC;

extern WzUdp			gUdpSocCRank;		// Rank Server

CDevilSquare			g_DevilSquare;

UINT WINAPI	DevilSquareThreadFunc(void* p);

UINT WINAPI	DevilSquareThreadFunc(void* p)
{
	CDevilSquare* pDevilSquare = (CDevilSquare*)p;
	return pDevilSquare->Run();	
}


// 
void DevilSquareEventProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case 0x01 :
		//CBUPR_ItemRequestRecv((LPCB_PMSG_REQ_PRESENT_RESULT)aRecv);
		break;

#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
	case 0x08 :		// 블러드캐슬에 입장 가능한지 (횟수가 남았는지) 확인
		EGBloodCastleEnterCountCheck ((LPPMSG_ANS_BLOODCASTLEENTERCHECK) aRecv);
		break;
#endif

#ifdef BLOODCASTLE_EVENT_3RD_20040401
#ifdef BLOODCASTLE_CHECK_ENTERCOUNT_20040304
	case 0x0B :		// 블러드캐슬 입장 가능횟수를 얻어온다.
		EGAnsBloodCastleEnterCount ((LPPMSG_ANS_BLOODCASTLE_ENTERCOUNT) aRecv);
		break;
#endif
#endif
		
#ifdef DEVILSQUARE_CHECK_ENTERCOUNT_20040715
	case 0x0F :		// 데빌스퀘어 입장 가능횟수를 얻어온다.
		EGAnsDevilSquareEnterCount((LPPMSG_ANS_DEVILSQUARE_ENTERCOUNT) aRecv);
		break;
	case 0x10 :		// 데빌스퀘어에 입장 가능한지 (횟수가 남았는지) 확인
		EGAnsDevilSquareEnterCountCheck((LPPMSG_ANS_DEVILSQUAREENTERCHECK) aRecv);
		break;
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 랭킹서버로 부터 온 데이타 처리
	case 0x12:
		g_IllusionTempleEvent.EGAnsIllusionTempleEnterCountCheck((LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK) aRecv);
		break;
#endif
		
#ifdef ADD_SEASON_3_NEW_UI_20071122	
	case 0x14:
		g_IllusionTempleEvent.EGAnsIllusionTempleEnterCount((LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT) aRecv);
		break;
#endif
	}
}

void	DataSendRank(char* pMsg, int size)
{
	if( !IsDevilSquareEventConnected && !DevilSquareEventConnect )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);
		if( GMRankingServerConnect(gDevilSquareEventServerIp, WM_ASS_RANKINGCLIMSG) == FALSE )
		{
			IsDevilSquareEventConnected = 0;
			LogAddTD("Can not connect Ranking Server");
			return;
		}	
		IsDevilSquareEventConnected = 1;
	}

	if( !DevilSquareEventConnect && IsDevilSquareEventConnected )
		wsRServerCli.DataSend(pMsg, size);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDevilSquare::CDevilSquare()
{
	m_bQuit = FALSE;
	m_iCloseTime = 2;
	m_iOpenTime = 2;
	m_iPlaytime = 10;

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

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// 추가 보너스 테이블
	m_BonusScoreTable[CLASS_WIZARD][6]		= 170;
	m_BonusScoreTable[CLASS_KNIGHT][6]		= 400;
	m_BonusScoreTable[CLASS_ELF][6]			= 200;
	m_BonusScoreTable[CLASS_MAGUMSA][6]		= 0;
	m_BonusScoreTable[CLASS_DARKLORD][6]	= 0;
#endif
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	for( int i=0; i<MAX_DEVILSQUARE; ++i )
		m_fRewardExpRate[i] = 1.0f;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

}

CDevilSquare::~CDevilSquare()
{
	m_bQuit = TRUE;
}

void CDevilSquare::Init()
{	
	m_bQuit = TRUE;
	Sleep(500);
	SetState(DevilSquare_CLOSE);

	m_bQuit = FALSE;

	UINT	dwThreadId;	
	m_hThread = _beginthreadex(NULL, NULL, DevilSquareThreadFunc, this, NULL, &dwThreadId);
}

void CDevilSquare::Load(char* filename)
{
	for( int i = 0; i < MAX_DEVILSQUARE; i++ )
	{
		m_DevilSquareGround[i].Init(i);
	}

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("[DevilSquare] Info file Load Fail [%s]", filename);
		return ;
	}
	SMDToken Token;
	
	int  number, type;
	int	 monstertype;
	int	 starttime;
	int  endtime;

	int  x, y, tx, ty;	
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// 악마의 광장 게임시간 설정
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iCloseTime = (int)TokenNumber;
				Token = (*GetToken)(); m_iOpenTime = (int)TokenNumber;
				Token = (*GetToken)(); m_iPlaytime = (int)TokenNumber;
			}
			else if( type == 1 )
			{	// 기본 몬스터 리젠 설정				
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				number = (int)TokenNumber;			
				Token = (*GetToken)(); monstertype		= (int)TokenNumber;
				Token = (*GetToken)(); starttime		= (int)TokenNumber;			
				Token = (*GetToken)(); endtime			= (int)TokenNumber;
				
				m_DevilSquareGround[number].Set(monstertype, starttime, endtime);		// 저주받은왕
			}				
			else if( type == 2 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				number = (int)TokenNumber;			
				Token = (*GetToken)(); monstertype		= (int)TokenNumber;
				Token = (*GetToken)(); starttime		= (int)TokenNumber;
				
				Token = (*GetToken)(); x		= (int)TokenNumber;
				Token = (*GetToken)(); y		= (int)TokenNumber;
				Token = (*GetToken)(); tx		= (int)TokenNumber;
				Token = (*GetToken)(); ty		= (int)TokenNumber;

				m_DevilSquareGround[number].SetBoss(monstertype, starttime, x, y, tx, ty);
			}
			else if( type == 3 )
			{
				int index, rank, zen, exp;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				index = (int)TokenNumber;			
				Token = (*GetToken)(); rank		= (int)TokenNumber;
				Token = (*GetToken)(); exp		= (int)TokenNumber;
				Token = (*GetToken)(); zen		= (int)TokenNumber;
				m_DevilSquareGround[index].SetBonus(rank, exp, zen);
			}
#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511	// 파일 로드시 시작 시간 읽음
			else if( type == 4)
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
			
				DEVIL_SQARE_STARTTIME startTime;
		
				startTime.nHour = (int)TokenNumber;
				Token = (*GetToken)(); startTime.nMin = (int)TokenNumber;

				m_lstStartTime.push_back(startTime);			
			}
#endif //UPDATE_DEVIL_SQUARE_START_TIME_20070511
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			else if( type == 5 )
			{	// 악마의 광장 보상 경험치 비율
				Token = (*GetToken)();
				if( strcmp("end", TokenString) == NULL )
				{
					// 읽어온 비율을 적용 시킨다.
					for( INT i=0; i<MAX_DEVILSQUARE; ++i )
						m_DevilSquareGround[i].ApplyBonusRate( m_fRewardExpRate[i] );
					break;
				}

				INT iDSquare = (int)TokenNumber;				
				if( CHECK_LIMIT(iDSquare, MAX_DEVILSQUARE) ) {
					Token = (*GetToken)();
					m_fRewardExpRate[iDSquare] = (FLOAT)TokenNumber;						
				}
				else Token = (*GetToken)();				
			}
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
		}
	}
	fclose(SMDFile);

	LogAdd("%s file load!", filename);

// grooving 사내 테스트 악마의 광장 시간 조정
#if TESTSERVER == 1 && defined(UPDATE_DEVIL_SQUARE_START_TIME_20070511)

	m_iCloseTime = 2;
	m_iOpenTime = 1;
	m_iPlaytime = 5;

	DEVIL_SQARE_STARTTIME startTime;

	m_lstStartTime.clear();

	for( int i = 0; i < 24; i++)
	{
		startTime.nHour = i;
		
		for( int j = 0; j < 60; j+= 6 )
		{
			startTime.nMin = j;
			
			m_lstStartTime.push_back(startTime);
		}
	}			
#endif//TESTSERVER
}
void CDevilSquare::SetState(eDevilSquareState eState)
{
	m_eState = eState;

	switch(m_eState)
	{
		case DevilSquare_CLOSE:
			{
				SetClose();
			}
		break;

		case DevilSquare_OPEN:
			{
				SetOpen();
			}
		break;

		case DevilSquare_PLAYING:
			{
				SetPlaying();
			}
		break;
	}
}

void CDevilSquare::SetClose()
{
	if( LogAdd )
	{
		LogAddTD("[DevilSquare] Close");
	}

	// 몬스터 정리
	ClearMonstr();
	// 등수 계산 (등수 계산 하면서, 보너스 경험치 젠 정보를 같이 전송해준다)
	CalcScore();	

	for( int i = 0; i < MAX_DEVILSQUARE; i++ )
	{
		m_DevilSquareGround[i].Clear();
	}

	m_iRemainTime = m_iCloseTime;

#if TESTSERVER == 0
	CheckSync();
#endif


	m_bFlag = FALSE;

	if( m_iRemainTime <= 1 )
	{
		m_iTime = GetTickCount();
		m_iremainTimeSec = 60;
	}
	else
	{
		m_iTime = GetTickCount()+DEVILSQUARE_MIN;
		m_iremainTimeSec = -1;
	}
	m_bSendTimeCount = FALSE;
}

void CDevilSquare::SetOpen()
{
	m_iRemainTime = m_iOpenTime;
	
	if( m_iOpenTime <= 1 )
	{
		m_iTime = GetTickCount();
		m_iremainTimeSec = 60;
	}
	else
	{
		m_iTime = GetTickCount()+DEVILSQUARE_MIN;
		m_iremainTimeSec = -1;
	}

	m_bSendTimeCount = FALSE;
}

void CDevilSquare::SetPlaying()
{
	gObjScoreClear();
	SetMonster();

	m_iRemainTime = m_iPlaytime;	

	if( m_iPlaytime <= 1 )
	{
		m_iTime = GetTickCount();
		m_iremainTimeSec = 60;
	}
	else
	{
		m_iTime = GetTickCount()+DEVILSQUARE_MIN;
		m_iremainTimeSec = -1;
	}

	m_bSendTimeCount = FALSE;
}

#ifdef DEVILSQUARE_EXTEND_20050221			
INT	CDevilSquare::GetDevilSquareIndex(int iGateNumber)
{
	switch(iGateNumber) {
	case 58 :		// 1광장
	case 59 :		// 2광장
	case 60 :		// 3광장
	case 61 :		// 4광장
		return iGateNumber - 58;
		break;
	case 111 :		// 5광장
	case 112 :		// 6광장
		return (iGateNumber - 111) + 4;
		break;
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// 7광장 게이트번호 얻기
	case 270 :		// 7광장
		return 6;
		break;
#endif
	default :
		return -1;
		break;
	}
	return -1;
}

#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010		// GetUserLevelToEnter(...) 수정
INT	CDevilSquare::GetUserLevelToEnter ( INT iUserIndex, INT & btMoveGate )			// 사용자가 어떤 광장에 들어갈 수 있는지 레벨을 반환한다.
{
	btMoveGate		= 0xFF;

	if (!gObjIsConnected(iUserIndex))
		return -1;

	INT iENTER_LEVEL = -1;

	// 마스터레벨 유저는 7광장
	if( g_MasterLevelSystem.IsMasterLevelUser( &gObj[iUserIndex] ) )
	{
		iENTER_LEVEL	= MAX_DEVILSQUARE - 1;
		btMoveGate		= g_sttDEVILSQUARE_LEVEL[iENTER_LEVEL].iGATE_NUM;
	}
	else
	{
		for (INT i = 0 ; i < MAX_DEVILSQUARE -1; i++) 
		{
			if( gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA ) 
			{
				if( (gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].iLOWER_BOUND_EXCHAR) &&
					(gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].iUPPER_BOUND_EXCHAR)
					)
				{
					iENTER_LEVEL	= i;
					btMoveGate		= g_sttDEVILSQUARE_LEVEL[i].iGATE_NUM;
					break;
				}
			}
			else 
			{
				if ((gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].iLOWER_BOUND) &&
					(gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].iUPPER_BOUND)
					)
				{
					iENTER_LEVEL	= i;
					btMoveGate		= g_sttDEVILSQUARE_LEVEL[i].iGATE_NUM;
					break;
				}
			}
		}
	}

	return iENTER_LEVEL;
}
#else	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010
INT	CDevilSquare::GetUserLevelToEnter (INT iUserIndex, BYTE & btMoveGate)			// 사용자가 어떤 광장에 들어갈 수 있는지 레벨을 반환한다.
{
	btMoveGate		= 0xFF;

	if (!gObjIsConnected(iUserIndex))
		return -1;

	INT iENTER_LEVEL = -1;

	for (INT i = 0 ; i < MAX_DEVILSQUARE ; i++) {

#ifdef DARKLORD_WORK
		if (gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA) {
#else
		if(gObj[iUserIndex].Class == CLASS_MAGUMSA ) {
#endif

			if ((gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].iLOWER_BOUND_EXCHAR) &&
				(gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].iUPPER_BOUND_EXCHAR)
				)
			{
				iENTER_LEVEL	= i;
				btMoveGate		= g_sttDEVILSQUARE_LEVEL[i].iGATE_NUM;
				break;
			}
		}
		else {
			if ((gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].iLOWER_BOUND) &&
				(gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].iUPPER_BOUND)
				)
			{
				iENTER_LEVEL = i;
				btMoveGate		= g_sttDEVILSQUARE_LEVEL[i].iGATE_NUM;
				break;
			}
		}
	}

	return iENTER_LEVEL;
}
#endif	// ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	
#endif

BOOL CDevilSquare::Run()
{
	int count = 0;
	while(!m_bQuit)
	{
		if( gDevilSquareEvent )
		{
			switch(m_eState)
			{
				case DevilSquare_CLOSE:
					{
						ProcClose();
					}
				break;

				case DevilSquare_OPEN:
					{
						ProcOpen();
					}
				break;

				case DevilSquare_PLAYING:
					{
						ProcPlaying();
					}
				break;
			}		
		}
		Sleep(100);

		count++;
		if( count > 10*60 )
		{
#ifdef DELETE_DEVILSQURE_MESSAGE_TO_RANKINGSERVER_20070223
			SendLiveCall();
#endif
			count = 0;

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[12] != AUTH_CHECK_12 )
		DestroyGIocp();
#endif

		}
	}

	return TRUE;
}

#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511	// 파일 로드시 시작 시간 읽음

void CDevilSquare::CheckSync()
{
	list<DEVIL_SQARE_STARTTIME>::iterator iter;
	DEVIL_SQARE_STARTTIME	prevStartTime;
	DEVIL_SQARE_STARTTIME	nextStartTime;

	DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
	DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
	DWORD	dwCurTime	= 0;;		// 현재 시간(분)
	
	struct tm *today;
	time_t ltime;

	if(m_lstStartTime.size() == 0)
	{
		return;
	}
	
	time( &ltime );
	today = localtime( &ltime );
	dwCurTime	= today->tm_hour * 60 + today->tm_min;

	// 다음 이벤트 열리 시간을 검색
	prevStartTime = *m_lstStartTime.begin();
	for(iter = m_lstStartTime.begin(); iter != m_lstStartTime.end(); ++iter)
	{
		nextStartTime = *iter;

		// 시작시간을 분으로 계산
		dwPrevTime	= prevStartTime.nHour * 60 + prevStartTime.nMin;
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		if(dwPrevTime == dwNextTime)
		{
			if(dwCurTime < dwNextTime)
			{
				nextStartTime = *iter;
				break;
			}
			else
			{
				continue;
			}
		}
		
		if(dwCurTime >= dwPrevTime && dwCurTime < dwNextTime)	
		{
			break;			
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;
	while(Cnt--)
	{
		if(iter == m_lstStartTime.end())
		{
			// 다음 날 시간
			iter = m_lstStartTime.begin();
			nextStartTime = *iter;
		}
		
		// 다음 시작시간을 다시 계산
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		// 다음 시작까지 남은 시간 계산
		if(today->tm_hour <= nextStartTime.nHour)
		{
			// 오늘
			m_iRemainTime = dwNextTime - dwCurTime;
		}
		else
		{
			// 다음날
			m_iRemainTime = 24 * 60 - dwCurTime + dwNextTime;
		}

		// 현재 시작까지 남은 시간(m_iRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
		if( m_iRemainTime <= m_iOpenTime)
		{
			iter++;
			if(iter != m_lstStartTime.end())
			{
				nextStartTime = *iter;
			}
		}
		else
		{
			m_iRemainTime -= m_iOpenTime;
			break;
		}
	}

	LogAddTD("[DevilSquare] Sync Open Time. [%d]min remain", m_iRemainTime);
}

// 진행중일 때 다음 열릴때까지 시간
int CDevilSquare::GetNextRemainTime()
{
	list<DEVIL_SQARE_STARTTIME>::iterator iter;
	DEVIL_SQARE_STARTTIME	prevStartTime;
	DEVIL_SQARE_STARTTIME	nextStartTime;

	DWORD	dwPrevTime	= 0;;		// 이전 시작 시간(분)
	DWORD	dwNextTime	= 0;;		// 다음 시작 시간(분)
	DWORD	dwCurTime	= 0;;		// 현재 시간(분)
	
	struct tm *today;
	time_t ltime;

	int nNextRemainTime = 0;

	if(m_lstStartTime.size() == 0)
	{
		return 0;
	}
	
	time( &ltime );
	today = localtime( &ltime );
	dwCurTime	= today->tm_hour * 60 + today->tm_min;

	// 다음 이벤트 열리 시간을 검색
	prevStartTime = *m_lstStartTime.begin();
	for(iter = m_lstStartTime.begin(); iter != m_lstStartTime.end(); ++iter)
	{
		nextStartTime = *iter;

		// 시작시간을 분으로 계산
		dwPrevTime	= prevStartTime.nHour * 60 + prevStartTime.nMin;
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		if(dwPrevTime == dwNextTime)
		{
			if(dwCurTime < dwNextTime)
			{
				nextStartTime = *iter;
				break;
			}
			else
			{
				continue;
			}
		}
		
		if(dwCurTime >= dwPrevTime && dwCurTime < dwNextTime)	
		{
			break;			
		}

		prevStartTime = *iter;
	}

	int Cnt = 2;
	while(Cnt--)
	{
		if(iter == m_lstStartTime.end())
		{
			// 다음 날 시간
			iter = m_lstStartTime.begin();
			nextStartTime = *iter;
		}
		
		// 다음 시작시간을 다시 계산
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		// 다음 시작까지 남은 시간 계산
		if(today->tm_hour <= nextStartTime.nHour)
		{
			// 오늘
			nNextRemainTime = dwNextTime - dwCurTime;
		}
		else
		{
			// 다음날
			nNextRemainTime = 24 * 60 - dwCurTime + dwNextTime;
		}

		// 현재 시작까지 남은 시간(nNextRemainTime)이 오픈시간(m_iOpenTime)보다 작으면 다음 시간으로 넘긴다
		if( nNextRemainTime <= m_iOpenTime)
		{
			iter++;
			if(iter != m_lstStartTime.end())
			{
				nextStartTime = *iter;
			}
		}
		else
		{
			nNextRemainTime -= m_iOpenTime;
			break;
		}
	}

	return nNextRemainTime;
}

#else	// UPDATE_DEVIL_SQUARE_START_TIME_20070511

void CDevilSquare::CheckSync()
{
#if TESTSERVER == 1
	m_iRemainTime = 1;
	return;
#endif

	struct tm *today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );

#ifdef FOR_JAPAN
	if( (today->tm_hour%2) == 0 )
#else
	if( (today->tm_hour%2) == 1 )
#endif
	{
		m_iRemainTime = 120 - today->tm_min - m_iOpenTime;		
	}
	else
	{
		if( today->tm_min >= 55 )
		{
			m_iRemainTime = 60 - today->tm_min - m_iOpenTime;
			m_iRemainTime += 120;
		}
		else
		{
			m_iRemainTime = 60 - today->tm_min - m_iOpenTime;
		}
	}

	if( m_iRemainTime < 0 )
	{
		m_iRemainTime += 60;
	}

	LogAddTD("[DevilSquare] Sync Open Time. [%d]min remain", m_iRemainTime);
}

#endif	// UPDATE_DEVIL_SQUARE_START_TIME_20070511

// 이벤트는 매 시간 정각에 실행된다
// 이벤트가 종료된 시점에서는 매시간 55분에
// 이벤트에 입장 가능하게 해 준다
void CDevilSquare::ProcClose()
{		
	if( m_iremainTimeSec != -1 )
	{	// 1분대 부터 초단위 카운트

		int	iTime = (GetTickCount()-m_iTime)/1000;

		if( iTime != 0 )
		{
			m_iremainTimeSec -= iTime;
			m_iTime += iTime*1000;

			if( m_iremainTimeSec <= 30 && !m_bSendTimeCount )
			{	// 30초에 카운트 명령 보낸다
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
				pMsg.Type = 0;
#ifdef FOR_BLOODCASTLE
				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && (!CHECK_BLOODCASTLE(gObj[n].MapNumber))
#ifdef CHAOSCASTLE_SYSTEM_20040408
						&& (!CHECK_CHAOSCASTLE(gObj[n].MapNumber))
#endif
						)
					{
						DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
#else
				AllSendMsg((LPBYTE)&pMsg, sizeof( pMsg ));
#endif				
				m_bSendTimeCount = TRUE;
			}			
			
			if( m_iremainTimeSec < 1 )
			{	// 상태 전환
#ifdef FOR_BLOODCASTLE
				PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsg( &pNotice, 0, lMsg.Get(703));
#else
				pNotice.type		= 0;
				strcpy(pNotice.Notice, lMsg.Get(703));	//"[악마의 광장] 문이 열렸습니다"
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && (!CHECK_BLOODCASTLE(gObj[n].MapNumber))
#ifdef CHAOSCASTLE_SYSTEM_20040408
						&& (!CHECK_CHAOSCASTLE(gObj[n].MapNumber))
#endif
						)
					{
						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
				LogAddTD(pNotice.Notice);
#else
				char msg[255];
				wsprintf(msg, lMsg.Get(703));	//"[악마의 광장] 문이 열렸습니다"
				AllSendServerMsg(msg);
				LogAddTD(msg);
#endif				
				SetState(DevilSquare_OPEN);
			}			
		}
	}
	else if( m_iTime < GetTickCount() )
	{	
		ClearMonstr();
		m_iRemainTime--;

		if( !m_bFlag )
		{	// 광장을 빠져나가기까지 일분의 시간을 준다
			for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
			{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
				if( gObj[n].Connected == 3 && CHECK_DEVILSQUARE(gObj[n].MapNumber) && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#else
				if( gObj[n].Connected == 3 && gObj[n].MapNumber == 9  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#endif
				{					
					gObjMoveGate(n, 27);
				}
			}
			ItemClear();
		}
		else
		{
			CheckInvalidUser();
		}

		if( m_iRemainTime <= 15 )
		{
			if( !(m_iRemainTime % 5) )
			{
#ifdef FOR_BLOODCASTLE
				PMSG_NOTICE	pNotice;
					
#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(704), m_iRemainTime);
#else
				pNotice.type		= 0;
				wsprintf(pNotice.Notice, lMsg.Get(704), m_iRemainTime);	//[악마의 광장] 문이 열리기 %d분전 
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && (!CHECK_BLOODCASTLE(gObj[n].MapNumber)) 
#ifdef CHAOSCASTLE_SYSTEM_20040408
						&& (!CHECK_CHAOSCASTLE(gObj[n].MapNumber))
#endif
						)
					{
						DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
				LogAddTD(pNotice.Notice);
#else
				char msg[255];
				wsprintf(msg, lMsg.Get(703));	//"[악마의 광장] 문이 열렸습니다"
				AllSendServerMsg(msg);
				LogAddTD(msg);
#endif	
			}
		}
		
		if( m_iRemainTime == 1 && m_iremainTimeSec == -1 )
		{
			m_iremainTimeSec = 60;
			m_iTime = GetTickCount();
		}			
		else
		{
			m_iTime = GetTickCount()+DEVILSQUARE_MIN;
		}
	}		
}

void CDevilSquare::ProcOpen()
{		
	if( m_iremainTimeSec != -1 )
	{	// 1분대 부터 초단위 카운트
		int	iTime = (GetTickCount()-m_iTime)/1000;

		if( iTime != 0 )
		{
			m_iremainTimeSec -= iTime;
			m_iTime += iTime*1000;
			
			if( m_iremainTimeSec <= 30 && !m_bSendTimeCount )
			{	// 30초에 카운트 명령 보낸다
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
				pMsg.Type = 1;
#ifdef FOR_BLOODCASTLE
				for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && (!CHECK_BLOODCASTLE(gObj[n].MapNumber)) 
#ifdef CHAOSCASTLE_SYSTEM_20040408
						&& (!CHECK_CHAOSCASTLE(gObj[n].MapNumber))
#endif
						)
					{
						DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
#else
				AllSendMsg((LPBYTE)&pMsg, sizeof( pMsg ));
#endif				

				m_bSendTimeCount = TRUE;
			}			
			
			if( m_iremainTimeSec < 1 )
			{	// 상태 전환
				SendEventStartMsg();
				SetState(DevilSquare_PLAYING);
			}			
		}
	}
	else if( m_iTime < GetTickCount() )
	{	
		ClearMonstr();
		m_iRemainTime--;
		if( m_iRemainTime > 0 )
		{

#ifdef FOR_BLOODCASTLE
			PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(705), m_iRemainTime);
#else
			pNotice.type		= 0;
			wsprintf(pNotice.Notice, lMsg.Get(705), m_iRemainTime);	//%d분후 [악마의 광장] 이벤트가 시작 됩니다
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

			for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
			{
				if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && (!CHECK_BLOODCASTLE(gObj[n].MapNumber))
#ifdef CHAOSCASTLE_SYSTEM_20040408
						&& (!CHECK_CHAOSCASTLE(gObj[n].MapNumber))
#endif
					)
				{
					DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
				}
			}

			LogAddTD(pNotice.Notice);
#else
			char msg[255];
			wsprintf(msg, lMsg.Get(705), m_iRemainTime);			//%d분후 [악마의 광장] 이벤트가 시작 됩니다
			AllSendServerMsg(msg);

			LogAddTD(msg);
#endif
			m_iTime = GetTickCount();
		}		
		
		if( m_iRemainTime == 1 && m_iremainTimeSec == -1 )
		{
			m_iremainTimeSec = 60;
			m_iTime = GetTickCount();
		}			
		else
		{
			m_iTime = GetTickCount()+DEVILSQUARE_MIN;
		}
	}		
}

void CDevilSquare::ProcPlaying()
{
	if( m_iremainTimeSec != -1 )
	{	// 1분대 부터 초단위 카운트
		int	iTime = (GetTickCount()-m_iTime)/1000;

		if( iTime != 0 )
		{
			m_iremainTimeSec -= iTime;
			m_iTime += iTime*1000;

			if( m_iremainTimeSec <= 30 && !m_bSendTimeCount )
			{
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
				pMsg.Type = 2;
#ifdef MODIFY_DEVILSQUARE_CLOSE_MSG_20080717
				//맵 안에 유저만 보낸다. riverstyx
				for( int n=0; n<MAX_OBJECT; n++)
				{
					if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) && 
						CHECK_DEVILSQUARE(gObj[n].MapNumber) && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
					{
						DataSend(n, (LPBYTE)&pMsg, sizeof( pMsg ));
					}
				}
#else
				AllSendSameMapMsg((LPBYTE)&pMsg, sizeof( pMsg ), 9);
#endif // MODIFY_DEVILSQUARE_CLOSE_MSG_20080717				
				m_bSendTimeCount = TRUE;
			}
			
			if( m_iremainTimeSec < 1 )
			{	// 상태 전환
				//AllSendServerMsg("[악마의 광장]이 닫혔습니다");				
				SetState(DevilSquare_CLOSE);
			}
		}
	}
	else if( m_iTime < GetTickCount() )
	{
		m_iRemainTime--;
		if( m_iRemainTime > 0 )
		{
			if( !(m_iRemainTime % 5) )
			{
				PMSG_NOTICE	pMsg;

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pMsg, 0, lMsg.Get(706), m_iRemainTime);
				LogAddTD(pMsg.Notice);
#else
				wsprintf(pMsg.Notice, lMsg.Get(706), m_iRemainTime);	// %d분후 [악마의 광장] 문이 닫힙니다
				LogAddTD(pMsg.Notice);
				
				PHeadSetB((LPBYTE)&pMsg, 0x0D, 4+(strlen(pMsg.Notice)+1));
				pMsg.type		= 0;
#endif

				AllSendSameMapMsg((LPBYTE)&pMsg, pMsg.h.size, 9);
			}

			for( int i = 0; i < MAX_DEVILSQUARE; i++ )
			{	// 보스 몬스터 리젠을 설정한다
				m_DevilSquareGround[i].RegenBossMonster(m_iPlaytime-m_iRemainTime);
			}

			if( m_iRemainTime == 1 && m_iremainTimeSec == -1 )
			{
				m_iremainTimeSec = 60;
				m_iTime = GetTickCount();
			}			
			else
			{
				m_iTime = GetTickCount()+DEVILSQUARE_MIN;
			}
		}
	}
}

void CDevilSquare::SetMonster()
{
	int result;	
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(gMSetBase.m_Mp[n].m_MapNumber) )
#else
		if( gMSetBase.m_Mp[n].m_MapNumber == 9 )
#endif
		{
#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
			WORD monstertype = gMSetBase.m_Mp[n].m_Type;
#else
			BYTE monstertype = gMSetBase.m_Mp[n].m_Type;
#endif

			gMSetBase.m_Mp[n].m_X;
			gMSetBase.m_Mp[n].m_Y;

			BYTE devilSquareIndex = 0xff;

			if( gMSetBase.m_Mp[n].m_X == 119 )
			{
				devilSquareIndex = 0;
			}
			else if( gMSetBase.m_Mp[n].m_X == 121 )
			{
				devilSquareIndex = 1;
			}
			else if( gMSetBase.m_Mp[n].m_X == 49 )
			{
				devilSquareIndex = 2;
			}
			else if( gMSetBase.m_Mp[n].m_X == 53 )
			{
				devilSquareIndex = 3;
			}
#ifdef DEVILSQUARE_EXTEND_20050221		// 광장 일반몬스터 배치 (시작 X 중요)
			else if( gMSetBase.m_Mp[n].m_X == 120 )
			{
				devilSquareIndex = 4;
			}
			else if( gMSetBase.m_Mp[n].m_X == 122 )
			{
				devilSquareIndex = 5;
			}
#endif
#ifdef ADD_DEVILSQURE_FOR_MASTER_LEVEL_20071010	// 광장7 일반몬스터 배치
			else if( gMSetBase.m_Mp[n].m_X == 50 )
			{
				devilSquareIndex = 6;
			}
#endif

			if( devilSquareIndex == 0xff )
			{	// 악마의 광장에 들어가는 몬스터가 아니다				
				LogAddTD("[DevilSquare] [%d] Invalid MonterType", monstertype);
				continue;
			}

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// 맵서버 관련 몬스터, NPC 추가 (서버가 가지고 있는 맵만 추가가 가능하다.)
			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
#else
			result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObjSetPosMonster(result, n);
				gObjSetMonster(result, monstertype);

				gObj[result].MaxRegenTime = 1000;
				gObj[result].m_bDevilSquareIndex = devilSquareIndex;
			}
		}
	}
}

void CDevilSquare::ClearMonstr()
{
	for( int n=0; n<MAX_MONSTER; n++)
	{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(gObj[n].MapNumber) )
#else
		if( gObj[n].MapNumber == 9 )
#endif
		{
			gObjDel(n);
		}
	}
}

void CDevilSquare::gDevilSquareMonsterRegen(LPOBJECTSTRUCT lpObj)
{
	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;	
	if( devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE )
	{		
		LogAddTD("[DevilSquare] Invalid DevilSquareIndex [%d]", devilsquareindex);
		return;
	}

	if( lpObj->m_PosNum == -1 )
	{	// 보스 몬스터는 한번죽으면 다시 리젠되지 않는다
		gObjDel(lpObj->m_Index);
		return;
	}

#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
	WORD monstertype = m_DevilSquareGround[devilsquareindex].GetMonsterType(m_iPlaytime-m_iRemainTime);
#else
	BYTE monstertype = m_DevilSquareGround[devilsquareindex].GetMonsterType(m_iPlaytime-m_iRemainTime);
#endif
	if( monstertype == 0xff )
	{		
		LogAddTD("[DevilSquare] [%d] Invalid MonterType", monstertype);
		return;
	}	
	
	for( int n=0; n<MAX_MAGIC; n++)
	{
		lpObj->Magic[n].Clear();		
	}
	
	gObjSetMonster(lpObj->m_Index, monstertype);
	lpObj->DieRegen	= 0;
	gObjMonsterRegen(lpObj);
	CreateFrustrum(lpObj->X,lpObj->Y, lpObj->m_Index);

	lpObj->m_bDevilSquareIndex = devilsquareindex;	
	lpObj->MaxRegenTime = 1000;	
	
	LogAddTD("[DevilSquare] Monter Regen [%d][%d][%d,%d]", monstertype, devilsquareindex, lpObj->X,lpObj->Y);
}

void CDevilSquare::SendEventStartMsg()
{
	PMSG_NOTICE	pToEventer;
	PMSG_NOTICE	pWithOutEvneter;
	
#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pToEventer, 0, lMsg.Get(707) );
	TNotice::MakeNoticeMsg( &pWithOutEvneter, 0, lMsg.Get(708) );
#else
	pToEventer.type		= 0;	
	strcpy(pToEventer.Notice, lMsg.Get(707));	// [악마의 광장]이벤트가 시작되었습니다
	PHeadSetB((LPBYTE)&pToEventer, 0x0D, 4+(strlen(pToEventer.Notice)+1));

	pWithOutEvneter.type		= 0;
	strcpy(pWithOutEvneter.Notice, lMsg.Get(708));	//[악마의 광장] 문이 닫혔습니다. 늦으신 분들은 다음기회를 이용해주세요
	PHeadSetB((LPBYTE)&pWithOutEvneter, 0x0D, 4+(strlen(pWithOutEvneter.Notice)+1));
#endif

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 )
		{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
			if( CHECK_DEVILSQUARE(gObj[n].MapNumber) && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#else
			if( gObj[n].MapNumber == 9 && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#endif
			{
				DataSend(n, (LPBYTE)&pToEventer, pToEventer.h.size);
			}
			else
			{
				DataSend(n, (LPBYTE)&pWithOutEvneter, pWithOutEvneter.h.size);
			}
		}
	}

	LogAddTD("[DevilSquare] Start Event");
}


// 데빌 스퀘어에서 죽었을때 처리를 해준다
void CDevilSquare::DieProcDevilSquare(LPOBJECTSTRUCT lpObj)
{
	char msg[255];

	wsprintf(msg, lMsg.Get(709), lpObj->m_nEventScore);	//실패하셨군요. 사냥점수는%d점이며 다음 기회에 다시 도전해보시기 바랍니다
	GCServerMsgStringSend(msg, lpObj->m_Index, 1);	

	if( lpObj->m_nEventScore <= 0 )
		return;

	PMSG_ANS_EVENTUSERSCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x01;
	pMsg.h.size		= sizeof( pMsg );
	
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;
	
	if( lpObj->Class == 1 )
	{	// 가중치 적용
		if( 
#ifdef DEVILSQUARE_EXTEND_20050221
			lpObj->Class >= 0 && lpObj->Class < 5 && 
			CHECK_DEVILSQUARE_INDEX(lpObj->m_bDevilSquareIndex) && lpObj->m_stateEngagePVP != PVP_USER_PLAYING
#else
			lpObj->Class >= 0 && lpObj->Class < 4 && 
			lpObj->m_bDevilSquareIndex >= 0 && lpObj->m_bDevilSquareIndex < 4 && lpObj->m_stateEngagePVP != PVP_USER_PLAYING
#endif
			)
		{
			lpObj->m_nEventScore += m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex]/100;
		}
		pMsg.Score = lpObj->m_nEventScore;
	}
	else
	{
		pMsg.Score	= lpObj->m_nEventScore;
	}

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpObj->Name, MAX_IDSTRING);

	DataSendRank((char*)&pMsg, pMsg.h.size);
//	gUdpSocCRank.SendData((LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD("[DevilSquare] Dead [%s][%s][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->m_nEventExp, lpObj->m_nEventScore);

	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_nEventExp = 0;	
}


int CDevilSquare::gObjMonsterExpSingle(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int dmg, int tot_dmg)
{
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	INT64 exp, maxexp=0;
#else
	int exp, maxexp=0;
#endif

#ifdef MODIFY_GETTING_EXP_RATE_20060214
	int level = ((lpTargetObj->Level+25)*lpTargetObj->Level)/3;
#else
#ifdef MODIFY_GETTING_EXP_RATE_20051116
	int level = ((lpTargetObj->Level+20)*lpTargetObj->Level)/4;
#else
	int level = ((lpTargetObj->Level+10)*lpTargetObj->Level)/4;	
#endif
#endif // MODIFY_GETTING_EXP_RATE_20060214
	
	// 자신의 레벨보다 10 작으면 깎이기 시작해서..
	if( (lpTargetObj->Level+10) < lpObj->Level )
	{
		level =  level * (lpTargetObj->Level+10) / lpObj->Level;
	}
	
#ifdef EXP_CAL_CHANGE	
	if( lpTargetObj->Level >= 65 )	
	{	// 몬스터 레벨이 50이상이면

	#ifdef MODIFY_GETTING_EXP_RATE_20051116
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level/3);
	#else
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	#endif

	}
#endif

	if( level > 0 )
	{
		maxexp = level/2;
	}
	else level = 0;

	if( maxexp < 1 ) {
		exp = level;
	}
	else
	{
		exp = level+(rand()%maxexp);		
	}
	
	exp = (dmg*exp)/tot_dmg;	

	DWORD mymaxexp = gLevelExperience[lpObj->Level];
	// 자신의 경험치 최대값보다 크면..
	if( exp > mymaxexp ) exp = mymaxexp;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 경험치 증가 - 마스터레벨 유저는 제외	
	if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		exp = (int)((float)exp*gAddExperience);
#else
	exp = (int)((float)exp*gAddExperience);

	float _fDivider = _dynamic_experience._checkCondition(lpObj);
	// ----
	if (_fDivider != 0) 
	{
		exp = _dynamic_experience._getNewExperience(exp, _fDivider);
	}

#endif

#if defined(PCBANG_POINT_SYSTEM_20070206) && defined(ADD_PCBANG_EXPRATE)
	if( !g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
		g_PCBangPointSystem.CheckPCBangAddExperience(lpObj, exp);
#endif // ADD_PCBANG_EXPRATE

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					 // !! NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	if( lpObj->m_wExprienceRate == 0 )
	{
		exp = 0;
	}
	else
	{
		exp = (int)(exp * ((float)lpObj->m_wExprienceRate / 100 ));
	}
		#else
	if( lpObj->m_wExprienceRate > 0 )
	{
		exp = (int)(exp * ((float)lpObj->m_wExprienceRate / 100 ));
	}
		#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	//LogAdd("획득 : [%s][%s] [%s] %d (%d / %d:%d)", lpObj->AccountID, lpObj->Name, lpTargetObj->Name, exp, lpObj->MapNumber, lpObj->X, lpObj->Y);

#ifdef MU_CRYWOLF_PENALTY_20051215
	// MVP 실패 : 경험치 획득 감소 
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
		&& g_iCrywolfApplyMvpPenalty	
	  )
	{
		exp = exp * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}
#endif

	if( exp > 0 ) 
	{
		if( lpObj->Type == OBJTYPE_CHARACTER )
		{
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
			if (g_bChnPlayTimeLimitOn)
			{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
				if( lpObj->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
				{
					if (lpObj->m_iPlayTimeLimit >= 5*60*60) {
						exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
					}
					else if (lpObj->m_iPlayTimeLimit >= 3*60*60) {
						exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
					}
				}
	#else
				if (lpObj->m_iPlayTimeLimit >= 5*60*60) {
					exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
				}
				else if (lpObj->m_iPlayTimeLimit >= 3*60*60) {
					exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
				}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
			}
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 : 데빌스퀘어 경험치 획득량 조정
			if (g_bVtmPlayTimeLimitOn) 
			{
				if (lpObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60) 
				{
					exp = exp * g_iVtmPlayTimeLimitLevel2Exp / 100;
				}
				else if (lpObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
				{
					exp = exp * g_iVtmPlayTimeLimitLevel1Exp / 100;
				}
			}
#endif

/*#ifdef PCBANG_POINT_SYSTEM_20070206		// 악마의광장 - PC방 아이템 적용 확인(경험치)
			g_PCBangPointSystem.CheckItemOptForGetExp( lpObj, exp );
#endif*/
#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
			CheckItemOptForGetExpEx(lpObj, exp);
#else
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정			
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
			g_PCBangPointSystem.CheckItemOptForGetExp( lpObj, exp );
#else
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			CheckItemOptForGetExp(lpObj, exp);
	#else
			//20080416 - 아래 코드는 삭제해도 될것 같음
			int iExpRate = 0;
			iExpRate = gObjGetTotalValueOfEffect( lpObj, EFFECTTYPE_EXPERIENCE );
			if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_PCBANG_POINT_MARK3 ) == false
				&& iExpRate == 0
				)
			{
				iExpRate = 100;
			}
			exp = ( exp * iExpRate ) / 100;
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
			
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif	// BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205			
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

			lpObj->Experience += exp;
			lpObj->m_nEventExp += exp;

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004		// !!NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
			if( lpObj->m_wExprienceRate > 0 )
			{
				lpObj->Experience += exp;
				lpObj->m_nEventExp += exp;
			}
	#else
			lpObj->Experience += exp;
			lpObj->m_nEventExp += exp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
			if (gObjLevelUp(lpObj, exp, lpTargetObj->Class, EVENT_TYPE_NONE ) == false)
				return 0;
#else
	#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
			if (gObjLevelUp(lpObj, exp) == false)
				return 0;
	#endif
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

		}
	}
	return exp;
}


void CDevilSquare::gObjExpParty(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int AttackDamage, int MSBFlag)
{
	int n;
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	INT64 exp, maxexp=0, totalexp;
#else
	int exp, maxexp=0, totalexp;
#endif

#ifdef MODIFY_GETTING_EXP_RATE_20060214
	int level = ((lpTargetObj->Level+25)*lpTargetObj->Level)/3;
#else
#ifdef MODIFY_GETTING_EXP_RATE_20051116
	int level = ((lpTargetObj->Level+20)*lpTargetObj->Level)/4;
#else
	int level = ((lpTargetObj->Level+10)*lpTargetObj->Level)/4;
#endif
#endif // MODIFY_GETTING_EXP_RATE_20060214

	int number;
	int partynum=0;
	int totallevel=0;
	int partylevel;
	int partycount;
	int dis[MAX_PARTYUSER];
	int viewplayer=0;
	int viewpercent=100;

	int bApplaySetParty = FALSE;	// 셋트파티 경험치를 적용하는가?

#ifdef UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	BYTE bCheckSetParty[6];
#else
	#ifdef DARKLORD_WORK
		BYTE bCheckSetParty[5];
	#else
		BYTE bCheckSetParty[4];
	#endif //DARKLORD_WORK
#endif //UPDATE_ADD_NEW_PARTY_SET_20080325	
	
	LPOBJECTSTRUCT lpPartyObj;

	partynum = lpObj->PartyNumber;

#ifdef MODIFY_PARTY_PLAY_EXPERIENCE_20070208

	// 가장 높은 레벨의 파티원의 레벨
	int toplevel = 0;
	int iTmpDis;
	// 최대 레벨의 파티원 구하기
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

#ifdef MODIFY_PARTY_PLAY_EXPERIENCE_2_20070320
			iTmpDis = gObjCalDistance(lpTargetObj, lpPartyObj);
#else
			iTmpDis = gObjCalDistance(lpTargetObj, &lpPartyObj[number]);
#endif

#ifdef MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625	// 종합레벨 적용
			if( iTmpDis < 10 && ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel ) > toplevel )
			{
				toplevel = lpPartyObj->Level + lpPartyObj->m_nMasterLevel;
			}
#else
			if( iTmpDis < 10 && lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
#endif // MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
		}
	}

#else	// MODIFY_PARTY_PLAY_EXPERIENCE_20070208
	
#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
	// 가장 높은 레벨의 파티원의 레벨
	int toplevel = 0;
	// 최대 레벨의 파티원 구하기
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

			if( lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}
#endif	// UPDATE_LOWLEVEL_SUPPORT_1_20051215

#endif	// MODIFY_PARTY_PLAY_EXPERIENCE_20070208
	
	if( !CHECK_LIMIT(partynum, MAX_PARTY) )
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return;
	}

	partycount = gParty.m_PartyS[partynum].Count;
	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// 같은 맵에 존재하거나..
			if( lpTargetObj->MapNumber == lpPartyObj->MapNumber) 
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);
				if( dis[n] < 10 )
				{
					lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
#ifdef MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
					// 종합레벨 적용 - 파티경험치 계산 : 레벨 200이상 차이날 때 처리
					// 경험치 계산 방식 수정을 위해 레벨 200이상 차이나는 유저의 레벨에 200을 더함
					if( toplevel >= ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200 ) )
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel;
					}
#else
	#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
					// 경험치 계산 방식 수정을 위해 레벨 200이상 차이나는 유저의 레벨에 200을 더함 - hnine
					if( toplevel >= (lpPartyObj->Level + 200) )
					{
						totallevel += lpPartyObj->Level + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}
	#else
					totallevel += lpPartyObj->Level;
	#endif
#endif // MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
					viewplayer++;
					
					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}
	
#ifdef UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	if( bCheckSetParty[0] && bCheckSetParty[1] && bCheckSetParty[2]			// 법사, 기사, 요정 세트
		|| bCheckSetParty[3] && bCheckSetParty[4] && bCheckSetParty[5] )	// 마검사, 다크로드, 소환술사 세트
#else
	if( bCheckSetParty[0] && bCheckSetParty[1] && bCheckSetParty[2] )		// 법사, 기사, 요정 세트
#endif //UPDATE_ADD_NEW_PARTY_SET_EXP_20080325
	{	// 셋트 파티 적용(법사, 기사, 요정만 있으면 된다)
		bApplaySetParty = TRUE;
	}

	if( viewplayer > 1 )
	{

#ifdef MODIFY_GETTING_EXP_RATE_20060214
		if( bApplaySetParty )
		{	
			// 세트 경험치가 적용되면
			if( viewplayer == 3 )		
			{
				viewpercent = 230;
			}
			else if( viewplayer == 4 )
			{
				viewpercent = 270;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 300;
			}
			else
			{
				viewpercent = 120;
			}
		}
		else
		{
			if( viewplayer == 2 )
			{
				viewpercent = 160;
			}
			else if( viewplayer == 3 )
			{
				viewpercent = 180;
			}
			else if( viewplayer == 4 )	
			{
				viewpercent = 200;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 120;
			}
		}
#else
#ifdef MODIFY_GETTING_EXP_RATE_20051116
		if( bApplaySetParty )
		{	
			// 세트 경험치가 적용되면
			if( viewplayer == 3 )		
			{
				viewpercent = 170;
			}
			else if( viewplayer == 4 )
			{
				viewpercent = 190;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 120;
			}

		}
		else
		{
			if( viewplayer == 3 )
			{
				viewpercent = 150;
			}
			else if( viewplayer == 4 )	
			{
				viewpercent = 170;
			}
			else if( viewplayer >= 5 )	
			{
				viewpercent = 190;
			}
			else
			{
				viewpercent = 120;
			}
			
		}

#else
		if( bApplaySetParty )
		{	// 세트 경험치가 적용되면
			if( viewplayer == 3 )	viewpercent = 170;
			else if( viewplayer == 4 )	viewpercent = 190;
			else if( viewplayer >= 5 )	viewpercent = 220;
			else viewpercent = 120;
		}	
		else
		{
			if( viewplayer == 3 )	viewpercent = 150;
			else if( viewplayer == 4 )	viewpercent = 170;
			else if( viewplayer >= 5 )	viewpercent = 190;
			else viewpercent = 120;
		}

#endif // MODIFY_GETTING_EXP_RATE_20051116
#endif // MODIFY_GETTING_EXP_RATE_20060214
			
		partylevel = totallevel/viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}
	//LogAdd("viewpercent : Monster:%d %d %d party level : %d",level, viewpercent, totallevel, partylevel);

	
	if((lpTargetObj->Level+10) < partylevel)
	{
		level =  level * (lpTargetObj->Level+10) / partylevel;
	}

	
#ifdef EXP_CAL_CHANGE
	if( lpTargetObj->Level >= 65 )	
	{	// 몬스터 레벨이 50이상이면
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 종합레벨 - 악마의광장 몬스터레벨 계산 조정
		if( viewplayer == 1 )
		{
			level += ( ( lpTargetObj->Level + lpTargetObj->m_nMasterLevel ) - 64 ) * ( lpTargetObj->Level / 4 );
		}
		else
		{
			level += 200 - ( ( lpObj->Level + lpObj->m_nMasterLevel ) * 0.2 );
		}
#else
		if( viewplayer == 1 )
		{
	#ifdef MODIFY_GETTING_EXP_RATE_20051116
			level += (lpTargetObj->Level-64)*(lpTargetObj->Level/3);
	#else
			level += (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	#endif
		}
		else
		{
#ifdef MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
			level += 200 - ( ( lpObj->Level + lpObj->m_nMasterLevel ) * 0.2 );
#else
			level += 200 - (lpObj->Level*0.2);
#endif // MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
		}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912
	}
#endif

	if( level > 0 ) 
	{
		maxexp = level/2;
	}
	else level = 0;

	if( maxexp < 1 ) {
		totalexp = level;
	}
	else 
	{
		totalexp = level+(rand()%maxexp);
	}
	
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		lpTargetObj->Money = totalexp;
	}

	for( n=0; n<MAX_PARTYUSER; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// 같은 맵에 존재하거나..
			if( lpTargetObj->MapNumber == lpPartyObj->MapNumber) 
			{
				// 비슷한 거리에 존재하는 플레이어만..
				if( dis[n] < 10 )
				{
#ifdef MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
					// 종합레벨 적용 - 파티 경험치 구할 때 최대 경험치 구하기
					INT64 mymaxexp = 0;
					if( !g_MasterLevelSystem.CheckMLGetExp( lpPartyObj, lpTargetObj ) )
					{	
						// 파티먹은 마스터레벨 유저라도 제한레벨 이하의 몬스터를 잡으면 경치 없다.
						exp = 0;
					}
					else
					{
						if( g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						{
							mymaxexp = lpPartyObj->m_i64NextMasterLevelExp;
						}
						else
						{
							mymaxexp = gLevelExperience[lpPartyObj->Level];
						}				
						
						exp = ( totalexp * viewpercent ) * ( lpPartyObj->Level + lpPartyObj->m_nMasterLevel ) / totallevel / 100;
					}
#else
					DWORD mymaxexp = gLevelExperience[lpPartyObj->Level];
					
					exp = (totalexp*viewpercent)*lpPartyObj->Level/totallevel/100;
#endif // MODIFY_MASTER_LEVEL_DEVIL_SQUARE_PARTY_EXP_BUGFIX_20080625
					
					// 자신의 경험치 최대값보다 크면..
					if( exp > mymaxexp ) exp = mymaxexp;
					
					// PK는 경험치를 안준다.
					if( lpPartyObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
					{
						exp = 0;
					}
	#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 경험치 증가 - 마스터레벨 유저는 제외	
					if( !g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						exp = (int)((float)exp*gAddExperience);
	#else					
					exp = (int)((float)exp*gAddExperience);
	#endif
#if defined(PCBANG_POINT_SYSTEM_20070206) && defined(ADD_PCBANG_EXPRATE)
					if( !g_MasterLevelSystem.IsMasterLevelUser( lpPartyObj ) )
						g_PCBangPointSystem.CheckPCBangAddExperience(lpPartyObj, exp);
#endif // ADD_PCBANG_EXPRATE

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
					if( lpPartyObj->m_wExprienceRate == 0 )
					{
						exp = 0;
					}
					else
					{
						exp = (int)(exp * ((float)lpPartyObj->m_wExprienceRate / 100 ));
					}
		#else
					if( lpPartyObj->m_wExprienceRate > 0 )
					{
						exp = (int)(exp * ((float)lpPartyObj->m_wExprienceRate / 100 ));
					}
		#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef MU_CRYWOLF_PENALTY_20051215
					// MVP 실패 : 경험치 획득 감소 
					if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
						&& g_iCrywolfApplyMvpPenalty	
					  )
					{
						exp = exp * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
					}
#endif
					
					if( exp > 0 ) {
						if( lpPartyObj->Type == OBJTYPE_CHARACTER ) 
						{
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
							if (g_bChnPlayTimeLimitOn)
							{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
								if( lpPartyObj->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
								{
									if (lpPartyObj->m_iPlayTimeLimit >= 5*60*60)
									{
										exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
									}
									else if (lpPartyObj->m_iPlayTimeLimit >= 3*60*60)
									{
										exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
									}
								}
	#else
								if (lpPartyObj->m_iPlayTimeLimit >= 5*60*60)
								{
									exp = exp * g_iChnPlayTimeLimitLevel2Exp / 100;
								}
								else if (lpPartyObj->m_iPlayTimeLimit >= 3*60*60)
								{
									exp = exp * g_iChnPlayTimeLimitLevel1Exp / 100;
								}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
							}
#endif

							
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남 : 데비스퀘어 경험치 획득량 조정
							if (g_bVtmPlayTimeLimitOn) 
							{
								if (lpPartyObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60) 
								{
									exp = exp * g_iVtmPlayTimeLimitLevel2Exp / 100;
								}
								else if (lpPartyObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
								{
									exp = exp * g_iVtmPlayTimeLimitLevel1Exp / 100;
								}
							}
#endif


#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
								CheckItemOptForGetExpEx(lpPartyObj, exp);
	#else // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
	//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC 적용 후 아래코드 삭제할 예정
	#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
							g_PCBangPointSystem.CheckItemOptForGetExp( lpPartyObj, exp );
	#else
		#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
							CheckItemOptForGetExp(lpPartyObj, exp);
		#else
							int iExpRate = 0;
							iExpRate = gObjGetTotalValueOfEffect( lpPartyObj, EFFECTTYPE_EXPERIENCE );
							if( gObjCheckUsedBuffEffect( lpPartyObj, BUFFTYPE_PCBANG_POINT_MARK3 ) == false
								&& iExpRate == 0
							)
							{
								iExpRate = 100;
							}

							exp = ( exp * iExpRate ) / 100;
		#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318

	#endif	// BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205
	//<--
	#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

							lpPartyObj->Experience += exp;
							lpPartyObj->m_nEventExp += exp;
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
							if( lpPartyObj->m_wExprienceRate > 0 )
							{
								lpPartyObj->Experience += exp;
								lpPartyObj->m_nEventExp += exp;
							}
	#else
							lpPartyObj->Experience += exp;
							lpPartyObj->m_nEventExp += exp;
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

							if(lpTargetObj->Money < exp)
							{
								lpTargetObj->Money = exp;
							}

							/*if( exp > 1200 )
							{
								LogAddC(LOGC_RED, lMsg.Get(526), lpPartyObj->AccountID, lpPartyObj->Name, lpPartyObj->Level, exp, lpTargetObj->Name);
							}*/
							//LogAdd("획득 : [%s][%s] [%s] %d (%d / %d:%d)", lpPartyObj->AccountID, lpPartyObj->Name, lpTargetObj->Name, exp, lpPartyObj->MapNumber, lpPartyObj->X, lpPartyObj->Y);

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
							if (gObjLevelUp(lpPartyObj, exp, lpTargetObj->Class, EVENT_TYPE_PARTY ) == false)
								continue;
#else
	#ifdef MAX_LEVEL_BLOCK_GAIN_EXPERIENCE_20050613
							if (gObjLevelUp(lpPartyObj, exp) == false)
								continue;
	#endif
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

						}
					}
					if( lpPartyObj->Type == OBJTYPE_CHARACTER )
					{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
#else
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
						if( lpPartyObj->m_wExprienceRate > 0 )
						{
							GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
						}
	#else
						GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
					}
				}
			}
		}
	}
}

// 이벤트 몬스터 점수분배
void CDevilSquare::gObjMonsterScoreDivision(LPOBJECTSTRUCT lpMonObj, LPOBJECTSTRUCT lpObj, int AttackDamage, int MSBFlag)
{
	LPOBJECTSTRUCT lpTargetObj;	
	gObjMonsterHitDamageUserDel(lpMonObj);

	lpMonObj->Money = 0;

	int HitIndex;
	int LastHitObjNum = gObjMonsterLastHitDamageUser(lpMonObj, HitIndex);

	if( LastHitObjNum != -1 )
	{
		lpTargetObj = &gObj[LastHitObjNum];
		int score = (int)(float)((lpMonObj->sHD[HitIndex].HitDamage/lpMonObj->MaxLife)*lpMonObj->Level);
		score *= (lpTargetObj->m_bDevilSquareIndex+1);
		lpTargetObj->m_nEventScore += score;
	}
}

// 아이템을 정리해 준다
void CDevilSquare::ItemClear()
{
	DWORD CurTime=GetTickCount();
	for( int i = 0; i < MAX_MAPITEM; i++ )
	{
		MapC[9].m_cItem[i].m_State	= OBJST_DIECMD;
	}
}

void CDevilSquare::CalcScore()
{	// 랭킹을 구하고, 각각의 클라이언트에게 전송해준다
	for( int n = 0 ;n < MAX_DEVILSQUARE; n++ )
	{	// 점수 초기화 
		m_DevilSquareGround[n].ClearScore();
	}

	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{	// 각각의 광장에 객체를 넣어주고,
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( gObj[n].Connected == 3 && CHECK_DEVILSQUARE(gObj[n].MapNumber)  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#else
		if( gObj[n].Connected == 3 && gObj[n].MapNumber == 9  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#endif
		{	
#ifdef DEVILSQUARE_EXTEND_20050221
			if (CHECK_DEVILSQUARE_INDEX(gObj[n].m_bDevilSquareIndex))
#endif
			m_DevilSquareGround[gObj[n].m_bDevilSquareIndex].InsertObj(&gObj[n]);
		}
	}

	for( int n = 0 ;n < MAX_DEVILSQUARE; n++ )
	{	// 점수별로 정렬한후  
		m_DevilSquareGround[n].SortScore();
		// 각각에게 스코어를 보낸다
		m_DevilSquareGround[n].SendScore();
	}
}

void CDevilSquare::SendLiveCall()
{
	PMSG_ANS_EVENTLIVE pMsg;	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x03;
	pMsg.h.size		= sizeof( pMsg );	
	DataSendRank((char*)&pMsg, pMsg.h.size);
}

void CDevilSquare::gObjScoreClear()
{
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( gObj[n].Connected == 3 && CHECK_DEVILSQUARE(gObj[n].MapNumber)  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#else
		if( gObj[n].Connected == 3 && gObj[n].MapNumber == 9  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#endif
		{	
			gObj[n].m_nEventScore = 0;
			gObj[n].m_nEventMoney = 0;
			gObj[n].m_nEventExp = 0;	
		}
	}
}

void CDevilSquare::CheckInvalidUser()
{
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( gObj[n].Connected == 3 && CHECK_DEVILSQUARE(gObj[n].MapNumber)  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#else
		if( gObj[n].Connected == 3 && gObj[n].MapNumber == 9  && gObj[n].m_stateEngagePVP != PVP_USER_PLAYING )
#endif
		{
			if( m_eState == DevilSquare_CLOSE )
			{	// 게임중이 아니다
				LogAddC(LOGC_RED,"[DevilSquare] [%s][%s] Found user in DevilSquare [State:Close]",
						gObj[n].AccountID, 
						gObj[n].Name);
				gObjUserKill(gObj[n].m_Index);
			}			
		}
	}
}

#ifdef MODIFY_SEASON25_PCS_UNIFICATION_20070903
#if TESTSERVER == 1
void CDevilSquare::SetRemainTime(int iRemainTime)
{
	m_iRemainTime = iRemainTime;
}
#endif
#endif // MODIFY_SEASON25_PCS_UNIFICATION_20070903