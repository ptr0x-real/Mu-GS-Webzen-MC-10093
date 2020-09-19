// BloodCastle.cpp: implementation of the CBloodCastle class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BloodCastle.h"

#ifdef FOR_BLOODCASTLE
CBloodCastle			g_BloodCastle;		// ���� ĳ�� �̺�Ʈ ��ü


#include <stdio.h>

#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Include\\Public.h"
#include "..\\include\\cb_protocol.h"
#include "..\\Include\\public.h"
#include "..\\Include\\Readscript.h"

#include "User.h"
#include "NpcTalk.h"
#include "protocol.h"

#include "wsJoinServerCli.h"

#include "MonsterSetBase.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "PartyClass.h"
#include "wzUdp.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "DSProtocol.h"

#include "MapClass.h"
#include "GameServer.h"
#include "DirPath.h"
#include "..\\Common\\Classdef.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219
#include "CastleSiegeSync.h"
#endif

#ifdef BLOODCASTLE_EVENT_5TH_20050531
#include "MonsterAttr.h"
extern CMonsterAttr		gMAttr;
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
#include "CrywolfSync.h"
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#include "..\\common\\zzzitem.h"

extern ITEM_ATTRIBUTE	ItemAttribute[];

extern PartyClass		gParty;
extern CMonsterSetBase	gMSetBase;
extern MapClass			*MapC;
extern classdef			DCInfo;


extern wsJoinServerCli	wsRServerCli;			//  ��ŷ����
extern wsJoinServerCli	wsEvenChipServerCli;	//  �̺�Ʈ Ĩ ����

extern CDirPath			gDirPath;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBloodCastle::CBloodCastle()
{
	m_bBC_EVENT_ENABLE = false;

	m_iBC_TIME_MIN_OPEN = 10;		// ���߿� ���Ͽ��� �о�� ��

	m_iBC_NORMAL_ITEM_DROP = 100;
	m_iBC_EXCEL_ITEM_DROP = 1200;
	m_iBC_MONSTER_REGEN = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		m_BridgeData[i].m_iBC_STATE = BLOODCASTLE_STATE_NONE;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� ��ȣ ����
		m_BridgeData[i].m_iMapNumber = GetMapNumByBCBridge( i );
#else
		m_BridgeData[i].m_iMapNumber = MAP_INDEX_BLOODCASTLE1 + i;
#endif
		m_BridgeData[i].m_iBridgeIndex = i;
		m_BridgeData[i].m_iBC_REMAIN_MSEC = -1;
		m_BridgeData[i].m_iBC_TICK_COUNT = -1;
		m_BridgeData[i].m_nSaintStatuePosNum = -1;
		m_BridgeData[i].m_nCastleDoorPosNum = -1;
		m_BridgeData[i].m_iAngelKingPosNum = -1;
		
	#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
		m_BridgeData[i].m_fRewardExpRate = 1.0f;
	#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			
		InitializeCriticalSection (& m_BridgeData[i].m_critUserData);

		ClearBridgeData(i);
	}
}


CBloodCastle::~CBloodCastle()
{
	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		DeleteCriticalSection (& m_BridgeData[i].m_critUserData);
	}
}


void CBloodCastle::Init (bool bEVENT_ENABLE)
{
	m_bBC_EVENT_ENABLE = bEVENT_ENABLE;

	// 1 . ������ ���� ���� �����͸� �ʱ�ȭ �Ѵ�.
	for (int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
		for (int j = 0 ; j < MAX_BLOODCASTLE_BOSS_COUNT ; j++) {
			m_BridgeData[i].m_iAngelKingPosNum = -1;
			m_BridgeData[i].m_nBossMonsterPosNum[j] = -1;
		}
	}

	// 2 . MonsterSetBase.txt�� �о �����Ǽ���, ����, �������� �ε����� �о �����Ѵ�.
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if( CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber) )		// ���� ĳ���� ���� ���͸�
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���� Ÿ�� ������			
			WORD btMonsterType = gMSetBase.m_Mp[n].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[n].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ���ȣ ����
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btMonsterType == NPC_ANGELKING) {					// ��õ��� �̸� �߰��ǹǷ� ���⼭ ���� �߰����� �ʴ´�. -> ��� ������ ��ȣ�� �о�д�.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ���ȣ ����
				m_BridgeData[btBloodCastleIndex].m_iAngelKingPosNum = n;
#else
				m_BridgeData[gMSetBase.m_Mp[n].m_MapNumber - MAP_INDEX_BLOODCASTLE1].m_iAngelKingPosNum = n;
#endif
				continue;
			}

			if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3)) {	// ������ ������ ������ ��ȣ�� �д´�.
				m_BridgeData[btBloodCastleIndex].m_nSaintStatuePosNum = n;
				continue;
			}

			if (btMonsterType == 131) {								// ������ ������ ��ȣ�� �д´�.
				m_BridgeData[btBloodCastleIndex].m_nCastleDoorPosNum = n;
				continue;
			}

			// ������ ������ ���� ������ ��ȣ�� �д´�.
			if ((btMonsterType == 89) ||							// �����ذ�1
				(btMonsterType == 95) ||							// �����ذ�2
				(btMonsterType == 112) ||							// �����ذ�3
				(btMonsterType == 118) ||							// �����ذ�4
				(btMonsterType == 124) ||							// �����ذ�5
				(btMonsterType == 130)								// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// �����ذ�7
#endif		
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ�� �������� ������ ����
				|| (btMonsterType == 433)							// �����ذ�8
#endif				
				)
			{
				for (int i = 0 ; i < MAX_BLOODCASTLE_BOSS_COUNT ; i++) {
					if (m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[i] == -1) {
						m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[i] = n;
						break;
					}
				}				
			}
		}
	}

	for ( int i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++)
		SetState(i, BLOODCASTLE_STATE_CLOSED);
}


void CBloodCastle::Load(char* filename)
{
	// *> . ��ũ��Ʈ�� �о �ʿ��� �����͸� �����Ѵ�.
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("[Blood Castle] Info file Load Fail [%s]", filename);
		return ;
	}
	SMDToken Token;
	
	int type = -1;
	int BridgeCount = -1;
	int iBridgeNum;

	for(int i = 0;i < MAX_BLOODCASTLE_BRIDGE_COUNT;++i)
		m_LST_BLOODCASTLE_REWARD[i].clear();
	
	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		type = (int)TokenNumber;
		while(1)
		{				
			if( type == 0 )
			{	// ����ĳ�� ���ӽð� ����
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iBC_TIME_MIN_OPEN = (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_TIME_MIN_PLAY = (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_TIME_MIN_REST = (int)TokenNumber;
			}
			else if( type == 1 )
			{	// ����ĳ�� �⺻ �� ���� ������ �����, ���� �����ð� ����
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_iBC_NORMAL_ITEM_DROP = (int)TokenNumber;			
				Token = (*GetToken)(); m_iBC_EXCEL_ITEM_DROP	= (int)TokenNumber;
				Token = (*GetToken)(); m_iBC_MONSTER_REGEN		= (int)TokenNumber;
			}
			else if( type == 2 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;
				
				if (CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iCastleStatueHealth = (float)TokenNumber;
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iCastleDoorHealth = (float)TokenNumber;
					Token = (*GetToken)(); m_BridgeData[iBridgeNum].m_iDropItemCount = TokenNumber;
				}
				else {
					Token = (*GetToken)();
					Token = (*GetToken)();
				}
			}
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
			else if( type == 3 )
			{	// ���� ĳ�� ���� ����ġ �ۼ�Ʈ
				Token = (*GetToken)();
				if( strcmp("end", TokenString) == NULL ) break;

				iBridgeNum = (int)TokenNumber;
				
				if( CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT) ) {
					Token = (*GetToken)();
					m_BridgeData[iBridgeNum].m_fRewardExpRate = (FLOAT)TokenNumber;
				}
				else Token = (*GetToken)();
			}
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
			else if( type == 4 )
			{
				BLOODCASTLE_STARTTIME startTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				startTime.nHour = (int)TokenNumber;
				Token = (*GetToken)(); startTime.nMin = (int)TokenNumber;			

				m_LST_BLOODCASTLE_START_TIME.push_back(startTime);

			}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624			
			else if( type == 5 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;

				while(true)
				{
					BLOODCASTLE_REWARD reward;

					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					reward.item_type = (unsigned short)TokenNumber;

					Token = (*GetToken)();
					reward.item_type = ItemGetNumberMake(reward.item_type, (int)TokenNumber);

					Token = (*GetToken)();
					reward.item_level = (char)TokenNumber;

					Token = (*GetToken)();
					reward.is_skill = TokenNumber > 0;

					Token = (*GetToken)();
					reward.is_luck = TokenNumber > 0;

					Token = (*GetToken)();
					reward.option = (char)TokenNumber;

					Token = (*GetToken)();
					reward.exc_option = (char)TokenNumber;

					Token = (*GetToken)();
					reward.max_drop_rate = (unsigned short)TokenNumber;

					if (CHECK_LIMIT(iBridgeNum, MAX_BLOODCASTLE_BRIDGE_COUNT))
						m_LST_BLOODCASTLE_REWARD[iBridgeNum].push_back(reward);
				};
			}
			else if( type == 6 )
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				iBridgeNum = (int)TokenNumber;

				while(true)
				{
					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					if(iBridgeNum >= 0 && iBridgeNum < MAX_BLOODCASTLE_BRIDGE_COUNT)
					{
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iLOWER_BOUND_MAGUMSA = TokenNumber;

						Token = (*GetToken)();
						g_sttBLOODCASTLE_LEVEL[iBridgeNum].iUPPER_BOUND_MAGUMSA = TokenNumber;
					}
					else
					{
						Token = (*GetToken)();
						Token = (*GetToken)();
						Token = (*GetToken)();
					}
				};
			}
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!", filename);

// grooving �系 �׽�Ʈ ���� ĳ�� �ð� ����
#if TESTSERVER == 1 && defined(UPDATE_BLOODECASTLE_SCADULE_20080624)

	m_iBC_TIME_MIN_OPEN = 1;
	m_iBC_TIME_MIN_PLAY = 5;
	
	BLOODCASTLE_STARTTIME startTime;

	m_LST_BLOODCASTLE_START_TIME.clear();

	for( int i = 0; i < 24; i++)
	{
		startTime.nHour = i;
		
		for( int j = 0; j < 60; j+= 6 )
		{
			startTime.nMin = j;
			
			m_LST_BLOODCASTLE_START_TIME.push_back(startTime);
		}
	}			
#endif//TESTSERVER

}


void CBloodCastle::LoadItemDropRate ()
{
	// ���� ĳ�� ���࿩�ο� ������ ���Ȯ�� �κи� ���ε� �Ѵ�.
	g_bBloodCastle				= GetPrivateProfileInt("GameServerInfo", "BloodCastleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iAngelKingsPaperDropRate	= GetPrivateProfileInt("GameServerInfo", "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBloodBoneDropRate		= GetPrivateProfileInt("GameServerInfo", "BloodBoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iStoneDropRate			= GetPrivateProfileInt("GameServerInfo", "StoneDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_bStoneItemDrop			= GetPrivateProfileInt("GameServerInfo", "StoneItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));
}

#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
void CBloodCastle::CheckSync(int iBridgeIndex)			// ���ʿ� BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING ���·� ���ϱ� ������ �ð�üũ
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	list<BLOODCASTLE_STARTTIME>::iterator iter;
	BLOODCASTLE_STARTTIME	prevStartTime;
	BLOODCASTLE_STARTTIME	nextStartTime;

	DWORD	dwPrevTime	= 0;;		// ���� ���� �ð�(��)
	DWORD	dwNextTime	= 0;;		// ���� ���� �ð�(��)
	DWORD	dwCurTime	= 0;;		// ���� �ð�(��)
	
	struct tm *today;
	time_t ltime;

	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
	
	if(m_LST_BLOODCASTLE_START_TIME.size() == 0)
	{
		MsgBox("Error : Blood Castle StartTime size is 0");
		return;
	}
	
	time( &ltime );
	today = localtime( &ltime );
	dwCurTime	= today->tm_hour * 60 + today->tm_min;

	// ���� �̺�Ʈ ���� �ð��� �˻�
	prevStartTime = *m_LST_BLOODCASTLE_START_TIME.begin();
	for(iter = m_LST_BLOODCASTLE_START_TIME.begin(); iter != m_LST_BLOODCASTLE_START_TIME.end(); ++iter)
	{
		nextStartTime = *iter;

		// ���۽ð��� ������ ���
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
		if(iter == m_LST_BLOODCASTLE_START_TIME.end())
		{
			// ���� �� �ð�
			iter = m_LST_BLOODCASTLE_START_TIME.begin();
			nextStartTime = *iter;
		}
		
		// ���� ���۽ð��� �ٽ� ���
		dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;

		// ���� ���۱��� ���� �ð� ���
		if(today->tm_hour <= nextStartTime.nHour && dwNextTime > dwCurTime)
		{
			// ���� �̺�Ʈ�� ����
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (dwNextTime - dwCurTime) * 60 * 1000;
		}
		else
		{
			// ���� �̺�Ʈ�� ������
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (24 * 60 - dwCurTime + dwNextTime) * 60 * 1000;
		}

		// ���� ���۱��� ���� �ð�(m_nRemainTime)�� ���½ð�(m_iOpenTime)���� ������ ���� �ð����� �ѱ��
		if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000 )
		{
			iter++;
			if(iter != m_LST_BLOODCASTLE_START_TIME.end())
			{
				nextStartTime = *iter;
			}
		}
		else
		{
			break;
		}
	}

	// ���� ��(min) ���
	int tmpTime = GetTickCount();
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC - (today->tm_sec * 1000) ;

	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000));

}
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
void CBloodCastle::CheckSync(int iBridgeIndex)			// ���ʿ� BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING ���·� ���ϱ� ������ �ð�üũ
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	
#if TESTSERVER == 1
	// �׽�Ʈ�� ��ƾ
	//m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_OPEN * 60 * 1000 ;

#ifdef FOR_ONAIR
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (m_iBC_TIME_MIN_OPEN + 5) * 60 * 1000 ;
#else
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (m_iBC_TIME_MIN_OPEN + 1) * 60 * 1000 ;
#endif
	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
#else
	
	struct tm * today;
	time_t ltime;
	
	time( &ltime );
	today = localtime( &ltime );

	if(!CHECK_LIMIT(g_iBloodCastle_StartHour-1,2))
		g_iBloodCastle_StartHour = 1;

	switch(g_iBloodCastle_StartHour) {
	case 2 :	// 2�ð�����
		{
#ifdef UPDATE_DEVIL_SQUARE_START_TIME_20070511	// ¦�� �ð��뿡�� Ȧ�� �ð���� ����
#ifdef UPDATE_BLOODECASTLE_SCADULE_20070702
			if( (today->tm_hour%2) == g_iBloodCastle_odd_Even_Hour)
#else
			if( (today->tm_hour%2) )
#endif
#else
			if( !(today->tm_hour%2) )
#endif
			{
				// ¦���ð�
				if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
					// 30���� �Ѱ�� -> 2�ð� ���� 30���� ���
					m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (((60*2 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
				}
				else {
					// 30���� ���� �ʳѰ�� -> �ð�����
					m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
				}
			}
			else
			{
				// Ȧ���ð�
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (((60 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}
			
			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += (60 * 60) * 1000;
			}
			
			m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
		}
		break;
	case 1 :	// 1�ð�����
	default:
		{
			if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
				// 30���� �Ѱ�� -> 1�ð� ���� 30���� ���
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((60*60) + (30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}
			else {
				// 30���� ���� �ʳѰ�� -> �ð�����
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((30*60) - (today->tm_min * 60 + today->tm_sec)) * 1000;
			}

			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += ((60 * 60)) * 1000;
			}

			m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
		}
		break;
	}
	
	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000));
#endif

}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


void CBloodCastle::ClearBridgeData (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_iTOTAL_EXP							= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT				= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT				= 0;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT		= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT			= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT		= 0;
	m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT	= 0;
	m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER				= 0;
	m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN				= -1;
	m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL				= -1;
	m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX			= -1;
	m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS					= -1;
	m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT					= -1;
	m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE					= TRUE;
	m_BridgeData[iBridgeIndex].m_bBC_REWARDED						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END					= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT				= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_PLAY_START						= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE			= FALSE;
	m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE		= FALSE;

	m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE		= FALSE;
	

	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index		= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party		= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index			= -10;
	m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party			= -10;

	memset(m_BridgeData[iBridgeIndex].m_szKill_Door_CharName, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, '\0', MAX_IDSTRING+1);

	memset(m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, '\0', MAX_IDSTRING+1);
	memset(m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, '\0', MAX_IDSTRING+1);
	
	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
#ifdef BLOODCASTLE_EVENT_5TH_20050531
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
#endif
		}

		m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP					= 0;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore				= 0;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex				= -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState			= BC_USER_ALIVE;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg			= false;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak	= false;

#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
		memset(m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID, 0, sizeof(m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID));
		memset(m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName, 0, sizeof(m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName));
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iCharClass = -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleIndex = -1;
		m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleSubIndex = -1;
#endif	

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
#endif
		
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
		m_BridgeData[iBridgeIndex].m_iDoorIndex				= -1;
#endif
	}
}


void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if ((iBC_STATE < BLOODCASTLE_STATE_NONE) || (iBC_STATE > BLOODCASTLE_STATE_PLAYEND))
		return;

	m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch(m_BridgeData[iBridgeIndex].m_iBC_STATE) {
	case BLOODCASTLE_STATE_NONE :
		SetState_None(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_CLOSED :
		SetState_Closed(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_PLAYING :
		SetState_Playing(iBridgeIndex);
		break;
	case BLOODCASTLE_STATE_PLAYEND :
		SetState_PlayEnd(iBridgeIndex);
		break;
	default:
		break;
	}
}


void CBloodCastle::Run()					// �� ������ �ð��� ��´�.
{
	if (m_bBC_EVENT_ENABLE) {
	
		for (INT i = 0 ; i < MAX_BLOODCASTLE_BRIDGE_COUNT ; i++) {
			switch (m_BridgeData[i].m_iBC_STATE) {
			case BLOODCASTLE_STATE_NONE :
				ProcState_None(i);
				break;
			case BLOODCASTLE_STATE_CLOSED :
				ProcState_Closed(i);
				break;
			case BLOODCASTLE_STATE_PLAYING :
				ProcState_Playing(i);
				break;
			case BLOODCASTLE_STATE_PLAYEND :
				ProcState_PlayEnd(i);
				break;
			default:
				break;
			}
		}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[13] != AUTH_CHECK_13 )
		DestroyGIocp();
#endif
	}
}


void CBloodCastle::ProcState_None(int iBridgeIndex)
{
	
}


void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - CLOSED Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
#endif
		if (g_bBloodCastle) {
			if ( (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000) && (! m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER) ) {
				// �������� ����ĳ���� ����� ��Ƽ�� �����ϴ�.
				m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}
			
			// ����> . ������ �� �ʵ� �� �ִ�. !!
			if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60*1000) != m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT )) {	
				m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60*1000);

				// ���� ���ɽð����� 1�и��� �����Ѵ�.
				if (!iBridgeIndex) {
					// ���ϸ� �ʹ� ������ 1�� �ٸ��� ��ǥ�� => ������ ��� ����ڵ鿡��
					PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
					TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1160), m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT + 1);
#else
					pNotice.type = 0;
					wsprintf(pNotice.Notice, lMsg.Get(1160), m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT + 1);		// "����ĳ�� ������� %d �� ���ҽ��ϴ�."
					PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
					SendAllUserAnyMsg ((LPBYTE) &pNotice, pNotice.h.size);
				}
			}

			if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER)) {
				m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if (!iBridgeIndex) {
					// ���ϸ� �ʹ� ������ 1�� �ٸ��� ��ǥ�� => ������ ��� ����ڵ鿡��
					PMSG_SET_DEVILSQUARE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
					pMsg.Type = BC_MSG_BEFORE_ENTER;

					for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
					{
						if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
						{
							if (!CHECK_BLOODCASTLE(gObj[n].MapNumber)
#ifdef CHAOSCASTLE_SYSTEM_20040408
								&& !CHECK_CHAOSCASTLE(gObj[n].MapNumber)	// ī����ĳ���� �������� ���� ����鸸 ��� ������.
#endif
								)		// ����ĳ���� �������� ���� ����鸸 ��� ������.
								DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		}
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		if (g_bBloodCastle)
			SetState (iBridgeIndex, BLOODCASTLE_STATE_PLAYING);
		else
			SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);
	}

}


void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - PLAYING Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
#endif
		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= (m_iBC_TIME_MIN_PLAY * 60 - BC_MAX_START_WAIT_TERM + 30) * 1000) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_PLAY;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}

		if (m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE) {
			if (m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN != -1) {
				if (GetTickCount() > m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN) {
					// ���� BC_MAX_TICK_DOOR_OPEN �ʰ� ������ -> ������ �ٸ��Ӽ� �ٲپ��
					ReleaseCastleBridge(iBridgeIndex);						
					SendCastleBridgeBlockInfo(iBridgeIndex, false);
					LogAddTD ("[Blood Castle] (%d) Bridge Change Bridge Attribute -> Open", iBridgeIndex+1);

					m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1;

#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
					if(m_BridgeData[iBridgeIndex].m_iDoorIndex == -1)
					{
						LogAddTD ("[Blood Castle] (%d) Door Not Subsist", iBridgeIndex+1);
					}
					else
					{
						LogAddTD ("[Blood Castle] (%d) Door Subsist (%d)(Name: %s)(Dieregen:%d)"
							, iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iDoorIndex
							, gObj[m_BridgeData[iBridgeIndex].m_iDoorIndex].Name
							, gObj[m_BridgeData[iBridgeIndex].m_iDoorIndex].DieRegen);
					}
#endif
				}
			}
		}

		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= (m_iBC_TIME_MIN_PLAY * 60 - BC_MAX_START_WAIT_TERM) * 1000) && (!m_BridgeData[iBridgeIndex].m_bBC_PLAY_START)) {
			// ���ð� �缳��
			m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_PLAY * 60 * 1000;		// BLOODCASTLE_STATE_PLAYING -> BLOODCASTLE_STATE_PLAYEND ������ ���� �ð�
			m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;

			// �̺�Ʈ ���� �޽��� �ʿ�!!
			PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1161), iBridgeIndex + 1 );
#else
			pNotice.type = 0;
			wsprintf(pNotice.Notice, lMsg.Get(1161), iBridgeIndex + 1);				// "�� %d ���� ĳ�� ����Ʈ�� ���۵Ǿ����ϴ�."
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
			SendBridgeAnyMsg ((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);


			// �ٸ��� �Ա��Ӽ��� �����ϰ� ��ü �ο����� �Ա����� �Ӽ��� ������ �˸���.
			ReleaseCastleEntrance(iBridgeIndex);
			SendCastleEntranceBlockInfo(iBridgeIndex, false);

			m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			SetMonster(iBridgeIndex);

			
			// �ٸ��� ��ü �ο����� ���¸� �����ش�.
			SendNoticeState(iBridgeIndex, BC_STATE_START);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}


		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30 * 1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_END;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}


		if (CheckEveryUserDie(iBridgeIndex)) {
			// ���� ���� ����ߴٸ� ���� �޽����� ������.
			PMSG_NOTICE	pNotice;
			
#ifdef MODIFY_NOTICE_20040325
			TNotice::MakeNoticeMsg( &pNotice, 0, lMsg.Get(1162) );
#else
			pNotice.type = 0;
			wsprintf(pNotice.Notice, lMsg.Get(1162));								// "���� ĳ������ ������ �����߽��ϴ�."
			PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
			SendBridgeAnyMsg ((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", 
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Door_CharName
				);
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Status_CharName
				);

			// ���д� ������ ������ �ش�.
			GiveReward_Fail(iBridgeIndex);
			
			SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Failed -> Every User Out", iBridgeIndex+1);
		}
		else {
			if (m_BridgeData[iBridgeIndex].m_bBC_PLAY_START) {
				// �ƴϰ� ��� �������̶�� ���� ���¸� ��� �����ش�.
				if ( !m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE || m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE )
					SendNoticeState(iBridgeIndex, BC_STATE_PLAY);
				else
					SendNoticeState(iBridgeIndex, BC_STATE_PLAY_BOSS);
			}
		}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
		// ����ڰ� ��Ÿ������ üũ
		if (CheckWinnerExist(iBridgeIndex) == true) {
			if (CheckWinnerValid(iBridgeIndex) == true) {
				// ���� ����ĳ�� ��Ⱑ �������� Ȯ�� (������� ��Ƽ�� ��� ��õ�� ��� ����)
				if (CheckWinnerPartyComplete(iBridgeIndex) == true) {
					// ��� �ɸ��� �ִ� �ٸ��� ������ �Ѵ�.
					GiveReward_Win(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);

					// ����Ʈ�� �����Ѵ�.
					SetState(iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);

					LogAddTD ("[Blood Castle] (%d) CheckWinnerPartyComplete(iBridgeIndex) == true", iBridgeIndex+1);
					return;
				}
			}
			else {
				// ������� ���翩�ΰ� �Źٸ��� �ʴٸ� �ٷ� ����ó���Ѵ�.
				GiveReward_Fail(iBridgeIndex);

				// ����Ʈ�� �����Ѵ�.
				SetState(iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);

				LogAddTD ("[Blood Castle] (%d) CheckWinnerValid(iBridgeIndex) == false", iBridgeIndex+1);
				return;
			}
		}
#endif
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		if (!m_BridgeData[iBridgeIndex].m_bBC_REWARDED) {
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", 
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Door_CharName
				);
			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, 
				m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID,
				m_BridgeData[iBridgeIndex].m_szKill_Status_CharName
				);

			// ���д� ������ ������ �ش�.
			GiveReward_Fail(iBridgeIndex);

			LogAddTD ("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
		}

		SetState (iBridgeIndex, BLOODCASTLE_STATE_PLAYEND);
	}
}


void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
//	INT iTICK_SEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT) / 1000;
	INT iTICK_MSEC = (GetTickCount() - m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT);
	
	if (iTICK_MSEC >= 1000) {
		m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

#if TESTSERVER == 1
		//LogAddTD("[Blood Castle] (%d) - PLAY END Check Time. [%d] sec remain", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/1000);
#endif
		if ((m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30*1000) && (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0) && (!m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT)) {
			m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof( pMsg ));
			pMsg.Type = BC_MSG_BEFORE_QUIT;
			SendBridgeAnyMsg ((LPBYTE)&pMsg, sizeof( pMsg ), iBridgeIndex);
		}
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0) {
		SetState (iBridgeIndex, BLOODCASTLE_STATE_CLOSED);
	}
}


void CBloodCastle::SetState_None(int iBridgeIndex)
{
	// ���� �������� �̺�Ʈ�� �����ϰ� �ʱ�ȭ �� (���ڱ� �̺�Ʈ�� �������Ѿ� �� �� ����Ѵ�.)
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC	= -1;
	m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT		= -1;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearBridgeData(iBridgeIndex);

	ClearMonster(iBridgeIndex, true);

	// Ȥ�� ����ĳ���� �����ִ� ����� ������ ��� ����ƽ��� �̵���Ų��.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if( (gObj[n].MapNumber == iMapNum) && (gObj[n].Connected == 3) )
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected == 3) )
#endif
		{					
			gObjMoveGate(n, 22);
		}
	}

	// ���� �Ӽ� ���¸� ������ ���Ƴ��´�.
	BlockCastleDoor(iBridgeIndex);
}


void CBloodCastle::SetState_Closed(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearBridgeData(iBridgeIndex);

	ClearMonster(iBridgeIndex, true);

	CheckAngelKingExist (iBridgeIndex);				// ��õ�簡 �����ϴ��� üũ -> ������ �����.

//	BlockSector(iBridgeIndex + MAP_INDEX_BLOODCASTLE1, 13,	77,	15,	78);		// !!> ���� �׽�Ʈ�� -> ���� �ȹ��� ���� �̵��� �ݸ���

	// Ȥ�� ����ĳ���� �����ִ� ����� ������ ���빫�Ⱑ �ִ��� ã�Ƽ� ����� ��� ����ƽ��� �̵���Ų��.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );		
		if( (gObj[n].MapNumber == iMapNum) && (gObj[n].Connected > 2) )
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected > 2) )
#endif
		{
			SearchUserDeleteQuestItem(n);			
			gObjMoveGate(n, 22);
		}
	}

	// ���� �Ӽ� ���¸� ������ ���Ƴ��´�.
	BlockCastleDoor(iBridgeIndex);
	// ���� �ٸ��� �Ӽ� ���¸� ������ ���Ƴ��´�.
	BlockCastleBridge(iBridgeIndex);
	// �ٸ��� �Ա��� �Ӽ� ���¸� �ո����� ���Ƴ��´�.
	BlockCastleEntrance(iBridgeIndex);

	CheckSync(iBridgeIndex);						// BLOODCASTLE_STATE_CLOSED -> BLOODCASTLE_STATE_PLAYING ������ ���� �ð��� �������

	LogAddTD("[Blood Castle] (%d) SetState CLOSED", iBridgeIndex+1);
}


void CBloodCastle::SetState_Playing(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_PLAY * 60 * 1000;			// BLOODCASTLE_STATE_PLAYING -> BLOODCASTLE_STATE_PLAYEND ������ ���� �ð�
	
	// *> . ���� ��Ű�� ���� �ش� ĳ���� ����� �� ������ ���� �ο��� �ɷ�����. -> ���� �ε����� ���� �ٸ� ĳ���� ����ڿ� ��ġ�� �ʵ��� (2004.06.09)
	CheckUsersOnConnect(iBridgeIndex);
	
	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsgEx( &pNotice, 1, lMsg.Get(1163), iBridgeIndex + 1, BC_MAX_START_WAIT_TERM );
#else
	pNotice.type = 1;
	wsprintf(pNotice.Notice, lMsg.Get(1163), iBridgeIndex + 1, BC_MAX_START_WAIT_TERM);		// "�� %d ���� ĳ���� ����Ʈ ������ %d �� �� �Դϴ�."
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	SendBridgeAnyMsg ((LPBYTE) &pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD	ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;
	SendBridgeAnyMsg ((LPBYTE) &ServerCmd, ServerCmd.h.size, iBridgeIndex);

	// ����ĳ���� �����ִ� ����� ������ ���빫�Ⱑ �ִ��� ã�Ƽ� �����.
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		if( (gObj[n].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) && (gObj[n].Connected > 2) ) {
#else
		if( (gObj[n].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[n].Connected > 2) ) {
#endif
			SearchUserDeleteQuestItem(n);
		}
	}

	LogAddTD("[Blood Castle] (%d) SetState PLAYING", iBridgeIndex+1);
}


void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	SendNoticeState(iBridgeIndex, BC_STATE_END);

	ClearMonster(iBridgeIndex, false);

	m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = m_iBC_TIME_MIN_REST * 60 * 1000;		// BLOODCASTLE_STATE_PLAYEND -> BLOODCASTLE_STATE_CLOSED ������ ���� �ð�

	LogAddTD("[Blood Castle] (%d) SetState PLAYEND", iBridgeIndex+1);

	for (int n=0 ; n < MAX_BLOODCASTLE_USER_COUNT ; n++) {
		if (CHECK_LIMIT(m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex, MAX_OBJECT)) {
			SearchUserDeleteQuestItem(m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}

//	�׽�Ʈ !! - �ڽ��� ȹƯ�� ����ġ�� ������
//	SendNoticeScore(iBridgeIndex);
}


int CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	return m_BridgeData[iBridgeIndex].m_iBC_STATE;
}


int CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
}


int CBloodCastle::CheckEnterLevel (int iIndex, int iLevel)		// �ش� ����ڰ� ���� ĳ���� ������ �� �ִ��� Ȯ��
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 2;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 2;

#ifdef USER_ENTER_BLOODCASTLE7_WITH6_20040330
#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if (iLevel == 6) {	// �������� ������ 6�̰�
			if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel].iUPPER_BOUND_MAGUMSA)) {
				// ���� 7ĳ���� �� �� �ִ� �����̴�.
				return 0;
			}
		}
	}
	else {
		if (iLevel == 6) {	// �������� ������ 6�̰�
			if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel].iUPPER_BOUND)) {
				// ���� 7ĳ���� �� �� �ִ� �����̴�.
				return 0;
			}
		}
	}
#endif

#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA))
			return 0;		// �����ִ�.
		else if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA)
			return -1;		// �� ������ ����.
		else if (gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA)
			return 1;		// �� ������ ����.
	}
	else {
		if ( (gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND))
			return 0;		// �����ִ�.
		else if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND)
			return -1;		// �� ������ ����.
		else if (gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND)
			return 1;		// �� ������ ����.
	}

	return 2;
}

#ifdef MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627
bool CBloodCastle::CheckEnterFreeTicket(INT iIndex)		// ���� ������� �����ϰ� �ִ��� Ȯ��
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 0;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			// ���� ĳ�� ���� �����
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM( 13, 47 ) )
			{
				return true;
			}
		}
	}

	return false;
}
#endif // MODIFY_BLOODCASTLE_ENTER_ITEMCHECK_20070627

bool CBloodCastle::BloodCastleChaosMix (int iIndex, int iLEVEL)		// ����ĳ�� ����� (����) ������ �����Ѵ�.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;
	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
	int iMapNum = GetMapNumByBCBridge( iLEVEL - 1 );

	if( !CHECK_BLOODCASTLE( iMapNum ) )
		return false;
#else	
	if(!CHECK_BLOODCASTLE( iLEVEL - 1 + MAP_INDEX_BLOODCASTLE1 ) )
		return false;
#endif
	
	BOOL bMIX_RESULT=FALSE;		// ������ ����� �������� �������� ?
	
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;
	
	gObj[iIndex].ChaosLock = TRUE;
	LogAddTD("[Blood Castle] ������� Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", 
		gObj[iIndex].AccountID, 
		gObj[iIndex].Name, 
		iLEVEL);

#ifdef EXTEND_LOG_SYSTEM_05_20060823
	char chChaosLogText[20];

	wsprintf( chChaosLogText, "������� Mix,%d", iLEVEL );
	g_MixSystem.LogChaosItem(&gObj[iIndex], chChaosLogText );
#elif defined(MODIFY_MIX_SYSTEM_20070518)
	g_MixSystem.LogChaosItem(&gObj[iIndex], "������� Mix");
#endif // EXTEND_LOG_SYSTEM_05_20060823
	
	INT iMIX_SUCCESS_RATE = g_iBC_ChoasMixSuccessRate[iLEVEL-1];
	if ( (iMIX_SUCCESS_RATE < 0) || (iMIX_SUCCESS_RATE > 100) ) {
		// ����Ȯ�� ��� (��ü���� ->  ���� ���� Ȯ���� ����.)
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ������� Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)",
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
#ifdef MU_CRYWOLF_BENEFIT_20051215
	// MVP ���� : ī���� ���� ������ ���� ( ����ĳ�� �����)
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE 
		&& g_iCrywolfApplyMvpBenefit	
	  )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}
#endif

#ifdef MODIFY_CHAOSMIX_RATE_FIX_20070304
	if( iMIX_SUCCESS_RATE > 80 )
	{
		iMIX_SUCCESS_RATE = 80;
	}
#endif // MODIFY_CHAOSMIX_RATE_FIX_20070304

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	if( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result	= 0xF0;
		gObj[iIndex].ChaosLock = FALSE;
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	
	INT iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iLEVEL-1];
	
#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219	// ī���� �ý��� �������� - ����ĳ�� ������� ����
	INT iChaosTaxMoney		= (INT)((INT64)iMIX_NEED_MONEY * (INT64)g_CastleSiegeSync.GetTaxRateChaos(iIndex) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney	= 0;

	iMIX_NEED_MONEY			= iMIX_NEED_MONEY + iChaosTaxMoney;
#endif

	if ( iMIX_NEED_MONEY < 0 ) {
		// �� ���� (��ü���� ->  ���� ���� Ȯ���� ����.)
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ������� Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
	if( (gObj[iIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		// ���� ���ڸ���. (0x86, Result: 0x0B �� �˸�)
		pMsg.Result	= 0x0B;
		DataSend( iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ������� Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", 
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name, 
			iLEVEL
			);
		return FALSE;
	}
	
	gObj[iIndex].Money -= iMIX_NEED_MONEY;
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130		// ī���� ���� ������ ���� - ����ĳ�� ������� ����
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
#endif
	GCMoneySend(iIndex, gObj[iIndex].Money);
	
	if (rand()%100 < iMIX_SUCCESS_RATE) {
		// Ȯ���� ����Ǿ� ���տ� �����ߴ�. => ����� ����.
		int iItemType  = MAKE_ITEMNUM(13, 18);
#ifdef FOR_BLOODCASTLE_TEST
		ItemSerialCreateSend(iIndex, 255, 0, 0, iItemType, iLEVEL, 0, 0, 0, 0);
#else
		ItemSerialCreateSend(iIndex, 255, 0, 0, iItemType, iLEVEL, 255, 0, 0, 0);
#endif

		LogAddTD("[������� Mix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   gObj[iIndex].AccountID, gObj[iIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
	}
	else {
		// ���տ� �����ߴ�.
#ifdef MODIFY_MIX_SYSTEM_20070518
		g_MixSystem.ChaosBoxInit(&gObj[iIndex]);
#endif
		GCUserChaosBoxSend(&gObj[iIndex]);
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[������� Mix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   gObj[iIndex].AccountID, gObj[iIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
		
#ifdef 	CHAOS_MIX_UPGRADE
		return FALSE;
#endif
	}
	
	gObjInventoryCommit(iIndex);
	
	return TRUE;
}


int CBloodCastle::CheckChoasMixItem	(int iIndex)		// �ش� ����ڰ� ������並 ���� ��� (��õ���Ǽ�, ���庻) �� ������ �ִ��� Ȯ�� (0 �̸� ���� / 1 ~ 7 �� �� ������ ���) / 8 �� �ٸ������� �߰� / 9�� ���� �ʸ��� / 10�� ȥ������ / 11�� �������� �ϳ� ���� / 12�� �̺�Ʈ�������� �ʹ� ���� / 13�� ���� ���ڸ���. / 14 �� �� ������ ����.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	INT iCHAOS_MIX_LEVEL = 0;					// ����� ������� ������ �� �ִ°� ?

	BOOL bIsChaosGemExist = FALSE;				// ȥ���� ������ �ִ��� ?
	BOOL bIsAngelKingPaperExist = FALSE;		// ��õ���Ǽ��� �ִ��� ?
	BOOL bIsBloodBoneExist = FALSE;				// ���庻�� �ִ��� ?
	BOOL bIsOtherItemExist = FALSE;				// �ٸ� �������� �����ϴ��� ?

	INT iEventItemCount = 0;

	INT iAngelKingPaperLevel = 0;
	INT iBloodBoneLevel = 0;

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	int iCharmOfLuckCount = 0;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	for( int i=0; i<MAX_CHAOSBOXITEMS; i++)
	{
		if( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// ȥ���� ����
				bIsChaosGemExist = TRUE;
			}
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 16) )
			{	// ��õ���� ��
				INT iITEM_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsAngelKingPaperExist = TRUE;

				iAngelKingPaperLevel = iITEM_LEVEL;
			}
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 17) )
			{	// ���� ��
				INT iITEM_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsBloodBoneExist = TRUE;

				iBloodBoneLevel = iITEM_LEVEL;
			}
#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else if( gObj[iIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += gObj[iIndex].pChaosBox[i].m_Durability;
			}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
			else
			{	// �̿��� �������� �÷��� �ִ�
				bIsOtherItemExist = TRUE;
			}
		}		
	}

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	gObj[iIndex].ChaosSuccessRate = iCharmOfLuckCount;
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	if (bIsOtherItemExist) {
		return 8;		// �ٸ� ������ �߰� -> ������ ī���� �ý������� �ѱ��.
	}

	if ((!bIsAngelKingPaperExist) && (!bIsBloodBoneExist)) {
		return 0;		// ����ĳ���� �ƴ� �ٸ� ������ �� �����Ƿ� �Ѿ��.
	}

	else if ((!bIsAngelKingPaperExist) || (!bIsBloodBoneExist)) {
		return 11;		// ��õ���Ǽ�, ���庻 �� �ϳ��� ����
	}

	if (iEventItemCount > 2) {
		return 12;		// �̺�Ʈ �������� �ʹ� ����.
	}

#ifdef ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111
	if( iCharmOfLuckCount > 10 )
	{
		return 15;
	}
#endif // ADD_PCS_CHAOSMIX_SUCCESS_RATE_UPGRADE_CHARM_20070111

	if (iAngelKingPaperLevel != iBloodBoneLevel) {
		return 9;		// ������ ������ ���� ����
	}
	else {
		if (!CHECK_LIMIT(iAngelKingPaperLevel-1, MAX_BLOODCASTLE_BRIDGE_COUNT)) 
			return 9;

		if (!CHECK_LIMIT(iBloodBoneLevel-1, MAX_BLOODCASTLE_BRIDGE_COUNT))
			return 9;
	}

#ifdef	FOR_BLOODCASTLE3
	// 2003.10.21 ��ġ ���Ŀ� ����Ǹ� �ʵ�
/*	if ( (iAngelKingPaperLevel == 1) && (iBloodBoneLevel == 1) ) {
		// ���� 1�� ���� ȥ���� �ִ°����� ����
		bIsChaosGemExist = TRUE;
	}
*/
#endif

	if (!bIsChaosGemExist) {
		return 10;		// ȥ�� ����
	}

	// ����ĳ�� ī���� ������ �� �� �ִ� �ּҷ��� üũ
#ifdef DARKLORD_WORK
	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA) {
#else
	if (gObj[iIndex].Class == CLASS_MAGUMSA) {
#endif
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA) {
			return 14;
		}
	}
	else {
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND) {
			return 14;
		}
	}

	if (bIsChaosGemExist && bIsAngelKingPaperExist && bIsBloodBoneExist) {
		// ��� ������ ��ġ�ϹǷ� ��õ���Ǽ� ������ ���� (���庻�� ������ �������)
		return iAngelKingPaperLevel;	
	}

	return 0;			// �׿��� ����ġ ���� �ٸ� ����
}


int CBloodCastle::CheckEnterItem (int iIndex)					// �ش� ����ڰ� ���� ĳ���� ������ �� �ִ� ������ (�������) �� �ִ��� Ȯ��
{
	INT iITEM_LEVEL = 0;

	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return 0;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 18) )		// ������䰡 �����Ѵٸ�
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;				// ������ ������ ��´�.
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// �������8 ����Ҽ� �ְ�.
				if (!CHECK_LIMIT(iITEM_LEVEL, MAX_BLOODCASTLE_BRIDGE_COUNT+1))
#else
				if (!CHECK_LIMIT(iITEM_LEVEL, 8))
#endif
					iITEM_LEVEL = 0;

				if (iITEM_LEVEL)	return iITEM_LEVEL;
			}
#ifdef ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
			// ���� ĳ�� ���� ����� �߰�
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM( 13, 47 ) )
			{
				iITEM_LEVEL = 10;
			}
#endif // ADD_PCS_EVENT_MAP_FREE_TICKET_20070110
		}
	}

	return iITEM_LEVEL;
}


int CBloodCastle::CheckQuestItem (int iIndex)					// �ش� ����ڰ� ���� ĳ���� ����Ʈ ������ �� �ϳ��� ������ �ִ��� Ȯ��
{
	INT iITEM_LEVEL = -1;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return -1;

	if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
		return -1;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
	int iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );	
	if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1)
#else
	if (m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL == -1)
#endif
		return -1;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return -1;

	for(int x=0; x<MAX_INVENTORY; x++)
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// ��õ�� �ø�� �����Ѵٸ�
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				if (gObj[iIndex].pInventory[x].m_Number == m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL) {
#else
				if (gObj[iIndex].pInventory[x].m_Number == m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL) {
#endif
					// ����� �ø��� ��ȣ���� ���ٸ�
					iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// ������ ������ ��´�.

					if ((iITEM_LEVEL < 0) || (iITEM_LEVEL > 2))
						iITEM_LEVEL = -1;

					break;
				}
			}
		}
	}

	return iITEM_LEVEL;
}


bool CBloodCastle::CheckWalk (int iIndex, int iMoveX, int iMoveY)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if(!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber) )
		return false;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return false;
	
	if (this->GetCurrentState(gObj[iIndex].MapNumber) == BLOODCASTLE_STATE_CLOSED) {	// ������ �����̰� (BLOODCASTLE_STATE_CLOSED -> �ٲ��� ��!!)
//		if (CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {								// ���� ĳ�� ���̶��
			
		BYTE attr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) )			// �������밡 �ƴ϶��
			return true;					// ���� ���ǿ� �´ٴ� ���̴�. (1>.�������, 2>.����ĳ����, 3>.��������ƴ�)
//		}
	}

	return false;
}


bool CBloodCastle::CheckCanEnter (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}


bool CBloodCastle::CheckCanParty (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}


bool CBloodCastle::CheckQuestItemSerial (int iBridgeIndex, CMapItem * lpItem)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1)
		return false;

	if( lpItem->IsItem() == TRUE )
	{
		if( lpItem->m_Type == MAKE_ITEMNUM(13, 19) )		// ��õ�� �ø�� �����Ѵٸ�
		{
			INT iITEM_LEVEL = lpItem->m_Level;				// ������ ������ ��´�.

			if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
				// ������ �ϴ� �´�.
				if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number) {
					// �ø������ �´ٸ� �ٷ� �̰��̴�.
					return true;
				}
			}
		}
	}

	return false;
}


bool CBloodCastle::CheckPlayStart (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}


#ifdef UPDATE_BLOODECASTLE_SCADULE_20080624
int CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	INT iREMAIN_MINUTE = 0;

	if (this->GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_CLOSED) 
	{
		iREMAIN_MINUTE = (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000) - m_iBC_TIME_MIN_OPEN) + 1;
	}
	else
	{
		list<BLOODCASTLE_STARTTIME>::iterator iter;
		BLOODCASTLE_STARTTIME	prevStartTime;
		BLOODCASTLE_STARTTIME	nextStartTime;
		
		DWORD	dwPrevTime	= 0;;		// ���� ���� �ð�(��)
		DWORD	dwNextTime	= 0;;		// ���� ���� �ð�(��)
		DWORD	dwCurTime	= 0;;		// ���� �ð�(��)
		
		struct tm *today;
		time_t ltime;
		

		time( &ltime );
		today = localtime( &ltime );
		dwCurTime	= today->tm_hour * 60 + today->tm_min;
		
		// ���� �̺�Ʈ ���� �ð��� �˻�
		prevStartTime = *m_LST_BLOODCASTLE_START_TIME.begin();
		for(iter = m_LST_BLOODCASTLE_START_TIME.begin(); iter != m_LST_BLOODCASTLE_START_TIME.end(); ++iter)
		{
			nextStartTime = *iter;
			
			// ���۽ð��� ������ ���
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
			if(iter == m_LST_BLOODCASTLE_START_TIME.end())
			{
				// ���� �� �ð�
				iter = m_LST_BLOODCASTLE_START_TIME.begin();
				nextStartTime = *iter;
			}
			
			// ���� ���۽ð��� �ٽ� ���
			dwNextTime  = nextStartTime.nHour * 60 + nextStartTime.nMin;
			
			// ���� ���۱��� ���� �ð� ���
			if(today->tm_hour <= nextStartTime.nHour && dwNextTime > dwCurTime)
			{
				// ���� �̺�Ʈ�� ����
				iREMAIN_MINUTE = (dwNextTime - dwCurTime) * 60 * 1000;
			}
			else
			{
				// ���� �̺�Ʈ�� ������
				iREMAIN_MINUTE = (24 * 60 - dwCurTime + dwNextTime) * 60 * 1000;
			}
			
			// ���� ���۱��� ���� �ð�(m_nRemainTime)�� ���½ð�(m_iOpenTime)���� ������ ���� �ð����� �ѱ��
			if( m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= m_iBC_TIME_MIN_OPEN * 60 * 1000 )
			{
				iter++;
				if(iter != m_LST_BLOODCASTLE_START_TIME.end())
				{
					nextStartTime = *iter;
				}
			}
			else
			{
				break;
			}
		}
		
		// ���� ��(min) ���
		int tmpTime = GetTickCount();
		iREMAIN_MINUTE = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC - (today->tm_sec * 1000) ;
	}

	return iREMAIN_MINUTE;
}
#else	// UPDATE_BLOODECASTLE_SCADULE_20080624
int CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	INT iREMAIN_MINUTE = 0;

	if (this->GetCurrentState(iBridgeIndex) == BLOODCASTLE_STATE_CLOSED) {
		iREMAIN_MINUTE = (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / (60 * 1000) - m_iBC_TIME_MIN_OPEN) + 1;
	}
	else {
		struct tm * today;
		time_t ltime;

		time( &ltime );
		today = localtime( &ltime );

		switch(g_iBloodCastle_StartHour) {
		case 2 :	// 2�ð����� �� ¦���� 30��
			{
#ifdef MODIFY_BLOODECASTLE_SCADULE_BUGFIX_20070710
				if( (today->tm_hour%2) == g_iBloodCastle_odd_Even_Hour)
#else
				if( !(today->tm_hour%2) )
#endif	
				{
					// ¦���ð�
					if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
						// 30���� �Ѱ�� -> 2�ð� ���� 30���� ���
						iREMAIN_MINUTE = (((60*2 + 30) * 60) - (today->tm_min * 60 + today->tm_sec)) / 60;
					}
					else {
						// 30���� ���� �ʳѰ�� -> �ð�����
						iREMAIN_MINUTE  = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
					}
				}
				else
				{
					// Ȧ�� �ð�����
					iREMAIN_MINUTE = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}

				if( iREMAIN_MINUTE < 0 )
				{
					iREMAIN_MINUTE += 60;
				}
			}
			break;
		case 1 :	// 1�ð����� �Ž� 30��
		default:
			{
				if ((today->tm_min * 60 + today->tm_sec) >= 30 * 60) {
					// �Ž� 30���� �Ѱ�� -> 1�ð� ���� 30���� ���
					iREMAIN_MINUTE = ((60*60) + (30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
				else {
					// �Ž� 30���� ���� �ʳѰ�� -> �ð�����
					iREMAIN_MINUTE = ((30*60) - (today->tm_min * 60 + today->tm_sec)) / 60;
				}

				if( iREMAIN_MINUTE < 0 )
				{
					iREMAIN_MINUTE += 60;
				}
			}
			break;
		}
	}

	return iREMAIN_MINUTE;
}
#endif	// UPDATE_BLOODECASTLE_SCADULE_20080624


void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	for( int n=0; n<MAX_MONSTER; n++)
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		if (gObj[n].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {	// ���� Ư�� ���� ĳ���� �ִٸ� ���°� ��ϵ��� ������� �� ����
#else
		if (gObj[n].MapNumber == (MAP_INDEX_BLOODCASTLE1 + iBridgeIndex)) {	// ���� Ư�� ���� ĳ���� �ִٸ� ���°� ��ϵ��� ������� �� ����
#endif

			if (!bClearCastleDoor && (gObj[n].Class == 131))				// ���࿡ ������ ������ ����� �ϸ� ���д�.
				continue;
			if (gObj[n].Class == NPC_ANGELKING)										// �׷��� ��õ��� ����
				continue;

			gObjDel(n);
		}
	}
}


void CBloodCastle::SetMonster(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {			// �ε����� ����� ����
		return;
	}

	// 1 . ����� ���� �켱 ��ƾ��� ���� ���� ���� ��
	SetMonsterKillCount(iBridgeIndex);

	// !!> �׽�Ʈ
//	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 3;
	

	// 2 . ������ �����Ѵ�.
	int nCastleDoorPosNum = m_BridgeData[iBridgeIndex].m_nCastleDoorPosNum;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���� Ÿ�� ������			
	WORD btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
#else
	BYTE btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
#endif
	BYTE btMapNumber = gMSetBase.m_Mp[nCastleDoorPosNum].m_MapNumber;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
	BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

	if (nCastleDoorPosNum != -1) {
		if (btMonsterType == 131) {											// Ȯ���� ������ �������� �߰�
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			int result = gObjAddMonster(btMapNumber);
#else
			int result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObjSetPosMonster(result, nCastleDoorPosNum);
				gObjSetMonster(result, btMonsterType);
				
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = 1;
				gObj[result].m_PosNum = -1;
				gObj[result].Live	= 1;
				gObj[result].DieRegen = 0;
				gObj[result].m_State	= OBJST_CREATE;
				gObj[result].MaxRegenTime = 0;
				gObj[result].MaxLife	= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life		= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
				m_BridgeData[btBloodCastleIndex].m_iDoorIndex	=	result;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				LogAddTD("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btBloodCastleIndex + 1, result);
#else
				LogAddTD("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btMapNumber - MAP_INDEX_BLOODCASTLE1+1, result);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif
			}
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
			else
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				LogAddTD("[Blood Castle] (%d) Create Door Fail", btBloodCastleIndex + 1);
#else
				LogAddTD("[Blood Castle] (%d) Create Door Fail", btMapNumber - MAP_INDEX_BLOODCASTLE1+1);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
			}
#endif
		}
		else {
			// ������ �ƴϴ�. (�ɰ��� ����)
#ifdef ADD_LOG_BLOODCASTLE_DOOR_20070321
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
			LogAddTD("[Blood Castle] (%d) Door monster type Error", btBloodCastleIndex + 1);
#else
			LogAddTD("[Blood Castle] (%d) Door monster type Error", btMapNumber - MAP_INDEX_BLOODCASTLE1+1);
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
#endif	
		}
	}


	// 3 . �Ϲ� ���͵��� �߰��Ѵ�.
	int result;	
	for( int n=0; n<gMSetBase.m_Count; n++)
	{
		if( CHECK_BLOODCASTLE(gMSetBase.m_Mp[n].m_MapNumber) )		// ���� ĳ���� ���� ���͸�
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���� Ÿ�� ������			
			WORD btMonsterType = gMSetBase.m_Mp[n].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[n].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btBloodCastleIndex != iBridgeIndex)					// �����Ǿ�� �� ���� �ƴϸ� ����					
				continue;

			if (btMonsterType == NPC_ANGELKING)						// ��õ��� �̸� �߰��ǹǷ� ���⼭ ���� �߰����� �ʴ´�.
				continue;

			if (btMonsterType == 131)								// ������ �̸� �߰��Ǿ����Ƿ� ���⼭ ���� �߰��ϸ� �ʵȴ�.
				continue;

			// ������ ������ ���鵵 ���⼭ �߰����� �ʴ´�.
			if ((btMonsterType == 89) ||							// �����ذ�1							
				(btMonsterType == 95) ||							// �����ذ�2
				(btMonsterType == 112) ||							// �����ذ�3
				(btMonsterType == 118) ||							// �����ذ�4
				(btMonsterType == 124) ||							// �����ذ�5
				(btMonsterType == 130)								// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// �����ذ�7
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 �������� ���� ����
				|| (btMonsterType == 433)							// �����ذ�8
#endif				
				)
				continue;

			if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3))	// ������ ������ ������ �μ����� �������� �Բ� �߰��ǹǷ� ���⼭ �߰����� �ʴ´�.
				continue;

//			if (MAP_ATTR_NONE != MapC[gObj[result].MapNumber].GetAttr(gObj[result].X, gObj[result].Y))
//				continue;

#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);
#else
			result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObj[result].m_PosNum   = n;
				gObj[result].X			= gMSetBase.m_Mp[n].m_X;
				gObj[result].Y			= gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber	= gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX			= gObj[result].X;
				gObj[result].TY			= gObj[result].Y;
				gObj[result].m_OldX		= gObj[result].X;
				gObj[result].m_OldY		= gObj[result].Y;
				gObj[result].Dir		= gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX     = (BYTE)gObj[result].X;
				gObj[result].StartY     = (BYTE)gObj[result].Y;
/*
				LogAddTD ("[BTEST] Pos:%d, X:%d, Y:%d, Type:%d, MapAttr:%d",
					n,
					gObj[result].X,
					gObj[result].Y,
					gObj[result].Class,
					MapC[gObj[result].MapNumber].GetAttr(gObj[result].X, gObj[result].Y)
					);
*/
				gObjSetMonster(result, btMonsterType);

				gObj[result].MaxRegenTime = m_iBC_MONSTER_REGEN;
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
//				gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = rand()%8;					// ���� Dir���� �ٸ���ȣ�� �־����Ƿ� ���⿡�� ���� ������ �ش�.
			}
		}
	}
}


void CBloodCastle::SetBossMonster(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {		// �ε����� ����� ����
		return;
	}

	// * . ���������� �߰��Ѵ�.
	int result;
	for (int x = 0 ; x < MAX_BLOODCASTLE_BOSS_COUNT ; x++) {
		if (m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x] != -1) {
			INT iBossPosNum = m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x];
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���� Ÿ�� ������			
			WORD btMonsterType = gMSetBase.m_Mp[iBossPosNum].m_Type;
#else
			BYTE btMonsterType = gMSetBase.m_Mp[iBossPosNum].m_Type;
#endif
			BYTE btMapNumber = gMSetBase.m_Mp[iBossPosNum].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
			BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			if (btBloodCastleIndex != iBridgeIndex)				// �����Ǿ�� �� ���� �ƴϸ� ����					
				continue;

			// Ȯ���� �������� �������� �߰�
			if ((btMonsterType == 89) ||							// �����ذ�1						
				(btMonsterType == 95) ||							// �����ذ�2
				(btMonsterType == 112) ||							// �����ذ�3
				(btMonsterType == 118) ||							// �����ذ�4
				(btMonsterType == 124) ||							// �����ذ�5
				(btMonsterType == 130)								// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314
				|| (btMonsterType == 143)							// �����ذ�7
#endif	
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 �������� �߰�
				|| (btMonsterType == 433)							// �����ذ�8
#endif				
				)
			{
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
				result = gObjAddMonster(btMapNumber);
#else
				result = gObjAdd();
#endif
				if( result >= 0 )
				{
					gObjSetPosMonster(result, iBossPosNum);
					gObjSetMonster(result, btMonsterType);

					gObj[result].MaxRegenTime = m_iBC_MONSTER_REGEN;
					gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
					gObj[result].m_ItemRate = m_iBC_NORMAL_ITEM_DROP;
					gObj[result].Dir = rand()%8;				// ���� Dir���� �ٸ���ȣ�� �־����Ƿ� ���⿡�� ���� ������ �ش�.
				}
			}
		}
	}
}


void CBloodCastle::SetSaintStatue(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {		// �ε����� ����� ����
		return;
	}

	// * . �����Ǽ����� �켱 �߰��Ѵ�.
	int nSaintStatuePosNum = m_BridgeData[iBridgeIndex].m_nSaintStatuePosNum;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���� Ÿ�� ������			
	WORD btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
#else
	BYTE btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
#endif
	BYTE btMapNumber = gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
	BYTE btBloodCastleIndex = GetBridgeIndexByMapNum( btMapNumber );
#else
	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

	if (nSaintStatuePosNum != -1) {
		if (CHECK_LIMIT(btMonsterType-BC_SAINT_STATUE_1, 3)) {			// Ȯ���� ������ ������ �������� �߰�
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			int result = gObjAddMonster(btMapNumber);
#else
			int result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObjSetPosMonster(result, nSaintStatuePosNum);
				gObjSetMonster(result, btMonsterType);
				
				gObj[result].Class = BC_SAINT_STATUE_1 + rand()%3;
				gObj[result].m_cBloodCastleIndex	= btBloodCastleIndex;
				gObj[result].m_ItemRate		= m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir			= 1;
				gObj[result].m_PosNum		= -1;
				gObj[result].Live			= 1;
				gObj[result].DieRegen		= 0;
				gObj[result].m_State		= OBJST_CREATE;
				gObj[result].MaxRegenTime	= 0;
				gObj[result].MaxLife		= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life			= m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

#ifdef EXTEND_LOG_SYSTEM_05_20060823
				LogAddTD("[Blood Castle] (%d) Saint Status be created.", iBridgeIndex+1 );
#endif // EXTEND_LOG_SYSTEM_05_20060823
			}
		}
		else {
			// ������ ������ �ƴϴ�. (�ɰ��� ����)
#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
			LogAddTD( "[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )"
				, iBridgeIndex + 1
				, btMapNumber
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
				, GetBridgeIndexByMapNum( btMapNumber )
#else
				, gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber - MAP_INDEX_BLOODCASTLE1
#endif
				, btMonsterType 
				);
#endif			
		}
	}
}




int CBloodCastle::LeaveUserBridge (int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	if (!CHECK_LIMIT(iBridgeSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return -1;

	INT iRET_VAL = -1;

	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);

	if (m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex) {
		iRET_VAL = iUserIndex;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_ALIVE;
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}


int	CBloodCastle::EnterUserBridge (int iBridgeIndex, int iUserIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
	if (!gObjIsConnected(iUserIndex))
		return -1;
#endif

	INT iRET_VAL = -1;

	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			// ������� �ߺ� ������ ���´�.
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex) {
				iRET_VAL = i;
				break;
			}
		}

		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
			iRET_VAL = i;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
			memcpy (m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID, gObj[iUserIndex].AccountID, MAX_IDSTRING);
			memcpy (m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName, gObj[iUserIndex].Name, MAX_IDSTRING);
			m_BridgeData[iBridgeIndex].m_UserData[i].m_szAccountID[MAX_IDSTRING] = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_szCharName[MAX_IDSTRING] = 0;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iCharClass = gObj[iUserIndex].Class;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleIndex = iBridgeIndex;
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iBloodCastleSubIndex = i;
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
			gObj[iUserIndex].m_bBloodCastleComplete = false;
#endif
			break;
		}
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	return iRET_VAL;
}


#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
int	CBloodCastle::LevelUp (int iIndex, int iAddExp, int iEventType)
#else
int	CBloodCastle::LevelUp (int iIndex, int iAddExp)
#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return 0;

	INT iLEFT_EXP = 0;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ����ĳ�� MasterLevelUp ó��
	// �����ͷ����� ��������.
	if( g_MasterLevelSystem.MasterLevelUp( &gObj[iIndex], iAddExp, true ) )
		return 0;

	// �� ������ ����ġ �й�	
	gObjSetExpPetItem( iIndex, iAddExp );
#endif // MASTER_LEVEL_UP_SYSTEM_20070912	

#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %uz %d MonsterIndex : %d, EventType : %d", 
		gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].X,
		gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level, gObj[iIndex].Experience, iAddExp,
		0, iEventType );
#endif
	
	// �ִ� ������ MAX_LEVEL�� �Ѵ�.
	if( gObj[iIndex].Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iIndex].m_Index, 1);
		return 0;
	}
	
	if( gObj[iIndex].Experience + iAddExp < gObj[iIndex].NextExp ) {
		gObj[iIndex].Experience += iAddExp;
	}
	else {
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;

		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level += 1;
#ifdef DARKLORD_WORK
		if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA)
#else
		if(gObj[iIndex].Class == CLASS_MAGUMSA )
#endif
    		gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
		else
    		gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iNormal_Points;
		
#ifdef SECOND_QUEST	
		if( gObj[iIndex].PlusStatQuestClear )
		{	// +1���Ⱦ� ����Ʈ ������ �߰� ���� 
			gObj[iIndex].LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}
#endif		
			
		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;

		if(g_GlobalConfig.m_bLimitLife && gObj[iIndex].MaxLife > g_GlobalConfig.m_iLimitLife)
			gObj[iIndex].MaxLife = g_GlobalConfig.m_iLimitLife;

		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;	
		gObjNextExpCal(&gObj[iIndex]);

#ifdef NEW_FORSKYLAND2
		gObjSetBP(gObj[iIndex].m_Index);
#endif
	

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
		GCLevelUpMsgSend(gObj[iIndex].m_Index);
#else
		short AddPoint		= 0;
		short MaxAddPoint	= 0;
		gObjGetStatPointState(gObj[iIndex].m_Index, AddPoint, MaxAddPoint);
		GCLevelUpMsgSend(gObj[iIndex].m_Index, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
						 (WORD)(gObj[iIndex].MaxLife+gObj[iIndex].AddLife), 
						 (WORD)(gObj[iIndex].MaxMana+gObj[iIndex].AddMana), 
						 gObj[iIndex].MaxBP+gObj[iIndex].AddBP,
						 AddPoint, MaxAddPoint);
#endif // ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712

		gObjCalcMaxLifePower(gObj[iIndex].m_Index);	// �ִ� ������� �ٽ� ����Ѵ�
		
		LogAddTD(lMsg.Get(520), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);
		
#ifdef ADD_LOG_400LEVELUP_PARTY_20070628	// 400���� �޼��� ���� �ִ� ��Ƽ���� �α� �߰�
		if(gObj[iIndex].Level == 400 && gObj[iIndex].PartyNumber >=0)
		{
			int partynum = gObj[iIndex].PartyNumber;
			char partyLog[256];
			int number;
			sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", gObj[iIndex].AccountID, gObj[iIndex].Name);
			
			for( int n=0; n<MAX_PARTYUSER; n++)
			{
				number = gParty.m_PartyS[partynum].Number[n];
				if( number >= 0 )
				{
					int noffSet = strlen(partyLog);
					sprintf(partyLog + noffSet, ",(%s)(%s) ",gObj[number].AccountID, gObj[number].Name);								
				}
			}
			
			LogAddTD(partyLog);
		}
#endif
	}

	// ������ �ڵ� �����Ѵ�.
	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index);
	//LogAdd("ĳ���� ���� �޽��� ����");

	return iLEFT_EXP;
}



VOID CBloodCastle::CheckUsersOnConnect (							// ī���� ĳ���� ������ ����ڰ� ���� ������ üũ
			INT iBridgeIndex
			)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;
	
	EnterCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
	
	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			// Ư�� ����ڰ� ���� ���� �ƴϸ� ������ ����.
			if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex))
			{
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	LeaveCriticalSection(& m_BridgeData[iBridgeIndex].m_critUserData);
}


bool CBloodCastle::AddExperience (int iIndex, int iEXP)
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if (!CHECK_LIMIT(gObj[iIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (!CHECK_LIMIT(gObj[iIndex].m_cBloodCastleSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return false;

	if (iEXP > 0) {		// �ϴ� ���� ����ġ�� ���ʿ� �� �����Ѵ�.
		m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}


void CBloodCastle::BlockCastleEntrance(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleEntranceMapXY[iBridgeIndex][0] ; i <= g_btCastleEntranceMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleEntranceMapXY[iBridgeIndex][1] ; j <= g_btCastleEntranceMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#endif
		}
	}
}


void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleEntranceMapXY[iBridgeIndex][0] ; i <= g_btCastleEntranceMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleEntranceMapXY[iBridgeIndex][1] ; j <= g_btCastleEntranceMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#endif
		}
	}
}


void CBloodCastle::BlockCastleBridge(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleBridgeMapXY[iBridgeIndex][0] ; i <= g_btCastleBridgeMapXY[iBridgeIndex][2] ; i++) {
		for (INT j = g_btCastleBridgeMapXY[iBridgeIndex][1] ; j <= g_btCastleBridgeMapXY[iBridgeIndex][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_HOLLOW;
#endif
		}
	}
}


void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = g_btCastleBridgeMapXY[iBridgeIndex][0] ; i <= g_btCastleBridgeMapXY[iBridgeIndex][2] ; i++) 
	{
		for (INT j = g_btCastleBridgeMapXY[iBridgeIndex][1] ; j <= g_btCastleBridgeMapXY[iBridgeIndex][3] ; j++) 
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
			MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#else
			MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_HOLLOW);
#endif

#ifdef EXTEND_LOG_SYSTEM_05_20060823
			LogAddTD("[Blood Castle] (%d) Castle Door Subsist", iBridgeIndex+1);
#endif // EXTEND_LOG_SYSTEM_05_20060823

#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
			LogAddTD( "[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1 );
#endif
		}
	}
}


void CBloodCastle::BlockCastleDoor(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT x = 0 ; x < 3 ; x++) {
		for (INT i = g_btCastleDoorMapXY[iBridgeIndex][x][0] ; i <= g_btCastleDoorMapXY[iBridgeIndex][x][2] ; i++) {
			for (INT j = g_btCastleDoorMapXY[iBridgeIndex][x][1] ; j <= g_btCastleDoorMapXY[iBridgeIndex][x][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
				int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#else
				MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
#endif
			}
		}
	}
}


void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT x = 0 ; x < 3 ; x++) {
		for (INT i = g_btCastleDoorMapXY[iBridgeIndex][x][0] ; i <= g_btCastleDoorMapXY[iBridgeIndex][x][2] ; i++) {
			for (INT j = g_btCastleDoorMapXY[iBridgeIndex][x][1] ; j <= g_btCastleDoorMapXY[iBridgeIndex][x][3] ; j++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
				int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
				MapC[iMapNum].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#else
				MapC[MAP_INDEX_BLOODCASTLE1 + iBridgeIndex].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
#endif
			}
		}
	}
}


void CBloodCastle::BlockSector(
	INT iMAP_NUM,
	INT iSTART_X, 
	INT iSTART_Y, 
	INT iEND_X, 
	INT iEND_Y
	)
{
	if(!CHECK_BLOODCASTLE(iMAP_NUM))
		return;

	for (INT i = iSTART_X ; i <= iEND_X ; i++) {
		for (INT j = iSTART_Y ; j <= iEND_Y ; j++) {
			MapC[iMAP_NUM].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] |= (BYTE) MAP_ATTR_BLOCK;
		}
	}
}


void CBloodCastle::ReleaseSector(						
	INT iMAP_NUM,
	INT iSTART_X, 
	INT iSTART_Y, 
	INT iEND_X, 
	INT iEND_Y
	)
{
	if(!CHECK_BLOODCASTLE(iMAP_NUM))
		return;

	for (INT i = iSTART_X ; i <= iEND_X ; i++) {
		for (INT j = iSTART_Y ; j <= iEND_Y ; j++) {
			MapC[iMAP_NUM].m_attrbuf[(j*MAX_TERRAIN_SIZE)+i] &= ~((BYTE) MAP_ATTR_BLOCK);
		}
	}
}


void CBloodCastle::SendCastleEntranceBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = MAP_ATTR_BLOCK;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleEntranceMapXY[iBridgeIndex][0];
	lpMsgBody[0].btY = g_btCastleEntranceMapXY[iBridgeIndex][1];
	lpMsgBody[1].btX = g_btCastleEntranceMapXY[iBridgeIndex][2];
	lpMsgBody[1].btY = g_btCastleEntranceMapXY[iBridgeIndex][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) {
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) {
#endif
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);
		}
	}
}


void CBloodCastle::SendCastleBridgeBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = MAP_ATTR_HOLLOW;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleBridgeMapXY[iBridgeIndex][0];
	lpMsgBody[0].btY = g_btCastleBridgeMapXY[iBridgeIndex][1];
	lpMsgBody[1].btX = g_btCastleBridgeMapXY[iBridgeIndex][2];
	lpMsgBody[1].btY = g_btCastleBridgeMapXY[iBridgeIndex][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) 
	{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) 
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) 
#endif
		{
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);

#ifdef ADD_LOG_FOR_CASTLE_BUG_20060911
			LogAddTD( "[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)", 
				iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY );
#endif
		}
	}
}


void CBloodCastle::SendCastleDoorBlockInfo (int iBridgeIndex, bool bLive)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	char cTEMP_BUF [256];
	LPPMSG_SETMAPATTR_COUNT lpMsg = (LPPMSG_SETMAPATTR_COUNT) cTEMP_BUF;
	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT) + sizeof(PMSG_SETMAPATTR) * 2 * 3);

	LPPMSG_SETMAPATTR lpMsgBody = (LPPMSG_SETMAPATTR) (cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = MAP_ATTR_BLOCK;
	(bLive) ? lpMsg->btMapSetType = 0 : lpMsg->btMapSetType = 1;

	lpMsgBody[0].btX = g_btCastleDoorMapXY[iBridgeIndex][0][0];
	lpMsgBody[0].btY = g_btCastleDoorMapXY[iBridgeIndex][0][1];
	lpMsgBody[1].btX = g_btCastleDoorMapXY[iBridgeIndex][0][2];
	lpMsgBody[1].btY = g_btCastleDoorMapXY[iBridgeIndex][0][3];
	lpMsgBody[2].btX = g_btCastleDoorMapXY[iBridgeIndex][1][0];
	lpMsgBody[2].btY = g_btCastleDoorMapXY[iBridgeIndex][1][1];
	lpMsgBody[3].btX = g_btCastleDoorMapXY[iBridgeIndex][1][2];
	lpMsgBody[3].btY = g_btCastleDoorMapXY[iBridgeIndex][1][3];
	lpMsgBody[4].btX = g_btCastleDoorMapXY[iBridgeIndex][2][0];
	lpMsgBody[4].btY = g_btCastleDoorMapXY[iBridgeIndex][2][1];
	lpMsgBody[5].btX = g_btCastleDoorMapXY[iBridgeIndex][2][2];
	lpMsgBody[5].btY = g_btCastleDoorMapXY[iBridgeIndex][2][3];

	for (int i = MAX_MONSTER ; i < MAX_OBJECT ; i++) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int iMapNum = GetMapNumByBCBridge( iBridgeIndex );
		if ((gObj[i].MapNumber == iMapNum) && (gObj[i].Connected > 2)) {
#else
		if ((gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) && (gObj[i].Connected > 2)) {
#endif
			DataSend (i, (unsigned char*) lpMsg, lpMsg->h.size);
		}
	}
}


void CBloodCastle::SendNoticeMessage (int iBridgeIndex, char * lpszMSG)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	PMSG_NOTICE	pNotice;

#ifdef MODIFY_NOTICE_20040325
	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG );
#else
	pNotice.type = 0;
	strcpy(pNotice.Notice, lpszMSG);
	PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
}


void CBloodCastle::SendNoticeScore (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	PMSG_NOTICE	pNotice;
		
	pNotice.type = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1)) {

#ifdef MODIFY_NOTICE_20040325
				TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1164), iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
#else
				wsprintf(pNotice.Notice, lMsg.Get(1164), iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);	// "�� %d ���� ĳ�������� ����� �� ȹ�� ����ġ�� %d �Դϴ�."
				PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));
#endif
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}


void CBloodCastle::SendNoticeState (int iBridgeIndex, int iPlayState)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

//	return;
	PMSG_STATEBLOODCASTLE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
	pMsg.btPlayState		= iPlayState;
	pMsg.wRemainSec			= m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;

	if (iPlayState == BC_STATE_PLAY_BOSS) {
		pMsg.wMaxKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else {
		pMsg.wMaxKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster	= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon	= m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum		= m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;	// �̷��� �ؾ� �����ȣ�� ����� �´´�.

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
				DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}


bool CBloodCastle::CheckUserBridgeMember( int iBridgeIndex,	int iIndex )
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return false;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex) {
			return true;
		}
	}

	return false;
}


int CBloodCastle::GetAliveUserTotalEXP (INT iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return 0;

	int iRET_EXP = 0;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if ((m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2)) {
			if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
				iRET_EXP += m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
			}
		}
	}

	return iRET_EXP;
}


void CBloodCastle::SearchUserDeleteQuestItem (int iIndex)						// ����ڸ� �˻��Ͽ� ����Ʈ �������� �����.
{
	if (!gObjIsConnected (iIndex))
		return;

#ifdef PERSONAL_SHOP_20040113	// ���� ĳ������ ���λ��� �������� ����Ʈ �������� �˻��Ͽ� �����.
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// ��õ�� �ø�� �����Ѵٸ�
			{
				INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// ������ ������ ��´�.

				if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
					// �̰��� Ȯ���ϹǷ� �������� �����.
					gObjInventoryItemSet(iIndex, x, 0xFF);
					gObjInventoryDeleteItem(iIndex, x);
					GCInventoryItemDeleteSend(iIndex, x, 1);

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%u]",
#else
					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
						gObj[iIndex].AccountID,
						gObj[iIndex].Name,
						iITEM_LEVEL,
						gObj[iIndex].pInventory[x].m_Number
						);
				}
			}
		}
	}
}


void CBloodCastle::SearchUserDropQuestItem (int iIndex)							// ����ڸ� �˻��Ͽ� ����Ʈ �������� ��ӽ�Ų��.
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return;

#ifdef PERSONAL_SHOP_20040113	// ���� ĳ������ ���λ��� �������� ����Ʈ �������� �˻��Ͽ� ����Ѵ�.
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// ��õ�� �ø�� �����Ѵٸ�
			{
				INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// ������ ������ ��´�.

				if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
					// �̰��� Ȯ���ϹǷ� �������� ����Ѵ�.
					PMSG_ITEMTHROW	pDropItem;
					pDropItem.Ipos  = x;
					pDropItem.px    = (BYTE)gObj[iIndex].X;
					pDropItem.py	= (BYTE)gObj[iIndex].Y;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
					int iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );
					DropItemDirectly( iBridgeIndex, gObj[iIndex].m_Index, MAKE_ITEMNUM(13, 19), x );
#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728
					DropItemDirectly(gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1, gObj[iIndex].m_Index, MAKE_ITEMNUM(13, 19), x);
#else
					CGItemDropRequest((LPPMSG_ITEMTHROW)&pDropItem, gObj[iIndex].m_Index, 1);		// �׾ ���������� �αװ� ���� ���̴�.
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

					// �׸��� �ø������ ���ٸ� ���� �ٸ��� ���⸦ ��������� ���°��̴�.
					if (CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
						INT iBridgeIndex = GetBridgeIndexByMapNum( gObj[iIndex].MapNumber );
#else
						INT iBridgeIndex = gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

						if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1) {
							if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number) {
								m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1; 
							}
						}

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u-%u]",
#else
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d-%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							gObj[iIndex].AccountID,
							gObj[iIndex].Name,
							iITEM_LEVEL,
							m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL,
							gObj[iIndex].pInventory[x].m_Number
							);
					}
					else {
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u]",
#else
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d]",
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
							gObj[iIndex].AccountID,
							gObj[iIndex].Name,
							iITEM_LEVEL,
							gObj[iIndex].pInventory[x].m_Number
							);
					}
				}
			}
		}
	}
}


void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex;
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex;

	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (!CHECK_LIMIT(iBridgeSubIndex, MAX_BLOODCASTLE_USER_COUNT))
		return;

/*	if( (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) || (gObj[iIndex].Connected <= 2))
	{
		m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_DEAD;
		return;
	}
*/

	switch (iState) {
	case BC_USER_ALIVE :			// ��ǻ� ���� ������ ����.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_ALIVE;
		}
		break;
	case BC_USER_DEAD :				// ����Ʈ ������ ����� -> gObjUserDie() ����
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_DEAD;
		}
		break;
	case BC_USER_WINNER :			// ���ڶ�� ��Ƽ������ ã�Ƽ� BC_USER_WINNER_PARTY ���¸� �ش�.
		{
			if( gObj[iIndex].PartyNumber >= 0 )
			{
//				int iPartyCount = gParty.m_PartyS[gObj[iIndex].PartyNumber].Count;

				for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
					if (i == iBridgeSubIndex) {
						// ����ڴ� �Ѿ��.
						continue;
					}
					else {
#ifdef MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
						if( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 ) continue;							
#endif	// MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
						
						if (gObj[iIndex].PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber) {
							// ����ڿ� ���� ��Ƽ���̶�� BC_USER_WINNER_PARTY ���¸� �ش�.
							if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber))
								m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
							else
								m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
				}
			}

			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER;
			m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
		}
		break;
	case BC_USER_WINNER_PARTY_ALIVE :		// ��ǻ� ���� ������ ����.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
		}
		break;
	case BC_USER_WINNER_PARTY_DEAD :		// ��ǻ� ���� ������ ����.
		{
			m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
		}
		break;
	}
}


void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
#ifdef BLOODCASTLE_EVENT_5TH_20050531		// ������� ���¸� ���������� üũ ������ �ش�.
	FixUsersPlayStateWin(iBridgeIndex);
#endif

	CHAR szNOTIFY_MSG [256];
	INT iREWARD_EXP = 0;
	INT iREWARD_ZEN = 0;
	INT iREWARD_SCR = 0;

#ifdef BLOODCASTLE_EVENT_3RD_20040401
	INT iLEFT_TIME = m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
#endif
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	INT iALIVE_PARTYCOUNT = GetWinnerPartyCompleteCount(iBridgeIndex);
	INT iADD_PARTYPOINT = GetWinnerPartyCompletePoint(iBridgeIndex);

	LogAddTD("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d", 
		iBridgeIndex + 1,
		iALIVE_PARTYCOUNT,
		iADD_PARTYPOINT
		);
#endif

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;
	
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (m_BridgeData[iBridgeIndex].m_bBC_REWARDED)
		return;

	if (gObj[iIndex].Connected > 2) {
		gObj[iIndex].Name[10] = '\0';
		wsprintf(szNOTIFY_MSG, lMsg.Get(1165), gObj[iIndex].Name);		// "%s �Կ� ���� ����ĳ�� ����Ʈ�� �����Ͽ����ϴ�."
	}
	else {
		wsprintf(szNOTIFY_MSG, lMsg.Get(1166));							// "����ĳ�� ����Ʈ�� �����Ͽ����ϴ�."
	}

	if (m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1) {			// �̼��� �����ߴٸ�
		// ����ִ� ������� ����ġ�� ������ �̸� ���Ѵ�.
		INT iALIVE_USER_TOTAL_EXP = GetAliveUserTotalEXP(iBridgeIndex);
		LogAddTD("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d",
			iBridgeIndex+1,
			iALIVE_USER_TOTAL_EXP
			);


		for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1)
				continue;
			
			// ������ �ʵǾ� �ִٸ� ������ ���޴´�.
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
				SendRewardScore (&m_BridgeData[iBridgeIndex].m_UserData[i], g_iBC_EventScore_Fail[iBridgeIndex], 0);
#endif
				continue;
			}

			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
				) 
			{
				// ������ �´� ����ڸ� üũ
				continue;
			}

			// �켱 ���ϸ޽����� ������
			
#ifdef MODIFY_NOTICE_20040325
			PMSG_NOTICE	pNotice;
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_PRIORITY_LV1, szNOTIFY_MSG );
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_BLUE);
			TNotice::SendNoticeToUser( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, &pNotice );
#else
			GCServerMsgStringSend(szNOTIFY_MSG, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, 0);
#endif

			// ������� ����, �����ı�, ����Ʈ ���� ���ο� ���� �߰����� ����ġ�� �ش�.
			// 1 . ����
			INT iEXTRA_EXP = 0;
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) 
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR];
#if TESTSERVER == 1 
				LogAddTD("[�߰�����ġ - ����] [%s][%s] �����ı� : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR]
					);
#endif
			}
#ifdef FOR_BLOODCASTLE3
			else if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) {
				// ��� ���� ������ ���� �ʾ����� ��Ƹ� �ִٸ� ����ġ�� 50%�� �޴´�.
				if (m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak)
					iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR] / 2;
			}
#endif
			// 2 . ����
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party)
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE];
#if TESTSERVER == 1 
				LogAddTD("[�߰�����ġ - ����] [%s][%s] �����ı� : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE]
					);
#endif
			}
			// 3 . ����Ʈ����
			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party)
				||
				(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
				)
			{
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_WIN_QUEST];
#if TESTSERVER == 1 
				LogAddTD("[�߰�����ġ - ����] [%s][%s] ����Ʈ���� : %d",
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
					g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_WIN_QUEST]
					);
#endif
			}


			// ���������� �ؼ� ��� ���°� �����Ǿ����Ƿ� ���⼭�� ������ ���鼭 ���� �ϸ� �ȴ�.
			switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
			case BC_USER_ALIVE :		
				{
#ifdef FOR_BLOODCASTLE2	
					// ����Ʈ �Ϸ��� �߰� ����ġ
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// ����ġ ����
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_ALIVE] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// �� ����
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_LOSER]);
					// ���� ����
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_ALIVE];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "����ġ %d �� %d ��, %d ����Ʈ�� �߰��� ȹ���Ͽ����ϴ�."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/
					// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_ALIVE,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_DEAD :				
				{
#ifdef FOR_BLOODCASTLE2	
					// ����Ʈ �Ϸ��� �߰� ����ġ
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// ����ġ ����
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_LOSER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// �� ����
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_LOSER]);
					// ���� ����
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_DEAD];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "����ġ %d �� %d ��, %d ����Ʈ�� �߰��� ȹ���Ͽ����ϴ�."
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/
					// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_DEAD,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER :			
				{
#ifdef FOR_BLOODCASTLE2	
					// ����Ʈ �Ϸ��� �߰� ����ġ
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// ����ġ ����
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// �� ����
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// ���� ����
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER];

#ifdef BLOODCASTLE_EVENT_5TH_20050531
					iREWARD_SCR += iADD_PARTYPOINT;
#endif

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "����ġ %d �� %d ��, %d ����Ʈ�� �߰��� ȹ���Ͽ����ϴ�."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// ȥ�� ���� (iLootTime = 5 * 60 * 1000)
					if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
						// ������ ����� �ϳ��� �޴´�.
#ifdef FOR_CHINA		// �߱��� �ϳ� �� �ش�
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);
#endif						
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);
					}

					// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER_PARTY_ALIVE :
				{
#ifdef FOR_BLOODCASTLE2	
					// ����Ʈ �Ϸ��� �߰� ����ġ
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// ����ġ ����
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// �� ����
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// ���� ����
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER_PARTY_ALIVE];

#ifdef BLOODCASTLE_EVENT_5TH_20050531
					iREWARD_SCR += iADD_PARTYPOINT;
#endif

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "����ġ %d �� %d ��, %d ����Ʈ�� �߰��� ȹ���Ͽ����ϴ�."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// ȥ�� ���� (iLootTime = 5 * 60 * 1000)
					if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber))
						DropChaosGem(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,iBridgeIndex);

					// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531


#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER_PARTY_ALIVE,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			case BC_USER_WINNER_PARTY_DEAD :		
				{
#ifdef FOR_BLOODCASTLE2	
					// ����Ʈ �Ϸ��� �߰� ����ġ
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : %d", iREWARD_EXP);
	#endif
					iEXTRA_EXP += (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000) * g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_PER_SEC];
	#if TESTSERVER == 1 
					LogAddTD("[������] ����Ʈ �Ϸ�(�Ϸ���) : Exp:(%d) RemainSec:(%d)", iREWARD_EXP, m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000);
	#endif
#endif
					// ����ġ ����
//					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iALIVE_USER_TOTAL_EXP * g_iQuestWinExpendEXP[iBridgeIndex][BC_REWARD_WINNER] / 100 + iEXTRA_EXP);
					iREWARD_EXP = CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);
					// �� ����
					iREWARD_ZEN = CalcSendRewardZEN (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iQuestWinExpendZEN[iBridgeIndex][BC_REWARD_WINNER]);
					// ���� ����
					iREWARD_SCR = g_iBC_EventScore[iBridgeIndex][BC_USER_WINNER_PARTY_DEAD];

/*					wsprintf(szNOTIFY_MSG2, lMsg.Get(1167),	// "����ġ %d �� %d ��, %d ����Ʈ�� �߰��� ȹ���Ͽ����ϴ�."
						iREWARD_EXP, 
						iREWARD_ZEN,
						iREWARD_SCR
						);
*/					
					// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME);
#else
					SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", 
#else
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d)", 
#endif
						iBridgeIndex+1,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						BC_USER_WINNER_PARTY_DEAD,
						gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP,
						iREWARD_ZEN,
						iREWARD_SCR
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
						,iLEFT_TIME
						,gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
#endif
						);
				}
				break;
			}

			// ȹ���� ����ġ�� ���� �˷��ش�.
			PMSG_DEVILSQUARERESULT pMsg;
			pMsg.MyRank = 1;	// ����
			pMsg.Count = 255;
			memcpy(pMsg.Score[0].Name, gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, sizeof(pMsg.Score[0].Name));


			pMsg.Score[0].BonusExp		= iREWARD_EXP;


			pMsg.Score[0].BonusZen		= iREWARD_ZEN;
			pMsg.Score[0].TotalScore	= iREWARD_SCR;
			PHeadSetB((LPBYTE)&pMsg, 0x93, sizeof(PBMSG_HEAD) + 2 + sizeof(DevilSquareScoreInfo) * 2);

			DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
//			SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
		}

		// �̹� ������ �̷�������� ����Ѵ�.
		m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}


void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
#ifdef BLOODCASTLE_EVENT_5TH_20050531		// ������� ���¸� ���������� üũ ������ �ش�.
	FixUsersPlayStateFail(iBridgeIndex);
#endif

	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	INT iUserWhoGotUltimateWeapon = -1;
	if ( (iUserWhoGotUltimateWeapon = GetWhoGotUltimateWeapon(iBridgeIndex)) != -1 ) {
		if (CHECK_LIMIT(iUserWhoGotUltimateWeapon, MAX_OBJECT)) {
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out", 
				iBridgeIndex+1,
				gObj[iUserWhoGotUltimateWeapon].AccountID,
				gObj[iUserWhoGotUltimateWeapon].Name,
				m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER				
				);
		}
	}

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1)
			continue;

		// ������ �ʵǾ� �ִٸ� ������ ���޴´�.
		if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604
			SendRewardScore (&m_BridgeData[iBridgeIndex].m_UserData[i], g_iBC_EventScore_Fail[iBridgeIndex], 0);
#endif
			continue;
		}

		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1))
			continue;

		// ������� ����, �����ı� ���ο� ���� �߰����� ����ġ�� �ش�.
		// 1 . ����
		INT iEXTRA_EXP = 0;
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party)
			||
			(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			)
		{
			iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR];
#if TESTSERVER == 1
			LogAddTD("[�߰�����ġ - ����] [%s][%s] �����ı� : %d",
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR]
				);
#endif
		}
#ifdef FOR_BLOODCASTLE3
		else if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) {
			// ��� ���� ������ ���� �ʾ����� ��Ƹ� �ִٸ� ����ġ�� 50%�� �޴´�.
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak)
				iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_DOOR] / 2;
		}
#endif

		// 2 . ����
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1) &&
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party)
			||
			(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			)
		{
			iEXTRA_EXP += g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE];
#if TESTSERVER == 1 
			LogAddTD("[�߰�����ġ - ����] [%s][%s] �����ı� : %d",
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				g_iBC_Add_Exp[iBridgeIndex][ADD_EXP_KILL_STATUE]
				);
#endif
		}
		
		// ����ġ ����
		CalcSendRewardEXP (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iEXTRA_EXP);

		// �ٸ� ������ ��ŷ������ ����
#ifdef BLOODCASTLE_EVENT_5TH_20050531
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);
#else
#ifdef BLOODCASTLE_EVENT_3RD_20040401
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex], 0);
#else
		SendRewardScore(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, g_iBC_EventScore_Fail[iBridgeIndex]);
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)", 
#else
		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d)",
#endif
			iBridgeIndex+1,
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
			g_iBC_EventScore_Fail[iBridgeIndex],
			iEXTRA_EXP
#ifdef BLOODCASTLE_EVENT_4TH_20040531		// Ÿ�Ӿ��� ���� �α��߰�
			,0
#endif
			);


		// ȹ���� ����ġ�� ���� �˷��ش�.
		PMSG_DEVILSQUARERESULT pMsg;
		pMsg.MyRank = 0;	// ����
		pMsg.Count = 255;
		memcpy(pMsg.Score[0].Name, gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, sizeof(pMsg.Score[0].Name));
		pMsg.Score[0].BonusExp		= iEXTRA_EXP;
		pMsg.Score[0].BonusZen		= 0;
		pMsg.Score[0].TotalScore	= g_iBC_EventScore_Fail[iBridgeIndex];
		PHeadSetB((LPBYTE)&pMsg, 0x93, sizeof(PBMSG_HEAD) + 2 + sizeof(DevilSquareScoreInfo) * 2);

		// �ٸ��� ���� ����ڸ� �������� ���� ĳ���ʿ� ��� ��Ƹ� �ִٸ� �޽����� �����ش�.
		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1)) {
			// �ٸ� ������ ����� �ε����� ��ġ�� ��� ����
			DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			LogAddTD("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)",
				iBridgeIndex+1,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState,
				gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber
				);
		}

//		SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
	}	
}


int CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP) 
{
	if (iEXP <= 0)
		return 0;

#ifdef BUGFIX_GAIN_EXP_20071210
	INT64 iRET_EXP = 0;
	INT64 iCAL_EXP = iEXP;
#else
	INT iRET_EXP = 0;
	INT iCAL_EXP = iEXP;
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
	// MVP ���� : ����ġ ȹ�� ���� 
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
		&& g_iCrywolfApplyMvpPenalty	
	  )
	{
		iCAL_EXP = iCAL_EXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
	}
#endif
	
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	#ifdef ADD_PCS_MARK_OF_EXP_20070205
		#ifdef MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	if( gObj[iIndex].m_wExprienceRate == 0 )
	{
		iEXP = 0;
		iCAL_EXP = 0;
	}
		#else
	if( gObj[iIndex].m_wExprienceRate > 0 )
	{
		iEXP = (int)(iEXP * ((float)gObj[iIndex].m_wExprienceRate / 100 ));
	}
		#endif // MODIFY_MARKITEM_EFFECT_REMOVE_IN_EVENTMAP_20070328
	#endif // ADD_PCS_MARK_OF_EXP_20070205
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return 0;

	if (gObj[iIndex].Connected < 3)
		return 0;

	iRET_EXP = iCAL_EXP;

	if( gObj[iIndex].Type == OBJTYPE_CHARACTER ) {
		while (iCAL_EXP > 0) {
			if (iCAL_EXP > 0) {
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱�����> . ��ü ���� ������ ��� Ȯ���� ������.
				if (g_bChnPlayTimeLimitOn)
				{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
					if( gObj[iIndex].m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
					{
						if (gObj[iIndex].m_iPlayTimeLimit >= 5*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
						}
						else if (gObj[iIndex].m_iPlayTimeLimit >= 3*60*60) {
							iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
						}
					}
	#else
					if (gObj[iIndex].m_iPlayTimeLimit >= 5*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iIndex].m_iPlayTimeLimit >= 3*60*60) {
						iCAL_EXP = iCAL_EXP * g_iChnPlayTimeLimitLevel1Exp / 100;
					}
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
				}
#endif
				
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626 // ��Ʈ�� : ����ĳ�� ����ġ ȹ�淮 ����
				if (g_bVtmPlayTimeLimitOn) 
				{
					if (gObj[iIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60 ) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel2Exp / 100;
					}
					else if (gObj[iIndex].m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
					{
						iCAL_EXP = iCAL_EXP * g_iVtmPlayTimeLimitLevel1Exp / 100;
					}
				}
#endif

#ifdef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
				CheckItemOptForGetExpEx(&gObj[iIndex], iCAL_EXP, TRUE);
				
				// ����� ���� ���� ����ġ�� ����ġ ������ �����Ŵ
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = (int)iCAL_EXP;
#else // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
//--> MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC ���� �� �Ʒ��ڵ� ������ ����
#ifdef MODIFY_EVENTMAP_REWARD_BUGFIX_20080404	// �������ġ�� ������ ���常 ����
				if( gObjCheckUsedBuffEffect( &gObj[iIndex], BUFFTYPE_PCBANG_POINT_MARK3 ))
				{
					iCAL_EXP = 0;
				}
				
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				
				iRET_EXP = iCAL_EXP;
#else	// MODIFY_EVENTMAP_REWARD_BUGFIX_20080404				
#ifdef PCBANG_POINT_SYSTEM_20070206	// ����ĳ�� - ����ġ ����
				g_PCBangPointSystem.CheckItemOptForGetExp( &gObj[iIndex], (int)iCAL_EXP );
#endif
#endif // MODIFY_EVENTMAP_REWARD_BUGFIX_20080404

		// ����� ���� ���� ����ġ�� ����ġ ������ �����Ŵ
		#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
				iCAL_EXP = (int)iCAL_EXP * m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_fRewardExpRate;
		#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

#ifdef MODIFY_MARKITEM_20080506
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				CheckItemOptForGetExp(&gObj[iIndex], iCAL_EXP, TRUE);
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#endif // MODIFY_MARKITEM_20080506
				
		#ifdef BUGFIX_GAIN_EXP_20071210				
				iRET_EXP = iCAL_EXP;
		#else
				iRET_EXP = (int)iCAL_EXP;
		#endif
//<--
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC

#ifndef MODIFY_MARKITEM_20080506
	//���� �Ƚ��� ���� ���� riverstyx
	#ifdef MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
				CheckItemOptForGetExp(&gObj[iIndex], iCAL_EXP, TRUE);
	#endif // MODIFY_BUFF_SYSTEM_EXTENTION_CASHSHOP_20080318
#endif // MODIFY_MARKITEM_20080506

	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
				iCAL_EXP = LevelUp( iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
	#else
				iCAL_EXP = LevelUp( iIndex, iCAL_EXP );
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !!NOT
	#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
		#ifdef ADD_PCS_MARK_OF_EXP_20070205
				if( gObj[iIndex].m_wExprienceRate > 0 )
				{
					iCAL_EXP = LevelUp(iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
				}
				else
				{
					iEXP = 0;
					iCAL_EXP = 0;
					iRET_EXP = 0;
				}
		#else
				iCAL_EXP = LevelUp(iIndex, iCAL_EXP, EVENT_TYPE_BLOODCASTLE );
		#endif // ADD_PCS_MARK_OF_EXP_20070205
	#endif // MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			}
		}

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
	#if TESTSERVER == 1 
			LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardEXP() - EXP : %d", 
				gObj[iIndex].AccountID,
				gObj[iIndex].Name,
				iRET_EXP
				);
	#endif
		
		// ����ڿ��� �������ġ�� ������.
		// *�����ͷ��� ������ ����ġ�� �̹� ��������.
		if( !g_MasterLevelSystem.IsMasterLevelUser( &gObj[iIndex] ) )
		{
	#ifdef BUGFIX_GAIN_EXP_20071210
			GCSendExp_INT64( iIndex, 0xFFFF, iRET_EXP, 0, 0 );
	#endif
		}
#else
		PMSG_KILLPLAYER_EXT	pkillMsg;

		PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof( pkillMsg));		// ���� 0x16 �̾����� INT�� Ȯ���ϱ� ���� 0x9C�� ����.

		pkillMsg.NumberH	= HIBYTE( 0xFFFF );
		pkillMsg.NumberL	= LOBYTE( 0xFFFF );
		pkillMsg.ExpH		= HIWORD( iRET_EXP ) ;
		pkillMsg.ExpL		= LOWORD( iRET_EXP ) ;
		pkillMsg.DamageH    = HIBYTE( 0 );
		pkillMsg.DamageL    = LOBYTE( 0 );

#if TESTSERVER == 1 
		LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardEXP() - EXP : %d", 
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			iRET_EXP
			);
#endif

		// ����ڿ��� �������ġ�� ������.
		DataSend(iIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
#endif
	}

	
	return iRET_EXP;
}


int CBloodCastle::CalcSendRewardZEN (int iIndex, int iZEN)
{
	if (iZEN <= 0)
		return 0;

	INT iRET_ZEN = 0;

	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return 0;

	if (gObj[iIndex].Connected < 3)
		return 0;

#ifdef MODIFY_ZEN_MAX_20040414
	if( !gObjCheckMaxZen( iIndex, iZEN ) )
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].Money;
		gObj[iIndex].Money += iRET_ZEN;
		return iRET_ZEN;
	}
	else
#endif
	gObj[iIndex].Money += iZEN;

	iRET_ZEN = iZEN;

#if TESTSERVER == 1 
	LogAddTD("[Blood Castle] [%s][%s] CBloodCastle::CalcSendRewardZEN() - ZEN : %d", 
		gObj[iIndex].AccountID,
		gObj[iIndex].Name,
		iZEN
		);
#endif

	GCMoneySend(iIndex, gObj[iIndex].Money);

	return iRET_ZEN;
}


void CBloodCastle::DropChaosGem (int iIndex, int iBridgeNum)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

	int iMaxHitUser = iIndex;

	int drop_count = 0, used_count = 0;

	int count = m_LST_BLOODCASTLE_REWARD[iBridgeNum].size();

	tdBCRewardList::iterator it = m_LST_BLOODCASTLE_REWARD[iBridgeNum].begin();

	for(std::advance(it, rand()%count);used_count < count && drop_count < m_BridgeData[iBridgeNum].m_iDropItemCount;advance(it = m_LST_BLOODCASTLE_REWARD[iBridgeNum].begin(),rand()%count),++used_count)
	{
		BLOODCASTLE_REWARD &reward = (*it);

		if((rand()%1000) < reward.max_drop_rate)
		{
			drop_count++;
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// BC_MAPNUMBER_BASE_CHAOSGEM ����
			int iChaosGemMapNum = -1;
			switch( gObj[iIndex].MapNumber )
			{
			case MAP_INDEX_BLOODCASTLE1: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM1; break;
			case MAP_INDEX_BLOODCASTLE2: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM2; break;
			case MAP_INDEX_BLOODCASTLE3: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM3; break;
			case MAP_INDEX_BLOODCASTLE4: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM4; break;
			case MAP_INDEX_BLOODCASTLE5: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM5; break;
			case MAP_INDEX_BLOODCASTLE6: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM6; break;
			case MAP_INDEX_BLOODCASTLE7: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM7; break;
			case MAP_INDEX_BLOODCASTLE8: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM8; break;
			}

			ItemSerialCreateSend(gObj[iIndex].m_Index, iChaosGemMapNum, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, reward.item_type, reward.item_level, (BYTE)ItemAttribute[reward.item_type].Durability, reward.is_skill, reward.is_luck, reward.option, iMaxHitUser, reward.exc_option);
#else
			ItemSerialCreateSend(gObj[iIndex].m_Index, BC_MAPNUMBER_BASE_CHAOSGEM + gObj[iIndex].MapNumber, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, reward.item_type, reward.item_level, (BYTE)ItemAttribute[reward.item_type].Durability, reward.is_skill, reward.is_luck, reward.option, iMaxHitUser, reward.exc_option);
#endif
		}
	}
/*
	int iType = ItemGetNumberMake(12, 15);
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// BC_MAPNUMBER_BASE_CHAOSGEM ����
	int iChaosGemMapNum = -1;
	switch( gObj[iIndex].MapNumber )
	{
	case MAP_INDEX_BLOODCASTLE1: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM1; break;
	case MAP_INDEX_BLOODCASTLE2: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM2; break;
	case MAP_INDEX_BLOODCASTLE3: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM3; break;
	case MAP_INDEX_BLOODCASTLE4: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM4; break;
	case MAP_INDEX_BLOODCASTLE5: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM5; break;
	case MAP_INDEX_BLOODCASTLE6: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM6; break;
	case MAP_INDEX_BLOODCASTLE7: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM7; break;
	case MAP_INDEX_BLOODCASTLE8: iChaosGemMapNum = BC_MAPNUMBER_CHAOSGEM8; break;
	}
	
	ItemSerialCreateSend(gObj[iIndex].m_Index, iChaosGemMapNum, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);
#else
	ItemSerialCreateSend(gObj[iIndex].m_Index, BC_MAPNUMBER_BASE_CHAOSGEM + gObj[iIndex].MapNumber, (BYTE)gObj[iIndex].X, (BYTE)gObj[iIndex].Y, iType, 0, (BYTE)0, 0, 0, 0, iMaxHitUser);
#endif
*/
}

#ifdef BLOODCASTLE_EVENT_5TH_20050531
void CBloodCastle::SendRewardScore (INT iIndex, INT iSCORE, INT iLeftTime,	INT iAlivePartyCount)	// ��ŷ������ ���������� ������.
#else	
#ifdef BLOODCASTLE_EVENT_3RD_20040401
void CBloodCastle::SendRewardScore (int iIndex, int iSCORE, INT iLeftTime)	// ��ŷ������ ���������� ������.
#else
void CBloodCastle::SendRewardScore (int iIndex, int iSCORE)	// ��ŷ������ ���������� ������.
#endif	// BLOODCASTLE_EVENT_3RD_20040401
#endif	// BLOODCASTLE_EVENT_5TH_20050531
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
		return;

#ifdef BLOODCASTLE_EVENT_3RD_20040401
#ifdef BLOODCASTLE_EVENT_5TH_20050531
	PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;	
#else
	PMSG_ANS_BLOODCASTLESCORE_3RD pMsg;
#endif
	
	pMsg.h.c		= PMHC_BYTE;

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	pMsg.h.headcode	= 0x0D;
#else	
#ifdef BLOODCASTLE_EVENT_4TH_20040531
	pMsg.h.headcode	= 0x0C;
#else
	pMsg.h.headcode	= 0x0A;
#endif	// BLOODCASTLE_EVENT_4TH_20040531
#endif	// BLOODCASTLE_EVENT_5TH_20050531
	
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score	= iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;

	pMsg.ServerCode=gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    gObj[iIndex].Name, MAX_IDSTRING);

#ifndef BLOODCASTLE_EVENT_5TH_20050531
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = gObj[iIndex].PcBangGuid;
#endif	// FOR_BLOODCASLTE_PCEVENT
#endif	// BLOODCASTLE_EVENT_5TH_20050531

#ifdef BLOODCASTLE_EVENT_5TH_20050531
	pMsg.iAlivePartyCount = iAlivePartyCount;
#endif

#else
	PMSG_ANS_BLOODCASTLESCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x07;
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score	= iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    gObj[iIndex].Name, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = gObj[iIndex].PcBangGuid;
#endif
#endif 

	
	// ������ ��ŷ������ �����ϴ� �κ�
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
		wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


#ifdef BLOODCASTLE_SENDPOINT_BUGFIX_20040604

void CBloodCastle::SendRewardScore		(											// ��ŷ������ ���������� ������.
	LPBLOODCASTLE_USER lpBC_USER, 
	INT iSCORE,
	INT iLeftTime
	)
{

#ifdef BLOODCASTLE_EVENT_3RD_20040401
	PMSG_ANS_BLOODCASTLESCORE_3RD pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
#ifdef BLOODCASTLE_EVENT_4TH_20040531
	pMsg.h.headcode	= 0x0C;
#else
	pMsg.h.headcode	= 0x0A;
#endif
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score		= iSCORE;
	pMsg.BridgeNum	= lpBC_USER->m_iBloodCastleIndex;
	pMsg.Class		= lpBC_USER->m_iCharClass;

	pMsg.ServerCode=gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, lpBC_USER->m_szAccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpBC_USER->m_szCharName, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = 0;
#endif
#else
	PMSG_ANS_BLOODCASTLESCORE pMsg;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x07;
	pMsg.h.size		= sizeof( pMsg );

	pMsg.Score		= iSCORE;
	pMsg.BridgeNum	= lpBC_USER->m_iBloodCastleIndex;
	pMsg.Class		= lpBC_USER->m_iCharClass;

	pMsg.ServerCode=gGameServerCode;
	memcpy(pMsg.AccountID, lpBC_USER->m_szAccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID,    lpBC_USER->m_szCharName, MAX_IDSTRING);
#ifdef FOR_BLOODCASLTE_PCEVENT
	pMsg.iPCRoomGUID = 0;
#endif
#endif 

	
	// ������ ��ŷ������ �����ϴ� �κ�
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
		wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	
}

#endif


void CBloodCastle::SendBridgeAnyMsg(LPBYTE lpMsg, int iSize, int iBridgeIndex )		// Ư�� �ٸ��� ����ڵ鿡�� �޽����� ������.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			if(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected == 3)
			{
				// �ٸ��� ���� ����ڸ� �������� ���� ĳ���ʿ� ��� ��Ƹ� �ִٸ� �޽����� �����ش�.
				if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1) && (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1))
					DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)lpMsg, iSize);
			}
		}
	}
}


void CBloodCastle::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)						// �������ִ� ��� ����ڵ鿡�� �޽����� ������.
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}


void CBloodCastle::SetMonsterKillCount		(int iBridgeIndex)						// Ư�� �ٸ��� ���� ���� �Ҵ���� �����Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	INT iTOT_USER_COUNT		= 0;
	INT iLIVE_USER_COUNT	= 0;
	INT iKILL_USER_COUNT	= 0;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		
#ifdef FOR_ONAIR
#ifdef MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
		if( m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 ) continue;
#endif	// MODIFY_BLOODCASTLE_USERINDEX_BUGFIX_20080709
		
		if( CheckAuthority(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex], AUTHORITY_ADMIN) == true)
//			gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Authority&AUTHORITY_ADMIN )
		{
			continue;
		}
#endif
		
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			iTOT_USER_COUNT++;

			if (CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber)) {
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2) {
					iLIVE_USER_COUNT++;
				}
				else {
					iKILL_USER_COUNT++;
				
				}
			}
			else {
				iKILL_USER_COUNT++;
			}
		}
	}
	
//	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = (iTOT_USER_COUNT * BC_PERUSER_MONSTER_KILL) - (iKILL_USER_COUNT * BC_PERUSER_MONSTER_KILL * 8 /10);

	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * BC_PERUSER_MONSTER_KILL;
	m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	LogAddTD("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d",
		iBridgeIndex+1,
		m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
		m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT,
		iTOT_USER_COUNT,
		iLIVE_USER_COUNT,
		iKILL_USER_COUNT
		);
}


bool CBloodCastle::CheckMonsterKillCount	(int iBridgeIndex)		// Ư�� �ٸ��� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
		if (m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT)
		{
			LogAddTD( "[Blood Castle][Bug Tracer] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)"
			, iBridgeIndex + 1
			, m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT
			, m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT 
			);

			return true;
		}
#else
	if (m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT)
		return true;
#endif

	return false;
}


bool CBloodCastle::CheckMonsterKillSuccess	(int iBridgeIndex)		// Ư�� �ٸ��� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}


bool CBloodCastle::CheckBossKillCount	(int iBridgeIndex)			// Ư�� �ٸ��� ���� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

#ifdef ADD_LOG_CHECK_CAHOSCASTLE_BLOODCASTLE_EVENT_20060314
	if( m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		LogAddTD( "[Blood Castle][Bug Tracer] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)"
			, iBridgeIndex + 1
			, m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT
			, m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT 
			);

		return true;
	}
#else
	if (m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT)
		return true;
#endif

	return false;
}


bool CBloodCastle::CheckBossKillSuccess	(int iBridgeIndex)		// Ư�� �ٸ��� ���� ���� ���� �Ҵ���� ä�������� Ȯ���Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	return m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}


bool CBloodCastle::CheckEveryUserDie	(int iBridgeIndex)			// Ư�� �ٸ��� ��� ����ڰ� �ʿ� ���ų� �׾����� Ȯ���Ѵ�.
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	bool bRET_VAL = true;

	for (INT i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1) {
			if (!CHECK_LIMIT(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, MAX_OBJECT)) {
				LogAddTD("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				continue;
			}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
				if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > 2) {
					// ����� �ȿ��� �ְ� ������ ����ִٸ� ���� �� ���� ���� ���̴�.
					bRET_VAL = false;
				}
				else
				{
					LogAddTD("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d", iBridgeIndex+1, m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				}
			}
			else {
				if (!m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg) {
					// ���� ����޽����� �������� �ʾҴٸ� ����Ǿ��ٴ� �޽����� ���⼭ �����ش�.
					m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_END;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;

					DataSend(m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}


bool CBloodCastle::CheckAngelKingExist	(int iBridgeIndex)			// Ư�� �ٸ��� ��õ�簡 �ִ��� ������ üũ -> ������ �������
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return false;

	bool bRET_VAL = false;

	if (m_BridgeData[iBridgeIndex].m_iAngelKingPosNum == -1) {
		// �ƿ� ��õ�簡 ���� �����ǿ��� ������. -> ġ������ ����
		LogAddTD("[Blood Castle] (%d) Angel King's Monster Position Doesn't Exist", iBridgeIndex+1);
		return false;
	}

	for( int i=0 ; i<ALLOC_USEROBJECTSTART ; i++) {
		if( gObj[i].Connected == 3 && (gObj[i].Type == OBJTYPE_NPC) ) {
			if (gObj[i].Class == NPC_ANGELKING) {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����				
				if (gObj[i].MapNumber == GetMapNumByBCBridge( iBridgeIndex ) ) {
#else
				if (gObj[i].MapNumber == MAP_INDEX_BLOODCASTLE1 + iBridgeIndex) {
#endif
					// ��õ�簡 �ִٸ� -> �׳� ������.
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if (!bRET_VAL) {
		// ��õ�簡 ����!! -> �����
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
		int result = gObjAddMonster( GetMapNumByBCBridge( iBridgeIndex ) );
#else
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
		int result = gObjAddMonster(MAP_INDEX_BLOODCASTLE1 + iBridgeIndex);
#else
		int result = gObjAdd();
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		if( result >= 0 )
		{
			gObjSetPosMonster(result, m_BridgeData[iBridgeIndex].m_iAngelKingPosNum);
			gObjSetMonster(result, NPC_ANGELKING);

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
			gObj[result].m_cBloodCastleIndex = GetBridgeIndexByMapNum( gObj[result].MapNumber );
#else
			gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - MAP_INDEX_BLOODCASTLE1;
#endif

			bRET_VAL = true;
		}
		else {
			LogAddTD("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)", iBridgeIndex+1, result);
			return false;
		}
	}

	return bRET_VAL;
}


int CBloodCastle::GetWhoGotUltimateWeapon (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	int iBridgeUserIndex = -1;


	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		int iIndex = m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
			continue;

		if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
			continue;

		if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
			continue;

#ifdef PERSONAL_SHOP_20040113	// ���� ĳ������ ���λ��� �������� �˻��Ͽ� ���� ���� ���⸦ �������� ����
		for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
		for(int x=0; x<MAX_INVENTORY; x++)
#endif
		{
			if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if( gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(13, 19) )		// ��õ�� �ø�� �����Ѵٸ�
				{
					INT iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;			// ������ ������ ��´�.
					if (CHECK_LIMIT(iITEM_LEVEL, 3)) {
						if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1) {
							if (m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number) {
								// �ø������ �´�. -> �� �ɸ��� ��õ���� ���빫�⸦ ������ �ִ°��� Ȯ����
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}
	}

	return iBridgeUserIndex;
}


int CBloodCastle::GetCurrentLiveUserCount (int iBridgeIndex)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return -1;

	int iRetLiveUserCount = 0;


	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		int iIndex = m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if (!CHECK_LIMIT(iIndex, MAX_OBJECT))
			continue;

		if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber))
			continue;

		if (gObj[iIndex].Type != OBJTYPE_CHARACTER)
			continue;

#ifdef FOR_ONAIR
		if( gObj[iIndex].Authority&AUTHORITY_ADMIN )
		{
			continue;
		}
#endif
		if (gObj[iIndex].Connected > 2) {
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}


#ifdef BLOODCASTLE_ITEMDROP_DIRECTLY_20040728

#include "ObjUseSkill.h"
extern CObjUseSkill gObjUseSkill;

BOOL CBloodCastle::DropItemDirectly		(										// �������� ���ǰ˻� ���� �ٷ� ����Ѵ�. (��õ���� ���빫�� ��ӿ��� ���)
			INT iBridgeIndex,
			INT iIndex, 
			INT iItemType,
			INT iItemPos
			)
{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �� �ε��� ����
	if (!CHECK_BLOODCASTLE( GetMapNumByBCBridge( iBridgeIndex ) )) {
#else
	if (!CHECK_BLOODCASTLE(MAP_INDEX_BLOODCASTLE1 + iBridgeIndex)) {
#endif
		LogAddTD("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);
		return false;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof( pResult ));
	pResult.Result		= 0x01;
	pResult.Ipos		= iItemPos;		

	int map_num = gObj[iIndex].MapNumber;	
	int type    = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level   = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur   = gObj[iIndex].pInventory[iItemPos].m_Durability;
	int ret     = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1= gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2= gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3= gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption= gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	
#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		BYTE ItemExOption	= g_kJewelOfHarmonySystem.GetItemStrengthenOption( &gObj[iIndex].pInventory[iItemPos] );
		BYTE ItemExLevel	= g_kJewelOfHarmonySystem.GetItemOptionLevel( &gObj[iIndex].pInventory[iItemPos] );
	#endif
#endif // EXTEND_LOG_SYSTEM_03_20060816
	
	BYTE NewOption[8];
	ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);

#ifdef DARKLORD_WORK
	int	 PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int	 PetExp = 	gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
#endif

	BYTE SOption= gObj[iIndex].pInventory[iItemPos].m_SetOption;
	
#ifdef ADD_380ITEM_NEWOPTION_20060711
	#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
		BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	#else
		BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx >> 7;
	#endif // MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
#endif // ADD_380ITEM_NEWOPTION_20060711

	DWORD item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50]="Item";
	
	int aAntiLootIndex = -1;					// �̷��� ������ ������ ���� �� �ִ�.
	
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
	BYTE SocketOption[MAX_SOCKET_SLOT] = { SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE, SOCKETSLOT_NONE };
	BYTE SocketBonusOption = 0;

	g_SocketOptionSystem.GetSocketOption( &gObj[iIndex].pInventory[iItemPos], SocketOption, SocketBonusOption );
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef DARKLORD_WORK

	#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketBonusOption ) == TRUE )
	#else
		#ifdef ADD_380ITEM_NEWOPTION_20060711
			if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx) == TRUE )
		#else
			if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp) == TRUE )
		#endif // ADD_380ITEM_NEWOPTION_20060711
	#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#else	// #ifdef DARKLORD_WORK


		if( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y, Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex) == TRUE )

#endif	// #ifdef DARKLORD_WORK
	{   // ������ �����⿡ �����ϸ�..
		gObjInventoryDeleteItem(iIndex, iItemPos);	
		pResult.Result = 0x01;

#ifdef EXTEND_LOG_SYSTEM_03_20060816
	#ifdef ADD_380ITEM_NEWOPTION_20060711
		#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
		LogAddTD(lMsg.Get(479),
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			gObj[iIndex].MapNumber, 
			gObj[iIndex].X, 
			gObj[iIndex].Y, 
			s_num, 
			szItemName, 
			type, 
			level, 
			Option1, 
			Option2, 
			Option3,
			(int)dur,
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			SOption,
		#ifdef MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
			ItemEffectEx>>7,
		#else
			ItemEffectEx,
		#endif// MODIFY_380_ITEM_OPTION_DELETE_AT_DROP_20060919
			ItemExOption,
			ItemExLevel
			);
		#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#endif // ADD_380ITEM_NEWOPTION_20060711
#else // EXTEND_LOG_SYSTEM_03_20060816
		LogAddTD(lMsg.Get(479),
			gObj[iIndex].AccountID, 
			gObj[iIndex].Name,
			gObj[iIndex].MapNumber, 
			gObj[iIndex].X, 
			gObj[iIndex].Y, 
			s_num, 
			szItemName, 
			type, 
			level, 
			Option1, 
			Option2, 
			Option3,
			NewOption[0],
			NewOption[1],
			NewOption[2],
			NewOption[3],
			NewOption[4],
			NewOption[5],
			NewOption[6],
			SOption
			);
#endif // EXTEND_LOG_SYSTEM_03_20060816

	}
	else
	{
		// ������ ����.
		pResult.Result = 0x00;
	}
		
	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	if( pResult.Result == 0x01 )
	{
		// ��� ��ü �ߴٸ� 
		if( iItemPos < MAX_EQUIPMENT ) 
		{
			if( iItemPos == EQUIPMENT_RING_RIGHT || iItemPos == EQUIPMENT_RING_LEFT)
			{
				gObjUseSkill.SkillChangeUse(iIndex);				
			}
			gObjMakePreviewCharSet(iIndex);
			
			PMSG_USEREQUIPMENTCHANGED	pChange;
			
			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof( pChange ));

			pChange.NumberH    = HIBYTE( iIndex );
			pChange.NumberL	   = LOBYTE( iIndex );
						
			//pChange.ItemInfo[0] = (BYTE)gObj[iIndex].pInventory[iItemPos].m_Type;
			ItemByteConvert(pChange.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pChange.ItemInfo[1] = iItemPos<<4;
			pChange.ItemInfo[1]|= LevelSmallConvert(iIndex, iItemPos)&0x0F;

			
			DataSend(iIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(&gObj[iIndex], (LPBYTE)&pChange, pChange.h.size);
		}
	}
	return pResult.Result;
}

#endif


bool CBloodCastle::CheckUserHaveUlimateWeapon (int iIndex)						// ����ڸ� �˻��Ͽ� ���빫�⸦ ������ �ִ��� Ȯ��
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return false;

	if ((gObj[iIndex].Type != OBJTYPE_CHARACTER) || (gObj[iIndex].Connected <= 2))
		return false;

	bool bRetVal = false;

#ifdef PERSONAL_SHOP_20040113	// ���� ĳ������ ���λ��� �������� �˻��Ͽ� Ư�� ����ڿ��� ���빫�Ⱑ �ִ��� ����
	for(int x=0; x<MAX_INVENTORY_EXTEND; x++)
#else
	for(int x=0; x<MAX_INVENTORY; x++)
#endif
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( 
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(0, 19)) ||	// �����
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(5, 10)) ||	// ����������		
				(gObj[iIndex].pInventory[x].m_Type == MAKE_ITEMNUM(4, 18))		// ���뼮��			
				)
			{
				// ���� ������ ������ �� �ϳ��� ������ �ִٸ� ��
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}


#ifdef BLOODCASTLE_EVENT_5TH_20050531

bool CBloodCastle::CheckWinnerExist (								// ����� ����ڰ� �ִ��� üũ
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerExist() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerExist() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1",	iBridgeIndex + 1);
		return true;
	}

	return false;
}


bool CBloodCastle::CheckWinnerValid (								// ����� ����ڰ� ������ ���ų� ���� ��Ż���� �ʾҴ��� üũ
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1",	iBridgeIndex + 1);
		return false;
	}

	// ������ �������� üũ
	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",	iBridgeIndex + 1);
		return false;
	}

	// ĳ�������� ���� ��������� üũ
	if ((gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) || 
		(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1) ||
		(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex)
		) 
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...",	iBridgeIndex + 1);
		return false;
	}

	// ĳ���ʿ� �������� �ʴ��� üũ
	if (!CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)) {
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)",	iBridgeIndex + 1);
		return false;
	}

	return true;
}


bool CBloodCastle::CheckUserWinnerParty (							// ����ڰ� ������� ��Ƽ�� �ִ��� Ȯ�� (������ ����ĳ�� ���� �����ؾ� ��)
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",	iBridgeIndex + 1);
		return false;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",	iBridgeIndex + 1);
		return false;
	}
	
	if (!gObjIsConnected(iIndex)) {
//		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckUserWinnerParty() - !gObjIsConnected(iIndex)",	iBridgeIndex + 1);
		return false;
	}

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if (CHECK_LIMIT(iPartyIndex1, MAX_PARTY) &&
		iPartyIndex1 == iPartyIndex2
		)
	{
		// ����ڿ� ����ڴ� ���� ��Ƽ�̴�.
		return true;
	}

	return false;
}


bool CBloodCastle::CheckPartyExist		(							// Ư�� ������� ��Ƽ���� ����ĳ�� �ʳ��� �����ϰ� ��õ��� ��ȭ�� ���� ����� �ִ��� ����
	INT iIndex
	)
{
	if (gObjIsConnected(iIndex) == FALSE) {
		return false;
	}

	int iPartyIndex = gObj[iIndex].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// ��Ƽ�� �ƿ� �������� �ʾ���
		return false;
	}

	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == false) {
					// ����ĳ�� �ʾȿ� �����ϴµ��� �ұ��ϰ� ��ȭ�� ���ߴٸ�
					return true;
					break;
				}
			}
		}
	}
	
	return false;
}


bool CBloodCastle::CheckWinnerPartyComplete	(						// ������� ��Ƽ�� �¸��ߴ��� ����
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return false;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		return false;
	}

	int iPartyIndex = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// ��Ƽ�� �ƿ� �������� �ʾҴٸ� ����
		return true;
	}

	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == false) {
					// ����ĳ�� �ʾȿ� �����ϴµ��� �ұ��ϰ� ��ȭ�� ���ߴٸ�
					return false;
					break;
				}
			}
		}
	}
	
	return true;
}


bool CBloodCastle::SetBridgeWinner		(							// Ư�� �ٸ��� ����ڸ� ���Ѵ�.
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return false;
	}

	if (gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {		// �̹� ����ڰ� �����Ѵٸ� ��ȿ
		return false;
	}
	
	if (!gObjIsConnected(iIndex)) {
		return false;
	}

	if (!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)) {
		return false;
	}

	m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;
	return true;
}


int CBloodCastle::GetWinnerPartyCompleteCount (					// Ư�� �ٸ��� ������� ��Ƽ�� ��ȭ�� ������ ����� ������� Ȯ���Ѵ�.
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return 0;
	}

	if (!gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)) {
		return 0;
	}

	int iPartyIndex = gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	if (!CHECK_LIMIT(iPartyIndex, MAX_PARTY)) {						// ��Ƽ�� �ƿ� �������� �ʾ���
		return 0;
	}

	int iPartyComplete = 0;
	int iUserIndex;
	for (int iPartyUserIndex = 0; iPartyUserIndex < MAX_PARTYUSER ; iPartyUserIndex++) {
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];
		if (gObjIsConnected(iUserIndex)) {
			if (CHECK_BLOODCASTLE(gObj[iUserIndex].MapNumber) && 
				CHECK_LIMIT(gObj[iUserIndex].m_cBloodCastleIndex, MAX_BLOODCASTLE_BRIDGE_COUNT) &&
				gObj[iUserIndex].Live == 1) 
			{
				if (gObj[iUserIndex].m_bBloodCastleComplete == true) {
					// ������ ��Ƽ��
					iPartyComplete++;
				}
			}
		}
	}
	
	return iPartyComplete;
}


int CBloodCastle::GetWinnerPartyCompletePoint (					// Ư�� �ٸ��� ������� ��Ƽ�����ڿ� ���� ������ ���Ѵ�.
	INT iBridgeIndex
	)
{
	int iPartyComplete = GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;
	if (CHECK_LIMIT(iPartyComplete, MAX_PARTYUSER)) {
		return g_iBC_Party_EventPoint[iPartyComplete];
	}
	return 0;
}


void CBloodCastle::ChangeMonsterState (							// ����ĳ���� �����ϴ� ���ʹ� ������ �� �ɷ�ġ ���� ���θ� �����Ͽ� �����Ѵ�.
	INT iBridgeIndex,
	INT iIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)) {
		return;
	}
	
/*
	if (!gObjIsConnected(iIndex))
		return;

	if (gObj[iIndex].Type != OBJTYPE_MONSTER ||
		!CHECK_BLOODCASTLE(gObj[iIndex].MapNumber)
		)
		return;
*/

	INT iAddDamageMax	= 0;
	INT iAddDamageMin	= 0;
	INT iAddDefense		= 0;
	if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 5*60*1000) {
		// ���� �ð��� 5�� �����̴� (10�а��)
		iAddDamageMin	= g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax	= g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense		= g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if (m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 10*60*1000) {
		// ���� �ð��� 10�� �����̴� (5�а��)
		iAddDamageMin	= g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax	= g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense		= g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

/*
	if (!iAddDamageMin &&
		!iAddDamageMax &&
		!iAddDefense
		)
		return;
*/

	LPMONSTER_ATTRIBUTE lpMA;
	lpMA = gMAttr.GetAttr(gObj[iIndex].Class);	
	if( lpMA == NULL ) {
		return;
	}

	gObj[iIndex].m_AttackDamageMin	= lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax	= lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense			= lpMA->m_Defense + iAddDefense;
}


void CBloodCastle::FixUsersPlayStateWin (	// ����ĳ���� ���� �� �������� ������ ����ڵ��� ���¸� �ٽ��ѹ� üũ ������ ��
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	if (m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1) {			// �̼��� �����ߴٸ�

		for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
			if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
				// �����ϴ� ����� üũ
				continue;
			}

			if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
				// ���ӵǾ� �ִ� ����ڸ� üũ
				continue;
			}

			if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
				(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
				) 
			{
				// ������ �´� ����ڸ� üũ
				continue;
			}
			
			LPOBJECTSTRUCT lpObj = &gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];
			
			switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
			case BC_USER_ALIVE :				// ������	
				{
					// ������� �������ΰ� ������ΰ� Ȯ�� �ʿ�
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// ������� ��Ƽ�̴�.
						if (CHECK_BLOODCASTLE(lpObj->MapNumber) &&
							lpObj->Live == 1 &&
							lpObj->Life > 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_ALIVE;
						}
						else {
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
					else {
						// Ȥ�� �װų� ������ �̵��ߴ��� üũ�Ѵ�.
						if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
							lpObj->Live == 0 ||
							lpObj->Life <= 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
						}
					}
				}
				break;
			case BC_USER_DEAD :					// �����
				{
					// ������� ������ΰ� Ȯ�� �ʿ�
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// ������� ��Ƽ�̴�.
						m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
					}
				}
				break;
			case BC_USER_WINNER :				// �����
				{
					// �ѹ� ����ڴ� ���� ���� (������ CheckWinnerValid() ���� ���Ἲ�� �̸� üũ�Ѵٴ� ����)
				}
				break;
			case BC_USER_WINNER_PARTY_ALIVE :	// ������ (������� ��Ƽ��)
				{
					// ����ڰ� ������ �� ���� �������� ��Ƽ�� �߰��� ������� üũ, ����ִ��� üũ
					if ((CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) &&
						(lpObj->PartyNumber == gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// ������� ��Ƽ�̴�.
						if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
							lpObj->Live == 0 ||
							lpObj->Life <= 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_WINNER_PARTY_DEAD;
						}
					}
					else {
						// ���̻� ������� ��Ƽ�� �ƴϴ�
						if (CHECK_BLOODCASTLE(lpObj->MapNumber) &&
							lpObj->Live == 1 &&
							lpObj->Life > 0.0
							)
						{
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
						}
						else {
							m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
						}
					}
				}
				break;
			case BC_USER_WINNER_PARTY_DEAD :	// ����� (������� ��Ƽ��)
				{
					// ����ڰ� ������ �� ���� �������� ��Ƽ�� �߰��� ������� üũ
					if (!(CHECK_LIMIT(lpObj->PartyNumber, MAX_PARTY)) ||
						(lpObj->PartyNumber != gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber)
						) 
					{
						// ���̻� ������� ��Ƽ�� �ƴϴ�
						m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
					}
				}
				break;
			}

			LogAddTD ("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d", 
				iBridgeIndex+1,
				lpObj->AccountID,
				lpObj->Name,
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState
				);
		}
	}
}


void CBloodCastle::FixUsersPlayStateFail (	// ����ĳ���� ���� �� �������� ������ ����ڵ��� ���¸� �ٽ��ѹ� üũ ������ ��
	INT iBridgeIndex
	)
{
	if (!CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT))
		return;

	for (int i = 0 ; i < MAX_BLOODCASTLE_USER_COUNT ; i++) {
		if (m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) {
			// �����ϴ� ����� üũ
			continue;
		}

		if (gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < 3) {
			// ���ӵǾ� �ִ� ����ڸ� üũ
			continue;
		}

		if ((gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1) || 
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1) ||
			(gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex)
			) 
		{
			// ������ �´� ����ڸ� üũ
			continue;
		}
		
		LPOBJECTSTRUCT lpObj = &gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];
		
		switch (m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState) {
		case BC_USER_ALIVE :				// ������	
			{
				// �ش���� ����
			}
			break;
		case BC_USER_DEAD :					// �����
			{
				// �ش���� ����
			}
			break;
		case BC_USER_WINNER :				// �����
			{
				// �ش���� ����
			}
			break;
		case BC_USER_WINNER_PARTY_ALIVE :	// ������ (������� ��Ƽ��)
			{
				// ��Ҵ��� �׾����� Ȯ���ϰ� ����ó��
				if (!CHECK_BLOODCASTLE(lpObj->MapNumber) ||
					lpObj->Live == 0 ||
					lpObj->Life <= 0.0
					)
				{
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
				}
				else {
					m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_ALIVE;
				}
			}
			break;
		case BC_USER_WINNER_PARTY_DEAD :	// ����� (������� ��Ƽ��)
			{
				// ����ó��
				m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = BC_USER_DEAD;
			}
			break;
		}

		LogAddTD ("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d", 
			iBridgeIndex+1,
			lpObj->AccountID,
			lpObj->Name,
			m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState
			);
	}
}


#endif

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ���ȣ�� �ʹ�ȣ ���
INT CBloodCastle::GetMapNumByBCBridge( INT iBridgeIndex )
{
	INT iMapNum = 0;
	switch( iBridgeIndex )
	{
	case 0: 	iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case 1:		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case 2:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case 3:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case 4:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case 5:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case 6:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case 7:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;		
	}

	return iMapNum;
}

// �� ��ȣ�� ����ĳ�� �ε��� �˾Ƴ���
INT	CBloodCastle::GetBridgeIndexByMapNum( INT iMapNum )
{
	int iBridgeIndex = -1;

	switch( iMapNum )
	{
	case 11:	iBridgeIndex = 0;	break;
	case 12:	iBridgeIndex = 1;	break;
	case 13:	iBridgeIndex = 2;	break;
	case 14:	iBridgeIndex = 3;	break;
	case 15:	iBridgeIndex = 4;	break;
	case 16:	iBridgeIndex = 5;	break;
	case 17:	iBridgeIndex = 6;	break;
	case 52:	iBridgeIndex = 7;	break;
	}

	return iBridgeIndex;
}

// DS�� ���(������ ������ ����)�� ���� �� ��ȣ�� ���Ѵ�.
INT	CBloodCastle::GetMapNumByBC_CHAOSGEM( INT iChaosGem )
{
	// ** ����ĳ�� ������ ȥ���� ���빫�⸦ ����� ���� DS�� ��ſ��� ����ϴ� ��ȣ��
	// ** �������� ����ؾ� �� �� ��ȣ�� ���Ѵ�.
	// �� �̷��� �ߴ����� ����.

	int iMapNum		= iChaosGem;
	switch( iChaosGem )
	{
	case BC_MAPNUMBER_CHAOSGEM1:		iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case BC_MAPNUMBER_CHAOSGEM2: 		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case BC_MAPNUMBER_CHAOSGEM3:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case BC_MAPNUMBER_CHAOSGEM4:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case BC_MAPNUMBER_CHAOSGEM5:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case BC_MAPNUMBER_CHAOSGEM6:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case BC_MAPNUMBER_CHAOSGEM7:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case BC_MAPNUMBER_CHAOSGEM8:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;	
	}

	return iMapNum;
}

INT	CBloodCastle::GetMapNumByBC_ULTIMATEWEAPON( INT iUltimateWeapon )
{
	// ** ����ĳ�� ������ ȥ���� ���빫�⸦ ����� ���� DS�� ��ſ��� ����ϴ� ��ȣ��
	// ** �������� ����ؾ� �� �� ��ȣ�� ���Ѵ�.
	// �� �̷��� �ߴ����� ����.

	int iMapNum		= iUltimateWeapon;
	switch( iUltimateWeapon )
	{
	case BC_MAPNUMBER_ULTIMATEWEAPON1:		iMapNum = MAP_INDEX_BLOODCASTLE1;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON2: 		iMapNum = MAP_INDEX_BLOODCASTLE2;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON3:		iMapNum = MAP_INDEX_BLOODCASTLE3;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON4:		iMapNum = MAP_INDEX_BLOODCASTLE4;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON5:		iMapNum = MAP_INDEX_BLOODCASTLE5;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON6:		iMapNum = MAP_INDEX_BLOODCASTLE6;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON7:		iMapNum = MAP_INDEX_BLOODCASTLE7;	break;
	case BC_MAPNUMBER_ULTIMATEWEAPON8:		iMapNum = MAP_INDEX_BLOODCASTLE8;	break;	
	}

	return iMapNum;
}

#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#endif