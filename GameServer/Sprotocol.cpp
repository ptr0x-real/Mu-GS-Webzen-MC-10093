//----------------------------------------------------------------------------
// GAME SERVER - SPROTOCOL.CPP
// 서버와의 정보를 주고받는 프로토콜 처리 모듈
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "User.h"
#include "Protocol.h"
#include "GameServer.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "GameMain.h"
#include "DragonEvent.h"

#ifdef ADD_MANAGER_SENDMAIL
#include "EDSProtocol.h"
#endif

#ifdef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
	#include "DSProtocol.h"
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
#include "AntiHack.h"
#endif

//----------------------------------------------------------------------------
// E X T E R N
extern CwsGameServer	wsGServer;
extern wsJoinServerCli  wsJServerCli;
extern wsJoinServerCli  wsDataCli;
extern CDragonEvent*	DragonEvent;

//----------------------------------------------------------------------------
// F U N C T I O N 
//----------------------------------------------------------------------------
void GJPAccountFail(short number, char * Id, int DBNumber, int UserNumber );
void JGServerLoginResult(LPSDHP_RESULT lpMsg);
void JGPAccountRequest(LPSDHP_IDPASSRESULT lpMsg);

/*void JGCharacterCreateRequest(LPSDHP_CREATECHARRESULT lpMsg);
void JGCharDelRequest(LPSDHP_CHARDELETERESULT lpMsg);
//void JGAccountCreateResult(LPSDHP_ACCOUNTRESULT lpMsg);
void JGGetCharacterInfo(LPSDHP_DBCHAR_INFORESULT lpMsg);
void JGPPassQuestionQueryRecv(LPSDHP_PASSQUESTION_QUERY_RESULT lpMsg);
void JGPPassQueryRecv(LPSDHP_PASSQUERY_RESULT lpMsg);
void JGPPassChangeRecv(LPSDHP_PASSCHANGE_RESULT lpMsg);
*/
void GJPBillCeckRecv(LPSDHP_SDHP_BILLSEARCH_RESULT lpMsg);
void GJPUserKillRecv(LPSDHP_BILLKILLUSER lpMsg);

void GJPUserDisconnectRecv(LPSDHP_BILLKILLUSER lpMsg);

void JGPEventStart(LPSDHP_EVENTSTART lpMsg);

void JGPEventControl(LPSDHP_EVENTCONTROL lpMsg);
void JGPOptionControl(LPSDHP_OPTIONCONTROL lpMsg);

void JGOtherJoin(LPSDHP_OTHERJOINMSG lpMsg);
void JGPExitCodeRecv(LPSDHP_EXITMSG lpMsg);
void LoveHeartEventRecv(LPSDHP_LOVEHEARTEVENT_RESULT lpMsg );
void LoveHeartCreateRecv(LPSDHP_LOVEHEARTCREATERECV lpMsg);

void AllNoticeRecv(LPSDHP_NOTICE lpMsg);
void UserNoticeRecv(LPSDHP_USER_NOTICE lpMsg);

#ifdef ADD_MANAGER_SENDMAIL
void JGPSendMail(LPPMSG_JG_MEMO_SEND lpMsg);
#endif


#ifdef MAP_SERVER_WORK_20041030		// 서버 맵분할 관련 패킷 - 게임서버 <-> 인증서버

//----------------------------------------------------------------------------
// GJ [0x7A] 인증서버에게 특정 사용자가 맵서버를 이동해야 함을 알림 (0xC1)
//----------------------------------------------------------------------------
void GJReqMapSvrMove		(INT iIndex, WORD wDestMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY);
//----------------------------------------------------------------------------
// JG [0x7A] 인증서버가 특정 사용자의 맵서버 이동에 대한 결과를 알림 (0xC1)
//----------------------------------------------------------------------------
void JGAnsMapSvrMove		(LPPMSG_ANS_MAPSVRMOVE lpMsg);

//----------------------------------------------------------------------------
// GJ [0x7B] 인증서버에게 특정 사용자가 맵서버 이동 후 인증을 요청함을 알림 (0xC1)		-> 인증서버는 해당 사용자의 게임서버 코드도 갱신
//----------------------------------------------------------------------------
void GJReqMapSvrAuth		(INT iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, INT iJA1, INT iJA2, INT iJA3, INT iJA4);
//----------------------------------------------------------------------------
// JG [0x7B] 인증서버에게 특정 사용자가 맵서버 이동 후 인증요청에 대한 결과 (0xC1)
//----------------------------------------------------------------------------
void JGAnsMapSvrAuth		(LPPMSG_ANS_MAPSVRAUTH lpMsg);

//----------------------------------------------------------------------------
// JG [0x7C] 특정 게임서버의 현재 최대 사용자 수를 주기적으로 보냄 (0xC1) 
//----------------------------------------------------------------------------
void GJNotifyMaxUserCount	();

#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
void GJRequestAntiHackStatistics(char* pchFileName);
void JGRequestAntiHackStatistics(LPPMSG_HACKTOOL_USE_STATISTICS lpMsg);
void GJRequestHacktoolMapMoveBlock(char* pchFilename, DWORD dwReservationTime, DWORD dwBlockTerm, WORD wBlockRate);
void JGRequestHacktoolMapMoveBlock(LPPMSG_RESERVE_HACKTOOL_MAP_MOVE lpMsg);
void GJAntiHackStatisticsResult(char* pchFilename, int iHacktoolUserCount);
#endif // ADD_HACKING_TOOL_BLOCK_20090311

//----------------------------------------------------------------------------
void SProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case 0x00 :
		JGServerLoginResult((LPSDHP_RESULT)aRecv);
		break;
	case 0x01: // 인증요청 결과를 처리
		JGPAccountRequest((LPSDHP_IDPASSRESULT)aRecv);
		break;
	case 0x06 :	// 남은 시간 확인
		GJPBillCeckRecv((LPSDHP_SDHP_BILLSEARCH_RESULT)aRecv);
		break;
	case 0x07 :
		GJPUserKillRecv((LPSDHP_BILLKILLUSER)aRecv);
		break;
	case 0x08 :
		JGOtherJoin((LPSDHP_OTHERJOINMSG)aRecv);
		break;
	case 0x09 :	// 튕처리
		GJPUserDisconnectRecv((LPSDHP_BILLKILLUSER)aRecv);
		break;
	case 0x20 :
		JGPExitCodeRecv((LPSDHP_EXITMSG)aRecv);
		break;
	case 0x60 :
		JGPEventStart((LPSDHP_EVENTSTART)aRecv);
		break;
	case 0x61 :
		JGPEventControl((LPSDHP_EVENTCONTROL)aRecv);
		break;
	case 0x62 :
		JGPOptionControl((LPSDHP_OPTIONCONTROL)aRecv);
		break;

#ifdef ADD_MANAGER_SENDMAIL
	case 0x63 :
		JGPSendMail((LPPMSG_JG_MEMO_SEND)aRecv);
		break;
#endif
	case 0x30 :
		LoveHeartEventRecv((LPSDHP_LOVEHEARTEVENT_RESULT)aRecv);
		break;
	case 0x40 :
		AllNoticeRecv((LPSDHP_NOTICE)aRecv);
		break;
	case 0x41 :
		UserNoticeRecv((LPSDHP_USER_NOTICE)aRecv);
		break;

#ifdef MAP_SERVER_WORK_20041030		// 서버 맵분할 관련 패킷 - 게임서버 <-> 인증서버
	case 0x7A :
		JGAnsMapSvrMove		((LPPMSG_ANS_MAPSVRMOVE)aRecv);
		break;
	case 0x7B :
		JGAnsMapSvrAuth		((LPPMSG_ANS_MAPSVRAUTH)aRecv);
		break;
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311
		// 해킹 툴 사용자 맵이동 차단에 대한 프로토콜
	case 0x8A:				// 인증서버의 해킹툴 통계 요청
		JGRequestAntiHackStatistics( (LPPMSG_HACKTOOL_USE_STATISTICS)aRecv );
		break;
	case 0x8B:				// 인증서버의 해킹툴 맵이동 차단 요청
		JGRequestHacktoolMapMoveBlock( (LPPMSG_RESERVE_HACKTOOL_MAP_MOVE)aRecv );
		break;
#endif // ADD_HACKING_TOOL_BLOCK_20090311
	}
}

//----------------------------------------------------------------------------
// [0x00] 인증서버에 접속할때 이 프로그램의 정보를 보낸다.
//----------------------------------------------------------------------------
void GJServerLogin()
{
	SDHP_SERVERINFO	   pInfo;

	pInfo.h.c		 = PMHC_BYTE;
	pInfo.h.size	 = sizeof(pInfo);
	pInfo.h.headcode = 0x00;
	pInfo.Port		 = GameServerPort;
	pInfo.Type		 = ST_GAMESERVER;
	strcpy( pInfo.ServerName, szServerName);

#ifdef JOINSERVER_USING_SERVERCODE
	pInfo.ServerCode = gGameServerCode;
#endif
	wsJServerCli.DataSend((char*)&pInfo, pInfo.h.size);
}

//----------------------------------------------------------------------------
// [0x00] 인증서버에 접속후 결과
//----------------------------------------------------------------------------
void JGServerLoginResult(LPSDHP_RESULT lpMsg)
{
	if( lpMsg->Result == FALSE )
	{
		LogAdd(lMsg.Get(466));
	}
	else 
	{
		JoinServerConnected = 1;
		gServerReady++;
		SendMessage(ghWnd, WM_ASS_CREATEGAMESERVER, 0, 0);
	}
}

//----------------------------------------------------------------------------
// [0x05] 유저가 접속을 종료했다. (ID)
//----------------------------------------------------------------------------
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
void GJPUserClose(char * szAccountID, INT iPlayTimeLimit)
#else
void GJPUserClose(char * szAccountID)
#endif
{
	SDHP_USERCLOSE_ID	pClose;

	pClose.h.c			= PMHC_BYTE;
	pClose.h.size		= sizeof( pClose );
	pClose.h.headcode	= 0x05;
	memcpy(pClose.szId, szAccountID, MAX_IDSTRING);
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
	pClose.iAddPlayTime	= iPlayTimeLimit;
#endif

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

void GJPUserClose2(LPSTR szAccountID)
{
	SDHP_USERCLOSE_ID pClose;

	pClose.h.c =0xC1;
	pClose.h.size= sizeof(SDHP_USERCLOSE_ID);
	pClose.h.headcode = 0x05;
	memcpy(pClose.szId , szAccountID, 10);

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

#ifdef LOGINSERVER_MULOG
// [0x05] 유저가 접속을 종료했다. (ID, Name, Class, Level)
void GJPUserClose(LPOBJECTSTRUCT lpObj)
{
	SDHP_USERCLOSE_ID	pClose;

	pClose.h.c			= PMHC_BYTE;
	pClose.h.size		= sizeof( pClose );
	pClose.h.headcode	= 0x05;
	memcpy(pClose.szId, lpObj->AccountID, MAX_IDSTRING);
	memcpy(pClose.szName, lpObj->BackName, MAX_IDSTRING);

	pClose.Level = lpObj->Level;
	pClose.DbClass = lpObj->DbClass;
#if defined(CHN_PLAYTIME_LIMIT_SYSTEM_20050818) || defined(VTM_PLAYTIME_LIMIT_SYSTEM_20060626)
	pClose.iAddPlayTime = lpObj->m_iPlayTimeLimit;
#endif

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}
#endif

//----------------------------------------------------------------------------
// [0xF1][0x01] 계정 인증 요청 결과를 클라이언트에게 보낸다.
//----------------------------------------------------------------------------
void JGPAccountRequest(LPSDHP_IDPASSRESULT lpMsg)
{
	char				szId[MAX_IDSTRING+1];
	int					aIndex;
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	char				szJN[MAX_JOOMINNUMBER_LENGTH+1]="";
#else
	char				szJN[14]="";
#endif

	memset(szId, 0,         sizeof(szId));
	memcpy(szId, lpMsg->Id, MAX_IDSTRING);
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	memcpy(szJN, lpMsg->JoominNumber, 13);//MAX_JOOMINNUMBER_LENGTH);
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
	memcpy(szJN, lpMsg->JoominNumber, 13);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

	aIndex = (int)lpMsg->Number;

#ifndef UPDATE_USERPASSWORD_LENGTH_20090424	
	// lpMsg->result == 0x0F 이면 계정캐릭터 블럭이다
	if( lpMsg->result == 0x01 || lpMsg->result == 0x0F )
	{
		// non pk 서버면 다 들어오고 아닐경우엔 15세 미만 접속 불가능
		if( gNonPK == 0 && gLanguage == 0 )
		{
			if( gJoomin15Check(szJN) == FALSE )
			{
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber,  lpMsg->UserNumber);
				lpMsg->result = 0x11;	// 실패로 만들고..	사용자한테 실패라고 알린다.
			}
		}
		
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324	// 18세 미만 접속 불가능
		if( gPkLimitFree == 1 && gLanguage == 0 )
		{
			if (gJoominCheck(szJN, 18) == FALSE) {
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber,  lpMsg->UserNumber);
				lpMsg->result = 0x11;	// 실패로 만들고..	사용자한테 실패라고 알린다.
			}
		}
#endif
	}
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424	

	switch( lpMsg->result )
	{
	case 0x01 :	// 인증 성공		
	case 0x0F :	// 인증 성공 - 계정 아이템 블럭 (MeMuOnline.MEMB_INFO.bloc_code 'F')
#ifdef PCBANG_DEFAULTER	
	case 0x10 :	// 인증 성공 - PC방 미납자 (MeMuOnline.MEMB_INFO.bloc_code 'G')
#endif
#ifdef ADD_PROTOCOL_PCBANG_GUID
	#ifdef PCBANG_ADVANTAGE 
			if( gObjSetAccountLogin(aIndex, szId,	lpMsg->UserNumber, 
													lpMsg->DBNumber, 
													lpMsg->PcBangGuid,
													szJN, lpMsg->bUserInPCBang) == FALSE )
	#else
			if( gObjSetAccountLogin(aIndex, szId,	lpMsg->UserNumber, 
													lpMsg->DBNumber, 
													lpMsg->PcBangGuid,
													szJN) == FALSE )
	#endif //PCBANG_ADVANTAGE
#else
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
		if( gObjSetAccountLogin(aIndex, szId,	lpMsg->UserNumber, 
												lpMsg->DBNumber, 
												0,
												szJN, lpMsg->bUserInPCBang) == FALSE )
	#else
		if( gObjSetAccountLogin(aIndex, szId,	lpMsg->UserNumber, 
												lpMsg->DBNumber, 
												szJN) == FALSE )
	#endif
#endif
		{	// 인증 실패일경우.. (인증에도 알리고..)
			GJPAccountFail(aIndex, szId, lpMsg->DBNumber,  lpMsg->UserNumber);
			lpMsg->result = 0x04;	// 실패로 만들고..	사용자한테 실패라고 알린다.
		}
		else
		{
	#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
			gObj[aIndex].m_bCheckGGAuth = true;
	#endif // MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
			LogAddTD(lMsg.Get(467), gObj[aIndex].Ip_addr, szId, lpMsg->UserNumber, lpMsg->DBNumber);
		}		
		if( lpMsg->UserNumber == 0 && lpMsg->DBNumber == 0 )
		{
			LogAddTD(lMsg.Get(468), szId, lpMsg->UserNumber, lpMsg->DBNumber);
		}
		break;
	}

#ifdef FOR_JAPAN
	if( lpMsg->result == 0 )
		lpMsg->result = 2;	//  일본의 경우 비밀번호가 틀리면 무조건 계정없음으로 한다.
#endif

#ifdef FOR_THAILAND
	if( lpMsg->result == 0x12 )
	{
		lpMsg->result = 0x0E;
		GCServerMsgStringSend(lMsg.Get(1609), aIndex, 1);		
	}
	gObj[aIndex].m_AuthAdult = lpMsg->AuthAdult;
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_PLAYTIME_LIMIT_MESSAGE_ONOFF_20060427
		if( g_bChnPlayTimeLimitOn == TRUE )
	#endif
		{
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
			gObj[aIndex].m_iVerifyType = lpMsg->iVerifiedType; 
			gObj[aIndex].m_iPlayTimeLimit = lpMsg->iAddPlayTime;
			gObj[aIndex].m_dwPlayTick = GetTickCount();
			gObj[aIndex].m_iPlayNoticeTick = 0;

			// 중국추가> . 로그인한 사용자에게 남은 시간에 대한 공지를 한다.
			if( gObj[aIndex].m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
			{
				INT iCalcRestTime = (5*60*60)-lpMsg->iAddRestTime;
				if (iCalcRestTime < 0)	iCalcRestTime = 0;
				PMSG_NOTICE	pNotice;
				CHAR szNotice[256] = {0,};
				TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);
				wsprintf(szNotice, lMsg.Get(1259),					// "플레이시간:%02d시%02d분, 정상 플레이까지 남은 휴식시간:%02d시%02d분"
					gObj[aIndex].m_iPlayTimeLimit/(60*60),	
					(gObj[aIndex].m_iPlayTimeLimit%(60*60))/60,
					iCalcRestTime/(60*60),
					(iCalcRestTime%(60*60))/60
					);
				TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice);
				TNotice::SendNoticeToUser( aIndex, &pNotice );
			}
	#else
			gObj[aIndex].m_iPlayTimeLimit = lpMsg->iAddPlayTime;
			gObj[aIndex].m_dwPlayTick = GetTickCount();
			gObj[aIndex].m_iPlayNoticeTick = 0;

			// 중국추가> . 로그인한 사용자에게 남은 시간에 대한 공지를 한다.
			INT iCalcRestTime = (5*60*60)-lpMsg->iAddRestTime;
			if (iCalcRestTime < 0)	iCalcRestTime = 0;
			PMSG_NOTICE	pNotice;
			CHAR szNotice[256] = {0,};
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);
			wsprintf(szNotice, lMsg.Get(1259),					// "플레이시간:%02d시%02d분, 정상 플레이까지 남은 휴식시간:%02d시%02d분"
				gObj[aIndex].m_iPlayTimeLimit/(60*60),	
				(gObj[aIndex].m_iPlayTimeLimit%(60*60))/60,
				iCalcRestTime/(60*60),
				(iCalcRestTime%(60*60))/60
				);
			TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice);
			TNotice::SendNoticeToUser( aIndex, &pNotice );
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
		}
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
		if( g_bVtmPlayTimeLimitOn == TRUE && lpMsg->result == 0x01 )
		{
			gObj[aIndex].m_iPlayTimeLimit	= lpMsg->iAddPlayTime;
			gObj[aIndex].m_iPlayRestTime	= lpMsg->iAddRestTime;
			gObj[aIndex].m_dwPlayTick		= GetTickCount();
			gObj[aIndex].m_iPlayNoticeTick	= 0;

			INT iCalcRestTime = (g_iVtmPlayTimeLimitLevel2Time*60)-lpMsg->iAddRestTime;

			if (iCalcRestTime < 0)	
			{
				iCalcRestTime = 0;
			}

			PMSG_NOTICE	pNotice;
			CHAR szNotice[256] = {0,};
			TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);

			// "현재 플레이 시간은 %d시 %d분이며, 초기화까지 %d시 %d분의 휴식이 필요합니다."
			TNotice::MakeNoticeMsgEx( &pNotice, TNOTICE_TYPE_BOTTOM, lMsg.Get(1270), 
				gObj[aIndex].m_iPlayTimeLimit/(60*60),	
				(gObj[aIndex].m_iPlayTimeLimit%(60*60))/60,
				iCalcRestTime/(60*60), 
				(iCalcRestTime%(60*60))/60 );
			TNotice::SendNoticeToUser( aIndex, &pNotice );

		}
#endif

#ifdef PCBANG_DEFAULTER	
	if( lpMsg->result == 0x10 )
	{
		// 결과코드를 기존과 다른 코드로 설정하여 전달한다.
	#ifdef MODIFY_PCBANG_DEFAULTER_MESSAGE_20061011
		lpMsg->result = 0x20;
	#else
		lpMsg->result = 0x01;
	#endif // MODIFY_PCBANG_DEFAULTER_MESSAGE_20061011
		
	#ifdef MODIFY_NOTICE_20040325
		PMSG_NOTICE	pNotice;
		TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_RED);
		
		TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, "PC방 사용 요금 미지불에 대한 신고가 접수되었습니다" );
		TNotice::SendNoticeToUser( aIndex, &pNotice );

		TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, "자세한 사항은 PC방 홈페이지를 참고하시기 바랍니다" );
		TNotice::SendNoticeToUser( aIndex, &pNotice );
	#else
		GCServerMsgStringSend("PC방 사용 요금 미지불에 대한 신고가 접수되었습니다", aIndex, 0);
		GCServerMsgStringSend("자세한 사항은 PC방 홈페이지를 참고하시기 바랍니다", aIndex, 0);		
	#endif
		
	}
#endif

	if( lpMsg->result == 0x0F )
	{
		lpMsg->result = 0x01;
		gObj[aIndex].m_cAccountItemBlock = TRUE;
	}
		

	gObj[aIndex].LoginMsgSnd = 0;
	if( gObj[aIndex].LoginMsgCount >= 3 )
	{
		lpMsg->result = 0x08;
	}
	
#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
	if( lpMsg->result == 0x06 )
	{
		// 접속오류 입니다.
		gObj[aIndex].Connected = CSS_LOGINWAIT;
	}
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801

#ifdef SMS_BLOCK
	if( lpMsg->result == 0x11 )
	{	// SMS 블럭
		lpMsg->result = 0x0E;
		GCServerMsgStringSend(lMsg.Get(1607), aIndex, 1);
	}
#endif

#ifdef MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801
	if( lpMsg->result != 0x06 )
	{
		GCJoinResult(lpMsg->result, aIndex);
	}
#else
	GCJoinResult(lpMsg->result, aIndex);
#endif // MODIFY_USER_CLOSE_AT_AUTH_WAITTIME_20070801

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	
	if( lpMsg->result == 0x01 )
	{
		USER_CONNECT_TIME pUserConnectTime;
		
		memset(&pUserConnectTime, 0, sizeof(USER_CONNECT_TIME));
		
		pUserConnectTime.dwConnectTime = GetTickCount();
		pUserConnectTime.iIndex = aIndex;
		memcpy(pUserConnectTime.szAccount, lpMsg->Id, MAX_IDSTRING);
		memcpy(pUserConnectTime.Ip_addr, gObj[aIndex].Ip_addr, 15);
		
		SetConnectUser(&pUserConnectTime);
	}
	
	// 3번 이상 실패하면..
	if( lpMsg->result != 0x01 )
	{
		if( gObj[aIndex].LoginMsgCount > 3 )
		{	
			IP_BLOCK pIpBlock;
			
			pIpBlock.dwBlockTime = GetTickCount();
			memcpy(pIpBlock.szAccount, lpMsg->Id, MAX_IDSTRING);
			memcpy(pIpBlock.Ip_addr, gObj[aIndex].Ip_addr, 15);
			
			setBlockIP(&pIpBlock);
			
			LogAddTD_TempLog(", [Login Fail], Account [%s], IP [%s], result[%d]",lpMsg->Id, gObj[aIndex].Ip_addr, lpMsg->result);
			
			CloseClient ( aIndex );
		}
		
		//LogAddTD_TempLog(", [TempLog - Login] Account [%s], IP [%s], Fail",lpMsg->Id, gObj[aIndex].Ip_addr);
	}
#else // MODIFY_CONNNECT_HACK_ATTACK_20090409
	
	// 3번 이상 실패하면..
	if( lpMsg->result != 0x01 )
	{
		if( gObj[aIndex].LoginMsgCount > 3 )
		{
			//wsGServer.Close(gObj[aIndex].m_socket);
			CloseClient ( aIndex );
		}
	}

#endif // MODIFY_CONNNECT_HACK_ATTACK_20090409
	
	if(gObj[aIndex].Connected == 1)
	{
		int itemp = (lpMsg->result == 0x01 ?  0 : 1 );
		char *temp[2] = { "Sucess", "Fail" };
		
#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		LogAddTD_TempLog(", [Login], Account [%s], IP [%s], result[%d], %s",gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, lpMsg->result, temp[itemp]);
#endif
	}
}

// [0x02] 인증서버에 사용자 인증을 실패했다고 알린다.
void GJPAccountFail(short number, char * Id, int DBNumber, int UserNumber )
{
	SDHP_JOINFAIL		pMsg;

	memset( &pMsg, 0, sizeof( pMsg ));

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x02;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.Number     = number;
	pMsg.DBNumber	= DBNumber;
	pMsg.UserNumber = UserNumber;
	
	strcpy( pMsg.Id, Id );

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// [0x04] 인증서버에 사용자를 블럭 시키라고 한다.
void GJPAccountBlock(short number, char * Id, int DBNumber, int UserNumber, BYTE BlockCode )
{
	SDHP_COMMAND_BLOCK	pMsg;

	memset( &pMsg, 0, sizeof( pMsg ));

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x04;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.Number     = number;
	pMsg.DBNumber	= DBNumber;
	pMsg.UserNumber = UserNumber;
	pMsg.BlockCode  = BlockCode;
	
	strcpy( pMsg.Id, Id );

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// 0x06
void GJPBillCeckRecv(LPSDHP_SDHP_BILLSEARCH_RESULT lpMsg)
{
	int		aIndex = lpMsg->Number;
	char	szId[MAX_IDSTRING+1];
	char    EndsDays[13];


	szId[MAX_IDSTRING] = '\0';

	memcpy(szId, lpMsg->Id, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		return;
	}
	memset(EndsDays, 0, 13);
	memcpy(EndsDays, lpMsg->EndsDays, 12);
	
#ifdef MODIFY_PCBANG_EXPIRE_USER_ADVANTAGE_REMOVE_20070120
	if( m_ObjBill[aIndex].GetCertify() == CERTIFYTYPE_IP && lpMsg->cCertifyType == CERTIFYTYPE_ACCOUNT )
	{
		// PC방 만료 유저는 PC방 해택 부분 적용을 삭제한다.
		gObj[aIndex].m_bPCBangUser = FALSE;
		gObj[aIndex].m_bPCBangCouponUser = FALSE;
		gObj[aIndex].m_bPCBangFreeChaosCastleUser = FALSE;
	}
#endif // MODIFY_PCBANG_EXPIRE_USER_ADVANTAGE_REMOVE_20070120

#ifdef 	UPDATE_UNIFICATIONBILL_20090203
	if( m_ObjBill[aIndex].GetDeductType() != BILL_TYPE_NONE )
	{
		if(m_ObjBill[aIndex].GetDeductType() != lpMsg->btDeductType )
		{
			LogAddTD("[WzUnificationBill][DeducateType] - Change %d -> %d (ID:%s)",
				m_ObjBill[aIndex].GetDeductType(),
				lpMsg->btDeductType, szId );
			
			gLCount[m_ObjBill[aIndex].GetDeductType()].Delete();
			gLCount[lpMsg->btDeductType].Add();
			
			gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 100);
		}			
	}
	
	if( m_ObjBill[aIndex].SetBill(lpMsg->btDeductType, lpMsg->dwRestPoint, lpMsg->dwRestTime, EndsDays) == TRUE )
	{
#if TESTSERVER == 1
		gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 500);
#else
		gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 100);
#endif
	}
#else	// UPDATE_UNIFICATIONBILL_20090203
#ifdef PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103
	if( m_ObjBill[aIndex].GetCertify() != -1 )
	{
		if( m_ObjBill[aIndex].GetCertify() != lpMsg->cCertifyType )
		{
			LogAddTD("[UnificationBill][CertifyType] - Change %d -> %d (ID:%s)",
				m_ObjBill[aIndex].GetCertify(),
				lpMsg->cCertifyType,
				szId );
			gLCount[m_ObjBill[aIndex].GetCertify()].Delete();
			gLCount[lpMsg->cCertifyType].Add();
			
	#ifdef MODIFY_BILLING_MESSAGE_BUGFIX_20070208
			gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 100);
	#endif // MODIFY_BILLING_MESSAGE_BUGFIX_20070208
		}
	}
#endif // PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103

	if( m_ObjBill[aIndex].SetBill(lpMsg->cCertifyType, lpMsg->PayCode, lpMsg->EndTime, EndsDays) == TRUE )
	{

#ifndef MODIFY_BILLING_RESTTIME_BUGFIX_20070212			// !!NOT
	#ifdef PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103
		if( lpMsg->cCertifyType == CERTIFYTYPE_IP )
		{
			return;
		}
	#endif // PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103
#endif // MODIFY_BILLING_RESTTIME_BUGFIX_20070212

#if TESTSERVER == 1
		gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 500);
#else
		gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 100);
#endif
	}
#endif	// UPDATE_UNIFICATIONBILL_20090203

#ifndef MODIFY_BILLING_MESSAGE_BUGFIX_20070208
	#ifdef PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103
	else
	{
		gObjAddMsgSendDelay(&gObj[aIndex], MSG_BILLSEND, aIndex, 100);
	}
	#endif // PCBANG_UNIFICATION_BILLING_CONNECT_USER_COUNT_20070103
#endif // MODIFY_BILLING_MESSAGE_BUGFIX_20070208
}

// 0x07
void GJPUserKillRecv(LPSDHP_BILLKILLUSER lpMsg)
{
	char szId[MAX_IDSTRING+1];
	int number;
	szId[MAX_IDSTRING] = '\0';

	memcpy(szId, lpMsg->Id, MAX_IDSTRING);
	number = lpMsg->Number;

#ifdef MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	gObj[number].m_UserKillRecvCount++;
	
	// 사용자를 강제 종료 시키도록 한다.
	LogAddTD( "[TRACELOG][HACKING] User kill from JoinServer (%d) [%s][%s]", number, szId, gObj[number].Name );
	
	if( gObj[number].m_UserKillRecvCount > 10 )
	{
		// m_UserKillRecvCount 이 5회 이상이면 강제로 접속을 종료시킨다.
		gObjDel( number );
	}
#endif // MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
	
	if( gObjIsAccontConnect(number, szId) == FALSE )
	{
#ifdef MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
		// 접속 종료 프로토콜을 처리 못하는 부분이 있는지 확인한다.
		LogAddTD( "[HACKING][USERKILL DENY] Can't find User (%d) [%s][%s]", number, szId, gObj[number].Name );
#endif // MODIFY_DENY_USER_DISCONNNECT_HACK_20080323
		return;
	}

	GCServerMsgStringSend(lMsg.Get(1602), number, 0);
	gObjUserKill(number);
}

// 0x08 
void JGOtherJoin(LPSDHP_OTHERJOINMSG lpMsg)
{
	char szId[MAX_IDSTRING+1];

	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected >= CSS_LOGINOK )
		{
			if( gObj[n].AccountID[0] == szId[0] )
			{
				if( gObj[n].AccountID[1] == szId[1] )
				{
					if( strcmp( gObj[n].AccountID,szId) == 0 )
					{
						GCServerMsgStringSend(lMsg.Get(1131), n, 1);
						// 다른사용자가 접속을 시도하고 있습니다
						return;
					}
				}
			}		
		}
	}
}

// 0x09
void GJPUserDisconnectRecv(LPSDHP_BILLKILLUSER lpMsg)
{
	char szId[MAX_IDSTRING+1];
	int number;
	szId[MAX_IDSTRING] = '\0';

	memcpy(szId, lpMsg->Id, MAX_IDSTRING);
	number = lpMsg->Number;

	if( gObjIsAccontConnect(number, szId) == FALSE )
	{
		return;
	}

	GCServerMsgStringSend(lMsg.Get(1603), number, 0);
	gObjUserKill(number);
}


// 0x20 인증서버에서 서버의 종료를 보냈다..
void JGPExitCodeRecv(LPSDHP_EXITMSG lpMsg)
{
	if( lpMsg->ExitCode[0] == 'F' &&
		lpMsg->ExitCode[1] == 'F' &&
		lpMsg->ExitCode[2] == 'X' )
	{
		SendMessage(ghWnd, WM_COMMAND, ID_MENU_THREECLOSE, NULL);
	}
}

// [0x60] 이벤트 시작을 알린다.
void JGPEventStart(LPSDHP_EVENTSTART lpMsg)
{
	if ( lpMsg->Event == 1 )
	{
		if( DragonEvent->GetState() )
		{
			DragonEvent->End();
		}
		else DragonEvent->Start();
	}
}

// [0x61] 인증서버에서 이벤트 시작 또는 종료를 알린다
void JGPEventControl(LPSDHP_EVENTCONTROL lpMsg)
{
	switch(lpMsg->Event)
	{
		case 0:
			{	// 드래곤 이벤트 
				if( lpMsg->Stat )
				{
					if( !DragonEvent->GetState() )
					{
						DragonEvent->Start();
					}					
				}
				else
				{
					DragonEvent->End();
				}
			}
		break;

		case 1:
			{	// 성탄의 별 이벤트
				gXMasEvent = lpMsg->Stat;
			}
		break;

		case 2:
			{	// 폭죽(마법주머니) 이벤트
				gFireCrackerEvent = lpMsg->Stat;
			}
		break;

		case 3:
			{	// 사랑의 하트 이벤트
				gHeartOfLoveEvent = lpMsg->Stat;
			}
		break;

		case 4:
			{	// 메달 이벤트
				gMedalEvent = lpMsg->Stat;
			}
		break;
	}
}

// [0x62] 인증서버에서 Reload 설정을 알린다
void JGPOptionControl(LPSDHP_OPTIONCONTROL lpMsg)
{
	switch( lpMsg->Option )
	{
		case 0:
			{	// Option reLoad
				ReadCommonServerInfo();
			}
		break;

		case 1:
			{	// Monster reLoad
				GameMonsterAllCloseAndReLoad();
			}
		break;

		case 2:
			{	// Shop reLoad
				ShopDataLoad();
			}
		break;

		case 3:
			{	// All reLoad
				ReadCommonServerInfo();
				GameMonsterAllCloseAndReLoad();
				ShopDataLoad();
			}
		break;

#ifdef ADD_OPTION_RELOAD_ITEM_TO_JOINSERVER
		case 4:
			{	// All Event Reload
				ReadEventInfo(MU_EVENT_ALL);
			}
		break;

		case 5:
			{	// All ETC Reload
				ReadGameEtcInfo(MU_ETC_ALL);				
			}
		break;
#endif
	}
}


// [0x30] 인증 서버에 러브하트 아이템을 저장시킨다.
void LoveHeartEventSend(int aIndex, char * AcountId, char* Name )
{
	SDHP_LOVEHEARTEVENT	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x30;
	pMsg.h.size		= sizeof( pMsg );
	memcpy(pMsg.Account, AcountId, MAX_IDSTRING);
	memcpy(pMsg.Name,	Name, MAX_IDSTRING);
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void LoveHeartEventRecv(LPSDHP_LOVEHEARTEVENT_RESULT lpMsg )
{
	char szName[MAX_IDSTRING+1];
	int  number;
	char Msg[256];

	szName[MAX_IDSTRING] = '\0';

	memcpy(szName, lpMsg->Name, MAX_IDSTRING);
	number = lpMsg->Number;

	//LogAdd("러브 팡팡 이벤트 결과 받음 %s %d %x", szName, number, lpMsg->Result);
	// 이벤트 결과 타입
	switch( lpMsg->Result	)
	{
	case 0 :
		wsprintf(Msg, lMsg.Get(1104), number);
		{
		for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
		{
			if( gObj[n].Connected >= CSS_LOGINOK )
			{				
				if( gObj[n].Name[0] == szName[0] )
				{
					if( gObj[n].Name[1] == szName[1] )
					{
						if( strcmp( gObj[n].Name,szName) == 0 )
						{
							GCServerMsgStringSend(Msg, n, 1);
							wsprintf(Msg, lMsg.Get(1105));
							GCServerMsgStringSend(Msg, n, 1);
							return;
						}
					}
				}			
			}
		}
		}
		break;
	case 1 :
		wsprintf(Msg, lMsg.Get(1106), szName, number);
		AllSendServerMsg(Msg);
		break;
	}
}

void LoveHeartCreateSend()
{		
	SDHP_LOVEHEARTCREATE	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x31;
	pMsg.h.size		= sizeof( pMsg );
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// 0x40
void AllNoticeRecv(LPSDHP_NOTICE lpMsg)
{
	char szNotice[61];
	szNotice[60] = '\0';
	
	memcpy(szNotice, lpMsg->Notice, 60);
	
#ifdef ADD_LOG_GM_TOOL_NOTICE_20051004
	LogAddTD("[Mu Manager][Notice Log][Type:ALL] %s", szNotice );
#endif

	if( strlen( szNotice) > 1 )
	{
#ifdef FOR_JAPAN	// 일본은 뮤매니저 전체공지를 기존 방식대로 중앙에 보여준다.
		AllSendServerMsg(szNotice);
		return;
#endif

// 20041116 기존 방식대로 중앙에 보여준다.
//#ifdef MODIFY_NOTICE_20040325
//		PMSG_NOTICE	pNotice;
//		TNotice::MakeNoticeMsg( &pNotice, 0, szNotice );
//		TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
//		TNotice::SendNoticeToAllUser( &pNotice );
//#else
		AllSendServerMsg(szNotice);
//#endif
	}
}

// 0x41
void UserNoticeRecv(LPSDHP_USER_NOTICE lpMsg)
{
	char szNotice[61];
	char szId[MAX_IDSTRING+1];

	szNotice[60] = '\0';
	szId[MAX_IDSTRING] = '\0';
	
	memcpy(szNotice, lpMsg->Notice, 60);
	memcpy(szId, lpMsg->szId, MAX_IDSTRING);

#ifdef ADD_LOG_GM_TOOL_NOTICE_20051004
	LogAddTD("[Mu Manager][Notice Log][Type:USER] (%s) %s", szId, szNotice );
#endif

	if( strlen( szNotice) > 1 )
	{
		if( strlen( szId ) > 1 )
		{
			for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
			{
				if( gObj[n].Connected >= CSS_LOGINOK )
				{					
					if( gObj[n].AccountID[0] == szId[0] )
					{
						if( gObj[n].AccountID[1] == szId[1] )
						{
							if( strcmp( gObj[n].AccountID, szId) == 0 )
							{

						#ifdef MODIFY_NOTICE_20040325
								PMSG_NOTICE	pNotice;
								TNotice::MakeNoticeMsg( &pNotice, 0, szNotice );
								TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_RED);
								TNotice::SendNoticeToUser( n, &pNotice );
						#else
								GCServerMsgStringSend(szNotice, n, 0);
						#endif
								
								return;
							}
						}
					}				
				}
			}
		}
	}
}


/*
// 0x31 ZEN 아이템을 생성한다.
void LoveHeartCreateSend(BYTE MapNumber, BYTE x, BYTE y)
{	
	if( !gEvent1 ) return;

	SDHP_LOVEHEARTCREATE	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x31;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.MapNumber	= MapNumber;
	pMsg.x			= x;
	pMsg.y			= y;

	wsDataCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void LoveHeartCreateRecv(LPSDHP_LOVEHEARTCREATERECV lpMsg)
{
	if( !gEvent1 ) return;

	int mapnumber = lpMsg->MapNumber;
	int lItem = ItemGetNumberMake(14,12);

	int x   = lpMsg->x;
	int y   = lpMsg->y;

	if( mapnumber > 5 || mapnumber < 0 )
	{
		mapnumber = 0;
	}
	MapC[mapnumber].MonsterItemDrop(lItem, 0, 0, x+1, y+1, 0, 0, 0);
}
*/


#ifdef MAP_SERVER_WORK_20041030		// 서버 맵분할 관련 패킷 - 게임서버 <-> 인증서버

#include "MapServerManager.h"
extern CMapServerManager		g_MapServerManager;

#include "DBSockMng.h"
extern DBSockMng				cDBSMng;

#include "SProtocol.h"


extern void GCAnsMapSvrAuth		(INT iIndex, INT iResult);


//----------------------------------------------------------------------------
// GJ [0x7A] 인증서버에게 특정 사용자가 맵서버를 이동해야 함을 알림 (0xC1)
//----------------------------------------------------------------------------
void GJReqMapSvrMove		(INT iIndex, WORD wDestMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY)
{
	if (!gObjIsConnected(iIndex)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error GJ [0x7A] - User not Connected : %d",
			iIndex
			);
		return;
	}

	PMSG_REQ_MAPSVRMOVE		pMsg;
	PHeadSetB				((LPBYTE)&pMsg, 0x7A, sizeof(pMsg));

	pMsg.iIndex				= iIndex;
	memcpy(pMsg.szAccountID, gObj[iIndex].AccountID, MAX_IDSTRING);
	pMsg.szAccountID[MAX_IDSTRING]	= 0;
	memcpy(pMsg.szCharName,	gObj[iIndex].Name, MAX_IDSTRING);
	pMsg.szCharName[MAX_IDSTRING]	= 0;
	pMsg.wCurMapSvrCode		= gGameServerCode;
	pMsg.wDstMapSvrCode		= wDestMapSvrCode;
	pMsg.wMapNumber			= wMapNumber;
	pMsg.btX				= btX;
	pMsg.btY				= btY;

#ifdef MAP_SERVER_ADDINFO_20050509
#ifdef PCBANG_ADVANTAGE
	pMsg.bPcBangUser		= gObj[iIndex].m_bPCBangUser;					// PC방 사용자인지 여부
#endif
#endif	
	
#ifdef MAP_SERVER_ADDINFO_ADULT_20050610
	pMsg.bIsAdult			= gObj[iIndex].m_AuthAdult;						// 성인인지 여부
#endif
	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	pMsg.iAddPlayTime		= gObj[iIndex].m_iPlayTimeLimit;
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	pMsg.iAddPlayTime		= gObj[iIndex].m_iPlayTimeLimit;
#endif

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


//----------------------------------------------------------------------------
// JG [0x7A] 인증서버가 특정 사용자의 맵서버 이동에 대한 결과를 알림 (0xC1)
//----------------------------------------------------------------------------
void JGAnsMapSvrMove		(LPPMSG_ANS_MAPSVRMOVE lpMsg)
{
	if (!gObjIsConnected(lpMsg->iIndex)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7A] - User not Connected : %d",
			lpMsg->iIndex
			);
		return;
	}

	LPOBJECTSTRUCT lpObj	= (LPOBJECTSTRUCT) &gObj[lpMsg->iIndex];
	if ( strcmp(lpObj->AccountID, lpMsg->szAccountID) || strcmp(lpObj->Name, lpMsg->szCharName)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7A] - User Info Mismatch [%s][%s]-[%s][%s] (%d)",
			lpMsg->szAccountID,
			lpMsg->szCharName,
			lpObj->AccountID, 
			lpObj->Name,
			lpObj->m_Index
			);
		return;
	}

	if (lpMsg->iResult	== 0) {
		// 사용자에게 특정 맵서버로 이동해야 함을 알림
		PMSG_ANS_MAPSERVERCHANGE	pMsg;
		PHeadSubSetBE				((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));
		
		pMsg.wMapSvrCode			= g_MapServerManager.GetMapSvrGroup();
		pMsg.iJoinAuthCode1			= lpMsg->iJoinAuthCode1;
		pMsg.iJoinAuthCode2			= lpMsg->iJoinAuthCode2;
		pMsg.iJoinAuthCode3			= lpMsg->iJoinAuthCode3;
		pMsg.iJoinAuthCode4			= lpMsg->iJoinAuthCode4;

		BOOL bRESULT				= g_MapServerManager.GetSvrCodeData(lpMsg->wDstMapSvrCode, pMsg.szMapSvrIpAddress, &pMsg.wMapSvrPort);
		if (bRESULT == FALSE) {
			LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7A] - Failed to find Map Info [%s]-[%s] (%d)",
				lpObj->AccountID,
				lpObj->Name,
				lpMsg->wDstMapSvrCode
				);
			return;
		}

		// 맵이동 전에 인벤토리 세이브를 한다.
#ifdef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
		GJSetCharacterInfo(lpObj, lpObj->m_Index, TRUE);
#else
		GJSetCharacterInfo(lpObj, lpObj->m_Index);
#endif
	
		// 맵서버 이동을 위해 나간다는 표시를 한다.
		lpObj->m_bMapSvrMoveQuit	= true;
		lpObj->m_dwMapSvrQuitTick	= GetTickCount();

		DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		LogAddTD("[MapServerMng] [%s][%s] JG [0x7A] - Move Map Request OK : MAPSVR_D:%d",
			lpObj->AccountID,
			lpObj->Name,
			lpMsg->wDstMapSvrCode
			);
	}
	else {
		// 사용자에게 실패했다는 메시지를 보낸다.
#ifdef MODIFY_MOVECOMMAND_BUGFIX_20080806
		PMSG_ANS_MAPSERVERCHANGE	pMsg = {0};
		PHeadSubSetBE				((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));
		DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->m_MoveGateNumber = -1;
		lpObj->m_State = OBJST_PLAYING;
#endif // MODIFY_MOVECOMMAND_BUGFIX_20080806
		
		PMSG_NOTICE	pNotice;
#ifdef MODIFY_NOTICE_20040325
		TNotice::MakeNoticeMsgEx( &pNotice, 0, lMsg.Get(1613));				// "현재 해당 맵으로의 이동이 불가능합니다."
#else
		pNotice.type = 0;
		wsprintf(pNotice.Notice, lMsg.Get(1613));
		PHeadSetB((LPBYTE)&pNotice, 0x0D, 4+(strlen(pNotice.Notice)+1));	// "현재 해당 맵으로의 이동이 불가능합니다."
#endif

		DataSend(lpObj->m_Index, (LPBYTE)&pNotice, pNotice.h.size);
	
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7A] - Failed to Authorize User [%s][%s] Result:%d",
			lpObj->AccountID,
			lpObj->Name,
			lpMsg->iResult
			);
	}
}

//----------------------------------------------------------------------------
// GJ [0x7B] 인증서버에게 특정 사용자가 맵서버 이동 후 인증을 요청함을 알림 (0xC1)		-> 인증서버는 해당 사용자의 게임서버 코드도 갱신
//----------------------------------------------------------------------------
void GJReqMapSvrAuth		(INT iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, INT iJA1, INT iJA2, INT iJA3, INT iJA4)
{
	if (!CHECK_LIMIT(iIndex, MAX_OBJECT)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error GJ [0x7B] - Index out of bound : %d",
			iIndex
			);
		return;
	}

#ifdef MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418
	LogAddTD("[BUGTRACE][GJReqMapSvrAuth] (%d) Id : %s, Name : %s, ServerCode : %d",
		iIndex, lpszAccountID, lpszCharName, gGameServerCode );
#endif // MODIFY_CASTLE_MAP_MOVE_TRACE_LOG_20080418	

	PMSG_REQ_MAPSVRAUTH			pMsg;
	PHeadSetB					((LPBYTE)&pMsg, 0x7B, sizeof(pMsg));

	pMsg.iIndex					= iIndex;
	memcpy(pMsg.szAccountID, lpszAccountID, MAX_IDSTRING);
	memcpy(pMsg.szCharName,	lpszCharName, MAX_IDSTRING);
	pMsg.wDstMapSvrCode			= gGameServerCode;
	pMsg.iJoinAuthCode1			= iJA1;
	pMsg.iJoinAuthCode2			= iJA2;
	pMsg.iJoinAuthCode3			= iJA3;
	pMsg.iJoinAuthCode4			= iJA4;
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	
	// 이 사용자는 맵서버 이동을 신청했었음을 알림
	gObj[iIndex].m_bMapSvrMoveReq	= true;
}


//----------------------------------------------------------------------------
// JG [0x7B] 인증서버에게 특정 사용자가 맵서버 이동 후 인증요청에 대한 결과 (0xC1)
//----------------------------------------------------------------------------
void JGAnsMapSvrAuth		(LPPMSG_ANS_MAPSVRAUTH lpMsg)
{
	if (!CHECK_LIMIT(lpMsg->iIndex, MAX_OBJECT)) {
		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",
			lpMsg->iIndex
			);
		return;
	}

	LPOBJECTSTRUCT lpObj	= (LPOBJECTSTRUCT) &gObj[lpMsg->iIndex];
//	if ( strcmp(lpObj->AccountID, lpMsg->szAccountID)) {		//  -> 클라이언트 
//		LogAddC(LOGC_RED, "[MapServerMng] Packet Error JG [0x7B] - User Info Mismatch [%s]-[%s] (%d)",
//			lpMsg->szAccountID,
//			lpObj->AccountID,
//			lpObj->m_Index
//			);
//		return;
//	}
	memcpy (lpObj->Name, lpMsg->szCharName, MAX_IDSTRING);

	if (lpObj->m_bMapSvrMoveReq == false) {
		LogAddC(LOGC_RED, "[MapServerMng] MapSvr MoveReq Error JG [0x7B] - No Request [%s] (%d)",
			lpObj->AccountID,
			lpObj->m_Index
			);
		gObjDel (lpMsg->iIndex);
		return;
	}

	//------------------------------------------------------------------------
	// 약식 인증확인 - 이미 인증되었으므로 대부분의 이전 인증결과를 믿는다.
	BOOL				bAuthOK			= FALSE;
	INT					iSendResult		= 0;

	CHAR				szId[MAX_IDSTRING+1];
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	CHAR				szJN[MAX_JOOMINNUMBER_LENGTH+1]		="";
#else	// UPDATE_USERPASSWORD_LENGTH_20090424	
	CHAR				szJN[14]		="";
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424	
	INT					iIndex			= lpMsg->iIndex;

	memset(szId, 0, sizeof(szId));
	memcpy(szId, lpMsg->szAccountID, MAX_IDSTRING);
#ifdef UPDATE_USERPASSWORD_LENGTH_20090424	
	memcpy(szJN, lpMsg->cJoominNumber, 13);//MAX_JOOMINNUMBER_LENGTH);
#else	// UPDATE_USERPASSWORD_LENGTH_20090424
	memcpy(szJN, lpMsg->cJoominNumber, 13);
#endif	// UPDATE_USERPASSWORD_LENGTH_20090424

	switch(lpMsg->iResult) {
	case 0 :		// 성공
		{
			iSendResult					= 0;
			bAuthOK						= FALSE;
#ifdef ADD_PROTOCOL_PCBANG_GUID	
	#ifdef PCBANG_ADVANTAGE 
		#ifdef MAP_SERVER_ADDINFO_20050509
			if( gObjSetAccountLogin(iIndex, szId,	lpMsg->iUserNumber, 
													lpMsg->iDBNumber, 
													lpMsg->iPcBangGuid,
													szJN, lpMsg->bPcBangUser) == FALSE )
		#else
			if( gObjSetAccountLogin(iIndex, szId,	lpMsg->iUserNumber, 
													lpMsg->iDBNumber, 
													lpMsg->iPcBangGuid,
													szJN, FALSE) == FALSE )
		#endif	// MAP_SERVER_ADDINFO_20050509
	#else
			if( gObjSetAccountLogin(iIndex, szId,	lpMsg->iUserNumber, 
													lpMsg->iDBNumber, 
													lpMsg->iPcBangGuid,
													szJN) == FALSE )
	#endif // PCBANG_ADVANTAGE

#else
	#if defined(PCBANG_ADVANTAGE) && defined(FOR_TAIWAN)
			if( gObjSetAccountLogin(iIndex, szId,	lpMsg->iUserNumber, 
													lpMsg->iDBNumber, 
													0,
													szJN, lpMsg->bPcBangUser) == FALSE )
	#else
			if( gObjSetAccountLogin(iIndex, szId,	lpMsg->iUserNumber, 
													lpMsg->iDBNumber, 
													szJN) == FALSE )
	#endif
#endif // ADD_PROTOCOL_PCBANG_GUID
			{	// 인증 실패일경우.. (인증에도 알리고..)
				GJPAccountFail(iIndex, szId, lpMsg->iDBNumber,  lpMsg->iUserNumber);
				lpMsg->iResult			= 0x04;		// 실패로 만들고..	사용자한테 실패라고 알린다.
			}
			else
			{
				LogAddTD(lMsg.Get(467), gObj[iIndex].Ip_addr, szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
				// 로그인 성공
				iSendResult				= 1;
				bAuthOK					= TRUE;
			}		
			if( lpMsg->iUserNumber == 0 && lpMsg->iDBNumber == 0 )
			{
				LogAddTD(lMsg.Get(468), szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
			}

			// 블록코드 처리
			switch(lpMsg->btBlockCode) {
			case 0 :	// 없음
				break;
			case 'F' :	// 케릭터 블록
				{
					gObj[iIndex].m_cAccountItemBlock = TRUE;
				}
				break;
			case 'G' :	// PC방 요금미납
				{
				}
				break;
			default :
				{
				}
				break;
			}
			break;
		}
		break;
	case 1 :		// 계정이 틀림
	case 2 :		// 인증코드 틀림
	case 3 :		// 서버코드 틀림
	case 4 :		// 서버 이동중 아님
	default :		// 잘못된 결과값
		{
			iSendResult					= 0;
			bAuthOK						= FALSE;
		}
		break;
	}

	// 클라이언트에 인증결과를 보냄 [0xB1][0x01]
	GCAnsMapSvrAuth	(iIndex, iSendResult);
	//------------------------------------------------------------------------

	if (bAuthOK == FALSE) {
		LogAddC(LOGC_RED, "[MapServerMng] [%s][%s] JG [0x7B] - Join Result Failed : RST:%d, BLK:%c",
			szId,
			lpMsg->szCharName,
			lpMsg->iResult,
			lpMsg->btBlockCode
			);
		gObjDel (lpMsg->iIndex);
		return;
	}

	// 인증서버에서 이전 맵서버 코드를 다시 받아온다.
	gObj[iIndex].m_sPrevMapSvrCode	= lpMsg->wPrevMapSvrCode;
	gObj[iIndex].m_sDestMapNumber	= lpMsg->wMapNumber;
	gObj[iIndex].m_btDestX			= lpMsg->btX;
	gObj[iIndex].m_btDestY			= lpMsg->btY;
	
#ifdef MAP_SERVER_ADDINFO_20050509	
#ifdef PCBANG_ADVANTAGE
	gObj[iIndex].m_bPCBangUser		= lpMsg->bPcBangUser;
#endif
#endif

#ifdef MAP_SERVER_ADDINFO_ADULT_20050610	
	gObj[iIndex].m_AuthAdult		= lpMsg->bIsAdult;
#endif

#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818
	#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	gObj[iIndex].m_iVerifyType		= lpMsg->iVerifiedType;
	#endif // MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
	gObj[iIndex].m_iPlayTimeLimit	= lpMsg->iAddPlayTime;
	gObj[iIndex].m_dwPlayTick		= GetTickCount();
#endif

#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626
	gObj[iIndex].m_iPlayTimeLimit	= lpMsg->iAddPlayTime;
	gObj[iIndex].m_dwPlayTick		= GetTickCount();
#endif
	
#ifdef BLOCK_INVALID_CHARACTER_DELETE_01_20060327
	gObj[iIndex].bEnableDelCharacter = FALSE;
#endif

#ifdef MODIFY_GAMEGUARD_NOT_AUTH_MAPSERVER_MOVE_20060513
	lpObj->m_bCheckGGAuth = false;
#ifdef MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
	lpObj->m_dwNPGGMapSvrMoveTime = GetTickCount();
#endif	// #ifdef MODIFY_GAMEGARD_AUTH_MAPSERVER_MOVE_20080723
#endif

	// 여기서 바로 데이터 서버에 케릭정보를 요청
	SDHP_DBCHARINFOREQUEST	pMsg;
	CHAR szName[MAX_IDSTRING+1];

	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof( pMsg ));
	
	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->szCharName, MAX_IDSTRING);
	BuxConvert(szName, MAX_IDSTRING);
	memcpy(pMsg.Name, szName, MAX_IDSTRING);
	strcpy(pMsg.AccountID, lpMsg->szAccountID);
	
	pMsg.Number = lpObj->m_Index;	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);

	LogAddTD("[MapServerMng] [%s][%s] JG [0x7B] - Join Result OK : RST:%d, BLK:%c",
		szId,
		lpMsg->szCharName,
		lpMsg->iResult,
		lpMsg->btBlockCode
		);

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050714
	LogAddTD(lMsg.Get(467), gObj[iIndex].Ip_addr, szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
#endif
}


//----------------------------------------------------------------------------
// JG [0x7C] 특정 게임서버의 현재 최대 사용자 수를 주기적으로 보냄 (0xC1) 
//----------------------------------------------------------------------------
void GJNotifyMaxUserCount	()
{
	PMSG_NOTIFY_MAXUSER		pMsg;
	PHeadSetB				((LPBYTE)&pMsg, 0x7C, sizeof(pMsg));

	pMsg.iSvrCode			= gGameServerCode;
	pMsg.iMaxUserCount		= gServerMaxUser;
	pMsg.iCurUserCount		= gObjTotalUser;

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

#endif


#ifdef ADD_MANAGER_SENDMAIL
void JGPSendMail(LPPMSG_JG_MEMO_SEND lpMsg)
{
	MngFriendMemoSend(lpMsg);
}
#endif

#ifdef MODIFY_UPDATE_MATCHDB_AT_CHARACTERLIST_VIEW_20060725
void GJUpdateMatchDBUserCharacters(LPOBJECTSTRUCT lpObj)
{
	SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x16, sizeof(pMsg) );

	pMsg.DbClass = lpObj->DbClass;
	pMsg.Level	 = lpObj->Level;
	
	CopyMemory( pMsg.szId, lpObj->AccountID, MAX_IDSTRING );
	CopyMemory( pMsg.szName, lpObj->BackName, MAX_IDSTRING );
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}
#endif

#ifdef ADD_HACKING_TOOL_BLOCK_20090311

#include "HackToolBlock.h"

void GJRequestAntiHackStatistics(char* pchFileName)
{
	PMSG_HACKTOOL_USE_STATISTICS_REQ pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x8A, sizeof( pMsg ) );
	
	memcpy( pMsg.chHackToolFilename, pchFileName, 64 );
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size );
}

void JGRequestAntiHackStatistics(LPPMSG_HACKTOOL_USE_STATISTICS lpMsg)
{
	// 인증 서버로 부터 해킹툴의 통계에 대한 확인 요청을 받았다.
	// 전체 유저에게 해당 프로그램의 사용 여부에 대한 확인을 요청한다.
	g_HackToolBlock.HacktoolStatisticsAll( lpMsg->chHackToolFilename );
	// 해당 로그를 남긴다.
}

void GJAntiHackStatisticsResult(char* pchFilename, int iHacktoolUserCount)
{
	PMSG_HACKTOOL_USE_STATISTICS_ANS pMsg = {0,};
	
	if( pchFilename == NULL )
	{
		return;
	}
	
	PHeadSetB( (LPBYTE)&pMsg, 0x8C, sizeof( pMsg ) );
	
	memcpy( pMsg.chHackToolFilename, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	pMsg.wHackToolUserCount = iHacktoolUserCount;
	
	wsJServerCli.DataSend( (char*)&pMsg, pMsg.h.size );
}

void GJRequestHacktoolMapMoveBlock(BYTE btModifyType, char* pchFilename, DWORD dwReservationTime, DWORD dwBlockTerm, WORD wBlockRate)
{
	PMSG_RESERVE_HACKTOOL_MAP_MOVE_REQ pMsg;
	
	PHeadSetB( (LPBYTE)&pMsg, 0x8B, sizeof( pMsg ) );
	
	memcpy( pMsg.chHackToolFilename, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	pMsg.btModifyType = btModifyType;
	pMsg.dwReservationTime = dwReservationTime;
	pMsg.dwBlockTerm = dwBlockTerm;
	pMsg.wBlockRate = wBlockRate;
	
	wsJServerCli.DataSend( (char*)&pMsg, pMsg.h.size );
}

void JGRequestHacktoolMapMoveBlock(LPPMSG_RESERVE_HACKTOOL_MAP_MOVE lpMsg)
{
	// 인증서버로 부터 받은 해킹 툴 사용자의 맵 이동 제한을 설정한다.
	// btModifyType에 따라 해킹 툴 등록 타입이 바뀐다.
	switch( lpMsg->btModifyType )
	{
	case HACKTOOL_MODIFY_TYPE_ADD:
		{
			g_HackToolBlock.AddHacktoolData( lpMsg->chHackToolFilename, lpMsg->dwReservationTime, lpMsg->dwBlockTerm, lpMsg->wBlockRate );
		}
		break;
	case HACKTOOL_MODIFY_TYPE_MODIFY:
		{
			g_HackToolBlock.ModifyHacktoolData( lpMsg->chHackToolFilename, lpMsg->dwReservationTime, lpMsg->dwBlockTerm, lpMsg->wBlockRate );
		}
		break;
	case HACKTOOL_MODIFY_TYPE_REMOVE:
		{
			g_HackToolBlock.RemoveHacktoolData( lpMsg->chHackToolFilename );
		}
		break;
	}
}

//PMSG_RESERVE_HACKTOOL_MAP_MOVE_REQ
//void CJRequestHacktoolMapmoveReserve(char* pchFilename, 
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
//----------------------------------------------------------------------------
// GJ [0x8F] 블럭시킬 아이피를 등록한다. (0xC1) 
//----------------------------------------------------------------------------
void GJRequestBlockIp(char *BlockIp)
{
	PMSG_BLOCK_IP pMsg;
	PHeadSetB( (LPBYTE)&pMsg, 0x8F, sizeof( pMsg ) );
	memcpy( pMsg.BlockIp, BlockIp, 16 );
	
	wsJServerCli.DataSend( (char*)&pMsg, pMsg.h.size );
}
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605