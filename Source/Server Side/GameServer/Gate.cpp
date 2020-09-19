// Gate.cpp: implementation of the CGate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Include\public.h"
#include "Include\Readscript.h"
#include "Gate.h"
#include "MapClass.h"

#include "user.h"

#ifdef SCRIPT_DECODE_WORK	
#include "Common\WZScriptEncode.h"
#endif

#ifdef AUTH_GAMESERVER
#include "Common\WzMemScript.h"
#endif

#include "Gamemain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
// gate number , flag, mapnumber, x, y, x2, y2, target gate, Dir, Level
// flag : 1 : 들어갈수만 있는곳, 2 : 나갈수만 있는곳, 3 : 들어가고 나가고 모두 가능
// target gate : 이동하려는 gate number
// Level : 레벨 제한, 0이면 레벨제한이 없는거다. 20이면 20 이상만 이동할수 있다.

1  1 0 121 232 123 233 2   0   0 
2  2 1 107 247 110 247 0   1   0	//로렌시아 -> 던젼1층
*/

extern MapClass		*MapC;

CGate	gGateC;

CGate::CGate()
{
	
}

CGate::~CGate()
{

}

void CGate::Init()
{
	for( int n=0; n<MAX_GATE; n++)
	{
		m_This[n] = 0xFF;
		m_Level[n] = 0;
	}
}


#ifdef SCRIPT_DECODE_WORK
void CGate::LoadDecode(char *filename)
{
	Init();

	CWZScriptEncode		Encoder;
	if( !Encoder.Open(filename) )
	{
		LogAdd("[%s] gate information load error", filename);
		return ;
	}

	WZSMDToken Token;

	int number;

	while( true )
	{
		Token = Encoder.GetToken();
		if(Token == END) break;
		if(Token == T_NUMBER)
		{
			number = Encoder.GetNumber();
			m_This[number] = number;
			Token = Encoder.GetToken(); m_Flag[number]		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_MapNumber[number]	= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Sx[number]			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Sy[number]			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Ex[number]			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Ey[number]			= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_TargetGate[number] = Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Dir[number]		= Encoder.GetNumber();
			Token = Encoder.GetToken(); m_Level[number]		= Encoder.GetNumber();
		}
	}	
	LogAdd("[%s] Gate information data load complete.", filename);
}
#endif


void CGate::Load(char *filename)
{
	Init();

	if((SMDFile=fopen(filename,"r")) == NULL)	
	{
		LogAdd("[%s] gate information load error", filename);
		return ;
	}
	SMDToken Token;

	int number;

	while( true )
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			number = (int)TokenNumber;
			m_This[number] = number;
			Token = (*GetToken)(); m_Flag[number]		= (int)TokenNumber;
			Token = (*GetToken)(); m_MapNumber[number]	= (int)TokenNumber;
			Token = (*GetToken)(); m_Sx[number]			= (int)TokenNumber;
			Token = (*GetToken)(); m_Sy[number]			= (int)TokenNumber;
			Token = (*GetToken)(); m_Ex[number]			= (int)TokenNumber;
			Token = (*GetToken)(); m_Ey[number]			= (int)TokenNumber;
			Token = (*GetToken)(); m_TargetGate[number] = (int)TokenNumber;
			Token = (*GetToken)(); m_Dir[number]		= (int)TokenNumber;
			Token = (*GetToken)(); m_Level[number]		= (int)TokenNumber;
		}
	}
	fclose(SMDFile);
	LogAdd("[%s] Gate information data load complete.", filename);
}

#ifdef AUTH_GAMESERVER
void CGate::Load(char *Buffer, int iSize)
{
	CWzMemScript	WzMemScript;
	WzMemScript.SetBuffer(Buffer, iSize);

	Init();
	
	SMDToken Token;

	int number;

	while( true )
	{	
		Token = WzMemScript.GetToken();
		if(Token == END) break;
		if(Token == NUMBER)
		{		
			number = WzMemScript.GetNumber();
			m_This[number] = number;
			Token = WzMemScript.GetToken(); m_Flag[number]		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_MapNumber[number]	= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Sx[number]			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Sy[number]			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Ex[number]			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Ey[number]			= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_TargetGate[number] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Dir[number]		= WzMemScript.GetNumber();
			Token = WzMemScript.GetToken(); m_Level[number]		= WzMemScript.GetNumber();
		}
	}	
	LogAdd("Gate information data load complete.");
}
#endif

BOOL CGate::IsGate(int GateNumber)
{
	if( GateNumber < 0 || GateNumber > MAX_GATE-1 ) 
	{
		LogAdd(lMsg.Get(451),GateNumber, __FILE__, __LINE__);
		return FALSE;
	}

	if( m_This[GateNumber] == 0xFF ) {
		LogAdd(lMsg.Get(452),__FILE__, __LINE__);
		return FALSE;
	}
	return TRUE;
}

int CGate::GetGate(int mgt, short &x, short &y, BYTE & MapNumber, BYTE & dir, short & Level)
{
	if( IsGate(mgt) == FALSE ) return -1;

	int gt = m_TargetGate[mgt];

	if( gt == 0 ) gt = mgt;
	int tx, ty;
	int loopcount=10;
	BYTE attr;

	while( loopcount-- )
	{
		if( (m_Ex[gt]-m_Sx[gt]) > 0 )
		{
			tx = m_Sx[gt]+(rand()%((m_Ex[gt]-m_Sx[gt])));
		}
		else tx = m_Sx[gt];

		if( (m_Ey[gt]-m_Sy[gt]) > 0 )
		{
			ty = m_Sy[gt]+(rand()%((m_Ey[gt]-m_Sy[gt])));
		}
		else ty = m_Sy[gt];

		attr = MapC[MapNumber].GetAttr(tx, ty);
		if( ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) && 
			((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
			((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
		{			
			break;
		}
	}
	x			= tx;
	y			= ty;
	MapNumber	= m_MapNumber[gt];
	dir			= m_Dir[gt];
	Level		= m_Level[gt];
	return gt;
}

// 게이트의 레벨을 얻는다
int CGate::GetLevel(int GateNumber)
{
	if( GateNumber < 0 || GateNumber > MAX_GATE-1 ) 
	{
		LogAdd(lMsg.Get(451),GateNumber, __FILE__, __LINE__);
		return -1;
	}
		
	return m_Level[GateNumber];
}

BOOL CGate::IsInGate(int aIndex, int GateNumber)
{
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !IsGate(GateNumber) )
	{
#ifdef MODIFY_CONNNECT_HACK_ATTACK_20090409
		// 맵이동시 오류 난 유저
		LogAddTD_TempLog(", [Error GateNumber], Account [%s], IP [%s], Name [%s], GateNumber [%d]", 
			lpObj->AccountID, lpObj->Ip_addr,lpObj->Name, GateNumber);
#endif
		return FALSE;
	}
	
	int level = m_Level[GateNumber];

#ifdef DARKLORD_WORK
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// 평온의 늪은 마검사, 다크로드라도 Movereq.txt 파을에 따라 입장
	if( GateNumber != 273 )
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	
	if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else				
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{
		if( level > 0 )
		{
			level = ((level/3)*2);
		}
	}
	
#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[15] != AUTH_CHECK_15 )
		DestroyGIocp();
#endif

	if( lpObj->Level < level )
	{
		return FALSE;
	}

	if( lpObj->MapNumber != m_MapNumber[GateNumber]	 )
	{
		return FALSE;
	}

	if( lpObj->X < m_Sx[GateNumber]-5 || lpObj->X > m_Ex[GateNumber]+5 ||
		lpObj->Y < m_Sy[GateNumber]-5 || lpObj->Y > m_Ey[GateNumber]+5 )
	{
		return FALSE;
	}

	return TRUE;
}


#ifdef MAP_SERVER_WORK_20041030		// 특정 게이트번호로 이동 가능한지 체크
BOOL CGate::CheckGateLevel(int aIndex, int GateNumber)
{
	LPOBJECTSTRUCT lpObj;
	lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !IsGate(GateNumber) )
	{
		return FALSE;
	}
	
	int level = m_Level[GateNumber];

#ifdef DARKLORD_WORK
#ifdef UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108	// 평온의 늪은 마검사, 다크로드라도 Movereq.txt 파을에 따라 입장
	if( GateNumber != 273 )
#endif	// UPDATE_NEWMAP_SWAMP_OF_CALMNESS_20080108
	if (lpObj->Class == CLASS_DARKLORD||lpObj->Class == CLASS_MAGUMSA) 
#else				
	if( lpObj->Class == CLASS_MAGUMSA )
#endif
	{
		if( level > 0 )
		{
			level = ((level/3)*2);
		}
	}
	
	if( lpObj->Level < level )
	{
		return FALSE;
	}

	return TRUE;
}
#endif

