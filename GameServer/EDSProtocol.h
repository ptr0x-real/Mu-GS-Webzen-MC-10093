#ifndef __EDSPROTOCOL_H__
#define __EDSPROTOCOL_H__

extern void ExDataServerProtocolCore(BYTE protoNum, BYTE *aRecv, int aLen);
extern void ExDataServerLogin();

///////////////////////////////////////////////////////////////////
//
//	★ 친구 ☆
//
///////////////////////////////////////////////////////////////////

#ifdef __FRIEND_WORK__
extern void FriendListRequest(int aIndex);
extern void FriendDelRequest(LPPMSG_FRIEND_DEL_REQ lpMsg, int aIndex);
extern void FriendAddRequest(LPPMSG_FRIEND_ADD_REQ lpMsg, int aIndex);
extern void WaitFriendAddRequest(LPPMSG_FRIEND_ADD_SIN_RESULT lpMsg, int aIndex);
extern void FriendMemoSend(LPPMSG_FRIEND_MEMO lpMsg, int aIndex);
extern void FriendMemoReadReq(LPPMSG_FRIEND_READ_MEMO_REQ lpMsg, int aIndex);
extern void FriendMemoDelReq(LPPMSG_FRIEND_MEMO_DEL_REQ lpMsg, int aIndex);
extern void FriendListReq(int aIndex);
extern void FriendMemoListReq(int aIndex);
extern void FriendChatRoomCreateReq(LPPMSG_FRIEND_ROOMCREATE_REQ lpMsg, int aIndex);
extern void FriendRoomInvitationReq(LPPMSG_ROOM_INVITATION lpMsg, int aIndex);
extern void FriendStateClientRecv(LPPMSG_FRIEND_STATE_C lpMsg, int aIndex);
extern void FriendStateRecv(LPFHP_FRIEND_STATE lpMsg);
extern void GDCharClose(int type, char *GuildName, char *Name);

#ifdef ADD_MANAGER_SENDMAIL
extern void MngFriendMemoSend(LPPMSG_JG_MEMO_SEND  lpMsg);
#endif

#endif


///////////////////////////////////////////////////////////////////
//
//	★ 길드 & 연합 보강 기능☆
//
///////////////////////////////////////////////////////////////////
#ifdef ADD_SERVERGROUP_CHATTING_FOR_GUILD_UNION_01_20050706
// [0x50] 서버그룹 길드채팅 
extern void GDGuildServerGroupChattingSend( int iGuildNum, LPPMSG_CHATDATA lpChatData );
extern void DGGuildServerGroupChattingRecv( LPEXSDHP_SERVERGROUP_GUILD_CHATTING_RECV lpMsg );

// [0x51] 서버그룹 연합채팅
extern void GDUnionServerGroupChattingSend( int iUnionNum, LPPMSG_CHATDATA lpChatData );
extern void DGUnionServerGroupChattingRecv( LPEXSDHP_SERVERGROUP_UNION_CHATTING_RECV lpMsg );
#endif


///////////////////////////////////////////////////////////////////
//
//	★ 길드 ☆
//
///////////////////////////////////////////////////////////////////
#ifdef _NEW_EXDB_
extern void DGGuildListRequest();
#endif
extern void DGGuildMemberInfoRequest(int aIndex);

// 20050614 아래 처리를 DSProtocol.h 에서 EDSProtol.h로 옮겨옴
#ifdef ADD_NEW_GUILD_02_20040922
extern void GDGuildCreateSend(int aIndex, char* Name, char * Master, LPBYTE Mark, INT iGuildType = G_TYPE_COMMON);
#else
extern void GDGuildCreateSend(int aIndex, char* Name, char * Master, LPBYTE Mark);
#endif

#ifdef _NEW_EXDB_
extern void GDGuildDestroySend(int aIndex, char* Name, char* Master);
extern void GDGuildMemberAdd(int aIndex, char *Name, char* MemberId);
extern void GDGuildMemberDel(int aIndex, char *Name, char * MemberId);
extern void GDGuildUpdate(char *Name, char *Master, LPBYTE Mark, int score, BYTE count);
extern void DGGuildScoreUpdate(char *guildname, int score);
extern void GDGuildNoticeSave(char *guild_name, char *guild_notice);
#endif


///////////////////////////////////////////////////////////////////
//
//	★ 연합 ☆
//
///////////////////////////////////////////////////////////////////

#ifdef ADD_NEW_GUILD_01_20040913
// [0xE1] 길드 직책 임명 요청
extern void GDGuildReqAssignStatus( int aIndex, int iAssignType, char* szTagetName, int iGuildStatus );
extern void DGGuildAnsAssignStatus( LPEXSDHP_GUILD_ASSIGN_STATUS_RESULT lpMsg);

// [0xE2] 길드 Type 설정
extern void GDGuildReqAssignType( int aIndex, int iGuildType );
extern void DGGuildAnsAssignType( LPEXSDHP_GUILD_ASSIGN_TYPE_RESULT lpMsg);
#endif

#ifdef ADD_NEW_UNION_01_20041006
typedef struct {}__UNION_EDSPROTOCOL_H___;
// [0xE5] RelationShip 맺기
extern void GDRelationShipReqJoin( int aIndex, int iTargetUserIndex, int iRelationShipType );
extern void DGRelationShipAnsJoin( LPEXSDHP_RELATIONSHIP_JOIN_RESULT lpMsg);

// [0xE6] RelationShip 끊기
extern void GDRelationShipReqBreakOff( int aIndex, int iTargetUserIndex, int iRelationShipType );
extern void DGRelationShipAnsBreakOff( LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT lpMsg );

// [0xE6] Detail : Union 해체.
extern void GDUnionBreakOff( int aIndex, int iUnionNumber );
extern void DGUnionBreakOff( LPEXSDHP_RELATIONSHIP_BREAKOFF_RESULT lpMsg );

// [0xE7] RelationShip List
extern void DGRelationShipListRecv( LPEXSDHP_UNION_RELATIONSHIP_LIST lpMsg );

// [0xE8] Notification
extern void DGRelationShipNotificationRecv( LPEXSDHP_NOTIFICATION_RELATIONSHIP lpMsg );

// [0xE9] Union List
extern void GDUnionListSend( INT iUserIndex, INT iUnionMasterGuildNumber );
extern void DGUnionListRecv( LPBYTE aRecv);
#endif


#ifdef ADD_ABILITY_OF_UNIONMASTER_TO_KICK_OUT_UNIONMEMBER_01_20050418
// [0xEB][0x01] Kick Out UnionMember
extern void GDRelationShipReqKickOutUnionMember( int aIndex, char *szTargetGuildName );
extern void DGRelationShipAnsKickOutUnionMember( LPEXSDHP_KICKOUT_UNIONMEMBER_RESULT lpMsg );
#endif




#endif // __EDSPROTOCOL_H__

