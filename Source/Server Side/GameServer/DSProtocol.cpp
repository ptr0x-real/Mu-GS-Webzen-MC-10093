//----------------------------------------------------------------------------
// GAME SERVER - DSProtocol.CPP
// DB 서버와의 정보를 주고받는 프로토콜 처리 모듈
//----------------------------------------------------------------------------
#include "stdafx.h"
#include <CommCtrl.h>
#include "..\\Include\\Prodef.h"
#include "..\\Include\\SProdef.h"
#include "..\\Common\\winutil.h"
#include "User.h"
#include "Protocol.h"
#include "GameServer.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "GameMain.h"
#include "GuildClass.h"
#include "DBSockMng.h"
#include "QuestUtil.h"
#include "PickUpSystem.h"
#ifdef MODIFY_MIX_SYSTEM_20070518
//#include "ChaosBox.h"
#include "MixSystem.h"
#endif

#include "SimpleModulus.h"
#include "giocp.h"
#include "DbSave.h"
#include "DSProtocol.h"

#include "ObjCalCharacter.h"

#ifdef MAP_SERVER_WORK_20041030
#include "MapServerManager.h"
#include "Sprotocol.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
#include "CastleSiegeSync.h"
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "ChinaHackUserKick.h"
#include "Sprotocol.h"
#endif

#ifdef _NEW_EXDB_
//##EXDB##
#include "EDSProtocol.h"
//##EXDB##
#endif

#ifdef NEW_SKILL_FORSKYLAND
#include "QuestInfo.h"
#endif

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef DELETE_DARKLORD_PET_SERIAL_ZERO_20050617
extern BYTE gObjWarehouseDeleteItem(int aIndex, int itempos);
#endif

#ifdef MU_CRYWOLF_SERVER_SYNC_20050505
#include "CrywolfSync.h"
#endif 

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// #include "ObjUseSkill.h"
#include "ObjUseSkill.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704
#include "QuestUtil.h"
#endif

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
#include "CashItemPeriodSystem.h"
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
#include "MasterLevelSystem.h"
#endif

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
#include "KeyGenerater.h"
#endif // ADD_MAPMOVE_PROTOCOL_20090327

#include "TNotice.h"

//----------------------------------------------------------------------------
// E X T E R N
//----------------------------------------------------------------------------
extern CwsGameServer	wsGServer;
extern wsJoinServerCli  wsJServerCli;
extern wsJoinServerCli  wsDataCli;
extern CDbSave			gDbSave;
extern CGuildClass		Guild;
extern CSimpleModulus	g_SimpleModulusCS;
extern CSimpleModulus	g_SimpleModulusSC;
extern ITEM_ATTRIBUTE   ItemAttribute[];

#ifdef GAMESERVER_INFO_TCP_20031203
extern wsJoinServerCli	wsConnectServerCli;
#endif

#ifdef __FRIEND_WORK__
extern wsJoinServerCli	wsExDbCli;
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912
extern CMasterLevelSystem	g_MasterLevelSystem;
#endif
//----------------------------------------------------------------------------
// F U N C T I O N 
//----------------------------------------------------------------------------
void GCItemListSend(int aIndex);

void JGPGetCharList(BYTE *lpRecv);
void JGCharacterCreateRequest(LPSDHP_CREATECHARRESULT lpMsg);
void JGCharDelRequest(LPSDHP_CHARDELETERESULT lpMsg);
void JGGetCharacterInfo(LPSDHP_DBCHAR_INFORESULT lpMsg);
void DataServerLoginResult(LPSDHP_RESULT lpMsg);

void TestDSSend();

void ItemSerialCreateRecv(LPSDHP_ITEMCREATERECV lpMsg);
void GDSetWarehouseList(int aIndex);
void DGGetWarehouseList(LPSDHP_GETWAREHOUSEDB_SAVE lpMsg);
void GDGetWarehouseNoItem(LPSDHP_GETWAREHOUSEDB_RESULT lpMsg);
void DGOptionDataRecv(LPSDHP_SKILLKEYDATA_SEND lpMsg);
void DGGuildListState(LPSDHP_GUILDLISTSTATE lpMsg);

void DGMoveOtherServer(LPSDHP_CHARACTER_TRANSFER_RESULT lpMsg);


#ifdef DARKLORD_WORK	
void DGRecvPetItemInfo(LPBYTE lpMsg);
#endif

#ifdef GS_DS_LIVECHECK_20041008			// [0x58] GS -> DS 로 LIVE 신호를 지속적으로 보낸다.
void GS_GDReqLiveCheck();
void GS_DGAnsLiveCheck(LPSDHP_ANS_GSLIVECHECK aRecv);
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM	
void DGBlockHackUserResult(LPSDHP_HACKUSERBLOCK_RESULT aRecv);
#endif

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
void DGChangeNameResult(LPSDHP_CHANGE_NAME_RESULT lpMsg);
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef ADD_CHARCARD_SUMMONER_20080313
void JGGetAccountInfo(LPSDHP_ANS_ACCOUNTINFO lpMsg);
void JGSetAccountInfo(LPSDHP_ANS_SETACCOUNTINFO lpMsg);
#endif // ADD_CHARCARD_SUMMONER_20080313

#ifndef _NEW_EXDB_

//##EXDB##
// 길드리스트보여지는데 사용되는 변수들..
HWND GuildListhDlg=NULL;
HWND GuildListhPrs=NULL;
HWND GuildListhSts=NULL;
int  gGuildListCount=0;
int  gGuildListTotal=0;
//##EXDB##
#endif

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
// '[ CRYWOLF ] SYNC Occupation: %d, State: %d' 로그를 1분마다 남기기 위한 틱 저장 변수
DWORD dwCrywolfSyncLogTickCount = 0;
#endif

//----------------------------------------------------------------------------
void DataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case 0x00 :
		DataServerLoginResult((LPSDHP_RESULT)aRecv);
		break;
	case 0x01 : // 캐릭터 리스트 요청 결과
		JGPGetCharList(aRecv);
		break;
	//#BUGFIX_20040203_01
	//case 0x02 부분을 EDSProtocol.cpp 로 이전
	//#BUGFIX_20040203_01
	case 0x04 :	// 캐릭터 만들기 요청 결과
		JGCharacterCreateRequest((LPSDHP_CREATECHARRESULT)aRecv);
		break;
	case 0x05 : // 캐릭터 삭제 결과
		JGCharDelRequest((LPSDHP_CHARDELETERESULT)aRecv);
		break;
	case 0x06 :	// 캐릭터 정보를 달라고 한 요청 결과
		JGGetCharacterInfo((LPSDHP_DBCHAR_INFORESULT)aRecv);
		break;
	case 0x08 :	// 창고 아이템 리스트를 얻는다.
		DGGetWarehouseList((LPSDHP_GETWAREHOUSEDB_SAVE)aRecv);
		break;
	case 0x10 :	
		GDGetWarehouseNoItem((LPSDHP_GETWAREHOUSEDB_RESULT)aRecv);
		break;
		
#ifdef MODIFY_QUEST_SYSTEM_20070525	// DG [0x13] 퀘스트 - 몬스터 킬카운트 정보를 받았다.
	case 0x14:	
		g_QuestUtil.DGGetQuestMonKillInfo( aRecv );
		break;
#endif		

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
	case 0x16:
		DGChangeNameResult((LPSDHP_CHANGE_NAME_RESULT) aRecv);
		break;
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// DG 정보 요청 결과
	case 0x30:	// 마스터레벨 정보 요청 결과
		g_MasterLevelSystem.DGAnsMasterLevelInfo( aRecv );
		break;
#endif

	case 0x52 :
		ItemSerialCreateRecv((LPSDHP_ITEMCREATERECV)aRecv);
		break;


#ifdef DARKLORD_WORK	
	case 0x56 :
		DGRecvPetItemInfo((LPBYTE) aRecv);		
		break;
#endif

#ifdef GS_DS_LIVECHECK_20041008			// [0x58] GS -> DS 로 보내온 LIVE 신호를 받아서 응답신호를 보낸다.
	case 0x58 :
		GS_DGAnsLiveCheck((LPSDHP_ANS_GSLIVECHECK) aRecv);		
		break;
#endif
		
	case 0x60 :	// 스킬키를 전송 받는다.
		DGOptionDataRecv((LPSDHP_SKILLKEYDATA_SEND)aRecv);
		break;

#ifdef CHINA_HACKUSER_KICK_SYSTEM
	case 0x70 :	// 핵유저 블럭에 대한 결과를 받는다
		DGBlockHackUserResult((LPSDHP_HACKUSERBLOCK_RESULT)aRecv);
		break;
#endif
		
#ifdef MU_CASTLESIEGE_DS_PROTOCOL_20041105
	case 0x80 :	// 공성전 관련 데이터 처리
		{
			LPPMSG_DEFAULT2 lpDef1 = (LPPMSG_DEFAULT2)aRecv;
			switch(lpDef1->subcode) {
			case 0x00 :		// [0x80][0x00] 성에 대한 전체적인 정보 요청 응답
				GS_DGAnsCastleTotalInfo		(aRecv);
				break;
			case 0X01 :		// [0x80][0x01] 수성측 길드의 길마이름 요청 응답 -> 경비병 NPC 성 상태 출력
				GS_DGAnsOwnerGuildMaster	(aRecv);
				break;
			case 0x03 :		// [0x80][0x03] 수성측 NPC 구입 요청 응답
				GS_DGAnsCastleNpcBuy		(aRecv);
				break;
			case 0x04 :		// [0x80][0x04] 수성측 NPC 수리 요청 응답
				GS_DGAnsCastleNpcRepair		(aRecv);
				break;
			case 0x05 :		// [0x80][0x05] 수성측 NPC 업그레이드 요청 응답
				GS_DGAnsCastleNpcUpgrade	(aRecv);
				break;
			case 0x06 :		// [0x80][0x06] 성주측 세금정보 요청 응답 (소유 젠도 함께 보내야함)
				GS_DGAnsTaxInfo				(aRecv);
				break;
			case 0x07 :		// [0x80][0x07] 성주측 세율 변경 요청 응답
				GS_DGAnsTaxRateChange		(aRecv);
				break;
			case 0x08 :		// [0x80][0x08] 성주측 소유 젠 변경 요청 응답
				GS_DGAnsCastleMoneyChange	(aRecv);
				break;
			case 0x09 :		// [0x80][0x09] 성주측 공성 시작/종료 시간 변경 요청 응답
				GS_DGAnsSiegeDateChange		(aRecv);
				break;
			case 0x0A :		// [0x80][0x0A] 공성측 특정 길드등록 정보 요청 응답
				GS_DGAnsGuildMarkRegInfo	(aRecv);
				break;
			case 0x0B :		// [0x80][0x0B] 현재 공성 종료여부 변경 요청 응답
				GS_DGAnsSiegeEndedChange	(aRecv);
				break;
			case 0x0C :		// [0x80][0x0C] 현재 성 소유상태 변경 요청 응답
				GS_DGAnsCastleOwnerChange	(aRecv);
				break;
			case 0x0D :		// [0x80][0x0D] 공성측 길드의 공성전 등록 요청
				GS_DGAnsRegAttackGuild		(aRecv);
				break;
			case 0x0E :		// [0x80][0x0E] 공성에 대한 주기가 끝났으므로 현재 성에 대한 정보를 리셋 요청 응답
				GS_DGAnsRestartCastleState	(aRecv);
				break;
			case 0x0F :		// [0x80][0x0F] 특정 맵서버군에 메시지를 멀티 캐스팅 요청 응답
				GS_DGAnsMapSvrMsgMultiCast	(aRecv);
				break;
			case 0x10 :		// [0x80][0x10] 특정 길드의 문장 등록을 요청 응답
				GS_DGAnsRegGuildMark		(aRecv);
				break;
			case 0x11 :		// [0x80][0x11] 공성측 특정 길드 마크 개수 초기화 요청 응답
				GS_DGAnsGuildMarkReset		(aRecv);
				break;
			case 0x12 :		// [0x80][0x12] 공성측 특정 길드 공성 포기여부 수정 요청 응답
				GS_DGAnsGuildSetGiveUp		(aRecv);
				break;
			case 0x16 :		// [0x80][0x16] 수성측 NPC 제거 요청 응답
				GS_DGAnsNpcRemove			(aRecv);
				break;
			case 0x17 :		// [0x80][0x17] 현재의 공성상태 동기화 요청 응답
				GS_DGAnsCastleStateSync		(aRecv);
				break;
			case 0x18 :		// [0x80][0x18] 맵서버군의 성 공납금 추가 요청
				GS_DGAnsCastleTributeMoney	(aRecv);
				break;
			case 0x19 :		// [0x80][0x19] 맵서버군의 성 세율, 돈 초기화 요청 응답
				GS_DGAnsResetCastleTaxInfo	(aRecv);
				break;
			case 0x1A :		// [0x80][0x1A] 공성 참여길드 리스트 초기화 요청 응답
				GS_DGAnsResetSiegeGuildInfo	(aRecv);
				break;
			case 0x1B :		// [0x80][0x1B] 공성 참여신청 길드 리스트 초기화 요청 응답
				GS_DGAnsResetRegSiegeInfo	(aRecv);
				break;
			}
		}
		break;
	case 0x81 :				// [0x81] 공성전 최초 모든 초기화 데이터 요청 응답
		GS_DGAnsCastleInitData				(aRecv);
		break;
	case 0x82 :				// [0x82] 수성측 NPC 정보 요청 응답
		GS_DGAnsCastleNpcInfo				(aRecv);
		break;
	case 0x83 :				// [0x83] 공성측 길드등록 정보 요청 응답 (길드 개수 N개 제한)
		GS_DGAnsAllGuildMarkRegInfo			(aRecv);
		break;
	case 0x84 :				// [0x84] 수성측 NPC 리스트 즉시 생성 요청 응답
		GS_DGAnsFirstCreateNPC				(aRecv);
		break;
	case 0x85 :				// [0x85] 공성측 길드선정 후보자료 요청 응답
		GS_DGAnsCalcREgGuildList			(aRecv);
		break;
	case 0x86 :				// [0x86] 공/수 길드의 연합정보를 요청 응답
		GS_DGAnsCsGulidUnionInfo			(aRecv);
		break;
	case 0x87 :				// [0x87] 공/수 길드의 최종정보를 저장 요청 응답
		GS_DGAnsCsSaveTotalGuildInfo		(aRecv);
		break;
	case 0x88 :				// [0x88] 공/수 길드의 최종정보를 불러오기 요청 응답
		GS_DGAnsCsLoadTotalGuildInfo		(aRecv);
		break;
	case 0x89 :				// [0x89] 수성측 NPC 리스트 정보갱신 요청 응답
		GS_DGAnsCastleNpcUpdate				(aRecv);
		break;
#endif
		
#ifdef MU_CRYWOLF_DS_PROTOCOL_20050505
	case 0xB0 :				// [0xB0] 크라이울프 동기화 요청 응답
		DGAnsCrywolfSync					(aRecv);
		break;
	case 0xB1 :				// [0xB1] 크라이울프 동기화 요청 응답
		DGAnsCrywolfInfoLoad					(aRecv);
		break;
	case 0xB2 :				// [0xB2] 크라이울프 동기화 요청 응답
		DGAnsCrywolfInfoSave					(aRecv);
		break;
#endif

#ifdef GAMESERVER_DIVISION
	case 0xCF:	// 서버이전에 대한 결과를 받는다
		DGMoveOtherServer((LPSDHP_CHARACTER_TRANSFER_RESULT) aRecv);		
		break;
#endif

#ifdef ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205
	case 0xD0:
		{
			LPPMSG_DEFAULT2 lpMsg = (LPPMSG_DEFAULT2)aRecv;
			switch( lpMsg->subcode )
			{
			case 0x02:				// 기간제 아이템 정보 확인
				g_CashItemPeriodSystem.DGAnsPeriodItemInquiry( (LPPMSG_ANS_PERIODITEM_INQUIRY)aRecv );
				break;
			case 0x04:				// 기간제 아이템 추가 결과
				g_CashItemPeriodSystem.DGAnsPeriodItemInsert( (LPPMSG_ANS_PERIODITEM_INSERT)aRecv );
				break;
			case 0x06:				// 기간제 아이템 리스트
				g_CashItemPeriodSystem.DGAnsPeriodItemList( (LPPMSG_ANS_PERIODITEM_LIST)aRecv );
				break;
			case 0x08:				// 기간제 아이템 업데이트
				g_CashItemPeriodSystem.DGAnsPeriodItemUpdate( (LPPMSG_ANS_PERIODITEM_UPDATE)aRecv );
				break;
			}
		}
		break;
#endif // ADD_PARTIALY_CHARGE_PERIOD_SYSTEM_20070205

#ifdef ADD_CHARCARD_SUMMONER_20080313
	case 0xA1:
		{
			JGGetAccountInfo((LPSDHP_ANS_ACCOUNTINFO)aRecv);
		} break;
	case 0xA2:
		{
			JGSetAccountInfo((LPSDHP_ANS_SETACCOUNTINFO)aRecv);
		} break;
#endif // ADD_CHARCARD_SUMMONER_20080313

	case 0xA8:
		g_ResetSystem.DBLoadReset(aRecv);
		break;

	case 0xA9:
		g_ResetSystem.DBAnsUpdateReset(aRecv);
		break;

	/*case 0xAA:
		g_VIPSystem.UpdateVip((LPPMSG_ANS_VIP)aRecv);
		break;*/

	case 0xFF :
		{
		LPPMSG_TEST	pMsg = (LPPMSG_TEST)aRecv;
		LogAddC(LOGC_RED,"error-L2DataServer RECV : %d (%d)", aLen, pMsg->count);
		//TestDSSend();
		}
		break;

	}	
}


//#ifdef PERSONAL_SHOP_20040113

int	iCount=0;

void TestDSSend()
{
	PMSG_TEST	pMsg;
	int			size;
	
	size = sizeof( PMSG_TEST );
	pMsg.h.c			= 0xC2;
	pMsg.h.headcode		= 0xFF;
	pMsg.h.sizeH			= HIBYTE(size);
	pMsg.h.sizeL			= LOBYTE(size);
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);	
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	wsprintf(pMsg.testbuf,"%d", iCount);LogAdd("%s",pMsg.testbuf);
	cDBSMng.Send((char*)&pMsg, size); iCount++;
	/*
	cDBSMng.Send((char*)&pMsg, size);
	cDBSMng.Send((char*)&pMsg, size);
	cDBSMng.Send((char*)&pMsg, size);
	cDBSMng.Send((char*)&pMsg, size);
	cDBSMng.Send((char*)&pMsg, size);
	cDBSMng.Send((char*)&pMsg, size);
	*/
	//cDBSMng.Send((char*)&pMsg, size);
	//cDBSMng.Send((char*)&pMsg, size);
	//cDBSMng.Send((char*)&pMsg, size);

	LogAdd("Test Data Send");
}

//#endif


//----------------------------------------------------------------------------
// [0x00] 인증서버에 접속할때 이 프로그램의 정보를 보낸다.
//----------------------------------------------------------------------------
void DataServerLogin(int server)
{
	SDHP_SERVERINFO	   pInfo;

	pInfo.h.c		 = PMHC_BYTE;
	pInfo.h.size	 = sizeof(pInfo);
	pInfo.h.headcode = 0x00;
	pInfo.Port		 = GameServerPort;
	pInfo.Type		 = ST_GAMESERVER;

	pInfo.ServerCode = gGameServerCode;

	strcpy( pInfo.ServerName, szServerName);

	cDBSMng.Send(server, (char*)&pInfo, pInfo.h.size);
}

//----------------------------------------------------------------------------
// [0x00] 데이터 서버에 접속후 결과
//----------------------------------------------------------------------------
void DataServerLoginResult(LPSDHP_RESULT lpMsg)
{
	if( lpMsg->Result == FALSE )
	{
		MsgBox(lMsg.Get(466));
	}
	else
	{
		//gItemNumberCount = lpMsg->ItemCount;
		//LogAdd("아이템 번호 설정 : %d", gItemNumberCount);
		DataServerConnected = 1;
		gServerReady++;
		SendMessage(ghWnd, WM_ASS_CREATEGAMESERVER, 0, 0);
	}
}



//----------------------------------------------------------------------------
// [0x01] 캐릭터 리스트를 게임서버로 부터 얻는다.
//----------------------------------------------------------------------------
void JGPGetCharList(BYTE *lpRecv)
{
	LPSDHP_CHARLISTCOUNT	lpCount = (LPSDHP_CHARLISTCOUNT)lpRecv;
	LPSDHP_CHARLIST			lpCL;
	
	char szId[MAX_IDSTRING+1];
	BYTE sendbuf[256];
	int  lOfs=0;
	int  lsOfs=sizeof(SDHP_CHARLISTCOUNT);
	int  aIndex = lpCount->Number;
	BYTE index;
	
	PMSG_CHARLISTCOUNT	pCLCount;
	PMSG_CHARLIST		pCList;

#ifdef ITEM_INDEX_EXTEND_20050706
	WORD TempInventory[12];
#else
	BYTE TempInventory[12];
#endif

	pCLCount.h.c        = PMHC_BYTE;
	pCLCount.h.headcode = 0xF3;
	pCLCount.subcode    = 0x00;
	pCLCount.Count      = lpCount->Count;

	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpCount->AccountId, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(421),gObj[aIndex].AccountID, szId);
		CloseClient ( aIndex );
		return;
	}

	gObj[aIndex].Magumsa = lpCount->Magumsa;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	gObj[aIndex].Magumsa = 1;
#endif

#ifdef FOR_NEW_TESTSERVER
//	if( gTestServer )
	{
		gObj[aIndex].Magumsa = 2;
	}
#endif

	#ifdef DARKLORD_WORK	
		pCLCount.MaxClass = gObj[aIndex].Magumsa+2;
	#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		pCLCount.MaxClass += 1;
#endif		

#ifdef CHARACTER_MOVE_20040810
	pCLCount.MoveCnt = lpCount->MoveCnt;
#endif

	memset( sendbuf, 0, sizeof(sendbuf));
	lOfs += sizeof(pCLCount);

	if( pCLCount.Count > 0 )
	{
		for( int n=0; n<pCLCount.Count; n++)
		{
#ifdef ITEM_INDEX_EXTEND_20050706
			memset(&pCList, 0, sizeof(pCList));
#endif

			lpCL = (LPSDHP_CHARLIST)(lpRecv+lsOfs);
			pCList.Index = lpCL->Index;
			pCList.Level = lpCL->Level;
			pCList.CtlCode = lpCL->CtlCode;
			
#ifdef ADD_CHARACTERLIST_GUILD_STATUS_01_20050126
			pCList.btGuildStatus = lpCL->btGuildStatus;
#endif
			if( gObj[aIndex].m_cAccountItemBlock )
			{	// 계정 캐릭터 블럭 표시
				pCList.CtlCode |= 0x10;
			}

			memcpy(pCList.Name, lpCL->Name, MAX_IDSTRING);
			
			pCList.CharSet[0] = ((lpCL->Class>>4)<<5)&0xE0; 
#ifdef NEW_SKILL_FORSKYLAND	
			//(lpMsg->Class>>1)&0x01;
			int changeup = (lpCL->Class)&CHANGEUP_MASK;
			pCList.CharSet[0] |= (changeup<<4)&0x10;			
#else
			pCList.CharSet[0] |= ((lpCL->Class&0x0F)<<4)&0x10;		// skin
#endif

#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 클라이언트에 3차전직 정보를 보낸다.			
			pCList.CharSet[0] |= ( ( changeup >> 1 )<< 3)&0x08;
#endif
			
			pCList.CharSet[9] = 0x00;
			
			
			//---------------------------------------------------------------------------------
			//
			//  [] DB Version - 0x00
			//
			//	   Check Date - 20051117
			// 	
			//---------------------------------------------------------------------------------
			if( lpCL->DbVersion == 0x00 )
			{
//				if( lpCL->dbInventory[4] == 0xFF )
//					index = 0xF0;
//				else index = (lpCL->dbInventory[4]&0x0F)<<4;

#ifdef ITEM_INDEX_EXTEND_20050706
				// 오른손 무기
				if (lpCL->dbInventory[0] != 0xFF) 
				{
					WORD wItemCode = (lpCL->dbInventory[0]/32)*MAX_ITEM_INDEX+(lpCL->dbInventory[0]%32);
					pCList.CharSet[1] = (BYTE) wItemCode;
					pCList.CharSet[12] |= (BYTE) ((wItemCode&0x0F00) >> 4);
				}
				else 
				{
					pCList.CharSet[1] = 0xFF;
					pCList.CharSet[12] |= 0xF0;
				}
				// 왼손 무기
				if (lpCL->dbInventory[2] != 0xFF) 
				{
					WORD wItemCode = (lpCL->dbInventory[2]/16)*MAX_ITEM_INDEX+(lpCL->dbInventory[2]%16);
					pCList.CharSet[2] = (BYTE) wItemCode;
					pCList.CharSet[13] |= (BYTE) ((wItemCode&0x0F00) >> 4);
				}
				else 
				{
					pCList.CharSet[2] = 0xFF;
					pCList.CharSet[13] |= 0xF0;
				}
#else
				pCList.CharSet[1] = (lpCL->dbInventory[0]/16)*MAX_ITEM_INDEX+(lpCL->dbInventory[0]%16);
				pCList.CharSet[2] = (lpCL->dbInventory[2]/16)*MAX_ITEM_INDEX+(lpCL->dbInventory[2]%16);
#endif

				if( lpCL->dbInventory[4] == 0xFF )
					index = 0xF0;
				else index = (lpCL->dbInventory[4]&0x0F)<<4;
				if( lpCL->dbInventory[6] == 0xFF )
					index |= 0x0F;
				else index |=(lpCL->dbInventory[6]&0x0F);
				pCList.CharSet[3] = index;
				
				if( lpCL->dbInventory[8] == 0xFF )
					index = 0xF0;					
				else index = (lpCL->dbInventory[8]&0x0F)<<4;
				if( lpCL->dbInventory[10] == 0xFF )
					index |= 0x0F;
				else index |=(lpCL->dbInventory[10]&0x0F);
				pCList.CharSet[4] = index;
				
				if( lpCL->dbInventory[12] == 0xFF )
					index = 0xF0;					
				else index = (lpCL->dbInventory[12]&0x0F)<<4;

				// 날개와 헬퍼는 2비트씩
				if( lpCL->dbInventory[14] == 0xFF )
					index |= 0x0C;
				else index |=(lpCL->dbInventory[14]&0x03)<<2;
				if( lpCL->dbInventory[16] == 0xFF  )
					index |= 0x03;
				else index |=(lpCL->dbInventory[16]&0x03);
				pCList.CharSet[5] = index;

				if( lpCL->dbInventory[4] == 0xFF ) pCList.CharSet[9]  = 0x80;
				if( lpCL->dbInventory[6] == 0xFF ) pCList.CharSet[9] |= 0x40;
				if( lpCL->dbInventory[8] == 0xFF ) pCList.CharSet[9] |= 0x20;
				if( lpCL->dbInventory[10] == 0xFF ) pCList.CharSet[9] |= 0x10;
				if( lpCL->dbInventory[12] == 0xFF ) pCList.CharSet[9] |= 0x08;
				//if( lpCL->dbInventory[9] == 0xFF ) pCList.CharSet[9] |= 0x80;

				int levelindex;
				
				levelindex  = ((int)LevelSmallConvert((lpCL->dbInventory[1]>>3)&0xF));
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[3]>>3)&0xF))<<3);
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[5]>>3)&0xF))<<6);
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[7]>>3)&0xF))<<9);
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[9]>>3)&0xF))<<12);
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[11]>>3)&0xF))<<15);
				levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[13]>>3)&0xF))<<18);
				
				pCList.CharSet[6] = (BYTE)((levelindex>>16)&255);
				pCList.CharSet[7] = (BYTE)((levelindex>> 8)&255);
				pCList.CharSet[8] = (BYTE)((levelindex    )&255);
				//pCList.CharSet[9] = 0;
				pCList.CharSet[10]= 0;
#ifdef DARKLORD_WORK
				pCList.CharSet[11]= 0;
#endif

#ifdef DARKLORD_WORK	
#ifndef ITEM_INDEX_EXTEND_20050706
				pCList.CharSet[12]= 0;
#endif
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
				// 아이템 유무에 따라서 케릭터 뷰에 추가된 비트들을 셋한다.

				// 오른손 무기
//				if( lpCL->dbInventory[0] == 0xFF )	pCList.CharSet[12] |= 0xF0;
				// 왼손 무기
//				if( lpCL->dbInventory[2] == 0xFF )	pCList.CharSet[13] |= 0xF0;
				// 헬멧
				if( lpCL->dbInventory[4] == 0xFF )	pCList.CharSet[13] |= 0x0F;
				// 갑옷
				if( lpCL->dbInventory[8] == 0xFF )	pCList.CharSet[14] |= 0xF0;
				// 바지
				if( lpCL->dbInventory[10] == 0xFF )	pCList.CharSet[14] |= 0x0F;
				// 장갑
				if( lpCL->dbInventory[12] == 0xFF )	pCList.CharSet[15] |= 0xF0;
				// 부츠
				if( lpCL->dbInventory[14] == 0xFF )	pCList.CharSet[15] |= 0x0F;

				// 예비 바이트 셋
				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;
#endif
			}

			//---------------------------------------------------------------------------------
			//
			//  [] DB Version - 0x01, 0x02
			//
			//	   Check Date - 20051117
			// 	
			//---------------------------------------------------------------------------------
#ifdef ITEM_DBSIZE_EXTEND_20050706
			else if (lpCL->DbVersion <= 0x02)
#else
			else
#endif
			{
#ifdef ITEM_INDEX_EXTEND_20050706
				// 오른손 무기
				if (lpCL->dbInventory[0] != 0xFF)
					TempInventory[0] = (lpCL->dbInventory[0]/32)*MAX_ITEM_INDEX+(lpCL->dbInventory[0]%32);
				else
					TempInventory[0] = 0xFFFF;
				// 왼손 무기
				if (lpCL->dbInventory[3] != 0xFF)
					TempInventory[1] = (lpCL->dbInventory[3]/32)*MAX_ITEM_INDEX+(lpCL->dbInventory[3]%32);
				else
					TempInventory[1] = 0xFFFF;
#else
				TempInventory[0] = lpCL->dbInventory[0];
				TempInventory[1] = lpCL->dbInventory[3];
#endif

				if( lpCL->dbInventory[6] == 255 && ((lpCL->dbInventory[8]&0x80)==0x80) )
					TempInventory[2] = 0xFFFF;
				else TempInventory[2] = MAKE_ITEMNUM_32(lpCL->dbInventory[6], lpCL->dbInventory[8]);
				
				if( lpCL->dbInventory[9] == 255 && ((lpCL->dbInventory[11]&0x80)==0x80) )
					TempInventory[3] = 0xFFFF;
				else TempInventory[3] = MAKE_ITEMNUM_32(lpCL->dbInventory[9], lpCL->dbInventory[11]);
				
				if( lpCL->dbInventory[12] == 255 && ((lpCL->dbInventory[14]&0x80)==0x80) )
					TempInventory[4] = 31;
				else TempInventory[4] = MAKE_ITEMNUM_32(lpCL->dbInventory[12], lpCL->dbInventory[14]);
				
				if( lpCL->dbInventory[15] == 255 && ((lpCL->dbInventory[17]&0x80)==0x80) )
					TempInventory[5] = 31;
				else TempInventory[5] = MAKE_ITEMNUM_32(lpCL->dbInventory[15], lpCL->dbInventory[17]);

				if( lpCL->dbInventory[18] == 255 && ((lpCL->dbInventory[20]&0x80)==0x80) )
					TempInventory[6] = 31;
				else TempInventory[6] = MAKE_ITEMNUM_32(lpCL->dbInventory[18], lpCL->dbInventory[20]);

				if( lpCL->dbInventory[21] == 255 && ((lpCL->dbInventory[23]&0x80)==0x80) )
					TempInventory[7] = 31;
				else TempInventory[7] = MAKE_ITEMNUM_32(lpCL->dbInventory[21], lpCL->dbInventory[23]);

				if( lpCL->dbInventory[24] == 255 && ((lpCL->dbInventory[26]&0x80)==0x80) )
					TempInventory[8] = 31;
				else TempInventory[8] = MAKE_ITEMNUM_32(lpCL->dbInventory[24], lpCL->dbInventory[26]);
												
#ifdef ITEM_INDEX_EXTEND_20050706
				// 오른손 무기
				if (TempInventory[0] != 0xFFFF) {
					pCList.CharSet[1] = (BYTE) TempInventory[0];
					pCList.CharSet[12] |= (BYTE) ((TempInventory[0]&0x0F00) >> 4);
				}
				else {
					pCList.CharSet[1] = 0xFF;
					pCList.CharSet[12] |= 0xF0;
				}
				// 왼손 무기
				if (TempInventory[1] != 0xFFFF) {
					pCList.CharSet[2] = (BYTE) TempInventory[1];
					pCList.CharSet[13] |= (BYTE) ((TempInventory[1]&0x0F00) >> 4);
				}
				else {
					pCList.CharSet[2] = 0xFF;
					pCList.CharSet[13] |= 0xF0;
				}
#else
				pCList.CharSet[1] = (BYTE) TempInventory[0];
				pCList.CharSet[2] = (BYTE) TempInventory[1];
#endif
				
				pCList.CharSet[3] = ((TempInventory[2]%16)<<4)+(TempInventory[3]%16);
				pCList.CharSet[4] = ((TempInventory[4]%16)<<4)+(TempInventory[5]%16);
				index             = (TempInventory[6]%16)<<4;
				
				// 날개와 헬퍼는 2비트씩
				if( TempInventory[7] == 31 )
					index |= 0x0C;
				else index |=(TempInventory[7]&0x03)<<2;
				
#ifdef DARKLORD_WORK
				if( !(TempInventory[8]&0x04) )
				{	// 다크호스가 아니면
					if( TempInventory[8] == 31  )
					index |= 0x03;
					else index |=(TempInventory[8]&0x03);
				}
				else
				{					
					index |=  0x03;		// 다크호스를 장착하면 수호천사가 안보이게 하기 위해										
				}
#else
				if( TempInventory[8] == 31  )
					index |= 0x03;
				else index |=(TempInventory[8]&0x03);
#endif
				
				pCList.CharSet[5] = index;
				
				int levelindex=0;
								
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[0] != 0xFFFF )
					levelindex  = ((int)LevelSmallConvert((lpCL->dbInventory[1]>>3)&0xF));
				if( TempInventory[1] != 0xFFFF )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[4]>>3)&0xF))<<3);
#else
				if( TempInventory[0] != 0xFF )
					levelindex  = ((int)LevelSmallConvert((lpCL->dbInventory[1]>>3)&0xF));
				if( TempInventory[1] != 0xFF )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[4]>>3)&0xF))<<3);
#endif
				if( TempInventory[2] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[7]>>3)&0xF))<<6);
				if( TempInventory[3] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[10]>>3)&0xF))<<9);
				if( TempInventory[4] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[13]>>3)&0xF))<<12);
				if( TempInventory[5] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[16]>>3)&0xF))<<15);
				if( TempInventory[6] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[19]>>3)&0xF))<<18);
				
				//levelindex = 0;
				
				pCList.CharSet[6] = (BYTE)((levelindex>>16)&255);
				pCList.CharSet[7] = (BYTE)((levelindex>> 8)&255);
				pCList.CharSet[8] = (BYTE)((levelindex    )&255);


				if( TempInventory[2] > 15 ) pCList.CharSet[9]  = 0x80;
				if( TempInventory[3] > 15 ) pCList.CharSet[9] |= 0x40;
				if( TempInventory[4] > 15 ) pCList.CharSet[9] |= 0x20;
				if( TempInventory[5] > 15 ) pCList.CharSet[9] |= 0x10;
				if( TempInventory[6] > 15 ) pCList.CharSet[9] |= 0x08;

#ifdef NEW_FORSKYLAND2
				// 날개....
	#ifdef DARKLORD_WORK
				if( (TempInventory[7] >= 3 && TempInventory[7] <= 6) || 
					TempInventory[7] == 30 )
	#else
				if( TempInventory[7] >= 3 && TempInventory[7] <= 6)
	#endif	// #ifdef DARKLORD_WORK
				{
					pCList.CharSet[5] |= 0x0C;

	#ifdef DARKLORD_WORK
					if( TempInventory[7] == 30 )
						pCList.CharSet[9] |= 0x05;		// 망토 설정(0x07까지 마스킹이 되므로 앞으로 두개 더 쓸수 있음)
					else
	#endif	// #ifdef DARKLORD_WORK
						pCList.CharSet[9] |= ((TempInventory[7]-2)&0x7);
				}
	#ifdef ADD_THIRD_WING_20070525	// 캐릭터 정보 보낼 때 3차날개 추가
		#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 차원의 날개 캐릭터 정보 추가
				if( ( TempInventory[7] >= 36 && TempInventory[7] <= 40 )
					|| TempInventory[7] == 43)	
		#else
				if( TempInventory[7] >= 36 && TempInventory[7] <= 40 )
		#endif
				{
					pCList.CharSet[5] |= 0x0C;
					pCList.CharSet[16] |= ( (( TempInventory[7] - 35 )&0x7 ) << 2 );
		#ifdef MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109	// 차원의 날개는 0x06으로 보냄
					if(TempInventory[7] == 43)
					{
						pCList.CharSet[16] |= ( (( 0x06 )&0x7 ) << 2 );
					}
		#endif	// MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109					
				}
	#endif	// ADD_THIRD_WING_20070525
#else
	#ifdef NEW_SKILL_FORSKYLAND
				// 페가시아용...
				if( TempInventory[8] & 0x03 && TempInventory[8] != 31 )
				{
					pCList.CharSet[9] |= (BYTE)0x04;
				}
	#endif
#endif	

				pCList.CharSet[10] = 0;

#ifdef NEW_FORSKYLAND2
				// 페가시아용...
				if( TempInventory[8] & 0x03 && TempInventory[8] != 31 )
				{
					pCList.CharSet[10] |= (BYTE)0x01;
				}
#endif

#ifdef DARKLORD_WORK
				pCList.CharSet[11]  = 0;
#endif

#ifdef DARKLORD_WORK
#ifndef ITEM_INDEX_EXTEND_20050706
				pCList.CharSet[12]= 0;
#endif				
				if( TempInventory[8] & 0x04 && TempInventory[8] != 31 )
				{	// 다크호스
					pCList.CharSet[12] |= (BYTE)0x01;
				}
#endif
				/*if( lpObj->pInventory[EQUIPMENT_HELM  ].IsExtItem()) lpObj->CharSet[10] = 0x80;
				if( lpObj->pInventory[EQUIPMENT_ARMOR ].IsExtItem()) lpObj->CharSet[10] = 0x40;
				if( lpObj->pInventory[EQUIPMENT_PANTS ].IsExtItem()) lpObj->CharSet[10] = 0x20;
				if( lpObj->pInventory[EQUIPMENT_GLOVES].IsExtItem()) lpObj->CharSet[10] = 0x10;
				if( lpObj->pInventory[EQUIPMENT_BOOTS ].IsExtItem()) lpObj->CharSet[10] = 0x08;*/

#ifdef ITEM_INDEX_EXTEND_20050706
				// 아이템 유무에 따라서 케릭터 뷰에 추가된 비트들을 셋한다.

				// 오른손 무기
//				if( TempInventory[0] == 0xFFFF )	pCList.CharSet[12] |= 0xF0;
				// 왼손 무기
//				if( TempInventory[1] == 0xFFFF )	pCList.CharSet[13] |= 0xF0;
				// 헬멧
				if( TempInventory[2] >= 31 )	pCList.CharSet[13] |= 0x0F;
				// 갑옷
				if( TempInventory[3] >= 31 )	pCList.CharSet[14] |= 0xF0;
				// 바지
				if( TempInventory[4] >= 31 )	pCList.CharSet[14] |= 0x0F;
				// 장갑
				if( TempInventory[5] >= 31 )	pCList.CharSet[15] |= 0xF0;
				// 부츠
				if( TempInventory[6] >= 31 )	pCList.CharSet[15] |= 0x0F;

				// 예비 바이트 셋
				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;
#endif

			}

			//---------------------------------------------------------------------------------
			//
			//  [] DB Version - 0x03
			//
			//	   Check Date - 20051117
			// 	
			//---------------------------------------------------------------------------------
#ifdef ITEM_DBSIZE_EXTEND_20050706
			// !!!. 중요> MAX_PREVIEWCHARSET (클라이언트) 확장 시 한번 더 수정 필요
			else 
			{
				// lpCL->DbVersion > 0x02
				// 인벤토리 4바이트 마다 아이템 있음
				// TempInventory -> 아이템 인덱스 생성

#ifdef ITEM_INDEX_EXTEND_20050706
				// 오른손 무기
				if( lpCL->dbInventory[0] == 255 && ((lpCL->dbInventory[2]&0x80)==0x80) && ((lpCL->dbInventory[3]&0xF0)==0xF0))
					TempInventory[0] = 0xFFFF;
				else TempInventory[0] = (lpCL->dbInventory[0]+((lpCL->dbInventory[2]&0x80)<<1)+((lpCL->dbInventory[3]&0xF0)<<5));
				// 완손 무기
				if( lpCL->dbInventory[4] == 255 && ((lpCL->dbInventory[6]&0x80)==0x80) && ((lpCL->dbInventory[7]&0xF0)==0xF0))
					TempInventory[1] = 0xFFFF;
				else TempInventory[1] = (lpCL->dbInventory[4]+((lpCL->dbInventory[6]&0x80)<<1)+((lpCL->dbInventory[7]&0xF0)<<5));
#else
				// 오른손 무기
				TempInventory[0] = lpCL->dbInventory[0];	
				// 완손 무기
				TempInventory[1] = lpCL->dbInventory[4];	
#endif

				// 헬멧
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[8] == 255 && ((lpCL->dbInventory[10]&0x80)==0x80) && ((lpCL->dbInventory[11]&0xF0)==0xF0))
					TempInventory[2] = ITEM_INDEX_NOT_EXIST;
				else TempInventory[2] = MAKE_ITEMNUM_512(lpCL->dbInventory[8], lpCL->dbInventory[10], lpCL->dbInventory[11]);
#else
				if( lpCL->dbInventory[8] == 255 && ((lpCL->dbInventory[10]&0x80)==0x80))
					TempInventory[2] = 0xFF;
				else TempInventory[2] = MAKE_ITEMNUM_32(lpCL->dbInventory[8], lpCL->dbInventory[10]);
#endif
				
				// 갑옷
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[12] == 255 && ((lpCL->dbInventory[14]&0x80)==0x80) && ((lpCL->dbInventory[15]&0xF0)==0xF0))
					TempInventory[3] = ITEM_INDEX_NOT_EXIST;
				else TempInventory[3] = MAKE_ITEMNUM_512(lpCL->dbInventory[12], lpCL->dbInventory[14], lpCL->dbInventory[15]);
#else
				if( lpCL->dbInventory[12] == 255 && ((lpCL->dbInventory[14]&0x80)==0x80))
					TempInventory[3] = 0xFF;
				else TempInventory[3] = MAKE_ITEMNUM_32(lpCL->dbInventory[12], lpCL->dbInventory[14]);
#endif
				
				// 바지
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[16] == 255 && ((lpCL->dbInventory[18]&0x80)==0x80) && ((lpCL->dbInventory[19]&0xF0)==0xF0)) 
				{
					TempInventory[4] = ITEM_INDEX_NOT_EXIST;
				}
				else TempInventory[4] = MAKE_ITEMNUM_512(lpCL->dbInventory[16], lpCL->dbInventory[18], lpCL->dbInventory[19]);
#else
				if( lpCL->dbInventory[16] == 255 && ((lpCL->dbInventory[18]&0x80)==0x80))
					TempInventory[4] = 31;
				else TempInventory[4] = MAKE_ITEMNUM_32(lpCL->dbInventory[16], lpCL->dbInventory[18]);
#endif
				
				// 장갑
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[20] == 255 && ((lpCL->dbInventory[22]&0x80)==0x80) && ((lpCL->dbInventory[23]&0xF0)==0xF0)) {
					TempInventory[5] = ITEM_INDEX_NOT_EXIST;
				}
				else TempInventory[5] = MAKE_ITEMNUM_512(lpCL->dbInventory[20], lpCL->dbInventory[22], lpCL->dbInventory[23]);
#else
				if( lpCL->dbInventory[20] == 255 && ((lpCL->dbInventory[22]&0x80)==0x80))
					TempInventory[5] = 31;
				else TempInventory[5] = MAKE_ITEMNUM_32(lpCL->dbInventory[20], lpCL->dbInventory[22]);
#endif

				// 부츠
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[24] == 255 && ((lpCL->dbInventory[26]&0x80)==0x80) && ((lpCL->dbInventory[27]&0xF0)==0xF0)) 
				{
					TempInventory[6] = ITEM_INDEX_NOT_EXIST;
				}
				else TempInventory[6] = MAKE_ITEMNUM_512(lpCL->dbInventory[24], lpCL->dbInventory[26], lpCL->dbInventory[27]);
#else
				if( lpCL->dbInventory[24] == 255 && ((lpCL->dbInventory[26]&0x80)==0x80))
					TempInventory[6] = 31;
				else TempInventory[6] = MAKE_ITEMNUM_32(lpCL->dbInventory[24], lpCL->dbInventory[26]);
#endif

				// 날개
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[28] == 255 && ((lpCL->dbInventory[30]&0x80)==0x80) && ((lpCL->dbInventory[31]&0xF0)==0xF0)) 
				{
					TempInventory[7] = ITEM_INDEX_NOT_EXIST;
				}
				else TempInventory[7] = MAKE_ITEMNUM_512(lpCL->dbInventory[28], lpCL->dbInventory[30], lpCL->dbInventory[31]);
#else
				if( lpCL->dbInventory[28] == 255 && ((lpCL->dbInventory[30]&0x80)==0x80))
					TempInventory[7] = 31;
				else TempInventory[7] = MAKE_ITEMNUM_32(lpCL->dbInventory[28], lpCL->dbInventory[30]);
#endif

				// 헬퍼 (수호/사탄)
#ifdef ITEM_INDEX_EXTEND_20050706
				if( lpCL->dbInventory[32] == 255 && ((lpCL->dbInventory[34]&0x80)==0x80) && ((lpCL->dbInventory[35]&0xF0)==0xF0)) 
				{
					TempInventory[8] = ITEM_INDEX_NOT_EXIST;
				}
				else TempInventory[8] = MAKE_ITEMNUM_512(lpCL->dbInventory[32], lpCL->dbInventory[34], lpCL->dbInventory[35]);
#else
				if( lpCL->dbInventory[32] == 255 && ((lpCL->dbInventory[34]&0x80)==0x80))
					TempInventory[8] = 31;
				else TempInventory[8] = MAKE_ITEMNUM_32(lpCL->dbInventory[32], lpCL->dbInventory[34]);
#endif

				// *> . 목걸이, 반지 (우/좌) 등은 않보이므로 필요없음
												
#ifdef ITEM_INDEX_EXTEND_20050706	// 기존의 기본 인덱스 + 확장 인덱스 세팅 부분 (아래 확장 인덱스 세팅 제거)
				// !!! . 인덱스작업 
				// 이 방식을 적용하면 없는 아이템들의 모든 비트는 알아서 전부 1로 셋된다.

				// !!! . 중요> 무기류는 코드가 4096을 넘을 수 없다는 가정하에 최상위 1비트를 계산하지 않는다.

				// 오른손 무기
				// 상위 4비트 (4096,2048,1024,512) 저장
				pCList.CharSet[12] |= (BYTE)((TempInventory[0]&0x0F00)>>4);
				// 하위 4비트 저장
				pCList.CharSet[1] = TempInventory[0]%256;

				// 왼손 무기
				// 상위 4비트 (4096,2048,1024,512) 저장
				pCList.CharSet[13] |= (BYTE)((TempInventory[1]&0x0F00)>>4);
				// 하위 4비트 저장
				pCList.CharSet[2] = TempInventory[1]%256;

				// 헬멧
				// 상위 4비트 (256,128,64,32) 저장
				pCList.CharSet[13] |= (BYTE)((TempInventory[2]&0x01E0)>>5);
				// 상위 5번째 비트 (32) 저장
				pCList.CharSet[9] |= (BYTE)((TempInventory[2]&0x0010)<<3);
				// 하위 4비트 저장
				pCList.CharSet[3] |= (BYTE)((TempInventory[2]&0x000F)<<4);

				// 갑옷
				// 상위 4비트 (256,128,64,32) 저장
				pCList.CharSet[14] |= (BYTE)((TempInventory[3]&0x01E0)>>1);
				// 상위 5번째 비트 (32) 저장
				pCList.CharSet[9] |= (BYTE)((TempInventory[3]&0x0010)<<2);
				// 하위 4비트 저장
				pCList.CharSet[3] |= (BYTE)((TempInventory[3]&0x000F));

				// 바지
				// 상위 4비트 (256,128,64,32) 저장
				pCList.CharSet[14] |= (BYTE)((TempInventory[4]&0x01E0)>>5);
				// 상위 5번째 비트 (32) 저장
				pCList.CharSet[9] |= (BYTE)((TempInventory[4]&0x0010)<<1);
				// 하위 4비트 저장
				pCList.CharSet[4] |= (BYTE)((TempInventory[4]&0x000F)<<4);

				// 장갑
				// 상위 4비트 (256,128,64,32) 저장
				pCList.CharSet[15] |= (BYTE)((TempInventory[5]&0x01E0)>>1);
				// 상위 5번째 비트 (32) 저장
				pCList.CharSet[9] |= (BYTE)((TempInventory[5]&0x0010));
				// 하위 4비트 저장
				pCList.CharSet[4] |= (BYTE)((TempInventory[5]&0x000F));

				// 신발
				// 상위 4비트 (256,128,64,32) 저장
				pCList.CharSet[15] |= (BYTE)((TempInventory[6]&0x01E0)>>5);
				// 상위 5번째 비트 (32) 저장
				pCList.CharSet[9] |= (BYTE)((TempInventory[6]&0x0010)>>1);
				// 하위 4비트 저장
				pCList.CharSet[5] |= (BYTE)((TempInventory[6]&0x000F)<<4);

				index = 0;	// 여기서는 사용 않함
#else
				// 무기 인덱스는 그냥 1바이트씩 저장 (기존에 무기번호는 256 을 넘을 수 없었음)
				pCList.CharSet[1] = (BYTE) TempInventory[0];
				pCList.CharSet[2] = (BYTE) TempInventory[1];
				
				// 장구류 (헬멧, 갑옷, 바지, 장갑, 부츠) 는 Item Type 은 약속되어 있으므로 Item Index 만 저장 4바이트 씩 저장
				pCList.CharSet[3] = ((TempInventory[2]%16)<<4)+(TempInventory[3]%16);
				pCList.CharSet[4] = ((TempInventory[4]%16)<<4)+(TempInventory[5]%16);
				index             = (TempInventory[6]%16)<<4;
#endif
				
				// 날개와 헬퍼는 2비트씩 -> 부츠를 저장했던 하위 4비트에 2비트씩 사용

				// 날개
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[7] == ITEM_INDEX_NOT_EXIST )
#else
				if( TempInventory[7] == 31 )
#endif
					index |= 0x0C;
				else index |=(TempInventory[7]&0x03)<<2;
				
#ifdef DARKLORD_WORK
				// 헬퍼
				if( !(TempInventory[8]&0x04) )
				{	// 다크호스가 아니면
#ifdef ITEM_INDEX_EXTEND_20050706
					if( TempInventory[8] == ITEM_INDEX_NOT_EXIST  )
#else
					if( TempInventory[8] == 31  )
#endif
					{
						index |= 0x03;
					}	
					else
					{
						if( TempInventory[8] != 67 )//20080930 크리스마스 이벤트 루돌푸 펫(grooving)
						{
							index |=(TempInventory[8]&0x03);
						}
					}
				}
				else
				{					
					index |=  0x03;		// 다크호스를 장착하면 수호천사가 안보이게 하기 위해										
				}
#else
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[8] == ITEM_INDEX_NOT_EXIST  )
#else
				if( TempInventory[8] == 31  )
#endif					
					index |= 0x03;
				else index |=(TempInventory[8]&0x03);
#endif // DARKLORD_WORK
				
#ifdef ITEM_INDEX_EXTEND_20050706
				pCList.CharSet[5] |= index;
#else
				pCList.CharSet[5] = index;
#endif
				
				int levelindex=0;
								
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[0] != 0xFFFF )
					levelindex  = ((int)LevelSmallConvert((lpCL->dbInventory[1]>>3)&0xF));
				if( TempInventory[1] != 0xFFFF )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[5]>>3)&0xF))<<3);
				if( TempInventory[2] < ITEM_INDEX_NOT_EXIST )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[9]>>3)&0xF))<<6);
				if( TempInventory[3] < ITEM_INDEX_NOT_EXIST )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[13]>>3)&0xF))<<9);
				if( TempInventory[4] < ITEM_INDEX_NOT_EXIST )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[17]>>3)&0xF))<<12);
				if( TempInventory[5] < ITEM_INDEX_NOT_EXIST )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[21]>>3)&0xF))<<15);
				if( TempInventory[6] < ITEM_INDEX_NOT_EXIST )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[25]>>3)&0xF))<<18);
#else
				if( TempInventory[0] != 0xFF )
					levelindex  = ((int)LevelSmallConvert((lpCL->dbInventory[1]>>3)&0xF));
				if( TempInventory[1] != 0xFF )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[5]>>3)&0xF))<<3);
				if( TempInventory[2] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[9]>>3)&0xF))<<6);
				if( TempInventory[3] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[13]>>3)&0xF))<<9);
				if( TempInventory[4] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[17]>>3)&0xF))<<12);
				if( TempInventory[5] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[21]>>3)&0xF))<<15);
				if( TempInventory[6] < 31 )
					levelindex |= ((int)LevelSmallConvert(((lpCL->dbInventory[25]>>3)&0xF))<<18);
#endif
				
				//levelindex = 0;
				
				pCList.CharSet[6] = (BYTE)((levelindex>>16)&255);
				pCList.CharSet[7] = (BYTE)((levelindex>> 8)&255);
				pCList.CharSet[8] = (BYTE)((levelindex    )&255);


#ifndef ITEM_INDEX_EXTEND_20050706		// 아이템 확장비트를 배분하는 부분 제거
				if( TempInventory[2] > 15 ) pCList.CharSet[9]  = 0x80;
				if( TempInventory[3] > 15 ) pCList.CharSet[9] |= 0x40;
				if( TempInventory[4] > 15 ) pCList.CharSet[9] |= 0x20;
				if( TempInventory[5] > 15 ) pCList.CharSet[9] |= 0x10;
				if( TempInventory[6] > 15 ) pCList.CharSet[9] |= 0x08;
#endif

#ifdef NEW_FORSKYLAND2
				// 날개....
	#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// CharList에 신규캐릭터 재앙의 날개 추가
	if( (TempInventory[7] >= 3 && TempInventory[7] <= 6) || 
					TempInventory[7] == 30 
					|| TempInventory[7] == 41 || TempInventory[7] == 42 
					)
	#else	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	#ifdef DARKLORD_WORK
				if( (TempInventory[7] >= 3 && TempInventory[7] <= 6) || 
					TempInventory[7] == 30 )
	#else
				if( TempInventory[7] >= 3 && TempInventory[7] <= 6)
	#endif	// #ifdef DARKLORD_WORK
	#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
				{
					pCList.CharSet[5] |= 0x0C;

	#ifdef DARKLORD_WORK
					if( TempInventory[7] == 30 )
						pCList.CharSet[9] |= 0x05;		// 망토 설정(0x07까지 마스킹이 되므로 앞으로 두개 더 쓸수 있음)
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// CharList에 신규캐릭터 재앙의 날개 추가
					else if( TempInventory[7] == 41 )
						pCList.CharSet[9] |= 0x06;
					else if( TempInventory[7] == 42 )
						pCList.CharSet[9] |= 0x07;
#endif
					else
	#endif	// #ifdef DARKLORD_WORK
						pCList.CharSet[9] |= ((TempInventory[7]-2)&0x7);
				}
#else
	#ifdef NEW_SKILL_FORSKYLAND
				// 페가시아용...
				if( TempInventory[8] & 0x03 && TempInventory[8] != 31 )
				{
					pCList.CharSet[9] |= (BYTE)0x04;
				}
	#endif
#endif	

				pCList.CharSet[10] = 0;

#ifdef NEW_FORSKYLAND2
				// 페가시아용...
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[8] & 0x03 && TempInventory[8] != ITEM_INDEX_NOT_EXIST  )
#else
				if( TempInventory[8] & 0x03 && TempInventory[8] != 31 )
#endif
				{
					pCList.CharSet[10] |= (BYTE)0x01;
				}
#endif

				pCList.CharSet[11]  = 0;

#ifdef DARKLORD_WORK	
#ifdef ITEM_INDEX_EXTEND_20050706
				if( TempInventory[8] & 0x04 && TempInventory[8] != ITEM_INDEX_NOT_EXIST )
#else
				if( TempInventory[8] & 0x04 && TempInventory[8] != 31 )
#endif
				{	// 다크호스
					pCList.CharSet[12] |= (BYTE)0x01;
				}
#endif

#ifdef ITEM_INDEX_EXTEND_20050706
				// 예비 바이트 셋
				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
				// HELPER가 펜릴인 경우 - 펜릴 번호 : (13, 37)
				if( TempInventory[8] == 37 && TempInventory[8] != ITEM_INDEX_NOT_EXIST )
				{	
					pCList.CharSet[10] &= ~ (BYTE)0x01;	// 디노란트 아님 
					pCList.CharSet[12] &= ~ (BYTE)0x01;	// 다크호스 아님
					pCList.CharSet[12] |=	(BYTE)0x04; // 펜릴

					// 액설런트 옵션 검사
					
					// ( DB -> GS ) 캐릭리스트 4BYTE 정보 
					//  [0] 아이템 인덱스 
					//	[1] 아이템 레벨, 옵션
					//	[2] 아이템 인덱스 9번째 BIT, 액설런트&옵션 
					//	[3] 아이템 인덱스 10~13번째 BIT

					// ( GS -> CL ) 예비 바이트 
					// CharSet[16], CharSet[17] 중
					// CharSet[16]을 사용한다.

					BYTE btExcellentOption = lpCL->dbInventory[34] & 0x3F;
					
					if( (btExcellentOption & PLUS_FENRIR_INCREASE_LAST_DAMAGE) == PLUS_FENRIR_INCREASE_LAST_DAMAGE )
					{
						pCList.CharSet[16] |= (BYTE)0x01;	// 펜릴 - 최종 데미지 증가
					}
					
					if( (btExcellentOption & PLUS_FENRIR_DECREASE_LAST_DAMAGE) == PLUS_FENRIR_DECREASE_LAST_DAMAGE )
					{
						pCList.CharSet[16] |= (BYTE)0x02;	// 펜릴 - 최종 데미지 증가
					}

#ifdef ADD_ILLUSION_FENRIR_20070703	// 환영 펜릴 옵션에 따른 스킬 추가(CharSet[17]중 2번째 Bit)
					if( (btExcellentOption & PLUS_ILLUSION_FENRIR_OPTION) == PLUS_ILLUSION_FENRIR_OPTION )
					{
						pCList.CharSet[17] |= (BYTE)0x01;	// 펜릴 - 최종 데미지 증가
					}
#endif
				}
#endif // ADD_ITEM_FENRIR_01_20051110
				
#ifdef ADD_THIRD_WING_20070525	// 캐릭터 정보 보낼 때 3차날개 추가	( DB 0x03 )
	#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// 차원의 날개 캐릭터 정보 추가
				if( ( TempInventory[7] >= 36 && TempInventory[7] <= 40 )
					|| TempInventory[7] == 43)	
	#else
				if( TempInventory[7] >= 36 && TempInventory[7] <= 40 )
	#endif
				{
					pCList.CharSet[5] |= 0x0C;
					pCList.CharSet[16] |= ( (( TempInventory[7] - 35 )&0x7 ) << 2 );
		#ifdef MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109	// 차원의 날개는 0x06으로 보냄
					if(TempInventory[7] == 43)
					{
						pCList.CharSet[16] |= ( (( 0x06 )&0x7 ) << 2 );
					}
		#endif	// MODIFY_SUMMONER_THIRD_WING_BUFGIX_20090109
				}
#endif	// ADD_THIRD_WING_20070525

#ifdef ADD_SAFEGUARD_ITEM_20080702
				// 일본 데몬,추호천사 추가
				// pCList.CharSet[16] : 1110 0000 앞에 3자리 사용함.
				if( !(TempInventory[8]&0x04) && TempInventory[8] != ITEM_INDEX_NOT_EXIST )
				{
					BYTE btItemIndex = 0;
					switch(TempInventory[8])
					{
					case 64://데몬
						{
							btItemIndex = (BYTE)(0x01 << 5);
							break;
						}
					case 65://수호정령
						{
							btItemIndex = (BYTE)(0x02 << 5);
							break;
						}
					case 67://20080930 크리스마스 이벤트 루돌푸 펫(grooving)
						{
							btItemIndex = (BYTE)(0x04 << 5);
							break;
						}
#ifdef ADD_UNICORN_PET_20090915		// 유니콘 펫
					case 106:
						{
							btItemIndex = (BYTE)(0x05 << 5);
							break;
						}
#endif // ADD_UNICORN_PET_20090915
#ifdef ADD_PREMIUMITEM_PANDA
					case 80://팬더펫
						{
							btItemIndex = (BYTE)(0x07 << 5);
							break;
						}
#endif // ADD_PREMIUMITEM_PANDA
					}
					
					pCList.CharSet[16] |= btItemIndex;
				}
#endif	//ADD_SAFEGUARD_ITEM_20080702
			}
#endif	// ITEM_DBSIZE_EXTEND_20050706
		
			memcpy(sendbuf+lOfs, (char*)&pCList, sizeof(PMSG_CHARLIST));
			lsOfs += sizeof(SDHP_CHARLIST);
			lOfs  += sizeof(PMSG_CHARLIST);
		}
	}
	pCLCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCLCount, sizeof(PMSG_CHARLISTCOUNT));
	DataSend(aIndex, sendbuf, lOfs);

	//캐릭터 리스트 받기(클라에서 캐릭터 선택 화면)
#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	LogAddTD_TempLog(", [D->G Recv GetCharList], Account [%s], IP [%s]", gObj[aIndex].AccountID, gObj[aIndex].Ip_addr);
#endif
	
	if( gCreateCharacter == 0 )
	{
		//if( lpCount->Count == 0 ) 
		//{
		//	GCServerMsgStringSend("이벤트 서버엔 등록된 사용자만 접속 가능합니다.", aIndex, 0);
		//	gObjCloseSet(aIndex, CL_SERVERLIST);
		//}	
	}
	return;

}


//----------------------------------------------------------------------------
// [0xF3][0x00] 유저가 캐릭터 리스트를 달라고 요청하면
// [0x02] 데이터 서버에 계정으로 캐릭터 리스트를 요청한다.
//----------------------------------------------------------------------------
void DataServerGetCharListRequest(short aIndex)
{
	SDHP_GETCHARLIST	pMsg;
	char TempId[MAX_IDSTRING+1];
	char *AccountId;
	
	memset(TempId, 0, MAX_IDSTRING+1);
	
	AccountId = gObjGetAccountId(aIndex);
	if( AccountId == NULL )
	{
		LogAdd( lMsg.Get(422), __FILE__, __LINE__);
		CloseClient ( aIndex );
		return;
	}
	if( strlen(AccountId) < 1 ) 
	{
		LogAdd(lMsg.Get(423), __FILE__, __LINE__);
		CloseClient ( aIndex );
		return;
	}
	
#ifdef UPDATE_RESEND_GETCHARlIST_DISABLE_20090605
	if( gObj[aIndex].m_bRequestPaket == TRUE )
	{
		LogAdd("ReSend CharacterList (%s)", AccountId);
		gObj[aIndex].m_nRequestPacketCnt++;
		if(gObj[aIndex].m_nRequestPacketCnt > 3)
		{
			// 인증서버에 아이피를 등록
			//GJRequestBlockIp(gObj[aIndex].Ip_addr);
			
			// 사용자 종료
			LogAdd("ReSend CharacterList CloseUser (%s)", AccountId);
			CloseClient ( aIndex );
			return;
		}
	}
	
	gObj[aIndex].m_bRequestPaket = TRUE;
#endif	// UPDATE_RESEND_GETCHARlIST_DISABLE_20090605

	strcpy(TempId, AccountId);

	pMsg.h.c        = PMHC_BYTE;
	pMsg.h.size     = sizeof( pMsg );
	pMsg.h.headcode = 0x01;
	pMsg.Number     = aIndex;
	memcpy(pMsg.Id, TempId, MAX_IDSTRING);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	//창고 정보 요청
	LogAddTD_TempLog(", [G->D Req GetCharList], Account [%s], IP [%s] ", AccountId, gObj[aIndex].Ip_addr);
#endif

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x04] 캐릭터 생성 결과를 받는다.
//----------------------------------------------------------------------------
void JGCharacterCreateRequest(LPSDHP_CREATECHARRESULT lpMsg)
{	
	PMSG_CHARCREATERESULT	pResult;
	
	char szAccountId[MAX_IDSTRING+1];
	
	int	 aIndex = lpMsg->Number;
	
	szAccountId[MAX_IDSTRING] = '\0';
	
	memcpy(szAccountId, lpMsg->AccountId, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(424),szAccountId);
		CloseClient ( aIndex );
		return;
	}

	pResult.h.c        = PMHC_BYTE;
	pResult.h.size     = sizeof( pResult );
	pResult.h.headcode = 0xF3;
	pResult.subcode    = 0x01;
	pResult.Result     = lpMsg->Result;
	pResult.pos        = lpMsg->Pos;

#ifdef WORLD_TOURNAMENT_EVENT_SETTING
	if( lpMsg->ClassSkin == 0 || lpMsg->ClassSkin == 16 || lpMsg->ClassSkin == 32 )
	{
		lpMsg->ClassSkin++;
	}

	pResult.Class = ((lpMsg->ClassSkin>>4)<<5)&0xE0; 
	int changeup = (lpMsg->ClassSkin)&CHANGEUP_MASK;
	pResult.Class |= (changeup<<4)&0x10;
	
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 캐릭생성결과를 받았다.	
	pResult.Class |= ( changeup << 4 )&0x20;
#endif

	//pResult.Class = lpMsg->ClassSkin;
#else
	#ifdef ADD_CLASSINFO_ON_CHARCTER_CREATION_20041224
//		if( lpMsg->ClassSkin == 0 || lpMsg->ClassSkin == 16 || lpMsg->ClassSkin == 32 )
//		{
//			lpMsg->ClassSkin++;
//		}
		
		pResult.Class = ((lpMsg->ClassSkin>>4)<<5)&0xE0; 
		int changeup = (lpMsg->ClassSkin)&CHANGEUP_MASK;
		pResult.Class |= (changeup<<4)&0x10;	
		
#ifdef THIRD_CHANGEUP_SYSTEM_20070507	// 캐릭생성결과를 받았다.
		pResult.Class |= ( changeup << 4 )&0x20;
#endif

	#endif
#endif
	



#ifdef DARKLORD_WORK
	pResult.Level        = lpMsg->Level;
#endif
	memcpy(pResult.Name, lpMsg->Name, MAX_IDSTRING);
	
	if( pResult.Result == 0x01 ) // 성공이면 장비 착용
	{
		memcpy(pResult.Equipment, lpMsg->Equipment, 24);
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void JGCharacterCreateFailSend(int aIndex, char *id)
{	
	PMSG_CHARCREATERESULT	pResult;
	
	pResult.h.c        = PMHC_BYTE;
	pResult.h.size     = sizeof( pResult );
	pResult.h.headcode = 0xF3;
	pResult.subcode    = 0x01;
	pResult.Result     = 0x00;
	pResult.pos        = 0x00;
	memcpy(pResult.Name, id, MAX_IDSTRING);

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// [0x05] 캐릭터 삭제 결과를 받는다.
//----------------------------------------------------------------------------
void JGCharDelRequest(LPSDHP_CHARDELETERESULT lpMsg)
{
	PMSG_RESULT pResult;

	char szAccountId[MAX_IDSTRING+1];
	int	 aIndex = lpMsg->Number;	
	
	szAccountId[MAX_IDSTRING] = '\0';
	
	memcpy(szAccountId, lpMsg->AccountID, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(425), szAccountId);
		CloseClient ( aIndex );
		return;
	}

	pResult.h.c        = PMHC_BYTE;
	pResult.h.size     = sizeof( pResult );
	pResult.h.headcode = 0xF3;
	pResult.subcode    = 0x02;
	pResult.result	   = lpMsg->Result;

	DataSend(lpMsg->Number, (LPBYTE)&pResult, pResult.h.size);
}

//----------------------------------------------------------------------------
// [0x06] 한 캐릭터의 모든 정보를 얻는다.
//----------------------------------------------------------------------------
void JGGetCharacterInfo(LPSDHP_DBCHAR_INFORESULT lpMsg)
{
	PMSG_CHARMAPJOINRESULT	pjMsg;
		
	char szAccountId[MAX_IDSTRING+1];
	char szName[MAX_IDSTRING+1];

	int	 aIndex = lpMsg->Number;	
	
	szAccountId[MAX_IDSTRING] = '\0';
	memcpy(szAccountId, lpMsg->AccountID, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(426), szAccountId);
		CloseClient ( aIndex );
		return;
	}
	szName[MAX_IDSTRING] = '\0';
	memcpy(szName, lpMsg->Name, MAX_IDSTRING);
	if( lpMsg->result == 0x00 )
	{
		LogAddC(LOGC_RED, lMsg.Get(427), szName, gObj[aIndex].AccountID);
		CloseClient ( aIndex );
		return;
	}

	if( lpMsg->CtlCode & 0x01 )
	{	// 캐릭터 블럭일때는 강제로 접속을 종료 시킨다
		LogAddC(LOGC_RED, "error-L1 : Load Block Character infomation", szName, gObj[aIndex].AccountID);
		CloseClient ( aIndex );
		return;
	}

// 본서버만 적용 된다
#if TESTSERVER == 0
#ifdef MODIFY_CHARACTER_DELETE_IN_MAP_20051121
	// 동일한 이름의 캐릭터가 이미 존재하면 접속 할수 없다.
	for( int i=ALLOC_USEROBJECTSTART ; i<MAX_OBJECT; i++)
	{
		if( (gObj[i].Connected == CSS_GAMEPLAYING) && (gObj[i].Type == OBJTYPE_CHARACTER))
		{
			if( strncmp(szName, gObj[i].Name, MAX_IDSTRING) == 0 || strncmp(szAccountId, gObj[i].AccountID, MAX_IDSTRING) == 0 )
			{
				LogAddTD("[Anti-HACK][JGGetCharacterInfo] Attempted Character-Copy by double logging [%s][%s]", 
					szName, gObj[aIndex].AccountID);
				
				CloseClient ( aIndex );
				return;
			}
		}
	}

#endif // MODIFY_CHARACTER_DELETE_IN_MAP_20051121
#endif // TESTSERVER
	
	// 세팅한다.
	if( gObjSetCharacter((LPBYTE)lpMsg, aIndex) == FALSE) 
	{
		LogAddC(LOGC_RED, lMsg.Get(428), szName);
		CloseClient ( aIndex );
		return;
	}
	
#ifdef ADD_SHIELD_POINT_01_20060403
	gObj[aIndex].iShield = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield);
#endif

	
#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[0] != AUTH_CHECK_00 )
		DestroyGIocp();
#endif

	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];
	if( gObj[aIndex].m_Index != aIndex )
	{
		LogAddC(LOGC_RED, lMsg.Get(429), __FILE__, __LINE__);
		CloseClient ( aIndex );
		return;
	}

#ifdef MODIFY_ZEN_MAX_20040414
	// Zen이 0보다 작으면 0으로 세팅해준다.
	if( lpObj->Money < 0 )
	{
		LogAddC(LOGC_RED, "★☆ ZEN MODIFY ☆★ [%s][%s] %d -> 0", 
			szName, gObj[aIndex].AccountID, lpObj->Money);
		lpObj->Money = 0;
	}
#endif

#ifdef MAP_SERVER_WORK_20041030		// 맵서버로 이동여부 체크 -> 최초 로그인 하여 케릭터 정보를 받을 때 이 맵이 아니라면 다른 맵서버로 이동시켜 준다.
	if (lpObj->m_bMapSvrMoveReq == false) {
		// 최초 로그인인 경우에만 여기서 처리한다. -> 다른 맵서버에서 온 경우는 gObjSetCharacter() 에서 처리
		if( !MapNumberCheck(lpObj->MapNumber) ) 
		{
			LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
			CloseClient ( aIndex );
			return;
		}

		SHORT sSvrCode	= g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->MapNumber, lpObj->m_sPrevMapSvrCode);
		if (sSvrCode != gGameServerCode) {
			if (sSvrCode == -1) {
				// 치명적인 에러 -> 거의 없슴
				LogAddC(LOGC_RED, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
					lpObj->AccountID,
					lpObj->Name,
					lpObj->m_Index
					);
				CloseClient ( aIndex );
				return;
			}

			// 인증서버에 맵서버 이동 요청을 한다.
			GJReqMapSvrMove (lpObj->m_Index, sSvrCode, lpObj->MapNumber, lpObj->X, lpObj->Y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
				sSvrCode,
				lpObj->AccountID,
				lpObj->Name,
				lpObj->m_Index
				);
			return;
		}
	}
	
	lpObj->m_bMapSvrMoveReq	= false;
#endif

	pjMsg.h.c			= PMHCE_BYTE;
	pjMsg.h.headcode	= 0xF3;
	pjMsg.h.size		= sizeof(pjMsg);
	pjMsg.subcode		= 0x03;
	pjMsg.MapX			= (BYTE)lpObj->X;
	pjMsg.MapY			= (BYTE)lpObj->Y;
	pjMsg.MapNumber		= lpObj->MapNumber;
	pjMsg.Dir			= lpObj->Dir;
#ifdef	MASTER_LEVEL_UP_SYSTEM_20070912	// 경험치 전송 -> EXP 타입 변경	
	// *마스터레벨 정보를 받기 전이라 의미는 없음. 나중을 위해서 일단 작업.
	INT64 i64Exp = lpObj->Experience;
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		i64Exp = lpObj->m_i64MasterLevelExp;
	}

	// 마스터레벨 유저를 위해 타입 변경						
	pjMsg.btExp1			= HIBYTE(HIWORD(HILONG( i64Exp )));
	pjMsg.btExp2			= LOBYTE(HIWORD(HILONG( i64Exp )));
	pjMsg.btExp3			= HIBYTE(LOWORD(HILONG( i64Exp )));
	pjMsg.btExp4			= LOBYTE(LOWORD(HILONG( i64Exp )));
	pjMsg.btExp5			= HIBYTE(HIWORD(LOLONG( i64Exp )));
	pjMsg.btExp6			= LOBYTE(HIWORD(LOLONG( i64Exp )));
	pjMsg.btExp7			= HIBYTE(LOWORD(LOLONG( i64Exp )));
	pjMsg.btExp8			= LOBYTE(LOWORD(LOLONG( i64Exp )));	

	INT64 i64NextExp = lpObj->NextExp;
	if( g_MasterLevelSystem.IsMasterLevelUser( lpObj ) )
	{
		i64NextExp = lpObj->m_i64NextMasterLevelExp;
	}	
	pjMsg.btNextExp1		= HIBYTE(HIWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp2		= LOBYTE(HIWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp3		= HIBYTE(LOWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp4		= LOBYTE(LOWORD(HILONG( i64NextExp )));
	pjMsg.btNextExp5		= HIBYTE(HIWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp6		= LOBYTE(HIWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp7		= HIBYTE(LOWORD(LOLONG( i64NextExp )));
	pjMsg.btNextExp8		= LOBYTE(LOWORD(LOLONG( i64NextExp )));	
#else
	pjMsg.Exp			= lpObj->Experience;
	pjMsg.NextExp		= lpObj->NextExp;
#endif	
	pjMsg.LevelUpPoint	= lpObj->LevelUpPoint;
	pjMsg.Str			= lpObj->Strength;
	pjMsg.Dex			= lpObj->Dexterity;
	pjMsg.Vit			= lpObj->Vitality;	
	pjMsg.Energy		= lpObj->Energy;
	pjMsg.Money			= lpObj->Money;
	pjMsg.PkLevel		= lpObj->m_PK_Level;
	pjMsg.Life			= (WORD)(lpObj->Life);
	pjMsg.MaxLife		= (WORD)((lpObj->MaxLife+lpObj->AddLife));
	pjMsg.Mana			= (WORD)(lpObj->Mana);
	pjMsg.MaxMana		= (WORD)((lpObj->MaxMana+lpObj->AddMana));
#ifdef ADD_SHIELD_POINT_01_20060403
	pjMsg.wShield		= (WORD)(lpObj->iShield);
	pjMsg.wMaxShield	= (WORD)(lpObj->iMaxShield + lpObj->iAddShield);
#endif
	pjMsg.CtlCode		= lpMsg->CtlCode;

#ifdef NEW_FORSKYLAND2
	pjMsg.BP			= lpObj->BP;
	pjMsg.MaxBP			= lpObj->MaxBP+lpObj->AddBP;
#endif

#ifdef DARKLORD_WORK
	pjMsg.Leadership = lpObj->Leadership;
#endif

	short AddPoint		= 0;
	short MaxAddPoint	= 0;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	short MinusPoint	= 0;
	short MaxMinusPoint = 0;
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
#else
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint);
#endif

	pjMsg.AddPoint		= AddPoint;
	pjMsg.MaxAddPoint	= MaxAddPoint;

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	pjMsg.wMinusPoint	= MinusPoint;
	pjMsg.wMaxMinusPoint= MaxMinusPoint;

	LogAddTD("[FRUIT System] [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", 
		lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
#endif

#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	if( AddPoint<0 || AddPoint>MaxAddPoint  || MinusPoint<0 || MinusPoint>MaxMinusPoint )
	{
		LogAddTD("[FRUIT System] Character Stat Error [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)", 
			lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
	}
#else
	if( AddPoint < 0 || AddPoint > MaxAddPoint)
	{
		LogAddTD(lMsg.Get(500), lpObj->AccountID, lpObj->Name, AddPoint, MaxAddPoint);
	}
#endif

#ifdef EXTEND_LOG_SYSTEM_20060202
	g_QuestInfo.QuestInfoSave(lpObj);
#endif // EXTEND_LOG_SYSTEM_20060202

	DataSend(aIndex, (LPBYTE)&pjMsg, pjMsg.h.size);		// 여기까지 하면 사람의 모습만 보인다.
	GCItemListSend(aIndex);								// 여기까지 하면 사람이 장비한 것이 보인다.
	GCMagicListMultiSend( lpObj );						// 여기까지 하면 사람의 마법리스트를 한방에 보낸다.

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912					// DS에 마스터레벨 정보 요청
	g_MasterLevelSystem.GDReqMasterLevelInfo( lpObj );
#endif

#ifdef ADD_SKILL_WITH_COMBO
	CGRequestQuestInfo(aIndex);
#endif

#ifdef MODIFY_QUEST_MONSTER_KILL_COUNT_BUGFIX_20070704	// DS에 퀘스트 몬스터 킬카운트 정보 요청
	// 정보 셋팅이 완료되면 몬스터 킬카운트 정보를 요청한다.
	g_QuestUtil.GDReqQuestMonsterKillInfo( lpObj );
#endif

#ifdef _NEW_EXDB_
	//##EXDB##
	DGGuildMemberInfoRequest(aIndex);					// 해당 케릭에 대한 길드정보를 길드서버에 요청??
	//##EXDB##
#endif
	
// gObjSetCharacter에서 호출하던 것을 맵서버 적용에 따라 뒤쪽으로 이동시킴.
#ifdef MODIFY_ORDER_REQUEST_FRIENDLIST_20050205
	#ifdef __FRIEND_WORK__
		FriendListRequest(aIndex);
	#endif
#endif

#ifdef FOR_JAPAN	
	// 타마짱 이벤트가 진행중이면
	if( gTamaJJangEvent )
	{
		if( gAppearTamaJJang && lpObj->MapNumber == 0 )
		{	// 타마짱이 나와 있고, 로렌시아에 있으면
			GCMapEventStateSend(0, gAppearTamaJJang, 2);	
		}
	}
#endif

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// 게임에 입장했을 때 변신 보여주기
	gObjUseSkill.SkillChangeUse( lpObj->m_Index );
#endif
	
#ifdef ADD_HACKING_TOOL_BLOCK_20090311
	// 신규 접속자에게 서버에서 설정된 블록된 해킹툴의 사용여부를 확인한다.
	g_HackToolBlock.HacktoolStatisticsNewConnect( lpObj );
#endif // ADD_HACKING_TOOL_BLOCK_20090311

#ifdef ADD_MAPMOVE_PROTOCOL_20090327
	lpObj->m_dwMapMoveKeyValue = g_KeyGenerater.GenerateSeedValue();
	GCSendMapMoveChecksum( lpObj );
#endif // ADD_MAPMOVE_PROTOCOL_20090327

	g_ResetSystem.DBGetResetInfo(lpObj);

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	// 캐릭터 선택 후 맵
	LogAddTD_TempLog(", [D->G Recv MapJoin] CharSelected, Account [%s], IP [%s], Name[%s]", 
		gObj[aIndex].AccountID, gObj[aIndex].Ip_addr, gObj[aIndex].Name);
#endif

	PMSG_NOTICE pNotice;

	TNotice::MakeNoticeMsg((TNotice*)&pNotice, 0x00,  g_GlobalConfig.m_szConnectMessage);
	DataSend(aIndex, (unsigned char*)&pNotice , pNotice.h.size );
}

void GCItemListSend(int aIndex)
{
	PMSG_INVENTORYLISTCOUNT	pMsgILC;
	PMSG_INVENTORYLIST		pMsgIL;
	int						sOfs       = sizeof(pMsgILC);
	int						pMsgILSize = sizeof(pMsgIL);
	
#ifdef ITEM_INDEX_EXTEND_20050706
	BYTE					sendBuf[2048];
#else
	BYTE					sendBuf[1024];
#endif
	
	LPOBJECTSTRUCT	lpObj = &gObj[aIndex];

	int itemcount=0;
	
#ifdef PERSONAL_SHOP_20040113
	for( int n=0; n<MAX_INVENTORY_EXTEND; n++)
#else
	for( int n=0; n<MAX_INVENTORY; n++)
#endif
	{
		if( lpObj->pInventory[n].IsItem() == TRUE )
		{
#ifdef ITEMDELETE_MODIFY_20040604	// 존재하지 않는 아이템이라면 리스트를 보내주지 않는다.
			if (lpObj->pInventory[n].m_bItemExist == false)
				continue;
#endif
			pMsgIL.Pos = n;
			ItemByteConvert( pMsgIL.ItemInfo, lpObj->pInventory[n] );
			memcpy(sendBuf+sOfs, (char*)&pMsgIL, pMsgILSize);
			itemcount++;
			sOfs += pMsgILSize;
		}
	}
	
	pMsgILC.h.c        = 0xC4;
	pMsgILC.h.headcode = 0xF3;
	pMsgILC.subcode    = 0x10;
	pMsgILC.h.sizeH    = HIBYTE( sOfs );
	pMsgILC.h.sizeL    = LOBYTE( sOfs );
	pMsgILC.Count      = itemcount;
	memcpy( sendBuf, (char*)&pMsgILC, sizeof(pMsgILC));
	
	DataSend(aIndex, (LPBYTE)sendBuf, sOfs);
	//LogAddL("암호화 패킷 데이터 전송 : [%x][%x]", pMsgILC.h.headcode, pMsgILC.subcode);


	
}


//----------------------------------------------------------------------------
// [0x07] 인증서버에 캐릭터 정보를 저장하라고 보냄
// 주의 할 것은 유저 정보를 사용하기 때문에 쓰레드를 정지하고 사용해야 한다.
//----------------------------------------------------------------------------
#ifdef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
	void GJSetCharacterInfo(LPOBJECTSTRUCT lpObj, int aIndex, BOOL bMapServerMove)
#else
	void GJSetCharacterInfo(LPOBJECTSTRUCT lpObj, int aIndex)
#endif
{
	if(lpObj->m_bShell == true)
		return;
#ifdef MAP_SERVER_WORK_20041030				// 맵서버 이동 전에 한번 저장했으므로 두번 저장하지 않는다.
	if (lpObj->m_bMapSvrMoveQuit == true) {
		LogAddC(LOGC_RED, "[MapServerMng] GJSetCharacterInfo() - Inventory Already Saved [%s][%s] (%d)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->m_Index
			);
		return;
	}

#ifdef MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
	// 맵 서버 이동인 경우 m_bMapSvrMoveQuit 값을 TRUE로 미리 설정한다. DB에 창고/인벤 정보가 저장되기 전에 설정.
	if ( bMapServerMove == TRUE )
	{
		lpObj->m_bMapSvrMoveQuit = true;
		LogAddC(LOGC_LIGHTRED, "[MapServerMng][%s][%s] GJSetCharacterInfo() - MapServerMove Character Save Start (%d)",
			lpObj->AccountID,
			lpObj->Name,
			lpObj->m_Index
			);
	}
#endif // MODIFY_MAPSERVERMOVE_FLAG_BUGFIX_20060817
	
#endif // MAP_SERVER_WORK_20041030
	
	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type == I_WAREHOUSE && lpObj->m_IfState.state == 1 ) 
	{	// 창고가 열려 있을때는 창고도 저장한다
		GDSetWarehouseList(aIndex);	
	}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[1] != AUTH_CHECK_01 )
		DestroyGIocp();
#endif

	SDHP_DBCHAR_INFOSAVE	pCSave;
	
#if (DATANOSAVE == 1)
	return;
#endif

	pCSave.h.c			= PMHC_WORD;
	pCSave.h.headcode	= 0x07;
	pCSave.h.sizeH      = HIBYTE(sizeof( pCSave ));
	pCSave.h.sizeL      = LOBYTE(sizeof( pCSave ));
	
	pCSave.AccountID[MAX_IDSTRING] = '\0';
	memcpy(pCSave.AccountID, lpObj->AccountID, MAX_IDSTRING);

	pCSave.Name[MAX_IDSTRING] = '\0';
	memcpy(pCSave.Name,	lpObj->Name, MAX_IDSTRING);

	pCSave.Class		= (BYTE)lpObj->DbClass;
	pCSave.Level		= lpObj->Level;
	pCSave.LevelUpPoint = lpObj->LevelUpPoint;
	pCSave.Exp			= lpObj->Experience;
	pCSave.NextExp		= lpObj->NextExp;
	pCSave.Money		= lpObj->Money;
	pCSave.Str			= lpObj->Strength;
	pCSave.Dex			= lpObj->Dexterity;
	pCSave.Vit			= lpObj->Vitality;
	pCSave.Energy		= lpObj->Energy;
	pCSave.Life			= (WORD)(lpObj->Life*10.0f);
	pCSave.MaxLife		= (WORD)(lpObj->MaxLife*10.0f);
	pCSave.Mana			= (WORD)(lpObj->Mana*10.0f);
	pCSave.MaxMana		= (WORD)(lpObj->MaxMana*10.0f);

	if((lpObj->Authority&AUTHORITY_DISABLE) == AUTHORITY_DISABLE)
		pCSave.CtlCode = 1;
	else if((lpObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN)
		pCSave.CtlCode = 32;
	else if((lpObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM)
		pCSave.CtlCode = 8;
	else if((lpObj->Authority&AUTHORITY_USER) == AUTHORITY_USER)
		pCSave.CtlCode = 0;

	//ItemByteConvert7(pCSave.dbInventory, lpObj->pInventory, MAX_INVENTORY);
#ifdef ITEM_DBSIZE_EXTEND_20050706
	ItemByteConvert16(pCSave.dbInventory, lpObj->pInventory, MAX_INVENTORY_EXTEND);
#else
#ifdef PERSONAL_SHOP_20040113
	ItemByteConvert10(pCSave.dbInventory, lpObj->pInventory, MAX_INVENTORY_EXTEND);
#else
	ItemByteConvert10(pCSave.dbInventory, lpObj->pInventory, MAX_INVENTORY);
#endif	// #define PERSONAL_SHOP_20040113
#endif	// #define ITEM_DBSIZE_EXTEND_20050706

	MagicByteConvert(pCSave.dbMagicList, lpObj->Magic, MAX_MAGIC);

	pCSave.MapNumber	= lpObj->MapNumber;
	pCSave.MapX			= (BYTE)lpObj->X;
	pCSave.MapY			= (BYTE)lpObj->Y;

	pCSave.Dir			= lpObj->Dir;

#ifdef TEST_SETITEM_CLEAR
	pCSave.Dir			|= (lpObj->ClearInfoForSetItem<<4);
#endif
	pCSave.PkCount		= lpObj->m_PK_Count;
	pCSave.PkLevel		= lpObj->m_PK_Level;
	pCSave.PkTime		= lpObj->m_PK_Time;

#ifdef DARKLORD_WORK	
	pCSave.Leadership	 = lpObj->Leadership;			// 통솔
	pCSave.ChatLitmitTime = lpObj->ChatLitmitTime;		// 채팅 제한 시간
#endif
	
#ifdef ADD_MINUS_STAT_SYSTEM_USING_FRUIT_20050712
	pCSave.iFruitPoint	= lpObj->iFruitPoint;			// 추가된 열매 포인트.
#endif


#ifdef ITEMSAVE_MODIFY_FORSTATICS

#ifdef FOR_KOREA

	#ifndef REMOVE_CONNECTED_CHARACTER_NAME_INFO_20051004
		if( (GetTickCount()-lpObj->SaveTimeForStatics) > (1000*60*60) )	
		{
			lpObj->SaveTimeForStatics = GetTickCount()+(1000*60*60);
			pCSave.CharInfoSave = 1;
		}
		else
	#endif
			pCSave.CharInfoSave = 0;
#else
		pCSave.CharInfoSave = 0;
#endif


#endif

#ifdef NEW_SKILL_FORSKYLAND
	memcpy(pCSave.dbQuest, lpObj->m_Quest, MAX_DBQUEST);
#endif

	pCSave.AccountType = lpObj->m_AccountType;
	pCSave.DecreaseTime = time(NULL)-lpObj->m_JoinTime;

	if(pCSave.DecreaseTime > lpObj->m_EndTime)
		pCSave.DecreaseTime = lpObj->m_EndTime;

	if( cDBSMng.Send((char*)&pCSave, sizeof(pCSave)) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(430), lpObj->Name);
		return;
	}
	gObjItemTextSave(lpObj);

	gObjStatTextSave(lpObj);

#ifdef DARKLORD_WORK	// 펫 아이템 정보도 저장한다
	gObjSavePetItemInfo(aIndex, 0);
#endif
	
#ifdef NEW_SKILL_FORSKYLAND
	#ifndef EXTEND_LOG_SYSTEM_20060202
		g_QuestInfo.QuestInfoSave(lpObj);
	#endif // EXTEND_LOG_SYSTEM_20060202
#endif

#ifdef MODIFY_QUEST_SYSTEM_20070525	// 퀘스트 정보를 저장한다.
	g_QuestUtil.GDReqQuestMonKillInfoSave( lpObj );
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912		// 마스터레벨 정보 저장
	g_MasterLevelSystem.GDReqMasterLevelInfoSave( lpObj );
#endif
}


// [0x08] 창고의 리스트를 달라고한다.
void GDGetWarehouseList(int aIndex, char *AccountID)
{

	if( strlen(AccountID) < 1 ) return;

	SDHP_GETWAREHOUSEDB		pMsg;
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x08;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.aIndex		= (short)aIndex;

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
	//창고 요청
	LogAddTD_TempLog(", [G->D Req GetWarehouseList], Account [%s], IP [%s] ", AccountID, gObj[aIndex].Ip_addr);
#endif

	memcpy(pMsg.AccountID, AccountID, MAX_IDSTRING);
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x08] 창고 리스트를 받는다.
void DGGetWarehouseList(LPSDHP_GETWAREHOUSEDB_SAVE lpMsg)
{
	char	szId[MAX_IDSTRING+1];
	int		aIndex=0;
	PMSG_TALKRESULT		pResult;
	
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	aIndex = (int)lpMsg->aIndex;

	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), szId, aIndex);
		return;
	}
	LPOBJECTSTRUCT	lpObj;

	lpObj = &gObj[aIndex];

	int		itype, _type;
	BYTE	OptionData;
	CItem	item;
	
	memset(gObj[aIndex].pWarehouseMap, 0xFF, MAX_WAREHOUSEITEMS);
	gObj[aIndex].WarehouseMoney = lpMsg->Money;
	gObj[aIndex].WarehousePW	= lpMsg->pw;
	
	int dbsize = MAX_ITEMDBBYTE_V00;
	
#ifdef ITEM_DBSIZE_EXTEND_20050706
	if( lpMsg->DbVersion >= 0x03 ) {
		dbsize = MAX_ITEMDBBYTE;
	}
	else if (lpMsg->DbVersion >= 0x01) {
		dbsize = MAX_ITEMDBBYTE_V02;
	}
#else
#ifdef SETITEM_BUFFER_CLEAR
	if( lpMsg->DbVersion != 0x00 ) dbsize = MAX_ITEMDBBYTE;
#else
	if( lpMsg->DbVersion == 0x01 ) dbsize = MAX_ITEMDBBYTE;
#endif
#endif

	for( int n=0; n<MAX_WAREHOUSEITEMS; n++)
	{
		itype = lpMsg->dbItems[n*dbsize];

#ifdef ITEM_DBSIZE_EXTEND_20050706
		if( dbsize == MAX_ITEMDBBYTE ) {
#ifdef ITEM_INDEX_EXTEND_20050706
//			itype |= (BYTE)lpMsg->dbInventory[n*ItemDbByte];
			itype |= (lpMsg->dbItems[(n*dbsize)+9]&0xF0)<<5;
			itype |= (lpMsg->dbItems[(n*dbsize)+7]&0x80)<<1;
			_type = itype;
#endif
		}
		else if (dbsize == MAX_ITEMDBBYTE_V02)
#else
		if( dbsize == 10 )
#endif
		{
			if( (lpMsg->dbItems[(n*dbsize)+7]>>7)&1 ) itype += 256;

#ifdef ITEM_INDEX_EXTEND_20050706
			_type = ((itype/32)*MAX_ITEM_INDEX)+(itype%32);
#else
			_type = itype;
#endif
		}
		else
		{
			_type = ((itype/16)*MAX_ITEM_INDEX)+(itype%16);
		}

		gObj[aIndex].pWarehouse[n].Clear();
		
		// 2003-01-08 : 잘못된 타입의 창고 아이템 검사 추가
		// type == 0xef extype = 0x80 현재 의미없는 아이템 타입 
#ifdef ITEM_INDEX_EXTEND_20050706
		if( dbsize >= MAX_ITEMDBBYTE ) {
			if( (lpMsg->dbItems[n*dbsize] == 0xFF)
				&& (lpMsg->dbItems[(n*dbsize)+9]&0xF0) == 0xF0
				&& (lpMsg->dbItems[(n*dbsize)+7]&0x80) == 0x80
				) 
			{
				// 저장할 때 아이템이 없으면 모든 비트를 1로 set 하므로 이런 식별이 가능
				itype = -1;
			}
		}
		else if (dbsize >= MAX_ITEMDBBYTE_V02) {
			if( (lpMsg->dbItems[n*dbsize] == 0xFF) && (lpMsg->dbItems[(n*dbsize)+7]&0x80) == 0x80) 
			{
				itype = -1;
			}
		}
		else {
			if(lpMsg->dbItems[n*dbsize] == 0xFF) 
			{
				itype = -1;
			}
		}
#else
#ifdef INVENTORY_EMPTYSLOT_CHECK_20050217
		if( (lpMsg->dbItems[n*dbsize] == 0xFF) && (lpMsg->dbItems[(n*dbsize)+7]&0x80) == 0x80) {
#else
		if( (lpMsg->dbItems[n*dbsize] == 0xFF) ) {
#endif
			itype = -1;
		}
#endif

		if( !IsItem(_type) )
		{
			itype = -1;
		}

		if( _type == MAKE_ITEMNUM(14,17) || _type == MAKE_ITEMNUM(14,18) )
		{	// 악마의 열쇠, 눈의 경우에는 내구도가 0인건 다 무시한다			
			if( lpMsg->dbItems[(n*dbsize)+2] == 0 )
			{
				_type = -1;
			}
		}

		if( _type == MAKE_ITEMNUM(14,19) )
		{	// 악마의 초대권의 경우에는 내구도가 255인건 다 무시한다			
			if( lpMsg->dbItems[(n*dbsize)+2] == 255 )
			{
				itype = -1;
			}
		}
		
#ifdef CHINA_BUG_RETOUCH		
		if( _type == MAKE_ITEMNUM(13,20) )
		{	// 마법사의 반지, 용/전사의 반지는 창고에 있으면 안된다
			LogAddTD("(%s)(%s) Invalid Ring Item Found - Removed", lpObj->AccountID, lpObj->Name);
			itype = -1;
		}
#endif

#ifndef FOR_BLOODCASTLE_TEST
/*		if ( 
			_type == MAKE_ITEMNUM(13, 16) ||	// 대천사의서
			_type == MAKE_ITEMNUM(13, 17)		// 블러드본
//			_type == MAKE_ITEMNUM(13, 18)		// 투명망토
			)	
		{
			if( lpMsg->dbItems[(n*dbsize)+2] != 128 )
			{
				itype = -1;
			}
		}

		if (_type == MAKE_ITEMNUM(14, 21))
		{
			int _level = ((lpMsg->dbItems[(n*dbsize)+1]>>3)&0xF);
			if (_level == 1) {	// 스톤임
				if( lpMsg->dbItems[(n*dbsize)+2] != 128 )
				{
					itype = -1;
				}
			}
		}
*/
#endif


		if( itype != -1 )
		{	
			item.m_Level = ((lpMsg->dbItems[(n*dbsize)+1]>>3)&0xF);

#ifdef SETITEM_BUFFER_CLEAR
			if( lpMsg->DbVersion < 2 )
			{	// 인벤이 세트용으로 클리어 되어 있지 않으면 세트아이템 정보를 클리어 한다
				lpMsg->dbItems[(n*dbsize)+8] = 0xFF;
				lpMsg->dbItems[(n*dbsize)+9] = 0xFF;
			}
#endif


#ifdef TEST_SETITEM_CLEAR			
			if( !(lpObj->ClearInfoForSetItem&0x02) )
			{	// 인벤이 세트용으로 클리어 되어 있지 않으면 세트아이템 정보를 클리어 한다
				lpMsg->dbItems[(n*dbsize)+8] = 0xFF;
				lpMsg->dbItems[(n*dbsize)+9] = 0xFF;
			}
#endif

			if( _type != MAKE_ITEMNUM(14,11) )	// 행운의 상자는 레벨 체크 안한다
				if( item.m_Level >  MAX_ITEMLEVEL ) item.m_Level = 0;
			
			OptionData = lpMsg->dbItems[(n*dbsize)+1];
			item.m_Option1 = OptionData>>7;
			item.m_Option2 = (OptionData>>2)&0x1;
			item.m_Option3 = OptionData&0x3;

#ifdef ITEM_DBSIZE_EXTEND_20050706
			if( dbsize >= 10 )
#else
			if( dbsize == 10 )
#endif
			{
				if( _type == MAKE_ITEMNUM(13,3) )
				{	// 디노란트는 옵션 3Bit를 모두다 사용한다
					item.m_Option3 |= (lpMsg->dbItems[(n*dbsize)+7]&0x40)>>4;
				}
				else
				{
					if( (lpMsg->dbItems[(n*dbsize)+7]&0x40) == 0x40 )
						item.m_Option3 += 4;
				}
			}

			item.m_Durability = lpMsg->dbItems[(n*dbsize)+2];

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#ifdef MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727 // ITEM_DBSIZE_EXTEND_20050706 define 반드시 On
			if ( dbsize >= MAX_ITEMDBBYTE)
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbItems[(n*dbsize)+10];	// 10번째 버퍼 : 조화의보석 강화 옵션
			}
	#else
			item.m_JewelOfHarmonyOption = lpMsg->dbItems[(n*dbsize)+10];	// 10번째 버퍼 : 조화의보석 강화 옵션
	#endif // MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
	#ifdef MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727 // ITEM_DBSIZE_EXTEND_20050706 define 반드시 On
			if ( dbsize >= MAX_ITEMDBBYTE)
			{
				item.m_ItemOptionEx = (lpMsg->dbItems[(n*dbsize)+9]&0x08)<<4;	// 9번째 버퍼. 상위 5번째 1 비트값을 m_ItemOptionEx의 상위 1비트로 설정
				// 2006.8.2 추가
				if ( item.m_ItemOptionEx != 0 )
				{
					// 380 아이템이 아닌 경우 옵션 삭제
					item.m_Type = itype;	// DBVersion3이상이기 때문에 itype값을 그대로 설정
					if ( g_kItemSystemFor380.Is380Item( &item ) == FALSE )
					{
						item.m_ItemOptionEx = 0;
						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Warehouse pos[%d]", lpObj->AccountID, lpObj->Name, n);
					}
				}
			}
	#else
			item.m_ItemOptionEx = (lpMsg->dbItems[(n*dbsize)+9]&0x08)<<4;	// 9번째 버퍼. 상위 5번째 1 비트값을 m_ItemOptionEx의 상위 1비트로 설정
	#endif // MODIFY_JEWELOFHARMONY_380OPTION_DBVERSION_BUGFIX_20060727
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			BYTE SocketOption[MAX_SOCKET_SLOT];
			BYTE SocketBonusOption = SOCKETSLOT_NONE;
			int iSocketSlotIndex = 0;

			memset( SocketOption, SOCKETSLOT_NONE, MAX_SOCKET_SLOT );

			if( g_SocketOptionSystem.IsEnableSocketItem( itype ) == true )
			{
				SocketBonusOption = lpMsg->dbItems[(n*dbsize)+10];
				for( iSocketSlotIndex = 0; iSocketSlotIndex < MAX_SOCKET_SLOT ; iSocketSlotIndex++ )
				{
					SocketOption[iSocketSlotIndex] = lpMsg->dbItems[(n*dbsize)+11+iSocketSlotIndex];
				}
			}
			else
			{
				SocketBonusOption = 0;

				for( iSocketSlotIndex = 0 ; iSocketSlotIndex < MAX_SOCKET_SLOT ; iSocketSlotIndex++ )
				{
					SocketOption[iSocketSlotIndex] = SOCKETSLOT_NONE;
				}
			}
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

			if( lpMsg->DbVersion == 0x00 )
	#ifdef ADD_380ITEM_NEWOPTION_20060711
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, 0, 0, 0, 0);
	#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, 0, 0, 0);
	#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ITEM_INDEX_EXTEND_20050706
			else if (lpMsg->DbVersion <= 0x02) 
			{
			#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], item.m_ItemOptionEx, NULL, SOCKETSLOT_NONE, 1);
			#else
				#ifdef ADD_380ITEM_NEWOPTION_20060711
					item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], item.m_ItemOptionEx, 1);
				#else
					item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], 1);
				#endif // ADD_380ITEM_NEWOPTION_20060711
			#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			}
			else 
			{
#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], item.m_ItemOptionEx, SocketOption, SocketBonusOption, 3);
#else
	#ifdef ADD_380ITEM_NEWOPTION_20060711
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], item.m_ItemOptionEx, 3);
	#else
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], 3);
	#endif	// ADD_380ITEM_NEWOPTION_20060711
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
			}
#else
			else 
				item.Convert(itype, item.m_Option1, item.m_Option2, item.m_Option3, lpMsg->dbItems[(n*dbsize)+7]&0x3F, lpMsg->dbItems[(n*dbsize)+8], 1);
#endif


#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
			if( _type == ITEM_POTION + 7 )
			{
				// 공성물약 제외
			}
			else
#endif
			if( _type >= ITEM_POTION && 
				_type <= ITEM_POTION+8 
#ifdef ADD_SHIELD_POINT_01_20060403
				|| ( _type >= ITEM_POTION+35 && _type <= ITEM_POTION+40 )
#endif
				) 
			{
				if( item.m_Durability == 0 ) item.m_Durability = 1;
				if( item.m_Durability > g_GlobalConfig.m_iMaxPotionDurability  ) item.m_Durability = g_GlobalConfig.m_iMaxPotionDurability;
			}

			else if( _type != MAKE_ITEMNUM(13,10)
#ifdef ADD_NEW_MAP_KALIMA_20040518					// 쿤둔의 표식의 내구력은 초기값에 맞춰 초기화되면 않된다.
				 && _type != MAKE_ITEMNUM(14,29)
#endif
#ifdef MU_CASTLESIEGE_CS_MARKS_20041214				// 성주의 표식의 내구력은 초기값에 맞춰 초기화되면 않된다.
				 && (_type != MAKE_ITEMNUM(14,21) && item.m_Level == 3)
#endif
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029				// 행운의 동전의 내구력은 초기값에 맞춰 초기화되면 않된다.
				 && _type != MAKE_ITEMNUM(14, 100)
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029				 
				)			
			{
				if( item.m_Durability > item.m_BaseDurability )
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}

			lpObj->pWarehouse[n].m_Option1 = item.m_Option1;
			lpObj->pWarehouse[n].m_Option2 = item.m_Option2;
			lpObj->pWarehouse[n].m_Option3 = item.m_Option3;


#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
			lpObj->pWarehouse[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
#endif //	ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

#ifdef ADD_380ITEM_NEWOPTION_20060711
			lpObj->pWarehouse[n].m_ItemOptionEx = item.m_ItemOptionEx;
#endif // ADD_380ITEM_NEWOPTION_20060711
			
			WORD hiWord, loWord;

			hiWord = MAKEWORD(lpMsg->dbItems[(n*dbsize)+4], lpMsg->dbItems[(n*dbsize)+3]);
			loWord = MAKEWORD(lpMsg->dbItems[(n*dbsize)+6], lpMsg->dbItems[(n*dbsize)+5]);			
			item.m_Number		= MAKELONG(loWord, hiWord);

			gObjWarehouseInsertItemPos(lpObj->m_Index, item, n, -1);
			gObjWarehouseItemSet(lpObj->m_Index, n, 0x01);
		}
	}

#ifdef TEST_SETITEM_CLEAR	// 세트 아이템용 정보를 클리어 했다
	lpObj->ClearInfoForSetItem |= 0x02;
#endif

	pResult.h.c        = PMHCE_BYTE;
	pResult.h.headcode = 0x30;
	pResult.h.size     = sizeof( pResult );
	pResult.result     = 0x02;
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

	// 창고의 아이템을 보낸다.
	GCUserWarehouseSend(&gObj[aIndex]);
	
    if( bCanWarehouseLock == TRUE )
	{
		if( gObj[aIndex].WarehousePW > 0 )
			if( gObj[aIndex].WarehouseLock == 0  )
				GCWarehouseStateSend(aIndex, 12);
			else GCWarehouseStateSend(aIndex, 1);
		else GCWarehouseStateSend(aIndex, 0);
	}

	lpObj->LoadWareHouseInfo = TRUE;

#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	lpObj->m_ReqWarehouseOpen = FALSE;
#endif

#ifdef DARKLORD_WORK
	gObjRequestPetItemInfo(lpObj->m_Index, 1);
#endif	

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[2] != AUTH_CHECK_02 )
		DestroyGIocp();
#endif

#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
    //창고 요청 정보 받고 클라로
	LogAddTD_TempLog(", [G->C Recv GetWarehouseList], Account [%s], IP [%s] ", gObj[aIndex].AccountID, gObj[aIndex].Ip_addr);
#endif
}


// 0x10 창고에 아이템이 없다.
void GDGetWarehouseNoItem(LPSDHP_GETWAREHOUSEDB_RESULT lpMsg)
{
	char szId[MAX_IDSTRING+1];
	int  aIndex=lpMsg->aIndex;

	PMSG_TALKRESULT		pResult;

	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountID, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(431), szId, aIndex);
		return;
	}
	LPOBJECTSTRUCT	lpObj;

	lpObj = &gObj[aIndex];
	if( lpObj->m_IfState.type == I_WAREHOUSE )
	{
		if( lpObj->m_IfState.state == 0 )
		{
			lpObj->m_IfState.state	= 1;
			lpObj->WarehouseSave	= TRUE;

			pResult.h.c        = PMHCE_BYTE;
			pResult.h.headcode = 0x30;
			pResult.h.size     = sizeof( pResult );
			pResult.result     = 0x02;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			LogAddC(LOGC_RED, lMsg.Get(432), lpObj->Name);
			
#ifdef MODIFY_NEW_ACCOUNT_WAREHOUSE_ZEN_BUGFIX_20080812	// 젠 보유량도 보내주도록 수정
			PMSG_MONEY	pMsg;
			
			PHeadSetB((LPBYTE)&pMsg, 0x81, sizeof( pMsg ));
			pMsg.Result     = 0x01;
			pMsg.iMoney		= lpObj->Money; 
			pMsg.wMoney		= lpObj->WarehouseMoney;
			DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#endif // MODIFY_NEW_ACCOUNT_WAREHOUSE_ZEN_BUGFIX_20080812
		}
	}

	lpObj->LoadWareHouseInfo = TRUE;

#ifdef WAREHOUSE_ITEMCOPY_BUG_FIX
	lpObj->m_ReqWarehouseOpen = FALSE;
#endif
}


void GDSetWarehouseList(int aIndex)
{
	if( !gObj[aIndex].LoadWareHouseInfo )
	{
		LogAddTD("[%s][%s] WareHouse Save Fail : Not Open", 
			     gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	SDHP_GETWAREHOUSEDB_SAVE	pMsg;

#if (DATANOSAVE == 1)
	return;
#endif
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	pMsg.aIndex		= aIndex;
	pMsg.h.c		= PMHC_WORD;
	pMsg.h.headcode = 0x09;
	pMsg.h.sizeH    = HIBYTE( sizeof( pMsg ) );
	pMsg.h.sizeL    = LOBYTE( sizeof( pMsg ) );
	pMsg.Money      = gObj[aIndex].WarehouseMoney;
	pMsg.pw			= gObj[aIndex].WarehousePW;

#ifdef ITEM_DBSIZE_EXTEND_20050706
	ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, MAX_WAREHOUSEITEMS);
#else
	ItemByteConvert10(pMsg.dbItems, gObj[aIndex].pWarehouse, MAX_WAREHOUSEITEMS);
#endif
	
	cDBSMng.Send((char*)&pMsg, sizeof( pMsg ));

#ifdef DARKLORD_WORK	// 펫 아이템 정보도 저장한다
	gObjSavePetItemInfo(aIndex, 1);
#endif

	gObjWarehouseTextSave(&gObj[aIndex]);
}


// 0x11 아이템을 DB에 저장시킨다.
void GDUserItemSave(LPOBJECTSTRUCT lpObj)
{
#if (DATANOSAVE == 1)
	return;
#endif

	SDHP_DBCHAR_ITEMSAVE	pMsg;

	pMsg.h.c		= PMHC_WORD;
	pMsg.h.headcode = 0x11;
	pMsg.h.sizeH    = HIBYTE( sizeof( pMsg ) );
	pMsg.h.sizeL    = LOBYTE( sizeof( pMsg ) );
	//ItemByteConvert7(pMsg.dbInventory, lpObj->pInventory, MAX_INVENTORY);
	memcpy(pMsg.Name, lpObj->Name, MAX_IDSTRING);
	
#ifdef ITEM_DBSIZE_EXTEND_20050706
	ItemByteConvert16(pMsg.dbInventory, lpObj->pInventory, MAX_INVENTORY_EXTEND);
#else
#ifdef PERSONAL_SHOP_20040113
	ItemByteConvert10(pMsg.dbInventory, lpObj->pInventory, MAX_INVENTORY_EXTEND);
#else
	ItemByteConvert10(pMsg.dbInventory, lpObj->pInventory, MAX_INVENTORY);
#endif	// #define PERSONAL_SHOP_20040113
#endif	// #define ITEM_DBSIZE_EXTEND_20050706

	//wsDataCli.DataSend((char*)&pMsg, sizeof( pMsg ));
	cDBSMng.Send((char*)&pMsg, sizeof( pMsg ));
}

// 0x12 창고의 돈을 저장시킨다.
void GDSetWarehouseMoney(int aIndex)
{
#if (DATANOSAVE == 1)
	return;
#endif

	SDHP_WAREHOUSEMONEY_SAVE	pMsg;
	
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	pMsg.aIndex		= aIndex;
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode = 0x12;
	pMsg.h.size     = sizeof( pMsg );
	pMsg.Money      = gObj[aIndex].WarehouseMoney;
	
	cDBSMng.Send((char*)&pMsg, sizeof( pMsg ));
}

//----------------------------------------------------------------------------
// [0x20] 게임서버의 정보를 보낸다.
void GDGameServerInfoSave()
{
	/*SDHP_GAMESERVERINFO	pMsg;
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x20;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.ItemCount	= gItemNumberCount;
	wsDataCli.DataSend((char*)&pMsg, pMsg.h.size);
	LogAdd("아이템 번호 저장 : %d", gItemNumberCount);
	*/
}

// 0x52 상위 아이템을 생성하며 아이템 번호를 할당받는다.
void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption, BOOL SpecialShop)
{
	if(ItemAttribute[type].HaveItemInfo == FALSE)
	{
		LogAddTD("[ERROR] Item doesn't exists %d", type);
		return;
	}

	SDHP_ITEMCREATE	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x52;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.MapNumber	= MapNumber;
	pMsg.x			= x;
	pMsg.y			= y;
	pMsg.Type		= (short)type;
	pMsg.Level		= level;
	
	// 2003/02/28 내구력 수치를 얻어 기본 세팅함
	// pMsg.Dur		= dur;

	if( dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur		= dur;

	pMsg.Op1		= Op1;
	pMsg.Op2		= Op2;
	pMsg.Op3		= Op3;
	pMsg.NewOption	= NewOption;
	pMsg.aIndex		= aIndex;
	pMsg.lootindex	= LootIndex;

	pMsg.SetOption = SetOption;

	pMsg.SpecialShop = SpecialShop;

#ifdef MONSTER_ITEMDROP_LOG_ADD_NAME_20041202		// 몬스터가 아이템을 드롭한다는 로그를 남김
	if (CHECK_LIMIT(aIndex, MAX_OBJECT) && gObj[aIndex].Type != OBJTYPE_CHARACTER) 
	{
		// 사람이 아닌 -> 몬스터라면 로그를 남긴다.
		BYTE NewOption[8];
		for( int i=0; i<8; i++) NewOption[i]=0;

		if( pMsg.NewOption&0x20) NewOption[0] = 1;
		if( pMsg.NewOption&0x10) NewOption[1] = 1;
		if( pMsg.NewOption&0x08) NewOption[2] = 1;
		if( pMsg.NewOption&0x04) NewOption[3] = 1;
		if( pMsg.NewOption&0x02) NewOption[4] = 1;
		if( pMsg.NewOption&0x01) NewOption[5] = 1;

		LogAddTD("Monster Item Drop Request [%s] [%d][%d][%d] : [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
				gObj[aIndex].Name,
				MapNumber, pMsg.x, pMsg.y,
				ItemAttribute[pMsg.Type].Name, 				
				pMsg.Level, 
				pMsg.Op1, 
				pMsg.Op2, 
				pMsg.Op3,
				NewOption[0],
				NewOption[1],
				NewOption[2],
				NewOption[3],
				NewOption[4],
				NewOption[5],
				NewOption[6],
				pMsg.SetOption
			   );
	}
#endif

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


#ifdef DARKLORD_WORK
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	SDHP_ITEMCREATE	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x55;
	pMsg.h.size		= sizeof( pMsg );
	pMsg.MapNumber	= MapNumber;
	pMsg.x			= x;
	pMsg.y			= y;
	pMsg.Type		= (short)type;
	pMsg.Level		= level;
	
	// 2003/02/28 내구력 수치를 얻어 기본 세팅함
	// pMsg.Dur		= dur;

	if( dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur		= dur;

	pMsg.Op1		= Op1;
	pMsg.Op2		= Op2;
	pMsg.Op3		= Op3;
	pMsg.NewOption	= NewOption;
	pMsg.aIndex		= aIndex;
	pMsg.lootindex	= LootIndex;

	pMsg.SetOption = SetOption;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}
#endif	// DARKLORD_WORK

// 0x52 시리얼이 할당된 아이템 생성을 받는다.
void ItemSerialCreateRecv(LPSDHP_ITEMCREATERECV lpMsg)
{
	LPOBJECTSTRUCT			lpObj;
	
	int mapnumber	= lpMsg->MapNumber;
	int	aIndex		= lpMsg->aIndex;
	int lootindex   = lpMsg->lootindex;
	
	if( mapnumber != 255 )
	{
//#if defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
#ifdef MODIFY_GAMBLING_COMPLEMENT_20090513
		if( (mapnumber < 0) || ((mapnumber > g_TerrainManager.Size()-1) && (mapnumber < ITEMMAKE_INVENTORY_GAMBLINGITEM)))
#else // MODIFY_GAMBLING_COMPLEMENT_20090513
		if( (mapnumber < 0) || ((mapnumber > g_TerrainManager.Size()-1) && (mapnumber < ITEMMAKE_INVENTORY_1CELL)))
#endif // MODIFY_GAMBLING_COMPLEMENT_20090513
				
//#else // else EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO
/*	#ifdef FOR_BLOODCASTLE
			if( (mapnumber < 0) || ((mapnumber > MAX_MAP-1) && (mapnumber < BC_MAPNUMBER_CHAOSGEM1)))
	
	#else // else FOR_BLOODCASTLE
			if( mapnumber < 0 || mapnumber > MAX_MAP-1)
	
	#endif
*/
//#endif // EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO
		{
			return;
		}
	}

	if( lpMsg->SpecialShop == TRUE )	// 카오스 박스 였는지..
	{
		if( gObjIsConnectedGP(aIndex) == FALSE ) 
		{
			LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
			return;
		}
		lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

		CItem NewItem;								

		NewItem.m_Level      = lpMsg->Level;

		NewItem.m_Durability = (float)ItemGetDurability(lpMsg->Type, lpMsg->Level, lpMsg->NewOption, lpMsg->SetOption);

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
		// 축복의 물약, 영혼의 물약은 내구력이 데이터서버에서 넘어온 상태로 세팅되어야 한다.
		if( lpMsg->Type == MAKE_ITEMNUM(14,7) )
		{
			NewItem.m_Durability = lpMsg->Dur;
		}
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
		// 펜릴의 경우, 업그레이드 시 액설런트 옵션을 사용 하기 때문에 내구력 재조정 필요!
		if( lpMsg->Type == MAKE_ITEMNUM(13,37))
		{
			NewItem.m_Durability = lpMsg->Dur;
		}
#endif

#ifdef ADD_380ITEM_NEWOPTION_20060711
		// 주의 : 380 추가 옵션은 0, 380 추가옵션은 시리얼발급을 필요로하는 아이템이 아님.
		NewItem.Convert(lpMsg->Type, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0);	
#else
		NewItem.Convert(lpMsg->Type, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption);	
#endif // ADD_380ITEM_NEWOPTION_20060711

		NewItem.m_Number = lpMsg->m_Number;	

#ifdef ITEM_INDEX_EXTEND_20050706
		INT iType = lpMsg->Type/MAX_ITEM_INDEX;
		INT iTypeIndex = lpMsg->Type%MAX_ITEM_INDEX;
#else
		BYTE iType = lpMsg->Type >> ITEM_BITSHIFT;
		BYTE iTypeIndex = lpMsg->Type & 0x1F;
#endif

		gObjInventoryInsertItem(lpObj,iType,iTypeIndex,lpMsg->Level,lpMsg->m_Number,lpMsg->Dur);

		BYTE ExOption[8];		
		ItemIsBufExOption(ExOption, &NewItem);

		LogAddTD("[%s][%s] Special Shop Item:%s [%d][%d] [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d] Set:[%d]",
			lpObj->AccountID, lpObj->Name, NewItem.GetName(), lpMsg->m_Number, lpMsg->Dur, NewItem.m_Level, 
			NewItem.m_Option1, NewItem.m_Option2, NewItem.m_Option3, ExOption[0], ExOption[1],
			ExOption[2], ExOption[3], ExOption[4], lpMsg->SetOption);
	}	
#ifdef DARKLORD_WORK
	else if( lpMsg->MapNumber == 255 || lpMsg->MapNumber == 254 )	// 카오스 박스 였는지..
#else
	else if( lpMsg->MapNumber == 255 )	// 카오스 박스 였는지..
#endif
	{
		PMSG_CHAOSMIXRESULT		pMsg;

		if( gObjIsConnectedGP(aIndex) == FALSE ) 
		{
			LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
			return;
		}
		lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
		
#ifdef DARKLORD_WORK
		if( lpMsg->MapNumber == 254 )
		{
			if( lpObj->m_IfState.type != I_DARK_TRAINER )
			{
				LogAdd("error-L2: DarkTrainerBox not used.");
				return;
			}
		}
		else		
#endif
		{
			if( lpObj->m_IfState.type != I_CHAOSBOX )
			{
				LogAdd("error-L2: ChaosBox not used.");
				return;
			}
		}
		pMsg.h.c		= PMHC_BYTE;
		pMsg.h.headcode	= 0x86;
		pMsg.h.size		= sizeof( pMsg );

#ifdef DARKLORD_WORK
		if( lpMsg->MapNumber == 254 )
			pMsg.Result		= 100;	// 조련사 조합은 성공이 100이다
		else
#endif
		pMsg.Result		= 0x01;
		
		CItem NewItem;								
		
		NewItem.m_Level      = lpMsg->Level;

		NewItem.m_Durability = (float)ItemGetDurability(lpMsg->Type, lpMsg->Level, lpMsg->NewOption, lpMsg->SetOption);

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116
		// 축복의 물약, 영혼의 물약은 내구력이 데이터서버에서 넘어온 상태로 세팅되어야 한다.
		if( lpMsg->Type == MAKE_ITEMNUM(14,7) )
		{
			NewItem.m_Durability = lpMsg->Dur;
		}
#endif
		
#ifdef ADD_ITEM_FENRIR_01_20051110
		// 펜릴의 경우, 업그레이드 시 액설런트 옵션을 사용 하기 때문에 내구력 재조정 필요!
		if( lpMsg->Type == MAKE_ITEMNUM(13,37))
		{
			NewItem.m_Durability = lpMsg->Dur;
		}
#endif
		
#ifdef ADD_380ITEM_NEWOPTION_20060711
		// 주의 : 380 추가 옵션은 0, 380 추가옵션은 시리얼발급을 필요로하는 아이템이 아님.
		NewItem.Convert(lpMsg->Type, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0);	
#else
		NewItem.Convert(lpMsg->Type, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption);	
#endif // ADD_380ITEM_NEWOPTION_20060711
		
		ItemByteConvert(pMsg.ItemInfo, NewItem);
		NewItem.m_Number = lpMsg->m_Number;
		
#ifdef MODIFY_MIX_SYSTEM_20070518
		g_MixSystem.ChaosBoxInit(lpObj);
#endif

		gObjChaosBoxInsertItemPos(aIndex, NewItem, 0, -1);
		gObjChaosItemSet(aIndex, 0, 1);		

		BYTE ExOption[8];		
		ItemIsBufExOption(ExOption, &NewItem);
		
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("[%s][%s] CBMix Item Create Item:%s [%u][%d] [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#else
		LogAddTD("[%s][%s] CBMix Item Create Item:%s [%d][%d] [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				lpObj->AccountID, lpObj->Name,
				NewItem.GetName(),
			    lpMsg->m_Number, lpMsg->Dur, 				
				NewItem.m_Level, NewItem.m_Option1, NewItem.m_Option2, NewItem.m_Option3,
				ExOption[0],
			    ExOption[1],
			    ExOption[2],
			    ExOption[3],
			    ExOption[4],
			    ExOption[5],
			    ExOption[6],
				lpMsg->SetOption
			   );

		DataSend( aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		gObj[aIndex].ChaosLock = FALSE;
	}
//#if defined ( EVENT_FRIENDSHIP_20040204 ) || defined ( EVENT_LOTTO )
	else if (lpMsg->MapNumber == ITEMMAKE_INVENTORY_1CELL) {
		if(gObj[lpMsg->aIndex].Connected > 1) {			// 인증을 받은 상태까지는 인정
#ifdef ITEM_INDEX_EXTEND_20050706
			INT iType = lpMsg->Type/MAX_ITEM_INDEX;
			INT iTypeIndex = lpMsg->Type%MAX_ITEM_INDEX;
#else
			BYTE iType = lpMsg->Type >> ITEM_BITSHIFT;
			BYTE iTypeIndex = lpMsg->Type & 0x1F;
#endif
			BYTE iItemPos = gObjInventoryInsertItem(&gObj[lpMsg->aIndex], iType, iTypeIndex, lpMsg->Level, lpMsg->m_Number, lpMsg->Dur);
			if (0xFF == iItemPos) {
				// 에러
				LogAddTD("[Mu_2Anv_Event] Error : Failed To Gift Item Type:%d, TypeIndex:%d to [%s][%s]",
					iType,
					iTypeIndex,
					gObj[lpMsg->aIndex].AccountID,
					gObj[lpMsg->aIndex].Name
					);
			}
			else {
				GCInventoryItemOneSend (lpMsg->aIndex, iItemPos);
			}
		}
	}
//#endif // EVENT_FRIENDSHIP_20040204 || EVENT_LOTTO	
#ifdef ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
	else if( lpMsg->MapNumber == ITEMMAKE_INVENTORY_CASHITEM )
	{
		if( gObj[lpMsg->aIndex].Connected > 1 )
		{
			CItem pCreateItem;
			int iItemType	= lpMsg->Type / MAX_ITEM_INDEX;
			int iItemIndex	= lpMsg->Type % MAX_ITEM_INDEX;

			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;
			
			pCreateItem.Convert( MAKE_ITEMNUM( iItemType, iItemIndex ), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption );
			//pCreateItem.m_Level = lpMsg->Level;
			//pCreateItem.m_Durability = lpMsg->Dur;
			BYTE btItemPos = gObjInventoryInsertItem( lpMsg->aIndex, pCreateItem );
			if (0xFF == btItemPos) {
				// 에러
				LogAddTD("[CashShop] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
					iItemType,
					iItemIndex,
					gObj[lpMsg->aIndex].AccountID,
					gObj[lpMsg->aIndex].Name
					);
			}
			else {
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
			}
		}
	}
#endif // ADD_PARTIALY_CHARGE_CASH_SHOP_20070117
#ifdef UPDATE_LUCKY_COIN_EVENT_20081029	// 행운의 동전을 교환된 아이템을 인벤에 추가
	else if( lpMsg->MapNumber == ITEMMAKE_INVENTORY_COINITEM )
	{
		if( gObj[lpMsg->aIndex].Connected > 1 )
		{
			CItem pCreateItem;
			int iItemType	= lpMsg->Type / MAX_ITEM_INDEX;
			int iItemIndex	= lpMsg->Type % MAX_ITEM_INDEX;

			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;

			pCreateItem.Convert( MAKE_ITEMNUM( iItemType, iItemIndex ), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption );
			pCreateItem.m_Level = lpMsg->Level;
			//pCreateItem.m_Durability = lpMsg->Dur;
			//pCreateItem.m_Number = lpMsg->m_Number;
			BYTE btItemPos = gObjInventoryInsertItem( lpMsg->aIndex, pCreateItem );
			if (0xFF == btItemPos) {
				// 에러
				LogAddTD("[LuckyCoin Trade] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
					iItemType,
					iItemIndex,
					gObj[lpMsg->aIndex].AccountID,
					gObj[lpMsg->aIndex].Name
					);
				GCANSTradeCoin(lpMsg->aIndex, 2);
			}
			else {
				// 아이템 전송
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
				GCANSTradeCoin(lpMsg->aIndex, 1);
			}
			
			gObj[aIndex].ChaosLock = FALSE;
		}
	}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029
#ifdef ADD_GAMBLING_20090120		// 겜블링을 해서 획득한 아이템을 인벤에 추가
	else if( lpMsg->MapNumber == ITEMMAKE_INVENTORY_GAMBLINGITEM )
	{
		if( gObj[lpMsg->aIndex].Connected > 1 )
		{
			CItem pCreateItem;
			int iItemType	= lpMsg->Type / MAX_ITEM_INDEX;
			int iItemIndex	= lpMsg->Type % MAX_ITEM_INDEX;
			
			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;

#ifdef MODIFY_GAMBLING_BUGFIX_1_20090323
			pCreateItem.m_Number = lpMsg->m_Number;
#endif // MODIFY_GAMBLING_BUGFIX_1_20090323
			
			pCreateItem.Convert( MAKE_ITEMNUM( iItemType, iItemIndex ), lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption );
			pCreateItem.m_Level = lpMsg->Level;

			// 인벤에 2*4의 공간이 있는지 체크한다.
			BOOL bIsBlank = gObjGamblingInventoryCheck( lpMsg->aIndex, 2, 4 );
			if ( bIsBlank == TRUE ) {
				// 아이템을 인벤에 넣어준다.
				BYTE btItemPos = gObjInventoryInsertItem( lpMsg->aIndex, pCreateItem );

				if( btItemPos == 0xFF )
				{
					// 에러
					LogAddTD("[Gambling Trade] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
						iItemType,
						iItemIndex,
						gObj[lpMsg->aIndex].AccountID,
						gObj[lpMsg->aIndex].Name
					);

#ifdef MODIFY_GAMBLING_COMPLEMENT_20090513
					// 클라이언트로 일단 패킷을 리턴해준다.
					PMSG_BUYRESULT	pResult;			
					PHeadSetB((LPBYTE)&pResult, 0x32, sizeof( pResult ));
					pResult.Result = 0xFF;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
#endif // MODIFY_GAMBLING_COMPLEMENT_20090513

#ifdef MODIFY_GAMBLING_BUGFIX_1_20090323
					return;
#endif // MODIFY_GAMBLING_BUGFIX_1_20090323
				}
				
				int iBuyTaxMoney = (INT)((INT64)g_iGamblingValue * (INT64)g_CastleSiegeSync.GetTaxRateStore(gObj[lpMsg->aIndex].m_Index) / (INT64)100);
				g_CastleSiegeSync.AddTributeMoney(iBuyTaxMoney);	// 상점에서 물건을 판 세금을 저장

				gObj[lpMsg->aIndex].Money -= (g_iGamblingValue + iBuyTaxMoney);
								
				if( gObj[lpMsg->aIndex].Money < 0 )
					gObj[lpMsg->aIndex].Money = 0;
				
				GCMoneySend(gObj[lpMsg->aIndex].m_Index, gObj[lpMsg->aIndex].Money);
				
				// 아이템 전송
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
				
				// 겜블링 아이템을 받은 유저를 로그로 남긴다.
				BYTE ExOption[6];
				ItemIsBufExOption(ExOption, &pCreateItem);

				// 겜블링 아이템일 경우 축하 메시지와 폭죽을 터트려준다.
				if( lpMsg->Type ==  MAKE_ITEMNUM(3, 11)
					|| lpMsg->Type ==  MAKE_ITEMNUM(5, 33)
					|| lpMsg->Type ==  MAKE_ITEMNUM(4, 24)
					|| lpMsg->Type ==  MAKE_ITEMNUM(2, 18)
					|| lpMsg->Type ==  MAKE_ITEMNUM(5, 34)
					)
				{					
					char szBuffer[512] = "";
					sprintf( szBuffer, lMsg.Get(1262), gObj[lpMsg->aIndex].Name, pCreateItem.GetName() );
					
					PMSG_NOTICE	pNotice;
					TNotice::MakeNoticeMsg( &pNotice, 0, szBuffer );
					TNotice::SendNoticeToAllUser( &pNotice );

					PMSG_SERVERCMD	ServerCmd;
					PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));			
					ServerCmd.CmdType = 0;
					ServerCmd.X = gObj[lpMsg->aIndex].X;
					ServerCmd.Y = gObj[lpMsg->aIndex].Y;
					MsgSendV2( &gObj[lpMsg->aIndex], (LPBYTE)&ServerCmd, sizeof(ServerCmd) );
					DataSend( lpMsg->aIndex, (LPBYTE)&ServerCmd, sizeof(ServerCmd));				

					LogAddTD("[Gambling Unique] User:%s(%s) Item:[%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] Socket[%d,%d,%d,%d,%d]", 
						gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name,
						pCreateItem.GetName(),
						pCreateItem.m_Level,
						pCreateItem.m_Option1,
						pCreateItem.m_Option2,
						pCreateItem.m_Option3,
						pCreateItem.m_Number,
						(int)pCreateItem.m_Durability,
						ExOption[0],
						ExOption[1],
						ExOption[2],
						ExOption[3],
						ExOption[4],
						ExOption[5],
						ExOption[6],
						pCreateItem.m_SetOption,
						pCreateItem.m_ItemOptionEx >> 7,
						g_kJewelOfHarmonySystem.GetItemStrengthenOption( &pCreateItem ),
						g_kJewelOfHarmonySystem.GetItemOptionLevel( &pCreateItem ),
						pCreateItem.m_SocketOption[0],
						pCreateItem.m_SocketOption[1],
						pCreateItem.m_SocketOption[2],
						pCreateItem.m_SocketOption[3],
						pCreateItem.m_SocketOption[4]
					);
				}
				else
				{
					LogAddTD("[Gambling Normal] User:%s(%s) Item:[%s,%d,%d,%d,%d]serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] Socket[%d,%d,%d,%d,%d]", 
						gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name,
						pCreateItem.GetName(),
						pCreateItem.m_Level,
						pCreateItem.m_Option1,
						pCreateItem.m_Option2,
						pCreateItem.m_Option3,
						pCreateItem.m_Number,
						(int)pCreateItem.m_Durability,
						ExOption[0],
						ExOption[1],
						ExOption[2],
						ExOption[3],
						ExOption[4],
						ExOption[5],
						ExOption[6],
						pCreateItem.m_SetOption,
						pCreateItem.m_ItemOptionEx >> 7,
						g_kJewelOfHarmonySystem.GetItemStrengthenOption( &pCreateItem ),
						g_kJewelOfHarmonySystem.GetItemOptionLevel( &pCreateItem ),
						pCreateItem.m_SocketOption[0],
						pCreateItem.m_SocketOption[1],
						pCreateItem.m_SocketOption[2],
						pCreateItem.m_SocketOption[3],
						pCreateItem.m_SocketOption[4]
					);
				}
			}
			else {
				// 겜블 아이템 구매 실패
				GCServerMsgStringSend( lMsg.Get(1260), lpMsg->aIndex, 1);
				
				LogAddTD("[Gambling Fail] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
					iItemType,
					iItemIndex,
					gObj[lpMsg->aIndex].AccountID,
					gObj[lpMsg->aIndex].Name
					);
			}

			// 클라이언트로 일단 패킷을 리턴해준다.
			PMSG_BUYRESULT	pResult;			
			PHeadSetB((LPBYTE)&pResult, 0x32, sizeof( pResult ));
			pResult.Result = 0xFF;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			
			gObj[aIndex].ChaosLock = FALSE;
		}
	}
#endif	// ADD_GAMBLING_20090120
	else 
	{

		//#ifdef PICKUP_SYSTEM
		//	s_PickUp.PickUpActive(lpObj->m_Index, 0);
		//#endif
		//PICKUP
#ifdef PICKUP_SYSTEM
		BOOL DropItem;
		if (s_PickUp.PickUpActive(lpMsg->lootindex, lpMsg->Type))
		{

			if (gObj[lpMsg->lootindex].Connected > PLAYER_CONNECTED)
			{
				CItem pCreateItem;
				int iItemType = ITEM_GET_TYPE(lpMsg->Type);
				int iItemIndex = ITEM_GET_INDEX(lpMsg->Type);

				pCreateItem.Convert(ITEMGET(iItemType, iItemIndex), lpMsg->Op1, lpMsg->Op2,
					lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0);
				pCreateItem.m_Level = lpMsg->Level;
				pCreateItem.m_Durability = lpMsg->Dur;

				BYTE btItemPos = gObjInventoryInsertItem(lpMsg->lootindex, pCreateItem);

				if (btItemPos == (BYTE)-1)
				{
					DropItem = false;
					LogAddTD("[PickupSystem] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
						iItemType, iItemIndex, gObj[lpMsg->lootindex].AccountID, gObj[lpMsg->lootindex].Name);
				}
				else
				{
					DropItem = true;
					GCInventoryItemOneSend(lpMsg->lootindex, btItemPos);
					MsgOutput(lpMsg->lootindex, "[PickUp System] : %s Obtained", ItemAttribute[lpMsg->Type].Name);
				}
			}

		}
		else
		{

			DropItem = false;
		}
		if (!DropItem)
		{
#endif
#ifdef FOR_BLOODCASTLE
		INT iRetMapNumber = lpMsg->MapNumber;

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 맵 체크 변경
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (lpMsg->MapNumber <= BC_MAPNUMBER_CHAOSGEM8)) 
		{
			mapnumber = g_BloodCastle.GetMapNumByBC_CHAOSGEM( iRetMapNumber );
		}
#else		
#ifdef BLOODCASTLE_EXTEND_20040314
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (lpMsg->MapNumber <= BC_MAPNUMBER_CHAOSGEM7)) {
#else
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (lpMsg->MapNumber <= BC_MAPNUMBER_CHAOSGEM6)) {
#endif
			mapnumber -= BC_MAPNUMBER_BASE_CHAOSGEM;
		}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 맵 체크 변경		
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (lpMsg->MapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON8)) 
		{
			mapnumber = g_BloodCastle.GetMapNumByBC_ULTIMATEWEAPON( iRetMapNumber );
		}
#else
#ifdef BLOODCASTLE_EXTEND_20040314
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (lpMsg->MapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON7)) {
#else
		if ((lpMsg->MapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (lpMsg->MapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON6)) {
#endif
			mapnumber -= BC_MAPNUMBER_BASE_ULTIMATEWEAPON;
		}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
		// 소켓아이템은 엑설런트옵션과 세트옵션을 삭제한다.

		BYTE btSocketSlotCount = 0;
		BYTE btSocketOption[MAX_SOCKET_SLOT];
		BYTE btBonusSocketOption = 0;

		if( g_SocketOptionSystem.IsEnableSocketItem( lpMsg->Type ) == true )
		{	
#ifdef MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
			btSocketSlotCount = g_SocketOptionSystem.GetMakeSocketSlotCount( lpMsg->Type );
#else
			btSocketSlotCount = g_SocketOptionSystem.GetMakeSocketSlotCount();
#endif // MODIFY_ITEM_SOCKET_OPTION_BUGFIX_05_20080715
			
#ifdef ADD_SOCKET_OPTION_LOG_02_20080821
			LogAddTD( "[SocketItem] Drop Socket Item ( Socket Slot : %d ) - %s, [%d,%d,%d,%d], Serial : %u, ItemCode : %d, Level : %d, MapNumber : %d(%d/%d)",
				 btSocketSlotCount, ItemAttribute[lpMsg->Type].Name, lpMsg->Level, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->m_Number, lpMsg->Type, lpMsg->Level, lpMsg->MapNumber, lpMsg->x, lpMsg->y );
#else
	#ifdef ADD_SOCKET_OPTION_LOG_20080715
			LogAddTD( "[SocketItem] Drop Socket Item ( Socket Slot : %d ) - ItemCode : %d, Level : %d, MapNumber : %d(%d/%d)",
				 btSocketSlotCount, lpMsg->Type, lpMsg->Level, lpMsg->MapNumber, lpMsg->x, lpMsg->y );
	#endif // ADD_SOCKET_OPTION_LOG_20080715
#endif // ADD_SOCKET_OPTION_LOG_02_20080821

			btBonusSocketOption = SOCKETSLOT_NONE;

			for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
			{
				if( i < btSocketSlotCount )
				{
					btSocketOption[i] = SOCKETSLOT_EMPTY;
				}
				else
				{
					btSocketOption[i] = SOCKETSLOT_NONE;
				}
			}
		}
		else
		{
			btBonusSocketOption = 0;

			for( int i = 0 ; i < MAX_SOCKET_SLOT ; i++ )
			{
				btSocketOption[i] = SOCKETSLOT_NONE;
			}
		}

		INT iItemCount = MapC[mapnumber].MonsterItemDrop(
							lpMsg->Type, 
							lpMsg->Level, 
							lpMsg->Dur, 
							lpMsg->x, 
							lpMsg->y, 
							lpMsg->Op1, 
							lpMsg->Op2, 
							lpMsg->Op3, 
							lpMsg->NewOption,
							lpMsg->SetOption,
							lootindex, 
							lpMsg->m_Number,
							0,
							btSocketOption,
  							btBonusSocketOption );

#else

		INT iItemCount = MapC[mapnumber].MonsterItemDrop(
							lpMsg->Type, 
							lpMsg->Level, 
							lpMsg->Dur, 
							lpMsg->x, 
							lpMsg->y, 
							lpMsg->Op1, 
							lpMsg->Op2, 
							lpMsg->Op3, 
							lpMsg->NewOption,
							lpMsg->SetOption,
							lootindex, 
							lpMsg->m_Number);

#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

		if (iItemCount != -1) 
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 맵 체크 변경 
			if ((iRetMapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (iRetMapNumber <= BC_MAPNUMBER_CHAOSGEM8)) {
#else
#ifdef BLOODCASTLE_EXTEND_20040314
			if ((iRetMapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (iRetMapNumber <= BC_MAPNUMBER_CHAOSGEM7)) {
#else
			if ((iRetMapNumber >= BC_MAPNUMBER_CHAOSGEM1) && (iRetMapNumber <= BC_MAPNUMBER_CHAOSGEM6)) {
#endif
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
				// 만약 아이템이 대천사 보상 혼석이라면
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + BC_REWARD_CHAOSEGEM_LOOT_TIME;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + BC_ANGEL_WEAPON_LOOT_TIME * 2;
			}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 맵 체크 변경
			if ((iRetMapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (iRetMapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON8)) 
			{				
				// 만약 아이템이 대천사 절대아이템 이라면
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + BC_REWARD_CHAOSEGEM_LOOT_TIME * 3;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + BC_ANGEL_WEAPON_LOOT_TIME;

				int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( mapnumber );
				g_BloodCastle.m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = lpMsg->m_Number;
			}
#else
#ifdef BLOODCASTLE_EXTEND_20040314
			if ((iRetMapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (iRetMapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON7)) {
#else
			if ((iRetMapNumber >= BC_MAPNUMBER_ULTIMATEWEAPON1) && (iRetMapNumber <= BC_MAPNUMBER_ULTIMATEWEAPON6)) {
#endif
				// 만약 아이템이 대천사 절대아이템 이라면
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + BC_REWARD_CHAOSEGEM_LOOT_TIME * 3;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + BC_ANGEL_WEAPON_LOOT_TIME;
				g_BloodCastle.m_BridgeData[iRetMapNumber - BC_MAPNUMBER_ULTIMATEWEAPON1].m_nBC_QUESTITEM_SERIAL = lpMsg->m_Number;
			}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
		}

#else
		MapC[mapnumber].MonsterItemDrop(
			lpMsg->Type, 
			lpMsg->Level, 
			lpMsg->Dur, 
			lpMsg->x, 
			lpMsg->y, 
			lpMsg->Op1, 
			lpMsg->Op2, 
			lpMsg->Op3, 
			lpMsg->NewOption,
			lpMsg->SetOption,
			lootindex, 
			lpMsg->m_Number);
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 성물 일경우 없어지는 시간과 루팅 시간 설정
			if(CHECK_ILLUSION_TEMPLE(mapnumber))
			{
				// 성물 루팅 시간
				if(lpMsg->Type == MAKE_ITEMNUM(14, 64))
				{
					MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + ILLUSION_TEMPLE_RELICS_LOOT_TIME * 3 * 1000;
					MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + ILLUSION_TEMPLE_RELICS_LOOT_TIME * 1000;
				}
				
				// 혼석 루팅 시간
				if(lpMsg->Type == MAKE_ITEMNUM(12, 15))
				{
					MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + ILLUSION_TEMPLE_CHAOSGEM_LOOT_TIME;
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_2_20070724	//보상 혼석 루팅시간 10초
					MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 10*1000;
#else
					MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + ILLUSION_TEMPLE_CHAOSGEM_LOOT_TIME * 2;
#endif
				}
			}
#endif

		BYTE NewOption[8];
		for( int i=0; i<8; i++) NewOption[i]=0;

		if( lpMsg->NewOption&0x20) NewOption[0] = 1;
		if( lpMsg->NewOption&0x10) NewOption[1] = 1;
		if( lpMsg->NewOption&0x08) NewOption[2] = 1;
		if( lpMsg->NewOption&0x04) NewOption[3] = 1;
		if( lpMsg->NewOption&0x02) NewOption[4] = 1;
		if( lpMsg->NewOption&0x01) NewOption[5] = 1;		

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAddTD("Monster Item Drop [%d][%d][%d] : serial:%u [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#else
		LogAddTD("Monster Item Drop [%d][%d][%d] : serial:%d [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				mapnumber, lpMsg->x, lpMsg->y,
			    lpMsg->m_Number, 				
				ItemAttribute[lpMsg->Type].Name, 				
				lpMsg->Level, 
				lpMsg->Op1, 
				lpMsg->Op2, 
				lpMsg->Op3,
				NewOption[0],
				NewOption[1],
				NewOption[2],
				NewOption[3],
				NewOption[4],
				NewOption[5],
				NewOption[6],
				lpMsg->SetOption
			   );

#ifdef MONSTER_ITEMDROP_LOG_20040309
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
		LogAdd_MonsterItemDrop("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%u",
	#else
		LogAdd_MonsterItemDrop("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
				mapnumber, lpMsg->x, lpMsg->y,
				lpMsg->Type, 
				lpMsg->Level, 
				lpMsg->Dur, 
				lpMsg->Op1, 
				lpMsg->Op2, 
				lpMsg->Op3,
				lpMsg->NewOption & 0x3F,
				lpMsg->SetOption,
			    lpMsg->m_Number 				
			);
#endif
		
	}
	#ifdef PICKUP_SYSTEM
	}
#endif
}

void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, 
						BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, 
						DWORD serial)
{
	SDHP_ITEMMOVESAVE	pMsg;

	pMsg.h.c = PMHC_BYTE;
	pMsg.h.headcode	= 0x53;
	pMsg.h.size = sizeof( pMsg );
	
	pMsg.ItemLevel  = level;
	pMsg.Serial		= serial;
	pMsg.ItemOp1 = op1;
	pMsg.ItemOp2 = op2;
	pMsg.ItemOp3 = op3;
	pMsg.X		 = x;
	pMsg.Y		 = y;
	memcpy(pMsg.Account, ActID, MAX_IDSTRING);
	strcpy(pMsg.ItemName, Item);
	memcpy(pMsg.Name, Name, MAX_IDSTRING);
	strcpy(pMsg.ServerName, szServerName);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x60] 
#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel)
#else	// UPDATE_OPTIONKEY_LEVEL_20080116
#ifdef VER_CHATWINDOW_OPTION
#ifdef ADD_SEASON_3_NEW_UI_20071122 
void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk)
#else
void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd)
#endif
#else
void DGOptionDataSend(int aIndex, char * szName, LPBYTE KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek)
#endif
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116
{
	SDHP_SKILLKEYDATA	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x60;
	pMsg.h.size			= sizeof( pMsg );

	memcpy(pMsg.Name, szName, MAX_IDSTRING);
#ifdef UPDATE_SKILLKEY_EXPAND_20080804
	memcpy(pMsg.SkillKeyBuffer, KeyBuffer, 20);
#else	// UPDATE_SKILLKEY_EXPAND_20080804
	memcpy(pMsg.SkillKeyBuffer, KeyBuffer, 10);
#endif	// UPDATE_SKILLKEY_EXPAND_20080804
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;

#ifdef VER_CHATWINDOW_OPTION
	pMsg.ChatWindow = ChatWnd;
#endif

#ifdef ADD_SEASON_3_NEW_UI_20071122 
	pMsg.RkeyDefine = Rk;
#endif

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	pMsg.QWERLevel	= QWERLevel;
#endif

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x60] 
void DGOptionDataRecv(LPSDHP_SKILLKEYDATA_SEND lpMsg)
{
	int aIndex = lpMsg->aIndex;
	char szName[MAX_IDSTRING+1];

	if( aIndex < 0 || aIndex >= MAX_OBJECT)
		return;

	memset(szName, 0, MAX_IDSTRING+1);
	memcpy(szName, lpMsg->Name, MAX_IDSTRING);

	szName[MAX_IDSTRING] = '\0';

	if( strcmp(gObj[aIndex].Name, szName) != 0 )
	{
		return;
	}

#ifdef UPDATE_OPTIONKEY_LEVEL_20080116
	GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine, lpMsg->QWERLevel);
#else	// UPDATE_OPTIONKEY_LEVEL_20080116	
#ifdef VER_CHATWINDOW_OPTION	
#ifdef ADD_SEASON_3_NEW_UI_20071122 
	GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine);
#else
	GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow);
#endif
#else
	GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine);
#endif
#endif	// UPDATE_OPTIONKEY_LEVEL_20080116
}

// [0xCF] 서버 이전에 대한 결과를 받는다
void DGMoveOtherServer(LPSDHP_CHARACTER_TRANSFER_RESULT lpMsg)
{
	if( gObjIsConnectedGP(lpMsg->Number) == FALSE ) 
	{
		LogAddC(LOGC_RED, "error-L3 [%s][%d]", __FILE__,__LINE__);
		return;
	}
	
	LPOBJECTSTRUCT lpObj = &gObj[lpMsg->Number];	
	if( lpMsg->Result != 0 )
	{	// 서버 이전 실패 했다
		PMSG_DEFRESULT pResult;	
		PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
		pResult.result	   = 0x01;
		DataSend(lpMsg->Number, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[CharTrasfer] Fail [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpMsg->Result);
#ifdef GAMESERVER_DIVISION
		lpObj->m_MoveOtherServer = FALSE;
#endif

		GCServerMsgStringSend("문제 발생시 change@webzen.co.kr로 문의해 주시기바랍니다", lpObj->m_Index, 1);
		return;
	}

	LogAddTD("[CharTrasfer] Success [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpMsg->Result);

	GCServerMsgStringSend("접속이 종료됩니다.", lpObj->m_Index, 1);	
	GCServerMsgStringSend("분할 서버로 접속해주시기 바랍니다.", lpObj->m_Index, 1);	

	// 마지막으로 저장한번 하고
	GJSetCharacterInfo(lpObj, lpObj->m_Index);

	// 창고는 저장안되게 만들어 두고.
	lpObj->LoadWareHouseInfo = FALSE;

	// 캐릭터를 서버 선택화면으로 보낸다
	gObjCloseSet(lpObj->m_Index, CL_SERVERLIST);
#ifdef GAMESERVER_DIVISION
	lpObj->m_MoveOtherServer = FALSE;
#endif
}

#ifdef DARKLORD_WORK	
void gObjRequestPetItemInfo(int aIndex, int inventype)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	char	pbuffer[2048];
	int		lofs = sizeof(SDHP_REQUEST_PETITEM_INFO);
	

	//SDHP_REQUEST_PETITEM_INFO	pMsg;

	//pMsg.h.set(&pMsg, 0x56, )

	BOOL	founditemcount = 0;
	Request_PetItem_Info	petIteminfo;

	if( inventype == 0 )
	{	// 인벤토리
	#ifdef PERSONAL_SHOP_20040113		
		for( int n=0; n<MAX_INVENTORY_EXTEND; n++)
	#else
		for( int n=0; n<MAX_INVENTORY; n++)
	#endif
		{
			if( lpObj->pInventory[n].IsItem() )
			{
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,4) || 
					lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,5) )
				{	// 다크스피릿또는 다크호스는 펫 정보를 요청한다
#ifdef DELETE_DARKLORD_PET_SERIAL_ZERO_20050617
					if (lpObj->pInventory[n].m_Number == 0) {
						gObjInventoryDeleteItem(aIndex, n);
						GCInventoryItemDeleteSend(aIndex, n, 1);
						continue;
					}
#endif
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
					memcpy(pbuffer+lofs, &petIteminfo, sizeof(Request_PetItem_Info));
					lofs+=sizeof(Request_PetItem_Info);
				}
			}
		}
	}
	else if( inventype == 1 )
	{
		for( int n=0; n<MAX_WAREHOUSEITEMS; n++)
		{
			if( lpObj->pWarehouse[n].IsItem() )
			{
				if( lpObj->pWarehouse[n].m_Type == MAKE_ITEMNUM(13,4) || 
					lpObj->pWarehouse[n].m_Type == MAKE_ITEMNUM(13,5) )
				{	// 다크스피릿또는 다크호스는 펫 정보를 요청한다
#ifdef DELETE_DARKLORD_PET_SERIAL_ZERO_20050617
					if (lpObj->pWarehouse[n].m_Number == 0) {
						gObjWarehouseDeleteItem(aIndex, n);
						continue;
					}
#endif
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
					memcpy(pbuffer+lofs, &petIteminfo, sizeof(Request_PetItem_Info));
					lofs+=sizeof(Request_PetItem_Info);
				}
			}
		}
	}

	if( founditemcount )
	{
		SDHP_REQUEST_PETITEM_INFO pMsg;
		pMsg.h.set((LPBYTE)&pMsg, 0x56,  sizeof(SDHP_REQUEST_PETITEM_INFO)+lofs);

		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_IDSTRING);
		pMsg.Number = aIndex;
		pMsg.InvenType = inventype;
		pMsg.nCount = founditemcount;
		

		memcpy(pbuffer, &pMsg, sizeof(SDHP_REQUEST_PETITEM_INFO));
		cDBSMng.Send(pbuffer, sizeof(SDHP_REQUEST_PETITEM_INFO)+lofs);
	}
}

void DGRecvPetItemInfo(LPBYTE lpData)
{
	LPSDHP_RECV_PETITEM_INFO lpMsg = (LPSDHP_RECV_PETITEM_INFO)lpData; 
	Recv_PetItem_Info*	pPetItemInfo = (Recv_PetItem_Info*)(lpData+sizeof(SDHP_RECV_PETITEM_INFO));

	int		aIndex = lpMsg->Number;
	char	szAccountId[MAX_IDSTRING+1];

	szAccountId[MAX_IDSTRING] = '\0';
	memcpy(szAccountId, lpMsg->AccountID, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(LOGC_RED, "Request to receive petitem infomation doesn't match the user %s",
				szAccountId);
		CloseClient ( aIndex );
		return;
	}

	if( lpMsg->InvenType == 0 )
	{	// 인벤토리
		BOOL	ReCalc = FALSE;
		for( int n=0; n<lpMsg->nCount; n++)
		{
			if( pPetItemInfo[n].nPos < MAX_EQUIPMENT )
			{
				ReCalc = TRUE;
			}

			if( gObj[aIndex].pInventory[pPetItemInfo[n].nPos].IsItem() && 
				gObj[aIndex].pInventory[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial)
			{	

#if TESTSERVER == 1
				DebugOutput(aIndex, "[인벤] %s/%d Lv:%d Exp:%d", 
							gObj[aIndex].pInventory[pPetItemInfo[n].nPos].GetName(),
							pPetItemInfo[n].nPos, 
							pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
#endif

				gObj[aIndex].pInventory[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
			}
		}

		if( ReCalc )
		{
			gObjCalCharacter(aIndex);
		}
	}
	else if( lpMsg->InvenType == 1 )
	{	// 창고
		for( int n=0; n<lpMsg->nCount; n++)
		{
			if( gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].IsItem() && 
				gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial)
			{	
#if TESTSERVER == 1
				DebugOutput(aIndex, "[창고] %s/%d Lv:%d Exp:%d", 
							gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].GetName(),
							pPetItemInfo[n].nPos, 
							pPetItemInfo[n].Level, pPetItemInfo[n].Exp);				
#endif

				gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
			}
		}
	}	
}

void gObjSavePetItemInfo(int aIndex, int inventype)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	char	pbuffer[2048];
	int		lofs = sizeof(SDHP_SAVE_PETITEM_INFO);	

	BOOL	founditemcount = 0;
	Save_PetItem_Info	petIteminfo;

	if( inventype == 0 )
	{	// 인벤토리
	#ifdef PERSONAL_SHOP_20040113		
		for( int n=0; n<MAX_INVENTORY_EXTEND; n++)
	#else
		for( int n=0; n<MAX_INVENTORY; n++)
	#endif
		{
			if( lpObj->pInventory[n].IsItem() )
			{
				if( lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,4) || 
					lpObj->pInventory[n].m_Type == MAKE_ITEMNUM(13,5) )
				{	// 다크스피릿또는 다크호스는 펫 정보를 요청한다
					if( lpObj->pInventory[n].m_IsLoadPetItemInfo )
					{
						founditemcount++;					
						petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
						petIteminfo.Level = lpObj->pInventory[n].m_PetItem_Level;					
						petIteminfo.Exp = lpObj->pInventory[n].m_PetItem_Exp;

						// PI : 펫아이템 인벤 PW : 펫아이템 창고
#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[%s][%s][PI][%d,%s]serial:[%u] Level:%d Exp:%d", 
#else
						LogAddTD("[%s][%s][PI][%d,%s]serial:[%d] Level:%d Exp:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
								 lpObj->AccountID, lpObj->Name, 
								 n, lpObj->pInventory[n].GetName(),
								 petIteminfo.nSerial,
								 petIteminfo.Level,
								 petIteminfo.Exp);

						memcpy(pbuffer+lofs, &petIteminfo, sizeof(Save_PetItem_Info));
						lofs+=sizeof(Save_PetItem_Info);
					}					
				}
			}
		}
	}
	else if( inventype == 1 )
	{
		for( int n=0; n<MAX_WAREHOUSEITEMS; n++)
		{
			if( lpObj->pWarehouse[n].IsItem() )
			{
				if( lpObj->pWarehouse[n].m_Type == MAKE_ITEMNUM(13,4) || 
					lpObj->pWarehouse[n].m_Type == MAKE_ITEMNUM(13,5) )
				{	// 다크스피릿또는 다크호스는 펫 정보를 요청한다
					if( lpObj->pWarehouse[n].m_IsLoadPetItemInfo )
					{
						founditemcount++;					
						petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
						petIteminfo.Level = lpObj->pWarehouse[n].m_PetItem_Level;
						petIteminfo.Exp = lpObj->pWarehouse[n].m_PetItem_Exp;					

#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
						LogAddTD("[%s][%s][PW][%d,%s]serial:[%u] Level:%d Exp:%d", 
#else
						LogAddTD("[%s][%s][PW][%d,%s]serial:[%d] Level:%d Exp:%d", 
#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
								 lpObj->AccountID, lpObj->Name, 
								 n, lpObj->pWarehouse[n].GetName(),
								 petIteminfo.nSerial,
								 petIteminfo.Level,
								 petIteminfo.Exp);

						memcpy(pbuffer+lofs, &petIteminfo, sizeof(Save_PetItem_Info));
						lofs+=sizeof(Save_PetItem_Info);
					}					
				}
			}
		}
	}

	if( founditemcount )
	{
		SDHP_SAVE_PETITEM_INFO pMsg;
		pMsg.h.set((LPBYTE)&pMsg, 0x57,  sizeof(SDHP_SAVE_PETITEM_INFO)+lofs);
		
		pMsg.nCount = founditemcount;		

		memcpy(pbuffer, &pMsg, sizeof(SDHP_SAVE_PETITEM_INFO));
		cDBSMng.Send(pbuffer, sizeof(SDHP_SAVE_PETITEM_INFO)+lofs);
	}
}
#endif


#ifdef GS_DS_LIVECHECK_20041008			// [0x58] GS -> DS 로 LIVE 신호를 지속적으로 보낸다.


void GS_GDReqLiveCheck()
{
	SDHP_REQ_GSLIVECHECK	pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x58,  sizeof(SDHP_REQ_GSLIVECHECK));

	pMsg.g_iGsCode			= gGameServerCode;

	cDBSMng.Send((char*)&pMsg, sizeof(SDHP_REQ_GSLIVECHECK));
}


void GS_DGAnsLiveCheck(LPSDHP_ANS_GSLIVECHECK pMsg)
{
	if (pMsg->g_iGsCode	== gGameServerCode) {
		// LIVE 체크가 성공적으로 이루어졌다.
		LogAddTD("[DS-LiveCheck] [%d] Live Check OK (%d-%d, TICK:%d)",
			cDBSMng.GetActiveDS(),
			pMsg->g_iGsCode,
			gGameServerCode,
			GetTickCount()
			);
	}
	else {
		// LIVE 체크에 오류가 있다.
		LogAddTD("[DS-LiveCheck] [%d] Live Check ERROR (%d-%d, TICK:%d)",
			cDBSMng.GetActiveDS(),
			pMsg->g_iGsCode,
			gGameServerCode,
			GetTickCount()
			);
	}

	cDBSMng.ResetActiveServerLiveTime();
}
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
void	GDRequestBlockHackUser(int aIndex, char* AccountID, char* Name)
{
	if( !gBlockHackUserCharacter )
		return;

	SDHP_REQ_HACKUSERBLOCK pMsg;	
	pMsg.h.set((LPBYTE)&pMsg, 0x70,  sizeof(SDHP_REQ_HACKUSERBLOCK));	

	memcpy(pMsg.AccountID, AccountID, MAX_IDSTRING);
	memcpy(pMsg.Name, Name, MAX_IDSTRING);
	pMsg.UserIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, sizeof(SDHP_REQ_HACKUSERBLOCK));
}

void DGBlockHackUserResult(LPSDHP_HACKUSERBLOCK_RESULT aRecv)
{
	char	AccountID[MAX_IDSTRING+1], Name[MAX_IDSTRING+1];
	memcpy(AccountID, aRecv->AccountID, MAX_IDSTRING);
	memcpy(Name, aRecv->Name, MAX_IDSTRING);

	AccountID[MAX_IDSTRING] = NULL;
	Name[MAX_IDSTRING] = NULL;

	ChinaHackLogFile.Output("[HLS] [%s][%s] Block Character HackUser Result : %d %d",
						    AccountID, Name, aRecv->Result, aRecv->WarnLevel);

	int aIndex = aRecv->UserIndex;
	
	if( !strcmp(gObj[aRecv->UserIndex].AccountID, AccountID) )
	{
		if( aRecv->WarnLevel == 2 )
		{
			ChinaHackLogFile.Output("[HLS] [%s][%s] Block Account HackUser Result : %d %d",
						     AccountID, Name, aRecv->Result, aRecv->WarnLevel);

			// 핵로그툴 사용 2차 경고 메시지
			GCServerMsgStringSend(lMsg.Get(1612), aRecv->UserIndex, 0);

			if( gBlockHackUserAccount )
				GJPAccountBlock(aIndex, gObj[aIndex].AccountID, gObj[aIndex].DBNumber,  gObj[aIndex].UserNumber, 1);
		}
		else
		{
			// 핵로그툴 사용 1차 경고 메시지
			GCServerMsgStringSend(lMsg.Get(1611), aRecv->UserIndex, 0);
		}
		gObjCloseSet(aRecv->UserIndex, CL_EXIT);
		//CloseClient(aRecv->UserIndex);
	}
}
#endif


#ifdef MU_CASTLESIEGE_DS_PROTOCOL_20041105		// 공성전 관련 GS <-> DS 프로토콜 정의

// < GS -> DS >
// [0x80][0x00] 성에 대한 전체적인 정보 요청
void GS_GDReqCastleTotalInfo		(int iMapSvrGroup, int iCastleEventCycle)
{
	if (iMapSvrGroup < 0)
		return;

	CSP_REQ_CASTLEDATA pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x00, sizeof(CSP_REQ_CASTLEDATA));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	pMsg.iCastleEventCycle		= iCastleEventCycle;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x01] 수성측 길드의 길마이름 요청 -> 경비병 NPC 성 상태 출력
void GS_GDReqOwnerGuildMaster		(int iMapSvrGroup, int aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;

	CSP_REQ_OWNERGUILDMASTER pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x01, sizeof(CSP_REQ_OWNERGUILDMASTER));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x03] 수성측 NPC 구입 요청
void GS_GDReqCastleNpcBuy			(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR, int iBuyCost)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_NPCBUY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x03, sizeof(CSP_REQ_NPCBUY));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iNpcNumber		= iNpcNumber;
	pMsg.iNpcIndex		= iNpcIndex;
	pMsg.iNpcDfLevel	= iNpcDfLevel;
	pMsg.iNpcRgLevel	= iNpcRgLevel;
	pMsg.iNpcMaxHp		= iNpcMaxHP;
	pMsg.iNpcHp			= iNpcHP;
	pMsg.btNpcX			= btNpcX;
	pMsg.btNpcY			= btNpcY;
	pMsg.btNpcDIR		= btNpcDIR;
	pMsg.iBuyCost		= iBuyCost;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x04] 수성측 NPC 수리 요청
void GS_GDReqCastleNpcRepair		(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_NPCREPAIR pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x04, sizeof(CSP_REQ_NPCREPAIR));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iNpcNumber		= iNpcNumber;
	pMsg.iNpcIndex		= iNpcIndex;
	pMsg.iRepairCost	= iRepairCost;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x05] 수성측 NPC 업그레이드 요청
void GS_GDReqCastleNpcUpgrade		(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_NPCUPGRADE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x05, sizeof(CSP_REQ_NPCUPGRADE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iNpcNumber		= iNpcNumber;
	pMsg.iNpcIndex		= iNpcIndex;
	pMsg.iNpcUpType		= iNpcUpType;
	pMsg.iNpcUpValue	= iNpcUpValue;
	pMsg.iNpcUpIndex	= iNpcUpIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x06] 성주측 세금정보 요청 (소유 젠도 함께 보내야함)
void GS_GDReqTaxInfo				(int iMapSvrGroup, int aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_TAXINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x06, sizeof(CSP_REQ_TAXINFO));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x07] 성주측 세율 변경 요청
void GS_GDReqTaxRateChange			(int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_TAXRATECHANGE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x07, sizeof(CSP_REQ_TAXRATECHANGE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iTaxRate		= iTaxRate;
	pMsg.iTaxKind		= iTaxType;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x08] 성주측 소유 젠 변경 요청
void GS_GDReqCastleMoneyChange		(int iMapSvrGroup, int aIndex, int iMoneyChange)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_MONEYCHANGE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x08, sizeof(CSP_REQ_MONEYCHANGE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iMoneyChanged	= iMoneyChange;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x09] 성주측 공성 시작/종료 시간 변경 요청
void GS_GDReqSiegeDateChange		(int iMapSvrGroup, int aIndex, WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay)
{
	if (iMapSvrGroup < 0)
		return;
//	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
//		return;
	
	CSP_REQ_SDEDCHANGE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x09, sizeof(CSP_REQ_SDEDCHANGE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.wStartYear		= wStartYear;
	pMsg.btStartMonth	= btStartMonth;
	pMsg.btStartDay		= btStartDay;
	pMsg.wEndYear		= wEndYear;
	pMsg.btEndMonth		= btEndMonth;
	pMsg.btEndDay		= btEndDay;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x0A] 공성측 특정 길드등록 정보 요청
void GS_GDReqGuildMarkRegInfo		(int iMapSvrGroup, int aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_GUILDREGINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0A, sizeof(CSP_REQ_GUILDREGINFO));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	memcpy (pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x0B] 현재 공성 종료여부 변경 요청
void GS_GDReqSiegeEndedChange		(int iMapSvrGroup, BOOL bIsSiegeEnded)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_SIEGEENDCHANGE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0B, sizeof(CSP_REQ_SIEGEENDCHANGE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.bIsSiegeEnded	= bIsSiegeEnded;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x0C] 현재 성 소유상태 변경 요청
void GS_GDReqCastleOwnerChange		(int iMapSvrGroup, BOOL bIsCastleOccupied, LPSTR lpszGuildName)
{
	if (iMapSvrGroup < 0)
		return;
	if (lpszGuildName == NULL)
		return;
	
	CSP_REQ_CASTLEOWNERCHANGE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0C, sizeof(CSP_REQ_CASTLEOWNERCHANGE));
	pMsg.wMapSvrNum			= (WORD) iMapSvrGroup;
	pMsg.bIsCastleOccupied	= bIsCastleOccupied;
	memcpy (pMsg.szOwnerGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x0D] 공성측 길드의 공성전 등록 요청
void GS_GDReqRegAttackGuild		(int iMapSvrGroup, INT aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_REGATTACKGUILD pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0D, sizeof(CSP_REQ_REGATTACKGUILD));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	memcpy (pMsg.szEnemyGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x80][0x0E] 공성에 대한 주기가 끝났으므로 현재 성에 대한 정보를 리셋 요청
void GS_GDReqRestartCastleState		(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_CASTLESIEGEEND pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0E, sizeof(CSP_REQ_CASTLESIEGEEND));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x80][0x0F] 특정 맵서버군에 메시지를 멀티 캐스팅 요청
void GS_GDReqMapSvrMsgMultiCast		(int iMapSvrGroup, LPSTR lpszMsgText)
{
	if (iMapSvrGroup < 0)
		return;
	if (lpszMsgText == NULL)
		return;
	
	CSP_REQ_MAPSVRMULTICAST pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0F, sizeof(CSP_REQ_MAPSVRMULTICAST));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	strcpy (pMsg.szMsgText, lpszMsgText);
	pMsg.szMsgText[sizeof(pMsg.szMsgText)-1]	= 0;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x10] 특정 길드의 문장 등록을 요청
void GS_GDReqRegGuildMark			(int iMapSvrGroup, int aIndex, int iItemPos)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_GUILDREGMARK pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x10, sizeof(CSP_REQ_GUILDREGMARK));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.iItemPos		= iItemPos;
	memcpy (pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x11] 공성측 특정 길드 마크 개수 초기화 요청
void GS_GDReqGuildMarkReset			(int iMapSvrGroup, int aIndex, LPSTR lpszGuildName)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	if (lpszGuildName == NULL)
		return;
	
	CSP_REQ_GUILDRESETMARK pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x11, sizeof(CSP_REQ_GUILDRESETMARK));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	memcpy (pMsg.szGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x12] 공성측 특정 길드 공성 포기여부 수정 요청
void GS_GDReqGuildSetGiveUp			(int iMapSvrGroup, int aIndex, BOOL bIsGiveUp)
{
	if (iMapSvrGroup < 0)
		return;
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return;
	
	CSP_REQ_GUILDSETGIVEUP pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x12, sizeof(CSP_REQ_GUILDSETGIVEUP));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	pMsg.bIsGiveUp		= bIsGiveUp;
	memcpy (pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x16] 수성측 NPC 제거 요청
void GS_GDReqNpcRemove				(int iMapSvrGroup, int iNpcNumber, int iNpcIndex)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_NPCREMOVE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x16, sizeof(CSP_REQ_NPCREMOVE));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iNpcNumber		= iNpcNumber;
	pMsg.iNpcIndex		= iNpcIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x17] 현재의 공성상태 동기화 요청
void GS_GDReqCastleStateSync		(int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, INT iTaxHuntZone, LPSTR lpszOwnerGuild)
{
	if (iMapSvrGroup < 0)
		return;
		
	CSP_REQ_CASTLESTATESYNC pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x17, sizeof(CSP_REQ_CASTLESTATESYNC));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iCastleState	= iCastleState;
	pMsg.iTaxRateChaos	= iTaxRateChaos;
	pMsg.iTaxRateStore	= iTaxRateStore;
	pMsg.iTaxHuntZone	= iTaxHuntZone;
	memcpy(pMsg.szOwnerGuildName, lpszOwnerGuild, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x80][0x18] 맵서버군의 성 공납금 추가 요청
void GS_GDReqCastleTributeMoney		(int iMapSvrGroup, int iCastleTributeMoney)
{
	if (iMapSvrGroup < 0)
		return;
	
	if (iCastleTributeMoney < 0)
		return;
		
	CSP_REQ_CASTLETRIBUTEMONEY pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x18, sizeof(CSP_REQ_CASTLETRIBUTEMONEY));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	pMsg.iCastleTributeMoney	= iCastleTributeMoney;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// GD [0x80][0x19] 맵서버군의 성 세율, 돈 초기화 요청 (0xC1)
void GS_GDReqResetCastleTaxInfo		(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
		
	CSP_REQ_RESETCASTLETAXINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x19, sizeof(CSP_REQ_RESETCASTLETAXINFO));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// GD [0x80][0x1A] 공성 참여길드 리스트 초기화 요청 (0xC1)
void GS_GDReqResetSiegeGuildInfo	(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
		
	CSP_REQ_RESETSIEGEGUILDINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1A, sizeof(CSP_REQ_RESETSIEGEGUILDINFO));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// GD [0x80][0x1B] 공성 참여신청 길드 리스트 초기화 요청 (0xC1)
void GS_GDReqResetRegSiegeInfo		(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
		
	CSP_REQ_RESETREGSIEGEINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1B, sizeof(CSP_REQ_RESETREGSIEGEINFO));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x81] 공성전 최초 모든 초기화 데이터 요청
void GS_GDReqCastleInitData			(int iMapSvrGroup, int iCastleEventCycle)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_CSINITDATA pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x81, sizeof(CSP_REQ_CSINITDATA));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	pMsg.iCastleEventCycle		= iCastleEventCycle;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x82] 수성측 NPC정보 정보 요청
void GS_GDReqCastleNpcInfo			(int iMapSvrGroup, int aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	// 인덱스가 -1 이면 게임서버 자체에서 요청
	
	CSP_REQ_NPCDATA pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x82, sizeof(CSP_REQ_NPCDATA));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
		
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// [0x83] 공성측 길드등록 정보 요청 (길드 개수 N개 제한)
void GS_GDReqAllGuildMarkRegInfo	(int iMapSvrGroup, int aIndex)
{
	if (iMapSvrGroup < 0)
		return;
	// 인덱스가 -1 이면 게임서버 자체에서 요청
	
	CSP_REQ_ALLGUILDREGINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x83, sizeof(CSP_REQ_ALLGUILDREGINFO));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	pMsg.iIndex			= aIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x85] 공성측 길드선정 후보자료 요청
void GS_GDReqCalcRegGuildList	(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_CALCREGGUILDLIST pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x85, sizeof(CSP_REQ_CALCREGGUILDLIST));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


// [0x88] 공/수 길드의 최종정보를 불러오기 요청
void GS_GDReqCsLoadTotalGuildInfo	(int iMapSvrGroup)
{
	if (iMapSvrGroup < 0)
		return;
	
	CSP_REQ_CSLOADTOTALGUILDINFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x88, sizeof(CSP_REQ_CSLOADTOTALGUILDINFO));
	pMsg.wMapSvrNum		= (WORD) iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


//==============================================================================

#include "PROTOCOL.H"

// < DS -> GS >
// [0x80][0x00] 성에 대한 전체적인 정보 요청 응답
void GS_DGAnsCastleTotalInfo		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_CASTLEDATA lpMsg	= (LPCSP_ANS_CASTLEDATA) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#endif
}

// [0x80][0x01] 수성측 길드의 길마이름 요청 응답 -> 경비병 NPC 성 상태 출력
void GS_DGAnsOwnerGuildMaster		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_OWNERGUILDMASTER lpMsg	= (LPCSP_ANS_OWNERGUILDMASTER) lpRecv;
	
	if (lpMsg == NULL) {
		return;
	}
	
#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	GCAnsCastleSiegeState (lpMsg->iIndex, lpMsg->iResult, lpMsg->szCastleOwnGuild, lpMsg->szCastleOwnGuildMaster);

#endif
}

// [0x80][0x03] 수성측 NPC 구입 요청 응답
void GS_DGAnsCastleNpcBuy			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_NPCBUY lpMsg	= (LPCSP_ANS_NPCBUY) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x03] GS_DGAnsCastleNpcBuy() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	if (lpMsg->iResult == 1) {
		// 위의 결과가 성공이면 여기서 NPC를 실제로 추가
		BOOL bRETVAL	= g_CastleSiege.AddDbNPC(lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		if (bRETVAL == TRUE) {
			// 사용자에게서 구입비를 삭감한다.
			if (gObjIsConnected(lpMsg->iIndex)) {
				gObj[lpMsg->iIndex].Money -= lpMsg->iBuyCost;
				if (gObj[lpMsg->iIndex].Money < 0) {
					gObj[lpMsg->iIndex].Money	= 0;
				}
				GCMoneySend(lpMsg->iIndex, gObj[lpMsg->iIndex].Money);	
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() OK - Npc:(CLS:%d, IDX:%d)",
				lpMsg->iNpcNumber,
				lpMsg->iNpcIndex
				);
		}
		else {
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",
				lpMsg->iNpcNumber,
				lpMsg->iNpcIndex
				);
		}
	}
	else {
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",
			lpMsg->iResult,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
	}

	GCAnsNpcBuy (lpMsg->iIndex, lpMsg->iResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
	
#endif
}

// [0x80][0x04] 수성측 NPC 수리 요청 응답
void GS_DGAnsCastleNpcRepair		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_NPCREPAIR lpMsg	= (LPCSP_ANS_NPCREPAIR) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x04] GS_DGAnsCastleNpcRepair() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		// 위의 결과가 성공이면 여기서 NPC를 실제로 수리
		BOOL bRETVAL	= g_CastleSiege.RepairDbNPC(lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcHp, lpMsg->iNpcMaxHp);
		if (bRETVAL == TRUE) {
			// 사용자에게서 수리비를 삭감한다.
			if (gObjIsConnected(lpMsg->iIndex)) {
				gObj[lpMsg->iIndex].Money -= lpMsg->iRepairCost;
				if (gObj[lpMsg->iIndex].Money < 0) {
					gObj[lpMsg->iIndex].Money	= 0;
				}
				GCMoneySend(lpMsg->iIndex, gObj[lpMsg->iIndex].Money);	
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() OK - Npc:(CLS:%d, IDX:%d)",
				lpMsg->iNpcNumber,
				lpMsg->iNpcIndex
				);
		}
		else {
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",
				lpMsg->iNpcNumber,
				lpMsg->iNpcIndex
				);
		}
	}
	else {
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",
			lpMsg->iResult,
			lpMsg->iNpcNumber,
			lpMsg->iNpcIndex
			);
	}
	
	GCAnsNpcRepair(lpMsg->iIndex, lpMsg->iResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcHp, lpMsg->iNpcMaxHp);

#endif
}

// [0x80][0x05] 수성측 NPC 업그레이드 요청 응답
void GS_DGAnsCastleNpcUpgrade		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_NPCUPGRADE lpMsg	= (LPCSP_ANS_NPCUPGRADE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x05] GS_DGAnsCastleNpcUpgrade() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
#ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619	
	if( lpMsg->iResult == 0 )
	{
		// DS에서 업그레이드 실패 한 경우 - 에러상황		
		LogAddC(LOGC_RED, "[CastleSiege] ERROR - Castle NPC Upgrade Fail() (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",
			lpMsg->iNpcNumber, 
			lpMsg->iNpcIndex, 
			lpMsg->iNpcUpType, 
			lpMsg->iNpcUpValue
			);
	}
	else
	{
		// 이곳에서 실제 업그레이드를 한다.
		g_CastleSiege.UpgradeDbNPC( lpMsg->iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, lpMsg->iNpcUpIndex );
		LogAddTD("[CastleSiege] [0x80][0x06] GS_DGAnsTaxInfo() - Npc Upgrade OK (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",
			lpMsg->iNpcNumber, 
			lpMsg->iNpcIndex, 
			lpMsg->iNpcUpType, 
			lpMsg->iNpcUpValue
			);
	}
#else	// #ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619	
	if (lpMsg->iResult == 1) {
		// 위의 결과가 성공이면 여기서 NPC를 실제로 업그레이드
		BOOL bRETVAL	= g_CastleSiege.UpgradeDbNPC(lpMsg->iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, lpMsg->iNpcUpIndex);
		if (bRETVAL == FALSE)
			return;

		LogAddTD("[CastleSiege] [0x80][0x06] GS_DGAnsTaxInfo() - Npc Upgrade OK (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",
			lpMsg->iNpcNumber, 
			lpMsg->iNpcIndex, 
			lpMsg->iNpcUpType, 
			lpMsg->iNpcUpValue
			);
	}
#endif	// #ifdef MODIFY_CASTLE_NPC_UPGRADE_PROCESS_20060619	
	GCAnsNpcUpgrade (lpMsg->iIndex, lpMsg->iResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);

#endif
}

// [0x80][0x06] 성주측 세금정보 요청 응답 (소유 젠도 함께 보내야함)
void GS_DGAnsTaxInfo				(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_TAXINFO lpMsg	= (LPCSP_ANS_TAXINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x06] GS_DGAnsTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		GCAnsTaxMoneyInfo (lpMsg->iIndex, lpMsg->iResult, lpMsg->iTaxRateChaos, lpMsg->iTaxRateStore, lpMsg->i64CastleMoney);

		// 성의 돈을 반환값으로 세팅한다.
		g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
	else {

	}
	

#endif
}

// [0x80][0x07] 성주측 세율 변경 요청 응답
void GS_DGAnsTaxRateChange			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_TAXRATECHANGE lpMsg	= (LPCSP_ANS_TAXRATECHANGE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x07] GS_DGAnsTaxRateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	if (lpMsg->iResult == 1) {
		// 여기서 실제 세율을 변경하고 서버마다 동기화 시킨다.
		g_CastleSiege.SetTaxRate(lpMsg->iTaxKind, lpMsg->iTaxRate);
		GCAnsTaxRateChange (lpMsg->iIndex, lpMsg->iResult, lpMsg->iTaxKind, lpMsg->iTaxRate);
	}
	else {
		
	}

#endif
}

// [0x80][0x08] 성주측 소유 젠 변경 요청 응답
void GS_DGAnsCastleMoneyChange		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_MONEYCHANGE lpMsg	= (LPCSP_ANS_MONEYCHANGE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x08] GS_DGAnsCastleMoneyChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		if (gObjIsConnected(lpMsg->iIndex)) {
			// 출금 요청이 성공했다면 사용자의 돈을 가감한다.
			if (lpMsg->iMoneyChanged < 0) {
				gObj[lpMsg->iIndex].Money = gObj[lpMsg->iIndex].Money - lpMsg->iMoneyChanged;
#ifdef MODIFY_ZEN_MAX_20040414
				if (gObj[lpMsg->iIndex].Money > MAX_ZEN)
					gObj[lpMsg->iIndex].Money = MAX_ZEN;
#else
				if (gObj[lpMsg->iIndex].Money > 2000000000)
					gObj[lpMsg->iIndex].Money = 2000000000;
#endif

				GCMoneySend(lpMsg->iIndex, gObj[lpMsg->iIndex].Money);

				LogAddTD("[CastleSiege] [0x80][0x08] GS_DGAnsCastleMoneyChange() - Withdraw Request OK [%s][%s] (ReqMoney:%d, TotMoney:%I64d)",
					gObj[lpMsg->iIndex].AccountID,
					gObj[lpMsg->iIndex].Name,
					lpMsg->iMoneyChanged,
					lpMsg->i64CastleMoney
					);
			}
		}
		// 성의 돈을 반환값으로 세팅한다.
		g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
	GCAnsMoneyDrawOut (lpMsg->iIndex, lpMsg->iResult, lpMsg->i64CastleMoney);

#endif
}

// [0x80][0x09] 성주측 공성 시작/종료 시간 변경 요청 응답
void GS_DGAnsSiegeDateChange		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_SDEDCHANGE lpMsg	= (LPCSP_ANS_SDEDCHANGE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x09] GS_DGAnsSiegeDateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#endif
}

// [0x80][0x0A] 공성측 특정 길드등록 정보 요청 응답
void GS_DGAnsGuildMarkRegInfo		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_GUILDREGINFO lpMsg	= (LPCSP_ANS_GUILDREGINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0A] GS_DGAnsGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	GCAnsGuildRegInfo (lpMsg->iIndex, lpMsg->iResult, lpMsg);

#endif
}

// [0x80][0x0B] 현재 공성 종료여부 변경 요청 응답
void GS_DGAnsSiegeEndedChange		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_SIEGEENDCHANGE lpMsg	= (LPCSP_ANS_SIEGEENDCHANGE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0B] GS_DGAnsSiegeEndedChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#endif
}

// [0x80][0x0C] 현재 성 소유상태 변경 요청 응답
void GS_DGAnsCastleOwnerChange		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CASTLEOWNERCHANGE lpMsg	= (LPCSP_ANS_CASTLEOWNERCHANGE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0C] GS_DGAnsCastleOwnerChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#endif
}

// [0x80][0x0D] 공성측 길드의 공성전 등록 요청 응답
void GS_DGAnsRegAttackGuild		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_REGATTACKGUILD lpMsg	= (LPCSP_ANS_REGATTACKGUILD) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0D] GS_DGAnsRegAttackGuild() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	GCAnsRegCastleSiege (lpMsg->iIndex, lpMsg->iResult, lpMsg->szEnemyGuildName);

#endif
}

// [0x80][0x0E] 공성에 대한 주기가 끝났으므로 현재 성에 대한 정보를 리셋 요청 응답
void GS_DGAnsRestartCastleState		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CASTLESIEGEEND lpMsg	= (LPCSP_ANS_CASTLESIEGEEND) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0E] GS_DGAnsRestartCastleState() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 데이터 저장 유무에 상관없이 무조건 초기화 한다.
	g_CastleSiege.ResetCastleCycle();

#endif
}

// [0x80][0x0F] 특정 맵서버군에 메시지를 멀티 캐스팅 요청 응답
void GS_DGAnsMapSvrMsgMultiCast		(LPBYTE lpRecv)
{
	// 모든 서버들에 공통적으로 존재해야 함 !!
	
	LPCSP_ANS_MAPSVRMULTICAST lpMsg	= (LPCSP_ANS_MAPSVRMULTICAST) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x0F] GS_DGAnsMapSvrMsgMultiCast() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 현재 본 서버의 전체 인원에게 맵서버 공지를 한다.
	char szNotice[sizeof(lpMsg->szMsgText) + 16]	= {0,};
	memcpy(szNotice, lpMsg->szMsgText, sizeof(lpMsg->szMsgText));
	if( strlen( szNotice) > 1 )
	{
#ifdef MODIFY_NOTICE_20040325
		PMSG_NOTICE	pNotice;
		TNotice::MakeNoticeMsg( &pNotice, 0, szNotice );
//		TNotice::SetNoticeProperty(&pNotice, TNOTICE_PRIORITY_LV1, TNOTICE_COLOR_DEFAULT);
		TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);
		TNotice::SendNoticeToAllUser( &pNotice );
#else
		AllSendServerMsg(szNotice);
#endif
	}
}

// [0x80][0x10] 특정 길드의 문장 등록을 요청 응답
void GS_DGAnsRegGuildMark			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_GUILDREGMARK lpMsg	= (LPCSP_ANS_GUILDREGMARK) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x10] GS_DGAnsRegGuildMark() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	GCAnsRegGuildMark (lpMsg->iIndex, lpMsg->iResult, lpMsg);

#endif
}

// [0x80][0x11] 공성측 특정 길드 마크 개수 초기화 요청 응답
void GS_DGAnsGuildMarkReset			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_GUILDRESETMARK lpMsg	= (LPCSP_ANS_GUILDRESETMARK) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x11] GS_DGAnsGuildMarkReset() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
#endif
}

// [0x80][0x12] 공성측 특정 길드 공성 포기여부 수정 요청 응답
void GS_DGAnsGuildSetGiveUp			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_GUILDSETGIVEUP lpMsg	= (LPCSP_ANS_GUILDSETGIVEUP) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x12] GS_DGAnsGuildSetGiveUp() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	GCAnsGiveUpCastleSiege (lpMsg->iIndex, lpMsg->iResult, lpMsg->bIsGiveUp, lpMsg->iRegMarkCount, lpMsg->szGuildName);

#endif
}

// [0x80][0x16] 수성측 NPC 제거 요청 응답
void GS_DGAnsNpcRemove				(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_NPCREMOVE lpMsg	= (LPCSP_ANS_NPCREMOVE) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x16] GS_DGAnsNpcRemove() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#endif
}

// [0x80][0x17] 현재의 공성상태 동기화 요청 응답
void GS_DGAnsCastleStateSync		(LPBYTE lpRecv)
{
	// 모든 서버들에 공통적으로 존재해야 함 !!

	LPCSP_ANS_CASTLESTATESYNC lpMsg	= (LPCSP_ANS_CASTLESTATESYNC) lpRecv;
	if (lpMsg == NULL)
		return;

	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
		return;

	// 여기서 성의 동기화 정보들을 관리객체에 세팅한다.
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	g_CastleSiegeSync.SetCastleState(lpMsg->iCastleState);
	g_CastleSiegeSync.SetTaxRateChaos(lpMsg->iTaxRateChaos);
	g_CastleSiegeSync.SetTaxRateStore(lpMsg->iTaxRateStore);
	g_CastleSiegeSync.SetTaxHuntZone(lpMsg->iTaxHuntZone);
	g_CastleSiegeSync.SetCastleOwnerGuild(lpMsg->szOwnerGuildName);
#endif
}

// [0x80][0x18] 맵서버군의 성 공납금 추가 요청 응답
void GS_DGAnsCastleTributeMoney		(LPBYTE lpRecv)
{
	// 모든 서버들에 공통적으로 존재해야 함 !!
	
	LPCSP_ANS_CASTLETRIBUTEMONEY lpMsg	= (LPCSP_ANS_CASTLETRIBUTEMONEY) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x18] GS_DGAnsCastleTributeMoney() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

#ifdef MU_CASTLESIEGE_TAX_SYSTEM_20041219		// 만약 공납이 성공했다면 현재까지 걷은 공납금을 리셋
#ifdef MU_CASTLESIEGE_SERVER_SYNC_20041130
	LogAddTD("[CastleSiege] [0x80][0x18] GS_DGAnsCastleTributeMoney() - Money Tribute OK (%d)", g_CastleSiegeSync.GetTributeMoney());

	g_CastleSiegeSync.ResetTributeMoney();
#endif
#endif
}

// [0x80][0x19] 맵서버군의 성 세율, 돈 초기화 요청 응답
void GS_DGAnsResetCastleTaxInfo		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_RESETCASTLETAXINFO lpMsg	= (LPCSP_ANS_RESETCASTLETAXINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x19] GS_DGAnsResetCastleTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		g_CastleSiege.ResetCastleTaxInfo();
	}

#endif
}

// [0x80][0x1A] 공성 참여길드 리스트 초기화 요청 응답
void GS_DGAnsResetSiegeGuildInfo	(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_RESETSIEGEGUILDINFO lpMsg	= (LPCSP_ANS_RESETSIEGEGUILDINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x1A] GS_DGAnsResetSiegeGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		// 공성 참여길드 리스트가 초기화 됨
	}
#endif
}

// [0x80][0x1B] 공성 참여신청 길드 리스트 초기화 요청 응답
void GS_DGAnsResetRegSiegeInfo	(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_RESETREGSIEGEINFO lpMsg	= (LPCSP_ANS_RESETREGSIEGEINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x80][0x1B] GS_DGAnsResetRegSiegeInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
	if (lpMsg->iResult == 1) {
		// 공성 참여신청 길드 리스트가 초기화 됨
	}
#endif
}

// [0x81] 공성전 최초 모든 초기화 데이터 요청 응답
void GS_DGAnsCastleInitData			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CSINITDATA lpMsg	= (LPCSP_ANS_CSINITDATA) lpRecv;
	LPCSP_CSINITDATA lpMsgBody	= (LPCSP_CSINITDATA) (lpRecv + sizeof(CSP_ANS_CSINITDATA));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x81] GS_DGAnsCastleInitData() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	if (g_CastleSiege.GetDataLoadState() != CASTLESIEGE_DATALOAD_2) {
		LogAddC(LOGC_RED,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)", g_CastleSiege.GetDataLoadState());
		return;
	}

	g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_3);

	if (lpMsg->iResult == 0) {
		LogAddC(LOGC_RED,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		MsgBox("[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		return;
	}
	BOOL bRET_VAL	= FALSE;
	bRET_VAL		= g_CastleSiege.SetCastleInitData(lpMsg);
	if (bRET_VAL == FALSE) {
		LogAddC(LOGC_RED,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleInitData() == FALSE");
		return;
	}
	bRET_VAL		= g_CastleSiege.SetCastleNpcData(lpMsgBody, lpMsg->iCount);
	if (bRET_VAL == FALSE) {
		LogAddC(LOGC_RED,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleNpcData() == FALSE");
		return;
	}
	
	// 데이터 세팅에 성공하였다 -> 공성전 스케쥴러를 계산하여 시작한다.
	g_CastleSiege.SetDbDataLoadOK(TRUE);
	g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_4);

	// 만약 최초로 만들어지는 것이라면 공성전 DB NPC들을 모두 살리고 DB에 집어넣는다.
	if (lpMsg->iFirstCreate == 1) {
		g_CastleSiege.FirstCreateDbNPC();
	}

	g_CastleSiege.Init();
	
#endif
}

// [0x82] 수성측 NPC 정보 요청 응답
void GS_DGAnsCastleNpcInfo			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_NPCDATA lpMsg	= (LPCSP_ANS_NPCDATA) lpRecv;
	LPCSP_NPCDATA lpMsgBody	= (LPCSP_NPCDATA) (lpRecv + sizeof(CSP_ANS_NPCDATA));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x82] GS_DGAnsCastleNpcInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif
	
#endif
}

// [0x83] 공성측 길드등록 정보 요청 응답 (길드 개수 N개 제한)
void GS_DGAnsAllGuildMarkRegInfo	(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_ALLGUILDREGINFO lpMsg	= (LPCSP_ANS_ALLGUILDREGINFO) lpRecv;
	LPCSP_GUILDREGINFO lpMsgBody	= (LPCSP_GUILDREGINFO) (lpRecv + sizeof(CSP_ANS_ALLGUILDREGINFO));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x83] GS_DGAnsAllGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	if (!gObjIsConnected(lpMsg->iIndex)) {
		return;
	}

	CHAR cBUFFER[sizeof(PMSG_ANS_CSREGGUILDLIST) + sizeof(PMSG_CSREGGUILDLIST) * 100 + 256];
	LPPMSG_ANS_CSREGGUILDLIST lpMsgSend		= (LPPMSG_ANS_CSREGGUILDLIST) cBUFFER;
	LPPMSG_CSREGGUILDLIST lpMsgSendBody		= (LPPMSG_CSREGGUILDLIST) (cBUFFER + sizeof(PMSG_ANS_CSREGGUILDLIST));
	
	lpMsgSend->iCount						= 0;
	lpMsgSend->btResult						= (BYTE) lpMsg->iResult;

	if (lpMsg->iResult == 1) {
		lpMsgSend->iCount					= lpMsg->iCount;
		for (INT iCNT = 0 ; iCNT < lpMsg->iCount ; iCNT++ ) {
			lpMsgSendBody[iCNT].btSeqNum	= lpMsgBody[iCNT].btRegRank;
			lpMsgSendBody[iCNT].btIsGiveUp	= lpMsgBody[iCNT].bIsGiveUp;
			lpMsgSendBody[iCNT].btRegMarks1	= HIBYTE(HIWORD(lpMsgBody[iCNT].iRegMarkCount));
			lpMsgSendBody[iCNT].btRegMarks2	= LOBYTE(HIWORD(lpMsgBody[iCNT].iRegMarkCount));
			lpMsgSendBody[iCNT].btRegMarks3	= HIBYTE(LOWORD(lpMsgBody[iCNT].iRegMarkCount));
			lpMsgSendBody[iCNT].btRegMarks4	= LOBYTE(LOWORD(lpMsgBody[iCNT].iRegMarkCount));
			memcpy (lpMsgSendBody[iCNT].szGuildName, lpMsgBody[iCNT].szGuildName, 8);
		}
	}
	else {
		
	}
	
	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB4, sizeof(PMSG_ANS_CSREGGUILDLIST) + sizeof(PMSG_CSREGGUILDLIST) * lpMsgSend->iCount);
	DataSend(lpMsg->iIndex, (LPBYTE)lpMsgSend, sizeof(PMSG_ANS_CSREGGUILDLIST) + sizeof(PMSG_CSREGGUILDLIST) * lpMsgSend->iCount);

#endif
}


// [0x84] 수성측 NPC 리스트 즉시 생성 요청 응답
void GS_DGAnsFirstCreateNPC			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_NPCSAVEDATA lpMsg	= (LPCSP_ANS_NPCSAVEDATA) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x84] GS_DGAnsFirstCreateNPC() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	if (lpMsg->iResult == 1) {
		// 성공했을 경우 NPC_LIVE_READY 상태의 DB NPC 들을 모두 생성해 준다.

		// 임시 !!! -> CCastleSiege::SetState() 에서 상태 변화시마다 하기 때문에 여기서 필요 없을 수도 있다.
		if (g_CastleSiege.GetDbNpcCreated() == FALSE) {
			g_CastleSiege.CreateDbNPC();
			g_CastleSiege.SetDbNpcCreated(TRUE);
		}
	}
	
#endif
}


// [0x85] 공성측 길드선정 후보자료 요청 응답
void GS_DGAnsCalcREgGuildList			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CALCREGGUILDLIST lpMsg	= (LPCSP_ANS_CALCREGGUILDLIST) lpRecv;
	LPCSP_CALCREGGUILDLIST lpMsgBody	= (LPCSP_CALCREGGUILDLIST) (lpRecv + sizeof(CSP_ANS_CALCREGGUILDLIST));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 공성측 최종길드 선정후보 데이터를 세팅한다.
	if (lpMsg->iResult == 1) {
		g_CastleSiege.SetCalcRegGuildList(lpMsgBody, lpMsg->iCount);
	}
	else {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->iResult != 1 (%d)", lpMsg->iResult);
	}

#endif
}


// [0x86] 공/수 길드의 연합정보를 요청 응답
void GS_DGAnsCsGulidUnionInfo			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CSGUILDUNIONINFO lpMsg	= (LPCSP_ANS_CSGUILDUNIONINFO) lpRecv;
	LPCSP_CSGUILDUNIONINFO lpMsgBody	= (LPCSP_CSGUILDUNIONINFO) (lpRecv + sizeof(CSP_ANS_CSGUILDUNIONINFO));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x86] GS_DGAnsCsGulidUnionInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 공/수 길드의 연합정보를 기존 공성길드 정보와 합쳐서 완전한 공성길드 정보를 만든다.
	if (lpMsg->iResult == 1) {
		g_CastleSiege.MakeCsTotalGuildInfo(lpMsgBody, lpMsg->iCount);
	}
	else {
		
	}

#endif
}


// [0x87] 공/수 길드의 최종정보를 저장 요청 응답
void GS_DGAnsCsSaveTotalGuildInfo		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111
	
	LPCSP_ANS_CSSAVETOTALGUILDINFO lpMsg	= (LPCSP_ANS_CSSAVETOTALGUILDINFO) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x87] GS_DGAnsCsSaveTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 공/수 길드의 전체정보 저장에 성공했다.
	if (lpMsg->iResult == 1) {
		g_CastleSiege.SetIsSiegeGuildList(TRUE);
	}
	else {
		g_CastleSiege.SetIsSiegeGuildList(FALSE);
	}

#endif
}


// [0x88] 공/수 길드의 최종정보를 불러오기 요청 응답
void GS_DGAnsCsLoadTotalGuildInfo		(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_CSLOADTOTALGUILDINFO lpMsg	= (LPCSP_ANS_CSLOADTOTALGUILDINFO) lpRecv;
	LPCSP_CSLOADTOTALGUILDINFO lpMsgBody	= (LPCSP_CSLOADTOTALGUILDINFO) (lpRecv + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x88] GS_DGAnsCsLoadTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	// 공/수 길드의 전체정보 불러오기에 성공했다.
	if (lpMsg->iResult == 1) {
		g_CastleSiege.SetCsTotalGuildInfo(lpMsgBody, lpMsg->iCount);
	}
	else {
		
	}

#endif
}


// [0x89] 수성측 NPC 리스트 정보갱신 요청 응답
void GS_DGAnsCastleNpcUpdate			(LPBYTE lpRecv)
{
#ifdef CASTLE_MAIN_SCHEDULER_20041111

	LPCSP_ANS_NPCUPDATEDATA lpMsg	= (LPCSP_ANS_NPCUPDATEDATA) lpRecv;
	if (lpMsg == NULL)
		return;

#ifdef MAP_SERVER_WORK_20041030		// DS -> GS 패킷 교환 시 검증용
	if (lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()) {
		LogAddC(LOGC_RED,"[CastleSiege] PACKET-ERROR [0x89] GS_DGAnsCastleNpcUpdate() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
#endif

	LogAddTD("[CastleSiege] [0x89] GS_DGAnsCastleNpcUpdate() - Npc Data Update Result : '%s'", (lpMsg->iResult)?"SUCCEED":"FAIL");

#endif
}


#endif // MU_CASTLESIEGE_DS_PROTOCOL_20041105


#ifdef MU_CRYWOLF_DS_PROTOCOL_20050505

// GD [0xB0] 크라이울프 현재 상태 동기화 요청
void GDReqCrywolfSync( int iMapSvrGroup, int iCrywolfState, int iOccupationState )
{
	if (iMapSvrGroup < 0)
		return;

	CWP_REQ_CRYWOLFSYNC pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB0, sizeof(CWP_REQ_CRYWOLFSYNC));
	pMsg.wMapSvrNum				= (WORD) iMapSvrGroup;
	pMsg.iCrywolfState			= iCrywolfState;
	pMsg.iOccupationState		= iOccupationState;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// GD [0xB0] 크라이울프 현재 상태 동기화 요청 응답
void DGAnsCrywolfSync( LPBYTE lpRecv )
{
	LPCWP_ANS_CRYWOLFSYNC lpMsg	= (LPCWP_ANS_CRYWOLFSYNC) lpRecv;
	
	if (lpMsg == NULL)
		return;

	int iOldOccupationState = g_CrywolfSync.GetOccupationState();

	g_CrywolfSync.SetCrywolfState( lpMsg->iCrywolfState );
	g_CrywolfSync.SetOccupationState( lpMsg->iOccupationState );

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	if( GetTickCount() > dwCrywolfSyncLogTickCount + ( 60 * 1000 ) )
	{
		LogAddC( LOGC_BLUE, "[ CRYWOLF ] SYNC Occupation: %d, State: %d", lpMsg->iOccupationState, lpMsg->iCrywolfState );
		dwCrywolfSyncLogTickCount = GetTickCount();
	}
#else
	LogAddC( LOGC_BLUE, "[ CRYWOLF ] SYNC Occupation: %d, State: %d", lpMsg->iOccupationState, lpMsg->iCrywolfState );
#endif

}

// GD [0xB1] 크라이울프 정보 요청
void GDReqCrywolfInfoLoad( int iMapSvrGroup )
{
	if (iMapSvrGroup < 0)
		return;

	CWP_REQ_CRYWOLFINFOLOAD pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB1, sizeof(CWP_REQ_CRYWOLFINFOLOAD));
	pMsg.wMapSvrNum	= (WORD) iMapSvrGroup;
		
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// GD [0xB1] 크라이울프 정보 요청 응답
void DGAnsCrywolfInfoLoad( LPBYTE lpRecv)
{
	LPCWP_ANS_CRYWOLFINFOLOAD lpMsg	= (LPCWP_ANS_CRYWOLFINFOLOAD) lpRecv;
	
	if (lpMsg == NULL)
		return;
	

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	// 점령 상태만 사용한다.
	if( lpMsg->btResult = 1
		&& lpMsg->iOccupationState >= CRYWOLF_OCCUPATION_STATE_PEACE
		&& lpMsg->iOccupationState <= CRYWOLF_OCCUPATION_STATE_WAR )
	{
		g_Crywolf.ApplyCrywolfDBInfo( lpMsg->iCrywolfState, lpMsg->iOccupationState );
	}
	else
	{
		LogAddC( LOGC_RED, "[ Crywolf ] Incorrect DB OccupationState!!!" );
	}
	
#endif

}

// GD [0xB2] 크라이울프 정보 저장 요청
void GDReqCrywolfInfoSave( int iMapSvrGroup, int iCrywolfState, int iOccupationState  )
{
	if (iMapSvrGroup < 0)
		return;

	CWP_REQ_CRYWOLFINFOSAVE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, sizeof(CWP_REQ_CRYWOLFINFOSAVE));
	pMsg.wMapSvrNum			= (WORD) iMapSvrGroup;
	
	pMsg.iCrywolfState		= iCrywolfState;
	pMsg.iOccupationState	= iOccupationState;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

// GD [0xB2] 크라이울프 정보 저장 요청 응답
void DGAnsCrywolfInfoSave( LPBYTE lpRecv)
{
	LPCWP_ANS_CRYWOLFINFOSAVE lpMsg	= (LPCWP_ANS_CRYWOLFINFOSAVE) lpRecv;
	
	if (lpMsg == NULL)
		return;
}


#endif // MU_CRYWOLF_DS_PROTOCOL_20050505

#ifdef UPDATE_CHANGE_CHARACTERNAME_20080410
//----------------------------------------------------------------------------
// DG[0x16] 캐릭터 변경을 요청 결과.
//----------------------------------------------------------------------------
void DGChangeNameResult(LPSDHP_CHANGE_NAME_RESULT lpMsg)
{
	char szAccountId[MAX_IDSTRING+1];
	
	int	 aIndex = lpMsg->aIndex;
	BYTE btResult = lpMsg->btResult;
	
	szAccountId[MAX_IDSTRING] = '\0';
	
	memcpy(szAccountId, lpMsg->AccountId, MAX_IDSTRING);
	if( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(424),szAccountId);
		CloseClient ( aIndex );
		return;
	}

	PMSG_CHAGNE_NAME_RESULT pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x16, sizeof(pResult));
	pResult.btResult = lpMsg->btResult;	
	memcpy(pResult.OldName, lpMsg->OldName, MAX_IDSTRING);
	memcpy(pResult.NewName, lpMsg->NewName, MAX_IDSTRING);
	
	DataSend( aIndex, (LPBYTE)&pResult, pResult.h.size);

	if( btResult == 0 )
	{
		char szTemp[MAX_IDSTRING+1], szTemp2[MAX_IDSTRING+1];
		szTemp[MAX_IDSTRING] = '\0';
		szTemp2[MAX_IDSTRING] = '\0';
		memcpy(szTemp, lpMsg->NewName, MAX_IDSTRING);
		memcpy(szTemp2, lpMsg->OldName, MAX_IDSTRING);
		
		LogAddTD("Character Name Change : %s,%s", szTemp2, szTemp);
	}
}
#endif	// UPDATE_CHANGE_CHARACTERNAME_20080410

#ifdef ADD_CHARCARD_SUMMONER_20080313
void JGGetAccountInfo(LPSDHP_ANS_ACCOUNTINFO lpMsg)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountId, MAX_IDSTRING);
	
	int aIndex = lpMsg->Number;
	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(421),gObj[aIndex].AccountID, szId);
		CloseClient ( aIndex );
		return;
	}
	
	{
		//소환술사 캐릭터카드 구매 유/무 정보를 설정한다.
		gObj[aIndex].m_bCharCardSummoner = lpMsg->bSummoner;
	}
	
	{
		PMSG_ANS_ACCOUNTINFO res;
		res.h.set((LPBYTE)&res, 0xDE, sizeof(res));
		res.subcode = 0x00;
		res.bSummoner = lpMsg->bSummoner;
		DataSend(aIndex, reinterpret_cast<LPBYTE>(&res), sizeof(res));
	}
}

void DataServerSetAccountInfo(LPOBJECTSTRUCT lpObj, int aIndex)
{
	SDHP_REQ_SETACCOUNTINFO res;
	res.h.set(reinterpret_cast<LPBYTE>(&res), 0xA2, sizeof(res));
	memset(res.AccountId, 0, MAX_IDSTRING+1);
	memcpy(res.AccountId, lpObj->AccountID, MAX_IDSTRING);
	res.Number = lpObj->m_Index;
	cDBSMng.Send(reinterpret_cast<char*>(&res), sizeof(res));
}

void JGSetAccountInfo(LPSDHP_ANS_SETACCOUNTINFO lpMsg)
{
	char szId[MAX_IDSTRING+1];
	szId[MAX_IDSTRING] = '\0';
	memcpy(szId, lpMsg->AccountId, MAX_IDSTRING);
	
	int aIndex = lpMsg->Number;
	if( gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(LOGC_RED, lMsg.Get(421),gObj[aIndex].AccountID, szId);
		CloseClient ( aIndex );
		return;
	}
	
	if (lpMsg->Result == 0x01)
	{
		//소환술사 캐릭터카드 구매 정보를 설정한다.
		gObj[aIndex].m_bCharCardSummoner = TRUE;
	}
	
	{
		PMSG_ANS_SETACCOUNTINFO res;
		res.h.set((LPBYTE)&res, 0xDE, sizeof(res));
		res.subcode = 0x01;
		res.Result = lpMsg->Result;
		DataSend(aIndex, reinterpret_cast<LPBYTE>(&res), sizeof(res));
	}
}
#endif // ADD_CHARCARD_SUMMONER_20080313