#include "Stdafx.h"

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714

#include "Include\ReadScript.h"
#include "Common\WinUtil.h"
#include "User.h"
#include "protocol.h"
#include "ObjUseSkill.h"
#include "SkillHitBox.h"

#include "TMonsterSkillManager.h"

BOOL								TMonsterSkillManager::s_bDataLoad = FALSE;
TMonsterSkillInfo					TMonsterSkillManager::s_MonsterSkillInfoArray[MAX_MONSTER_SKILLINFO];
ST_MONSTER_SKILL_DELAYTIME_INFO		TMonsterSkillManager::s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO];
TSync								TMonsterSkillManager::s_Sync;


TMonsterSkillManager::TMonsterSkillManager()
{

}

TMonsterSkillManager::~TMonsterSkillManager()
{
	
}

int TMonsterSkillManager::LoadData( LPSTR lpszFileName )
{
	s_bDataLoad = FALSE;

	if (lpszFileName == NULL || !strcmp(lpszFileName, "")) 
	{
		MsgBox("[Monster Skill Manager] - File load error : File Name Error");
		return FALSE;
	}

	try
	{		
		if((SMDFile=fopen(lpszFileName, "r")) == NULL)	
		{
			MsgBox("[Monster Skill Manager] - Can't Open %s ", lpszFileName);
			return false;
		}

		DelAllSkillManagerInfo();
		
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
					BOOL bVerified = TRUE;
					char szMonsterName[20] = {0,};
					TMonsterSkillInfo	MonsterSkillInfo;
	

					// 2. >	데이터 읽기
					Token = GetToken();
					if(strcmp("end",TokenString)==NULL) break;

					MonsterSkillInfo.m_iMonsterIndex = (int)TokenNumber;
					Token = GetToken(); CopyMemory(szMonsterName, TokenString, 20);

					for( int i=0; i<MAX_MONSTER_SKILLUNIT_SLOT; i++ )
					{
						int iSkillUnitNumber = _NONE;
						Token = GetToken(); MonsterSkillInfo.m_iSkillUnitTypeArray[i]	= (int)TokenNumber;
						Token = GetToken(); iSkillUnitNumber = (int)TokenNumber;

						if( iSkillUnitNumber == _NONE )
							continue;

						TMonsterSkillUnit *lpSkillUnit = TMonsterSkillUnit::FindSkillUnit( iSkillUnitNumber );
						
						// 3. >	데이터 유효성 검사
						if( lpSkillUnit == NULL )
						{
							bVerified = FALSE;
							continue;
						}
						
						MonsterSkillInfo.m_lpSkillUnitArray[i] = lpSkillUnit;
						MonsterSkillInfo.m_iSkillUnitCount++;
					}
					

					// 3. >	데이터 유효성 검사
					if( MAX_MONSTER_SKILLINFO<0 || MonsterSkillInfo.m_iMonsterIndex>=MAX_MONSTER_SKILLINFO )
					{
						MsgBox("[Monster Manager] - MonsterIndex(%d) Error (%s) File. ", MonsterSkillInfo.m_iMonsterIndex, lpszFileName);
						continue;
					}

					if( bVerified == FALSE )
					{
						LogAddC( 2, "[Monster Manager] - Invalid SkillInfo : MIndex(%s/%d)", 
							szMonsterName, MonsterSkillInfo.m_iMonsterIndex, lpszFileName);
					}
					
			
					// 4. > 데이터 세팅
					CopyMemory( &s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex], &MonsterSkillInfo, sizeof(MonsterSkillInfo) );

					char szMsg[200] = {0,};
					TMonsterSkillInfo* pMonSInfo = &s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex];

					wsprintf( szMsg, "○Index:%d,Name:%s,Count:%d,Type0:%d,Unit0:%d,Type1:%d,Unit1:%d,Type2:%d,Unit2:%d,Type3:%d,Unit3:%d,Type4:%d,Unit4:%d\n", 
						pMonSInfo->m_iMonsterIndex, szMonsterName, pMonSInfo->m_iSkillUnitCount, 
						pMonSInfo->m_iSkillUnitTypeArray[0], pMonSInfo->m_lpSkillUnitArray[0],pMonSInfo->m_iSkillUnitTypeArray[1], pMonSInfo->m_lpSkillUnitArray[1],
						pMonSInfo->m_iSkillUnitTypeArray[2], pMonSInfo->m_lpSkillUnitArray[2],pMonSInfo->m_iSkillUnitTypeArray[3], pMonSInfo->m_lpSkillUnitArray[3],
						pMonSInfo->m_iSkillUnitTypeArray[4], pMonSInfo->m_lpSkillUnitArray[4]);
					OutputDebugString(szMsg);			
					// T E S T
					
				}	
			}
		}
		fclose(SMDFile);
		
		LogAddC( 2, "[Monster Skill Manager] - %s file is Loaded", lpszFileName);

		s_bDataLoad = TRUE;
	
	}
	catch(...) 
	{
		MsgBox("[Monster Skill Manager] - Loading Exception Error (%s) File. ", lpszFileName);
	}
	
	return 0;
}

void TMonsterSkillManager::DelAllSkillManagerInfo()
{
	for( int i=0; i<MAX_MONSTER_SKILLINFO; i++ )
	{
		s_MonsterSkillInfoArray[i].Reset();
	}

	for( int j=0; j<MAX_MONSTER_SKILL_DELAY_INFO; j++ )
	{
		s_MonsterSkillDelayInfoArray[j].RESET();
	}
}

BOOL TMonsterSkillManager::CheckMonsterSkill( int iMonsterClass )
{
	if( iMonsterClass < _ZERO || iMonsterClass > MAX_MONSTER_SKILLINFO )
		return FALSE;

	TMonsterSkillInfo* lpMonsterSkillInfo = &s_MonsterSkillInfoArray[iMonsterClass];

	if( !lpMonsterSkillInfo->IsValid() )
		return FALSE;

	return TRUE;
}

BOOL TMonsterSkillManager::AddMonsterSkillDelayInfo( INT iIndex, INT iTargetIndex, INT iDelayTime, TMonsterSkillUnit *lpMonsterSkillUnit )
{
	for( int i=0; i<MAX_MONSTER_SKILL_DELAY_INFO; i++ )
	{
		if( s_MonsterSkillDelayInfoArray[i].bIsUsed == FALSE )
		{
			s_MonsterSkillDelayInfoArray[i].iIndex				= iIndex;
			s_MonsterSkillDelayInfoArray[i].iTargetIndex		= iTargetIndex;
			s_MonsterSkillDelayInfoArray[i].dwDelayTime			= GetTickCount() + iDelayTime;	
			s_MonsterSkillDelayInfoArray[i].lpMonsterSkillUnit	= lpMonsterSkillUnit;

			s_MonsterSkillDelayInfoArray[i].bIsUsed				= TRUE;
			
			return TRUE;
		}
	}

	LogAddC( 2, "[Monster Skill Manager] - (MonsterSkillDelayInfo) Slot Full!! " );
	
	return FALSE;
}

void TMonsterSkillManager::MonsterSkillProc()
{
	DWORD dwCurrentTick = GetTickCount();

	for( int i=0; i<MAX_MONSTER_SKILL_DELAY_INFO ; i++ )
	{
		if( s_MonsterSkillDelayInfoArray[i].bIsUsed == TRUE )
		{
			if( dwCurrentTick < s_MonsterSkillDelayInfoArray[i].dwDelayTime )
			{
				ST_MONSTER_SKILL_DELAYTIME_INFO& stInfo =  s_MonsterSkillDelayInfoArray[i];

				// 2. > 유효성 검사
				if( !stInfo.IsValid() )
				{
					s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				LPOBJECTSTRUCT	lpObj		= &gObj[stInfo.iIndex];
				LPOBJECTSTRUCT	lpTargetObj = &gObj[stInfo.iTargetIndex];

				// 3. > 스킬 처리
				if( !lpObj->Live || !lpTargetObj->Live )
				{
					s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}
				
				if( !gObjIsConnected(lpTargetObj) )
				{
					s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
				stInfo.lpMonsterSkillUnit->RunSkill( stInfo.iIndex, stInfo.iTargetIndex );

				// 4. > 리스트에서 제거
				s_MonsterSkillDelayInfoArray[i].RESET();
			}
		}

	}

}

TMonsterSkillUnit* TMonsterSkillManager::FindMonsterSkillUnit( int iIndex, int iMonsterSkillUnitType)
{
	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	TMonsterSkillInfo* lpMonsterSkillInfo = &s_MonsterSkillInfoArray[lpObj->Class];

	// 1. > 스킬 사용 가능성 검사
	if( !lpMonsterSkillInfo->IsValid() )
		return NULL;

	int iFoundSkillArray[MAX_MONSTER_SKILLUNIT_SLOT] ={ _INVALID, };
	int iFoundSkillCount = 0;

	// 특정 몬스터 스킬을 사용한다.
	for( int i=0; i<MAX_MONSTER_SKILLUNIT_SLOT; i++ )
	{
		if( iMonsterSkillUnitType == lpMonsterSkillInfo->m_iSkillUnitTypeArray[i] )
		{
			iFoundSkillArray[iFoundSkillCount] = i;
			iFoundSkillCount++;
		}
	}

	if( iFoundSkillCount > 0 )
	{
		int iSlot = iFoundSkillArray[ rand() % iFoundSkillCount ];

		if( iSlot == _INVALID )
			return NULL;

		TMonsterSkillUnit *lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[ iSlot ];
		return lpMonsterSkillUnit;
	}
	
	return NULL;
}

#ifdef ADD_RAKLION_20080408
void TMonsterSkillManager::UseMonsterSkill( int iIndex, int iTargetIndex, int iMonsterSkillUnitType, int iUnitNumber, CMagicInf *lpMagic)
#else
void TMonsterSkillManager::UseMonsterSkill( int iIndex, int iTargetIndex, int iMonsterSkillUnitType)
#endif // ADD_RAKLION_20080408
{
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
	if( iIndex >= MAX_MONSTER || iIndex < 0 )
	{
		LogAddC( LOGC_RED, "[TMonsterSkillManager][UserMonsterSkill] error: iIndex(%d)", iIndex );
		return;
	}
	if( iTargetIndex >= MAX_OBJECT || iTargetIndex < 0 )
	{
		LogAddC( LOGC_RED, "[TMonsterSkillManager][UserMonsterSkill] error: iTargetIndex(%d)", iTargetIndex );
		return;
	}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

	LPOBJECTSTRUCT lpObj		= &gObj[iIndex];
	LPOBJECTSTRUCT lpTargetObj	= &gObj[iTargetIndex];
	
	TMonsterSkillInfo* lpMonsterSkillInfo = &s_MonsterSkillInfoArray[lpObj->Class];
	
	// 1. > 스킬 사용 가능성 검사
	if( !lpMonsterSkillInfo->IsValid() )
		return;

	// 2. > SkillUnitType 이 공격일 경우 기본 공격을 이용해 gObjAttack 함수를 타게 한다.
	
#ifdef ADD_RAKLION_20080408
	TMonsterSkillUnit *lpMonsterSkillUnit = NULL;
	if( iUnitNumber > -1 )		// 지정된 유닛의 스킬을 사용한다.
	{
		int iSkillUnitSlot = iUnitNumber;
		lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iSkillUnitSlot];
	}
	else						// 유닛 번호가 지정되어 있지 않으면 기존의 방식대로...
		lpMonsterSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( iIndex, iMonsterSkillUnitType );
#else
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// 2. 1 > Slot을 검색한다.
	
	TMonsterSkillUnit *lpMonsterSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit( iIndex, iMonsterSkillUnitType );
	
#else 
	int iSkillUnitSlot = rand()%lpMonsterSkillInfo->m_iSkillUnitCount;
	TMonsterSkillUnit *lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iSkillUnitSlot];
#endif //ADD_NEW_MONSTER_SYSTEM_02_20050725
#endif // ADD_RAKLION_20080408

	
	if( lpMonsterSkillUnit == NULL )
	{
#ifdef ADD_RAKLION_20080408
		LogAddC(LOGC_RED, "[Monster Skill Manager] SkillUnit is NULL : ObjIndex[%d], SkillUnitType[%d] ", iIndex, iMonsterSkillUnitType );
#else
		LogAddC(LOGC_RED, "[Monster Skill Manager] SkillUnit is NULL ");
#endif // ADD_RAKLION_20080408
		return;
	}

	GCUseMonsterSkillSend( lpObj, lpTargetObj, lpMonsterSkillUnit->m_iUnitNumber );	

#if TESTSERVER == 1
	MsgOutput(lpTargetObj->m_Index, "[USEMONSTERSKILL] %d ", lpMonsterSkillUnit->m_iUnitNumber);
	//DebugLog("[USEMONSTERSKILL] %d \n", lpMonsterSkillUnit->m_iUnitNumber);
#endif


	// 3. > 스킬 사용 범위 체크
	if( lpMonsterSkillUnit->m_iUnitScopeType == MSU_SCOPE_NONE )
	{
		// 4. > 스킬 처리 큐잉
//		AddMonsterSkillDelayInfo( iIndex, iTargetIndex, lpMonsterSkillUnit->m_iDelay, lpMonsterSkillUnit );
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
		// 일반공격과 동시에 사용되지 않는 스킬도 존재한다.
		// 테스트를 위해 호출하지 않아본다.
		// 무조건 호출하지말고, 각 스킬 별로 공격처리를 해주는게 좋을 것 같다.
		//gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
#else
		gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
#endif
		
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
		if( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
		{
			// 라클리온 필드의 몬스터들은 스킬 사용시 이곳에서 데미지 처리를 한다.
			gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
		}
		if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
		{
			// 보스존의 몬스터들 중 세루판은 이곳에서 처리한다.
			if( lpObj->Class == 459 )
			{
				// 얼리기일 경우 lpMagic은 NULL이므로 여기서 데미지 처리
				if( lpMonsterSkillUnit->m_iUnitNumber == 40 )
					gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
				else
				{
					// 세루판의 나머지 스킬들
					if( lpMagic != NULL )
						gObjAttack( lpObj, lpTargetObj, lpMagic, 0, 0 );
				}
			}
			else	// 보스존에서 세루판이 소환한 몬스터들
				gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
		}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710
		
		lpMonsterSkillUnit->RunSkill( iIndex, iTargetIndex );
	}
	else
	{
		int iCount = 0;
		int iRangeTargetIndex = -1;

		int iAngle = gObjUseSkill.GetAngle(lpObj->X, lpObj->Y, lpTargetObj->X, lpTargetObj->Y);

		while( TRUE )
		{
			if( lpObj->VpPlayer2[iCount].state )
			{
				iRangeTargetIndex = lpObj->VpPlayer2[iCount].number;
				
				if( CHECK_LIMIT(iRangeTargetIndex, MAX_OBJECT) && gObj[iRangeTargetIndex].Type == OBJTYPE_CHARACTER)
				{
					BOOL bTargetOK = FALSE;

					if( lpMonsterSkillUnit->m_iUnitScopeType == MSU_SCOPE_DISTANCE )
					{
						if( gObjCalDistance(lpObj, &gObj[iRangeTargetIndex]) < lpMonsterSkillUnit->m_iUnitScopeValue )
							bTargetOK = TRUE;
					}
					else if( lpMonsterSkillUnit->m_iUnitScopeType == MSU_SCOPE_TABLE_01 )
					{
						if( SkillElectricSparkHitBox.HitCheck(iAngle, lpObj->X, lpObj->Y, gObj[iRangeTargetIndex].X, gObj[iRangeTargetIndex].Y) )
							bTargetOK = TRUE;
					}
					else if( lpMonsterSkillUnit->m_iUnitScopeType == MSU_SCOPE_TABLE_02 )
					{
						bTargetOK = TRUE;
					}
					else if( lpMonsterSkillUnit->m_iUnitScopeType == MSU_SCOPE_TABLE_03 )
					{
						bTargetOK = TRUE;
					}

					if( bTargetOK )
					{
						// 4. > 스킬 처리 큐잉 
//						AddMonsterSkillDelayInfo( iIndex, iTargetIndex, lpMonsterSkillUnit->m_iDelay, lpMonsterSkillUnit );
#ifdef MODIFY_RAKLION_BUGFIX_2ND_20080710
						if( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
						{
							// 라클리온 필드의 몬스터들은 스킬 사용시 이곳에서 데미지 처리를 한다.
							gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
						}
						if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
						{
							// 보스존의 몬스터들 중 세루판은 이곳에서 처리한다.
							if( lpObj->Class == 459 )
							{
								// 얼리기일 경우 lpMagic은 NULL이므로 여기서 데미지 처리
								if( lpMonsterSkillUnit->m_iUnitNumber == 40 )
									gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
								else
								{
									// 세루판의 나머지 스킬들
									if( lpMagic != NULL )
										gObjAttack( lpObj, lpTargetObj, lpMagic, 0, 0 );
								}
							}
							else	// 보스존에서 세루판이 소환한 몬스터들
								gObjAttack( lpObj, lpTargetObj, NULL, 0, 0 );
						}
#endif // MODIFY_RAKLION_BUGFIX_2ND_20080710

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
						//gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
						lpMonsterSkillUnit->RunSkill( iIndex, iRangeTargetIndex );
#else						
						gObjAttack(lpObj, lpTargetObj, NULL, 0, 0);
						lpMonsterSkillUnit->RunSkill( iIndex, iTargetIndex );
#endif
					}
					
				}
			}
			iCount++;
			if( iCount > MAX_MONVIEWPORTOBJECT-1 )
			{
				break;
			}
		}


	}
}

#endif