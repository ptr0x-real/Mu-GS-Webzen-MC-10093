//----------------------------------------------------------------------------------
// FileName				: RaklionUtil.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: 라클리온 통신 관련 유틸리티 클래스
//----------------------------------------------------------------------------------
// RaklionUtil.h: interface for the CRaklionUtil class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONUTIL_H__9158B756_ABC8_4FDD_AD01_9F094F4A8164__INCLUDED_) && defined(ADD_RAKLION_20080408)
#define AFX_RAKLIONUTIL_H__9158B756_ABC8_4FDD_AD01_9F094F4A8164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRaklionUtil  
{
public:
	CRaklionUtil();
	virtual ~CRaklionUtil();

	// 라클리온의 상태가 변경될 때마다 클라이언트에 통보한다.
	void	NotifyRaklionChangeState( int iState, int iDetailState );
	
	// 라클리온 보스전의 성공/실패 정보를 클라이언트에 알린다.
	void	NotifyRaklionSuccessValue( int iSuccessValue );
	
	// 라클리온 보스맵에 입장하면 현재 보스맵의 상태를 클라이언트에 알린다.
	void	NotifyRaklionCurrentState( int iIndex, int iState, int iDetailState );
	
	// 라클리온 전투중 전체공격 스킬에 맞았음을 해당 클라이언트에 알린다.
	void	NotifyRaklionWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType );
	
	// 라클리온 전투중 남은 몬스터/유저수를 클라이언트에 알린다.
	void	NotifyRaklionUserMonsterCount( int iMonsterCount, int iUserCount );
	
	// 라클리온 전투맵에 입장한 [전투중인] 유저에게 특정한 메세지를 [왼쪽상단공지]로 보낸다.
	void	NotifyLeftTopMsgRaklionBattleUser( LPSTR lpszMsg, ... );
	
	//----------------------------------------------------------------------------------------
	// 라클리온 보스존에 입장한 유저에게 특정한 메세지를 보낸다.
	void	SendMsgRaklionBossMapUser( LPSTR lpszMsg, ... );
	// 라클리온 보스존에 입장한 유저에게 특정한 데이터를 보낸다.
	void	SendDataRaklionBossMapUser( LPBYTE lpMsg, int iSize );

	// 라클리온 필드, 보스존에 입장한 모든 유저에게 특정한 메세지를 보낸다.
	void	SendMsgRaklionMapUser( LPSTR lpszMsg, ... );
	// 라클리온 필드, 보스존에 입장한 모든 유저에게 특정한 데이터를 보낸다.
	void	SendDataRaklionMapUser( LPBYTE lpMsg, int iSize );
	
	// 전체 유저에게 특정한 메세지를 보낸다.
	void	SendMsgAllUser( LPSTR lpszMsg, ... );
	// 전체 유저에게 특정한 데이터를 보낸다.
	void	SendDataAllUser( LPBYTE lpMsg, int iSize );
	
	// 특정 유저에게 특정한 메세지를 보낸다.
	void	SendMsgToUser( int iIndex, LPSTR lpszMsg, ... );
	// 특정 유저에게 특정한 데이터를 보낸다.
	void	SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize );

	// 특정 유저에게 시스템 메시지를 보낸다.
	void	SendSystemMsgToUser( INT iIndex, LPSTR lpszMsg, ... );
};

extern CRaklionUtil	g_RaklionUtil;

#endif // !defined(AFX_RAKLIONUTIL_H__9158B756_ABC8_4FDD_AD01_9F094F4A8164__INCLUDED_)
