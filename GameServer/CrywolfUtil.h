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

		// true 가 나오게 하는게 앞으로 정렬
		return bResult;
	}
};


class CCrywolfUtil
{
public:
	CCrywolfUtil();
	virtual ~CCrywolfUtil();

public:

	// 맵서버 그룹 전체의 사용자에게 특정 메시지를 보낸다.
	void	SendMapServerGroupMsg( LPSTR lpszMsg, ... );

	// 모든 사용자들에게 특정 데이터를 보낸다.
	void	SendAllUserAnyData( LPBYTE lpMsg, int iSize );

	// 모든 사용자들에게 특정 메시지를 보낸다.		
	void	SendAllUserAnyMsg( int iType, LPSTR lpszMsg, ... );

	// 크라이울프 1차 맵에 접속해 있는 모든 사용자들에게 특정 데이터를 보낸다.
	void	SendCrywolfUserAnyData( LPBYTE lpMsg, int iSize );

	// 크라이울프 1차 맵에 접속해 있는 모든 사용자들에게 특정 메시지를 보낸다.
	void	SendCrywolfUserAnyMsg( int iType, LPSTR lpszMsg, ... );

	// 나를 보고 있는 사람들에게 채팅메세지를 보낸다. - 몬스터가 말하는데 사용
	void	SendCrywolfChattingMsg( int iObjIndex, LPSTR lpszMsg, ... );

	// 크라이울프 MVP 보상 경험치 LevelUp
	int		CrywolfMVPLevelUp( int iUserIndex, int iAddExp );
};

#endif