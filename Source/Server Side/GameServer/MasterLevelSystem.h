// MasterLevelSystem.h: interface for the CMasterLevelSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_)
#define AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912

const int ML_DEFAULT_ADD_POINT			= 1;	// �����ͷ��� ����Ʈ �⺻ ������

// �׾��� �� ����ġ �ս� ��(1/1000)
const int ML_DIE_DECEXP_RATE_COMMON		= 7;	// �Ϲ���
const int ML_DIE_DECEXP_RATE_PKLEVEL1	= 20;	// ������ 1�ܰ�
const int ML_DIE_DECEXP_RATE_PKLEVEL2	= 30;	// ������ 2�ܰ�
const int ML_DIE_DECEXP_RATE_PKLEVEL3	= 40;	// ������ 3�ܰ�

// �׾��� �� �� �ս� ��(1/100)
const int ML_DIE_DECZEN_RATE			= 4;

class CMasterLevelSystem  
{
public:
	CMasterLevelSystem();
	virtual ~CMasterLevelSystem();

private:

	INT64		m_i64MasterLevelExpTlb[MAX_MASTER_LEVEL +1];	// �����ͷ��� ����ġ ���̺�

	int			m_iMinMonsterKillLevel;							// �����ͷ��� ������ ����ġ�� ���� �� �ִ� �ּ� ���� ����
	float		m_fAddExpRate;									// �����ͷ��� �߰� ����ġ(%)


public:	
	//////////////////////////////////////////////////////////////////////////
	// �����ͷ��� �⺻ ó��
	//////////////////////////////////////////////////////////////////////////	
	VOID	LoadData();																// ���Ͽ��� ���� �б�
	VOID	SetMasterLevelExpTlb();													// �����ͷ��� ����ġ ���̺� ����
	VOID	gObjNextMLExpCal( LPOBJECTSTRUCT lpObj );								// ���� ������ �������� �ִ� ����ġ
	BOOL	MasterLevelUp( LPOBJECTSTRUCT lpObj, INT64 iAddExp, bool bEventMapReward = false);						// �����ͷ����� ó�� (���⼭ ����ġ �߰�)

	BOOL	IsMasterLevelUser( LPOBJECTSTRUCT lpObj );								// �����ͷ��� �����ΰ�? TRUE/FALSE
	BOOL	CheckMLGetExp( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );		// �����ͷ��� ������ ����ġ������ �� �ִ� �������� üũ

	INT64	GetMasterLevelExpTlbInfo( int iMasterLevel );							// �ش� �����ͷ����� �ִ� ����ġ�� �����ش�.
	INT		GetDieDecExpRate( LPOBJECTSTRUCT lpObj );								// ��� �� ����ġ �ս� ��(1/1000)
	INT		GetDieDecZenRate( LPOBJECTSTRUCT lpObj );								// ��� �� �� �ս� ��(1/1000)


	//////////////////////////////////////////////////////////////////////////
	// DS <-> GS ó��
	//////////////////////////////////////////////////////////////////////////
	VOID GDReqMasterLevelInfo( LPOBJECTSTRUCT lpObj );		// GD [0x30] �����ͷ��� ���� ��û
	VOID DGAnsMasterLevelInfo( LPBYTE aRecv );				// DG [0x30] �����ͷ��� ���� ��û ��� 	
	VOID GDReqMasterLevelInfoSave( LPOBJECTSTRUCT lpObj );	// GD [0x31] �����ͷ��� ���� ����
	

	//////////////////////////////////////////////////////////////////////////
	// Client <-> GS ó��
	//////////////////////////////////////////////////////////////////////////
	VOID GCMasterLevelInfo( LPOBJECTSTRUCT lpObj );			// GC [0xF3][0x50] �����ͷ��� ����
	VOID GCMasterLevelUpInfo( LPOBJECTSTRUCT lpObj );		// GC [0xF3][0x51] �����ͷ����� �ö���. 



};

extern CMasterLevelSystem g_MasterLevelSystem;
#endif

#endif // !defined(AFX_MASTERLEVELSYSTEM_H__2F3543A9_CA19_4471_842B_FB5451ABCD02__INCLUDED_)
