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

	// ĭ������ ���°� ����� ������ Ŭ���̾�Ʈ�� �뺸�Ѵ�.
	void	NotifyKanturuChangeState( int iState, int iDetailState );
	// ĭ������ ���� ��û�� ���� ������ Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuEntranceReqResult( int iIndex, int iResult );
	// ĭ���� �� ������ ����/���� ������ Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuSuccessValue( int iSuccessValue );

	// ĭ���� �����ʿ� �����ϸ� ���� �������� ���¸� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuCurrentState( int iIndex, int iState, int iDetailState );

	// ĭ���� ������ �����ð��� �����ʿ� ������ Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuBattleTime( int iBattleTime );

	// ĭ���� ������ ��ü���� ��ų�� �¾����� �ش� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType );

	// ĭ���� ������ ���� ����/�������� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyKanturuUserMonsterCount( int iMonsterCount, int iUserCount );

#ifdef KANTURU_TEST_200607119
	// ĭ���� �����ʿ� ������ [��������] �������� Ư���� �޼����� [���ʻ�ܰ���]�� ������.
	void	NotifyLeftTopMsgKanturuBattleUser( LPSTR lpszMsg, ... );
#endif

	//----------------------------------------------------------------------------------------
	// ĭ���� �����ʿ� ������ [��������] �������� Ư���� �޼����� ������.
	void	SendMsgKanturuBattleUser( LPSTR lpszMsg, ... );
	// ĭ���� �����ʿ� ������ [��������] �������� Ư���� �����͸� ������.
	void	SendDataKanturuBattleUser( LPBYTE lpMsg, int iSize );

	// ĭ���� �����ʿ� ������ [���] �������� Ư���� �޼����� ������.
	void	SendMsgKauturuBossMapUser( LPSTR lpszMsg, ... );
	// ĭ���� �����ʿ� ������ [���] �������� Ư���� �����͸� ������.
	void	SendDataKanturuBossMapUser( LPBYTE lpMsg, int iSize );

	// ĭ���� 1, 2�� ��, �����ʿ� ������ ��� �������� Ư���� �޼����� ������.
	void	SendMsgKauturuMapUser( LPSTR lpszMsg, ... );
	// ĭ���� 1, 2�� ��, �����ʿ� ������ ��� �������� Ư���� �����͸� ������.
	void	SendDataKanturuMapUser( LPBYTE lpMsg, int iSize );

	// ��ü �������� Ư���� �޼����� ������.
	void	SendMsgAllUser( LPSTR lpszMsg, ... );
	// ��ü �������� Ư���� �޼����� ������.
	void	SendDataAllUser( LPBYTE lpMsg, int iSize );

	// Ư�� �������� Ư���� �޼����� ������.
	void	SendMsgToUser( int iIndex, LPSTR lpszMsg, ... );
	// Ư�� �������� Ư���� �����͸� ������.
	void	SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize );

	//----------------------------------------------------------------------------------------
	// ������ ä�ø޼���. - AI �׽�Ʈ��.
	void	SendKanturuChattingMsg( int iIndex, LPSTR lpszMsg, ... );
	
#ifdef KANTURU_TIMEATTACK_EVENT_20060822
	//----------------------------------------------------------------------------------------
	// ĭ���� ������ Ÿ�Ӿ��� �̺�Ʈ - EventServer�� ������ ����
	void	SendDataKanturuTimeAttackEvent( int iIndex, BYTE btFlag, int iClearTime );
#endif
	
};

extern CKanturuUtil	g_KanturuUtil;

#endif // !defined(AFX_KANTURUUTIL_H__762782A1_98C2_46DA_B18B_1FCD2B4C6AEE__INCLUDED_)
