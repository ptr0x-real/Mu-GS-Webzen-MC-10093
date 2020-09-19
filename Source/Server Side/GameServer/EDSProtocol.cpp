
//##EXDB##
//----------------------------------------------------------------------------
// GAME SERVER - EDSProtocol.CPP
// ExDB �������� ������ �ְ�޴� �������� ó�� ���
//----------------------------------------------------------------------------
#include "stdafx.h"
#include <CommCtrl.h>
#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "User.h"
#include "Protocol.h"
#include "GameServer.h"
#include "wsGameServer.h"
#include "wsJoinServerCli.h"
#include "GameMain.h"
#include "GuildClass.h"
#include "DBSockMng.h"
#include "SimpleModulus.h"
#include "giocp.h"
#include "DbSave.h"
#include "EDSProtocol.h"

#ifdef  UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
#include "CastleSiegeSync.h"
#endif	// UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217

#ifdef _NEW_EXDB_


#ifdef NEW_SKILL_FORSKYLAND
#include "QuestInfo.h"
#endif

#ifdef ADD_NEW_UNION_01_20041006
#include "TUnion.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �о�� �������� ������ �� ��/�� �� ����������� ���� ������ ���´�.
#include "CastleSiege.h"
#endif


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
extern wsJoinServerCli	wsExDbCli;

//----------------------------------------------------------------------------
// F U N C T I O N 
//----------------------------------------------------------------------------
void GDGuildMemberDelResult(LPSDHP_GUILDMEMBERDEL_RESULT lpMsg);
void GDGuildMemberAddResult(LPSDHP_GUILDMEMBERADD_RESULT lpMsg);
void GDGuildDestroyRecv(LPSDHP_DEFAULT_RESULT lpMsg);
void GDGuildCreateRecv(LPSDHP_GUILDCREATE_RESULT lpMsg);
void DGGuildMemberInfo(LPSDHP_GUILDMEMBER_INFO lpMsg);
void GDGuildDestroyRecv(LPSDHP_GUILDDESTROY_RESULT lpMsg);
void DGGuildList(LPSDHP_GUILDCREATED lpMsg);
void DGGuildMasterListRecv(LPBYTE lpData);
void GDGuildScoreUpdateRecv(LPSDHP_GUILDSCOREUPDATE lpMsg);
void DGGuildNoticeRecv(LPSDHP_GUILDNOTICE lpMsg);
void DGGuildListState(LPSDHP_GUILDLISTSTATE lpMsg);
void GDCharCloseRecv(LPSDHP_USERCLOSE lpMsg);

// ��帮��Ʈ�������µ� ���Ǵ� ������..
HWND GuildListhDlg=NULL;
HWND GuildListhPrs=NULL;
HWND GuildListhSts=NULL;
int  gGuildListCount=0;
int  gGuildListTotal=0;



#ifdef __FRIEND_WORK__
void FriendListResult(LPBYTE lpMsg);
void FriendAddResult(LPFHP_FRIEND_ADD_RESULT lpMsg);
void WaitFriendListResult(LPFHP_WAITFRIENDLIST_COUNT lpMsg);
void FriendDelResult(LPFHP_FRIEND_DEL_RESULT lpMsg);
void WaitFriendAddResult(LPFHP_WAITFRIEND_ADD_RESULT lpMsg);
void FriendMemoList(LPFHP_FRIEND_MEMO_LIST lpMsg);
void FriendMemoSendResult(LPFHP_FRIEND_MEMO_SEND_RESULT lpMsg);
void FriendMemoRead(LPFHP_FRIEND_MEMO_RECV lpMsg);
void FriendMemoDelResult(LPFHP_FRIEND_MEMO_DEL_RESULT lpMsg);
void FriendChatRoomCreateResult(LPFHP_FRIEND_CHATROOM_CREATE_RESULT	lpMsg);
void FriendStateRecv(LPFHP_FRIEND_STATE lpMsg);
void FriendRoomInvitationRecv(LPFHP_FRIEND_INVITATION_RET lpMsg);
void ExDataServerLoginResult(LPSDHP_RESULT lpMsg);
#endif

//----------------------------------------------------------------------------
void ExDataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen)
{
	switch( protoNum )
	{
	case 0x00 :
#ifdef __FRIEND_WORK__
		ExDataServerLoginResult((LPSDHP_RESULT)aRecv);
#endif
		break;
		//#BUGFIX_20040203_01
		// DSProtocol.cpp �� �ִ°��� �������� ����
	case 0x02 :	
		GDCharCloseRecv((LPSDHP_USERCLOSE)aRecv);
		break;
		//#BUGFIX_20040203_01
	case 0x30 :
		GDGuildCreateRecv((LPSDHP_GUILDCREATE_RESULT)aRecv);
		break;
	case 0x31 :
		GDGuildDestroyRecv((LPSDHP_GUILDDESTROY_RESULT)aRecv);
		break;
	case 0x32 :
		GDGuildMemberAddResult((LPSDHP_GUILDMEMBERADD_RESULT)aRecv);
		break;
	case 0x33 :
		GDGuildMemberDelResult((LPSDHP_GUILDMEMBERDEL_RESULT)aRecv);
		break;
	case 0x35 :	// ���ӿ� ������ ĳ������ ��� ������ ����
		DGGuildMemberInfo((LPSDHP_GUILDMEMBER_INFO)aRecv);
		break;
	case 0x36 :
		DGGuildMasterListRecv(aRecv);
		break;
	case 0x37 :	// ��� ���ھ ������Ʈ �Ǿ���.
		GDGuildScoreUpdateRecv((LPSDHP_GUILDSCOREUPDATE)aRecv);
		break;
	case 0x38 :
		DGGuildNoticeRecv((LPSDHP_GUILDNOTICE)aRecv);
		break;
	case 0x40 :
		DGGuildList((LPSDHP_GUILDCREATED)aRecv);
		break;
	case 0x41 : // ��帮��Ʈ �ޱ� ���� ��������
		DGGuildListState((LPSDHP_GUILDLISTSTATE)aRecv);
		break;

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
	case 0x50 : // �����׷� ��� ä��
		DGGuildServerGroupChattingRecv((LPEXSDHP_SERVERGROUP_GUILD_CHATTING_RECV)aRecv);
		break;

	case 0x51 : // �����׷� ���� ä��
		DGUnionServerGroupChattingRecv((LPEXSDHP_SERVERGROUP_UNION_CHATTING_RECV)aRecv);
		break;
#endif

#ifdef ADD_NEW_GUILD_01_20040913
	case 0xE1 :	// ��� ��å �Ӹ� ��������
		DGGuildAnsAssignStatus((LPEXSDHP_GUILD_ASSIGN_STATUS_RESULT)aRecv);
		break;
#endif

#ifdef ADD_NEW_GUILD_02_20040922
	case 0xE2 : // ��� Type ���� ��������
		DGGuildAnsAssignType((LPEXSDHP_GUILD_ASSIGN_TYPE_RESULT)aRecv);
		break;
#endif

#ifdef ADD_NEW_UNION_01_20041006
	case 0xE5 : // ��� ���� ����
		DGRelationShipAnsJoin((LPEXSDHP_RELATIONSHIP_JOIN_RESULT)aRecv);
		break;

	case 0xE6 : // ��� ���� ����
		DGRelationShipAnsBreakOff((LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT)aRecv);
		break;
	case 0xE7 : // ��� ���� ����Ʈ	 �ޱ�
		DGRelationShipListRecv((LPEXSDHP_UNION_RELATIONSHIP_LIST)aRecv);
		break;
	case 0xE8 : // ���� �޼����� �޴´�.
		DGRelationShipNotificationRecv((LPEXSDHP_NOTIFICATION_RELATIONSHIP)aRecv);
		break;

	case 0xE9 :
		// Union ������ �޴´�.
		DGUnionListRecv((LPBYTE)aRecv);
		break;
#endif

#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
	case 0xEB :
		{
			LPPMSG_DEFAULT2 lpDef = (LPPMSG_DEFAULT2)aRecv;
			switch(lpDef->subcode) 
			{
			// ���� �����Ͱ� Ư�� ���� ����� ����Ż�� ��Ų��.
			case 0x01 :	
				DGRelationShipAnsKickOutUnionMember( (LPEXSDHP_KICKOUT_UNIONMEMBER_RESULT)aRecv );
				break;
			}

		}
		break;


#endif




#ifdef __FRIEND_WORK__
	case 0x60 :
		FriendListResult(aRecv);
		break;
	case 0x61 :
		WaitFriendListResult((LPFHP_WAITFRIENDLIST_COUNT)aRecv);
		break;
	case 0x62 :
		FriendStateRecv((LPFHP_FRIEND_STATE)aRecv);
		break;
	case 0x63 :
		FriendAddResult((LPFHP_FRIEND_ADD_RESULT)aRecv);
		break;
	case 0x64 :
		WaitFriendAddResult((LPFHP_WAITFRIEND_ADD_RESULT)aRecv);
		break;
	case 0x65 :
		FriendDelResult((LPFHP_FRIEND_DEL_RESULT)aRecv);
		break;
	case 0x66 :
		FriendChatRoomCreateResult((LPFHP_FRIEND_CHATROOM_CREATE_RESULT)aRecv);
		break;
	case 0x70 :
		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)aRecv);
		break;
	case 0x71 :
		FriendMemoList((LPFHP_FRIEND_MEMO_LIST)aRecv);
		break;
	case 0x72 :
		FriendMemoRead((LPFHP_FRIEND_MEMO_RECV)aRecv);
		break;
	case 0x73 :
		FriendMemoDelResult((LPFHP_FRIEND_MEMO_DEL_RESULT)aRecv);
		break;
	case 0x74 : 
		FriendRoomInvitationRecv((LPFHP_FRIEND_INVITATION_RET)aRecv);
		break;
#endif

	}	
}

//----------------------------------------------------------------------------
// [0x00] ���������� �����Ҷ� �� ���α׷��� ������ ������.
//----------------------------------------------------------------------------
void ExDataServerLogin()
{
	SDHP_SERVERINFO	   pInfo;

	pInfo.h.c		 = PMHC_BYTE;
	pInfo.h.size	 = sizeof(pInfo);
	pInfo.h.headcode = 0x00;
	pInfo.Port		 = GameServerPort;
	pInfo.Type		 = ST_GAMESERVER;

	pInfo.ServerCode = gGameServerCode;

	strcpy( pInfo.ServerName, szServerName);

	wsExDbCli.DataSend((char*)&pInfo, pInfo.h.size);
}


//----------------------------------------------------------------------------
// [0x02] ���� ���� ����.. ��� ����
// Type �߰�
//	 0 �̸� ���� ģ�� ��� ���� ó��
//	 1 �̸� ģ���� ���� ó�� 
//----------------------------------------------------------------------------
void GDCharClose(int type, char *GuildName, char *Name)
{
	SDHP_USERCLOSE	pMsg;

	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.size		= sizeof(pMsg);
	pMsg.h.headcode = 0x02;

#ifdef __FRIEND_WORK__
	//#BUGFIX_20040218_01
	pMsg.Type		= type;
#endif

	//#BUGFIX_20040218_01
	
	memcpy( pMsg.CharName, Name, MAX_IDSTRING);
	
	memset( pMsg.GuildName, 0, MAX_GUILDNAMESTRING);	
	if( GuildName!=NULL )
		memcpy( pMsg.GuildName, GuildName, MAX_GUILDNAMESTRING);	
	
	//#BUGFIX_20040203_01
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//#BUGFIX_20040218_01
//void GDCharCloseFriend() �Լ� ����
//#BUGFIX_20040218_01

#ifdef __FRIEND_WORK__
//----------------------------------------------------------------------------
// [0x00] Ex ������ ������ ������ ���
//----------------------------------------------------------------------------
void ExDataServerLoginResult(LPSDHP_RESULT lpMsg)
{
	if( lpMsg->Result == FALSE )
	{
		LogAddTD("ExDB Server Login Fail: %d", lpMsg->Result);
	}
	else
	{
		for( int n=0; n<MAX_OBJECT; n++)
		{
			if( gObjIsConnectedGP(n)==TRUE)
			{
				if( gObj[n].m_FriendServerOnline == FRIENDSERVER_OFFLINE ) 
					gObj[n].m_FriendServerOnline = FRIENDSERVER_ONLINE;	// ���� �ϰ� �ض�..
			}
		}
	}
}

#endif

//----------------------------------------------------------------------------
// [0x02] ������ ������ �����ߴ�. ��� ���� �ؾ� �Ѵ�.
//----------------------------------------------------------------------------
void GDCharCloseRecv(LPSDHP_USERCLOSE lpMsg)
{
	char szName[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	memset(szName, 0, MAX_IDSTRING+1);
	memset(szGuildName, 0, MAX_GUILDNAMESTRING+1);

	memcpy(szName, lpMsg->CharName, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	Guild.CloseMember(szGuildName, szName);
}

//----------------------------------------------------------------------------
// [0x30] ��带 ������û
//----------------------------------------------------------------------------
#ifdef ADD_NEW_GUILD_02_20040922
void GDGuildCreateSend(int aIndex, char* Name, char * Master, LPBYTE Mark, INT iGuildType)
#else
void GDGuildCreateSend(int aIndex, char* Name, char * Master, LPBYTE Mark)
#endif
{
	SDHP_GUILDCREATE pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x30;
	pMsg.h.size			= sizeof(pMsg);

	memcpy(pMsg.Mark, Mark, 32);
	
	pMsg.Master[MAX_IDSTRING] = '\0';
	pMsg.GuildName[MAX_GUILDNAMESTRING] = '\0';
	memcpy(pMsg.Master, Master, MAX_IDSTRING);
	memcpy(pMsg.GuildName, Name, MAX_GUILDNAMESTRING);

	pMsg.NumberH = HIBYTE(aIndex);
	pMsg.NumberL = LOBYTE(aIndex);

#ifdef ADD_NEW_GUILD_02_20040922
	pMsg.btGuildType = iGuildType;
#elif GAME_VERSION >= G_V_99B
	pMsg.btGuildType = 0;
#endif

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x30] ��带 ���� ��û ���
// ��� ���� ��û�� ���� ����� ������ �������� �޾Ƽ� ����ڿ��� �ٽ� �˷��ش�. 
//----------------------------------------------------------------------------
void GDGuildCreateRecv(LPSDHP_GUILDCREATE_RESULT lpMsg)
{
	int aIndex=-1;
	PMSG_GUILDCREATED_RESULT	pMsg;
	LPGUILD_INFO lpNode;

	char szMaster[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	szMaster[MAX_IDSTRING] = '\0';
	szGuildName[MAX_GUILDNAMESTRING]='\0';

	memcpy(szMaster, lpMsg->Master, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	
	if( lpMsg->Result == 0x00 )
	{
		aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		PMSG_GUILDCREATED_RESULT	pMsg;
		
		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof( pMsg ));

		pMsg.Result		= 0x00;

#ifdef ADD_NEW_GUILD_02_20040922
		pMsg.btGuildType = lpMsg->btGuildType;
#endif

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == I_GUILDCREATE )
		{
			gObj[aIndex].m_IfState.use    = 0;
		}
		return;
	}

		
	if( lpMsg->Flag == 0x01 )	// �츮 ������ ����ھ�?
	{
		aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		if( gObjIsConnected(aIndex) == TRUE ) 
		{
			if( szMaster[0] == gObj[aIndex].Name[0] )
			{
				if( strcmp(szMaster, gObj[aIndex].Name ) == 0 ) 
				{
					pMsg.h.c		= PMHC_BYTE;
					pMsg.h.headcode	= 0x56;
					pMsg.h.size		= sizeof( pMsg );
					pMsg.Result		= lpMsg->Result;
					
#ifdef ADD_NEW_GUILD_02_20040922
					pMsg.btGuildType = lpMsg->btGuildType;
#endif
					
					DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
	if( lpMsg->Result == 0x01 )
	{	
		lpNode = Guild.AddGuild(lpMsg->GuildNumber, szGuildName, lpMsg->Mark, szMaster, 0);

		if( aIndex >= 0 && lpNode )
		{
			gObj[aIndex].lpGuild     = lpNode;

#ifdef ADD_NEW_GUILD_01_20040913
			// �� 1 �̾�����..;
			gObj[aIndex].GuildNumber = lpMsg->GuildNumber;
			gObj[aIndex].GuildStatus = G_MASTER;
#else
			gObj[aIndex].GuildNumber = 1;
#endif

#ifdef ADD_NEW_UNION_01_20041006
			lpNode->iGuildRival		 = G_RELATIONSHIP_NONE;
			lpNode->iGuildUnion		 = G_RELATIONSHIP_NONE;
			lpNode->iTimeStamp		 = 0;
			gObj[aIndex].iGuildUnionTimeStamp = 0;
			
			LogAddTD("[U.System] Guild is Created - Guild : %s / [%s][%s]", szGuildName, gObj[aIndex].AccountID, gObj[aIndex].Name );
#endif

			LogAdd(lMsg.Get(433),lpNode->WarState, lpNode->WarDeclareState, szGuildName);

#ifdef GUILD_VIEWPORT_MODIFY
			GCGuildViewportNowPaint(aIndex, szGuildName, lpMsg->Mark, 1);
#else
			GCGuildViewportNowPaint(aIndex, szGuildName, lpMsg->Mark);
#endif
		}
	}
	
	

	//LogAdd("��� ����Ʈ ���� : GuildNumber:%d GuildName:%s Master:%s", lpMsg->GuildNumber, szGuildName, szMaster);
}

//----------------------------------------------------------------------------
// [0x31] ��� ����
//----------------------------------------------------------------------------
void GDGuildDestroySend(int aIndex, char* Name, char* Master)
{
	SDHP_GUILDDESTROY	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x31;
	pMsg.h.size			= sizeof(pMsg);
	pMsg.NumberH		= HIBYTE(aIndex);
	pMsg.NumberL		= LOBYTE(aIndex);

	memcpy(pMsg.Master,    Master, MAX_IDSTRING);
	memcpy(pMsg.GuildName, Name  , MAX_GUILDNAMESTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x31] ��� ������û ��� : �� ���� ���� ����.
//----------------------------------------------------------------------------

// Union�� �߰� �Ǹ鼭  �� �Լ��� ���� �Ͽ���.
#ifndef ADD_NEW_UNION_01_20041006
	void GDGuildDestroyRecv(LPSDHP_GUILDDESTROY_RESULT lpMsg)
	{
		int aIndex = -1;
		char szMaster[MAX_IDSTRING+1];
		char szGuildName[MAX_GUILDNAMESTRING+1];

		szMaster[MAX_IDSTRING] = '\0';
		szGuildName[MAX_GUILDNAMESTRING]='\0';

		memcpy(szMaster, lpMsg->Master, MAX_IDSTRING);
		memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

		if( lpMsg->Flag == 0x01 )	// �츮 ������ ����ھ�?
		{
			aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
			if( gObjIsConnected(aIndex) == TRUE ) 
			{
				if( szMaster[0] == gObj[aIndex].Name[0] )
				{
					if( strcmp(szMaster, gObj[aIndex].Name ) == 0 ) 
					{
						GCResultSend(aIndex, 0x53, lpMsg->Result);

	#ifdef ADD_NEW_UNION_01_20041006
						if( lpMsg->Result == 0x01 || lpMsg->Result == 0x04 )
	#endif	

	#ifdef GUILD_VIEWPORT_MODIFY
						GCGuildViewportDelNow(aIndex, 1);
	#else
						GCGuildViewportDelNow(aIndex);
	#endif
						//gObjGuildWarEndSend(&gObj[aIndex], 0x05, 0x04);
					}
				}
			}
		}
		if( lpMsg->Result == 0x01 || lpMsg->Result == 0x04)
		{

	#ifdef ADD_NEW_UNION_01_20041006
			LPGUILD_INFO lpGuildInfo  = Guild.SearchGuild(szGuildName);
	#endif

			for( int n=0; n<MAX_OBJECT; n++)
			{
				if( gObj[n].Connected > 2 )
				{
					if( gObj[n].GuildNumber > 0 )
					{					
						if( gObj[n].lpGuild->Name[0] == szGuildName[0] &&
							gObj[n].lpGuild->Name[1] == szGuildName[1])
						{
							if( strcmp(gObj[n].lpGuild->Name, szGuildName ) == 0 )
							{
								gObj[n].lpGuild     = NULL;
								gObj[n].GuildNumber = 0;

	#ifdef ADD_NEW_GUILD_01_20040913
								gObj[n].GuildStatus = G_NONE;
	#endif

	#ifdef ADD_NEW_UNION_01_20041006
								gObj[n].iGuildUnionTimeStamp = 0;
	#endif
								
								GCResultSend(n, 0x53, 0x01);

	#ifdef GUILD_VIEWPORT_MODIFY
								GCGuildViewportDelNow(n, 0);
	#else
								GCGuildViewportDelNow(n);
	#endif
								LogAdd(lMsg.Get(434), gObj[n].Name);
							}
						}
					}
				}
			}

	#ifdef ADD_NEW_UNION_01_20041006
			UnionManager.DelUnion( lpGuildInfo->Number );
	#endif
			Guild.DeleteGuild(szGuildName, szMaster);
			LogAdd(lMsg.Get(435), szGuildName, szMaster);
		}
	}
#endif // 'N' DEF : ADD_NEW_UNION_01_20041006


//----------------------------------------------------------------------------
// [0x31] ��� ������û ��� : �� ���� �Ǿ���.
//----------------------------------------------------------------------------
#ifdef ADD_NEW_UNION_01_20041006
void GDGuildDestroyRecv(LPSDHP_GUILDDESTROY_RESULT lpMsg)
{
	int aIndex = -1;
	char szMaster[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	szMaster[MAX_IDSTRING]	= '\0';
	szGuildName[MAX_GUILDNAMESTRING]='\0';

	memcpy(szMaster, lpMsg->Master, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	if( lpMsg->Flag == 0x01 )	// �츮 ������ ����ھ�?
	{
		aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		
		if( gObjIsConnected(aIndex) == TRUE ) 
		{
			if( szMaster[0] == gObj[aIndex].Name[0] )
			{
				if( strcmp(szMaster, gObj[aIndex].Name ) == 0 ) 
				{
					GCResultSend(aIndex, 0x53, lpMsg->Result);

					// ������ ��쿡��.
					if( lpMsg->Result == 0x01 || lpMsg->Result == 0x04 )
						GCGuildViewportDelNow(aIndex, 1);
				}
			}
		}
	}

	if( lpMsg->Result == 0x01 || lpMsg->Result == 0x04)
	{
		LPGUILD_INFO lpGuildInfo  = Guild.SearchGuild(szGuildName);

		if( lpGuildInfo == NULL )
		{
			// ���� 
			return;
		}

		// ���� ��� ���� �ʱ�ȭ.
		LPGUILD_INFO lpRivalGuild = Guild.SearchGuild_Number( lpGuildInfo->iGuildRival );
		
		if( lpRivalGuild != NULL )
		{
			lpRivalGuild->iGuildRival = G_RELATIONSHIP_NONE;
			lpRivalGuild->szGuildRivalName[0] = '\0';
		}

		lpGuildInfo->iGuildUnion = G_RELATIONSHIP_NONE;
		lpGuildInfo->iGuildRival = G_RELATIONSHIP_NONE;
		
		for( int n=0; n<MAX_GUILD; n++)
		{
			INT iGuildMemberIndex = lpGuildInfo->Index[n];

			if( (lpGuildInfo->Use[n] > 0) && ( iGuildMemberIndex != -1) )
			{
				LPOBJECTSTRUCT lpObj = &gObj[iGuildMemberIndex];
					
				if( lpObj == NULL )	continue;
				if( !gObjIsConnected(iGuildMemberIndex) ) continue;

				gObjNotifyUpdateUnionV1(lpObj);
				gObjNotifyUpdateUnionV2(lpObj);
				
				// ������ ��� ���� �ʱ�ȭ.
				lpObj->lpGuild				= NULL;
				lpObj->GuildNumber			= 0;
				lpObj->GuildStatus			= G_NONE;
				lpObj->iGuildUnionTimeStamp = 0;

				GCResultSend(n, 0x53, 0x01);
				GCGuildViewportDelNow(lpObj->m_Index, 0);

				LogAddTD(lMsg.Get(434), gObj[n].Name);
			}
		}

		UnionManager.DelUnion( lpGuildInfo->Number );

		Guild.DeleteGuild(szGuildName, szMaster);
		LogAdd(lMsg.Get(435), szGuildName, szMaster);
	}

}

#endif // ADD_NEW_UNION_01_20041006



//----------------------------------------------------------------------------
// [0x32] ��� ��� �߰�
//----------------------------------------------------------------------------
void GDGuildMemberAdd(int aIndex, char *Name, char* MemberId)
{
	SDHP_GUILDMEMBERADD	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x32;
	pMsg.h.size			= sizeof(pMsg);

	pMsg.NumberH		= HIBYTE( aIndex );
	pMsg.NumberL		= LOBYTE( aIndex );

	pMsg.MemberID[MAX_IDSTRING] = '\0';
	pMsg.GuildName[MAX_GUILDNAMESTRING] = '\0';

	memcpy(pMsg.MemberID, MemberId, MAX_IDSTRING);
	memcpy(pMsg.GuildName,    Name, MAX_GUILDNAMESTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x32] ��� ��� �߰� ��û ���
//----------------------------------------------------------------------------
void GDGuildMemberAddResult(LPSDHP_GUILDMEMBERADD_RESULT lpMsg)
{
	LPGUILD_INFO	lpNode;
	int				aIndex = -1;
	int				HereUserIndex=-1;
	
	char szMember[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	szMember[MAX_IDSTRING] = '\0';
	szGuildName[MAX_GUILDNAMESTRING]='\0';

	memcpy(szMember, lpMsg->MemberID, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);

	if( lpMsg->Flag == 0x01 )	// �츮 ������ ����ھ�?
	{
		aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		if( gObjIsConnected(aIndex) == TRUE ) 
		{
			if( strcmp(szMember, gObj[aIndex].Name ) == 0 ) 
			{
				GCResultSend(aIndex, 0x51, lpMsg->Result);
				HereUserIndex = aIndex;				
			}			
		}
	}
	if( lpMsg->Result == 0x01 )
	{		

#ifdef ADD_NEW_GUILD_01_20040913
		lpNode = Guild.AddMember(szGuildName, szMember, HereUserIndex, -1, G_PERSON, lpMsg->pServer);
#else
		lpNode = Guild.AddMember(szGuildName, szMember, HereUserIndex, -1, lpMsg->pServer);
#endif
		
		if( aIndex >= 0 && lpNode )
		{
			gObj[aIndex].lpGuild     = lpNode;
			
#ifdef ADD_NEW_GUILD_01_20040913
			// ù ���Զ��� G_PERSON
			gObj[aIndex].GuildStatus = G_PERSON; // ��� ��å �� ����	
			gObj[aIndex].GuildNumber = lpNode->Number;
#else
			gObj[aIndex].GuildNumber = 1;
#endif

#ifdef ADD_NEW_UNION_01_20041006
			gObj[aIndex].iGuildUnionTimeStamp = 0;
#endif
			
#ifdef GUILD_VIEWPORT_MODIFY
			GCGuildViewportNowPaint(aIndex, szGuildName, gObj[aIndex].lpGuild->Mark, 0);
#else
			GCGuildViewportNowPaint(aIndex, szGuildName, gObj[aIndex].lpGuild->Mark);
#endif

#ifdef MODIFY_CASTLESIEGE_STATENOTIFY_JOINGUILD_BUGFIX
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
			// ���������� �������� �Ⱓ�� ������带 ������ ��� ���������ڷ� �����Ѵ�.
			if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
			{
				INT nState = g_CastleSiege.GetCastleState();
				switch (nState)
				{
					case CASTLESIEGE_STATE_NOTIFY:
					case CASTLESIEGE_STATE_READYSIEGE:
						{
							szGuildName[MAX_GUILDNAMESTRING] = '\0';
							g_CastleSiege.GetCsJoinSide(szGuildName, gObj[aIndex].m_btCsJoinSide, gObj[aIndex].m_bCsGuildInvolved);
							g_CastleSiege.NotifySelfCsJoinSide(aIndex);
						}
						break;
				}
			}
#endif // GAMESERVER_TO_CASTLESIEGESERVER
#endif // MODIFY_CASTLESIEGE_STATENOTIFY_JOINGUILD_BUGFIX

#ifdef ADD_NEWPVP_PKFIELD
			{
				// �������Խ� ���� ȭ�鳻�� �ִ� ������ ���� ����Ʈ�� �����Ѵ�.
				// Ŭ���̾�Ʈ���� ���� �������� �ν��ϱ����� ó��
				LPOBJECTSTRUCT lpObj = &gObj[aIndex];
				LPOBJECTSTRUCT lpTargetObj = NULL;

				// ����Ʈ �󿡼� ���� �����ִ� ������� ã�´�.
				// ���� �����ִ� ���� ����Ʈ�� �����Ѵ�.
				INT	tObjNum	= -1;
				for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
				{
					tObjNum = lpObj->VpPlayer2[i].number;
					if (tObjNum < 0)
						continue;

					lpTargetObj = &gObj[tObjNum];
					if(lpTargetObj->Type == OBJTYPE_CHARACTER && gObjIsConnected(lpTargetObj->m_Index))
					{
						if (lpTargetObj->lpGuild && lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number)
						{
							gObjClearViewport(lpTargetObj);
							gObjViewportListCreate(lpTargetObj->m_Index);
							gObjViewportListProtocolCreate(lpTargetObj);
						}
					}
				}
				gObjClearViewport(lpObj);
				gObjViewportListCreate(lpObj->m_Index);
				gObjViewportListProtocolCreate(lpObj);
			}
#endif // ADD_NEWPVP_PKFIELD
		}
	}
}

//----------------------------------------------------------------------------
// [0x33] ����� ���� 
//----------------------------------------------------------------------------
void GDGuildMemberDel(int aIndex, char *Name, char * MemberId)
{
	SDHP_GUILDMEMBERDEL	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x33;
	pMsg.h.size			= sizeof(pMsg);
	pMsg.NumberH		= HIBYTE( aIndex );
	pMsg.NumberL		= LOBYTE( aIndex );

	memcpy(pMsg.MemberID, MemberId, MAX_IDSTRING);
	memcpy(pMsg.GuildName,Name, MAX_GUILDNAMESTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x33] ����� ���� ��û ���
//----------------------------------------------------------------------------
void GDGuildMemberDelResult(LPSDHP_GUILDMEMBERDEL_RESULT lpMsg)
{
	int aIndex = -1;
	char szMember[MAX_IDSTRING+1];
	char szGuildName[MAX_GUILDNAMESTRING+1];

	szMember[MAX_IDSTRING] = '\0';
	szGuildName[MAX_GUILDNAMESTRING]='\0';

	memcpy(szMember, lpMsg->MemberID, MAX_IDSTRING);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);
	
	if( lpMsg->Flag == 0x01 )	// �츮 ������ ����ھ�?
	{
		aIndex = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		if( gObjIsConnected(aIndex) == TRUE ) 
		{
			if( strcmp(szMember, gObj[aIndex].Name ) == 0 ) 
			{
				GCResultSend(aIndex, 0x53, lpMsg->Result);
				LogAdd(lMsg.Get(436), gObj[aIndex].Name);
			}
			else GCResultSend(aIndex, 0x53, 0x05);			// "������ Ż����׽��ϴ�."
		}
		else {
			LogAdd(lMsg.Get(437), gObj[aIndex].Name);
		}
	}
	if( lpMsg->Result == 0x01 )
	{	
		int n=0;
		
		while(1)
		{
			if( gObj[n].Connected > 2 )
			{
				if( gObj[n].GuildNumber > 0 )
				{					
					if( gObj[n].Name[0] == szMember[0] &&
						gObj[n].Name[1] == szMember[1])
					{
						if( strcmp(gObj[n].Name, szMember) == 0 )
						{
#ifdef GUILD_VIEWPORT_MODIFY
							GCGuildViewportDelNow(n, 0);
#else
							GCGuildViewportDelNow(n);
#endif
							gObj[n].lpGuild     = NULL;
							gObj[n].GuildNumber = 0;

#ifdef ADD_NEW_GUILD_01_20040913
							gObj[n].GuildStatus = G_NONE; // ��� ��å �� ����	
#endif

#ifdef ADD_NEW_UNION_01_20041006
							gObjNotifyUpdateUnionV1(&gObj[n]);
#endif
							LogAdd(lMsg.Get(438),aIndex, gObj[n].Name);
							break;
						}
					}
				}
			}
			if( n < MAX_OBJECT-1 ) n++;
			else break;
		}
		Guild.DelMember(szGuildName, szMember);
	}
	else
	{
		LogAdd(lMsg.Get(439), szGuildName, lpMsg->Result);
	}
}

//----------------------------------------------------------------------------
// [0x34] ��� ���� ���� 
//----------------------------------------------------------------------------
void GDGuildUpdate(char *Name, char *Master, LPBYTE Mark, int score, BYTE count)
{
	SDHP_GUILDUPDATE	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x34;
	pMsg.h.size			= sizeof(pMsg);

	memcpy(pMsg.GuildName,  Name, MAX_GUILDNAMESTRING);
	memcpy(pMsg.Mark,		Mark, 32);
	memcpy(pMsg.Master,   Master, MAX_IDSTRING);
	
	pMsg.Score = score;
	pMsg.Count = count;
	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGGuildUpdateRecv(LPSDHP_GUILDUPDATE lpMsg)
{
	
}

void DGGuildMemberInfoRequest(int aIndex)
{
	SDHP_GUILDMEMBER_INFO_REQUEST pMsg;


	pMsg.h.set((LPBYTE)&pMsg, 0x35, sizeof(pMsg));
	pMsg.NumberH = HIBYTE(aIndex);
	pMsg.NumberL = LOBYTE(aIndex);
	memcpy(pMsg.MemberID, gObj[aIndex].Name, MAX_IDSTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// 0x35 ĳ���� ���ӽÿ� ������ ������ ���� ��忡 ���ԵǾ� ������ ������ �޾ƿ´�.
void DGGuildMemberInfo(LPSDHP_GUILDMEMBER_INFO lpMsg)
{
	BYTE GuildUserBuf[256] = {0,};
	BYTE GuildInfoBuf[256] = {0,};
	
#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
#else
	PMSG_GUILDVIEWPORT_USER pGVUMsg;
	PMSG_GUILDVIEWPORT	pMsg;
#endif
	
	char szGuildName[MAX_GUILDNAMESTRING+1];
	char szName[MAX_IDSTRING+1];
	
	szGuildName[MAX_GUILDNAMESTRING] = '\0';
	szName[MAX_IDSTRING] = '\0';
	
	memcpy( szGuildName, lpMsg->GuildName, MAX_GUILDNAMESTRING);
	memcpy( szName     , lpMsg->MemberID, MAX_IDSTRING);

	int GuildInfoOfs = sizeof( PMSG_GUILDVIEWPORT_COUNT );
	int GuildUserOfs = sizeof( PMSG_GUILDVIEWPORT_USERCOUNT );		
	
	for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected > 2 )
		{
			if( gObj[n].Name[0] == lpMsg->MemberID[0] )
			{
				if( strcmp(lpMsg->MemberID, gObj[n].Name ) == 0 )
				{
					
#ifdef CASTLE_MAIN_SCHEDULER_20041111	// ������ �� ������ ���踦 ����Ʈ�� �����ش�.

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
//					gObjClearBuffEffect( &gObj[n], CLEAR_TYPE_CASTLESIEGE );
#else
					gObj[n].m_ViewSkillState						&= ~(STATE_JOIN_SIDE_DEFEND | STATE_JOIN_SIDE_ATTACK);

	#ifdef MODIFY_CASTLESIEGE_IDENTIFICATION_20051012
					DEL_VIEWSKILLSTATE( gObj[n].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_2);
					DEL_VIEWSKILLSTATE( gObj[n].m_ViewSkillState, STATE_JOIN_SIDE_ATTACK_3);
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

					// �о�� �������� ������ �� ��/�� �� ����������� ���� ������ ���´�.
					szGuildName[MAX_GUILDNAMESTRING] = '\0';
					g_CastleSiege.GetCsJoinSide(szGuildName, gObj[n].m_btCsJoinSide, gObj[n].m_bCsGuildInvolved);
					g_CastleSiege.NotifySelfCsJoinSide (n);
#endif
					strcpy(gObj[n].GuildName, szGuildName);
					gObj[n].lpGuild = Guild.SearchGuild(gObj[n].GuildName);

					if( gObj[n].lpGuild != NULL ) 
					{

#ifdef ADD_NEW_GUILD_01_20040913
						gObj[n].GuildStatus = lpMsg->btGuildStatus;					
#endif
						gObj[n].GuildNumber = gObj[n].lpGuild->Number;
						Guild.ConnectUser(gObj[n].lpGuild, lpMsg->GuildName, gObj[n].Name, n, lpMsg->pServer);
						
#ifdef UNICODE_MODE_20050718
						gObj[n].lpGuild->Notice[MAX_GUILDNOTICE-2] = '\0';
						gObj[n].lpGuild->Notice[MAX_GUILDNOTICE-1] = '\0';
						if( wcslen((WCHAR*)gObj[n].lpGuild->Notice) > 0 )// ����� ������ �ִٸ�..
#else
						if( strlen(gObj[n].lpGuild->Notice) > 0 )// ����� ������ �ִٸ�..
#endif
						{
#ifdef APPLY_TESTSERVER1
							if( gGuildNoticeStringCheck(gObj[n].lpGuild->Notice) )
							{
#ifdef UNICODE_MODE_20050718
								PMSG_NOTICE	pNotice;
								TNotice::MakeNoticeMsgUnicode( &pNotice, 2, gObj[n].lpGuild->Notice, MAX_GUILDNOTICE);
								DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
#else
								GCServerMsgStringSend(gObj[n].lpGuild->Notice, n, 2);							
#endif
							}						
#else
#ifdef UNICODE_MODE_20050718
							PMSG_NOTICE	pNotice;
							TNotice::MakeNoticeMsgUnicode( &pNotice, 2, gObj[n].lpGuild->Notice, MAX_GUILDNOTICE);
							DataSend(n, (LPBYTE)&pNotice, pNotice.h.size);
#else
							GCServerMsgStringSend(gObj[n].lpGuild->Notice, n, 2);
#endif
#endif
						}
				
#ifdef ADD_NEW_GUILD_01_20040913
						LogAddTD("[U.System][Guild Status][Info] Guild : %s / [%s][%s] / %d", szGuildName, gObj[n].AccountID, szName, gObj[n].GuildStatus );
						LogAddTD(lMsg.Get(440), gObj[n].AccountID, szName, szGuildName);
#else
						LogAddTD("[U.System][Guild Status][Info] Guild : %s / [%s][%s]", szGuildName, gObj[n].AccountID, szName );
						LogAddTD(lMsg.Get(440), gObj[n].AccountID, szName, szGuildName);
#endif

#ifdef MODIFY_GUILD_VIEWPORT_01_20040701
						
	#ifdef MODIFY_GUILD_VIEWPORT_02_20040830
						pMsg.GuildNumber= gObj[n].GuildNumber;
	#else
						pMsg.GNumberH	= HIBYTE(gObj[n].GuildNumber);
						pMsg.GNumberL	= LOBYTE(gObj[n].GuildNumber);
	#endif
						pMsg.NumberH	= HIBYTE(n)&0x7F;
						pMsg.NumberL	= LOBYTE(n);

						if(!strcmp(gObj[n].lpGuild->Names[0], gObj[n].Name))
						{	// ��� ������ �̸�
							pMsg.NumberH |= 0x80;
						}
	
						
	#ifdef ADD_NEW_GUILD_01_20040913
						// ����� ��å�� �����ش�.!
						pMsg.btGuildStatus = gObj[n].GuildStatus;
	#endif

	#ifdef ADD_NEW_GUILD_02_20040922
						if( gObj[n].lpGuild )
							pMsg.btGuildType = gObj[n].lpGuild->btGuildType;
						else
							pMsg.btGuildType = G_TYPE_NONE;
	#endif

	#ifdef UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217
						if (g_CastleSiegeSync.CheckCastleOwnerMember(gObj[n].m_Index) == TRUE ||
							g_CastleSiegeSync.CheckCastleOwnerUnionMember(gObj[n].m_Index) == TRUE)
						{
							pMsg.btCastleOwner = 0x01;
						}
						else
						{
							pMsg.btCastleOwner = 0x00;
						}						
	#endif	// UPDATE_CHECK_CASTLE_OWNERMEMBER_20090217

						memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
						GuildInfoOfs += sizeof( pMsg );

						PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;
						
						pGVCount.h.c		= PMHC_WORD;
						pGVCount.h.headcode = 0x65;
						pGVCount.h.sizeH	= HIBYTE( GuildInfoOfs );
						pGVCount.h.sizeL	= LOBYTE( GuildInfoOfs );
						pGVCount.Count		= 1;
						memcpy(GuildInfoBuf, &pGVCount, sizeof(pGVCount));
						DataSend(n, (LPBYTE)GuildInfoBuf, GuildInfoOfs);
						
#else //MODIFY_GUILD_VIEWPORT_01_20040701
						pMsg.NumberH = HIBYTE(gObj[n].GuildNumber);
						pMsg.NumberL = LOBYTE(gObj[n].GuildNumber);
						strcpy(pMsg.GuildName, lpMsg->GuildName);
						memcpy(pMsg.Mark, gObj[n].lpGuild->Mark, 32);
						memcpy(GuildInfoBuf+GuildInfoOfs, &pMsg, sizeof(pMsg));
						GuildInfoOfs += sizeof( pMsg );
						
	#ifdef GUILD_VIEWPORT_MODIFY
						pGVUMsg.NumberH = HIBYTE(n)&0x7F;
						pGVUMsg.NumberL = LOBYTE(n);

						if(!strcmp(gObj[n].lpGuild->Names[0], gObj[n].Name))
						{	// ��� ������ �̸�
							pGVUMsg.NumberH |= 0x80;
						}
	#else
						pGVUMsg.NumberH = HIBYTE(n);
						pGVUMsg.NumberL = LOBYTE(n);
	#endif
						
						pGVUMsg.GNumberH = HIBYTE(gObj[n].GuildNumber);
						pGVUMsg.GNumberL = LOBYTE(gObj[n].GuildNumber);
						memcpy(GuildUserBuf+GuildUserOfs, &pGVUMsg, sizeof(pGVUMsg));
						GuildUserOfs += sizeof(pGVUMsg);

						PMSG_GUILDVIEWPORT_COUNT pGVCount;
						
						pGVCount.h.c		= PMHC_WORD;
						pGVCount.h.headcode = 0x5A;
						pGVCount.h.sizeH	= HIBYTE( GuildInfoOfs );
						pGVCount.h.sizeL	= LOBYTE( GuildInfoOfs );
						pGVCount.Count		= 1;
						memcpy(GuildInfoBuf, &pGVCount, sizeof(pGVCount));
						DataSend(n, (LPBYTE)GuildInfoBuf, GuildInfoOfs);

						PMSG_GUILDVIEWPORT_USERCOUNT pGUCount;
						pGUCount.h.c		= PMHC_WORD;
						pGUCount.h.headcode = 0x5B;
						pGUCount.h.sizeH	= HIBYTE( GuildUserOfs );
						pGUCount.h.sizeL	= LOBYTE( GuildUserOfs );
						pGUCount.Count		= 1;
						memcpy(GuildUserBuf, &pGUCount, sizeof(pGUCount));
						DataSend(n, (LPBYTE)GuildUserBuf, GuildUserOfs);
						
#endif	//MODIFY_GUILD_VIEWPORT_01_20040701
						
						// ���� �� �̾��ٸ�..
						if( gObj[n].lpGuild->WarState )
						{	
							GCGuildWarScore(n);
							GCGuildWarDeclare(n, gObj[n].lpGuild->TargetGuildName);
						}
					}
					else
					{
#ifdef CASTLE_MAIN_SCHEDULER_20041111	// �о�� �������� ������ �� ��/�� �� ����������� ���� ������ ���´�.
						gObj[n].m_btCsJoinSide		= CS_SIEGESIDE_NONE;
						gObj[n].m_bCsGuildInvolved	= 0;
#endif
						LogAddTD(lMsg.Get(441));
					}
					return;
				}
			}
		}
	}

	// ��� ����� ���� ���¸� �����Ѵ�.
	Guild.SetServer(szGuildName, szName, lpMsg->pServer);
}

// 0x36 ��� ������ ���ӽ� ������ ������ ���� ���� �����Ѵ�.
// 
// --> *���� ��
//
// 0x36 ���ӽ� ExDB �����κ��� ������� ������ ������ ��� '�����ü����' �� �д´�.(���� �Ǿ� ����)
// ���Ͽ� ������ �� Ÿ�ֿ̹� �о�;� ��.
void DGGuildMasterListRecv(LPBYTE lpData)
{
	LPSDHP_GUILDALL_COUNT	lpCount;
	LPSDHP_GUILDALL			lpList;
	int						lOfs=sizeof(SDHP_GUILDALL_COUNT);

	char szGuildName[MAX_GUILDNAMESTRING+1];
	char szMasterName[MAX_IDSTRING+1];

	memset(szMasterName, 0, MAX_IDSTRING+1);
	memset(szGuildName, 0, MAX_GUILDNAMESTRING+1);

	lpCount = (LPSDHP_GUILDALL_COUNT)lpData;

	memcpy(szGuildName, lpCount->GuildName, MAX_GUILDNAMESTRING);
#ifdef GUILD_REWORK
	memcpy(szMasterName, lpCount->Master, MAX_IDSTRING);	// #BUGFIX_20040615_01
#endif

	szGuildName[MAX_GUILDNAMESTRING] = '\0';
	szMasterName[MAX_IDSTRING] = '\0';

	if( strlen(szGuildName) < 1 ) return;
	if( lpCount->Count < 1 ) return;

#ifdef GUILD_REWORK
	Guild.AddGuild(lpCount->Number, szGuildName, lpCount->Mark, szMasterName, lpCount->score);
#endif
	
#ifdef ADD_NEW_GUILD_02_20040922
	Guild.SetGuildType( szGuildName, lpCount->btGuildType );
#endif

#ifdef ADD_NEW_UNION_01_20041006
	LPGUILD_INFO lpGuild = Guild.SearchGuild_Number( lpCount->Number );
	if( lpGuild != NULL )
	{
		lpGuild->iGuildUnion = lpCount->iGuildUnion;
		lpGuild->iGuildRival = lpCount->iGuildRival;
		
		memcpy( lpGuild->szGuildRivalName, lpCount->szGuildRivalName, MAX_GUILDNAMESTRING );
	}
#endif

	for( int n=0; n<lpCount->Count; n++)
	{
		lpList = (LPSDHP_GUILDALL)(lpData+lOfs);
		memcpy( szMasterName, lpList->MemberID, MAX_IDSTRING);
#ifdef GUILD_REWORK

#ifdef ADD_NEW_GUILD_01_20040913
		if( Guild.AddMember(szGuildName, szMasterName, -1, (int)lpCount->Count, lpList->btGuildStatus, lpList->pServer) == NULL ) 
#else
		if( Guild.AddMember(szGuildName, szMasterName, -1, (int)lpCount->Count, lpList->pServer) == NULL ) 
#endif
		{
			// �߰��� �� ���ٸ� �����Ѵ�.
			return;
		}
#else
		Guild.AddMember(szGuildName, szMasterName, -1, (int)lpCount->Count, lpList->pServer);
#endif
		lOfs+= sizeof( SDHP_GUILDALL );
		//LogAdd("������ ���� ����Ʈ ���� : %s",lpList->MemberID);
	}
}

// 0x37 ��� ���ھ ������Ʈ ��Ų��.
void DGGuildScoreUpdate(char *guildname, int score)
{
	SDHP_GUILDSCOREUPDATE	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x37, sizeof( pMsg ));

	pMsg.Score		= score;
	strcpy(pMsg.GuildName, guildname);
	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	LogAdd(lMsg.Get(442), guildname, score);
}

// 0x37 : ��� ���ھ� ������ ������Ʈ �Ѵ�.
void GDGuildScoreUpdateRecv(LPSDHP_GUILDSCOREUPDATE lpMsg)
{
	LPGUILD_INFO lpNode;
	lpNode = Guild.SearchGuild(lpMsg->GuildName);

	if( lpNode == NULL ) return;

	lpNode->TotalScore = lpMsg->Score;
	LogAdd(lMsg.Get(443), lpMsg->GuildName, lpMsg->Score);
}

// [0x38] ��� ���� ����
void GDGuildNoticeSave(char *guild_name, char *guild_notice)
{
	SDHP_GUILDNOTICE	pMsg;
	int len;
	len = strlen( guild_notice );
#ifdef APPLY_TESTSERVER1	
	if( len < 1 )
	{
		guild_notice[0] = ' ';
		guild_notice[1] = 0;		
	}
#else
	if( len < 1 ) return;
#endif
	
	if( len > MAX_CHAT-2 ) return;	
	
	pMsg.h.c		= PMHC_BYTE;
	pMsg.h.headcode	= 0x38;
	pMsg.h.size		= sizeof( pMsg );
	strcpy(pMsg.GuildName, guild_name);

	SQLSyntexCheckConvert(guild_notice);

#ifdef UNICODE_MODE_20050718
	memcpy(pMsg.szGuildNotice, guild_notice, MAX_GUILDNOTICE-4);
#else
	strcpy(pMsg.szGuildNotice, guild_notice);
#endif
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// [0x38] ��� ���� ������Ʈ ����
void DGGuildNoticeRecv(LPSDHP_GUILDNOTICE lpMsg)
{
	LPGUILD_INFO lpNode;
	lpNode = Guild.SearchGuild(lpMsg->GuildName);

	if( lpNode == NULL ) return;

#ifdef UNICODE_MODE_20050718
	memcpy(lpNode->Notice, lpMsg->szGuildNotice, MAX_GUILDNOTICE);
#else
	strcpy(lpNode->Notice, lpMsg->szGuildNotice);
	LogAddTD(lMsg.Get(444), lpMsg->GuildName, lpMsg->szGuildNotice);
#endif

#ifdef APPLY_TESTSERVER1
	if( !gGuildNoticeStringCheck(lpNode->Notice) )
	{
		return;
	}
#endif

	for( int n=0; n<MAX_GUILD; n++)
	{
		if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
		{
#ifdef UNICODE_MODE_20050718
			PMSG_NOTICE	pNotice;
			TNotice::MakeNoticeMsgUnicode( &pNotice, 2, lpMsg->szGuildNotice, MAX_GUILDNOTICE);
			DataSend(lpNode->Index[n], (LPBYTE)&pNotice, pNotice.h.size);
#else
			GCServerMsgStringSend(lpMsg->szGuildNotice, lpNode->Index[n], 2);
#endif
		}
	}
}

// 0x40 ��� ����Ʈ�� ��û�Ѵ�.
void DGGuildListRequest()
{
	SDHP_GUILDLISTREQUEST	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// 0x40 ������ ������ ���� ��� ����Ʈ�� �޾� �߰��Ѵ�.
void DGGuildList(LPSDHP_GUILDCREATED lpMsg)
{
	char szTemp[256];

	Guild.AddGuild(lpMsg->Number, lpMsg->GuildName, lpMsg->Mark, lpMsg->Master, lpMsg->score);

	if( GuildListhDlg )
	{
		SendMessage(GuildListhPrs, PBM_SETPOS, gGuildListCount, 0);
		gGuildListCount++;	
		wsprintf(szTemp, "%8s [%d / %d]", lpMsg->GuildName, gGuildListCount, gGuildListTotal);
		SetDlgItemText(GuildListhDlg,IDC_LIST_STATE, szTemp);
	}
}

BOOL WINAPI GuildListDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) 
	{
	case WM_INITDIALOG :
		break;
	case WM_COMMAND :
		switch( wParam )
		{
		case IDCANCEL :
			EndDialog(GuildListhDlg, 0);
			GuildListhDlg = NULL;
			break;
		}
		break;
	}
	return FALSE;
}

// 0x41 ������ ������ ���� ��� ����Ʈ�� �������� ���°�
void DGGuildListState(LPSDHP_GUILDLISTSTATE lpMsg)
{
	switch(lpMsg->State)
	{
	case 0x00 :	// ����Ʈ �ޱ� ����
		if(!IsWindow(GuildListhDlg))
		{
			GuildListhDlg = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DLG_GUILDLISTSTATE), ghWnd, GuildListDlgProc);
			ShowWindow(GuildListhDlg, SW_SHOW);
			GuildListhPrs = GetDlgItem( GuildListhDlg, IDC_LIST_PROGRESS);
			SendMessage( GuildListhPrs, PBM_SETRANGE, 0, MAKELPARAM(0,lpMsg->Count));
			gGuildListTotal = lpMsg->Count;
			SendMessage( GuildListhPrs, PBM_SETSTEP, 1, 0);
			gGuildListCount = 0;
			GameServerInfoSendStop();
		}
		break;
	case 0x01 :	// ����Ʈ �ޱ� �Ϸ�
		if(IsWindow(GuildListhDlg))
		{
			EndDialog(GuildListhDlg, 0);
			GameServerInfoSendStart();
			GuildListhDlg = NULL;			
		}
		break;
	}
}

#ifdef GUILD_BUGFIX_20040204_01
//----------------------------------------------------------------------------
// [0x58] ��弭���� ��� ���� �� �̴�.
// ��� ��������� �˸���.
//----------------------------------------------------------------------------
void GDGuildServerOffline()
{
	PMSG_DEFAULT	pMsg;

	pMsg.h.c			= PMHC_BYTE;
	pMsg.h.headcode		= 0x58;
	pMsg.h.size			= sizeof(pMsg);

	for( int n=0; n<MAX_OBJECT; n++)
	{
		if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
		{
			DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}
#endif

//##EXDB##

#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
// [GD][0x50] �����׷� ���ä�� Send.
void GDGuildServerGroupChattingSend( int iGuildNum, LPPMSG_CHATDATA lpChatData )
{
	EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0x50, sizeof(pMsg));

	pMsg.iGuildNum = iGuildNum;
	CopyMemory( pMsg.szCharacterName, lpChatData->chatid, MAX_IDSTRING );
	CopyMemory( pMsg.szChattingMsg, lpChatData->chatmsg, MAX_CHAT );
	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

}
// [DG][0x50] �����׷� ���ä�� Recv.
void DGGuildServerGroupChattingRecv( LPEXSDHP_SERVERGROUP_GUILD_CHATTING_RECV lpMsg )
{
	PMSG_CHATDATA pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0x00, sizeof(pMsg));
	CopyMemory( pMsg.chatid, lpMsg->szCharacterName, MAX_IDSTRING );
	CopyMemory( pMsg.chatmsg, lpMsg->szChattingMsg, MAX_CHAT );

	LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number(lpMsg->iGuildNum);

	// 1. ��� ���� ���� Ȯ��
	if( lpGuildInfo == NULL )
		return;

	int iUserIndex = -1;

	if( lpGuildInfo->Count >= 0)
	{
		for( int i=0; i<MAX_GUILD; i++)
		{
			if( lpGuildInfo->Use[i] )
			{
				iUserIndex = lpGuildInfo->Index[i];
				
				if( iUserIndex >= 0 )
				{
					if( lpGuildInfo->Names[i][0] ==  gObj[iUserIndex].Name[0] )
					{
						if( strcmp(lpGuildInfo->Names[i], gObj[iUserIndex].Name) == 0 )
						{
							DataSend(iUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		} // for

	} // if
				
}

// [GD][0x51] �����׷� ����ä�� Send.
void GDUnionServerGroupChattingSend( int iUnionNum, LPPMSG_CHATDATA lpChatData )
{
	EXSDHP_SERVERGROUP_UNION_CHATTING_SEND pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0x51, sizeof(pMsg));

	pMsg.iUnionNum = iUnionNum;
	CopyMemory( pMsg.szCharacterName, lpChatData->chatid, MAX_IDSTRING );
	CopyMemory( pMsg.szChattingMsg, lpChatData->chatmsg, MAX_CHAT );

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

}
// [GD][0x51] �����׷� ����ä�� Recv.
void DGUnionServerGroupChattingRecv( LPEXSDHP_SERVERGROUP_UNION_CHATTING_RECV lpMsg )
{
	PMSG_CHATDATA pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0x00, sizeof(pMsg));
	CopyMemory( pMsg.chatid, lpMsg->szCharacterName, MAX_IDSTRING );
	CopyMemory( pMsg.chatmsg, lpMsg->szChattingMsg, MAX_CHAT );

	int iUnionNum = lpMsg->iUnionNum;

	// 1. ���� ���� ���� Ȯ��	
	INT iGuildCount = 0;
	INT iGuildList[MAX_RELATIONSHIP_COUNT] = { 0, };

	if( UnionManager.GetGuildUnionMemberList( iUnionNum, iGuildCount, iGuildList ) == 1 )
	{
		int iUserIndex = -1;

		for( INT i=0; i<iGuildCount; i++ )
		{
			LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);
			
			if( lpGuildInfo == NULL )
				continue;
			
			for( int n=0; n<MAX_GUILD; n++)
			{
				if( lpGuildInfo->Use[n] )
				{
					iUserIndex = lpGuildInfo->Index[n];
					
					if( iUserIndex >= 0 )
					{
						if( lpGuildInfo->Names[n][0] ==  gObj[iUserIndex].Name[0] )
						{
							if( strcmp(lpGuildInfo->Names[n], gObj[iUserIndex].Name) == 0 )
							{
								DataSend(iUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
							}
						} 
					}
				}
			} // for
		}// for
	}


}

#endif // ## ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706



#ifdef ADD_NEW_GUILD_01_20040913
// [GD][0xE1] Req.
void GDGuildReqAssignStatus( int aIndex, int iAssignType, char* szTagetName, int iGuildStatus )
{
	EXSDHP_GUILD_ASSIGN_STATUS_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));

	pMsg.btType			= iAssignType;
	pMsg.btGuildStatus	= iGuildStatus;
	pMsg.wUserIndex		= aIndex;

	pMsg.szGuildName[MAX_GUILDNAMESTRING] = '\0';
	pMsg.szTargetName[MAX_IDSTRING] = '\0';

	memcpy( pMsg.szGuildName, gObj[aIndex].lpGuild->Name, MAX_GUILDNAMESTRING );
	memcpy( pMsg.szTargetName, szTagetName, MAX_IDSTRING );

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

// [DG][0xE1] Ans.
void DGGuildAnsAssignStatus( LPEXSDHP_GUILD_ASSIGN_STATUS_RESULT lpMsg )
{
	// 0. > P2P ���� BroadCasting ���� ���� 
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		// 1. > ��� ���� ���� ( ����� ��å ���� )
		LPGUILD_INFO lpNode;
		lpNode = Guild.SearchGuild(lpMsg->szGuildName);
		
		if( lpNode == NULL ) lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ����
		PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg = {0,};

		pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));

		pMsg.btType = lpMsg->btType;
		pMsg.btResult = lpMsg->btResult;
		memcpy( pMsg.szTagetName, lpMsg->szTargetName, MAX_IDSTRING );
		
		DataSend( lpMsg->wUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;

		// 1.1> �ǽð� ���ŵ� ���⼭ �ȴ�.
		Guild.SetGuildMemberStatus( lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus );

		// �α� ���
		LogAddTD("[U.System][Guild Status][Assign] Guild : ( %s ) Member : ( %s ) Status : ( %d )",
			lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus );

		// 3. > �������� ���� �����鿡�� ��å ������ �˸�.
		for( int n=0; n<MAX_GUILD; n++)
		{
			if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
			{
				TNotice _Notice;

				if( lpMsg->btGuildStatus == G_SUB_MASTER )
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1720), lpMsg->szTargetName ); // "%s �� ��å�� �α�� �����ͷ� ���� �Ǿ����ϴ�."
				else if ( lpMsg->btGuildStatus == G_BATTLE_MASTER )
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1721), lpMsg->szTargetName ); // "%s �� ��å�� ��Ʋ �����ͷ� ���� �Ǿ����ϴ�."
				else
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1722), lpMsg->szTargetName ); // "%s �� ��å�� �������� ���� �Ǿ����ϴ�."
			}
		}
	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{
		// 1. > ��� ���� ���� ( ����� ��å ���� )
		LPGUILD_INFO lpNode;
		lpNode = Guild.SearchGuild(lpMsg->szGuildName);
		
		if( lpNode == NULL ) return;

		Guild.SetGuildMemberStatus( lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus );

		// �α� ���
		LogAddTD("[U.System][Guild Status][Assign] Guild : ( %s ) Member : ( %s ) Status : ( %d )",
			lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus );

		// 2.1> ���� �� Return
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;

		// 3. > �������� ���� �����鿡�� ��å ������ �˸�.
		for( int n=0; n<MAX_GUILD; n++)
		{
			if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
			{
				TNotice _Notice;
				
				if( lpMsg->btGuildStatus == G_SUB_MASTER )
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1720), lpMsg->szTargetName ); // "%s �� ��å�� �α�� �����ͷ� ���� �Ǿ����ϴ�."
				else if ( lpMsg->btGuildStatus == G_BATTLE_MASTER )
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1721), lpMsg->szTargetName ); // "%s �� ��å�� ��Ʋ �����ͷ� ���� �Ǿ����ϴ�."
				else
					_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1722), lpMsg->szTargetName ); // "%s �� ��å�� �������� ���� �Ǿ����ϴ�."

			}
		}
	}
}


#endif
// ## ADD_NEW_GUILD_01_20040913

#ifdef ADD_NEW_GUILD_02_20040922
// [GD][0xE2] Req.
void GDGuildReqAssignType( int aIndex, int iGuildType )
{
	EXSDHP_GUILD_ASSIGN_TYPE_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));

	pMsg.btGuildType	= iGuildType;
	pMsg.wUserIndex		= aIndex;

	pMsg.szGuildName[MAX_GUILDNAMESTRING] = '\0';

	memcpy( pMsg.szGuildName, gObj[aIndex].lpGuild->Name, MAX_GUILDNAMESTRING );

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}
// [DG][0xE2] Ans.
void DGGuildAnsAssignType( LPEXSDHP_GUILD_ASSIGN_TYPE_RESULT lpMsg)
{
	// 0. > P2P ���� BroadCasting ���� ���� 
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		// 1. > ��� ���� ���� ( ����� ��å ���� )
		LPGUILD_INFO lpNode;
		lpNode = Guild.SearchGuild(lpMsg->szGuildName);
		
		if( lpNode == NULL ) lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ����
		PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg = {0,};

		pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));

		pMsg.btGuildType = lpMsg->btGuildType;
		pMsg.btResult = lpMsg->btResult;
		
		DataSend( lpMsg->wUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;

		// 1.1> �ǽð� ���ŵ� ���⼭ �Ǿ� �Ѵ�.
		Guild.SetGuildType( lpMsg->szGuildName, lpMsg->btGuildType );
	
		// 3. > �������� ���� �����鿡�� ��� Ÿ���� ����Ǿ����� �˸�
		for( int n=0; n<MAX_GUILD; n++)
		{
			if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
			{
				TNotice _Notice;
				_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1723), lpMsg->szGuildName, lpMsg->btGuildType ); // "�츮 %s ��� TYPE�� %d�� ���� �Ǿ����ϴ�."
			}
		}
	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{
		// 1. > ��� ���� ���� ( ����� ��å ���� )
		LPGUILD_INFO lpNode;
		lpNode = Guild.SearchGuild(lpMsg->szGuildName);
		
		if( lpNode == NULL ) return;

		Guild.SetGuildType( lpMsg->szGuildName, lpMsg->btGuildType );

		// 2.1> ���� �� Return
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;

		// 3. > �������� ���� �����鿡�� ��å ������ �˸�.
		for( int n=0; n<MAX_GUILD; n++)
		{
			if( (lpNode->Use[n] > 0) && (lpNode->Index[n] >= 0) )
			{
				TNotice _Notice;
				_Notice.SendToUser( lpNode->Index[n], lMsg.Get(1723), lpMsg->szGuildName, lpMsg->btGuildType ); // "�츮 %s ��� TYPE�� %d�� ���� �Ǿ����ϴ�."
			}
		}
	}
}

#endif
// ## ADD_NEW_GUILD_02_20040922

#ifdef ADD_NEW_UNION_01_20041006

typedef struct {}__UNION_EDSPROTOCOL_CPP___;

// [0xE5] RelationShip �α�
void GDRelationShipReqJoin( int aIndex, int iTargetUserIndex, int iRelationShipType )
{
	LPGUILD_INFO lpGuildInfo		= gObj[aIndex].lpGuild;
	LPGUILD_INFO lpTargetGuildInfo	= gObj[iTargetUserIndex].lpGuild;

	if( lpGuildInfo==NULL || lpTargetGuildInfo==NULL )
		return;

	EXSDHP_RELATIONSHIP_JOIN_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));

	pMsg.wRequestUserIndex	= aIndex;
	pMsg.wTargetUserIndex	= iTargetUserIndex;
	pMsg.btRelationShipType = (BYTE)iRelationShipType;
	pMsg.iRequestGuildNum	= gObj[aIndex].lpGuild->Number;
	pMsg.iTargetGuildNum	= gObj[iTargetUserIndex].lpGuild->Number;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGRelationShipAnsJoin( LPEXSDHP_RELATIONSHIP_JOIN_RESULT lpMsg)
{
	LPGUILD_INFO lpReqGuild		= Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	LPGUILD_INFO lpTargetGuild	= Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);
		
	// 0. > P2P ���� BroadCasting ���� ���� 
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		if( lpReqGuild==NULL || lpTargetGuild==NULL ) 
			lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Request )
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg = {0,};
		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult			= lpMsg->btResult;
		pMsg.btRequestType		= G_RELATION_OPERATION_JOIN;
		pMsg.btRelationShipType = lpMsg->btRelationShipType;
		pMsg.btTargetUserIndexH = HIBYTE(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = LOBYTE(lpMsg->wTargetUserIndex);

		DataSend( lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Target )
		pMsg.btTargetUserIndexH = HIBYTE(lpMsg->wRequestUserIndex);
		pMsg.btTargetUserIndexL = LOBYTE(lpMsg->wRequestUserIndex);
		
		DataSend( lpMsg->wTargetUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return

#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{
		if( lpReqGuild==NULL || lpTargetGuild==NULL ) 
			return;

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif
		
	}

	// Update!
	if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{
		lpReqGuild->SetGuildUnion(lpTargetGuild->Number);
		lpTargetGuild->SetGuildUnion(lpTargetGuild->Number);

		LogAddTD("[U.System][Union][Join] ( Union Master Guild: %s ), (Union Member Guild: %s)",
			lpTargetGuild->Name, lpReqGuild->Name);

	}
	else
	if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		lpReqGuild->SetGuildRival(lpTargetGuild->Number);
		lpTargetGuild->SetGuildRival(lpReqGuild->Number);
		
		CopyMemory( lpReqGuild->szGuildRivalName, lpMsg->szTargetGuildName, MAX_GUILDNAMESTRING );
		CopyMemory( lpTargetGuild->szGuildRivalName, lpMsg->szRequestGuildName, MAX_GUILDNAMESTRING );

		LogAddTD("[U.System][Rival][Join] (  %s ) vs ( %s )", lpReqGuild->Name, lpTargetGuild->Name );
	}

	// 3. > �������� ���� �����鿡�� RelationShip ������ �˸�.
	int n;
	for( n=0; n<MAX_GUILD; n++)
	{
		if( (lpReqGuild->Use[n] > 0) && (lpReqGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				_Notice.SendToUser( lpReqGuild->Index[n], lMsg.Get(1701), lpTargetGuild->Name ); // "%s ���տ� �����մϴ�. "
			else if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				_Notice.SendToUser( lpReqGuild->Index[n], lMsg.Get(1702), lpTargetGuild->Name ); // %s �� ���� ���谡 �����ƽ��ϴ�. 
		}
	}
	
	// 3. > �������� ���� �����鿡�� RelationShip ������ �˸�.
	for( n=0; n<MAX_GUILD; n++)
	{
		if( (lpTargetGuild->Use[n] > 0) && (lpTargetGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				_Notice.SendToUser( lpTargetGuild->Index[n], lMsg.Get(1703), lpReqGuild->Name, lpTargetGuild->Name ); // "%s ��尡 %s ���տ� �����մϴ�. "
			else if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				_Notice.SendToUser( lpTargetGuild->Index[n], lMsg.Get(1702), lpReqGuild->Name ); // "%s �� ���� ���谡 �����ƽ��ϴ�. "
		}
	}
	
}


// [0xE6] RelationShip ����

// Union Ż��. �Ϲ����� �����̱� ������ ���� ó��.
void GDUnionBreakOff( int aIndex, int iUnionNumber )
{
	LPGUILD_INFO lpGuildInfo = gObj[aIndex].lpGuild;

	if( lpGuildInfo == NULL )	
		return;

	EXSDHP_RELATIONSHIP_BREAKOFF_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));

	pMsg.wRequestUserIndex	= aIndex;
	pMsg.wTargetUserIndex	= INVALID_USER_INDEX; // Invalid User Index. ������� �ʰڴٴ� �ǹ�.
	pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
	pMsg.iRequestGuildNum	= lpGuildInfo->Number;
	pMsg.iTargetGuildNum	= iUnionNumber;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);	
}

void DGUnionBreakOff( LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT lpMsg )
{
	// ��ȣ �����Ͽ� �̷������ ���� �ƴϴ�.
	LPGUILD_INFO lpReqGuild		= Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	LPGUILD_INFO lpTargetGuild	= Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);
		
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		// Target Guild �� ĳ�̿� ���� ���Ӽ����� ������ ���� �� �ִ�.
		if( lpReqGuild==NULL ) 
			lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Request )
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg = {0,};

		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult			= lpMsg->btResult;
		pMsg.btRequestType		= G_RELATION_OPERATION_BREAKOFF;
		pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
		pMsg.btTargetUserIndexH = HIBYTE(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = LOBYTE(lpMsg->wTargetUserIndex);
		
		DataSend( lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{
		if( lpReqGuild==NULL) 
			return;

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}


	// Update!
	lpReqGuild->SetGuildUnion( G_RELATIONSHIP_NONE );

	LogAddTD("[GS][U.System][Union][BreakOff] Request Union Member Guild: %s)", lpReqGuild->Name);

	// 3. > �������� ���� �����鿡�� RelationShip ������ �˸�.
	int n;
	for( n=0; n<MAX_GUILD; n++)
	{
		if( (lpReqGuild->Use[n] > 0) && (lpReqGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			_Notice.SendToUser( lpReqGuild->Index[n], lMsg.Get(1704) ); // "���տ��� Ż�� �մϴ�. "
		}
	}

	// 3. > �������� ���� �����鿡�� RelationShip ������ �˸�.
	
	if( lpTargetGuild == NULL )
		return;

	// Log ���
	LogAddTD("[GS][U.System][Union][BreakOff] ( Union Master Guild: %s ), (Union Member Guild: %s)",
			lpTargetGuild->Name, lpReqGuild->Name);

	for( n=0; n<MAX_GUILD; n++)
	{
		if( (lpTargetGuild->Use[n] > 0) && (lpTargetGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			_Notice.SendToUser( lpTargetGuild->Index[n], lMsg.Get(1705), lpReqGuild->Name, lpTargetGuild->Name ); // "%s ��尡 %s ���տ��� Ż�� �մϴ�. "
		}
	}
}

void GDRelationShipReqBreakOff( int aIndex, int iTargetUserIndex, int iRelationShipType )
{
	LPGUILD_INFO lpGuildInfo		= gObj[aIndex].lpGuild;
	LPGUILD_INFO lpTargetGuildInfo	= gObj[iTargetUserIndex].lpGuild;

	if( lpGuildInfo==NULL || lpTargetGuildInfo==NULL )
		return;

	EXSDHP_RELATIONSHIP_BREAKOFF_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));

	pMsg.wRequestUserIndex	= aIndex;
	pMsg.wTargetUserIndex	= iTargetUserIndex;
	pMsg.btRelationShipType = (BYTE)iRelationShipType;
	pMsg.iRequestGuildNum	= lpGuildInfo->Number;
	pMsg.iTargetGuildNum	= lpTargetGuildInfo->Number;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGRelationShipAnsBreakOff( LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT lpMsg )
{
	if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{
		DGUnionBreakOff(lpMsg);
		return;
	}

	LPGUILD_INFO lpReqGuild		= Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	LPGUILD_INFO lpTargetGuild	= Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);
		
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		if( lpReqGuild==NULL || lpTargetGuild==NULL ) 
			lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Request )
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg = {0,};

		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult			= lpMsg->btResult;
		pMsg.btRequestType		= G_RELATION_OPERATION_BREAKOFF;
		pMsg.btRelationShipType = lpMsg->btRelationShipType;
		pMsg.btTargetUserIndexH = HIBYTE(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = LOBYTE(lpMsg->wTargetUserIndex);
		
		if( lpMsg->wRequestUserIndex!=INVALID_USER_INDEX)
			DataSend( lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Target )
		pMsg.btTargetUserIndexH = HIBYTE(lpMsg->wRequestUserIndex);
		pMsg.btTargetUserIndexL = LOBYTE(lpMsg->wRequestUserIndex);
		
		if( lpMsg->wTargetUserIndex!=INVALID_USER_INDEX)
			DataSend( lpMsg->wTargetUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{

		if( lpReqGuild==NULL || lpTargetGuild==NULL ) 
			return;

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}

	// Update
	if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		// �׺� ó��..?
		lpReqGuild->SetGuildRival(G_RELATIONSHIP_NONE);
		lpTargetGuild->SetGuildRival(G_RELATIONSHIP_NONE);

		// ���� ��� ���� �ʱ�ȭ.
		lpReqGuild->szGuildRivalName[0] = '\0';
		lpTargetGuild->szGuildRivalName[0] = '\0';
	}

	// Log ���
	LogAddTD("[U.System][Rival][BreakOff] (  %s ) vs ( %s )", lpReqGuild->Name, lpTargetGuild->Name );

	// 3. > �������� ���� �����鿡�� RelationShip ������ �˸�.
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( (lpReqGuild->Use[n] > 0) && (lpReqGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				_Notice.SendToUser( lpReqGuild->Index[n], lMsg.Get(1706), lpTargetGuild->Name ); // "%s ���տ��� Ż�� �մϴ�. "
			else if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				_Notice.SendToUser( lpReqGuild->Index[n], lMsg.Get(1707), lpTargetGuild->Name ); // "%s �� ���� ���谡 ���� �ƽ��ϴ�. "
		}
	}
	
}

// [0xE8] Notification Message
void DGRelationShipNotificationRecv( LPEXSDHP_NOTIFICATION_RELATIONSHIP lpMsg )
{
	// 0x10 : ���Ͽ��� ��ü �ƴ�.
	// ���Ͽ� ����� �����ͱ�� �ۿ� ������ �ڵ����� ��ü �Ǹ鼭 Notification�� ������.
	if( lpMsg->btUpdateFlag == G_NOTIFICATION_UNION_BREAKOFF )
	{
		LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number( lpMsg->iGuildList[0] );
		
		if( lpGuildInfo != NULL )
		{
			lpGuildInfo->iGuildUnion = G_RELATIONSHIP_NONE;
			lpGuildInfo->SetTimeStamp();

			// �α� ���
			LogAddTD("[U.System][Union][Auto BreakOff] ( Union Master Guild: %s ), (Union Member Guild: %s)", lpGuildInfo->Name, lpGuildInfo->Name);

			for( int n=0; n<MAX_GUILD; n++)
			{
				if( (lpGuildInfo->Use[n] > 0) && (lpGuildInfo->Index[n] >= 0) )
				{
					TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
					_Notice.SendToUser( lpGuildInfo->Index[n], lMsg.Get(1708), lpGuildInfo->Name ); // "%s ������ ��ü �˴ϴ�. "
				}
			}

		}
	}

	// RelationShip ������ ���� �Ͽ� �����ϵ��� �Ѵ�.
	for( INT i=0; i< lpMsg->btGuildListCount; i++ )
	{
		LPGUILD_INFO lpGuildInfo = Guild.SearchGuild_Number( lpMsg->iGuildList[i] );

		if( lpGuildInfo != NULL )
			lpGuildInfo->SetTimeStamp();
	}

}

// [0xE7] RelationShip List
void DGRelationShipListRecv( LPEXSDHP_UNION_RELATIONSHIP_LIST lpMsg )
{
	// ��� ���� ���� Relation ������ ���۵ȴ�,.
	if( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
		UnionManager.AddUnion( lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName, lpMsg->pUnionMasterGuildMark );
#else
		UnionManager.AddUnion( lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName );
#endif
		
		UnionManager.SetGuildUnionMemberList( lpMsg->iUnionMasterGuildNumber, lpMsg->btRelationShipMemberCount, lpMsg->iRelationShipMember );
	}
	else if( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{

#ifdef ADD_UNION_GET_UNIONMARK_01_20041222
		UnionManager.AddUnion( lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName, lpMsg->pUnionMasterGuildMark );
#else
		UnionManager.AddUnion( lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName );
#endif

		UnionManager.SetGuildRivalMemberList( lpMsg->iUnionMasterGuildNumber, lpMsg->btRelationShipMemberCount, lpMsg->iRelationShipMember );
	}
}

// [0xE9] Union List
void GDUnionListSend( INT iUserIndex, INT iUnionMasterGuildNumber )
{
	EXSDHP_UNION_LIST_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xE9, sizeof(pMsg));

	pMsg.wRequestUserIndex		 = iUserIndex;
	pMsg.iUnionMasterGuildNumber = iUnionMasterGuildNumber;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGUnionListRecv( LPBYTE aRecv )
{
	LPEXSDHP_UNION_LIST_COUNT	lpRecvMsg = (LPEXSDHP_UNION_LIST_COUNT) aRecv;
	LPEXSDHP_UNION_LIST 	lpRecvMsgBody = (LPEXSDHP_UNION_LIST) ( aRecv + sizeof(EXSDHP_UNION_LIST_COUNT) );

	// ���Ͽ� ����Ʈ�� ���Ӽ����� ������.
	CHAR cBUFFER_V1 [MAX_BUFF_SIZE];
	
	ZeroMemory( cBUFFER_V1, MAX_BUFF_SIZE );

	INT iSize = MAKEWORD( lpRecvMsg->h.sizeL, lpRecvMsg->h.sizeH );
	INT iBodySize = iSize - sizeof( EXSDHP_UNION_LIST_COUNT );

	if( iBodySize < 0 )	
		return;

	if( iBodySize > MAX_BUFF_SIZE )
		return;

	if( lpRecvMsg->btCount < 0 || lpRecvMsg->btCount > MAX_RELATIONSHIP_COUNT )
		return;
	
	LPPMSG_UNIONLIST_COUNT		lpMsg	= (LPPMSG_UNIONLIST_COUNT) cBUFFER_V1;
	LPPMSG_UNIONLIST		lpMsgBody	= (LPPMSG_UNIONLIST) ( cBUFFER_V1 + sizeof(PMSG_UNIONLIST_COUNT) );

	lpMsg->btResult					= lpRecvMsg->btResult;
	lpMsg->btCount					= lpRecvMsg->btCount;
	lpMsg->btRivalMemberNum			= lpRecvMsg->btRivalMemberNum;
	lpMsg->btUnionMemberNum			= lpRecvMsg->btUnionMemberNum;

	for ( int i=0; i<lpMsg->btCount; i++ )
	{
		lpMsgBody[i].btMemberNum = lpRecvMsgBody[i].btMemberNum;
		CopyMemory( lpMsgBody[i].szGuildName, lpRecvMsgBody[i].szGuildName, MAX_GUILDNAMESTRING );
		CopyMemory( lpMsgBody[i].Mark, lpRecvMsgBody[i].Mark, MAX_GUILDMARKBUFFER );
	}

	PHeadSetW((LPBYTE) lpMsg, 0xE9, sizeof(EXSDHP_UNION_LIST_COUNT) + (sizeof(EXSDHP_UNION_LIST) * lpMsg->btCount));
	
	DataSend( lpRecvMsg->wRequestUserIndex, (LPBYTE) lpMsg, MAKEWORD(lpMsg->h.sizeL, lpMsg->h.sizeH) );
	
}
#endif


#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
// [0xEB][0x01] Kick Out UnionMember
void GDRelationShipReqKickOutUnionMember( int aIndex, char *szTargetGuildName )
{
	LPGUILD_INFO lpGuildInfo = gObj[aIndex].lpGuild;

	if( lpGuildInfo == NULL )	
		return;

	EXSDHP_KICKOUT_UNIONMEMBER_REQ pMsg = {0,};

	pMsg.h.set((LPBYTE)&pMsg, 0xEB, 0x01, sizeof(pMsg));

	pMsg.wRequestUserIndex	= aIndex;
	pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
	CopyMemory( pMsg.szUnionMasterGuildName, lpGuildInfo->Name, MAX_GUILDNAMESTRING );
	CopyMemory( pMsg.szUnionMemberGuildName, szTargetGuildName, MAX_GUILDNAMESTRING );

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);	
}


void DGRelationShipAnsKickOutUnionMember( LPEXSDHP_KICKOUT_UNIONMEMBER_RESULT lpMsg )
{
	// ��ȣ �����Ͽ� �̷������ ���� �ƴϴ�.
	LPGUILD_INFO lpUnionMasterGuild	= Guild.SearchGuild(lpMsg->szUnionMasterGuildName);
	LPGUILD_INFO lpUnionMemberGuild	= Guild.SearchGuild(lpMsg->szUnionMemberGuildName);
		
	if( lpMsg->btFlag == EX_SEND_P2P )
	{
		// Union Member Guild �� ĳ�̿� ���� ���Ӽ����� ������ ���� �� �ִ�.
		if( lpUnionMasterGuild==NULL ) 
			lpMsg->btResult = EX_RESULT_FAIL;

		// 2. > ��û Ŭ���̾�Ʈ�� ��� ���� ( Request )
		PMSG_KICKOUT_UNIONMEMBER_RESULT pMsg = {0,};

		pMsg.h.set((LPBYTE)&pMsg, 0xEB, 0x01, sizeof(pMsg));
		pMsg.btResult			= lpMsg->btResult;
		pMsg.btRequestType		= G_RELATION_OPERATION_BREAKOFF;
		pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
		
		DataSend( lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg) );

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}
	else if( lpMsg->btFlag == EX_SEND_BROADCASTING )
	{
		if( lpUnionMemberGuild==NULL) 
			return;

		// 2.1> ���� �� Return
#ifdef EXDB_APPLY_CASTLEUPDATE_01_20050111
		if( lpMsg->btResult == EX_RESULT_FAIL || lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE ) return;
#else
		if( lpMsg->btResult == EX_RESULT_FAIL ) return;
#endif

	}

	// 3. > ���� ������ �����鿡�� RelationShip ������ �˸�.
#ifdef MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529
	// �����ͱ�尡 ������ ��찡 ���� ���� �ֱ� ������ NULL �˻縦 �ϵ��� �Ѵ�. 
	if ( lpUnionMasterGuild != NULL )
	{
#endif // MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529
	for( int n=0; n<MAX_GUILD; n++)
	{
		if( (lpUnionMasterGuild->Use[n] > 0) && (lpUnionMasterGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			_Notice.SendToUser( lpUnionMasterGuild->Index[n], lMsg.Get(1705), lpMsg->szUnionMemberGuildName, lpMsg->szUnionMasterGuildName ); // "%s ��尡 %s ���տ��� Ż�� �մϴ�. "
		}
	}
#ifdef MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529
	}
#endif // MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529

	
	LogAddTD("[GS][U.System][Union][KickOut] ( Union Master Guild: %s ), (Union Member Guild: %s)", lpMsg->szUnionMasterGuildName, lpMsg->szUnionMemberGuildName );


	if( lpUnionMemberGuild == NULL )
		return;

	// Update!
	lpUnionMemberGuild->SetGuildUnion( G_RELATIONSHIP_NONE );

	// 3. > Ż�� �� ���ո�� �����鿡�� RelationShip ������ �˸�.
#ifdef MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529
	int n;  // ���� ����
#endif // MODIFY_UNIONGUILD_KICKOUT_BUGFIX_20060529
	for( n=0; n<MAX_GUILD; n++)
	{
		if( (lpUnionMemberGuild->Use[n] > 0) && (lpUnionMemberGuild->Index[n] >= 0) )
		{
			TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
			_Notice.SendToUser( lpUnionMemberGuild->Index[n], lMsg.Get(1704) ); // "���տ��� Ż�� �մϴ�. "
		}
	}

}

#endif






//##__FRIEND_WORK__
//----------------------------------------------------------------------------
// [0x60] Ŭ���̾�Ʈ�� ģ�� ����Ʈ�� ��û�Ѵ�.  (���� ����)
// Exdb�� ģ������Ʈ�� �޶�� ��û
//----------------------------------------------------------------------------
extern BOOL gObjIsConnectedGP(int aIndex, char* CharName);

#ifdef __FRIEND_WORK__
void FriendListRequest(int aIndex)
{
	FHP_FRIENDLIST_REQ	pMsg;

	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	if( gObj[aIndex].m_FriendServerOnline == FRIENDSERVER_ONLINEREQUEST )
	{		
		LogAddTD("error-L3 : [%s][%s] FriendServer Online Request ", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}
	if( gObj[aIndex].m_FriendServerOnline == FRIENDSERVER_OFFLINE )
	{		
		LogAddTD("error-L3 : [%s][%s] FriendServer Offline", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	pMsg.h.set((LPBYTE)&pMsg, 0x60, sizeof(pMsg));
	memcpy( pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	
	pMsg.Number  = aIndex;
	pMsg.pServer = (BYTE)gGameServerCode;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

	LogAdd("[%s] Friend List Request", gObj[aIndex].Name);

	gObj[aIndex].m_FriendServerOnline = FRIENDSERVER_ONLINEREQUEST;
}

//----------------------------------------------------------------------------
// [0x60] ģ�� ����Ʈ ��û ���
//----------------------------------------------------------------------------
void FriendListResult(LPBYTE lpMsg)
{
	LPFHP_FRIENDLIST_COUNT	lpCount;	
	LPFHP_FRIENDLIST		lpList;
	int						lOfs=sizeof(FHP_FRIENDLIST_COUNT);

	PMSG_FRIEND_LIST_COUNT	pCount;
	int						pOfs=sizeof(pCount);
	
	lpCount      = (LPFHP_FRIENDLIST_COUNT)lpMsg;
	lpList       = (LPFHP_FRIENDLIST)(lpMsg+lOfs);

	char_ID		Name(lpCount->Name);
	
	if( gObjIsConnectedGP(lpCount->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	gObj[lpCount->Number].m_FriendServerOnline = FRIENDSERVER_ONLINE;	// �¶��� ���·� �����.

	BYTE	TmpSendBuf[2000];

	pCount.Count = lpCount->Count;
	pCount.MemoCount = lpCount->MailCount;
	pCount.MailTotal = 50;

	if( pCount.Count )
	{		
		for( int n=0; n<pCount.Count; n++)
		{
			lpList = (LPFHP_FRIENDLIST)(lpMsg+lOfs);
			memcpy(TmpSendBuf+pOfs, lpList, sizeof(FHP_FRIENDLIST));
			lOfs += sizeof(FHP_FRIENDLIST);
			pOfs += sizeof(PMSG_FRIEND_LIST);
		}
	}
	pCount.h.set((LPBYTE)&pCount, 0xC0, pOfs);
	memcpy(TmpSendBuf, (LPBYTE)&pCount, sizeof(pCount));	
	DataSend(lpCount->Number, TmpSendBuf, pOfs);
	LogAdd("[%s] Friend List Count (%d) Send", gObj[lpCount->Number].Name, pCount.Count);
}

//----------------------------------------------------------------------------
// [0x61] ������� ģ�� ����Ʈ ��û ���
//----------------------------------------------------------------------------
void WaitFriendListResult(LPFHP_WAITFRIENDLIST_COUNT lpMsg)
{
	PMSG_FRIEND_ADD_SIN_REQ	 pMsg;	
	
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : (%s) Index %s %d ", Name.GetBuffer(), __FILE__, __LINE__);
		return;
	}

	if( Name.GetLength() < 1 )
		return;

	pMsg.h.set((LPBYTE)&pMsg, 0xC2, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
	LogAdd("[%s] WaitFriend List [%s]", gObj[lpMsg->Number].Name, Name.GetBuffer());
	
}

//----------------------------------------------------------------------------
// [0xC4] Ŭ���̾�Ʈ�� ģ�� ���� ����
//----------------------------------------------------------------------------
void FriendStateClientRecv(LPPMSG_FRIEND_STATE_C lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_STATE_C	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	pMsg.State = lpMsg->ChatVeto;
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x62] ģ�� ���� ����
//----------------------------------------------------------------------------
void FriendStateRecv(LPFHP_FRIEND_STATE lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : (%s) Index %s %d ", Name.GetBuffer(), __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_STATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC4, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);
	pMsg.State = lpMsg->State;

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));	
	LogAdd("[%s] Friend State (%d)", gObj[lpMsg->Number].Name, lpMsg->State);
}


//----------------------------------------------------------------------------
// [0x63] ģ�� ����� ��û�Ѵ�.
//----------------------------------------------------------------------------
void FriendAddRequest(LPPMSG_FRIEND_ADD_REQ lpMsg, int aIndex)
{		
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

#ifdef MODIFY_REQUESTNPC_HACK
	int number = gObjGetIndex(lpMsg->Name);
	if ( (number >= 0 && number <= MAX_OBJECT-1)
		&&gObj[number].Type != OBJTYPE_CHARACTER )
	{
		LogAddC(LOGC_RED,"[HACKTOOL] : NPC-FriendAddRequest npc:%d ip:%s account:%s name:%s State:%d", 
			gObj[number].Class, 
			gObj[aIndex].Ip_addr,
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name, 
			gObj[aIndex].Connected);
		
		CloseClient(aIndex);
		return;
	}
#endif // MODIFY_REQUESTNPC_HACK
	
	FHP_FRIEND_ADD_REQ	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
	memcpy( pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	memcpy( pMsg.FriendName, lpMsg->Name, MAX_IDSTRING);	// ����� ģ���̸�
	
	pMsg.Number = aIndex;
	
	char_ID		szFriendName(lpMsg->Name);
	LogAdd("[%s] Friend add request Name: [%s]", gObj[aIndex].Name, szFriendName.GetBuffer());
	if( strcmp(gObj[aIndex].Name, szFriendName.GetBuffer()) == 0 )
	{
		LogAdd("error-L3 : [%s] Friend add : Not self", gObj[aIndex].Name);
		PMSG_FRIEND_ADD_RESULT	pResult;
		
		pResult.h.set((LPBYTE)&pResult, 0xC1, sizeof(pResult));
		memcpy( pResult.Name, lpMsg->Name, MAX_IDSTRING);		
		pResult.Result = 0x05;	
		pResult.State  =  255;
		DataSend(aIndex, (LPBYTE)&pResult, sizeof(pResult));
		return;
	}
	else wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//----------------------------------------------------------------------------
// [0x63] ģ�� ����� ��û�� ���� ���
//----------------------------------------------------------------------------
void FriendAddResult(LPFHP_FRIEND_ADD_RESULT lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	PMSG_FRIEND_ADD_RESULT	pMsg;

	char_ID szFriendName(lpMsg->FriendName);

	pMsg.h.set((LPBYTE)&pMsg, 0xC1, sizeof(pMsg));
	memcpy( pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);		
	pMsg.Result = lpMsg->Result;
	pMsg.State  = lpMsg->Server;
	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	LogAdd("[%s] Friend add result : (%d) friend Name : (%s)", Name.GetBuffer(), lpMsg->Result, szFriendName.GetBuffer());
}

//----------------------------------------------------------------------------
// [0x64] ������� ģ�� �߰� ��û
//----------------------------------------------------------------------------
void WaitFriendAddRequest(LPPMSG_FRIEND_ADD_SIN_RESULT lpMsg, int aIndex)
{		
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_WAITFRIEND_ADD_REQ	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
	memcpy( pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	memcpy( pMsg.FriendName,  lpMsg->Name, MAX_IDSTRING);
	pMsg.Result	= lpMsg->Result;
	pMsg.Number = aIndex;
	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	{
	char_ID	fname(lpMsg->Name);
	LogAdd("[%s] WaitFriend add request [%s]", gObj[aIndex].Name, fname.GetBuffer());
	}
}

//----------------------------------------------------------------------------
// [0x64] ������� ģ�� �߰� ��û ���
//----------------------------------------------------------------------------
void WaitFriendAddResult(LPFHP_WAITFRIEND_ADD_RESULT lpMsg)
{	
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_ADD_RESULT	pMsg;
	
	pMsg.h.set((LPBYTE)&pMsg, 0xC1, sizeof(pMsg));
	memcpy( pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);
	
	pMsg.Result = lpMsg->Result;
	pMsg.State = lpMsg->pServer;

	char_ID FriendName(lpMsg->FriendName);
	LogAdd("[%s] WaitFriend add result (%d) [%s]", Name.GetBuffer(),  lpMsg->Result, FriendName.GetBuffer());

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
}


//----------------------------------------------------------------------------
// [0x65] ģ�� ������ ��û
//----------------------------------------------------------------------------
void FriendDelRequest(LPPMSG_FRIEND_DEL_REQ lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_ADD_REQ	pMsg;
	char_ID Name(lpMsg->Name);
	
	pMsg.h.set((LPBYTE)&pMsg, 0x65, sizeof(pMsg));
	memcpy( pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	memcpy( pMsg.FriendName, Name.GetBuffer(), MAX_IDSTRING);
	pMsg.Number = aIndex;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

	LogAddTD("[%s] Friend del request [%s]", gObj[aIndex].Name, Name.GetBuffer() );
}


//----------------------------------------------------------------------------
// [0x65] ģ�� ���� ��û�� ���� ���
//----------------------------------------------------------------------------
void FriendDelResult(LPFHP_FRIEND_DEL_RESULT lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	PMSG_FRIEND_DEL_RESULT	pMsg;
	char_ID FriendName(lpMsg->FriendName);

	pMsg.h.set((LPBYTE)&pMsg, 0xC3, sizeof(pMsg));
	memcpy( pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);

	pMsg.Result = lpMsg->Result;

	LogAdd("[%s] Friend del result (%d) [%s]", Name.GetBuffer(), lpMsg->Result, FriendName.GetBuffer());
	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
}

// [0x66] ��ȭ�� ������ ��û�Ѵ�.
void FriendChatRoomCreateReq(LPPMSG_FRIEND_ROOMCREATE_REQ lpMsg, int aIndex)
{
	FHP_FRIEND_CHATROOM_CREATE_REQ	pMsg;

	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.h.set((LPBYTE)&pMsg, 0x66, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	pMsg.Number	= aIndex;
	memcpy(pMsg.fName, lpMsg->Name, MAX_IDSTRING);
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	
	char_ID szName(lpMsg->Name);	
	LogAdd("[%s] Chatroom create request [%s]", gObj[aIndex].Name, szName.GetBuffer());
}

// [0x66] ��ȭ�� ������ ��û ���
void FriendChatRoomCreateResult(LPFHP_FRIEND_CHATROOM_CREATE_RESULT	lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d (%s)", __FILE__, __LINE__, Name.GetBuffer());
		return;
	}
	
	PMSG_FRIEND_ROOMCREATE_RESULT	pMsg;

	pMsg.h.setE((LPBYTE)&pMsg, 0xCA, sizeof( pMsg ));
	
	memcpy(pMsg.ServerIp, lpMsg->ServerIp, 15);
	pMsg.RoomNumber = lpMsg->RoomNumber;
	pMsg.Ticket		= lpMsg->Ticket;
	pMsg.Type		= lpMsg->Type;
	pMsg.Result		= lpMsg->Result;
	memcpy(pMsg.Name, lpMsg->FriendName, MAX_IDSTRING);		

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
	LogAdd("[%s] Chatroom create result (%d) ticket:(%d)", gObj[lpMsg->Number].Name, lpMsg->Result, lpMsg->Ticket);
}

//----------------------------------------------------------------------------
// [0x70] ������ ������.
//----------------------------------------------------------------------------
void FriendMemoSend(LPPMSG_FRIEND_MEMO lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	char_ID		Name(lpMsg->Name);

	// ���� ���� ���� ���ٸ�..
	//if( (gObj[aIndex].Money<1000) && (gObj[aIndex].WarehouseMoney<1000) )	
	if( gObj[aIndex].Money < 1000 )	//#BUGFIX_20040506_01
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;
		pResult.Number = aIndex;
		pResult.Result = 0x07;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, MAX_IDSTRING);
		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)&pResult);
		return;
	}

	if( lpMsg->MemoSize < 0 || lpMsg->MemoSize > MAX_MEMO )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;
		pResult.Number = aIndex;
		pResult.Result = 0x00;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, MAX_IDSTRING);
		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)&pResult);
		return;
	}

	if( Name.GetLength() < 1 )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;
		pResult.Number = aIndex;
		pResult.Result = 0x00;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, MAX_IDSTRING);
		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)&pResult);
		return;
	}
	/*
	// �ڱ� �ڽ����״� ���� �� ����.
	if(strcmp(Name.GetBuffer(), gObj[aIndex].Name) == 0 )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;
		pResult.Number = aIndex;
		pResult.Result = 0x04;
		pResult.WindowGuid = lpMsg->WindowGuid;
		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)&pResult);
		return;
	}
	*/
	
	FHP_FRIEND_MEMO_SEND	pMsg;
	int						bsize = lpMsg->MemoSize+sizeof(pMsg)-MAX_MEMO;
	
	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);

	pMsg.WindowGuid = lpMsg->WindowGuid;
	pMsg.MemoSize	= lpMsg->MemoSize;
	pMsg.Number		= aIndex;
	pMsg.Dir		= lpMsg->Dir;
	pMsg.Action		= lpMsg->Action;

	memcpy(pMsg.Subject,	lpMsg->Subject,			MAX_MEMO_SUBJECT);
	memcpy(pMsg.Name,		gObj[aIndex].Name,		MAX_IDSTRING);

#ifdef ITEM_INDEX_EXTEND_20050706
	memcpy(pMsg.Photo,		gObj[aIndex].CharSet,	MAX_PREVIEWCHARSET+9);
#else
	#ifdef DARKLORD_WORK
	memcpy(pMsg.Photo,		gObj[aIndex].CharSet,	MAX_PREVIEWCHARSET+4);
	#else
	memcpy(pMsg.Photo,		gObj[aIndex].CharSet,	MAX_PREVIEWCHARSET+3);
	#endif
#endif
		
	memcpy(pMsg.ToName,		lpMsg->Name,			MAX_IDSTRING);
	memcpy(pMsg.Memo,		lpMsg->Memo,			lpMsg->MemoSize);
	
	wsExDbCli.DataSend((char*)&pMsg, bsize);
		
	LogAdd("[%s] Friend mail send %s (Size:%d)", gObj[aIndex].Name, Name.GetBuffer(), bsize);
}


//----------------------------------------------------------------------------
// [0x70] �Ŵ������� ������ ������.
//----------------------------------------------------------------------------
#ifdef ADD_MANAGER_SENDMAIL
void MngFriendMemoSend(LPPMSG_JG_MEMO_SEND  lpMsg)
{
	char_ID		Name(lpMsg->Name);
	
	if( lpMsg->MemoSize < 0 || lpMsg->MemoSize > MAX_MEMO )
	{	// �޸� ũ�Ⱑ ���� �ʴ�
//		FHP_FRIEND_MEMO_SEND_RESULT pResult;
//		pResult.Number = aIndex;
//		pResult.Result = 0x00;
//		pResult.WindowGuid = lpMsg->WindowGuid;
//		memcpy(pResult.Name, gObj[aIndex].Name, MAX_IDSTRING);
//		FriendMemoSendResult((LPFHP_FRIEND_MEMO_SEND_RESULT)&pResult);
		return;
	}

	if( Name.GetLength() < 1 )
	{	// ��� �̸��� ����		
		return;
	}
	
	FHP_FRIEND_MEMO_SEND	pMsg;
	int						bsize = lpMsg->MemoSize+sizeof(pMsg)-MAX_MEMO;
	
	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);

	pMsg.WindowGuid = 0;
	pMsg.MemoSize	= lpMsg->MemoSize;
	pMsg.Number		= -1;
	pMsg.Dir		= 0;
	pMsg.Action		= 0;

	memcpy(pMsg.Subject,	lpMsg->Subject,		MAX_MEMO_SUBJECT);
	memcpy(pMsg.Name,		lpMsg->Name,		MAX_IDSTRING);

#ifdef ITEM_INDEX_EXTEND_20050706
	memset(pMsg.Photo,0,MAX_PREVIEWCHARSET+9);
#else
	#ifdef DARKLORD_WORK
	memset(pMsg.Photo,0,MAX_PREVIEWCHARSET+4);
	#else
	memset(pMsg.Photo,0,MAX_PREVIEWCHARSET+3);
	#endif
#endif

	memcpy(pMsg.ToName,		lpMsg->TargetName,			MAX_IDSTRING);
	memcpy(pMsg.Memo,		lpMsg->Memo,			lpMsg->MemoSize);
	
	wsExDbCli.DataSend((char*)&pMsg, bsize);
		
	LogAdd("JoinServer mail send %s (Size:%d)", Name.GetBuffer(), bsize);
}
#endif


void GCMoneySend(int aIndex, DWORD money);
void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney);


// �������� ���� �ȴ´�.
BOOL WithdrawUserMoney(char *Type, LPOBJECTSTRUCT lpObj, int Withdraw_Money)
{
	int oldmoney;

//	if( lpObj->Money-Withdraw_Money > 0 )
	if( lpObj->Money-Withdraw_Money >= 0 )			// #BUGFIX_20040707_01
	{
		oldmoney	  = lpObj->Money;
		lpObj->Money -= Withdraw_Money;
		GCMoneySend(lpObj->m_Index, lpObj->Money);
		LogAdd("[%s][%s] (%s) Pay Money(In Inventory) : %d - %d = %d", lpObj->AccountID, lpObj->Name, Type, oldmoney, Withdraw_Money, lpObj->Money);
		return TRUE;
	}

	/*else if( lpObj->WarehouseMoney-Withdraw_Money > 0 )
	{
		oldmoney			   = lpObj->WarehouseMoney;
		lpObj->WarehouseMoney -= Withdraw_Money;
		LogAdd("[%s][%s] (%s) Pay WareHouse Money(In WareHouse) : %d - %d = %d", lpObj->AccountID, lpObj->Name, Type, oldmoney, Withdraw_Money, lpObj->WarehouseMoney);
		GCWarehouseInventoryMoneySend(lpObj->m_Index, 0x01, lpObj->Money, lpObj->WarehouseMoney);
		return TRUE;
	}
	*/
	return FALSE;
}

//----------------------------------------------------------------------------
// [0x70] ������ ���� ���
//----------------------------------------------------------------------------
void FriendMemoSendResult(LPFHP_FRIEND_MEMO_SEND_RESULT lpMsg)
{
 	char_ID		Name(lpMsg->Name);

#ifdef ADD_MANAGER_SENDMAIL
	if( !strcmp(Name.GetBuffer(), "webzen") )
	{
		LogAdd("JoinServer Send Mail result : (%d)", lpMsg->Result);
		return;
	}
#endif
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}	
	
	PMSG_FRIEND_MEMO_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC5, sizeof(pMsg));
	pMsg.Result		= lpMsg->Result;
	pMsg.WindowGuid = lpMsg->WindowGuid;
	
	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	LogAdd("[%s] Friend mail Send result : (%d) guid:(%d)", gObj[lpMsg->Number].Name, lpMsg->Result, lpMsg->WindowGuid);
	
	// ������ ���������� �������ٸ�.. 1000 ���� ���Ḧ �޴´�.
	if( pMsg.Result == 0x01 )
	{
		WithdrawUserMoney("Mail", &gObj[lpMsg->Number], 1000);
	}

}

//----------------------------------------------------------------------------
// [0x71] ���� ����Ʈ ��û
//----------------------------------------------------------------------------
void FriendMemoListReq(int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_MEMO_LIST_REQ	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x71, sizeof(pMsg));
	
	pMsg.Number	= aIndex;
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);	

	LogAdd("[%s] Friend mail list request", gObj[aIndex].Name);
}

//----------------------------------------------------------------------------
// [0x71] ��������Ʈ�� �޴´�.
//----------------------------------------------------------------------------
void FriendMemoList(LPFHP_FRIEND_MEMO_LIST lpMsg)
{
 	char_ID		SendName(lpMsg->SendName);
	char_ID		RecvName(lpMsg->RecvName);
	
	if( gObjIsConnectedGP(lpMsg->Number, RecvName.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	int  memoindex;
	char subject[MAX_MEMO_SUBJECT+1]="";
	char date[MAX_DATE+1]="";

	PMSG_FRIEND_MEMO_LIST pMsg;

	memoindex	  = lpMsg->MemoIndex;
	memcpy(subject, lpMsg->Subject, MAX_MEMO_SUBJECT);
	memcpy(date, lpMsg->Date, MAX_DATE);

	pMsg.h.setE((LPBYTE)&pMsg, 0xC6, sizeof(pMsg));
	memcpy(pMsg.Date, lpMsg->Date, MAX_DATE);
	memcpy(pMsg.Name, lpMsg->SendName, MAX_IDSTRING);
	memcpy(pMsg.Subject, lpMsg->Subject, MAX_MEMO_SUBJECT);
	pMsg.Number = memoindex;
	pMsg.read   = lpMsg->read;

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	LogAdd("[%s] Friend Mail list : (%d:%10s:%s:%s)", SendName.GetBuffer(), memoindex, date, RecvName.GetBuffer(), subject );
}

//----------------------------------------------------------------------------
// [0x72] ���� ������ ���� �޶�� ��û
//----------------------------------------------------------------------------
void FriendMemoReadReq(LPPMSG_FRIEND_READ_MEMO_REQ lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_MEMO_RECV_REQ	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x72, sizeof(pMsg));
	pMsg.MemoIndex	= lpMsg->MemoIndex;
	pMsg.Number		= aIndex;
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	
	LogAdd("[%s] Friend Mail read Index:%d", gObj[aIndex].Name, lpMsg->MemoIndex);
}

//----------------------------------------------------------------------------
// [0x72] ���� ������ �޴´�.
//----------------------------------------------------------------------------
void FriendMemoRead(LPFHP_FRIEND_MEMO_RECV lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	PMSG_FRIEND_READ_MEMO	pMsg;
	int						nsize=sizeof(pMsg)-MAX_MEMO;

	if( lpMsg->MemoSize > MAX_MEMO )
	{
		LogAddTD("error-L2 : Friend Memo Max %s %d", __FILE__, __LINE__);
		return;
	}
	nsize += lpMsg->MemoSize;
	pMsg.h.setE((LPBYTE)&pMsg, 0xC7, nsize);
	pMsg.MemoIndex	= lpMsg->MemoIndex;
	pMsg.MemoSize	= lpMsg->MemoSize;
	pMsg.Dir		= lpMsg->Dir;
	pMsg.Action		= lpMsg->Action;


#ifdef ITEM_INDEX_EXTEND_20050706
	memcpy(pMsg.Photo, lpMsg->Photo, MAX_PREVIEWCHARSET+9);
#else
	#ifdef DARKLORD_WORK
	memcpy(pMsg.Photo, lpMsg->Photo, MAX_PREVIEWCHARSET+4);
	#else
	memcpy(pMsg.Photo, lpMsg->Photo, MAX_PREVIEWCHARSET+3);
	#endif
#endif

	memcpy(pMsg.Memo, lpMsg->Memo, lpMsg->MemoSize);

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, nsize);
	
	LogAdd("[%s] Friend Mail read (%d)", gObj[lpMsg->Number].Name, lpMsg->MemoIndex);
}

//----------------------------------------------------------------------------
// [0x73] ���� ���� ��û
//----------------------------------------------------------------------------
void FriendMemoDelReq(LPPMSG_FRIEND_MEMO_DEL_REQ lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
 
	FHP_FRIEND_MEMO_DEL_REQ	pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x73, sizeof(pMsg));
	pMsg.MemoIndex	= lpMsg->MemoIndex;
	pMsg.Number		= aIndex;
	memcpy( pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

	LogAdd("[%s] Friend mail delete request Index:(%d)", gObj[aIndex].Name, lpMsg->MemoIndex);
}

//----------------------------------------------------------------------------
// [0x73] ���� ���� ��� 
//----------------------------------------------------------------------------
void FriendMemoDelResult(LPFHP_FRIEND_MEMO_DEL_RESULT lpMsg)
{
 	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	PMSG_FRIEND_MEMO_DEL_RESULT	 pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC8, sizeof(pMsg));
	pMsg.MemoIndex	= lpMsg->MemoIndex;
	pMsg.Result		= lpMsg->Result;
	
	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	LogAdd("[%s] Friend mail delete request (%d) index:(%d)", Name.GetBuffer(), lpMsg->Result, lpMsg->MemoIndex);
}

// 0xCB -> 0x74 : ģ�� �ʴ뿡 ���� ��û
void FriendRoomInvitationReq(LPPMSG_ROOM_INVITATION lpMsg, int aIndex)
{
	if( gObjIsConnectedGP(aIndex) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	} 

	FHP_FRIEND_INVITATION_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x74, sizeof(pMsg));
	pMsg.Number		= aIndex;
	pMsg.RoomNumber = lpMsg->RoomNumber;
	pMsg.WindowGuid	= lpMsg->WindowGuid;

	memcpy(pMsg.FriendName, lpMsg->Name, MAX_IDSTRING);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_IDSTRING);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	LogAdd("[%s] Friend Invistation request room:%d winguid:%d", gObj[aIndex].Name, lpMsg->RoomNumber, lpMsg->WindowGuid);
}

// 0x74 -> 0xCB : ģ�� �ʴ� ���
void FriendRoomInvitationRecv(LPFHP_FRIEND_INVITATION_RET lpMsg)
{
	char_ID		Name(lpMsg->Name);
	
	if( gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()) == FALSE ) 
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ROOM_INVITATION_RESULT	pMsg;


	pMsg.h.set((LPBYTE)&pMsg, 0xCB, sizeof(pMsg));
	pMsg.Result		= lpMsg->Result;
	pMsg.WindowGuid = lpMsg->WindowGuid;

	DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
	LogAdd("[%s] Friend Invitation result :%d", Name.GetBuffer(), pMsg.Result);
}
#endif
//##__FRIEND_WORK__

#endif
