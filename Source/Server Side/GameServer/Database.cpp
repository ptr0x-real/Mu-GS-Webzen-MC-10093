#include "StdAfx.h"

#if ENABLE_MC_SQL == 1
CDataBase::CDataBase(void)
{
	m_SQLErrorCount		= 1;
	m_AfftedRowCount 	= -1;
	m_Return			= SQL_SUCCESS;

	memset(m_szTemp, 0, sizeof(m_szTemp));
	memset(m_szMsg,0 , sizeof(m_szMsg));
	memset(m_SqlState, 0, sizeof(m_SqlState));

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, & m_hEnv);
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
}

CDataBase::~CDataBase(void)
{
	if (m_hStmt)SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	if (m_hDbc) SQLDisconnect(m_hDbc);
	if (m_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}

bool CDataBase::Connect(const char * szHost, const char * szDataBase, const char * szUser, const char * szPassword)
{
	bool bReturn						= false;
	int	iRecored						= 1;
    SQLSMALLINT cbOutCon				= 0;

	SQLCHAR InCon[255]					= {0};
	SQLCHAR OutCon[255]					= {0};
	SQLWCHAR szSQLState[500]	        = {0};
	SQLWCHAR szSQLMsg[500]		        = {0};

	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, & m_hDbc);

	wsprintf((char *)InCon, "DRIVER=SQL Server;Server=%s;DATABASE=%s;UID=%s;PWD=%s;", szHost,
		szDataBase, szUser, szPassword);

	OnMessage((char *)InCon);

	m_Return = SQLDriverConnect(m_hDbc, NULL, InCon, sizeof(InCon), OutCon,
		sizeof(OutCon), & cbOutCon, SQL_DRIVER_NOPROMPT);

	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
	{
		m_Return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, & m_hStmt);

		if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
		{
			m_strHost = szHost;
			m_strDataBase = szDataBase;
			m_strUser = szUser;
			m_strPassword = szPassword;

			bReturn  = true;
		}
		else
		{
			m_hStmt = 0;
		}
	}

	return bReturn;
}

bool CDataBase::Connect(const char * szDnsName)
{
	bool bReturn						= false;
	int	iRecored						= 1;
    SQLSMALLINT cbOutCon				= 0;

	SQLCHAR InCon[255]					= {0};
	SQLCHAR OutCon[255]					= {0};
	SQLWCHAR szSQLState[500]	        = {0};
	SQLWCHAR szSQLMsg[500]		        = {0};

	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, & m_hDbc);

	wsprintf((char *)InCon, "DATABASE=%s;DRIVER=SQL Server;Server=(local);", szDnsName);

	OnMessage((char *)InCon);

	m_Return = SQLDriverConnect(m_hDbc, NULL, InCon, sizeof(InCon), OutCon,
		sizeof(OutCon), & cbOutCon, SQL_DRIVER_NOPROMPT);

	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
	{
		m_Return = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, & m_hStmt);

		if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO))
		{
			m_strDnsName = szDnsName;

			bReturn  = true;
		}
		else
		{
			m_hStmt = 0;
		}
	}

	return bReturn;
}

bool CDataBase::Reconnect()
{
	if(m_strDnsName.length() == 0)
	{
		return Connect(m_strHost.c_str(), m_strDataBase.c_str(), m_strUser.c_str(), m_strPassword.c_str());
	}
	else
	{
		return Connect(m_strDnsName.c_str());
	}
}

bool CDataBase::Exec(const char * szQuery)
{
	bool bReturn = false;

	this->Clear( );

	m_Return = SQLExecDirect(m_hStmt, (SQLCHAR *)szQuery, SQL_NTS);

	OnQuery( szQuery );

	if((m_Return == SQL_SUCCESS) || (m_Return == SQL_SUCCESS_WITH_INFO) || (m_Return == SQL_NO_DATA))
	{
		SQLRowCount(m_hStmt, & m_AfftedRowCount);
		SQLNumResultCols(m_hStmt, & m_nCol);

		if (m_nCol > 100)
		{
			OnError("Você excedeu o número máximo de colunas!");
		}
		else if(m_nCol == 0)
		{
			this->Clear( );

			bReturn = true;
		}
		else
		{
			for (int i = 0 ; i < m_nCol ; i++)
			{
				SQLBindCol(m_hStmt, i+1, SQL_C_CHAR, m_Col[i], 255, & m_lCol[i]);
				SQLDescribeCol(m_hStmt, i+1, m_ColName[i], 30, NULL, NULL, NULL, NULL, NULL);
			}

			bReturn = true;
		}
	}
	else
	{
		Diagnostic();
	}

	return bReturn;
}

bool CDataBase::ExecFormat( const char * szQuery, ... )
{
	va_start( m_pArgs , szQuery );
	vsprintf( m_szTemp , szQuery , m_pArgs );
	va_end( m_pArgs );

	return Exec( m_szTemp );
}

int CDataBase::FindCol(const char * szName)
{
	int iReturn = -1;

	for (int i = 0 ; i < m_nCol ; i++)
	{
		if (_stricmp(szName, (LPCTSTR)m_ColName[i]) == 0)
		{
			iReturn = i;
			break;
		}
	}
	return iReturn;
}

int CDataBase::GetInt(int nCol)
{
	int Return = - 102;

	if (nCol <= m_nCol)
	{
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			Return = atoi(m_Col[nCol]);
		}
		else
		{
			Return = - 100;
		}
	}

	return Return;
}

int CDataBase::GetInt(const char *sCol)
{
	int Return = - 102;
	int n		= -1;

	n = FindCol(sCol);

	if (n != -1)
	{
		Return = GetInt(n);
	}

	return Return;
}

__int64 CDataBase::GetInt64(int nCol)
{
	__int64 Return = - 102;

	if (nCol <= m_nCol)
	{
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			Return = _atoi64(m_Col[nCol]);
		}
		else
		{
			Return = - 100;
		}
	}

	return Return;
}

__int64 CDataBase::GetInt64(const char *sCol)
{
	__int64 Return = - 102;
	int n		= -1;

	n = FindCol(sCol);

	if (n != -1)
	{
		Return = GetInt64(n);
	}

	return Return;
}

float CDataBase::GetFloat(int nCol)
{
	float Return = - 102;

	if (nCol <= m_nCol)
	{ 
		if (m_lCol[nCol] != SQL_NULL_DATA)
		{
			Return = (float)atof(m_Col[nCol]);
		}
		else
		{
			Return = - 100;
		}
	}

	return Return;
}

float CDataBase::GetFloat(const char *sCol)
{
	float Return	= - 102;
	int n			= -1;

	n = FindCol(sCol);

	if (n != -1)
	{
		Return = GetFloat(n);
	}

	return Return;
}

void CDataBase::GetStr(int nCol, char *buf)
{
	if (nCol > m_nCol)
	{
		buf[0] = '\0';
		return;
	}

	if (m_lCol[nCol] == SQL_NULL_DATA) 
	{
		buf[0] = '\0';
	} 
	else 
	{
		lstrcpy(buf, m_Col[nCol]);
	}
}

void CDataBase::GetStr(const char *sCol, char *buf)
{
	int n = -1;

	n = FindCol(sCol);

	if (n == -1)
	{
		buf[0] = '\0';
	} 
	else 
	{
		GetStr(n, buf);
	}
}

int	CDataBase::GetIntExec(char * szQuery)
{
	int Return = - 103;

	if (Exec(szQuery)) 
	{
		if (Fetch() != SQL_NO_DATA)
		{	
			Return = GetInt(1);
			this->Clear( );
		}
		else
		{
			this->Clear( );
			Return = - 101;
		}
	}

	return Return;
}

float CDataBase::GetFloatExec(char * szQuery)
{
	float Return = - 103;

	if (Exec(szQuery)) 
	{
		if (Fetch() != SQL_NO_DATA)
		{	
			Return = GetFloat(1);

			this->Clear( );
		}
		else
		{
			this->Clear( );

			Return = - 101;
		}
	}

	return Return;
}

bool CDataBase::GetStrExec(char * szQuery, char * buf)
{
	bool Return = false;

	if (Exec(szQuery))
	{
		if (Fetch() != SQL_NO_DATA)
		{
			GetStr(1,buf);
			this->Clear( );
			Return = true;
		}
		else
		{
			this->Clear( );
			buf[0] = '\0';
		}
	}
	else
	{
		buf[0] = '\0';
	}
	return Return;
}

int CDataBase::GetAsBinary(LPTSTR lpszStatement, LPBYTE OUT lpszReturnBuffer)
{
	SQLCHAR * pSQLBuf;
	SQLINTEGER BufLen;
	SQLCHAR SQLBinary[10000];
	SQLINTEGER lOfs = 0;
	SQLINTEGER SQLLen;
	SQLRETURN SQLResult;

	this->Clear( );

	OnQuery(lpszStatement);

	SQLResult = SQLExecDirect(m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);


	if ( SQLResult != SQL_SUCCESS )
	{
		Diagnostic();
		return -1;
	}

	while ( true )
	{
		SQLResult = SQLFetch(m_hStmt);

		if ( SQLResult == SQL_NO_DATA )
			break;

		pSQLBuf = lpszReturnBuffer;

		while ( true )
		{

			SQLResult = SQLGetData(m_hStmt,
				1, SQL_C_BINARY , SQLBinary, sizeof(SQLBinary), &SQLLen);

			if ( SQLResult == SQL_NO_DATA )
				break;

			if ( SQLLen == -1 )
			{
				Clear();
				return 0;
			}

			if ( SQLResult == SQL_SUCCESS )
			{
				BufLen = SQLLen;
			}
			else
			{
				BufLen = sizeof(SQLBinary);
			}

			lOfs += BufLen;
			memcpy(pSQLBuf, SQLBinary, BufLen);
			pSQLBuf = &pSQLBuf[lOfs];
		}
	}
	return BufLen;
}

void CDataBase::SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize)
{
	this->Clear( );

	SQLINTEGER cbValueSize = -0x64 - BinaryBufferSize;
	SQLPOINTER pToken;
	BYTE cBUFFER[10000];
	SQLRETURN Result;

	SQLBindParameter(m_hStmt, 1, SQL_PARAM_INPUT,
		SQL_C_BINARY , SQL_LONGVARBINARY, BinaryBufferSize, 0, (SQLPOINTER)1, 0, &cbValueSize);

	OnQuery(lpszStatement);
	SQLExecDirect(m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);
	Result = SQLParamData(m_hStmt, &pToken);

	int lOfs=0;

	while ( Result == SQL_NEED_DATA )
	{
		memcpy(cBUFFER, lpBinaryBuffer, sizeof(cBUFFER));
		Result = SQLPutData(m_hStmt, cBUFFER, BinaryBufferSize);
		Result = SQLParamData(m_hStmt, &pToken);
		lOfs += sizeof(cBUFFER);
	}

	SQLParamData(m_hStmt, &pToken);
	this->Clear( );
}

SQLRETURN CDataBase::Fetch()
{
	return SQLFetch(m_hStmt); 
}

void CDataBase::Clear() 
{
	SQLCloseCursor(m_hStmt); SQLFreeStmt(m_hStmt, SQL_CLOSE);
}

void CDataBase::Diagnostic()
{
	if (SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, m_SQLErrorCount, m_SqlState, &m_NativeError,
		m_szMsg, sizeof(m_szMsg), &m_MsgOutLen) != SQL_NO_DATA)
	{
		m_SQLErrorCount++;

		memcpy(m_szTemp, m_szMsg, m_MsgOutLen);

		OnError(m_szTemp);
	}
	else m_SQLErrorCount = 1;

	if (strcmp((LPCTSTR)m_SqlState, "08S01") == 0)
	{
		Reconnect();
	}
}

void CDataBase::DiagnosticConn()
{
	if (SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, m_SQLErrorCount, m_SqlState, &m_NativeError,
		m_szMsg, sizeof(m_szMsg), &m_MsgOutLen) != SQL_NO_DATA)
	{
		m_SQLErrorCount++;

		memcpy(m_szTemp, m_szMsg, m_MsgOutLen);

		OnError(m_szTemp);
	}
	else m_SQLErrorCount = 1;

	if (strcmp((LPCTSTR)m_SqlState, "08S01") == 0)
	{
		Reconnect();
	}
}
#endif // ENABLE_MC_SQL == 1