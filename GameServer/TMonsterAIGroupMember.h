#if !defined(__TMONSTERAIGROUPMEMBER_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIGROUPMEMBER_DEF__

#define MAX_MONSTER_AI_GROUP_MEMBER	100

class TMonsterAIGroupMember
{

public:
	TMonsterAIGroupMember();
	virtual ~TMonsterAIGroupMember();

public:
	void	Reset();


public:
	int	m_iGroupNumber;
	int	m_iGuid;
	int m_iClass;		
	int m_iRank;	
	int m_iStartAI;
	int	m_iAI01;
	int	m_iAI02;
	int	m_iAI03;
	int m_iCreateType;	
	int m_iMapNumber;	
	int m_iStartX;		
	int m_iStartY;		
	int m_iStartDir;	
	int m_iRegenType;

	// 1) 파일에서 읽지 않는다. 할당된값.
	// 2) Monster 첫 생성시 할당 시킨다.
	//
	// 주의) Monster Reload가 되어야 값이 할당 된다.. (옵션 리로드만으로는 안됨!)
	int m_iObjIndex;
};

#endif
