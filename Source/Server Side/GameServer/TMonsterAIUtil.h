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


	// ���� �̵� �������� ó�� �Լ�

	// ���� ��ã�� �Լ�
	BOOL	FindPathToMoveMonster( LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY, int iMaxPathCount = 5, BOOL bPreventOverMoving = FALSE );
	BOOL	SendMonsterMoveMsg( LPOBJECTSTRUCT lpObj );
	
	BOOL	CheckMovingCondition( LPOBJECTSTRUCT lpObj );
	void	SendMonsterV2Msg(LPOBJECTSTRUCT lpObj, LPBYTE lpMsg, int size);


	
	// ���� ���� ��ġ���� ���� ���� ������ üũ�ϴ� �Լ�
	BOOL	CheckMoveRange(LPOBJECTSTRUCT lpObj, int iTargetX, int iTargetY);

	// ���� ��ī���� ��ġ ��� ó�� �Լ�
	BOOL	GetXYToPatrol(LPOBJECTSTRUCT lpObj);

	// Ÿ������ ���� Ư�� ��ġ���� �������� ���� ó�� �Լ�
	BOOL	GetXYToEascape(LPOBJECTSTRUCT lpObj);
	
	// ���� �Ÿ� ���� ���� ��ó�� ����
	BOOL	GetXYToChase(LPOBJECTSTRUCT lpObj);
	
	

	// ���� �þ߿� Ư�� ����/����/NPC OBJECT�� �ִ��� �Ǵ��ϴ� �Լ�
	BOOL	FindMonViewportObj( int iObjIndex, int iTargetObjIndex );

	// Ư�� ����/����/NPC OBJECT�� ���� ���� �ִ��� �Ǵ��ϴ� �Լ�
	BOOL	FindMonViewportObj2( int iObjIndex, int iTargetObjIndex );

	// ���� ���� �ִ� ����鿡�� ä�ø޼����� ������. - ���Ͱ� ���ϴµ� ���
	void	SendChattingMsg( int iObjIndex, LPSTR lpszMsg, ... );

};


#endif //  __TMONSTERAIUTIL_DEF__