// MonsterAttr.cpp: implementation of the CMonsterAttr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\Readscript.h"
#include "MonsterAttr.h"

#ifdef SCRIPT_DECODE_WORK	
#include "..\\common\\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "..\\common\\WzMemScript.h"
#endif

#include "Gamemain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern int gMonsterHp;

CMonsterAttr::CMonsterAttr()
{

}

CMonsterAttr::~CMonsterAttr()
{

}

LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(int aClass)
{
	for( int n=0; n<MAX_MONSTERATTRIBUTE; n++)
	{
		if( m_MonsterAttr[n].m_Index == aClass )
		{
			return (LPMONSTER_ATTRIBUTE)&m_MonsterAttr[n];
		}
	}
	return NULL;
}

#ifdef GMCOMMAND_EX
LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(char* lpszName)
{
	for( int n=0; n<MAX_MONSTERATTRIBUTE; n++)
	{
		if( strcmp(m_MonsterAttr[n].m_Name, lpszName) == 0 )
		{
			return (LPMONSTER_ATTRIBUTE)&m_MonsterAttr[n];
		}
	}
	return NULL;
}
#endif // GMCOMMAND_EX

#ifdef SCRIPT_DECODE_WORK
void CMonsterAttr::LoadAttrDecode(char *filename)
{
	CWZScriptEncode		Encoder;
	if( !Encoder.Open(filename) )
	{
		MsgBox("load error %s", filename);
		return ;
	}

	WZSMDToken Token;
	
	int n=0;

	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			m_MonsterAttr[n].m_Index = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Rate			= Encoder.GetNumber();
			Token = Encoder.GetToken(); strcpy(m_MonsterAttr[n].m_Name, Encoder.GetString());
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Level			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Hp			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Mp			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_DamageMin		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_DamageMax		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Defense		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MagicDefense	= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_AttackRating	= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Successfulblocking = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MoveRange		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_AttackType	= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_AttackRange   = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_ViewRange     = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MoveSpeed		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_AttackSpeed	= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_RegenTime		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Attribute		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_ItemRate		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MoneyRate		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MaxItemLevel	= Encoder.GetNumber();

#ifdef MONSTER_SKILL
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_MonsterSkill	= Encoder.GetNumber();			
#endif

			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Resistance[0] = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Resistance[1] = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Resistance[2] = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MonsterAttr[n].m_Resistance[3] = Encoder.GetNumber();

			m_MonsterAttr[n].m_Hp -= (int)(((float)m_MonsterAttr[n].m_Hp/100)*gMonsterHp);

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
			m_MonsterAttr[n].m_iScriptHP = m_MonsterAttr[n].m_Hp;
#endif

#ifdef MODIFY_JAPAN_MONSTER_VALUE	// 일본의 몬스터 수치 조정때문에 여기서 특정 몬스터의 수치를 조절한다			
			if( m_MonsterAttr[n].m_Level <= 24 && m_MonsterAttr[n].m_Index < 50 )
			{	// 체력 : 호머드 까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19 || m_MonsterAttr[n].m_Level == 24 )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사, 호머드 (80%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*20/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
				else if( m_MonsterAttr[n].m_Level == 20 || m_MonsterAttr[n].m_Level == 22 )
				{	// 웜, 얼음괴물 (70%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*30/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}			
				else
				{	// 나머지 50%적용
					int modiryvalue = m_MonsterAttr[n].m_Hp/2;			
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
			}

			if( m_MonsterAttr[n].m_Level <= 19 && m_MonsterAttr[n].m_Index < 50 )
			{	// 최소, 최대 공격력 : 해골전사까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19  )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사 (70%적용)
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin*30/100;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax*30/100;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMin-modiryvaluemax;
				}				
				else
				{	// 나머지 50%적용
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin/2;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax/2;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMin-modiryvaluemax;
				}
			}
#endif
			
			n++;
			if( n > MAX_MONSTERATTRIBUTE-1 ) 
			{
				MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
				fclose(SMDFile);
				return;
			}
		}
	}	
}
#endif

void CMonsterAttr::LoadAttr(char *filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("load error %s", filename);
		return ;
	}
	SMDToken Token;
	
	int n=0;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			m_MonsterAttr[n].m_Index = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Rate			= (int)TokenNumber;
			Token = (*GetToken)(); strcpy(m_MonsterAttr[n].m_Name, TokenString);
			Token = (*GetToken)(); m_MonsterAttr[n].m_Level			= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Hp			= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Mp			= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_DamageMin		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_DamageMax		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Defense		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_MagicDefense	= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_AttackRating	= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Successfulblocking = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_MoveRange		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_AttackType	= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_AttackRange   = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_ViewRange     = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_MoveSpeed		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_AttackSpeed	= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_RegenTime		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Attribute		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_ItemRate		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_MoneyRate		= (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_MaxItemLevel	= (int)TokenNumber;

#ifdef MONSTER_SKILL
			Token = (*GetToken)(); m_MonsterAttr[n].m_MonsterSkill	= (int)TokenNumber;			
#else
			Token = (*GetToken)();
#endif

			Token = (*GetToken)(); m_MonsterAttr[n].m_Resistance[0] = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Resistance[1] = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Resistance[2] = (int)TokenNumber;
			Token = (*GetToken)(); m_MonsterAttr[n].m_Resistance[3] = (int)TokenNumber;

			m_MonsterAttr[n].m_Hp -= (int)(((float)m_MonsterAttr[n].m_Hp/100)*gMonsterHp);

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
			m_MonsterAttr[n].m_iScriptHP = m_MonsterAttr[n].m_Hp;
#endif

#ifdef MODIFY_JAPAN_MONSTER_VALUE	// 일본의 몬스터 수치 조정때문에 여기서 특정 몬스터의 수치를 조절한다			
			if( m_MonsterAttr[n].m_Level <= 24 && m_MonsterAttr[n].m_Index < 50 )
			{	// 체력 : 호머드 까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19 || m_MonsterAttr[n].m_Level == 24 )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사, 호머드 (80%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*20/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
				else if( m_MonsterAttr[n].m_Level == 20 || m_MonsterAttr[n].m_Level == 22 )
				{	// 웜, 얼음괴물 (70%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*30/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}			
				else
				{	// 나머지 50%적용
					int modiryvalue = m_MonsterAttr[n].m_Hp/2;			
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
			}

			if( m_MonsterAttr[n].m_Level <= 19 && m_MonsterAttr[n].m_Index < 50 )
			{	// 최소, 최대 공격력 : 해골전사까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19  )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사 (70%적용)
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin*30/100;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax*30/100;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMax-modiryvaluemax;					
				}				
				else
				{	// 나머지 50%적용
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin/2;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax/2;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMax-modiryvaluemax;
				}
			}
#endif		

			n++;
			if( n > MAX_MONSTERATTRIBUTE-1 ) 
			{
				MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
				fclose(SMDFile);
				return;
			}
		}
	}
	fclose(SMDFile);
}

#ifdef AUTH_GAMESERVER
void CMonsterAttr::LoadAttr(char *Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	SMDToken Token;
	
	int n=0;

	while( true )
	{
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			m_MonsterAttr[n].m_Index = WzMemScript.GetNumber();
			
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Rate			= WzMemScript.GetNumber();
			
			Token = WzMemScript.GetToken(); strcpy(m_MonsterAttr[n].m_Name, WzMemScript.GetString());
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Level			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Hp			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Mp			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_DamageMin		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_DamageMax		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Defense		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MagicDefense	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_AttackRating	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Successfulblocking = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MoveRange		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_AttackType	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_AttackRange   = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_ViewRange     = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MoveSpeed		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_AttackSpeed	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_RegenTime		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Attribute		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_ItemRate		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MoneyRate		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MaxItemLevel	= WzMemScript.GetNumber();

#ifdef MONSTER_SKILL
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_MonsterSkill	= WzMemScript.GetNumber();			
#endif

			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Resistance[0] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Resistance[1] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Resistance[2] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MonsterAttr[n].m_Resistance[3] = WzMemScript.GetNumber();

			m_MonsterAttr[n].m_Hp -= (int)(((float)m_MonsterAttr[n].m_Hp/100)*gMonsterHp);

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
			m_MonsterAttr[n].m_iScriptHP = m_MonsterAttr[n].m_Hp;
#endif

#ifdef MODIFY_JAPAN_MONSTER_VALUE	// 일본의 몬스터 수치 조정때문에 여기서 특정 몬스터의 수치를 조절한다			
			if( m_MonsterAttr[n].m_Level <= 24 && m_MonsterAttr[n].m_Index < 50 )
			{	// 체력 : 호머드 까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19 || m_MonsterAttr[n].m_Level == 24 )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사, 호머드 (80%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*20/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
				else if( m_MonsterAttr[n].m_Level == 20 || m_MonsterAttr[n].m_Level == 22 )
				{	// 웜, 얼음괴물 (70%적용)
					int modiryvalue = m_MonsterAttr[n].m_Hp*30/100;
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}			
				else
				{	// 나머지 50%적용
					int modiryvalue = m_MonsterAttr[n].m_Hp/2;			
					modiryvalue -= modiryvalue*gMonsterHPAdjust/100;
					m_MonsterAttr[n].m_Hp = m_MonsterAttr[n].m_Hp-modiryvalue;
				}
			}

			if( m_MonsterAttr[n].m_Level <= 19 && m_MonsterAttr[n].m_Index < 50 )
			{	// 최소, 최대 공격력 : 해골전사까지
				if( m_MonsterAttr[n].m_Level == 13 || m_MonsterAttr[n].m_Level == 14 || 
					m_MonsterAttr[n].m_Level == 17 || m_MonsterAttr[n].m_Level == 18 || 
					m_MonsterAttr[n].m_Level == 19  )
				{	// 헌터, 리치, 자이언트, 돌괴물, 해골 전사 (70%적용)
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin*30/100;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax*30/100;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMax-modiryvaluemax;					
				}				
				else
				{	// 나머지 50%적용
					int modiryvaluemin = m_MonsterAttr[n].m_DamageMin/2;
					int modiryvaluemax = m_MonsterAttr[n].m_DamageMax/2;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					m_MonsterAttr[n].m_DamageMin = m_MonsterAttr[n].m_DamageMin-modiryvaluemin;
					m_MonsterAttr[n].m_DamageMax = m_MonsterAttr[n].m_DamageMax-modiryvaluemax;
				}
			}
#endif		

			n++;
			if( n > MAX_MONSTERATTRIBUTE-1 ) 
			{
				MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
				fclose(SMDFile);
				return;
			}
		}
	}	
}
#endif