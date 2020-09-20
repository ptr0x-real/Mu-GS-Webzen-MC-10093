#pragma once

#if ENABLE_MC_SQL == 1
class CDataBase
{
private:
	SQLINTEGER 	m_AfftedRowCount;
	SQLRETURN	m_Return;

	SQLHENV 	m_hEnv;
	SQLHDBC 	m_hDbc;
	SQLHSTMT	m_hStmt;	
	SQLSMALLINT m_nCol;

	char		m_Col[100][255];
	SQLCHAR		m_ColName[100][50];
	SQLINTEGER	m_lCol[100];

	std::string	m_strHost;
	std::string	m_strUser;
	std::string	m_strPassword;
	std::string	m_strDataBase;
	std::string	m_strDnsName;

	char		m_szTemp[500];
	SQLCHAR		m_szMsg[500];
	SQLCHAR		m_SqlState[500];

	int			m_SQLErrorCount;
	SQLSMALLINT	m_MsgOutLen;
	SQLINTEGER	m_NativeError;

	va_list		m_pArgs;

public:
	CDataBase(void);
	~CDataBase(void);

	bool		Connect(const char * szHost, const char * szDataBase, const char * szUser, const char * szPassword);
	bool		Connect(const char * szDnsName);
	bool		Reconnect();
	bool		Exec(const char * szQuery);
	bool		ExecFormat(const char * szQuery, ...);
	int			FindCol(const char * szName);
	int			GetInt(int nCol);
	int			GetInt(const char *sCol);
	__int64		GetInt64(int nCol);
	__int64		GetInt64(const char *sCol);
	float		GetFloat(int nCol);
	float		GetFloat(const char *sCol);
	void		GetStr(int nCol, char *buf);
	void		GetStr(const char *sCol, char *buf);
	int			GetIntExec(char * szQuery);
	float		GetFloatExec(char * szQuery);
	bool		GetStrExec(char * szQuery, char * buf);
	int			GetAsBinary(LPSTR lpszStatement, LPBYTE OUT lpszReturnBuffer);
	void		SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize);

	SQLRETURN	inline Fetch();
	void		inline Clear();

	virtual	void OnError(const char * szMessage, ...){};
	virtual	void OnQuery(const char * szMessage, ...){};
	virtual void OnMessage(const char * szMessage, ...){};

	void Diagnostic();

	void DiagnosticConn();
};
#endif // ENABLE_MC_SQL == 1