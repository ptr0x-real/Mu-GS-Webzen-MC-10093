#if !defined(__TMONSTERAIUTIL_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIUTIL_DEF__

#include "User.h"
#include "protocol.h"
#include "gObjMonster.h"

class TMonsterAIUtil
{

public:
	TMonsterAIUtil();
	virtual ~TMonsterAIUtil();


	// 몬스터 이동 프로토콜 처리 함수

	// 몬스터 길찾기 함수
	BOOL	FindPathToMoveMonster( LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY, int iMaxPathCount = 5, BOOL bPreventOverMoving = FALSE );
	BOOL	SendMonsterMoveMsg( LPOBJECTSTRUCT lpObj );
	
	BOOL	CheckMovingCondition( LPOBJECTSTRUCT lpObj );
	void	SendMonsterV2Msg(LPOBJECTSTRUCT lpObj, LPBYTE lpMsg, int size);


	
	// 몬스터 시작 위치에서 범위 벗어 났는지 체크하는 함수
	BOOL	CheckMoveRange(LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY);

	// 몬스터 스카우팅 위치 잡기 처리 함수
	BOOL	GetXYToPatrol(LPOBJECTSTRUCT lpObj);

	// 타겟으로 부터 특정 위치까지 도망가기 위한 처리 함수
	BOOL	GetXYToEascape(LPOBJECTSTRUCT lpObj);
	
	// 공격 거리 밖의 유저 근처로 가기
	BOOL	GetXYToChase(LPOBJECTSTRUCT lpObj);
	
	

	// 나의 시야에 특정 유저/몬스터/NPC OBJECT가 있는지 판단하는 함수
	BOOL	FindMonViewportObj( int iObjIndex, int iTargetObjIndex );

	// 특정 유저/몬스터/NPC OBJECT가 나를 보고 있는지 판단하는 함수
	BOOL	FindMonViewportObj2( int iObjIndex, int iTargetObjIndex );

	// 나를 보고 있는 사람들에게 채팅메세지를 보낸다. - 몬스터가 말하는데 사용
	void	SendChattingMsg( int iObjIndex, LPSTR lpszMsg, ... );

};


#endif //  __TMONSTERAIUTIL_DEF__