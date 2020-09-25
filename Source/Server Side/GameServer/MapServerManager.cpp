// MapServerManager.cpp: implementation of the CMapServerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapServerManager.h"

#ifdef MAP_SERVER_WORK_20041030

#include "Common\LogProc.h"
#include "Include\ItemDef.h"
#include "Include\ReadScript.h"
#include "Common\zzzitem.h"
#include "MapClass.h"

#include <TIME.H>


extern short gGameServerCode;					// �� ���Ӽ����� �����ڵ�


_MAPSVR_DATA::_MAPSVR_DATA	()
{
	Clear();
}

_MAPSVR_DATA::~_MAPSVR_DATA	()
{
#if ENABLE_MC_CODE == 1
	delete[] m_sMAP_MOVE;
#endif // ENABLE_MC_CODE == 1
}

VOID _MAPSVR_DATA::Clear	(INT iInitSetVal)
{
	m_bIN_USE						= FALSE;	// ���� �� ��ü�� ��������� ����
	m_btMAPSVR_GROUP				= -1;		// �� ������ �����ִ� �ʼ��� �׷�
	m_sSVR_CODE						= -1;		// �� ������ �����ڵ�
	memset (m_cIPADDR, 0, sizeof(m_cIPADDR));	// �� ������ IP�ּ�
	m_wPORT							= 0;		// �� ������ ��Ʈ��ȣ

#if ENABLE_MC_CODE == 1
	if (g_TerrainManager.Size() < 1)
		g_TerrainManager.Load();

	m_sMAP_MOVE = new SHORT[g_TerrainManager.Size()];
#endif // ENABLE_MC_CODE == 1

#if ENABLE_MC_CODE == 1
	for (INT iMAP_COUNT = 0; iMAP_COUNT < g_TerrainManager.Size(); iMAP_COUNT++)
#else // ENABLE_MC_CODE == 1
	for (INT iMAP_COUNT = 0; iMAP_COUNT < MAX_MAP; iMAP_COUNT++)
#endif // ENABLE_MC_CODE == 1
	{
		switch(iInitSetVal) {
		case -1 :
			m_sMAP_MOVE[iMAP_COUNT]	= MAPSVR_DATASTAT_ANYOTHERFSTSVR;	// �ʱⰪ - �̵� ������ ���� �� ù��° ������ �����Ѵ�.
			break;
		case 0 :
			m_sMAP_MOVE[iMAP_COUNT]	= MAPSVR_DATASTAT_ANYOTHERRNDSVR;	// �ʱⰪ - �̵� ������ ���� �� �ƹ� ������ �����ϰ� �����Ѵ�.
			break;
		default :
			m_sMAP_MOVE[iMAP_COUNT]	= MAPSVR_DATASTAT_HAVEMAP;			// �ʱⰪ - �׻� ��� ������ �̵� �����̴�.
			break;
		}
	}
}


//--------------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CLASS : CMapServerManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapServerManager::CMapServerManager()
{
	srand(time(0));

	m_bMapDataLoadOk			= FALSE;
	m_lpThisMapSvrData			= NULL;
	InitializeCriticalSection	(& m_critSVRCODE_MAP);
}

CMapServerManager::~CMapServerManager()
{
	DeleteCriticalSection		(& m_critSVRCODE_MAP);
}

//////////////////////////////////////////////////////////////////////
// Method Implementation
//////////////////////////////////////////////////////////////////////

VOID CMapServerManager::Clear		()
{
	m_bMapDataLoadOk			= FALSE;

	for (INT iGROUP_COUNT = 0 ; iGROUP_COUNT < MAX_MAPSVR_GROUP ; iGROUP_COUNT++) {
		m_iMAPSVR_GROUP_COUNT [iGROUP_COUNT]			= 0;
		for (INT iSUBSVR_COUNT = 0 ; iSUBSVR_COUNT < MAX_SUBSVR_COUNT ; iSUBSVR_COUNT++) {
			m_MAPSVR_DATA[iGROUP_COUNT][iSUBSVR_COUNT].Clear();		
		}
	}
	m_mapSVRCODE_MAP.clear();
}

BOOL CMapServerManager::LoadData	(LPSTR lpszFileName)
{

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) {
		MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : File Name Error");
		return FALSE;
	}

	EnterCriticalSection		(& m_critSVRCODE_MAP);
	
	__try {
		// ���� �ʼ��� ���� �ʱ�ȭ
		Clear();
		
		// �ʼ��� ���� �ε�
		if((SMDFile=fopen(lpszFileName,"r")) == NULL)	
		{
			MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : fopen() : %s", lpszFileName);
			return FALSE;
		}


		int type = -1;
		SMDToken Token;
		
		while( true )
		{
			Token = (*GetToken)();
			if(Token == END) break;
			type = (int)TokenNumber;
			while(1)
			{				
				if( type == 0 )
				{	// �������� ����ϰ� ������ IP, ��Ʈ �������� ����
					SHORT	sSVR_CODE		= -1;
					SHORT	sMAPSVR_GROUP	= -1;
					INT		iInitSetVal		= 1;
					CHAR	szIpAddr[16]	= {0,};
					WORD	wPortNum		= 0;

					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					sSVR_CODE										= (int)TokenNumber;
					Token = (*GetToken)(); sMAPSVR_GROUP			= (int)TokenNumber;
					Token = (*GetToken)(); iInitSetVal				= (int)TokenNumber;
					Token = (*GetToken)(); memcpy(szIpAddr, TokenString+1, sizeof(szIpAddr));	szIpAddr[sizeof(szIpAddr)-1] = 0;
					Token = (*GetToken)(); wPortNum					= (int)TokenNumber;

					// ����üũ
					// 1 . �����ڵ� Ȯ��
					if (sSVR_CODE < 0) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 1", sSVR_CODE);
						return FALSE;
					}
					// 2 . ���� �������� Ȯ��
					if (iInitSetVal != -1 &&		// ���ӺҰ� - ���Ӱ����� ���� �� ù��° ������ ����
						iInitSetVal != 0 &&			// ���ӺҰ� - ���Ӱ����� ���� �� �ƹ� ������ ����
						iInitSetVal != 1			// ���Ӱ���
						) 
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : iInitSetting Value:%d (SVR:%d) - 1", iInitSetVal, sSVR_CODE);
						return FALSE;
					}
					// 3 . IP�ּ� Ȯ��
					if (!strcmp(szIpAddr, "")) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No IpAddress (SVR:%d)", sSVR_CODE);
						return FALSE;
					}
					// 4 . �ʼ��� �׷��ε��� Ȯ��
					if (!CHECK_LIMIT(sMAPSVR_GROUP, MAX_MAPSVR_GROUP)) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Server Group Index (IDX:%d)", sMAPSVR_GROUP);
						return FALSE;
					}
					// 5 . �����͸� ������ ���� �ִ��� Ȯ��
					if (m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP] >= MAX_SUBSVR_COUNT) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No Space to Save SvrInfo (SVR:%d)", sSVR_CODE);
						return FALSE;
					}

					// ������ ����
					LPMAPSVR_DATA	lpMapSvrData		= NULL;
					lpMapSvrData						= m_MAPSVR_DATA[sMAPSVR_GROUP] + m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP];
					lpMapSvrData->Clear(iInitSetVal);

					lpMapSvrData->m_bIN_USE				= TRUE;
					lpMapSvrData->m_sSVR_CODE			= sSVR_CODE;
					lpMapSvrData->m_btMAPSVR_GROUP		= sMAPSVR_GROUP;
					lpMapSvrData->m_wPORT				= wPortNum;
					memcpy(lpMapSvrData->m_cIPADDR, szIpAddr, sizeof(lpMapSvrData->m_cIPADDR));		lpMapSvrData->m_cIPADDR[sizeof(lpMapSvrData->m_cIPADDR)-1] = 0;
					
					m_mapSVRCODE_MAP.insert(pair<INT, LPMAPSVR_DATA>(sSVR_CODE ,lpMapSvrData));
					m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP]++;
				}
				else if( type == 1 )
				{	// �ʼ������� �̵� �Ұ����� �ʿ� ���� ������ ��´�.
					SHORT			sSVR_CODE			= -1;
					BYTE			btNotMoveOption		= 0;
					WORD			wMapNum				= 0;
					SHORT			sDEST_SVR_CODE		= -1;
					LPMAPSVR_DATA	lpMapSvrData		= NULL;

					Token = (*GetToken)();
					if(strcmp("end",TokenString)==NULL) break;

					sSVR_CODE									= (int)TokenNumber;			
					Token = (*GetToken)(); btNotMoveOption		= (int)TokenNumber;
					Token = (*GetToken)(); wMapNum				= (int)TokenNumber;
					Token = (*GetToken)(); sDEST_SVR_CODE		= (int)TokenNumber;
					
					// ����üũ
					// 1 . �����ڵ� Ȯ��
					if (sSVR_CODE < 0) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 2", sSVR_CODE);
						return FALSE;
					}
					if (sDEST_SVR_CODE < MAPSVR_DATASTAT_ANYOTHERFSTSVR) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sDEST_SVR_CODE < -1 (SVR:%d, DEST_SVR:%d) - 2", 
							sSVR_CODE,
							sDEST_SVR_CODE
							);
						return FALSE;
					}
					// 2 . �ش� �����ڵ尡 ��ϵ� ������ Ȯ��
					map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(sSVR_CODE);
					if (it == m_mapSVRCODE_MAP.end()) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE wasn't registered (SVR:%d)", sSVR_CODE);
						return FALSE;
					}
					// 3 . �ʵ����� ����Ұ� �ǹٷ� �����ϴ��� Ȯ��
					lpMapSvrData						= it->second;
					if (lpMapSvrData == NULL) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData == NULL (SVR:%d)", sSVR_CODE);
						return FALSE;
					}
					if (lpMapSvrData->m_bIN_USE == FALSE) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_bIN_USE == FALSE (SVR:%d)", sSVR_CODE);
						return FALSE;
					}
					if (lpMapSvrData->m_sSVR_CODE != sSVR_CODE) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d)", sSVR_CODE);
						return FALSE;
					}
					// 4 . ���̵� �Ұ� �ɼ������� ����� �������� Ȯ��
					if (btNotMoveOption != 0 && btNotMoveOption != 1) {
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)", 
							sSVR_CODE,
							btNotMoveOption
							);
						return FALSE;
					}
					// 5 . �� ��ȣ�� �ε��� ������ ��ȿ���� Ȯ��
#if ENABLE_MC_CODE == 1
					if (!CHECK_LIMIT(wMapNum, g_TerrainManager.Size()))
#else // ENABLE_MC_CODE == 1
					if (!CHECK_LIMIT(wMapNum, MAX_MAP))
#endif // ENABLE_MC_CODE == 1
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Number is out of bound (SVR:%d, MAP:%d)", 
							sSVR_CODE,
							wMapNum
							);
						return FALSE;
					}

					// ������ ����
					switch(btNotMoveOption) {
					case 0 :		// Ư�� ���� ���� ���Ѵ�.
						{
							lpMapSvrData->m_sMAP_MOVE[wMapNum]				= sDEST_SVR_CODE;
						}
						break;
					case 1 :		// Ư�� ���� �� �� �ִ�.
						{
							lpMapSvrData->m_sMAP_MOVE[wMapNum]				= MAPSVR_DATASTAT_HAVEMAP;
						}
						break;
					default :		// ������� ������ �����̴�.
						{
							MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)", 
								sSVR_CODE,
								btNotMoveOption
								);
						}
						break;
					}
				}
			}
		}
		
		fclose(SMDFile);

		// �� ������ ���� ������ �����͸� �̸� �����´�.
		map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(gGameServerCode);
		if (it != m_mapSVRCODE_MAP.end()) {
			m_lpThisMapSvrData		= it->second;
		}
		else {
			m_lpThisMapSvrData		= NULL;
		}

		if (m_lpThisMapSvrData == NULL) {
			MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : This GameServerCode (%d) doesn't Exist at file '%s' != sSVR_CODE", 
				gGameServerCode,
				lpszFileName
				);
			return FALSE;
		}

		m_bMapDataLoadOk			= TRUE;
	}
	__finally {
		LeaveCriticalSection	(& m_critSVRCODE_MAP);
	}

	return TRUE;
}


BOOL	CMapServerManager::CheckMapCanMove		(INT iMAP_NUM)	// �ܼ��� �ش� ������ �̵��� ���������� üũ
{
	if( !MapNumberCheck(iMAP_NUM) ) 
	{
		return FALSE;
	}

	LPMAPSVR_DATA	lpMapSvrData		= m_lpThisMapSvrData;
	if (lpMapSvrData == NULL) {
		return FALSE;
	}
	else if (lpMapSvrData->m_bIN_USE == FALSE) {
		return FALSE;
	}

	SHORT sMAPMOVE_INFO					= lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];
	if (sMAPMOVE_INFO != MAPSVR_DATASTAT_HAVEMAP)
		return FALSE;

	return TRUE;
}


SHORT	CMapServerManager::CheckMoveMapSvr		(	// �ش� ������ ������ �ٸ� ������ �����ϴ°�?
													// �����ڵ� - ��κ� �ڱ� �����ڵ� , -1 �϶��� ġ������ ���� (������ ����)
	INT iIndex,												 
	INT iMAP_NUM, 
	SHORT sSVR_CODE_BEFORE
	)
{
	if (m_bMapDataLoadOk == FALSE) {
		return gGameServerCode;
	}

	if (!gObjIsConnected(iIndex)) {
		return gGameServerCode;
	}

	if( !MapNumberCheck(iMAP_NUM) ) 
	{
		LogAddC(LOGC_RED, "[MapServerMng] CheckMoveMapSvr() - Map Index doesn't exist [%s][%s] : %d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			iMAP_NUM
			);
		return gGameServerCode;
	}

	LPMAPSVR_DATA	lpMapSvrData		= m_lpThisMapSvrData;
	if (lpMapSvrData == NULL) {
		LogAddC(LOGC_RED, "[MapServerMng] CheckMoveMapSvr() - m_lpThisMapSvrData == NULL [%s][%s] : %d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			iMAP_NUM
			);
		return gGameServerCode;
	}
	else if (lpMapSvrData->m_bIN_USE == FALSE) {
		LogAddC(LOGC_RED, "[MapServerMng] CheckMoveMapSvr() - lpMapSvrData->m_bIN_USE == FALSE [%s][%s] : %d",
			gObj[iIndex].AccountID,
			gObj[iIndex].Name,
			iMAP_NUM
			);
		return gGameServerCode;
	}
	
	SHORT sMAPMOVE_INFO					= lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];
	switch(sMAPMOVE_INFO) {
	case MAPSVR_DATASTAT_ANYOTHERRNDSVR :		// �ٸ� �ƹ� ������ �����ϰ� �����Ѵ�.
		{
			LPMAPSVR_DATA lpDestMapSvrData	= NULL;
			// ���� �������� �ش� �ʿ� �� �� �ִ��� ���� Ȯ�� (�����ϸ� ���������� ����)
			if (sSVR_CODE_BEFORE != -1) {
				EnterCriticalSection	(& m_critSVRCODE_MAP);
				map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);
				if (it != m_mapSVRCODE_MAP.end()) {
					lpDestMapSvrData						= it->second;
				}
				LeaveCriticalSection	(& m_critSVRCODE_MAP);
				
				if (lpDestMapSvrData != NULL) {
					if (lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP) {
						return sSVR_CODE_BEFORE;
					}
				}
			}
			
			// �� �� �ִ� ���� ����� ���� �� �����ϰ� �����Ѵ�.
			vector<LPMAPSVR_DATA> vtMapSvrData;
			EnterCriticalSection	(& m_critSVRCODE_MAP);
			map <INT, LPMAPSVR_DATA>::iterator it			= m_mapSVRCODE_MAP.begin();
			for ( ; it != m_mapSVRCODE_MAP.end() ; it++) {
				LPMAPSVR_DATA lpTempMapSvrData	= it->second;
				if (lpTempMapSvrData && 
					lpTempMapSvrData->m_bIN_USE == TRUE &&
					lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP
					) 
				{
					vtMapSvrData.push_back(it->second);
				}
			}					
			LeaveCriticalSection	(& m_critSVRCODE_MAP);

			SHORT sDestServerCode	= -1;
			if (!vtMapSvrData.empty()) {
				sDestServerCode		= vtMapSvrData[rand()%vtMapSvrData.size()]->m_sSVR_CODE;
			}

			if (sDestServerCode != -1) {
#ifdef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					sDestServerCode,
					gObj[iIndex].MapNumber,
					gObj[iIndex].X,
					gObj[iIndex].Y
					);
#else
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					sDestServerCode
					);
#endif // EXTEND_LOG_SYSTEM_20060202
				return sDestServerCode;
			}
		}
		break;
	case MAPSVR_DATASTAT_ANYOTHERFSTSVR :		// �ٸ� �ƹ� ������ ù��° ������ ������ �����Ѵ�.
		{
			LPMAPSVR_DATA lpDestMapSvrData	= NULL;
			// ���� �������� �ش� �ʿ� �� �� �ִ��� ���� Ȯ�� (�����ϸ� ���������� ����)
			if (sSVR_CODE_BEFORE != -1) {
				EnterCriticalSection	(& m_critSVRCODE_MAP);
				map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);
				if (it != m_mapSVRCODE_MAP.end()) {
					lpDestMapSvrData						= it->second;
				}
				LeaveCriticalSection	(& m_critSVRCODE_MAP);
				
				if (lpDestMapSvrData != NULL) {
					if (lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP) {
						return sSVR_CODE_BEFORE;
					}
				}
			}
			
			// �� �� �ִ� ���� �� ���� ù��° ������ �����Ѵ�.
			SHORT sDestServerCode	= -1;
			EnterCriticalSection	(& m_critSVRCODE_MAP);
			map <INT, LPMAPSVR_DATA>::iterator it			= m_mapSVRCODE_MAP.begin();
			for ( ; it != m_mapSVRCODE_MAP.end() ; it++) {
				LPMAPSVR_DATA lpTempMapSvrData	= it->second;
				if (lpTempMapSvrData && 
					lpTempMapSvrData->m_bIN_USE == TRUE &&
					lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP
					)
				{
					sDestServerCode	= lpTempMapSvrData->m_sSVR_CODE;
				}
			}					
			LeaveCriticalSection	(& m_critSVRCODE_MAP);

			if (sDestServerCode != -1) {
#ifdef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					sDestServerCode,
					gObj[iIndex].MapNumber,
					gObj[iIndex].X,
					gObj[iIndex].Y
					);
#else
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					sDestServerCode
					);
#endif // EXTEND_LOG_SYSTEM_20060202
				return sDestServerCode;
			}
		}
		break;
	case MAPSVR_DATASTAT_HAVEMAP :				// �� �������� �ش� �ʿ� �� �� �ִ�.
		{
#ifdef EXTEND_LOG_SYSTEM_20060202
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d (State Map:%d X:%d Y:%d)",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					gGameServerCode,
					gObj[iIndex].MapNumber,
					gObj[iIndex].X,
					gObj[iIndex].Y
					);
#else
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					gGameServerCode
					);
#endif // EXTEND_LOG_SYSTEM_20060202
			return gGameServerCode;
		}
		break;
	default :									// Ư�� �����ڵ� ?
		{	
			// -1 ���� ū ���̶�� Ư�� ������ �����϶�� ����
			if (sMAPMOVE_INFO > MAPSVR_DATASTAT_ANYOTHERRNDSVR) {
				// �ش� ������ ������ �������� Ȯ���Ѵ�.
				LPMAPSVR_DATA lpDestMapSvrData	= NULL;
				EnterCriticalSection	(& m_critSVRCODE_MAP);
				map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(sMAPMOVE_INFO);
				if (it != m_mapSVRCODE_MAP.end()) {
					lpDestMapSvrData						= it->second;
				}
				LeaveCriticalSection	(& m_critSVRCODE_MAP);
				
				if (lpDestMapSvrData != NULL &&
					lpDestMapSvrData->m_bIN_USE == TRUE &&
					lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP
					) 
				{
					return 	sMAPMOVE_INFO;
				}

				// ���� �����ٸ� �� �� �ִ� ���� �� ���� ù��° ������ �����Ѵ�.
				SHORT sDestServerCode	= -1;
				EnterCriticalSection	(& m_critSVRCODE_MAP);
				it			= m_mapSVRCODE_MAP.begin();
				for ( ; it != m_mapSVRCODE_MAP.end() ; it++) {
					LPMAPSVR_DATA lpTempMapSvrData	= it->second;
					if (lpTempMapSvrData && 
						lpTempMapSvrData->m_bIN_USE == TRUE &&
						lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == MAPSVR_DATASTAT_HAVEMAP
						)
					{
						sDestServerCode	= lpTempMapSvrData->m_sSVR_CODE;
					}
				}					
				LeaveCriticalSection	(& m_critSVRCODE_MAP);

				if (sDestServerCode != -1) {
#ifdef EXTEND_LOG_SYSTEM_20060202
					LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID,
						gObj[iIndex].Name,
						iMAP_NUM,
						sDestServerCode,
						gObj[iIndex].MapNumber,
						gObj[iIndex].X,
						gObj[iIndex].Y
					);
#else
					LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d",
						gObj[iIndex].AccountID,
						gObj[iIndex].Name,
						iMAP_NUM,
						sDestServerCode
					);
#endif // EXTEND_LOG_SYSTEM_20060202
					return sDestServerCode;
				}
			}
			else {
				// ����� ���� �����Ͱ� �߸��� ���̴�. 
				LogAddC(LOGC_RED, "[MapServerMng] CheckMoveMapSvr() - Unknown MapMove Info [%s][%s] : MAP-%d / INFO-%d",
					gObj[iIndex].AccountID,
					gObj[iIndex].Name,
					iMAP_NUM,
					sMAPMOVE_INFO
					);
				return gGameServerCode;
			}
		}
		break;
	}
	
	return gGameServerCode;
}


BOOL CMapServerManager::GetSvrCodeData		(
	WORD	wServerCode,
	LPSTR	lpszIpAddress,
	LPWORD	lpwPort
	)
{
	if( !lpszIpAddress || !lpwPort) {
		return FALSE;
	}

	LPMAPSVR_DATA lpMapSvrData	= NULL;
	EnterCriticalSection	(& m_critSVRCODE_MAP);
	map <INT, LPMAPSVR_DATA>::iterator it		= m_mapSVRCODE_MAP.find(wServerCode);
	if (it != m_mapSVRCODE_MAP.end()) {
		lpMapSvrData		= it->second;
	}
	LeaveCriticalSection	(& m_critSVRCODE_MAP);
	
	if(!lpMapSvrData) {
		return FALSE;
	}

	strcpy(lpszIpAddress, lpMapSvrData->m_cIPADDR);
	*lpwPort				= lpMapSvrData->m_wPORT;
	
	return TRUE;
}


CMapServerManager	g_MapServerManager;


#endif

