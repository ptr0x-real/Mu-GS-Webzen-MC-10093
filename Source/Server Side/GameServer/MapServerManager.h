// MapServerManager.h: interface for the CMapServerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSERVERMANAGER_H__180258A5_D0A6_4835_9037_211AC69C8D49__INCLUDED_)
#define AFX_MAPSERVERMANAGER_H__180258A5_D0A6_4835_9037_211AC69C8D49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

#ifdef MAP_SERVER_WORK_20041030

#include <MAP>
#include <VECTOR>

using namespace std;

#include "Include\DEFINE.H"
#include "Common\WINUTIL.H"

#define		MAX_SUBSVR_COUNT		20						// �ִ� ���꼭�� ����
#define		MAX_MAPSVR_GROUP		(MAX_SUBSVR_COUNT)		// �ִ� �����׷� ���� (���������� ���꼭���� ������ �Ѿ �� ����.)


enum {				
	// �������� Ư�� ���� �� �� �ִ��� ���� ��
	MAPSVR_DATASTAT_ANYOTHERRNDSVR	= -1,		// �� �������� ���� ���� �ٸ� �Ƹ� ������ �������� ����
	MAPSVR_DATASTAT_ANYOTHERFSTSVR	= -2,		// �� �������� ���� ���� �� �� �ִ� ù��° ������ ����
	MAPSVR_DATASTAT_HAVEMAP			= -3,		// �� ������ �ش� ���� �� �� �ִ�
												// �� ���� ���� �� : �� �������� ���� ���� Ư�� ������ ����
};

typedef struct _MAPSVR_DATA {	
	// ���� ���� �ϳ��� ������ �ִ� ������ �� �� �ִ��� ����
	BOOL	m_bIN_USE;							// ���� �� ��ü�� ��������� ����
	BYTE	m_btMAPSVR_GROUP;					// �� ������ �����ִ� �ʼ��� �׷�
	SHORT	m_sSVR_CODE;						// �� ������ �����ڵ�
	CHAR	m_cIPADDR[16];						// �� ������ IP�ּ�
	WORD	m_wPORT;							// �� ������ ��Ʈ��ȣ
	SHORT	*m_sMAP_MOVE;				// �� �������� �̵� ����/�Ұ����� �����鿡 ���� ���� (���� �������� �� ���)

	_MAPSVR_DATA			();
	virtual ~_MAPSVR_DATA	();
	VOID	Clear			(INT iInitSetVal = 1);
} MAPSVR_DATA, * LPMAPSVR_DATA;

//--------------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CLASS : CMapServerManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMapServerManager  
{	
	BOOL		m_bMapDataLoadOk;				// �� �����Ͱ� ���������� �ε�Ǿ��°� ? (�ƴ϶�� ��ü��� ��ȿ)

	MAPSVR_DATA m_MAPSVR_DATA [MAX_MAPSVR_GROUP][MAX_SUBSVR_COUNT];		// Ȥ�ö� �߰��� ���ε��۾����� ���� �޸� ������ ������ �������� ����
	INT			m_iMAPSVR_GROUP_COUNT [MAX_MAPSVR_GROUP];

	map <INT, LPMAPSVR_DATA>	m_mapSVRCODE_MAP;
	CRITICAL_SECTION			m_critSVRCODE_MAP;

	LPMAPSVR_DATA				m_lpThisMapSvrData;

public:
	CMapServerManager			();
	virtual ~CMapServerManager	();

	VOID	Clear				();
	BOOL	LoadData			(LPSTR lpszFileName);
	BOOL	CheckMapCanMove		(INT iMAP_NUM);		// �ܼ��� �ش� ������ �̵��� ���������� üũ
	SHORT	CheckMoveMapSvr		(					// �ش� ������ ������ �ٸ� ������ �����ϴ°�?
													// �����ڵ� - ��κ� �ڱ� �����ڵ� , -1 �϶��� ġ������ ���� (������ ����)
		INT iIndex,												 
		INT iMAP_NUM, 
		SHORT sSVR_CODE_BEFORE
		);

	BOOL	GetSvrCodeData		(
		WORD	wServerCode,
		LPSTR	lpszIpAddress,
		LPWORD	lpwPort
		);
	INT		GetMapSvrGroup		()	{
		if (m_lpThisMapSvrData)	return m_lpThisMapSvrData->m_btMAPSVR_GROUP;
		return -1;
	}
};


extern CMapServerManager	g_MapServerManager;

#endif


#endif // !defined(AFX_MAPSERVERMANAGER_H__180258A5_D0A6_4835_9037_211AC69C8D49__INCLUDED_)
