//----------------------------------------------------------------------------------
// FileName				: RaklionUtil.h
// Creation Date		: 2008/04/22(pelero)
// Modification Date	: 2008/06/05(pelero)
// Note					: ��Ŭ���� ��� ���� ��ƿ��Ƽ Ŭ����
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

	// ��Ŭ������ ���°� ����� ������ Ŭ���̾�Ʈ�� �뺸�Ѵ�.
	void	NotifyRaklionChangeState( int iState, int iDetailState );
	
	// ��Ŭ���� �������� ����/���� ������ Ŭ���̾�Ʈ�� �˸���.
	void	NotifyRaklionSuccessValue( int iSuccessValue );
	
	// ��Ŭ���� �����ʿ� �����ϸ� ���� �������� ���¸� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyRaklionCurrentState( int iIndex, int iState, int iDetailState );
	
	// ��Ŭ���� ������ ��ü���� ��ų�� �¾����� �ش� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyRaklionWideAreaAttack( int iIndex, int iTargetIndex, int iSkillType );
	
	// ��Ŭ���� ������ ���� ����/�������� Ŭ���̾�Ʈ�� �˸���.
	void	NotifyRaklionUserMonsterCount( int iMonsterCount, int iUserCount );
	
	// ��Ŭ���� �����ʿ� ������ [��������] �������� Ư���� �޼����� [���ʻ�ܰ���]�� ������.
	void	NotifyLeftTopMsgRaklionBattleUser( LPSTR lpszMsg, ... );
	
	//----------------------------------------------------------------------------------------
	// ��Ŭ���� �������� ������ �������� Ư���� �޼����� ������.
	void	SendMsgRaklionBossMapUser( LPSTR lpszMsg, ... );
	// ��Ŭ���� �������� ������ �������� Ư���� �����͸� ������.
	void	SendDataRaklionBossMapUser( LPBYTE lpMsg, int iSize );

	// ��Ŭ���� �ʵ�, �������� ������ ��� �������� Ư���� �޼����� ������.
	void	SendMsgRaklionMapUser( LPSTR lpszMsg, ... );
	// ��Ŭ���� �ʵ�, �������� ������ ��� �������� Ư���� �����͸� ������.
	void	SendDataRaklionMapUser( LPBYTE lpMsg, int iSize );
	
	// ��ü �������� Ư���� �޼����� ������.
	void	SendMsgAllUser( LPSTR lpszMsg, ... );
	// ��ü �������� Ư���� �����͸� ������.
	void	SendDataAllUser( LPBYTE lpMsg, int iSize );
	
	// Ư�� �������� Ư���� �޼����� ������.
	void	SendMsgToUser( int iIndex, LPSTR lpszMsg, ... );
	// Ư�� �������� Ư���� �����͸� ������.
	void	SendDataToUser( int iIndex, LPBYTE lpMsg, int iSize );

	// Ư�� �������� �ý��� �޽����� ������.
	void	SendSystemMsgToUser( INT iIndex, LPSTR lpszMsg, ... );
};

extern CRaklionUtil	g_RaklionUtil;

#endif // !defined(AFX_RAKLIONUTIL_H__9158B756_ABC8_4FDD_AD01_9F094F4A8164__INCLUDED_)
