#if !defined(__TMONSTERAIMOVEPATH_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIMOVEPATH_DEF__

#define MAX_MONSTER_MOVE_PATH_SPOT	300

class TMonsterAIMovePathInfo
{
public:
	TMonsterAIMovePathInfo()
	{
			Reset();
	}

	void	Reset()
	{
			m_iType			= _INVALID;
			m_iMapNumber	= _INVALID;
			m_iPathX		= _INVALID;
			m_iPathY		= _INVALID;
	}

public:
	int		m_iType;
	int		m_iMapNumber;
	int		m_iPathX;
	int		m_iPathY;

};

class TMonsterAIMovePath
{

public:
	TMonsterAIMovePath();
	virtual ~TMonsterAIMovePath();

public:
	int						LoadData( LPSTR lpszFileName );
	int						DelAllAIMonsterMovePath();
	
public:
	BOOL					m_bDataLoad; 
	
public:
	TMonsterAIMovePathInfo	m_MovePathInfo[MAX_MONSTER_MOVE_PATH_SPOT];
	int						m_iMovePathSpotCount;	
};


#endif