// MonsterSetBase.cpp: implementation of the CMonsterSetBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\Include\\ReadScript.h"
#include "GameMain.h"
#include "MonsterSetBase.h"

#ifdef SCRIPT_DECODE_WORK	
#include "..\\common\\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "..\\common\\WzMemScript.h"
#endif

#ifdef MAP_SERVER_WORK_20041030
#include "MapServerManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterSetBase::CMonsterSetBase()
{

}

CMonsterSetBase::~CMonsterSetBase()
{

}

void CMonsterSetBase::SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah)
{
	m_Mp[TableNum].m_MapNumber	= mapnumber;
	m_Mp[TableNum].m_W          = aw;
	m_Mp[TableNum].m_H          = ah;
	m_Mp[TableNum].m_X			= ax;
	m_Mp[TableNum].m_Y			= ay;
}

BOOL CMonsterSetBase::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short & mx, short& my)
{
	int count=100;
	int w, h;
	int tx, ty;
	BYTE attr;

	while(count--)
	{
		w = aw-ax;
		h = ah-ay;
		
		__try
		{
			tx = ax+(rand()%w);
			ty = ay+(rand()%h);
		}
		__except ( w=1, h=1, EXCEPTION_EXECUTE_HANDLER )
		{
		}
		
		attr = MapC[mapnumber].GetAttr(tx, ty);
		if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
			((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) // 비어있다면..
		{
			mx = tx;
			my = ty;
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef MODIFY_MONSTER_RANDOM_POS_CREATION_20051202
#include "LargeRand.h"
#endif

BOOL CMonsterSetBase::GetPosition(int TableNum, short MapNumber, short & x, short & y)
{
	int count=100;
	BYTE attr;
	int  tx, ty;
	int  w, h;
	
	if( TableNum < 0 || (TableNum > (MAX_MONSTER-1)) ) 
	{
		LogAdd("ERROR : %s %d",__FILE__, __LINE__);
		return FALSE;
	}
	if( m_Mp[TableNum].m_ArrangeType == 1 || m_Mp[TableNum].m_ArrangeType == 3)
	{
		while(count--)
		{
			w = m_Mp[TableNum].m_W-m_Mp[TableNum].m_X;
			h = m_Mp[TableNum].m_H-m_Mp[TableNum].m_Y;
			
			if( w < 1 ) w = 1;
			if( h < 1 ) h = 1;

#ifdef MODIFY_MONSTER_RANDOM_POS_CREATION_20051202
			int iRX = GetLargeRand()%w;
			int iRY = GetLargeRand()%h;
			
			tx = m_Mp[TableNum].m_X + iRX;
			ty = m_Mp[TableNum].m_Y + iRY;
#else
			tx = m_Mp[TableNum].m_X+(rand()%w);
			ty = m_Mp[TableNum].m_Y+(rand()%h);
#endif

			attr = MapC[MapNumber].GetAttr(tx, ty);
			if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
				((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
				((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) // 비어있다면..
			{
				x = tx;
				y = ty;
				return TRUE;
			}
		}
	}
	else if( m_Mp[TableNum].m_ArrangeType == 0 ) //if( m_Mp[TableNum].m_W && m_Mp[TableNum].m_H )
	{
		x = m_Mp[TableNum].m_X;
		y = m_Mp[TableNum].m_Y;
		return TRUE;
	}
	else if( m_Mp[TableNum].m_ArrangeType == 2 )
	{
		int Orx, Ory;
		
		while(count--)
		{
			Orx = m_Mp[TableNum].m_X-3;
			Ory = m_Mp[TableNum].m_Y-3;

			Orx += (rand()%7);
			Ory += (rand()%7);

			attr = MapC[MapNumber].GetAttr(Orx, Ory);
			if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
				((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
				((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) // 비어있다면..
			{
				x = Orx;
				y = Ory;
				return TRUE;
			}
		}		
	}
	else if( m_Mp[TableNum].m_ArrangeType == 3 )
	{
		return TRUE;
	}
#ifdef FOR_BLOODCASTLE
	else if( m_Mp[TableNum].m_ArrangeType == 4 )
	{	
		x = m_Mp[TableNum].m_X;
		y = m_Mp[TableNum].m_Y;			
		return TRUE;		
	}
#endif

	return FALSE;
}

#ifdef SCRIPT_DECODE_WORK	
void CMonsterSetBase::LoadSetBaseDecode(char *filename)
{
	CWZScriptEncode		Encoder;
	if( !Encoder.Open(filename) )
	{
		MsgBox("file open error %s", filename);
		return ;
	}

	WZSMDToken Token;
	
	int rcount;
	int copycount;
	m_Count = 0;
	BYTE Sdir;

	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == T_NUMBER)
		{
			int Type = Encoder.GetNumber();
			while(true)
			{
				Token = Encoder.GetToken();
				if(strcmp("end",Encoder.GetString())==NULL) break;

				m_Mp[m_Count].m_Type = Encoder.GetNumber();
				Token = Encoder.GetToken(); m_Mp[m_Count].m_MapNumber = (BYTE)Encoder.GetNumber();
				Token = Encoder.GetToken(); m_Mp[m_Count].m_Dis       = (BYTE)Encoder.GetNumber();
				Token = Encoder.GetToken(); m_Mp[m_Count].m_X		 = (BYTE)Encoder.GetNumber();
				Token = Encoder.GetToken(); m_Mp[m_Count].m_Y		 = (BYTE)Encoder.GetNumber();
				
				m_Mp[m_Count].m_ArrangeType = Type;
				if( Type == 1 || Type == 3)
				{
					Token = Encoder.GetToken(); m_Mp[m_Count].m_W = (BYTE)Encoder.GetNumber();
					Token = Encoder.GetToken(); m_Mp[m_Count].m_H = (BYTE)Encoder.GetNumber();
				}
				else if( Type == 2 )
				{
					int Orx, Ory;

					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
					
					Orx = m_Mp[m_Count].m_X-3;
					Ory = m_Mp[m_Count].m_Y-3;
					Orx += (rand()%7);
					Ory += (rand()%7);

					m_Mp[m_Count].m_X = Orx;
					m_Mp[m_Count].m_Y = Ory;
				}
#ifdef FOR_BLOODCASTLE
				else if( Type == 4 )
				{
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;									
				}
#endif
				else 
				{ 
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
				}
								
				Token = Encoder.GetToken(); m_Mp[m_Count].m_Dir = (BYTE)Encoder.GetNumber();
				Sdir = m_Mp[m_Count].m_Dir;
				if( m_Mp[m_Count].m_Dir == 0xFF )
					m_Mp[m_Count].m_Dir = (rand()%8);

				if( Type == 1 || Type == 3 )
				{
					BYTE Orx, Ory;
					Orx = m_Mp[m_Count].m_X;
					Ory = m_Mp[m_Count].m_Y;
					
					Token = Encoder.GetToken(); rcount = Encoder.GetNumber();
					copycount = m_Count;
					
					if( rcount > 1 )
					{
						for( int n=0; n<rcount-1; n++)
						{
							m_Count++;
							if( m_Count > MAX_MONSTER-1 ) 
							{
								MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
								fclose(SMDFile);
								return;
							}
							m_Mp[m_Count].m_Dis         = m_Mp[copycount].m_Dis;
							m_Mp[m_Count].m_Type		= m_Mp[copycount].m_Type;
							m_Mp[m_Count].m_MapNumber	= m_Mp[copycount].m_MapNumber;
							m_Mp[m_Count].m_W           = m_Mp[copycount].m_W;
							m_Mp[m_Count].m_H           = m_Mp[copycount].m_H;
							m_Mp[m_Count].m_X			= m_Mp[copycount].m_X;
							m_Mp[m_Count].m_Y			= m_Mp[copycount].m_Y;
							m_Mp[m_Count].m_ArrangeType = m_Mp[copycount].m_ArrangeType;
							if( Sdir == 0xFF ) m_Mp[m_Count].m_Dir = (rand()%8);
							else m_Mp[m_Count].m_Dir = m_Mp[copycount].m_Dir;
						}
					}
				}
				m_Count++;
				if( m_Count > MAX_MONSTER-1 ) 
				{
					MsgBox("Monster attribute max over %d (%s %d)", m_Count, __FILE__, __LINE__);
					fclose(SMDFile);
					return;
				}

			}
		}
	}	
}
#endif

void CMonsterSetBase::LoadSetBase(char *filename)
{
	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		MsgBox("file open error %s", filename);
		return ;
	}
	SMDToken Token;
	
	int rcount;
	int copycount;
	m_Count = 0;
	BYTE Sdir;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			while(true)
			{
				Token = (*GetToken)();
				if(strcmp("end",TokenString)==NULL) break;

				m_Mp[m_Count].m_Type = (int)TokenNumber;
				Token = (*GetToken)(); m_Mp[m_Count].m_MapNumber = (BYTE)TokenNumber;
				Token = (*GetToken)(); m_Mp[m_Count].m_Dis       = (BYTE)TokenNumber;
				Token = (*GetToken)(); m_Mp[m_Count].m_X		 = (BYTE)TokenNumber;
				Token = (*GetToken)(); m_Mp[m_Count].m_Y		 = (BYTE)TokenNumber;
				
				m_Mp[m_Count].m_ArrangeType = Type;
				if( Type == 1 || Type == 3)
				{
					Token = (*GetToken)(); m_Mp[m_Count].m_W = (BYTE)TokenNumber;
					Token = (*GetToken)(); m_Mp[m_Count].m_H = (BYTE)TokenNumber;
				}
				else if( Type == 2 )
				{
					int Orx, Ory;

					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
					
					Orx = m_Mp[m_Count].m_X-3;
					Ory = m_Mp[m_Count].m_Y-3;
					Orx += (rand()%7);
					Ory += (rand()%7);

					m_Mp[m_Count].m_X = Orx;
					m_Mp[m_Count].m_Y = Ory;
				}
#ifdef FOR_BLOODCASTLE
				else if( Type == 4 )
				{
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;									
				}
#endif
				else 
				{ 
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
				}
								
				Token = (*GetToken)(); m_Mp[m_Count].m_Dir = (BYTE)TokenNumber;
				Sdir = m_Mp[m_Count].m_Dir;
				if( m_Mp[m_Count].m_Dir == 0xFF )
					m_Mp[m_Count].m_Dir = (rand()%8);

				if( Type == 1 || Type == 3 )
				{
					BYTE Orx, Ory;
					Orx = m_Mp[m_Count].m_X;
					Ory = m_Mp[m_Count].m_Y;
					
					Token = (*GetToken)(); rcount = (int)TokenNumber;
					copycount = m_Count;

					if( rcount > 1 )
					{
						for( int n=0; n<rcount-1; n++)
						{
#ifdef MAP_SERVER_ADD_MONSTER_20041216
#ifdef MAP_SERVER_WORK_20041030
							if (!g_MapServerManager.CheckMapCanMove(m_Mp[copycount].m_MapNumber)) {
								break;
							}
#endif
#endif
							m_Count++;
							if( m_Count > MAX_MONSTER-1 ) 
							{
								MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
								fclose(SMDFile);
								return;
							}
							m_Mp[m_Count].m_Dis         = m_Mp[copycount].m_Dis;
							m_Mp[m_Count].m_Type		= m_Mp[copycount].m_Type;
							m_Mp[m_Count].m_MapNumber	= m_Mp[copycount].m_MapNumber;
							m_Mp[m_Count].m_W           = m_Mp[copycount].m_W;
							m_Mp[m_Count].m_H           = m_Mp[copycount].m_H;
							m_Mp[m_Count].m_X			= m_Mp[copycount].m_X;
							m_Mp[m_Count].m_Y			= m_Mp[copycount].m_Y;
							m_Mp[m_Count].m_ArrangeType = m_Mp[copycount].m_ArrangeType;
							if( Sdir == 0xFF ) m_Mp[m_Count].m_Dir = (rand()%8);
							else m_Mp[m_Count].m_Dir = m_Mp[copycount].m_Dir;
						}
					}
				}
				
#ifdef MAP_SERVER_ADD_MONSTER_20041216
#ifdef MAP_SERVER_WORK_20041030
				if (!g_MapServerManager.CheckMapCanMove(m_Mp[m_Count].m_MapNumber)) {
					continue;
				}
#endif
#endif
				m_Count++;
				if( m_Count > MAX_MONSTER-1 ) 
				{
					MsgBox("Monster attribute max over %d (%s %d)", m_Count, __FILE__, __LINE__);
					fclose(SMDFile);
					return;
				}

			}
		}
	}
	fclose(SMDFile);
}

#ifdef AUTH_GAMESERVER
void CMonsterSetBase::LoadSetBase(char *Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);
	
	SMDToken Token;
	
	int rcount;
	int copycount;
	m_Count = 0;
	BYTE Sdir;

	while( true )
	{	
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			
			int Type = WzMemScript.GetNumber();
			while(true)
			{
				Token = WzMemScript.GetToken();				
				if(strcmp("end",WzMemScript.GetString())==NULL) break;

				m_Mp[m_Count].m_Type = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); m_Mp[m_Count].m_MapNumber = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); m_Mp[m_Count].m_Dis       = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); m_Mp[m_Count].m_X		 = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken(); m_Mp[m_Count].m_Y		 = WzMemScript.GetNumber();
				
				m_Mp[m_Count].m_ArrangeType = Type;
				if( Type == 1 || Type == 3)
				{
					Token = WzMemScript.GetToken(); m_Mp[m_Count].m_W = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken(); m_Mp[m_Count].m_H = WzMemScript.GetNumber();
				}
				else if( Type == 2 )
				{
					int Orx, Ory;

					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
					
					Orx = m_Mp[m_Count].m_X-3;
					Ory = m_Mp[m_Count].m_Y-3;
					Orx += (rand()%7);
					Ory += (rand()%7);

					m_Mp[m_Count].m_X = Orx;
					m_Mp[m_Count].m_Y = Ory;
				}
#ifdef FOR_BLOODCASTLE
				else if( Type == 4 )
				{
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;									
				}
#endif
				else 
				{ 
					m_Mp[m_Count].m_W = 0;
					m_Mp[m_Count].m_H = 0;
				}
								

				Token = WzMemScript.GetToken(); m_Mp[m_Count].m_Dir = WzMemScript.GetNumber();
				Sdir = m_Mp[m_Count].m_Dir;
				if( m_Mp[m_Count].m_Dir == 0xFF )
					m_Mp[m_Count].m_Dir = (rand()%8);

				if( Type == 1 || Type == 3 )
				{
					BYTE Orx, Ory;
					Orx = m_Mp[m_Count].m_X;
					Ory = m_Mp[m_Count].m_Y;
					
					Token = WzMemScript.GetToken(); rcount = WzMemScript.GetNumber();
					copycount = m_Count;
					
					if( rcount > 1 )
					{
						for( int n=0; n<rcount-1; n++)
						{
#ifdef MAP_SERVER_ADD_MONSTER_20041216
#ifdef MAP_SERVER_WORK_20041030
							if (!g_MapServerManager.CheckMapCanMove(m_Mp[copycount].m_MapNumber)) {
								break;
							}
#endif
#endif
							m_Count++;
							if( m_Count > MAX_MONSTER-1 ) 
							{
								MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);								
								return;
							}
							m_Mp[m_Count].m_Dis         = m_Mp[copycount].m_Dis;
							m_Mp[m_Count].m_Type		= m_Mp[copycount].m_Type;
							m_Mp[m_Count].m_MapNumber	= m_Mp[copycount].m_MapNumber;
							m_Mp[m_Count].m_W           = m_Mp[copycount].m_W;
							m_Mp[m_Count].m_H           = m_Mp[copycount].m_H;
							m_Mp[m_Count].m_X			= m_Mp[copycount].m_X;
							m_Mp[m_Count].m_Y			= m_Mp[copycount].m_Y;
							m_Mp[m_Count].m_ArrangeType = m_Mp[copycount].m_ArrangeType;
							if( Sdir == 0xFF ) m_Mp[m_Count].m_Dir = (rand()%8);
							else m_Mp[m_Count].m_Dir = m_Mp[copycount].m_Dir;
						}
					}
				}

#ifdef MAP_SERVER_ADD_MONSTER_20041216
#ifdef MAP_SERVER_WORK_20041030
				if (!g_MapServerManager.CheckMapCanMove(m_Mp[m_Count].m_MapNumber)) {
					continue;
				}
#endif
#endif
				m_Count++;
				if( m_Count > MAX_MONSTER-1 ) 
				{
					MsgBox("Monster attribute max over %d (%s %d)", m_Count, __FILE__, __LINE__);					
					return;
				}

			}
		}
	}	
}
#endif
