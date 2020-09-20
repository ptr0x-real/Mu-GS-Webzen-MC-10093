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

#define		MAX_SUBSVR_COUNT		20						// 최대 서브서버 개수
#define		MAX_MAPSVR_GROUP		(MAX_SUBSVR_COUNT)		// 최대 서버그룹 개수 (물리적으로 서브서버의 개수를 넘어갈 수 없다.)


enum {				
	// 서버에서 특정 맵을 갈 수 있는지 상태 값
	MAPSVR_DATASTAT_ANYOTHERRNDSVR	= -1,		// 이 서버에는 맵이 없고 다른 아마 서버나 랜덤으로 가라
	MAPSVR_DATASTAT_ANYOTHERFSTSVR	= -2,		// 이 서버에는 맵이 없고 갈 수 있는 첫번째 서버로 가라
	MAPSVR_DATASTAT_HAVEMAP			= -3,		// 이 서버는 해당 맵을 갈 수 있다
												// 이 외의 양의 값 : 이 서버에는 맵이 없고 특정 서버로 가라
};

typedef struct _MAPSVR_DATA {	
	// 서버 마다 하나씩 가지고 있는 서버를 갈 수 있는지 여부
	BOOL	m_bIN_USE;							// 현재 이 객체를 사용중인지 여부
	BYTE	m_btMAPSVR_GROUP;					// 이 서버가 속해있는 맵서버 그룹
	SHORT	m_sSVR_CODE;						// 이 서버의 서버코드
	CHAR	m_cIPADDR[16];						// 이 서버의 IP주소
	WORD	m_wPORT;							// 이 서버의 포트번호
	SHORT	*m_sMAP_MOVE;				// 이 서버에서 이동 가능/불가능한 서버들에 대한 정보 (위의 상태정보 값 사용)

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
	BOOL		m_bMapDataLoadOk;				// 맵 데이터가 성공적으로 로드되었는가 ? (아니라면 객체사용 무효)

	MAPSVR_DATA m_MAPSVR_DATA [MAX_MAPSVR_GROUP][MAX_SUBSVR_COUNT];		// 혹시라도 중간에 리로드작업으로 인한 메모리 오류가 없도록 정적으로 선언
	INT			m_iMAPSVR_GROUP_COUNT [MAX_MAPSVR_GROUP];

	map <INT, LPMAPSVR_DATA>	m_mapSVRCODE_MAP;
	CRITICAL_SECTION			m_critSVRCODE_MAP;

	LPMAPSVR_DATA				m_lpThisMapSvrData;

public:
	CMapServerManager			();
	virtual ~CMapServerManager	();

	VOID	Clear				();
	BOOL	LoadData			(LPSTR lpszFileName);
	BOOL	CheckMapCanMove		(INT iMAP_NUM);		// 단순히 해당 맵으로 이동이 가능한지만 체크
	SHORT	CheckMoveMapSvr		(					// 해당 맵으로 가려면 다른 서버로 가야하는가?
													// 에러코드 - 대부분 자기 서버코드 , -1 일때는 치명적인 에러 (보내지 않음)
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
