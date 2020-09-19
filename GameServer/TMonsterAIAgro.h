#if !defined(__TMONSTERAIAGRO_DEF__) && defined(ADD_NEW_MONSTER_SYSTEM_02_20050725)
#define __TMONSTERAIAGRO_DEF__

#define MAX_MONSTER_AI_AGRO	100

class TMonsterAIAgroInfo
{

public:
	TMonsterAIAgroInfo()
	{
			Reset();
	}

	void	Reset()
	{
			m_iUserIndex	= _INVALID;
			m_iAgroValue	= _INVALID;
	}

	int		GetUserIndex()
	{
			return m_iUserIndex;
	}

	int		GetAgroValue()
	{
			return m_iAgroValue;
	}

	void	SetAgro( int iTargetIndex, int iValue )
	{
			m_iUserIndex = iTargetIndex;
			m_iAgroValue = iValue;
	}
	
	int		IncAgro( int iIncValue )
	{
			m_iAgroValue += iIncValue;
			return m_iAgroValue;
	}

	int		DecAgro( int iDecValue )
	{
			m_iAgroValue -= iDecValue;

			if( m_iAgroValue < 0 )
				m_iAgroValue = 0;

			return m_iAgroValue;
	}

private:
	int		m_iUserIndex;
	int		m_iAgroValue;

};


class TMonsterAIAgro
{

public:
	TMonsterAIAgro();
	virtual ~TMonsterAIAgro();
	
public:
	void	ResetAll();
	
	int		SetAgro( int iTargetIndex, int iValue );
	int		DelAgro( int iTargetIndex );
	int		GetAgro( int iTargetIndex );

	int		IncAgro( int iTargetIndex, int iIncValue );
	int		DecAgro( int iTargetIndex, int iDecValue );
	
	void	DecAllAgro( int iDecValue );

	int		GetMaxAgroUserIndex( int iMonsterIndex );

private:
	TMonsterAIAgroInfo	m_Agro[MAX_MONSTER_AI_AGRO];

};




#endif // __TMONSTERAIAGRO_DEF__
