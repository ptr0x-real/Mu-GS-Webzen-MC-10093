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

	// 1) ���Ͽ��� ���� �ʴ´�. �Ҵ�Ȱ�.
	// 2) Monster ù ������ �Ҵ� ��Ų��.
	//
	// ����) Monster Reload�� �Ǿ�� ���� �Ҵ� �ȴ�.. (�ɼ� ���ε常���δ� �ȵ�!)
	int m_iObjIndex;
};

#endif
