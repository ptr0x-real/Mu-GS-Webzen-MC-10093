// KanturuUtil.h: interface for the CKanturuUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUUTIL_H__762782A1_98C2_46DA_B18B_1FCD2B4C6AEE__INCLUDED_) && defined(ADD_KANTURU_20060627)
#define AFX_KANTURUUTIL_H__762782A1_98C2_46DA_B18B_1FCD2B4C6AEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuUtil  
{
public:
	CKanturuUtil();
	virtual ~CKanturuUtil();

	// 칸투르의 상태가 변경될 때마다 클라이언트에 통보한다.
	void	NotifyKanturuChangeState( int iState, int iDetailState );
	// 칸투르에 입장 요청에 대한 응답을 클라이언트에 알린다.
	void	NotifyKanturuEntranceReqResult( int iIndex, int iResult );
	// 칸투르 각 상태의 성공/실패 정보를 클라이언트에 알린다.
	void	NotifyKanturuSuccessValue( int iSuccessValue );

	// 칸투르 보스맵에 입장하면 현재 보스맵의 상태를 클라이언트에 알린다.
	void	NotifyKanturuCurrentState( int iIndex, int iState, int iDetailState );

	// 칸투르 전투중 전투시간을 보스맵에 입장한 클라이언트에 알린다.
	void	NotifyKanturuBattleTime( int iBattleTime );

	// 칸투르 전투중 전체공격 스킬에 맞았음을 해당 클라이언트에 알린다.
	void	NotifyKanturuWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType );

	// 칸투르 전투중 남은 몬스터/유저수를 클라이언트에 알린다.
	void	NotifyKanturuUserMonsterCount( int iMonsterCount, int iUserCount );

#ifdef KANTURU_TEST_200607119
	// 칸투르 전투맵에 입장한 [전투중인] 유저에게 특정한 메세지를 [왼쪽상단공지]로 보낸다.
	void	NotifyLeftTopMsgKanturuBattleUser( LPSTR lpszMsg, ... );
#endif

	//----------------------------------------------------------------------------------------
	// 칸투르 전투맵에 입장한 [전투중인] 유저에게 특정한 메세지를 보낸다.
	void	SendMsgKanturuBattleUser( LPSTR lpszMsg, ... );
	// 칸투르 전투맵에 입장한 [전투중인] 유저에게 특정한 데이터를 보낸다.
	void	SendDataKanturuBattleUser( LPBYTE lpMsg, int iSize );

	// 칸투르 전투맵에 입장한 [모든] 유저에게 특정한 메세지를 보낸다.
	void	SendMsgKauturuBossMapUser( LPSTR lpszMsg, ... );
	// 칸투르 전투맵에 입장한 [모든] 유저에게 특정한 데이터를 보낸다.
	void	SendDataKanturuBossMapUser( LPBYTE lpMsg, int iSize );

	// 칸투르 1, 2차 맵, 보스맵에 입장한 모든 유저에게 특정한 메세지를 보낸다.
	void	SendMsgKauturuMapUser( LPSTR lpszMsg, ... );
	// 칸투르 1, 2차 맵, 보스맵에 입장한 모든 유저에게 특정한 데이터를 보낸다.
	void	SendDataKanturuMapUser( LPBYTE lpMsg, int iSize );

	// 전체 유저에게 특정한 메세지를 보낸다.
	void	SendMsgAllUser( LPSTR lpszMsg, ... );
	// 전체 유저에게 특정한 메세지를 보낸다.
	void	SendDataAllUser( LPBYTE lpMsg, int iSize );

	// 특정 유저에게 특정한 메세지를 보낸다.
	void	SendMsgToUser( int iIndex, LPSTR lpszMsg, ... );
	// 특정 유저에게 특정한 데이터를 보낸다.
	void	SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize );

	//----------------------------------------------------------------------------------------
	// 몬스터의 채팅메세지. - AI 테스트용.
	void	SendKanturuChattingMsg( int iIndex, LPSTR lpszMsg, ... );
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822
	//----------------------------------------------------------------------------------------
	// 칸투르 보스전 타임어택 이벤트 - EventServer에 데이터 전송
	void	SendDataKanturuTimeAttackEvent( int iIndex, BYTE btFlag, int iClearTime );
#endif
	
};

extern CKanturuUtil	g_KanturuUtil;

#endif // !defined(AFX_KANTURUUTIL_H__762782A1_98C2_46DA_B18B_1FCD2B4C6AEE__INCLUDED_)
