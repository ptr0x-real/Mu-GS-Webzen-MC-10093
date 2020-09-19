#include "Stdafx.h"

#ifdef CRYWOLF_COMMON_PATCH_20050413

#include "CrywolfDefine.h"
#include "CrywolfUtil.h"

#include "DSProtocol.h"
#include "Protocol.h"
#include "User.h"

#include "MapServerManager.h"
#include "Gamemain.h"
#include "..\\Common\ClassDef.h"

#ifdef ADD_LOG_400LEVELUP_PARTY_20070628
#include "PartyClass.h"
extern PartyClass gParty;
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

extern classdef	DCInfo;

CCrywolfUtil::CCrywolfUtil()
{

}

CCrywolfUtil::~CCrywolfUtil()
{
	
}

void CCrywolfUtil::SendMapServerGroupMsg( LPSTR lpszMsg, ... )
{
	if (lpszMsg == NULL)
		return;

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, lpszMsg);
    vsprintf(szBuffer, lpszMsg, pArguments);
    va_end(pArguments);

	GS_GDReqMapSvrMsgMultiCast( g_MapServerManager.GetMapSvrGroup(), szBuffer );
}

void CCrywolfUtil::SendAllUserAnyData( LPBYTE lpMsg, int iSize )
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}

void CCrywolfUtil::SendAllUserAnyMsg( int iType, LPSTR lpszMsg, ... )
{
	if (lpszMsg == NULL)
		return;

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, lpszMsg);
    vsprintf(szBuffer, lpszMsg, pArguments);
    va_end(pArguments);


	PMSG_NOTICE	pNotice;

	switch(iType) 
	{
	case 1 : // 슬라이딩 공지
		{
			TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_DEFAULT);
			TNotice::SendNoticeToAllUser( &pNotice );
		}
		break;
	case 2 : // 중앙 공지
		{
			TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
			TNotice::SendNoticeToAllUser( &pNotice );
		}
		break;
	}
}

void CCrywolfUtil::SendCrywolfUserAnyData( LPBYTE lpMsg, int iSize )
{
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) 
			&& (gObj[i].Type == OBJTYPE_CHARACTER) 
			&& (gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		  )
		{
			DataSend(i, (LPBYTE)lpMsg, iSize);
		}
	}
}

void CCrywolfUtil::SendCrywolfUserAnyMsg( int iType, LPSTR lpszMsg, ... )
{
	if (lpszMsg == NULL)
		return;

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, lpszMsg);
    vsprintf(szBuffer, lpszMsg, pArguments);
    va_end(pArguments);


	PMSG_NOTICE	pNotice;

	switch(iType) 
	{
	case 1 : // 슬라이딩 공지
		{
			TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_DEFAULT);
		}
		break;
	case 2 : // 중앙 공지
		{
			TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
		}
		break;
	}

	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == 3) 
			&& (gObj[i].Type == OBJTYPE_CHARACTER) 
			&& (gObj[i].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			)
		{
			DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
		}
	}
	
}

void CCrywolfUtil::SendCrywolfChattingMsg( int iObjIndex, LPSTR lpszMsg, ... )
{
//	if( 
//		gObj[iObjIndex].Class != CRYWOLF_MON_DARKELF 
//		&& gObj[iObjIndex].Class != CRYWOLF_MON_BALGARS 
//	  )
	{
		return;
	}
	

	if (lpszMsg == NULL)
		return;
	
	if( !CHECK_LIMIT(iObjIndex, MAX_OBJECT) )
		return;

	LPOBJECTSTRUCT	lpObj = &gObj[iObjIndex];

	char		szBuffer[512]="";
	va_list		pArguments;
	
	va_start(pArguments, lpszMsg);
    vsprintf(szBuffer, lpszMsg, pArguments);
    va_end(pArguments);
	

	char szChat[MAX_CHAT] = {0,};

	CopyMemory( szChat, szBuffer, MAX_CHAT - 1 );

	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++ )
	{
		if( lpObj->VpPlayer2[i].state )
		{
			int iTargetNumber = lpObj->VpPlayer2[i].number;
			if( CHECK_LIMIT(iTargetNumber, MAX_OBJECT) )
			{
				ChatTargetSend(lpObj, szChat, iTargetNumber);
			}
		}
	}
}

int CCrywolfUtil::CrywolfMVPLevelUp( int iUserIndex, int iAddExp )
{
	if(!CHECK_LIMIT( iUserIndex, MAX_OBJECT ) )
		return 0;

	LogAddTD("[ Crywolf ][MVP Exp.] : [%s][%s](%d) %uz %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level, gObj[iUserIndex].Experience, iAddExp);	

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// 크라이울프 MasterLevel UP 처리
	// 마스터레벨은 이쪽에서.
	if( g_MasterLevelSystem.MasterLevelUp( &gObj[iUserIndex], iAddExp, true ) )
		return 0;

	// 펫 아이템 경험치 분배	
	gObjSetExpPetItem(iUserIndex,iAddExp);
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

	INT iLEFT_EXP = 0;	
	
#ifdef MODIFY_EXP_LOG_MONSTER_EVENT_INDEX_20060906
	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d", 
		gObj[iUserIndex].MapNumber, gObj[iUserIndex].X, gObj[iUserIndex].X,
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level, gObj[iUserIndex].Experience, iAddExp,
		0, EVENT_TYPE_CRYWOLF );
#endif

	// 최대 레벨을 MAX_LEVEL로 한다.
	if( gObj[iUserIndex].Level >= g_GlobalConfig.m_iMaxLevel )
	{
		GCServerMsgStringSend(lMsg.Get(1136), gObj[iUserIndex].m_Index, 1);
		return 0;
	}


	if( gObj[iUserIndex].Experience + iAddExp < gObj[iUserIndex].NextExp ) 
	{
		gObj[iUserIndex].Experience += iAddExp;
	}
	else 
	{
		iLEFT_EXP = gObj[iUserIndex].Experience + iAddExp - gObj[iUserIndex].NextExp;

		// 레벨 +1 과 경험치를 갱신한다.
		gObj[iUserIndex].Experience = gObj[iUserIndex].NextExp;
		gObj[iUserIndex].Level += 1;

		// 레벨업 포인트를 계산한다.
		if (gObj[iUserIndex].Class == CLASS_DARKLORD || gObj[iUserIndex].Class == CLASS_MAGUMSA)
		{
    		gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iSpecial_Points;
		}
		else
		{
    		gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iNormal_Points;
		}
		
		// +1스탯업 퀘스트 성공시 추가 스탯
		if( gObj[iUserIndex].PlusStatQuestClear )
		{	 
			gObj[iUserIndex].LevelUpPoint += g_GlobalConfig.m_iPowerUP_Points;
			LogAddTD("[ Crywolf ][MVP Exp.] [%s][%s] LevelUp PlusStatQuest Clear AddStat %d", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].LevelUpPoint);
		}
	
		gObj[iUserIndex].MaxLife += DCInfo.DefClass[gObj[iUserIndex].Class].LevelLife;

		if(g_GlobalConfig.m_bLimitLife && gObj[iUserIndex].MaxLife > g_GlobalConfig.m_iLimitLife)
			gObj[iUserIndex].MaxLife = g_GlobalConfig.m_iLimitLife;

		gObj[iUserIndex].MaxMana += DCInfo.DefClass[gObj[iUserIndex].Class].LevelMana;
		gObj[iUserIndex].Life = gObj[iUserIndex].MaxLife;
		gObj[iUserIndex].Mana = gObj[iUserIndex].MaxMana;	

		gObjNextExpCal(&gObj[iUserIndex]);
		gObjSetBP(gObj[iUserIndex].m_Index);
		GCLevelUpMsgSend(gObj[iUserIndex].m_Index);

		gObjCalcMaxLifePower(gObj[iUserIndex].m_Index);	// 최대 생명력을 다시 계산한다

		LogAddTD(lMsg.Get(520), gObj[iUserIndex].AccountID, gObj[iUserIndex].Name, gObj[iUserIndex].Level);

#ifdef ADD_LOG_400LEVELUP_PARTY_20070628	// 400레벨 달성시 같이 있던 파티원들 로그 추가
		if(gObj[iUserIndex].Level == 400 && gObj[iUserIndex].PartyNumber >=0)
		{
			int partynum = gObj[iUserIndex].PartyNumber;
			char partyLog[256];
			int number;
			sprintf(partyLog, "400 LevelUp (%s)(%s) Party ", gObj[iUserIndex].AccountID, gObj[iUserIndex].Name);
			
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
	
	return iLEFT_EXP;
}

#endif