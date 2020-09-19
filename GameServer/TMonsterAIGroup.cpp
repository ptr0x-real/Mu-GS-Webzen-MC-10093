#include "Stdafx.h"
#include "TMonsterAIGroup.h"

#include "Gamemain.h"

#include "..\Include\ReadScript.h"
#include "..\Common\WinUtil.h"

#include "TMonsterSkillManager.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725

//----------------------------------------------------------------------------------------

BOOL					TMonsterAIGroup::s_bDataLoad = FALSE;
TMonsterAIGroupMember	TMonsterAIGroup::s_MonsterAIGroupMemberArray[MAX_MONSTER_AI_GROUP][MAX_MONSTER_AI_GROUP_MEMBER];
int						TMonsterAIGroup::s_iMonsterAIGroupMemberCount[MAX_MONSTER_AI_GROUP];

//----------------------------------------------------------------------------------------

TMonsterAIGroup::TMonsterAIGroup()
{
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	// 초기화는 이곳에서
	DelAllGroupInfo();
#endif
}
TMonsterAIGroup::~TMonsterAIGroup()
{

}


int TMonsterAIGroup::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster AI Group] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster AI Group] - Can't Open %s ", lpszFileName);
			return false;
		}
		
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		// 이미 생성된 몹의 index가 삭제된다.
		//DelAllGroupInfo();
#else
		DelAllGroupInfo();
#endif

		SMDToken Token;
		
		int iType = -1;
		
		while( TRUE )
		{
			Token = GetToken();
			if(Token == END) break;
			iType = (int)TokenNumber;

			while( TRUE )
			{				
				if( iType == 0 )
				{	
					// 1. >	데이터 초기화
					int	iGroupNumber	= _INVALID;
					int iGuid			= _INVALID;
					int iClass			= _INVALID;
					int iRank			= _INVALID;
					int iStartAI		= _ZERO;
					int iAI01			= _ZERO;
					int iAI02			= _ZERO;
					int iAI03			= _ZERO;
					int iCreateType		= _INVALID;
					int iMapNumber		= _INVALID;
					int iStartX			= _INVALID;
					int iStartY			= _INVALID;
					int iStartDir		= _INVALID;
					int iRegenType		= _INVALID;
				
					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					iGroupNumber	= (int)TokenNumber;

					Token = GetToken(); iGuid			= (int)TokenNumber;
					Token = GetToken(); iClass			= (int)TokenNumber;
					Token = GetToken(); iRank			= (int)TokenNumber;
					Token = GetToken();	iStartAI		= (int)TokenNumber;
					Token = GetToken();	iAI01			= (int)TokenNumber;
					Token = GetToken();	iAI02			= (int)TokenNumber;
					Token = GetToken();	iAI03			= (int)TokenNumber;
					Token = GetToken();	iCreateType		= (int)TokenNumber;
					Token = GetToken();	iMapNumber		= (int)TokenNumber;
					Token = GetToken();	iStartX			= (int)TokenNumber;
					Token = GetToken();	iStartY			= (int)TokenNumber;
					Token = GetToken();	iStartDir		= (int)TokenNumber;
					Token = GetToken();	iRegenType		= (int)TokenNumber;

					// 3. >	데이터 유효성 검사
					if( iGroupNumber > MAX_MONSTER_AI_GROUP )
					{
						MsgBox("[Monster AI Group] Exceed Max GroupNumber %d", iGroupNumber );
						continue;
					}
					
					if( iGuid > MAX_MONSTER_AI_GROUP_MEMBER )
					{
						MsgBox("[Monster AI Group] Exceed Max GroupMember %d, %d ", iGroupNumber, iGuid);
						continue;
					}
					
					// 4. > 데이터 세팅
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGroupNumber		=	iGroupNumber;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGuid			=	iGuid;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iClass			= 	iClass;			
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iRank			=	iRank;		
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartAI			=	iStartAI;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI01			=	iAI01;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI02			=	iAI02;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI03			=	iAI03;
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iCreateType		= 	iCreateType;	
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iMapNumber		=	iMapNumber;	
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartX			=	iStartX;		
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartY			=	iStartY;		
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartDir		=	iStartDir;	
					s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iRegenType		=	iRegenType;
					
					s_iMonsterAIGroupMemberCount[iGroupNumber]++;
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster AI Group ] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster AI Group] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

void TMonsterAIGroup::ChangeAIOrder( int iGroupNumber, int iAIOrder )
{
	if( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] ChangeAIOrder() Error - (GroupNumber=%d)", iGroupNumber );
		return;
	}

	TMonsterAIGroupMember *lpMemb = NULL; 

	for( int j=0; j<s_iMonsterAIGroupMemberCount[iGroupNumber]; j++ )
	{
		lpMemb = &s_MonsterAIGroupMemberArray[iGroupNumber][j];
		
		if( lpMemb->m_iGroupNumber == iGroupNumber )
		{
			if( iAIOrder == MAG_AI_ORDER_DEFAULT )
			{	
				gObj[lpMemb->m_iObjIndex].m_iBasicAI	= lpMemb->m_iStartAI;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI	= lpMemb->m_iStartAI;
			}
			else if( iAIOrder == MAG_AI_ORDER_FIRST )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI	= lpMemb->m_iAI01;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI	= lpMemb->m_iAI01;
			}
			else if( iAIOrder == MAG_AI_ORDER_SECOND )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI	= lpMemb->m_iAI02;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI	= lpMemb->m_iAI02;
			}
			else if( iAIOrder == MAG_AI_ORDER_THIRD )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI	= lpMemb->m_iAI03;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI	= lpMemb->m_iAI03;
			}
		}
	}
}

int	TMonsterAIGroup::DelGroupInstance( int iGroupNumber )
{
	if( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] DelGroup() Error - (GroupNumber=%d)", iGroupNumber );
		return NULL;
	}

	TMonsterAIGroupMember *lpMemb = NULL; 

	for( int j=0; j<s_iMonsterAIGroupMemberCount[iGroupNumber]; j++ )
	{
		lpMemb = &s_MonsterAIGroupMemberArray[iGroupNumber][j];
		
		if( lpMemb->m_iGroupNumber == iGroupNumber 
			&& lpMemb->m_iObjIndex!=_INVALID )
		{
			gObjDel(lpMemb->m_iObjIndex);
			
		}
	}

	return 0;
}

int	TMonsterAIGroup::DelAllGroupInfo()
{
	for( int i=0; i<MAX_MONSTER_AI_GROUP; i++ )
	{
		for( int j=0; j<MAX_MONSTER_AI_GROUP_MEMBER; j++ )
		{
			s_MonsterAIGroupMemberArray[i][j].Reset();
		}
		
		s_iMonsterAIGroupMemberCount[i] = _ZERO;
	}

	s_bDataLoad	= FALSE;
	return 0;
}

int	TMonsterAIGroup::FindGroupLeader( int iGroupNumber )
{
	if( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupLeader() Error - (GroupNumber=%d)", iGroupNumber );
		return NULL;
	}

	for( int i=0; i<MAX_MONSTER_AI_GROUP_MEMBER; i++ )
	{
		// Rank 0 이 리더!
		if( s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iRank == 0 )
			return s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex;
	}
	
	return _INVALID;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
int	TMonsterAIGroup::FindGroupMemberObjectIndex( int iGroupNumber, int iGuid )
{
	if( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupMemberObjectIndex() Error - (GroupNumber=%d Guid=%d)", iGroupNumber, iGuid );
		return NULL;
	}

	for( int i=0; i<MAX_MONSTER_AI_GROUP_MEMBER; i++ )
	{
		if( s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGuid == iGuid )
			return s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex;
	}
	
	return _INVALID;
}
#endif	// MODIFY_NEW_MONSTER_AI_01_2006724

TMonsterAIGroupMember* TMonsterAIGroup::FindGroupMember( int iGroupNumber, int iGuid )
{
	if( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupMember() Error - (GroupNumber=%d)(Guid=%d) ", iGroupNumber, iGuid);
		return NULL;
	}

	if( s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGroupNumber == iGroupNumber 
		&& s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGuid == iGuid )
	{
		return &s_MonsterAIGroupMemberArray[iGroupNumber][iGuid];
	}
	else
		LogAddTD("[Monster AI Group] FindGroupMember() Error - (GroupNumber=%d)(Guid=%d) ", iGroupNumber, iGuid);
	
	return NULL;
}

TMonsterAIGroupMember*  TMonsterAIGroup::FindGroupMemberToHeal( int iObjIndex, int iGroupNumber, int iGuid, int iDistance )
{
	if( s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
		return NULL;
	
	// 치료가 최우선 적으로 필요한 그룹 멤버를 찾는다.

	int iMinHPGuid = _INVALID;
	int iMinHP	   = 100000000;

	int iCount = s_iMonsterAIGroupMemberCount[iGroupNumber];

	for( int i=0; i<iCount; i++ )
	{
		if( s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGroupNumber == iGroupNumber )
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex];
			
			if( lpTargetObj == NULL )
				continue;

			// 2. > 일정 거리 안에 있는 그룹 멤버만 적용 된다.
			if( gObjCalDistance(&gObj[iObjIndex], lpTargetObj) > iDistance )
				continue;

			// 1. > 자기 자신도 Heal
			int iMaxLife   = lpTargetObj->MaxLife + 1;
			int iPercentHP = lpTargetObj->Life * 100 / iMaxLife;
			
			// 3. > 50% 미만이 경우에만 Heal을 준다.
			if( iPercentHP < 60 )
			{
				// 자기 자신을 우선적 치료!
				if( lpTargetObj->m_Index == iObjIndex )
				{
					iPercentHP = 1;
				}

				if( iPercentHP < iMinHP )
				{
					iMinHP = iPercentHP;
					iMinHPGuid = lpTargetObj->m_iGroupMemberGuid;
				}
			}
		}
	}

	if( iMinHPGuid != _INVALID )
	{
		return &s_MonsterAIGroupMemberArray[iGroupNumber][iMinHPGuid];
	}
	
	return NULL;
}

TMonsterAIGroupMember*	TMonsterAIGroup::FindGroupMemberToSommon( int iObjIndex, int iGroupNumber, int iGuid )
{
	if( s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
		return NULL;

	int iCount = s_iMonsterAIGroupMemberCount[iGroupNumber];
	
	for( int i=0; i<iCount; i++ )
	{
		if( s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGroupNumber == iGroupNumber )
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex];
			
			// 자기 자신은 제외
			if( lpTargetObj->m_Index == iObjIndex )
				continue;

			if( lpTargetObj == NULL )
				continue;
			
			if( lpTargetObj->m_iRegenType == _INVALID )
				continue;

			// 죽어 있으면 소환한다.
			if( lpTargetObj->Live == 0 )
			{
				return &s_MonsterAIGroupMemberArray[iGroupNumber][i];
			}
		}
	}
	
	
	return NULL;
}

void TMonsterAIGroup::InitAll()
{
	for( int i=0; i<MAX_MONSTER_AI_GROUP; i++ )
	{
		if( s_iMonsterAIGroupMemberCount[i] == 0 )
		{
			continue;
		}

		Init( i );

	}
}

void TMonsterAIGroup::Init( int iGroupNumber )
{
	if( s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
	{
		return;
	}
	
	TMonsterAIGroup::DelGroupInstance( iGroupNumber );

	// Regen Type을 보고 몬스터를 초기화 한다.	
	
	for( int j=0; j<MAX_MONSTER_AI_GROUP_MEMBER; j++ )
	{
		TMonsterAIGroupMember &Memb = s_MonsterAIGroupMemberArray[iGroupNumber][j];
		
		if( Memb.m_iGuid == _INVALID )
			continue;
		
		int iResult = gObjAddMonster(Memb.m_iMapNumber);
		
		if( iResult >= 0 )
		{
			gObj[iResult].m_PosNum				= _INVALID;
			gObj[iResult].MapNumber				= Memb.m_iMapNumber;
			gObj[iResult].Live					= 1;
		
			gObjViewportListProtocolDestroy(&gObj[iResult]);
			gObjViewportClose(&gObj[iResult]);
			
			//---------------------------------------------------------------------------
			// . > 소환자로 부터 특정 범위내로 리젠 시킨다.
			//---------------------------------------------------------------------------
			
			UCHAR cX, cY;
			
			// . > 시작 위치 기준 랜덤 생성 
			if( Memb.m_iCreateType == 1 )
			{
				int iRadius = 10;
				
				BOOL bGetPosition = FALSE;
				
				INT iCount = 100;
				while (iCount--) 
				{
					cX = rand() % (iRadius + 1) * (rand() % 2 ? 1 : -1) + Memb.m_iStartX;
					cY = rand() % (iRadius + 1) * (rand() % 2 ? 1 : -1) + Memb.m_iStartX;
					
					BYTE btMapAttr = MapC[Memb.m_iMapNumber].GetAttr(cX, cY);
					if( btMapAttr == MAP_ATTR_NONE )	
					{
						bGetPosition = TRUE;
						break;
					}
				}
				
				if (!bGetPosition) 
				{
					gObj[iResult].Live			= 0;
					gObj[iResult].m_State		= OBJST_DYING;	
					gObj[iResult].RegenTime		= GetTickCount();
					gObj[iResult].DieRegen		= 1;			
					return;
				}
			}
			// . > 시작 위치 생성
			else if( Memb.m_iCreateType == 0)
			{
				cX = Memb.m_iStartX;
				cY = Memb.m_iStartY;
			}
			
			gObj[iResult].X			= cX;
			gObj[iResult].Y			= cY;
			
			gObj[iResult].MTX		= gObj[iResult].X;
			gObj[iResult].MTY		= gObj[iResult].Y;
			gObj[iResult].TX		= gObj[iResult].X;
			gObj[iResult].TY		= gObj[iResult].Y;
			gObj[iResult].StartX	= (BYTE)gObj[iResult].X;
			gObj[iResult].StartY	= (BYTE)gObj[iResult].Y;
			//---------------------------------------------------------------------------
			
			gObjSetMonster(iResult, Memb.m_iClass);
			
			
			gObj[iResult].m_iGroupNumber		= Memb.m_iGroupNumber;
			gObj[iResult].m_iGroupMemberGuid	= Memb.m_iGuid;
			gObj[iResult].m_iCurrentAI			= Memb.m_iStartAI;
			gObj[iResult].m_iBasicAI			= Memb.m_iStartAI;
			gObj[iResult].m_iRegenType			= Memb.m_iRegenType;
			gObj[iResult].Dir					= Memb.m_iStartDir;
			gObj[iResult].m_State				= OBJST_CREATE;
			gObj[iResult].DieRegen				= 0;
			
			
			// 소환 및 리젠을 위해 iResult를 저장한다
			Memb.m_iObjIndex					= iResult;
			
			
			// . > 생성 안함
			if( Memb.m_iCreateType == -1 )
			{
				gObj[iResult].Live			= 0;
				gObj[iResult].m_State		= OBJST_DYING;	
				gObj[iResult].RegenTime		= GetTickCount();
				gObj[iResult].DieRegen		= 1;
				continue;
			}

#ifdef ADD_KANTURU_20060627
			LogAddTD( "[ KANTURU ][ SetAIMonster ] %s(Index:%d ObjIndex:%d) Map:%d-[%d][%d]",
					gObj[iResult].Name, gObj[iResult].Class, iResult, 
					gObj[iResult].MapNumber, gObj[iResult].X, gObj[iResult].Y );
#endif
		
		}
	}
	


}


#endif // ADD_NEW_MONSTER_SYSTEM_02_20050725