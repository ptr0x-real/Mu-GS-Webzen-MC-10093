#if !defined(__CRYWOLFUTIL_DEF__) && defined(CRYWOLF_COMMON_PATCH_20050413)
#define __CRYWOLFUTIL_DEF__


class CCrywolfScoreSort
{
public:
	bool	operator() (const LPOBJECTSTRUCT lpObj1, const LPOBJECTSTRUCT lpObj2)
	{
		bool bResult = lpObj1->m_iCrywolfMVPScore >= lpObj2->m_iCrywolfMVPScore;
		
		if( lpObj1->m_iCrywolfMVPScore == lpObj2->m_iCrywolfMVPScore )
		{
			bResult = lpObj1->Experience < lpObj2->Experience;
		}

		// true �� ������ �ϴ°� ������ ����
		return bResult;
	}
};


class CCrywolfUtil
{
public:
	CCrywolfUtil();
	virtual ~CCrywolfUtil();

public:

	// �ʼ��� �׷� ��ü�� ����ڿ��� Ư�� �޽����� ������.
	void	SendMapServerGroupMsg( LPSTR lpszMsg, ... );

	// ��� ����ڵ鿡�� Ư�� �����͸� ������.
	void	SendAllUserAnyData( LPBYTE lpMsg, int iSize );

	// ��� ����ڵ鿡�� Ư�� �޽����� ������.		
	void	SendAllUserAnyMsg( int iType, LPSTR lpszMsg, ... );

	// ũ���̿��� 1�� �ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �����͸� ������.
	void	SendCrywolfUserAnyData( LPBYTE lpMsg, int iSize );

	// ũ���̿��� 1�� �ʿ� ������ �ִ� ��� ����ڵ鿡�� Ư�� �޽����� ������.
	void	SendCrywolfUserAnyMsg( int iType, LPSTR lpszMsg, ... );

	// ���� ���� �ִ� ����鿡�� ä�ø޼����� ������. - ���Ͱ� ���ϴµ� ���
	void	SendCrywolfChattingMsg( int iObjIndex, LPSTR lpszMsg, ... );

	// ũ���̿��� MVP ���� ����ġ LevelUp
	int		CrywolfMVPLevelUp( int iUserIndex, int iAddExp );
};

#endif