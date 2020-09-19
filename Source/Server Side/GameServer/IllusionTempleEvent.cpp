// IllusionTempleEvent.cpp: implementation of the CIllusionTempleEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IllusionTempleEvent.h"

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#include "DSProtocol.h"
#include "..\\Common\\Winutil.h"
#include "Gamemain.h"
#include "DirPath.h"
#include "Crywolf.h"
#include "CastleSiegeSync.h"
#include "protocol.h"
#include "PartyClass.h"
#include "..\\Include\\ReadScript.h"
#include "wsJoinServerCli.h"

#ifdef MODIFY_MIX_SYSTEM_20070518
#include "MixSystem.h"
#endif

extern CDirPath				gDirPath;
extern PartyClass			gParty;
extern wsJoinServerCli		wsRServerCli;			//  랭킹서버
extern VOID CGPShopReqClose				(int aIndex);

#ifdef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
#include "DevilSquare.h"
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIllusionTempleEvent g_IllusionTempleEvent;


CIllusionTempleEvent::CIllusionTempleEvent()
{

}

CIllusionTempleEvent::~CIllusionTempleEvent()
{

}

BOOL CIllusionTempleEvent::IllusionTempleEventInit()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_COUNT; i++)
	{
		m_cIllusionTempleProc[i].IllusionTempleProcessInit(i);
	}

	return TRUE;
}

BOOL CIllusionTempleEvent::LoadIllusionTempleEventInfo()
{
	LoadIllusionTempleScript(gDirPath.GetNewPath("IllusionTemple.dat"));
	g_bIllusionTampleEvent				= GetPrivateProfileInt("GameServerInfo", "IllusionTempleEvent",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	g_iOldScrollDropRate				= GetPrivateProfileInt("GameServerInfo", "OldScrollDropRate",				0,	gDirPath.GetNewPath("commonserver.cfg"));
	g_iCovenantOfIllusionDropRate		= GetPrivateProfileInt("GameServerInfo", "CovenantOfIllusionDropRate",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	g_bUseMaxLevelIllusionTemple		= GetPrivateProfileInt("GameServerInfo", "UseMaxLevelIllusionTemple",		0,	gDirPath.GetNewPath("commonserver.cfg"));;
	g_bSaveIllusionTempleRankingPoint	= GetPrivateProfileInt("GameServerInfo", "SaveIllusionTempleRankingPoint",	0,	gDirPath.GetNewPath("commonserver.cfg"));;

#ifdef UPDATE_ILLUSION_TEMPLE_SCRIPT_20070710	// commonserver.cfg읽기
	g_iIllusionTempleRewardPercent		= GetPrivateProfileInt("GameServerInfo", "IllusionTempleRewardPercent",		0,	gDirPath.GetNewPath("commonserver.cfg"));
	g_btIllusionTempleMinUseSkillCnt	= GetPrivateProfileInt("GameServerInfo", "IllusionTempleMinUseSkillCnt",	0,	gDirPath.GetNewPath("commonserver.cfg"));
#endif
	
#ifdef UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320
	g_iIllusionTempleRewardPercentMaster = GetPrivateProfileInt("GameServerInfo", "IllusionTempleRewardPercentMaster",	0,	gDirPath.GetNewPath("commonserver.cfg"));
#endif //UPDATE_ADD_EVENT_MAP_EXP_RATE_20080320

	return TRUE;
}

BOOL CIllusionTempleEvent::LoadIllusionTempleScript(char* filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("[Illusion Temple] Info file Load Fail [%s]", filename);
		return FALSE;
	}
	SMDToken Token;
	
	int  type;
	
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

				m_nIllusionTempleOpenTime							= (int)TokenNumber;
				Token = (*GetToken)(); m_nIllusionTempleReadyTime	= (int)TokenNumber;
				Token = (*GetToken)(); m_nIllusionTemplePlayTime	= (int)TokenNumber;
				Token = (*GetToken)(); m_nIllusionTempleEndTime		= (int)TokenNumber;
			}
			else if( type == 1)
			{
				ILLUSION_TEMPLE_STARTTIME startTime;

				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;
		
				startTime.nHour = (int)TokenNumber;
				Token = (*GetToken)(); startTime.nMin = (int)TokenNumber;

				m_lstIllusionTempleStartTime.push_back(startTime);
			}
		}
	}
	fclose(SMDFile);

	LogAdd("%s file load!", filename);
	return TRUE;
}

void CIllusionTempleEvent::IllusionTempleRun()
{
#ifdef MODIFY_ILLUSION_TEMPLE_SCEDULE_20070703	// 환영사원 비활성화 시 스케줄도 돌아가면 안됨
	if(!g_bIllusionTampleEvent)
		return;
#endif
	
	int nCurTime = GetTickCount();

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724	// 사원시작 10분전에 전체사원의 CheckSync를 한번 더해준다.
	BOOL bCheckSync	=	FALSE;
	if(m_cIllusionTempleProc[0].GetRealRemainTimeMin() == 10 
		&& m_cIllusionTempleProc[0].GetAllChecksync() == FALSE)
	{
		bCheckSync = TRUE;
		m_cIllusionTempleProc[0].SetAllCheckSync();
	}
#endif
	
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_COUNT; i++)
	{
		if(!g_bUseMaxLevelIllusionTemple)
		{
#ifndef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010			// 환영사원6번 맵 사용한다.
			if(i == 5)	// 환영사원6번 맵은 사용안함
			{
				continue;
			}
#endif
		}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		if(bCheckSync == TRUE)
		{
			m_cIllusionTempleProc[i].CheckSync();
		}
#endif

		m_cIllusionTempleProc[i].ProcIllusionTemple(nCurTime);
	}
}

// 성물이 들어있는 인벤토리 위치를 저장
void CIllusionTempleEvent::SetInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SetInventoryPos(aIndex, btPos);
}

// 성물을 집은사람을 사용자들에게 전송
void CIllusionTempleEvent::SendRelicsUser(LPOBJECTSTRUCT lpObj)
{
	if (!CHECK_LIMIT(lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SendRelicsUser(lpObj);
}

// 이벤트맵 입장
BOOL CIllusionTempleEvent::EnterIllusionTemple( int aIndex, BYTE btTempleIndex, BYTE btItemPos )
{
#ifdef MODIFY_ILLUSION_TEMPLE_SCEDULE_20070703	// 환영사원 비활성화 시 입장 안됨
	if(!g_bIllusionTampleEvent)
		return FALSE;
#endif
	
	if (!CHECK_LIMIT(btTempleIndex, MAX_ILLUSION_TEMPLE_COUNT))
		return FALSE;

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	PMSG_RESULT_ENTER_ILLUSION_TEMPLE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x00, sizeof(pResult));
	pResult.Result = 0;

#ifndef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010		// 환영 사원6 사용한다.
	// 환영 사원6은 사용안함
	if(!g_bUseMaxLevelIllusionTemple)
	{
		if(btTempleIndex == 5)
		{
			return FALSE;
		}
	}
#endif

	if ((lpObj->Type != OBJTYPE_CHARACTER) || (lpObj->Connected <= 2)) 
	{
		return FALSE;
	}

	// *> . 현재 인터페이스가 사용 중인지 체크한다.
	if (lpObj->m_IfState.use != 0 && lpObj->m_IfState.type != I_ETCWINDOW ) 
	{
		return FALSE;
	}

	// *> . 개인상점 개설 중이었으면 상점을 강제로 해제시킴
	if (lpObj->m_bPShopOpen == true) 
	{
		CGPShopReqClose (lpObj->m_Index);
	}

	// 카오 유저는 입장할 수 없다
	BOOL	PKFlag = FALSE;
	if(lpObj->PartyNumber >= 0)	// 파티에 속할경우
	{
		if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= PK_LEVEL_DEFAULT+2)
		{
			PKFlag = TRUE;
		}
	}
	else
	{
		if(lpObj->m_PK_Level >= PK_LEVEL_DEFAULT+1)
		{
			PKFlag = TRUE;
		}
	}
	
	if(PKFlag == TRUE)
	{
		pResult.Result = 7;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	// 변신 반지 착용중이면 입장할 수 없다.
	if( CheckWearingMOPH(aIndex))
	{
		pResult.Result = 8;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	// 1 . 지금이 들어갈 수 있는 시간인지 확인한다.
	if(m_cIllusionTempleProc[btTempleIndex].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE 
		|| m_cIllusionTempleProc[btTempleIndex].GetEntrance() == FALSE)
	{
		pResult.Result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	// 2 . 케릭터의 인벤의 해당 위치에 피의 두루마리 아이템이 있는지 확인한다.
	int iITEM_LEVEL;
	DWORD dwITEM_SERIAL;
	if( gObj[aIndex].pInventory[btItemPos].IsItem() == TRUE )
	{
		if( gObj[aIndex].pInventory[btItemPos].m_Type == MAKE_ITEMNUM(13, 51) )		// 피의 두루마리가 존재한다면
		{
			iITEM_LEVEL = gObj[aIndex].pInventory[btItemPos].m_Level;				// 아이템 레벨을 얻는다.
			dwITEM_SERIAL = gObj[aIndex].pInventory[btItemPos].m_Number;			// 아이템 시리얼을 얻는다.
			if (!CHECK_LIMIT(iITEM_LEVEL-1, MAX_ILLUSION_TEMPLE_COUNT)				// 0 < iITEM_LEVEL-1 < 최대사원개수 
				&& gObj[aIndex].pInventory[btItemPos].m_Durability != 1)			// 아이템 내구도
			{				
				pResult.Result = 1;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return FALSE;
			}
		}
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		else if( gObj[aIndex].pInventory[btItemPos].m_Type == MAKE_ITEMNUM( 13, 61 ) )	// 환영사원 자유 입장권
		{
			dwITEM_SERIAL = gObj[aIndex].pInventory[btItemPos].m_Number;
			iITEM_LEVEL = CheckCanEnterLevel( aIndex );
			if( iITEM_LEVEL <= 0 )
			{
				pResult.Result = 1;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return FALSE;
			}
		}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		else 
		{
			pResult.Result = 1;		// 피의 두루마리가 없음
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return FALSE;
		}
	}
	else {
		pResult.Result = 1;			// 해당 위치에 아이템이 없음
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 입장횟수 정보 요청
	//////////////////////////////////////////////////////////////////////////
#if TESTSERVER == 0
	if( GEReqIllusionTempleEnterCountCheck(aIndex, btTempleIndex, btItemPos, iITEM_LEVEL) )
	{
		return TRUE;
	}
#if defined ( UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925 ) && !defined ( FOR_JAPAN )
	else
	{
		return FALSE;
	}
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
#endif

	//////////////////////////////////////////////////////////////////////////
	// 무조건 입장
	//////////////////////////////////////////////////////////////////////////
	// 3 . 해당 케릭의 레벨과 맞는지 확인한다.
	if(!CheckEnterLevel(aIndex, iITEM_LEVEL))
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	// 4 . 입장 시키기 전에 해당 사원에 사용자 중 접속을 않한 인원을 걸러낸다.
	m_cIllusionTempleProc[iITEM_LEVEL-1].CheckUsersOnConnect();

	// 5 . 해당 사원에 인원제한에 걸리지 않는지 확인한다.
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	BOOL bRet =  m_cIllusionTempleProc[iITEM_LEVEL-1].EnterUserIllusionTemple(aIndex, btTempleIndex, btItemPos);
#else
	BOOL bRet =  m_cIllusionTempleProc[iITEM_LEVEL-1].EnterUserIllusionTemple(aIndex, btTempleIndex);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617	

	// 6 . 성공 -> 아이템 바꾸고 사용자를 이벤트 맵으로 이동시킨다.
	if(bRet == TRUE)
	{
		// *> . 파티를 맺고 있었다면 파티를 강제로 탈퇴시킴
		if( lpObj->PartyNumber >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			
			int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
			if( pindex >= 0 )
			{
				pMsg.Number = pindex;
				CGPartyDelUser( &pMsg, lpObj->m_Index);
			}
		}
		
		// 피의 두루마리의 내구도 변경
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		if( lpObj->pInventory[btItemPos].m_Type == MAKE_ITEMNUM( 13, 51 ) )
		{
			lpObj->pInventory[btItemPos].m_Durability = 0;
			GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
		}
		else if( lpObj->pInventory[btItemPos].m_Type == MAKE_ITEMNUM( 13, 61 ) )
		{
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
			// 내구도가 1이상일 경우만 내구도를 낮춘다.
			if (lpObj->pInventory[btItemPos].m_Durability > 0) {
				lpObj->pInventory[btItemPos].m_Durability--;
				GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
			}
#else
			lpObj->pInventory[btItemPos].m_Durability--;
			GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
		}
#else
		lpObj->pInventory[btItemPos].m_Durability = 0;
		GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

		// 로그수정
#ifdef PCBANG_ADVANTAGE
	#ifdef 	MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u). PCRoom:%d",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d). PCRoom:%d",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[btItemPos].m_Number, lpObj->m_bPCBangUser);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u). PCRoom:%d",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d). PCRoom:%d",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				btTempleIndex+1, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[btItemPos].m_serial, lpObj->m_bPCBangUser);
	#endif
#else
	#ifdef 	MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u).",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d).",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[btItemPos].m_Number);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u).",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d).",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				btTempleIndex+1, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[btItemPos].m_serial);
	#endif
#endif // PCBANG_ADVANTAGE

		// 성공 결과 전송
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		// 이벤트맵으로 이동
		gObjMoveGate(lpObj->m_Index, GATE_ILLUSION_TEMPLE1 + iITEM_LEVEL-1);	
	}

	m_cIllusionTempleProc[btTempleIndex].SendIllusionTempleState(ILLUSION_TEMPLE_STATE_NONE, lpObj->m_Index);

	SendIllusionTempleUserCount();

	return TRUE;
}

// 입장 횟수 검사 요청
BOOL CIllusionTempleEvent::GEReqIllusionTempleEnterCountCheck(int aIndex, BYTE btTempleIndex, BYTE btItemPos, int nItemLevel)
{
#ifndef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	if (!DevilSquareEventConnect) 
	{
		if (!IsDevilSquareEventConnected)
		{
			BOOL bRETVAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRETVAL == FALSE) {
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925

//----------------->
//일본 환영사원 랭킹 서버와 연결 안되도 이벤트 진행 가능 하게 수정 20081203(grooving)
#ifndef FOR_KOREA	
	if(DevilSquareEventConnect != 0)
	{
		return FALSE;
	}
#endif
//<-----------------

	// 입장횟수 요청
	PMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x12, sizeof(pMsg));
	memcpy (pMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy (pMsg.GameID, gObj[aIndex].Name, MAX_IDSTRING);
	pMsg.ServerCode		= gGameServerCode;
	pMsg.iObjIndex		= aIndex;
	pMsg.iItemPos		= btItemPos;
	pMsg.iItemLevel		= nItemLevel;
	pMsg.iTempleNumber	= btTempleIndex;

#ifdef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	DataSendRank((char*)&pMsg, pMsg.h.size);
#else	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925

	return TRUE;
}

// 입장 횟수 검사 응답
void CIllusionTempleEvent::EGAnsIllusionTempleEnterCountCheck(LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK aRecv)
{
	if (!CHECK_LIMIT(aRecv->iTempleNumber, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	if( !CHECK_LIMIT(aRecv->iObjIndex, MAX_OBJECT))
		return;

	if( !CHECK_LIMIT(aRecv->iItemPos, MAX_INVENTORY))
		return;
	
	LPOBJECTSTRUCT lpObj = &gObj[aRecv->iObjIndex];

	BYTE btTempleIndex = aRecv->iTempleNumber;

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	int iFreeTicketLevel = 0;
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
	
	PMSG_RESULT_ENTER_ILLUSION_TEMPLE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x00, sizeof(pResult));
	pResult.Result = 0;

	// 1일 입장제한을 초과하여 입장할 수 없다
	if(aRecv->bEnterCheck == 0)
	{
		pResult.Result = 5;
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 1 . 지금이 들어갈 수 있는 시간인지 확인한다.
	if(m_cIllusionTempleProc[btTempleIndex].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE 
		|| m_cIllusionTempleProc[btTempleIndex].GetEntrance() == FALSE)
	{
		pResult.Result = 2;
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 2 . 케릭터의 인벤의 해당 위치에 피의 두루마리 아이템이 있는지 확인한다.
	int iITEM_LEVEL;
	DWORD dwITEM_SERIAL;
	if( lpObj->pInventory[aRecv->iItemPos].IsItem() == TRUE )
	{
		if( lpObj->pInventory[aRecv->iItemPos].m_Type == MAKE_ITEMNUM(13, 51) )		// 피의 두루마리가 존재한다면
		{
			iITEM_LEVEL = lpObj->pInventory[aRecv->iItemPos].m_Level;				// 아이템 레벨을 얻는다.
			dwITEM_SERIAL = lpObj->pInventory[aRecv->iItemPos].m_Number;			// 아이템 시리얼을 얻는다.
			if (!CHECK_LIMIT(iITEM_LEVEL-1, MAX_ILLUSION_TEMPLE_COUNT)				// 0 < iITEM_LEVEL < 최대사원개수 + 1
				&& lpObj->pInventory[aRecv->iItemPos].m_Durability != 1)			// 아이템 내구도
			{				
				pResult.Result = 1;
				DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		if( lpObj->pInventory[aRecv->iItemPos].m_Type == MAKE_ITEMNUM( 13, 61 )
			&& ( lpObj->pInventory[aRecv->iItemPos].m_Durability > 0 )
		  )
		{
			dwITEM_SERIAL = lpObj->pInventory[aRecv->iItemPos].m_Number;
			// 사용자 레벨에 따른 입장가능한 사원의 레벨을 확인한다.
			iFreeTicketLevel = CheckCanEnterLevel( lpObj->m_Index );
			if( iFreeTicketLevel <= 0 )
			{
				// 입장 레벨이 되지 않는다.
				pResult.Result = 3;
				DataSend( aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size );
				return;
			}
			iITEM_LEVEL = iFreeTicketLevel;
		}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		else 
		{
			pResult.Result = 1;		// 피의 두루마리가 없음
			DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else {
		pResult.Result = 1;			// 해당 위치에 아이템이 없음
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 3 . 해당 케릭의 레벨과 맞는지 확인한다.
	if(!CheckEnterLevel(aRecv->iObjIndex, iITEM_LEVEL))
	{
		pResult.Result = 3;
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// 4 . 입장 시키기 전에 해당 사원에 사용자 중 접속을 않한 인원을 걸러낸다.
	m_cIllusionTempleProc[iITEM_LEVEL-1].CheckUsersOnConnect();

	// 5 . 해당 사원에 인원제한에 걸리지 않는지 확인한다.
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	BOOL bRet =  m_cIllusionTempleProc[iITEM_LEVEL-1].EnterUserIllusionTemple(aRecv->iObjIndex, btTempleIndex, aRecv->iItemPos);
#else
	BOOL bRet =  m_cIllusionTempleProc[iITEM_LEVEL-1].EnterUserIllusionTemple(aRecv->iObjIndex, btTempleIndex);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617

	// 6 . 성공 -> 아이템 바꾸고 사용자를 이벤트 맵으로 이동시킨다.
	if(bRet == TRUE)
	{
		// *> . 파티를 맺고 있었다면 파티를 강제로 탈퇴시킴
		if( lpObj->PartyNumber >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			
			int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
			if( pindex >= 0 )
			{
				pMsg.Number = pindex;
				CGPartyDelUser( &pMsg, lpObj->m_Index);
			}
		}
		
		// 피의 두루마리의 내구도 변경
#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
		if( iFreeTicketLevel != 0 )			// 환영사원 자유 입장권
		{
			lpObj->pInventory[aRecv->iItemPos].m_Durability--;
			GCItemDurSend( lpObj->m_Index, aRecv->iItemPos, lpObj->pInventory[aRecv->iItemPos].m_Durability, 0 );
		}
		else
		{
			lpObj->pInventory[aRecv->iItemPos].m_Durability = 0;
			GCItemDurSend(lpObj->m_Index, aRecv->iItemPos, lpObj->pInventory[aRecv->iItemPos].m_Durability, 0);
		}
#else
		lpObj->pInventory[aRecv->iItemPos].m_Durability = 0;
		GCItemDurSend(lpObj->m_Index, aRecv->iItemPos, lpObj->pInventory[aRecv->iItemPos].m_Durability, 0);
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

		// 로그수정
#ifdef PCBANG_ADVANTAGE
	#ifdef 	MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u). PCRoom:%d",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d). PCRoom:%d",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[aRecv->iItemPos].m_Number, lpObj->m_bPCBangUser);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u). PCRoom:%d",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d). PCRoom:%d",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				btTempleIndex+1, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[aRecv->iItemPos].m_serial, lpObj->m_bPCBangUser);
	#endif
#else
	#ifdef 	MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u).",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d).",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[aRecv->iItemPos].m_Number);
	#else
		#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u).",
		#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%d).",
		#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				btTempleIndex+1, lpObj->AccountID, lpObj->Name, lpObj->DbClass,
				lpObj->pInventory[aRecv->iItemPos].m_serial);
	#endif
#endif // PCBANG_ADVANTAGE
		// 성공 결과 전송
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);

		// 이벤트맵으로 이동
		gObjMoveGate(lpObj->m_Index, GATE_ILLUSION_TEMPLE1 + iITEM_LEVEL-1);	
	}
	else
	{
		pResult.Result = 4;
		DataSend(aRecv->iObjIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	m_cIllusionTempleProc[btTempleIndex].SendIllusionTempleState(ILLUSION_TEMPLE_STATE_NONE, aRecv->iObjIndex);

	SendIllusionTempleUserCount();

	//////////////////////////////////////////////////////////////////////////
	// 입장 횟수 증가 요청
	//////////////////////////////////////////////////////////////////////////
	GEReqIllusionTempleIncEnterCount(lpObj);
}

// 입장 횟수 증가 요청
void CIllusionTempleEvent::GEReqIllusionTempleIncEnterCount(LPOBJECTSTRUCT lpObj)
{
#ifndef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	if (!DevilSquareEventConnect) 
	{
		if (!IsDevilSquareEventConnected)
		{
			BOOL bRETVAL = GMRankingServerConnect(gDevilSquareEventServerIp, WM_USER+12);	// WM_ASS_RANKINGCLIMSG
			if (bRETVAL == FALSE) {
				return;
			}
		}
	}
	else
	{
		return;
	}
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925

	PMSG_REQ_ILLUSION_TEMPLE_INC_COUNT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x13, sizeof(pMsg));
	memcpy (pMsg.AccountID, lpObj->AccountID, MAX_IDSTRING);
	memcpy (pMsg.GameID, lpObj->Name, MAX_IDSTRING);
	pMsg.ServerCode		= gGameServerCode;
		
#ifdef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	DataSendRank((char*)&pMsg, pMsg.h.size);
#else	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);	
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
}

// 이벤트 맵에서 나감
BOOL CIllusionTempleEvent::LeaveIllusionTemple(int aIndex, BYTE btMapNumber)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return FALSE;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].LeaveUserIllusionTemple(aIndex);

	SendIllusionTempleUserCount();

	return TRUE;
}

// 입장할수 있는 레벨 체크
BOOL CIllusionTempleEvent::CheckEnterLevel(int nIndex, int nItemLevel)
{
	int nIllusionTempleNumber = -1;
	if(gObj[nIndex].Level < MIN_ENTER_LEVEL1)
	{
		nIllusionTempleNumber = -1;		
	}
	else if(gObj[nIndex].Level < MIN_ENTER_LEVEL2)
	{
		nIllusionTempleNumber = 0;
	}
	else if(gObj[nIndex].Level < MIN_ENTER_LEVEL3)
	{
		nIllusionTempleNumber = 1;
	}
	else if(gObj[nIndex].Level < MIN_ENTER_LEVEL4)
	{
		nIllusionTempleNumber = 2;
	}
	else if(gObj[nIndex].Level < MIN_ENTER_LEVEL5)
	{
		nIllusionTempleNumber = 3;
	}
	else if(gObj[nIndex].Level < MIN_ENTER_LEVEL6)
	{
		nIllusionTempleNumber = 4;
	}
#ifdef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if(gObj[nIndex].Level == MIN_ENTER_LEVEL6  && !gObj[nIndex].ThirdChangeUp)
	{
		nIllusionTempleNumber = 4;
	}
	else if(gObj[nIndex].Level == MIN_ENTER_LEVEL6 && gObj[nIndex].ThirdChangeUp)
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			nIllusionTempleNumber = 5;
		}
		else
		{
			nIllusionTempleNumber = 4;
		}
	}
#else	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if(gObj[nIndex].Level = MIN_ENTER_LEVEL6)
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			nIllusionTempleNumber = 5;
		}
		else
		{
			nIllusionTempleNumber = 4;
		}
	}
#endif	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else
	{
		nIllusionTempleNumber = -1;
	}

	if(nIllusionTempleNumber+1 == nItemLevel)
		return TRUE;

	return FALSE;
}

// 석상 행동
void CIllusionTempleEvent::IllusionHolyRelicsStatusAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IllusionHolyRelicsStatusAct(lpNpc, lpObj);	
}

// 성물 보관함행동(연합군)
void CIllusionTempleEvent::HolyRelicsBoxAlliedAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].HolyRelicsBoxAlliedAct(lpNpc, lpObj);	
}

// 성물 보관함행동(환영교)
void CIllusionTempleEvent::HolyRelicsBoxIllusionAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].HolyRelicsBoxIllusionAct(lpNpc, lpObj);	
}

// 미라쥬 행동
void CIllusionTempleEvent::PlatformMirageAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)	
{
	// *> . 현재 인터페이스가 사용 중인지 체크한다.
	if (lpObj->m_IfState.use != 0 && lpObj->m_IfState.type != I_ETCWINDOW ) {		
		return;
	}

#ifndef MODIFY_MIRAGE_NPC_ACT_CLOSESHOP_BUGFIX_20080123	
	// *> . 개인상점 개설 중이었으면 상점을 강제로 해제시킴
	if (lpObj->m_bPShopOpen == true) 
	{
		CGPShopReqClose (lpObj->m_Index);
	}
#endif

	// 카오인지 검사
	if( lpObj->m_PK_Level >= PK_LEVEL_DEFAULT + 1 )
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3400), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("카오유저는 입장할 수 없습니다.", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}

	// 임시로 레벨을 확인해서 해당 레벨별로 넣어줌
	int nIllusionTempleNumber = -1;
	if(lpObj->Level < MIN_ENTER_LEVEL1)
	{
		nIllusionTempleNumber = -1;		
	}
	else if(lpObj->Level < MIN_ENTER_LEVEL2)
	{
		nIllusionTempleNumber = 0;
	}
	else if(lpObj->Level < MIN_ENTER_LEVEL3)
	{
		nIllusionTempleNumber = 1;
	}
	else if(lpObj->Level < MIN_ENTER_LEVEL4)
	{
		nIllusionTempleNumber = 2;
	}
	else if(lpObj->Level < MIN_ENTER_LEVEL5)
	{
		nIllusionTempleNumber = 3;
	}
	else if(lpObj->Level < MIN_ENTER_LEVEL6)
	{
		nIllusionTempleNumber = 4;
	}
#ifdef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if(lpObj->Level == MIN_ENTER_LEVEL6 && !lpObj->ThirdChangeUp)
	{
		nIllusionTempleNumber = 4;
	}
	else if(lpObj->Level == MIN_ENTER_LEVEL6 && lpObj->ThirdChangeUp)
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			nIllusionTempleNumber = 5;
		}
		else
		{
			nIllusionTempleNumber = 4;
		}
	}
#else	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if(lpObj->Level = MIN_ENTER_LEVEL6)
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			nIllusionTempleNumber = 5;
		}
		else
		{
			nIllusionTempleNumber = 4;
		}
	}
#endif	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else
	{
		nIllusionTempleNumber = -1;
	}

	if( !CHECK_LIMIT(nIllusionTempleNumber, MAX_ILLUSION_TEMPLE_COUNT)) //nIllusionTempleNumber >= 0 && nIllusionTempleNumber < MAX_ILLUSION_TEMPLE_COUNT)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3401), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("환영 사원에 입장할 수 있는 레벨이 아닙니다.", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}

	// 피의 두루마리 아이템이 있는지 검사
#ifdef MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	int nItempos = 	m_cIllusionTempleProc[nIllusionTempleNumber].SearchTicketItem(lpObj->m_Index);
#else
	int nItempos = 	m_cIllusionTempleProc[nIllusionTempleNumber].SearchScollOfBloodItem(lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_FREE_TICK_20080617
	if(nItempos == -1)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3402), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("해당 레벨의 피의 두루마리가 존재하지 않습니다", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}

#ifdef MODIFY_ILLUSION_TEMPLE_20070629	// 미라주 클릭시 메시지 수정
	// 현재 게임상태 검사
	if(m_cIllusionTempleProc[nIllusionTempleNumber].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3403), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("해당 사원은 이미 게임중 입니다.", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}

	if(m_cIllusionTempleProc[nIllusionTempleNumber].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE
		|| m_cIllusionTempleProc[nIllusionTempleNumber].GetEntrance() == FALSE)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3404), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("입장시간이 아닙니다.", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}
#else
	// 현재 게임상태 검사
	if(m_cIllusionTempleProc[nIllusionTempleNumber].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE
		|| m_cIllusionTempleProc[nIllusionTempleNumber].GetEntrance() == FALSE)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		GCServerMsgStringSend(lMsg.Get(3403), lpObj->m_Index, 1);
#else
		GCServerMsgStringSend("해당 사원은 이미 게임중 입니다.", lpObj->m_Index, 1);
#endif // MODIFY_ILLUSIONTEMPLE_MESSAGE_20080417
		return;
	}
#endif

	// 입장가능 결과 전송
	PMSG_TALKRESULT		pResult;
	pResult.h.c				= PMHCE_BYTE;
	pResult.h.headcode		= 0x30;
	pResult.h.size			= sizeof( pResult );
	pResult.result			= 0x14;		// 환영 사원 입장 가능
	pResult.level1			= m_cIllusionTempleProc[nIllusionTempleNumber].GetRemainTimeMin();	// 입장까지 남은시간
	pResult.level2			= m_cIllusionTempleProc[nIllusionTempleNumber].GetUserCount();		// 입장한 유저수
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);	
}

// 성물 아이템을 지움
void CIllusionTempleEvent::DeleteRelicsItem(BYTE btMapNumber, LPOBJECTSTRUCT lpObj)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;
	
	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].DeleteRelicsItem(lpObj);
}

// 성물 아이템을 드랍
void CIllusionTempleEvent::DropRelicsItem(BYTE btMapNumber, int nIndex)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].DropRelicsItem(nIndex);
}



// Ready -> Play 로 바꾸거나 Play -> Ready 로 바꿀때 사용
void CIllusionTempleEvent::SetIllusionTempleState(BYTE btState, BYTE btMapNumber)
{
#if TESTSERVER == 0
	return;
#endif

	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;
	
	if( btState == ILLUSION_TEMPLE_STATE_READY)
	{
		if(m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_NONE)
			return;

		m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SetRemainTime();
	}
	else if( btState == ILLUSION_TEMPLE_STATE_PLAYING)
	{
		if(m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_READY)
			return;
		// 특정 이벤트 맵에 적용
		m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SetRemainTime();
	}
	else if(btState == ILLUSION_TEMPLE_STATE_END)
	{
		if(m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_PLAYING)
			return;
		
		// 특정 이벤트 맵에 적용
		m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SetRemainTime();
	}

}

// 환영 사원의 상태를 얻어옴
BYTE CIllusionTempleEvent::GetIllusionTempleState(BYTE btMapNumber)
{
	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState();
}

// 석상 등록
void CIllusionTempleEvent::AddNpcPosNum(BYTE btMapNumber, int nClass, int nPosNum)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].AddNpcPosNum(nClass, nPosNum);
}

// 몬스터 등록
void CIllusionTempleEvent::AddMonsterPosNum(BYTE btMapNumber, int nClass, int nPosNum)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].AddMonsterPosNum(nClass, nPosNum);
}

// NPC, 몬스터의 위치정보를 리셋
void CIllusionTempleEvent::ResetPosNum()
{
	for(int i = 0; i < MAX_ILLUSION_TEMPLE_COUNT; i++)
	{
		m_cIllusionTempleProc[i].ResetPosNum();
	}
}

BYTE CIllusionTempleEvent::GetUserTeam(BYTE btMapNumber, int nIndex)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return 0xFF;

	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetUserTeam(nIndex);
}

void CIllusionTempleEvent::SetStatusRegenTime(BYTE btMapNumber)
{
	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].SetStatusRegenTime();
}

void CIllusionTempleEvent::IllusionTempleChaosMixItem(LPOBJECTSTRUCT lpObj)
{
	lpObj->ChaosLock = TRUE;
	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;	

	int aIndex = lpObj->m_Index;

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int iCharmOfLuckCount = 0;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	// 낡은 두루마리, 환영교단의 서약 체크
	int iRET_VAL = CheckChaosMixItem(aIndex);

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	iCharmOfLuckCount = CheckCharmCount( aIndex );
	if( iCharmOfLuckCount > 10 )
	{
		iRET_VAL = 15;
	}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	if (CHECK_ILLUSION_TEMPLE(iRET_VAL - 1 + MAP_INDEX_ILLUSION_TEMPLE1)) {
		// 특정 레벨의 피의 두루마리 재료가 있다. => 피의 두루마리 조합을 한다.
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		if( !IllusionTempleChaosMix( aIndex, iRET_VAL, iCharmOfLuckCount ) )
#else
		if( !IllusionTempleChaosMix(aIndex, iRET_VAL) )
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			lpObj->ChaosLock = FALSE;
		return;
	}
	else {
		// 그외의 에러들
		switch(iRET_VAL) {
		case 0 :		// 예외 오류 - 이경우엔 일단 내려간다.
			{
			}
			break;
		case 8 :		// 다른 아이템이 존재 -> 기존의 카오스 시스템으로 넘김
			{
			}
			break;
		case 9 :		// 아이템 레벨이 맞지않음
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 10 :		// 혼석이 없다. (혼석은 가장 기본이 되므로 카오스조합 않됨)
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 11 :		// 환영 사원 카오스 아이템 중 하나가 없다. (적어도 한개는 있음)
			{
				pMsg.Result	= 0x0A;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 12 :		// 이벤트 아이템이 너무 많다.
			{
				pMsg.Result	= 0x08;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 13 :		// 조합에 필요한 돈이 모자르다. (사실 여기의 리턴값에는 없고 조합시 처리 된다.)
			{
				pMsg.Result	= 0x0B;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
		case 14 :		// 조합을 하려는 나의 레벨이 낮다.
			{
				pMsg.Result	= 0x09;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		case 15 :		// 행운의 부적 사용량 초과
			{
				pMsg.Result = 0xF0;
				DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size );
				lpObj->ChaosLock = FALSE;
				return;
			}
			break;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		default:
			{
			}
			break;
		}
	}	

	lpObj->ChaosLock = FALSE;
}

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
bool CIllusionTempleEvent::IllusionTempleChaosMix(int nIndex, int nLEVEL, int iCharmOfLuckCount)
#else
bool CIllusionTempleEvent::IllusionTempleChaosMix(int nIndex, int nLEVEL)
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
{
	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return false;
	
	if(!CHECK_ILLUSION_TEMPLE( MAP_INDEX_ILLUSION_TEMPLE1 + nLEVEL -1 ) )
		return false;
	
	BOOL bMIX_RESULT	= FALSE;		// 조합의 결과가 성공인지 실패인지 ?

	PMSG_CHAOSMIXRESULT	pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof( pMsg ));
	pMsg.Result		= 0x00;
	
	gObj[nIndex].ChaosLock = TRUE;
	LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", 
		gObj[nIndex].AccountID, 
		gObj[nIndex].Name, 
		nLEVEL);

	char chChaosLogText[20];

	wsprintf( chChaosLogText, "피의 두루마리 Mix,%d", nLEVEL );
	g_MixSystem.LogChaosItem(&gObj[nIndex], chChaosLogText );

	
	INT iMIX_SUCCESS_RATE = g_IllusionTempleChaosRate[nLEVEL - 1];
	if ( (iMIX_SUCCESS_RATE < 0) || (iMIX_SUCCESS_RATE > 100) ) {
		// 성공확률 요류 (자체오류 ->  거의 나올 확률은 없다.)
		DataSend( nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)",
			gObj[nIndex].AccountID, 
			gObj[nIndex].Name, 
			nLEVEL
			);
		return FALSE;
	}
	
	// MVP 성공 : 카오스 조합 성공률 증가 ( 환영 사원 입장권)
	if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE 
		&& g_iCrywolfApplyMvpBenefit	
	  )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}

	
	INT iMIX_NEED_MONEY = g_IllusionTempleChaosMixZen[nLEVEL-1];
	
	// 카오스 시스템 세율적용 - 환영 사원 피의 두루마리 조합
	INT iChaosTaxMoney		= (INT)((INT64)iMIX_NEED_MONEY * (INT64)g_CastleSiegeSync.GetTaxRateChaos(nIndex) / (INT64)100);
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney	= 0;

	iMIX_NEED_MONEY			= iMIX_NEED_MONEY + iChaosTaxMoney;

	if ( iMIX_NEED_MONEY < 0 ) {
		// 돈 오류 (자체오류 ->  거의 나올 확률은 없다.)
		DataSend( nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", 
			gObj[nIndex].AccountID, 
			gObj[nIndex].Name, 
			nLEVEL
			);
		return FALSE;
	}
	
	if( (gObj[nIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		// 돈이 모자르다. (0x86, Result: 0x0B 로 알림)
		pMsg.Result	= 0x0B;
		DataSend( nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", 
			gObj[nIndex].AccountID, 
			gObj[nIndex].Name, 
			nLEVEL
			);
		return FALSE;
	}
	
	gObj[nIndex].Money -= iMIX_NEED_MONEY;

	// 카오스 조합 세금을 저장 - 환영 사원 피의 두루마리 조합
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);

	GCMoneySend(nIndex, gObj[nIndex].Money);

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	iMIX_SUCCESS_RATE += iCharmOfLuckCount;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	
	if (rand()%100 < iMIX_SUCCESS_RATE) 
	{
		// 확률이 적용되어 조합에 성공했다. => 만들어 진다.
		int iItemType  = MAKE_ITEMNUM(13, 51);

		ItemSerialCreateSend(nIndex, 255, 0, 0, iItemType, nLEVEL, 0, 0, 0, 0);

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", 
			   gObj[nIndex].AccountID, gObj[nIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY,
			   iCharmOfLuckCount );
#else
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Success %d Money : %d-%d", 
			   gObj[nIndex].AccountID, gObj[nIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY);
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	}
	else {
		// 조합에 실패했다.
		g_MixSystem.ChaosBoxInit(&gObj[nIndex]);
		GCUserChaosBoxSend(&gObj[nIndex]);
		DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",  
			   gObj[nIndex].AccountID, gObj[nIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY,
			   iCharmOfLuckCount );
#else
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Fail %d Money : %d-%d",  
			   gObj[nIndex].AccountID, gObj[nIndex].Name, 
			   iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY);
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
		
		return FALSE;
	}
	
	gObjInventoryCommit(nIndex);
	
	return TRUE;
}

// 해당 사용자가 피의 두루마리를 만들 재료 (낡은 두루마리, 환영교단의 서약) 를 가지고 있는지 확인 
// (0 이면 없음 / 1 ~ 7 은 각 레벨의 재료) / 8 은 다른아이템 발견 / 9는 레벨 않맞음 / 10은 혼석없음 / 11은 아이템중 하나 없음 / 12는 이벤트아이템이 너무 많음 / 13은 돈이 모자르다. / 14 은 내 레벨이 낮다.
int CIllusionTempleEvent::CheckChaosMixItem(int nIndex)
{
	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return 0;

	INT iCHAOS_MIX_LEVEL			= 0;					// 몇레벨의 입장권을 생산할 수 있는가 ?

	BOOL bIsChaosGemExist			= FALSE;				// 혼돈의 보석이 있는지 ?
	BOOL bIsOldScrollExist			= FALSE;				// 낡은 두루마리가 있는지 ?
	BOOL bIsCovenantOfIllusionExist	= FALSE;				// 환영교단의 서가 있는지 ?
	BOOL bIsOtherItemExist			= FALSE;				// 다른 아이템이 존재하는지 ?

	INT iEventItemCount				= 0;

	INT iOldScroolLevel				= 0;
	INT iCovenantOfIllusionLevel	= 0;

#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
	int iCharmOfLuckCount			= 0;
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011

	for( int i=0; i<MAX_CHAOSBOXITEMS; i++)
	{
		if( gObj[nIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if( gObj[nIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(12, 15) )
			{	// 혼돈의 보석
				bIsChaosGemExist	= TRUE;
			}
			else if( gObj[nIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 49) )
			{	// 낡은 두루마리
				INT iITEM_LEVEL		= gObj[nIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsOldScrollExist	= TRUE;

				iOldScroolLevel		= iITEM_LEVEL;
			}
			else if( gObj[nIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM(13, 50) )
			{	// 환영교단의 서
				INT iITEM_LEVEL = gObj[nIndex].pChaosBox[i].m_Level;

				iEventItemCount++;
				bIsCovenantOfIllusionExist	= TRUE;

				iCovenantOfIllusionLevel	= iITEM_LEVEL;
			}
#ifdef MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else if( gObj[nIndex].pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += gObj[nIndex].pChaosBox[i].m_Durability;
			}
#endif // MODIFY_PCS_LUCKY_CHARM_TICKET_3RD_WING_20071011
			else
			{	// 이외의 아이템이 올려져 있다
				bIsOtherItemExist = TRUE;
			}
		}		
	}

	if (bIsOtherItemExist) {
		return 8;		// 다른 아이템 발견 -> 기존의 카오스 시스템으로 넘긴다.
	}

	if ((!bIsOldScrollExist) && (!bIsCovenantOfIllusionExist)) {
		return 0;		// 환영 사원이 아닌 다른 조합일 수 있으므로 넘어간다.
	}

	else if ((!bIsOldScrollExist) || (!bIsCovenantOfIllusionExist)) {
		return 11;		// 낡은 두루마리, 환영교단의 서 중 하나가 없음
	}

	if (iEventItemCount > 2) {
		return 12;		// 이벤트 아이템이 너무 많다.
	}

	if (iOldScroolLevel != iCovenantOfIllusionLevel) {
		return 9;		// 아이템 레벨이 맞지 않음
	}
	else {
		if (!CHECK_LIMIT(iOldScroolLevel-1, MAX_ILLUSION_TEMPLE_COUNT)) 
			return 9;

		if (!CHECK_LIMIT(iCovenantOfIllusionLevel-1, MAX_ILLUSION_TEMPLE_COUNT))
			return 9;
	}

	if (!bIsChaosGemExist) {
		return 10;		// 혼석 없음
	}

	// 환영 사원 카오스 조합을 할 수 있는 최소레벨 체크
	if (gObj[nIndex].Level < ILLUSION_TEMPLE_CHAOS_MIX_MIN_LEVEL) {
		return 14;
	}

	if (bIsChaosGemExist && bIsOldScrollExist && bIsCovenantOfIllusionExist) {
		// 모든 조건이 일치하므로 낡은 두루마리 레벨을 보냄 (둘중 아무거나)
		return iOldScroolLevel;	
	}

	return 0;			// 그외의 예상치 못한 다른 오류
}

// 스킬 포인트 증가
BYTE CIllusionTempleEvent::IncSkillPoint(int nIndex, BYTE btMapNumber, BYTE btObjectType)
{
	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return 0;

	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return 0;

	BYTE btPoint = 0;
	if(btObjectType == OBJTYPE_CHARACTER)
	{
		m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IncUserKillCount(nIndex);
		btPoint = INC_SKILL_POINT_CHARACTER;	
		
#ifndef  ADD_LOG_ILLUSION_TEMPLE_1_20070727		
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) Chracter Killed, (point: %d)", 
			btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1 +1, gObj[nIndex].AccountID, gObj[nIndex].Name, btPoint);
#endif
	}
	else
	{
		m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IncMonsterKillCount(nIndex);
		btPoint = INC_SKILL_POINT_MONSTER;

#ifndef  ADD_LOG_ILLUSION_TEMPLE_1_20070727
		LogAddTD("[Illusion Temple] (%d) (%s)(%s) Monster Killed, (point: %d)", 
			btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1 +1, gObj[nIndex].AccountID, gObj[nIndex].Name, btPoint);
#endif
	}

	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IncSkillPoint(nIndex, btPoint);
}

// 스킬 포인트 감소
BYTE CIllusionTempleEvent::DecSkillPoint(int nIndex, BYTE btMapNumber, BYTE btDecPoint)
{
	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return 0;

	if (!CHECK_LIMIT(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1, MAX_ILLUSION_TEMPLE_COUNT))
		return 0;

	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].DecSkillPoint(nIndex, btDecPoint);
}

// 전용스킬 사용
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107		// 스킬번호 BYTE -> WORD
void CIllusionTempleEvent::IllusionTempleUseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis)
#else
void CIllusionTempleEvent::IllusionTempleUseSkill(int iIndex, BYTE MagicNumber, int wTargetObjIndex, BYTE btDis)
#endif
{
	if(!CHECK_LIMIT( iIndex, MAX_OBJECT ) )
		return;

	if(!CHECK_ILLUSION_TEMPLE(gObj[iIndex].MapNumber))
		return;
	
	m_cIllusionTempleProc[gObj[iIndex].MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IllusionTempleUseSkill(iIndex, MagicNumber, wTargetObjIndex, btDis);
}

void CIllusionTempleEvent::EventSkillProc(LPOBJECTSTRUCT lpObj)
{
	m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IllusionTempleSkillProc(lpObj);
}

// 사용자가 죽었을 때의 처리
void CIllusionTempleEvent::IllusionTempleUserDie(LPOBJECTSTRUCT lpObj)
{
	m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IllusionTempleUserDie(lpObj);
}

// 사용자가 죽었다 다시 살아날때의 처리
void CIllusionTempleEvent::IllusionTempleUserDieRegen(LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT( lpObj->m_Index, MAX_OBJECT ) )
		return;

	if(!CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
		|| m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_PLAYING)
		return;

	m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].UseSkillProdection(lpObj);
}

// 사용자의 스킬 사용 횟수 처리
void CIllusionTempleEvent::IncUseSkillCount(int nIndex, BYTE btMapNumber)
{
	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return;

	if(!CHECK_ILLUSION_TEMPLE(btMapNumber) 
		|| m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_PLAYING)
		return;

	m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].IncUseSkillCount(nIndex);
}

// 미라쥬 인덱스 저장
void CIllusionTempleEvent::SetMirageIndex(int nIndex)
{
	m_nMirageIndex = nIndex;
}

// 미라쥬를 보고 있는 사람들에게 환영 사원 인원 수를 전송
void CIllusionTempleEvent::SendIllusionTempleUserCount()
{
	if(m_cIllusionTempleProc[1].GetIllusionTempleState() == ILLUSION_TEMPLE_STATE_NONE 
		&& m_cIllusionTempleProc[1].GetEntrance() == TRUE)
	{
		PMSG_ILLUSION_TEMPLE_USER_COUNT  pMsg;
	
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x03, sizeof( pMsg ));
		pMsg.btUserCount[0]		= m_cIllusionTempleProc[0].GetUserCount();	
		pMsg.btUserCount[1]		= m_cIllusionTempleProc[1].GetUserCount();	
		pMsg.btUserCount[2]		= m_cIllusionTempleProc[2].GetUserCount();	
		pMsg.btUserCount[3]		= m_cIllusionTempleProc[3].GetUserCount();	
		pMsg.btUserCount[4]		= m_cIllusionTempleProc[4].GetUserCount();
		
		if(!g_bUseMaxLevelIllusionTemple)
		{
			pMsg.btUserCount[5]		= m_cIllusionTempleProc[5].GetUserCount();	
		}
		else
		{
			pMsg.btUserCount[5] = 0;
		}

		MsgSendV2(&gObj[m_nMirageIndex], (LPBYTE)&pMsg, pMsg.h.size);		
	}
}

// 해당 사용자가 변신반지를 착용하고 있는지 확인한다.
BOOL CIllusionTempleEvent::CheckWearingMOPH(int nUserIndex)
{
	if (!gObjIsConnected(nUserIndex)) {
		// 사용자가 접속상태가 아니라면 에러
		return FALSE;
	}

	if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].IsItem())
	{							
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			return TRUE;
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 입장 금지
		// 엘리트해골전사변신반지(이벤트용)
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{			
			return TRUE;
		}
#endif
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		// 잭 오 랜턴 변신반지
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			return TRUE;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		// 산타크로스 변신반지
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) ) // 임시
		{
			return TRUE;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

//------------->
//20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			return TRUE;
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			return TRUE;
		}
#endif // ADD_PREMIUMITEM_PANDA
	}	

	if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].IsItem())
	{							
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			return TRUE;
		}
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 입장 금지
		// 엘리트해골전사변신반지(이벤트용)
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{			
			return TRUE;
		}
#endif
		
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			return TRUE;
		}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			return TRUE;
		}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

//------------->
//20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			return TRUE;
		}
#endif // ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
		if( gObj[nUserIndex].pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			return TRUE;
		}
#endif // ADD_PREMIUMITEM_PANDA
	}	

	return FALSE;
}

// 해당 아이템이 변신잔지인지 검사
BOOL CIllusionTempleEvent::CheckChangeRing(int nType)
{
	BOOL bRet = FALSE;

	switch(nType)
	{
	case MAKE_ITEMNUM2(13,10):		// 변신반지
	case MAKE_ITEMNUM2(13,39):		// 엘리트해골전사변신반지(이벤트용)
	case MAKE_ITEMNUM2(13,40):		// 잭 오 랜턴 변신반지
	case MAKE_ITEMNUM2(13,41):		// 산타크로스 변신반지
	case MAKE_ITEMNUM2(13,68):		// 20081013 해외 크리스마스 이벤트 - 변신반지(grooving)
#ifdef ADD_PREMIUMITEM_PANDA
	case MAKE_ITEMNUM2(13, 76):
#endif // ADD_PREMIUMITEM_PANDA
		bRet = TRUE;
		break;
	default:
		break;
	}

	return bRet;
}

// 캐릭터가 성물을 가지고 있는지 검사
BOOL CIllusionTempleEvent::CheckRelics(int nIndex)
{
	if (!gObjIsConnected(nIndex)) {
		// 사용자가 접속상태가 아니라면 에러
		return FALSE;
	}

	if(!CHECK_LIMIT( nIndex, MAX_OBJECT ) )
		return FALSE;

	if(!CHECK_ILLUSION_TEMPLE(gObj[nIndex].MapNumber) 
		|| m_cIllusionTempleProc[gObj[nIndex].MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState() != ILLUSION_TEMPLE_STATE_PLAYING)
		return FALSE;

	if(nIndex == m_cIllusionTempleProc[gObj[nIndex].MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].GetRelicsUserIndex())
	{
		return TRUE;
	}

	return FALSE;
}

// 몬스터가 죽었을 경우 아이템 드랍
void CIllusionTempleEvent::MonsterDieItemDrop(LPOBJECTSTRUCT lpObj)					
{
	if(!CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
		return;

	m_cIllusionTempleProc[lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].MonsterDieItemDrop(lpObj);
}

void CIllusionTempleEvent::ReqEventReward(int nIndex)
{
	if(!CHECK_ILLUSION_TEMPLE(gObj[nIndex].MapNumber))
		return;

	m_cIllusionTempleProc[gObj[nIndex].MapNumber - MAP_INDEX_ILLUSION_TEMPLE1].DropChaosGem(nIndex);
}

BYTE CIllusionTempleEvent::GetRemainTime()
{
	BYTE nRemainTime = 0;

	if(g_bIllusionTampleEvent)
	{
		nRemainTime = m_cIllusionTempleProc[0].GetEnterRemainTimeMin();

	}
	else
	{
		nRemainTime = 0;
	}

	return nRemainTime;
}

#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
BOOL CIllusionTempleEvent::CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if(!CHECK_ILLUSION_TEMPLE(btMapNumber))
		return FALSE;

	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].CheckSkillProdection(iIllusionTempleIndex);
}

BOOL CIllusionTempleEvent::CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if(!CHECK_ILLUSION_TEMPLE(btMapNumber))
		return FALSE;

	return m_cIllusionTempleProc[btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1].CheckSkillRestraint(iIllusionTempleIndex);
}
#endif

#ifdef ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011
int CIllusionTempleEvent::CheckCanEnterLevel(int aIndex)
{
	int iLevel = 0;
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];

	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if( lpObj->Level < MIN_ENTER_LEVEL1 )
	{
		return 0;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL2 )
	{
		return 1;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL3 )
	{
		return 2;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL4 )
	{
		return 3;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL5 )
	{
		return 4;
	}
	else if( lpObj->Level < MIN_ENTER_LEVEL6 )
	{
		return 5;
	}
#ifdef ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if( lpObj->Level == MIN_ENTER_LEVEL6 && !lpObj->ThirdChangeUp )
	{
		return 5;
	}
	else if( lpObj->Level == MIN_ENTER_LEVEL6 && lpObj->ThirdChangeUp )
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			return 6;
		}
		else
		{
			return 5;
		}
	}
#else	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else if( lpObj->Level == MIN_ENTER_LEVEL6 )
	{
		if(g_bUseMaxLevelIllusionTemple)
		{
			return 6;
		}
		else
		{
			return 5;
		}
	}
#endif	// ADD_ILLUSIONTEMPLE_FOR_MASTER_LEVEL_20071010
	else
	{
		return -1;
	}

	return -1;
}

int CIllusionTempleEvent::CheckCharmCount(int aIndex)
{
	if( !CHECK_LIMIT( aIndex, MAX_OBJECT ) )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	int iCharmOfLuckCount = 0;

	for( int i=0; i<MAX_CHAOSBOXITEMS; i++)
	{
		if( lpObj->pChaosBox[i].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[i].m_Type == MAKE_ITEMNUM( 14, 53 ) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[i].m_Durability;
			}
		}
	}

	return iCharmOfLuckCount;
}
#endif // ADD_PCS_ILLUSION_TEMPLE_FREE_TICKET_20071011

#ifdef ADD_SEASON_3_NEW_UI_20071122
// [0x14] 환영사원의 남은 하루 입장횟수를 요청한다.
void CIllusionTempleEvent::EGReqIllusionTempleEnterCount(int iIndex)
{
	if (!gObjIsConnected(iIndex)) {
		return;
	}

	PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x14;
	pMsg.h.size = sizeof(pMsg);

	memcpy (pMsg.AccountID, gObj[iIndex].AccountID, sizeof(pMsg.AccountID));
	memcpy (pMsg.GameID, gObj[iIndex].Name, sizeof(pMsg.GameID));
	pMsg.ServerCode = gGameServerCode / 20;
	pMsg.iObjIndex = iIndex;

#ifdef UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	DataSendRank((char*)&pMsg, pMsg.h.size);
#else	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
	wsRServerCli.DataSend((char*)&pMsg, pMsg.h.size);
#endif	// UPDATE_ILLUTIONTEMPLE_RANKINGSERVER_SENDDATA_FIX_20080925
}

// [0x14] 환영사원의 남은 하루 입장횟수를 받는다.
void CIllusionTempleEvent::EGAnsIllusionTempleEnterCount(LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT lpMsg)
{
	if (lpMsg == NULL) {
		return;
	}

	if (!gObjIsConnected(lpMsg->iObjIndex)) {
		return;
	}

	CHAR szAccountID[MAX_IDSTRING+1] = {0,};
	CHAR szName[MAX_IDSTRING+1] = {0,};
	memcpy (szAccountID, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	memcpy (szName, lpMsg->GameID, sizeof(lpMsg->GameID));

	if (strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		strcmp(gObj[lpMsg->iObjIndex].Name, szName)
		)
	{
		// 최초 환영사원 남은 횟수를 요청했던 케릭이 아니라면 보내지 않음
		return;
	}

	// 클라이언트에게 남은 횟수를 보내준다.
	PMSG_ANS_CL_ENTERCOUNT pMsgSend;
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 3;	// 환영사원
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	DataSend (lpMsg->iObjIndex, (LPBYTE) &pMsgSend, sizeof(pMsgSend));
}
#endif	// ADD_SEASON_3_NEW_UI_20071122

#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
