#include "StdAfx.h"
#include <math.h>
#include <winsock2.h>
#include "Include\public.h"
#include "Include\ProDef.h"
#include "Include\SproDef.h"
#include "Include\GMProtocol.h"
#include "Common\ZzzPath.h"
#include "Common\Winutil.h"
#include "SProtocol.h"
#include "User.h"
#include "Gameserver.h"
#include "Gamemain.h"
#include "Protocol.h"
#include "WsGameServer.h"
#include "gObjMonster.h"
#include "MonsterSetBase.h"
#include "DSProtocol.h"
#include "Event.h"

#include "MonsterAttr.h"
#include "PartyClass.h"

#include "MonsterItemMng.h"

#include "EledoradoEvent.h"

#ifdef NEW_SKILL_FORSKYLAND
#include "QuestInfo.h"
#endif

#ifdef DEVILSQUARE_EXTEND_20050221
#include "DevilSquare.h"
#endif

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef MONSTER_HERD_SYSTEM_20031120
#include "MonsterHerd.h"
#endif

#include "Gamemain.h"

#ifdef EVENT_ITEMBAG_UPGRADE
#include "ItemBagEx.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// 공성관련 NPC 들 처리를 위함
#include "CastleSiege.h"
#endif

#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214
#include "Mercenary.h"
#endif

#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208
#include "LifeStone.h"
#endif

#ifdef ADD_KUNDUN_CONTROL_20040923
#include "LogToFile.H"
CLogToFile	KUNDUN_EVENT_LOG ("KUNDUN_EVENT_LOG", ".\\KUNDUN_EVENT_LOG");		// 로그파일 객체
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
#include "TMonsterSkillManager.h"
#endif

#ifdef MU_CRYWOLF_BENEFIT_20051215
#include "CrywolfSync.h"
#endif

extern CwsGameServer	wsGServer;
extern CMonsterSetBase	gMSetBase;

extern CMonsterAttr		gMAttr;
extern PartyClass		gParty;

#ifdef ADD_NEW_MAP_KALIMA_20040518				// 마석 처리함수 객체
#include "KalimaGate.h"
#endif

#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207	// 수호석상의 동작
#include "GuardianStatue.h"
#endif

#ifdef HAPPY_POUCH //happycat@20050201
#include "GameEvent.h"
#endif

#ifdef KANTURU_MONSTER_ITEMDROP_20060807
#include "KanturuDefine.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_RAKLION_20080408
#include "RaklionDefine.h"
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ADD_ITEM_SOCKET_OPTION_EXTEND_20080422
#include "ItemSocketOptionSystem.h"
#endif // ADD_ITEM_SOCKET_OPTION_EXTEND_20080422

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

#ifdef SECOND_QUEST
CQeustNpcTeleport	gQeustNpcTeleport;			// NPC에게 말건 사람의 참조 포인트
#endif

#ifdef EVENT_ITEMBAG_UPGRADE
extern CItemBagEx	*   StarOfXMasItemBag;		// 성탄의 별 아이템 가방(?)
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
extern CItemBagEx *	ChnThanksCookieEvent;		// 중국 월병 이벤트
#endif


BOOL PathFindMoveMsgSend(LPOBJECTSTRUCT lpObj);
int  gObjMonsterGetTargetPos(LPOBJECTSTRUCT lpObj);

void gObjMonsterBaseAct(LPOBJECTSTRUCT lpObj);
void gObjMonsterTrapAct(LPOBJECTSTRUCT lpObj);

void gObjMonsterDieGiveItem(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
void gObjMonsterAttack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj );
void gObjMonsterHitDamageInit(LPOBJECTSTRUCT lpObj);

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
void gObjMonsterMagicAttack(LPOBJECTSTRUCT lpObj, INT iMonsterClass = -1 );
#else
void gObjMonsterMagicAttack(LPOBJECTSTRUCT lpObj);
#endif

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
void gObjMonsterBeattackRecv( LPBYTE lpRecv, int aIndex );
#endif

BOOL gEventMonsterItemDrop(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);

#define MON_ATTR_0		0	// 공격 당하기만 하는 몬스터
#define MON_ATTR_1		1	//
#define MON_ATTR_2		2	//
#define MON_ATTR_100	100	// 소환몬스터


void gObjMonsterMoveRegen(int x, int y, LPOBJECTSTRUCT lpObj)
{
	gMSetBase.GetBoxPosition(lpObj->MapNumber, x-1, y-1, x+1, y+1, lpObj->X, lpObj->Y);
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	//LogAdd("위치 이동 %d %d", x, y);
}

BOOL gObjMonsterRegen(LPOBJECTSTRUCT lpObj)
{
	int map[3] = { 0, 2, 3};

	gObjViewportListProtocolDestroy(lpObj);
	gObjViewportClose(lpObj);

	lpObj->m_ActState.Attack		= 0;
	lpObj->m_ActState.EmotionCount	= 0;
	lpObj->m_ActState.Escape		= 0;
	lpObj->m_ActState.Move			= 0;
	lpObj->m_ActState.Rest			= 0;
	lpObj->m_ActState.Emotion		= 0;
	lpObj->TargetNumber				= -1;
	lpObj->NextActionTime			= 5000;
		
	if( lpObj->Class == 43 )
	{
		lpObj->MapNumber = map[rand()%3];

		if( !gMSetBase.GetBoxPosition(lpObj->MapNumber, 10, 10, 240, 240, lpObj->X, lpObj->Y) )
		{
			lpObj->Live	   = 0;
			lpObj->m_State   = OBJST_DYING;	
			lpObj->RegenTime = GetTickCount();
			lpObj->DieRegen  = 1;			
			return FALSE;
		}
		
		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;
		lpObj->MTX = lpObj->X;
		lpObj->MTY = lpObj->Y;
		lpObj->StartX = (BYTE)lpObj->X;
		lpObj->StartY = (BYTE)lpObj->Y;
	}
	else if( lpObj->Class == 44 )
	{
	}
	else if( lpObj->Class == 53 || lpObj->Class == 54 ||
			 lpObj->Class == 55 || lpObj->Class == 56)
	{
		/* /* (수정해라)
		lpObj->Live	   = 0;
		lpObj->m_State   = OBJST_DYING;	
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen  = 1;			
		*/
		return FALSE;
	}
	else if( lpObj->Class >= 78 && lpObj->Class <= 83 )
	{	// 이벤트 몬스터는 부활시키지 않는다		
		/*/* (수정해라)
		lpObj->Live	   = 0;
		lpObj->m_State   = OBJST_DYING;	
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen  = 1;			
		*/
		return FALSE;		
	}
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311	
	else if( lpObj->Class >= 493 && lpObj->Class <= 502 )
	{	// 이벤트 몬스터는 부활시키지 않는다		
		
		return FALSE;		
	}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
#ifdef BUGFIX_KALIMAGATE_VANISHING_20041006
	else if( CHECK_KALIMAGATE(lpObj->m_Attribute) ) {
		// 혹시라도 이곳에 들어오면 리젠값을 수정한다. (마석은 여기에 들어올 이유가 없다.)
		lpObj->Live			= 1;
		lpObj->DieRegen		= 0;
		CreateFrustrum(lpObj->X,lpObj->Y, lpObj->m_Index);
		gObjClearViewport(lpObj);
		gObjViewportListCreate(lpObj->m_Index);
		gObjViewportListProtocolCreate(lpObj);
		return FALSE;
	}
#endif
#ifdef ADD_RAKLION_20080408
	else if( lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 )
	{
		// 라클리온 거미알은 이곳에서 부활시키지 않는다		
		if( g_Raklion.GetRaklionState() != RAKLION_STATE_END )
			return FALSE;
	}
	else if( lpObj->Class == 459 )
	{
		// 라클리온 보스는 이곳에서 부활 시키지 않는다
		if( g_Raklion.GetRaklionState() != RAKLION_STATE_STANDBY
			|| g_Raklion.GetRaklionState() != RAKLION_STATE_READY )
			return FALSE;
	}
#ifdef MODIFY_RAKLION_COOLUTIN_AND_IRON_KNIGHT_REGEN_BUGFIX_20080828
	else if( lpObj->Class == RAKLION_COOLUTIN && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS
			|| lpObj->Class == RAKLION_IRON_KNIGHT && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS
			)
	{
		// 라클리온 보스가 소환한 몬스터는 여기서 부활 시키지 않는다
		return FALSE;
	}
#else
	else if( lpObj->Class == RAKLION_COOLUTIN || lpObj->Class == RAKLION_IRON_KNIGHT )
	{
		// 라클리온 보스가 소환한 몬스터는 여기서 부활 시키지 않는다
		if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS )
			return FALSE;
	}
#endif // MODIFY_RAKLION_COOLUTIN_AND_IRON_KNIGHT_REGEN_BUGFIX_20080828
#endif // ADD_RAKLION_20080408
	else 
	{
		if( !gMSetBase.GetPosition(lpObj->m_PosNum, lpObj->MapNumber, lpObj->X, lpObj->Y) )
		{
#ifdef CASTLEDEEP_EVENT_20050413
			if (lpObj->m_Attribute == MON_ATTR_CASTLEDEEP) {
				// 로랜협곡 몬스터는 자신의 마지막위치 주변에 랜덤하게 출현시킨다.
				BYTE btX, btY;
				btX = lpObj->X;
				btY = lpObj->Y;
				if (gObjGetRandomFreeLocation(lpObj->MapNumber, btX, btY, 5, 5, 30) == FALSE) {
					return FALSE;
				}
				lpObj->X = btX;
				lpObj->Y = btY;
			}
			else {
#endif
			lpObj->Live	   = 0;
			lpObj->m_State   = OBJST_DYING;	
			lpObj->RegenTime = GetTickCount();
			lpObj->DieRegen  = 1;			
			return FALSE;
#ifdef CASTLEDEEP_EVENT_20050413
			}
#endif
		}

		lpObj->MTX		= lpObj->X;
		lpObj->MTY		= lpObj->Y;
		lpObj->TX		= lpObj->X;
		lpObj->TY		= lpObj->Y;
		lpObj->StartX	= (BYTE)lpObj->X;
		lpObj->StartY	= (BYTE)lpObj->Y;
	}	
	gObjMonsterHitDamageInit(lpObj);

#ifdef MU_CRYWOLF_BENEFIT_20051215
	if( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE 
		&& g_CrywolfSync.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_2
		&& g_CrywolfSync.GetCrywolfState() <= CRYWOLF_STATE_END			)
	{
		// MVP 몹은 체력 하락이 적용되지 않는다.
	}
	else
	{
		// MVP 성공 : 몬스터 체력 감소
		if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE 
			&& g_iCrywolfApplyMvpBenefit	
		  )
		{
			
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
			lpObj->Life		= lpObj->m_iScriptMaxLife	* g_CrywolfSync.GetMonHPBenefitRate() / 100;
			lpObj->MaxLife	= lpObj->m_iScriptMaxLife	* g_CrywolfSync.GetMonHPBenefitRate() / 100;
#else
			lpObj->Life		= lpObj->Life				* g_CrywolfSync.GetMonHPBenefitRate() / 100;
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315

#if TESTSERVER == 1
			LogAddTD( "★☆ 크라이울프 몬스터 혜택 적용 (%d)[%s] : %f / %f / %d", 
				lpObj->m_Index, lpObj->Name, lpObj->Life, lpObj->MaxLife, lpObj->m_iScriptMaxLife );
#endif // TESTSERVER == 1
	
		}
	}
#endif // MU_CRYWOLF_BENEFIT_20051215


	return TRUE;
	//gObjViewportListProtocolCreate(lpObj);
}

// 몬스터의 뷰포트에 캐릭터가 존재하는지..
int gObjMonsterViewportIsCharacter(LPOBJECTSTRUCT lpObj)
{
	int tObjNum;

	for(int n=0; n<MAX_MONVIEWPORTOBJECT; n++)
	{
		if( lpObj->VpPlayer2[n].state )
		{
			tObjNum = lpObj->VpPlayer2[n].number;
			if( tObjNum == lpObj->TargetNumber ) // 대상 유저와 같다면..
				return n;
		}
	}
	return -1;
}

void gObjMonsterHitDamageInit(LPOBJECTSTRUCT lpObj)
{
	for( int i=0; i<MAX_HITDAMAGE; i++)
	{
		lpObj->sHD[i].number = -1;
	}
	lpObj->sHDCount = 0;
}

// -타격을 한지 오래된 유저
// -접속을 종료한 사용자
// -다른 맵에있는 사용자
// -거리가 먼 사용자를 삭제한다.
int gObjMonsterHitDamageUserDel(LPOBJECTSTRUCT lpObj)
{
	int delok=0;
	int delindex=-1;

	for(int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpObj->sHD[n].number >= 0 )
		{
			delok = 0;
			// 접속해 있고..
			if( gObjIsConnected(lpObj->sHD[n].number) == TRUE )
			{
				// 마지막 타격이 20초 지난 사용자는 잊어버린다.
				if( (GetTickCount()-(lpObj->sHD[n].LastHitTime)) > (1000*20) )
				{
					delok = 1;
				}
				// 같은 맵에 위치하지 않았다면..
				if( gObj[lpObj->sHD[n].number].MapNumber != lpObj->MapNumber )
				{
					delok = 1;
				}
				else	// 같은 맵에 있다면.. 위치를 체크한다.
				{
					if( gObjCheckTileArea(	lpObj->m_Index, gObj[lpObj->sHD[n].number].X, gObj[lpObj->sHD[n].number].Y, 20) == FALSE )
					{
						delok = 1;	// 뷰 포트에 없다.
					}
				}
			}
			else delok = 1;	// 접속해 있지 않다면			
			if( delok == 1 ) {
				lpObj->sHD[n].number = -1;
				delindex = n;
				lpObj->sHDCount--;
			}
		}
	}
	return delindex;
}


//////////////////////////////////////////////////////////////////////////////
// 경험치 배분을 위해 몬스터에 타격을 가한 유저와 타격 값을 등록한다.
// lpObj		: 몬스터
// hit_player	: 타격을 가한 유저
// hit_damage	: 타격 값
void gObjMonsterSetHitDamage(LPOBJECTSTRUCT lpObj, int hit_player, int hit_damage )
{
	int blank=-1;

	for(int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpObj->sHD[n].number >= 0 )
		{
			if( lpObj->sHD[n].number == hit_player )
			{
				lpObj->sHD[n].HitDamage   += hit_damage;
				lpObj->sHD[n].LastHitTime  = GetTickCount();				

				if( lpObj->sHD[n].HitDamage > lpObj->MaxLife )
					lpObj->sHD[n].HitDamage = (int)lpObj->MaxLife;
				return;
			}
		}
		else {
			if( blank < 0 ) blank = n;
		}
	}

	if( blank < 0 )
	{
		// 빈 공간이 부족하다.. 공간을 확보하기 위해 지운다.
		blank = gObjMonsterHitDamageUserDel(lpObj);
	}
	
	// 기존 사용자가 아닐땐 빈공간에 새로 추가한다...
	// 이때.. 빈 공간이 없으면 추가할수 없기때문에.. 시간이 오래된 유저를 삭제한다.
	if( blank >= 0 )
	{
		lpObj->sHD[blank].number		= hit_player;
		lpObj->sHD[blank].HitDamage		= hit_damage;
		if( lpObj->sHD[blank].HitDamage > lpObj->MaxLife )
			lpObj->sHD[blank].HitDamage = (int)lpObj->MaxLife;
		lpObj->sHD[blank].LastHitTime	= GetTickCount();
		lpObj->sHDCount++;
	}
	else 
	{
		LogAdd("error-L2 : Hit Damage Set error");
	}
}

int gObjMonsterTopHitDamageUser(LPOBJECTSTRUCT lpMonObj)
{
//	int tObjNum;
	int MaxHitDamage=0;
	int MaxHitDamageUser=-1;
	
	for( int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpMonObj->sHD[n].number >= 0 )
		{
			if( lpMonObj->sHD[n].HitDamage > MaxHitDamage ) 
			{
				MaxHitDamage = lpMonObj->sHD[n].HitDamage;
				MaxHitDamageUser = lpMonObj->sHD[n].number;
			}
		}
	}
	return MaxHitDamageUser;
}


// 마지막으로 죽인 캐릭터를 찾는다
int gObjMonsterLastHitDamageUser(LPOBJECTSTRUCT lpMonObj, int& hitindex)
{
//	int tObjNum;
	DWORD LastHitTime=0;
	int LastHitUser=-1;
	
	for( int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpMonObj->sHD[n].number >= 0 )
		{
			if( lpMonObj->sHD[n].LastHitTime > LastHitTime ) 
			{
				LastHitTime = lpMonObj->sHD[n].LastHitTime;
				LastHitUser = lpMonObj->sHD[n].number;
				hitindex = n;
			}
		}
	}
	return LastHitUser;
}


// 몬스터가 해당 위치로 이동해도 되는지 체크하여 이동해도 되면 TRUE를 
// 이동할 수 없다면 FALSE를 리턴한다.
BOOL gObjMonsterMoveCheck(LPOBJECTSTRUCT lpObj, int tx, int ty)
{
	// 열받았을땐 이동해도 된다.
	if( lpObj->m_ActState.Emotion == 1 ) return TRUE;

#ifdef ADD_KANTURU_20060627
	if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->m_PosNum < 0 )
	{
		tx -= lpObj->StartX;
		ty -= lpObj->StartY;

		int dis = ( int )sqrt( (double)( tx * tx ) + ( ty * ty ) );

		if( dis > 30 ) 
		{
			LogAddTD( "[ KANTURU ][ Debug - m_PosNum ] Fail %s(Index:%d) X%d-Y%d -> X%d-Y%d(%d)",
				lpObj->Name, lpObj->Class, lpObj->StartX, lpObj->StartY, tx, ty, dis );
			return FALSE;		
		}
	}
	else
	{
		LPMONSTER_POSITION lpPos = (LPMONSTER_POSITION)&gMSetBase.m_Mp[lpObj->m_PosNum];

		if( lpPos->m_Dis < 1 ) return FALSE;

		tx -= lpObj->StartX;
		ty -= lpObj->StartY;

		int dis = (int)sqrt((double)(tx*tx)+(ty*ty));

		if( dis > lpPos->m_Dis ) return FALSE;
	}
#else
	LPMONSTER_POSITION lpPos = (LPMONSTER_POSITION)&gMSetBase.m_Mp[lpObj->m_PosNum];

	if( lpPos->m_Dis < 1 ) return FALSE;

	tx -= lpObj->StartX;
	ty -= lpObj->StartY;

	int dis = (int)sqrt((double)(tx*tx)+(ty*ty));

	if( dis > lpPos->m_Dis ) return FALSE;
#endif

	return TRUE;
}

// 상대방과 가장 가까운 위치를 얻는다.
int gObjMonsterGetTargetPos(LPOBJECTSTRUCT lpObj)
{
	int tpx, tpy, mtx, mty;
	int searchp=0, sn=0;
	int searchcount=7;
	BYTE attr;
	BOOL result;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( lpObj->m_MoveRange == 0
		&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true )
		&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true )
	  )
	#else
	if( lpObj->m_MoveRange == 0 && lpObj->m_SkillHarden != 0 && lpObj->m_iSkillStunTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return FALSE;
#else
	#ifdef NEW_FORSKYLAND2
	if( lpObj->m_MoveRange == 0 && lpObj->m_SkillHarden != 0 )
		return FALSE;
	#else
	if( lpObj->m_MoveRange == 0 ) return FALSE;
	#endif

#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태 상대의 위치를 못얻음
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		return FALSE;
	}
#endif
	
	LPOBJECTSTRUCT lpTargetObj;

	if( !CHECK_LIMIT(lpObj->TargetNumber, MAX_OBJECT) ) return FALSE;

	lpTargetObj = (LPOBJECTSTRUCT)&gObj[lpObj->TargetNumber];

	if( lpTargetObj->Teleport )
	{
		return FALSE;
	}

	int vpn = gObjMonsterViewportIsCharacter(lpObj);
	if( vpn < 0 ) return FALSE;

	

	mtx = tpx = lpTargetObj->X;
	mty = tpy = lpTargetObj->Y;
	
	int dis;


	if( lpObj->m_AttackType >= 100 )
	{
		dis = lpObj->m_AttackRange+2;
	}
	else dis = lpObj->m_AttackRange;
	
	// 타겟 근처로 위치를 잡는다.
	if( lpObj->X < mtx ) tpx -= dis;
	if( lpObj->X > mtx ) tpx += dis;
	if( lpObj->Y < mty ) tpy -= dis;
	if( lpObj->Y > mty ) tpy += dis;

	searchp = GetPathPacketDirPos(lpTargetObj->X-tpx, lpTargetObj->Y-tpy)*2;
	if( MapC[lpObj->MapNumber].GetStandAttr( tpx, tpy) == FALSE )	// 갈수 없다면.
	{
		while(searchcount--)
		{
			mtx = lpTargetObj->X+RoadPathTable[searchp];
			mty = lpTargetObj->Y+RoadPathTable[searchp+1];
			
			//  자리가 비어있으면..
			attr= MapC[lpObj->MapNumber].GetAttr(mtx, mty);
			result = gObjMonsterMoveCheck(lpObj, mtx, mty);

			if( lpObj->Class == 249 )
			{
				if( ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) && (result==TRUE) )
				{
					lpObj->MTX = mtx;
					lpObj->MTY = mty;
					return TRUE;
				}
			}
			else
			{
				if(    ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) 
					&& ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) 
					&& ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
					&& ((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) 
					&& (result==TRUE) )
				{
					lpObj->MTX = mtx;
					lpObj->MTY = mty;
					return TRUE;
				}
			}
			searchp += 2;

			if( searchp > 15 ) searchp = 0;
		}		
		return FALSE;
	}
	else
	{
		attr= MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
		result = gObjMonsterMoveCheck(lpObj, mtx, mty);

		if( lpObj->Class == 249 )
		{
			if( ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) && (result==TRUE) ) 
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				return TRUE;
			}

		}
		else
		{
			if( ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) && 
				((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
				((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) &&
				((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) && 
				(result==TRUE) ) // 안전지대와 못 가는길이면..
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				return TRUE;
			}
		}
	}
	return FALSE;
}


// 상대에 가장 가까운 위치를 얻는다.
int gObjGetTargetPos(LPOBJECTSTRUCT lpObj, int sx, int sy, int & tx, int & ty)
{
	int tpx, tpy, mtx, mty;
	int searchp=0, sn=0;
	int searchcount=7;
	BYTE attr;
	//BOOL result;
	
	mtx = tpx = sx;
	mty = tpy = sy;
	
	int dis;

	// 공격 형태가 100을 넘으면.. 공격길이를 2배로..
	if( lpObj->m_AttackType >= 100 )
	{
		dis = lpObj->m_AttackRange+2;
	}
	else dis = lpObj->m_AttackRange;
	
	// 타겟 근처로 위치를 잡는다.
	if( lpObj->X < mtx ) tpx -= dis;
	if( lpObj->X > mtx ) tpx += dis;
	if( lpObj->Y < mty ) tpy -= dis;
	if( lpObj->Y > mty ) tpy += dis;

	searchp = GetPathPacketDirPos(sx-tpx, sy-tpy)*2;
	if( MapC[lpObj->MapNumber].GetStandAttr( tpx, tpy) == FALSE )	// 갈수 없다면.
	{
		while(searchcount--)
		{
			mtx = sx+RoadPathTable[searchp];
			mty = sy+RoadPathTable[searchp+1];
			
			//  자리가 비어있으면..
			attr= MapC[lpObj->MapNumber].GetAttr(mtx, mty);
			//result = gObjMonsterMoveCheck(lpObj, mtx, mty);

			/*if( lpObj->Class == 249 )
			{
				if( ((attr&0x02) != 0x02) && (result==TRUE) )
				{
					lpObj->MTX = mtx;
					lpObj->MTY = mty;
					return TRUE;
				}
			}
			else
			{*/
				if(    ((attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE) 
					&& ((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) 
					&& ((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) 
					&& ((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW)) 
					//&& (result==TRUE) )
				{
					tx = mtx;
					ty = mty;
					return TRUE;
				}
			//}
			searchp += 2;

			if( searchp > 15 ) searchp = 0;
		}		
		return FALSE;
	}
	else
	{
		attr= MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
		//result = gObjMonsterMoveCheck(lpObj, mtx, mty);
		/*if( lpObj->Class == 249 )
		{
			if( ((attr&0x02) != 0x02) && (result==TRUE) ) 
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				return TRUE;
			}

		}
		else
		{*/
		if( lpObj->m_Attribute>= 100 )
		{
			if(	((attr&MAP_ATTR_STAND) != MAP_ATTR_STAND) &&
				((attr&MAP_ATTR_BLOCK) != MAP_ATTR_BLOCK) &&
				((attr&MAP_ATTR_HOLLOW) != MAP_ATTR_HOLLOW) )
				//(result==TRUE) ) // 안전지대와 못 가는길이면..
			{
				tx = tpx;
				ty = tpy;
				return TRUE;
			}
		}
		else
		{
			if( ((attr&0x01) != 0x01) && 
				((attr&0x02) != 0x02) &&
				((attr&0x04) != 0x04) &&
				((attr&0x08) != 0x08) )
				//(result==TRUE) ) // 안전지대와 못 가는길이면..
			{
				tx = tpx;
				ty = tpy;
				return TRUE;
			}
		}
	}
	return FALSE;
}

// 몬스터의 공격 대상을 나의 적으로 세팅한다.
int gObjCallMonsterSetEnemy(LPOBJECTSTRUCT lpObj, int Target)
{
	if( lpObj->Type != OBJTYPE_CHARACTER ) return -1;	
	LPOBJECTSTRUCT lpCallObj;

#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
	if( lpObj->m_RecallMon < 0 ) return -1;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
	
	lpCallObj = (LPOBJECTSTRUCT)&gObj[lpObj->m_RecallMon];

	if( lpCallObj->TargetNumber >= 0 ) return -1;

	if( lpCallObj->TargetNumber != Target )
	{
	//if( lpCallObj->TargetNumber = 0 )
		lpCallObj->TargetNumber = Target;
		//LogAdd("주인 공격 몬스터 세팅 %d", Target);
		lpCallObj->m_ActState.EmotionCount = 30;
		lpCallObj->m_ActState.Emotion = 1;
	}
	return -1;
}

int gObjMonsterSearchEnemy(LPOBJECTSTRUCT lpObj, BYTE objtype)
{
	int n;
	int tx, ty;
	int dis;
	int mindis=lpObj->m_ViewRange;
	int searchtarget=-1;
	int tObjNum;
	int count=3;
	int t1, t2;

	t1 = objtype;//&OBJTYPE_CHARACTER;
	t2 = objtype;//&OBJTYPE_MONSTER;

	for( n=0; n<MAX_MONVIEWPORTOBJECT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if( tObjNum >= 0 )
		{
			if( (gObj[tObjNum].Type == t1 ||
				gObj[tObjNum].Type == t2 ) &&
				gObj[tObjNum].Live)	// 유저가 할당되어 있다면..
			{
				if( (gObj[tObjNum].Class >= 100 && gObj[tObjNum].Class < 110) ||
					(gObj[tObjNum].Type==OBJTYPE_MONSTER&&gObj[tObjNum].m_RecallMon>=0) )
				{
				}
				else
				{
#ifdef ADD_NEWPVP_PKFIELD
					if ( g_NewPVP.IsObserver(gObj[tObjNum]) )
						continue;
#endif // ADD_NEWPVP_PKFIELD
					
#ifdef UPDATE_GM_FUNCTION_20070228	// EVENT_GM 일 경우도 공격 안되게
					if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN
						&& (gObj[tObjNum].Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
#else
					if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
#endif
					{
						if( !gObj[tObjNum].Teleport )
						{
							tx = lpObj->X-gObj[tObjNum].X;
							ty = lpObj->Y-gObj[tObjNum].Y;
							
							dis = (int)sqrt((double)(tx*tx)+(ty*ty));
							lpObj->VpPlayer2[n].dis = dis;
							//if( dis <= 3 )	// 최소 영역에 들어오고..
							{	// 다른 애들보다 가까이 있다면..
								if( dis < mindis ) 
								{					
									searchtarget = tObjNum;
									mindis = dis;
								}
							}
						}
					}
				}
			}
		}
	}

	return searchtarget;
}

int gObjGuardSearchEnemy(LPOBJECTSTRUCT lpObj)
{
	int n;
	int tx, ty;
	int dis;
	int mindis=lpObj->m_ViewRange;
	int searchtarget=-1;
	int tObjNum;
	BYTE attr;
	
	for( n=0; n<MAX_MONVIEWPORTOBJECT; n++)
	{
		tObjNum = lpObj->VpPlayer2[n].number;
		if( tObjNum >= 0 )
		{
			//if( gObj[tObjNum].Type == objtype && gObj[tObjNum].Live)	// 유저가 할당되어 있다면..
			//if( gObj[tObjNum].Live)	// 유저가 할당되어 있다면..

#ifdef UPDATE_PK_SYSTEM_20070302		// 살인마와 파티를 했을경우 경비원에게 공격 당함
			BOOL	PKFlag = FALSE;
			if(gObj[tObjNum].PartyNumber >= 0)	// 파티에 속할경우
			{
				if( gParty.GetPKPartyPenalty(gObj[tObjNum].PartyNumber) > PK_LEVEL_DEFAULT+1)
				{
					PKFlag = TRUE;
				}
			}
			else
			{
				if(gObj[tObjNum].m_PK_Level > PK_LEVEL_DEFAULT+1)
				{
					PKFlag = TRUE;
				}
			}

			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER&& PKFlag == TRUE )
#else
			if( (gObj[tObjNum].Type == OBJTYPE_CHARACTER&&
				gObj[tObjNum].m_PK_Level > PK_LEVEL_DEFAULT+1) )
#endif
			{
				attr= MapC[gObj[tObjNum].MapNumber].GetAttr(gObj[tObjNum].X, gObj[tObjNum].Y);

				if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE )
				{
					tx = lpObj->X-gObj[tObjNum].X;
					ty = lpObj->Y-gObj[tObjNum].Y;
					
					dis = (int)sqrt((double)(tx*tx)+(ty*ty));
					lpObj->VpPlayer2[n].dis = dis;
					//if( dis <= 3 )	// 최소 영역에 들어오고..
					{	// 다른 애들보다 가까이 있다면..
						if( dis < mindis ) {
							searchtarget = tObjNum;
							mindis = dis;
						}
					}
				}
			}
		}
	}
	return searchtarget;
}

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	void gObjMonsterStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex, int aMsgSubCode)
#else
	void gObjMonsterStateProc(LPOBJECTSTRUCT lpObj, int aMsgCode, int aIndex)
#endif
{
#ifdef ADD_NEW_MAP_KALIMA_20040518
	if (lpObj->m_iMonsterBattleDelay > 0) {
		// 머뭇거리는 동안은 아무 행동도 하지 않는다.
		return;
	}
#endif

	switch( aMsgCode )
	{
	case 0 :	// 공격당했다.
		if( lpObj->m_Attribute == 0 ) return;	// 공격당하기만 하는 캐릭터라면..
		
		if( gObj[aIndex].Live == 0 ||
			gObj[aIndex].m_State != OBJST_PLAYING ) return;	// 상대가 죽어있다면..

		if( lpObj->m_ActState.Emotion == 0 ) 
		{
			lpObj->m_ActState.Emotion = 1;
			lpObj->m_ActState.EmotionCount = 10;
		}
		else if( lpObj->m_ActState.Emotion == 1 ) 
		{
			lpObj->m_ActState.EmotionCount = 10;
		}
		if( !lpObj->m_ActState.Attack && lpObj->PathStartEnd == 0 )	// 공격중이아니면..
		{			
			if( CHECK_LIMIT(aIndex, MAX_OBJECT ) )
			{
				//int tuser = lpObj->TargetNumber;
				int map = gObj[aIndex].MapNumber;
				BYTE attr ;

				int dis = gObjCalDistance(lpObj, &gObj[aIndex]);
				int attackRange;

				if( lpObj->m_AttackType >= 100 )
				{
					attackRange = lpObj->m_AttackRange+2;
				}
				else attackRange = lpObj->m_AttackRange;

				if( dis <= attackRange )
				{
					if( gObj[aIndex].m_RecallMon >= 0 )
					{						
						if( lpObj->m_RecallMon >= 0 )
						{
							// 소환몬스터일 경우 몬스터만 공격 가능
							if( gObj[aIndex].Type == OBJTYPE_MONSTER)
							{
								lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
							}
						}
						else
						{
							lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
						}
					}
					else {
#ifdef MONSTER_HERD_SYSTEM_20031120
						if( rand()%100 < 90 )				// 몬스터가 공격에 증각 반응
						{
							
							if( lpObj->m_RecallMon >= 0 )
							{
								// 소환몬스터일 경우 몬스터만 공격 가능
								if( gObj[aIndex].Type == OBJTYPE_MONSTER)
								{
									lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
								}
							}
							else
							{
								lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
							}
						}
#else
						if( (rand()%3) == 0 )
						{
							
							if( lpObj->m_RecallMon >= 0 )
							{
								// 소환몬스터일 경우 몬스터만 공격 가능
								if( gObj[aIndex].Type == OBJTYPE_MONSTER)
								{
									lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
								}
							}
							else
							{
								lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
							}
						}
#endif
	
					}
				}
				// 벽이 있다면 공격을 안한다.
				else 
				{
					BYTE wall=0;
					wall = MapC[map].CheckWall2(lpObj->X, lpObj->Y, gObj[aIndex].X, gObj[aIndex].Y);

					if( wall == 0x01 )
					{
						attr = MapC[map].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
						if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE)
						{
							if( lpObj->TargetNumber < 0 ) // 공격상대가 없다면..
							{
								lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
							}
						}
					}
					else if( wall == 0x02 ) // 캐릭터가 앞에 있으면..
					{
					}
					else {	// 벽이거나 중간에 막혀 있다면..
						//lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고	
					}
				}

#ifdef MONSTER_HERD_SYSTEM_20031120
				if (lpObj->m_bIsInMonsterHerd &&
					lpObj->TargetNumber == aIndex
					)
				{
					if (lpObj->m_lpMonsterHerd != NULL)
						lpObj->m_lpMonsterHerd->BeenAttacked (lpObj, &gObj[aIndex]);
				}
#endif
			}
		}
		else 
		{	// 공격중일때는 계속 공격한다.. 어떤 넘은 상대를 바꾼다..
			if( ((rand()%2) == 1) && lpObj->PathStartEnd == 0 )
			{
				int tuser = lpObj->TargetNumber;
				
				if (!CHECK_LIMIT(tuser, MAX_OBJECT))		return;

				int map = gObj[tuser].MapNumber;
				// 벽이 있다면 공격을 안한다.

				int dis = gObjCalDistance(lpObj, &gObj[aIndex]);
				int attackRange;

				if( lpObj->m_AttackType >= 100 )
				{
					attackRange = lpObj->m_AttackRange+2;
				}
				else attackRange = lpObj->m_AttackRange;

				if( dis <= attackRange )
				{
					lpObj->m_ActState.Attack = 1;
					lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
				}
				else if( MapC[map].CheckWall2(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
				{
					lpObj->m_ActState.Attack = 1;
					lpObj->TargetNumber = aIndex;	// 공격상대로 지정하고
				}
			}
			else 
			{
				int centerlife = (int)(lpObj->MaxLife);	
				centerlife >>= 1;	// maxlife/2
				if( lpObj->Life < centerlife )
				{
					if( lpObj->m_Attribute != 2 )	//
					{
						lpObj->m_ActState.Emotion = 2;	// 도망간다.
						lpObj->m_ActState.EmotionCount = 2;
					}
				}
			}
		}
		break;
	case 1 :	// 내가(몬스터가) 죽었다
#ifdef CASTLE_MAIN_SCHEDULER_20041111		// 공성관련 NPC 들이 죽었을 경우 여기서 처리한다.
		if (lpObj->m_btCsNpcType != CS_NPC_TYPE_NONE) {
			switch (lpObj->m_btCsNpcType) {
			case CS_NPC_TYPE_DB_DFN :		// 공성전 NPC - 수성측 DB
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal, TRUE);
				break;
			case CS_NPC_TYPE_INS_DFN :		// 공성전 NPC - 수성측
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal);
				break;
			case CS_NPC_TYPE_INS_ATK :		// 공성전 NPC - 공성측
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal);
				break;
			}
#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214		
			if (lpObj->Class == NPC_INDEX_SPEARMAN || lpObj->Class == NPC_INDEX_BOWMAN) 
			{
				g_CsNPC_Mercenary.DeleteMercenary( lpObj->m_Index );
			}
#endif
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208	// 2005.05.23 추가 (b4nfter)
			if(lpObj->Class == NPC_INDEX_LIFESTONE)
			{
				g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
			}
#endif
			gObjDel(lpObj->m_Index);
		}
#endif
		
#ifdef HIDDEN_KALIMA_20050706				// 쿤둔의 잔영이 죽으면 그 자리에 마석을 소환한다.
		if (CHECK_KALIMAMAP(lpObj->MapNumber)) {
			if (
//			lpObj->Class == 147	||			// 테스트
//			lpObj->Class == 177 ||			// 테스트
//			lpObj->Class == 185 ||			// 테스트
//			lpObj->Class == 193 ||			// 테스트
//			lpObj->Class == 263				// 테스트
			lpObj->Class == 161 ||			// 쿤둔의 잔영1
			lpObj->Class == 181 || 			// 쿤둔의 잔영2
			lpObj->Class == 189 || 			// 쿤둔의 잔영3
			lpObj->Class == 197 || 			// 쿤둔의 잔영4
			lpObj->Class == 267				// 쿤둔의 잔영5
			)
			{
				g_KalimaGate.CreateKalimaGate2(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
		}
#endif
		
#ifdef MODIFY_QUEST_SYSTEM_20070525	// 퀘스트 완료요건이 몬스터Kill인지 확인한다.
		// 퀘스트 몬스터를 이곳에 미리 등록한다.
		// 차후 퀘스트가 늘어나면 다른 방법을 찾을것.
		if( lpObj->Class == 409		// 발람(훈련병)
			|| lpObj->Class == 410	// 데스스피릿(훈련병)
			|| lpObj->Class == 411	// 소람(훈련병)
			|| lpObj->Class == 412	// 다크엘프(훈련병) 
			)
		{
			g_QuestInfo.CheckQuestMonsterKill( lpObj, &gObj[aIndex] );
		}		
#endif

		gObjMonsterDieGiveItem(lpObj, &gObj[aIndex]);
		lpObj->NextActionTime = 500;
		if( lpObj->m_RecallMon >= 0 )
		{
			gObjMonsterCallKill(lpObj->m_RecallMon);
		}

#ifdef FOR_BLOODCASTLE		// 2004.02.02 블러드캐슬 몬스터 수 버그 수정 - 몬스터 수 체크부분 (이 루틴에 추가 됨)
		if( CHECK_BLOODCASTLE(lpObj->MapNumber) && lpObj->Type >= OBJTYPE_MONSTER )
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경
			int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
#endif			
			if (
				lpObj->Class == 89 ||		// 마법해골1
				lpObj->Class == 95 ||		// 마법해골2
				lpObj->Class == 112 ||		// 마법해골3
				lpObj->Class == 118 ||		// 마법해골4
				lpObj->Class == 124 ||		// 마법해골5
				lpObj->Class == 130			// 마법해골6
#ifdef BLOODCASTLE_EXTEND_20040314			// 블러드캐슬 보스몹의 체크를 늘림 (몬스터 죽였을 때)
				|| lpObj->Class == 143		// 마법해골7
#endif				
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// 블러드캐슬8 보스몹의 체크를 늘림 (몬스터 죽였을 때)
				|| lpObj->Class == 433		// 마법해골8
#endif				
				)
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경				
				g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT++;
#else
				g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT++;
#endif
			}
			else {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경	
				g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT++;
#else
				g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_KILL_COUNT++;

#endif
			}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬 다리 인덱스 변경	
			if (g_BloodCastle.CheckMonsterKillCount(iBridgeIndex)) {
				// 1 . 일반몬스터 할당량 완료체크				
				if (!g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE) {
					// 몬스터 할당량을 채웠다면 할당량이 완료되었음을 체크
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = -1;
					// 사람들에게 다리가 내려왔음을 알림 -> 클라이언트는 다리 내려오는 속성
					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_MONSTEREND;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;
					g_BloodCastle.SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
					// 일단 성문앞 다리의 서버쪽 속성만 바꾼다.
					g_BloodCastle.ReleaseCastleBridge(iBridgeIndex);
					// 성문앞의 다리의 속성을 해제할 시간을 세팅
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = GetTickCount() + BC_MAX_TICK_DOOR_OPEN;

					LogAddTD("[Blood Castle] (%d) All of the Monster Terminated -> %d", 
						iBridgeIndex+1, 
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT
						);
					
					// 보스몬스터 할당량을 계산한다.
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(iBridgeIndex) * BC_PERUSER_BOSS_MONSTER_KILL;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

					if( g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
					{
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
					}
				}

				if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT++;
					// 몬스터 할당량이 채워졌다고 해당 다리 사람들에게 공지한다.
					g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1168));							// "몬스터 제거완료!! 성문공격"
				}
			}


			if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE &&
				g_BloodCastle.CheckBossKillCount(iBridgeIndex)
				) 
			{
				// 2 . 보스몬스터 할당량 완료체크
				if (!g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE) {
					// 몬스터 할당량을 채웠다면 할당량이 완료되었음을 체크
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = -1;

					g_BloodCastle.SetSaintStatue(iBridgeIndex);

					LogAddTD("[Blood Castle] (%d) All of the Boss Monster Terminated -> %d", 
						iBridgeIndex + 1, 
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
				}

				if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT++;
					// 보스몬스터 할당량이 채워졌다고 해당 다리 사람들에게 공지한다.
					g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1180));		// "보스몬스터 제거완료!! 석상을 파괴하세요!"
				}
			}
#else	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
			if (g_BloodCastle.CheckMonsterKillCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)) {
				// 1 . 일반몬스터 할당량 완료체크
				if (!g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE) {
					// 몬스터 할당량을 채웠다면 할당량이 완료되었음을 체크
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_MAX_COUNT = -1;
					// 사람들에게 다리가 내려왔음을 알림 -> 클라이언트는 다리 내려오는 속성
					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_MONSTEREND;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;
					g_BloodCastle.SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					// 일단 성문앞 다리의 서버쪽 속성만 바꾼다.
					g_BloodCastle.ReleaseCastleBridge(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					// 성문앞의 다리의 속성을 해제할 시간을 세팅
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_dwBC_TICK_DOOR_OPEN = GetTickCount() + BC_MAX_TICK_DOOR_OPEN;

					LogAddTD("[Blood Castle] (%d) All of the Monster Terminated -> %d", 
						lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_KILL_COUNT
						);
					
					// 보스몬스터 할당량을 계산한다.
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) * BC_PERUSER_BOSS_MONSTER_KILL;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

					if( g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
					{
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
					}
				}

				if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_SUCCESS_MSG_COUNT++;
					// 몬스터 할당량이 채워졌다고 해당 다리 사람들에게 공지한다.
					g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1168));							// "몬스터 제거완료!! 성문공격"
				}
			}


			if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE &&
				g_BloodCastle.CheckBossKillCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)
				) 
			{
				// 2 . 보스몬스터 할당량 완료체크
				if (!g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_BOSS_MONSTER_KILL_COMPLETE) {
					// 몬스터 할당량을 채웠다면 할당량이 완료되었음을 체크
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_BOSS_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = -1;

					g_BloodCastle.SetSaintStatue(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);

					LogAddTD("[Blood Castle] (%d) All of the Boss Monster Terminated -> %d", 
						lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT);
				}

				if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT++;
					// 보스몬스터 할당량이 채워졌다고 해당 다리 사람들에게 공지한다.
					g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1180));		// "보스몬스터 제거완료!! 석상을 파괴하세요!"
				}
			}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010			
		}
#endif
		break;
	case 2 :	// 강한 공격을 당했다. (뒤로 물러난다)
#ifdef FOR_BLOODCASTLE
		if( gObj[aIndex].Live ) {
			if (!CHECK_BLOODCASTLE(gObj[aIndex].MapNumber)) {
				if ((gObj[aIndex].Class != 131) || (!CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))) {
					gObjBackSpring(lpObj, &gObj[aIndex]);
				}
			}
		}
#else
		if( gObj[aIndex].Live )
			gObjBackSpring(lpObj, &gObj[aIndex]);
#endif
		break;
	case 3 :	// 공격 대상자가 죽었다..
		lpObj->TargetNumber = -1;
#ifdef FOR_BLOODCASTLE
		lpObj->LastAttackerID = -1;
#endif
		lpObj->m_ActState.Emotion = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->m_ActState.Move = 0;
		lpObj->NextActionTime = 1000;
		break;
	case 4 :	// 공격을 당해 잠시 머뭇거린다.
		lpObj->m_ActState.Emotion = 3;
		lpObj->m_ActState.EmotionCount = 1;
		break;
	case 5 :
		gObjMemFree(lpObj->m_Index);
		break;
	case 6 : 
		if( gObj[aIndex].Live )
			gObjBackSpring2(lpObj, &gObj[aIndex], 2);
		break;
	case 7 : 
		if( gObj[aIndex].Live )
			gObjBackSpring2(lpObj, &gObj[aIndex], 3);
		break;
#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
	case MSG_ATTACK_NORMAL :  // 딜레이있는 일반 공격 설정
		gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0);		
		//MsgOutput(aIndex, "[MSG_ATTACKPROC][NORMAL ATTACK] %s... ", lpObj->Name);
		break;
	case MSG_MONSTER_SKILL_POISON :	// 딜레이 있는 스킬 공격 설정
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[aIndex];
			// 독 스킬
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_POISON ) == false )
			{
				if( retResistance( lpTargetObj, RESISTANCE_POISON ) == FALSE )
				{
#ifdef MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					lpTargetObj->lpAttackObj = lpObj;
#endif // MODIFY_BUFFSYSTEM_BUGFIX_01_20071128
					gObjAddBuffEffect( lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, aMsgSubCode );
				}
			}
	#else
		#ifdef ADD_SKILL_WITH_COMBO		
			if( lpTargetObj->m_PoisonType == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
		#else
			if( lpTargetObj->m_PoisonBeattackCount == 0 ) // 현재 중독 마법에 걸려있다면 다시 걸리지 않게..
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
			{
				if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
				{
		#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonType = AT_SKILL_POISON;
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonBeattackCount	= aMsgSubCode;	// 초 설정
					lpTargetObj->lpAttackObj			= lpObj;
					lpTargetObj->m_ViewSkillState		|= 0x01; //	1번째 비트
					GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
				}
			}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004		
		}
		break;
	case MSG_MONSTER_SKILL_SPRING: // 딜레이 있는 밀기 공격
		{
			// aMsgSubCode : 거리
			LPOBJECTSTRUCT lpTargetObj = &gObj[aIndex];
			gObjBackSpring2( lpTargetObj, lpObj, aMsgSubCode );
		}
		break;
#endif // MODIFY_NEW_MONSTER_AI_01_2006724
	}
}

extern int gCheckGoal(int x, int y, int & ground);
extern BOOL gBattleSoccerScoreUpdate(int ground, int team);

void gObjMonsterProcess(LPOBJECTSTRUCT	lpObj)
{
	gObjMsgProc(lpObj);

	if( lpObj->Live == 0 ) 
		return;

#ifdef ADD_NEW_MAP_KALIMA_20040518						// 칼리마 보스몹들의 추가 공격들
	if (lpObj->m_iMonsterBattleDelay > 0) {	// 딜레이 시간을 주기적으로 계속 깎는다.
		lpObj->m_iMonsterBattleDelay --;
	}
#endif

	if( (GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime) ) return;
	lpObj->CurActionTime = GetTickCount();	
	
#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {
		if ( (lpObj->Class == 131) || (CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))) {	// 성문이나 성자의석상은 않움직인다.
			return;
		}
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518						// 마석은 움직이지 않는다. -> 공격도 않한다. 대신 자신위에 서 있는 사용자를 찾아서 이동시키고 시간에 따라 수명도 줄어든다.
	if (CHECK_KALIMAGATE (lpObj->m_Attribute)) 
	{

	#ifdef HIDDEN_KALIMA_20050706
		if ( lpObj->m_Attribute== MON_ATTR_KALIMAGATE_NXT ) 
		{
			g_KalimaGate.KalimaGateAct2(lpObj->m_Index);
			return;
		}
	#endif

		g_KalimaGate.KalimaGateAct(lpObj->m_Index);
		return;
	}
#endif


#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207			// 수호석상은 움직이지 않는다.
	if (lpObj->Class == 283) {
		return;
	}
#endif

#ifdef CASTLE_NPC_CANNON_TOWER_20041223					// 캐논타워는 별도로 공격한다.
	if (lpObj->Class == 288) {
		return;
	}
#endif
	
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208				// 라이프스톤은 움직이지 않는다.
	if( lpObj->Class ==  NPC_INDEX_LIFESTONE )
	{
		return;
	}
#endif
	
#ifdef FOR_BLOODCASTLE
	if( lpObj->Class >= 100 && lpObj->Class <= 110 )	// 트랩일때
#else
	if( lpObj->Class >= 100 && lpObj->Class < 110 )		// 트랩일때
#endif
	{
		gObjMonsterTrapAct(lpObj);
	}
	else if( lpObj->Class == 200 )		// 배틀사커 축구공일 때
	{
		int ground;
		int team;
		team = gCheckGoal(lpObj->X, lpObj->Y, ground);
		if( team >= 0 )
		{
#ifdef WORLD_TOURNAMENT_EVENT_SETTING
			GCBTGoalSend(ground, lpObj->X, lpObj->Y);
#endif			
			gObjMonsterRegen(lpObj);
			gBattleSoccerScoreUpdate(ground, team);
			return;
		}
	}
#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214	
	// 공성전 용병은 다르게 행동한다.
	else if( lpObj->Class == NPC_INDEX_SPEARMAN || lpObj->Class == NPC_INDEX_BOWMAN )
	{
		g_CsNPC_Mercenary.MercenaryAct(lpObj->m_Index);
	}
#endif
	else 
	{

#ifdef MONSTER_HERD_SYSTEM_20031120
		if (lpObj->m_bIsInMonsterHerd) {
			if (lpObj->m_lpMonsterHerd != NULL)
				lpObj->m_lpMonsterHerd->MonsterBaseAct(lpObj);
		}
		else
#endif
			gObjMonsterBaseAct(lpObj);
	}
	
	// 소환 몬스터 일 때 -> 주인과의 위치 확인 후 멀면 텔레포트
	if( lpObj->m_Attribute == 100 )		
	{
		if( lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < MAX_OBJECT )
		{
			LPOBJECTSTRUCT lpCallObj;
			int				CallMonMove=0;
			lpCallObj = &gObj[lpObj->m_RecallMon];
			
			if( lpObj->MapNumber != lpCallObj->MapNumber ) CallMonMove = 1;
			if( (gObjCalDistance(lpCallObj, lpObj)) > 14 ) CallMonMove = 1;
			
			if( CallMonMove == 1 ) 
			{
				gObjTeleportMagicUse(lpObj->m_Index, (BYTE)lpCallObj->X+1, (BYTE)lpCallObj->Y);
				lpObj->MapNumber = lpCallObj->MapNumber;
				//LogAdd("위치 이동 %d %d", lpCallObj->X+1, lpCallObj->Y);
				return;
			}
		}
	}

	if( lpObj->m_ActState.Move )
	{
		if( PathFindMoveMsgSend(lpObj) == TRUE )
		{
			lpObj->m_ActState.Move = 0;			
		}
		lpObj->m_ActState.Move = 0;

	}
	else if( lpObj->m_ActState.Attack == 1 )
	{
#ifdef ADD_RAKLION_20080408		// 라클리온 세루판은 공격을 모두 따로 처리하고 있다.
		if( lpObj->Connected==3 && lpObj->Type==OBJTYPE_MONSTER && lpObj->Class==RAKLION_SELUPAN )
			return;
#endif // ADD_RAKLION_20080408

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
		// 아이다 몹 이상 부터 적용되는 몬스터 공격 시스템.(스킬 사용)
		if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
		{
			BOOL bEnableAttack = TRUE;
			// 1) 타겟이 없을때.
			if( lpObj->TargetNumber < 0 )
#ifdef MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
			{
				lpObj->TargetNumber			= -1;
				lpObj->m_ActState.Emotion	= 0;
				lpObj->m_ActState.Attack	= 0;
				lpObj->m_ActState.Move		= 0;
				lpObj->NextActionTime		= 1000;
				return;
			}
#else	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711
				bEnableAttack = FALSE;
#endif	// MODIFY_TARGETNUMBER_RECALLMON_INDEX_BUFGIX_20080711

			// 1) 죽었고
			// 2) 텔레포트 중일 때
			if( !gObj[lpObj->TargetNumber].Live 
				|| gObj[lpObj->TargetNumber].Teleport)
			{
				bEnableAttack = FALSE;
			}

			// 1) 게임 진행 중이 아니고,
			// 2) 종료 중이고,
			if( gObj[lpObj->TargetNumber].Connected <= 2 
				|| gObj[lpObj->TargetNumber].CloseCount != -1 )
			{
				bEnableAttack = FALSE;
			}

			if( !bEnableAttack )
			{
				lpObj->TargetNumber			= -1;
				lpObj->m_ActState.Emotion	= 0;
				lpObj->m_ActState.Attack	= 0;
				lpObj->m_ActState.Move		= 0;
				lpObj->NextActionTime		= 1000;
				return;
			}

			// 여기서 스킬매니저가 처리

			// AI에서 스킬 처리!!!!!!!

			// 1. 일반 공격
			if( rand()% 4 == 0 )
			{
				PMSG_ATTACK pAttackMsg;
				pAttackMsg.AttackAction = AT_ATTACK1;
				pAttackMsg.DirDis       = lpObj->Dir;
				pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
				pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);

#ifdef MODIFY_NEW_MONSTER_AI_01_2006724
				//CGAttack(&pAttackMsg, lpObj->m_Index);
#else
				CGAttack(&pAttackMsg, lpObj->m_Index);
#endif

#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
				GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index, lpObj->TargetNumber );
#else
				GCActionSend(lpObj, AT_ATTACK1, lpObj->m_Index);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731
				
				gObjAttack(lpObj, &gObj[lpObj->TargetNumber], NULL, 0, 0);
			}
			else
			{
				// 2. 몬스터 스킬 공격
				TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
			}
	
			lpObj->m_ActState.Attack = 0;

			return;
		}
		// 적용의 시작 부분.
#endif

		int AttackType = lpObj->m_AttackType;
		int MagicAttack=0;

		if( AttackType >= 100 )
		{
			if( (rand()%5) == 0 )	// 5 분의 1 확률로 마법공격
			{
				AttackType -= 100;
				MagicAttack = 1;
			}
			else 
			{
				AttackType = 0;
			}
		}

		if( MagicAttack || lpObj->m_AttackType == 50 )
		{
			if( lpObj->TargetNumber >= 0 )
			{	
				if( gObj[lpObj->TargetNumber].Connected > 2 && gObj[lpObj->TargetNumber].CloseCount == -1 )
				{
					if( !gObj[lpObj->TargetNumber].Live )
					{
						lpObj->TargetNumber = -1;
						lpObj->m_ActState.Emotion = 0;
						lpObj->m_ActState.Attack = 0;
						lpObj->m_ActState.Move = 0;
						lpObj->NextActionTime =		1000;
						//if( lpObj->m_Attribute == 100 )	LogAdd("상대가 죽었다.. 차분히 기다린다.");
					}
					else {
						if( !gObj[lpObj->TargetNumber].Teleport )
						{
							gObjMonsterMagicAttack(lpObj);

#ifdef MONSTER_HERD_SYSTEM_20031120
							if (
								lpObj->Type == OBJTYPE_MONSTER &&
								lpObj->m_bIsInMonsterHerd
								) 
							{
								if (lpObj->m_lpMonsterHerd != NULL)		// 무리에서 공격 시 행동을 지정할 때 이 메서드를 오버라이딩
									lpObj->m_lpMonsterHerd->MonsterAttackAction(lpObj, &gObj[lpObj->TargetNumber]);
							}
#endif
						}
					}					
				}
				else
				{
					lpObj->TargetNumber = -1;
					lpObj->m_ActState.Emotion = 0;
					lpObj->m_ActState.Attack = 0;
					lpObj->m_ActState.Move = 0;
					lpObj->NextActionTime = 1000;
				}
			}
		}
		else
		{
			if( lpObj->TargetNumber >= 0 )
			{	
				if( gObj[lpObj->TargetNumber].Connected > 2 && gObj[lpObj->TargetNumber].CloseCount == -1 )
				{
					if( !gObj[lpObj->TargetNumber].Live  )
					{
						lpObj->TargetNumber = -1;
						lpObj->m_ActState.Emotion = 0;
						lpObj->m_ActState.Attack = 0;
						lpObj->m_ActState.Move = 0;
						lpObj->NextActionTime = 1000;
						//if( lpObj->m_Attribute == 100 )	LogAdd("상대가 죽었다.. 차분히 기다린다.");
					}
					else {
						if( !gObj[lpObj->TargetNumber].Teleport )
						{
							gObjMonsterAttack(lpObj, &gObj[lpObj->TargetNumber]);
						
#ifdef MONSTER_HERD_SYSTEM_20031120
							if (
								lpObj->Type == OBJTYPE_MONSTER &&
								lpObj->m_bIsInMonsterHerd
								) 
							{
								if (lpObj->m_lpMonsterHerd != NULL)		// 무리에서 공격 시 행동을 지정할 때 이 메서드를 오버라이딩
									lpObj->m_lpMonsterHerd->MonsterAttackAction(lpObj, &gObj[lpObj->TargetNumber]);
							}
#endif						
						}
					}				
				}
				else
				{
					lpObj->TargetNumber = -1;
					lpObj->m_ActState.Emotion = 0;
					lpObj->m_ActState.Attack = 0;
					lpObj->m_ActState.Move = 0;
					lpObj->NextActionTime = 1000;
				}
			}
		}
		lpObj->m_ActState.Attack = 0;
	}
	else 
	{

	}
}


// 몬스터 범위공격 (악마의 불꽃)을 처리하는 부분
#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
void gObjMonsterMagicAttack(LPOBJECTSTRUCT lpObj, INT iMonsterClass)
#else
void gObjMonsterMagicAttack(LPOBJECTSTRUCT lpObj)
#endif
{
	int tObjNum;
	int  count=0;
	PMSG_BEATTACK_COUNT	pCount;
	PMSG_BEATTACK		pAttack;

	// 이 버퍼 크기 주의해야 한다. 
	// 추후 몬스터의 뷰포트개수가 증가했을시 수정되야함! (apple)
	BYTE AttackSendBuff[256];
	int  ASBOfs=0;
	
	PMSG_DURATION_MAGIC_RECV pDuration;
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	pCount.MagicNumberH	= HIBYTE( 0 );
	pCount.MagicNumberL	= LOBYTE( 0 );
#else
	pCount.MagicNumber = 0;
#endif
	pDuration.Dir = 0;
	pDuration.X = (BYTE)lpObj->X;
	pDuration.Y = (BYTE)lpObj->Y;

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
	// 쿤둔일 경우에는 데미지 판정만 한다 - apple
	//if( iMonsterClass != 275 )
#endif
	CGDurationMagicRecv(&pDuration, lpObj->m_Index);
		
	pCount.h.c = PMHC_BYTE;
#ifdef PACKET_CHANGE	
	pCount.h.headcode = PACKET_MAGIC_ATTACK;
#else
	pCount.h.headcode = 0x1D;
#endif
	pCount.h.size = 0;
	// 쿤둔(275) 기본 공격을 전체 마법으로. (apple)
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	pCount.MagicNumberH	= HIBYTE( 0 );
	pCount.MagicNumberL	= LOBYTE( 0 );
#else
	pCount.MagicNumber = 0;
#endif
	pCount.Count = 0;
	pCount.X = (BYTE)lpObj->X;
	pCount.Y = (BYTE)lpObj->Y;
	ASBOfs = sizeof( pCount );
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{
					if( lpObj->Class == 77 )
					{
						pAttack.NumberH = HIBYTE(tObjNum);
						pAttack.NumberL = LOBYTE(tObjNum);
						memcpy( AttackSendBuff+ASBOfs, (char*)&pAttack, sizeof( pAttack ));
						ASBOfs += sizeof( pAttack );
						pCount.Count++;
					}
					else
					{
#ifdef MODIFY_KUNDUN_VIEWPORT_20060523
						// 쿤둔 인 경우 전체 공격일 때 거리 무시.
						if( lpObj->Class == 275 || gObjCalDistance( lpObj, &gObj[tObjNum]) < 6 )
#else
						if( gObjCalDistance( lpObj, &gObj[tObjNum]) < 6 )
#endif // MODIFY_KUNDUN_VIEWPORT_20060523
						{
							pAttack.NumberH = HIBYTE(tObjNum);
							pAttack.NumberL = LOBYTE(tObjNum);
							memcpy( AttackSendBuff+ASBOfs, (char*)&pAttack, sizeof( pAttack ));
							ASBOfs += sizeof( pAttack );
							pCount.Count++;
						}
					}					
				}
			}
		}
		count++;
		if( count > MAX_MONVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
	if( pCount.Count > 0 )
	{			
		pCount.h.size = ASBOfs;
		memcpy(AttackSendBuff, (char*)&pCount, sizeof(pCount));

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
		// 쿤둔의 환영일 경우
		if (
			lpObj->Class == 161 ||		// 쿤둔의 환영1
			lpObj->Class == 181 || 		// 쿤둔의 환영2
			lpObj->Class == 189 || 		// 쿤둔의 환영3
			lpObj->Class == 197 || 		// 쿤둔의 환영4
			lpObj->Class == 267 || 		// 쿤둔의 환영5
			lpObj->Class == 275 		// 쿤둔의 환영6
		)
		{
			gObjMonsterBeattackRecv(AttackSendBuff, lpObj->m_Index);
		}
		else
		{
			CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
		}
#else
		CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
#endif
		
	}
}

#ifdef ADD_ABILLITY_KUNDUN_01_20040920
void gObjUseMonsterSpecialAbillity( LPOBJECTSTRUCT lpMonsterObj )
{
	// *. 6칼리마의 쿤둔일때 
	if( lpMonsterObj->Class == 275 )
	{
		PMSG_MAGICATTACK pAttackMsg;

		// 소환 마법 인덱스 = 1
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH	= HIBYTE( 1 );				
		pAttackMsg.MagicNumberL	= LOBYTE( 1 );				
#else
		pAttackMsg.MagicNumber	= 1;				
#endif		
		pAttackMsg.NumberH		= HIBYTE(lpMonsterObj->TargetNumber);
		pAttackMsg.NumberL		= LOBYTE(lpMonsterObj->TargetNumber);
		pAttackMsg.Dis			= 0;
	
		// 피가 얼마 남지 않았을때 부하 몹 소환!
		// 마법 인덱스 1 번이 소환 마법

		
		// 2. 몬스터 소환을 5번 시전 한다.
		if( lpMonsterObj->Life < (lpMonsterObj->MaxLife / 25) )
		{

			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
		}
		// 1. 몬스터 소환을 3번 시전 한다.
		else if( lpMonsterObj->Life < (lpMonsterObj->MaxLife / 5) )
		{
			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
		}
	}

}
#endif

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
// 몬스터 공격 데미지 처리를 위한 함수이다. (apple)
// CGBeattackRecv의 몬스터만을 위한 처리 함수이다.!
void gObjMonsterBeattackRecv( LPBYTE lpRecv, int aIndex )
{
	LPPMSG_BEATTACK_COUNT	lpCount = (LPPMSG_BEATTACK_COUNT)lpRecv;
	
	int	lOfs;
	int tNumber;
	CMagicInf *lpMagic;

	lOfs = sizeof(PMSG_BEATTACK_COUNT);
	LPPMSG_BEATTACK	lpMsg;

	// 몬스터일 경우 마법 번호 얻는 처리
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
	WORD wMagicNumber = MAKEWORD( lpCount->MagicNumberL, lpCount->MagicNumberH );
	lpMagic = gObjGetMagic(&gObj[aIndex], wMagicNumber);
#else
	lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
#endif

	if( lpMagic == NULL )
	{
		LogAdd("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}

	int lOfs2 = lOfs;
	int pTargetNumber[128] = {0,};

	for( int i = 0; i < lpCount->Count; i++ )
	{
		lpMsg = (LPPMSG_BEATTACK)(lpRecv+lOfs2);
		pTargetNumber[i] = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
		lOfs2 += sizeof(PMSG_BEATTACK);
	}
	
	for( int n=0; n<lpCount->Count; n++)
	{
		lpMsg = (LPPMSG_BEATTACK)(lpRecv+lOfs);
		tNumber = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);	

		gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, 1, 1);
		
		lOfs += sizeof(PMSG_BEATTACK);
	}
}
#endif // MODIFY_MONSTER_ENTIRE_ATTACK_20040915


void gObjMonsterAttack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj )
{
	int AttackType = lpObj->m_AttackType;

	if( AttackType >= 100 ) AttackType = 0;
	
#ifdef ADD_CASTLE_HUNTZONE_MONSTER_01_20050111
	if( lpObj->Class == 291 ||	
		lpObj->Class == 295 
	  )
	{
//		PMSG_MAGICATTACK pAttackMsg;
//		pAttackMsg.MagicNumber = 3;// 썬더로 처리.
//		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
//		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
//		pAttackMsg.Dis     = 0;
//		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518	// 칼리마 보스몹들의 추가 공격들
	if (lpObj->m_iMonsterBattleDelay > 0) {	// 머뭇거리는 동안 (클라이언트 삽입 신 등) 에는 공격을 하지 않는다.
		return;
	}

	if (
		lpObj->Class == 144 ||		// 데스 엔젤1
		lpObj->Class == 174 ||		// 데스 엔젤2
		lpObj->Class == 182 ||		// 데스 엔젤3
		lpObj->Class == 190 ||		// 데스 엔젤4
		lpObj->Class == 260 ||		// 데스 엔젤5
		lpObj->Class == 268			// 데스 엔젤6
		)
	{
		// 1/2 확률로 썬더공격
		if (rand()%2) {
			PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
			pAttackMsg.MagicNumberH = HIBYTE( 0 );	// 기본 AttackType이 3 임
			pAttackMsg.MagicNumberL = LOBYTE( 0 );
#else
			pAttackMsg.MagicNumber = 0;				// 기본 AttackType이 3 임
#endif
			pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
			pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
			pAttackMsg.Dis     = 0;
			CGMagicAttack(&pAttackMsg, lpObj->m_Index);
			return;
		}
		else {
			AttackType = 0;
		}
	}

	if (
		lpObj->Class == 161 ||		// 쿤둔의 환영1
		lpObj->Class == 181 || 		// 쿤둔의 환영2
		lpObj->Class == 189 || 		// 쿤둔의 환영3
		lpObj->Class == 197 || 		// 쿤둔의 환영4
		lpObj->Class == 267 || 		// 쿤둔의 환영5
		lpObj->Class == 275 		// 쿤둔의 환영6
		)
	{
		// 썬더
		PMSG_MAGICATTACK pAttackMsg;

		// 전체마법 0번이 (attacktype이 100 이상인몬스터) 추가 되어 있기 때문에
		// rand()%6 + 1이 된다. (apple)
		
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		WORD wRandValue = ( rand() % 6 ) + 1;
		pAttackMsg.MagicNumberH = HIBYTE( wRandValue );
		pAttackMsg.MagicNumberL = LOBYTE( wRandValue );
#else
		pAttackMsg.MagicNumber = rand()%6+1;
#endif

		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		if( wRandValue == 1 || wRandValue == 2 || wRandValue == 0)
			gObjUseMonsterSpecialAbillity( lpObj );
#else
#ifdef ADD_ABILLITY_KUNDUN_01_20040920	
		if( pAttackMsg.MagicNumber == 1 || pAttackMsg.MagicNumber == 2 || pAttackMsg.MagicNumber == 0)
			gObjUseMonsterSpecialAbillity( lpObj );
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
		gObjMonsterMagicAttack(lpObj, lpObj->Class);		// 쿤둔의 환영의 경우 범위 마법 (막마의 불꽃) 이 항상 나간다.
#else
		gObjMonsterMagicAttack(lpObj);						// 쿤둔의 환영의 경우 범위 마법 (막마의 불꽃) 이 항상 나간다.
#endif
		return;
	}

	if (
		lpObj->Class == 149 ||		// 네크론1
		lpObj->Class == 179 ||		// 네크론2
		lpObj->Class == 187 ||		// 네크론3
		lpObj->Class == 195 ||		// 네크론4
		lpObj->Class == 265 ||		// 네크론5
		lpObj->Class == 273			// 네크론6
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		WORD wRandValue = ( rand() % 2 )+ 1;
		pAttackMsg.MagicNumberH = HIBYTE( wRandValue );		// 네크론의 경우는 에너지볼과 포이즌 둘 중 하나가 나간다.
		pAttackMsg.MagicNumberL = LOBYTE( wRandValue );		
#else
		pAttackMsg.MagicNumber = rand()%2+1;				// 네크론의 경우는 에너지볼과 포이즌 둘 중 하나가 나간다.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}


	if (
		lpObj->Class == 145 ||		// 로드 센튜리온1
		lpObj->Class == 175 ||		// 로드 센튜리온2
		lpObj->Class == 183 ||		// 로드 센튜리온3
		lpObj->Class == 191 ||		// 로드 센튜리온4
		lpObj->Class == 261 ||		// 로드 센튜리온5
		lpObj->Class == 269 ||		// 로드 센튜리온6
		lpObj->Class == 146 ||		// 블러드 솔져1
		lpObj->Class == 176 || 		// 블러드 솔져2
		lpObj->Class == 184 ||		// 블러드 솔져3
		lpObj->Class == 192 ||		// 블러드 솔져4
		lpObj->Class == 262 ||		// 블러드 솔져5
		lpObj->Class == 270 ||		// 블러드 솔져6
		lpObj->Class == 147 ||		// 데빌 윙1
		lpObj->Class == 177 ||		// 데빌 윙2
		lpObj->Class == 185 ||		// 데빌 윙3
		lpObj->Class == 193 ||		// 데빌 윙4
		lpObj->Class == 263 ||		// 데빌 윙5
		lpObj->Class == 271 ||		// 데빌 윙6
		lpObj->Class == 148 ||		// 데스 센튜리온1
		lpObj->Class == 178 ||		// 데스 센튜리온2
		lpObj->Class == 186 ||		// 데스 센튜리온3
		lpObj->Class == 194 ||		// 데스 센튜리온4
		lpObj->Class == 264 ||		// 데스 센튜리온5
		lpObj->Class == 272 ||		// 데스 센튜리온6
//			lpObj->Class == 149 ||		// 네크론1
//			lpObj->Class == 179 ||		// 네크론2
//			lpObj->Class == 187 ||		// 네크론3
//			lpObj->Class == 195 ||		// 네크론4
//			lpObj->Class == 265 ||		// 네크론5
//			lpObj->Class == 273 ||		// 네크론6
		lpObj->Class == 160 ||		// 쿤둔의 잔영1
		lpObj->Class == 180 ||		// 쿤둔의 잔영2
		lpObj->Class == 188 ||		// 쿤둔의 잔영3
		lpObj->Class == 196 ||		// 쿤둔의 잔영4
		lpObj->Class == 266 ||		// 쿤둔의 잔영5
		lpObj->Class == 274			// 쿤둔의 잔영6
		)
	{
		// 1/2 확률로 에너지 볼 공격
		if (rand()%2) {
			PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
			pAttackMsg.MagicNumberH = HIBYTE( 1 );		// 기본 AttackType이 150인 상태에서 마법을 추가했으므로 인덱스가 1
			pAttackMsg.MagicNumberL = LOBYTE( 1 );	
#else
			pAttackMsg.MagicNumber = 1;				// 기본 AttackType이 150인 상태에서 마법을 추가했으므로 인덱스가 1
#endif
			pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
			pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
			pAttackMsg.Dis     = 0;
			CGMagicAttack(&pAttackMsg, lpObj->m_Index);
			return;
		}
		else {
			AttackType = 0;
		}
	}
#endif

#ifdef FOR_BLOODCASTLE
	if( 
		lpObj->Class == 89 ||		// 마법해골1
		lpObj->Class == 95 ||		// 마법해골2
		lpObj->Class == 112 ||		// 마법해골3
		lpObj->Class == 118 ||		// 마법해골4
		lpObj->Class == 124 ||		// 마법해골5
		lpObj->Class == 130			// 마법해골6
#ifdef BLOODCASTLE_EXTEND_20040314	// 블러드캐슬 보스몹의 체크를 늘림
		|| lpObj->Class == 143		// 마법해골7
#endif		
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드캐슬8 보스몹의 체크를 늘림
		|| lpObj->Class == 433		// 마법해골8
#endif		
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		WORD wRandValue = ( rand() % 2 ) + 1;
		pAttackMsg.MagicNumberH = HIBYTE( wRandValue );		// 마법해골의 경우는 썬더와 에너지볼 둘 중에 하나가 나간다.
		pAttackMsg.MagicNumberL = LOBYTE( wRandValue );		
#else
		pAttackMsg.MagicNumber = rand()%2+1;				// 마법해골의 경우는 썬더와 에너지볼 둘 중에 하나가 나간다.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}

#ifdef CHAOSCASTLE_SYSTEM_20040408		// 카오스 캐슬의 궁수들의 공격은 모두 에너지 볼이다.
	if( 
		lpObj->Class == 163 ||		// 카오스 캐슬 - 궁수1
		lpObj->Class == 165 ||		// 카오스 캐슬 - 궁수2
		lpObj->Class == 167 ||		// 카오스 캐슬 - 궁수3
		lpObj->Class == 169 ||		// 카오스 캐슬 - 궁수4
		lpObj->Class == 171 ||		// 카오스 캐슬 - 궁수5
		lpObj->Class == 173			// 카오스 캐슬 - 궁수6
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스궁수7 추가
		|| lpObj->Class == 427			// 카오스 캐슬 - 궁수7
#endif		
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 1 );							// 카오스캐슬 궁수의 경우는 항상 에너지 볼을 쏜다.
		pAttackMsg.MagicNumberL = LOBYTE( 1 );							// 카오스캐슬 궁수의 경우는 항상 에너지 볼을 쏜다.
#else
		pAttackMsg.MagicNumber = 1;							// 카오스캐슬 궁수의 경우는 항상 에너지 볼을 쏜다.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}
#endif
	
/*	if (
		lpObj->Class == 87 ||		// 자이언트오거1
		lpObj->Class == 93 ||		// 자이언트오거2
		lpObj->Class == 99 ||		// 자이언트오거3
		lpObj->Class == 116 ||		// 자이언트오거4
		lpObj->Class == 122 ||		// 자이언트오거5
		lpObj->Class == 128			// 자이언트오거6
		)
	{
		if(rand()%2) {
			// 자이언트오거의 경우는 1/2 확률로 마법공격과 일반공격을 번갈아 한다.
			PMSG_MAGICATTACK pAttackMsg;
			pAttackMsg.MagicNumber = 1;		// 추가되었던 번개마법
			pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
			pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
			pAttackMsg.Dis     = 0;
			CGMagicAttack(&pAttackMsg, lpObj->m_Index);
			return;
		}
	}
*/
	if( 
		lpObj->Class == 66 || 
		lpObj->Class == 73 || 
		lpObj->Class == 77
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 1 );
		pAttackMsg.MagicNumberL = LOBYTE( 1 );
#else
		pAttackMsg.MagicNumber = 1;//(BYTE)lpObj->m_AttackType;
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}
#else
	if( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{	// 저주받은 왕
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 1 );
		pAttackMsg.MagicNumberL = LOBYTE( 1 );
#else
		pAttackMsg.MagicNumber = 1;//(BYTE)lpObj->m_AttackType;
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}
#endif
	
	if( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{	// 저주받은 왕
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 1 );
		pAttackMsg.MagicNumberL = LOBYTE( 1 );
#else
		pAttackMsg.MagicNumber = 1;//(BYTE)lpObj->m_AttackType;
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}

	switch(AttackType)
	{
	case 0 :	// 일반 공격
		{
		PMSG_ATTACK pAttackMsg;
#if GAME_VERSION >= G_V_99B
		pAttackMsg.AttackAction = AT_ATTACK1;
#else
		pAttackMsg.AttackAction = AT_ATTACK1-20;
#endif
		pAttackMsg.DirDis       = lpObj->Dir;
		pAttackMsg.NumberH		= HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL		= LOBYTE(lpObj->TargetNumber);
		CGAttack(&pAttackMsg, lpObj->m_Index);
		}
		break;
	default :
		{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 0 );
		pAttackMsg.MagicNumberL = LOBYTE( 0 );
#else
		pAttackMsg.MagicNumber = 0;//(BYTE)lpObj->m_AttackType;
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		}
		break;
	}
}


// 몬스터가 타겟이 있다면 그 근처까지 이동한다.
/*void gObjMonsterGetTargetPos(LPOBJECTSTRUCT lpObj)
{
	int tpx, tpy, mtx, mty;
	int searchp=0;
	int searchcount=7;

	if( lpObj->TargetNumber < 0 ) return;

	int tObjNum = lpObj->VpPlayer[lpObj->TargetNumber].number;
	
	if( gObj[tObjNum].Live < 1 ) return;
	
	mtx = tpx = gObj[tObjNum].X;
	mty = tpy = gObj[tObjNum].Y;
	
	// 자신에게 가까운 위치를 잡는다.
	if( lpObj->X < mtx ) tpx = mtx-1;
	if( lpObj->X > mtx ) tpx = mtx+1;
	if( lpObj->Y < mty ) tpy = mty-1;
	if( lpObj->Y > mty ) tpy = mty+1;

	//searchp = GetPathPacketDirPos(gObj[tObjNum].X-tpx, gObj[tObjNum].Y-tpy)*2;

	while(searchcount--)
	{
		mtx = gObj[tObjNum].X+RoadPathTable[searchp];
		mty = gObj[tObjNum].Y+RoadPathTable[searchp+1];
		//  자리가 비어있으면..
		if( MapC[lpObj->MapNumber].GetStandAttr( mtx, mty) == FALSE )
		{
			//PathFindMoveMsgSend(lpObj, mtx, mty, gObj[tObjNum].X, gObj[tObjNum].Y);
			break;
		}
		searchp += 2;
		if( searchp > 15 ) searchp = 0;
	}
}
*/

// 이동하고자 하는곳까지의 이동패스를 찾아 이동했다면 이동 패킷을 만들어 보낸다.
BOOL PathFindMoveMsgSend(LPOBJECTSTRUCT lpObj)
{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true 
	  )
	{
		return FALSE;
	}
#else
	#ifdef NEW_FORSKYLAND2
	if( lpObj->m_SkillHarden )
		return FALSE;
	#endif

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	if( lpObj->m_iSkillStunTime > 0 )
		return FALSE;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태에서는 순간이동을 할 수 없다
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		return FALSE;
	}
#endif

	PATH_t			path;
	
	
	PMSG_MOVE		pMove;
	pMove.h.c		 = PMHC_BYTE;
#ifdef PACKET_CHANGE	
	pMove.h.headcode = PACKET_MOVE;
#else
	pMove.h.headcode = 0x10;
#endif
	pMove.h.size     = sizeof( pMove );
	pMove.X          = (BYTE)lpObj->X;
	pMove.Y			 = (BYTE)lpObj->Y;
		
	BYTE bPath[MAX_PATHDIR];
		
#ifdef MONSTER_HERD_SYSTEM_20031120
	BOOL bPathFound = FALSE;
	if (lpObj->Type == OBJTYPE_MONSTER &&
		lpObj->m_bIsInMonsterHerd
		)
		bPathFound = MapC[lpObj->MapNumber].PathFinding3(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY, &path);
	else
		bPathFound = MapC[lpObj->MapNumber].PathFinding2(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY, &path);

	if (bPathFound)
#else
	if( MapC[lpObj->MapNumber].PathFinding2(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY, &path) == TRUE)
#endif	
	{
		int tx, ty, sx, sy;
		BYTE pos=0;
		
		memset(bPath, 0, MAX_PATHDIR);
		
		bPath[0] = (lpObj->Dir<<4)+((path.PathNum-1)&0x0F);
		
		sx = lpObj->X;
		sy = lpObj->Y;
		lpObj->PathX[0] = lpObj->X;
		lpObj->PathY[0] = lpObj->Y;
		lpObj->PathDir[0] = lpObj->Dir;
		for( int n=1; n<path.PathNum; n++)
		{
			tx = path.PathX[n];
			ty = path.PathY[n];

			pos = GetPathPacketDirPos( tx-sx, ty-sy);

			lpObj->PathX[n] = tx;
			lpObj->PathY[n] = ty;
			lpObj->PathDir[n] = pos;
			
			sx = tx;
			sy = ty;
			if( (n%2) == 1 ) {
				bPath[(n+1)/2] = pos<<4;
			}
			else bPath[(n+1)/2] |= pos;
		}
		//--------
		lpObj->Dir       = bPath[0]>>4;
		lpObj->PathCount = (bPath[0]&0x0f);
		lpObj->PathCur   = 1;
		
		// ADD_NEW_MONSTER_SYSTEM_02_20050725 -- 양쪽에서 부딪혀서 여기껄 막아놓았다.
		// 일단 풀어놓음.
		lpObj->PathTime  = GetTickCount();

		//lpObj->X         = lpObj->MTX;
		//lpObj->Y         = lpObj->MTY;
		
		//AttackDir = lpObj->Dir;//GetPathPacketDirPos( rtx-sx, rty-sy);
		bPath[0] &= 0x0F;
		bPath[0] |= pos<<4;

		memcpy(pMove.Path, bPath, MAX_PATHDIR);
		
		if( lpObj->PathCount > 0 )
		{
			MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
			MapC[lpObj->MapNumber].SetStandAttr(sx, sy);
			lpObj->m_OldX = sx;
			lpObj->m_OldY = sy;
		}
		PMoveProc(&pMove, lpObj->m_Index);
		//vChatSend(lpObj, "이동시작 %d / %d  -> %d / %d", lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY);
		return TRUE;
	}
	return FALSE;
}


// 몬스터가 다른곳으로 이동해 본다.
void gObjMonsterMoveAction(LPOBJECTSTRUCT lpObj)
{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == true
		|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == true 
	  )
	{
		return;
	}
#else
	#ifdef NEW_FORSKYLAND2
	if( lpObj->m_SkillHarden != 0 )
		return;
	#endif

	#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	if( lpObj->m_iSkillStunTime > 0 )
		return;
	#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태에서는 이동을 할 수 없다
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
	{
		return;
	}
#endif

	int maxmoverange = (lpObj->m_MoveRange*2)+1;
	int searchc=10;
	BYTE tpx, tpy;
	lpObj->NextActionTime = 1000;
				
	while(searchc--)
	{
		__try
		{
			tpx = (lpObj->X-lpObj->m_MoveRange)+(rand()%maxmoverange);
			tpy = (lpObj->Y-lpObj->m_MoveRange)+(rand()%maxmoverange);		
		}
		__except( maxmoverange = 1, EXCEPTION_EXECUTE_HANDLER )
		{
		}
		
		// 너무 멀리 가지 않았나?? 체크
		BOOL mchk = gObjMonsterMoveCheck(lpObj, tpx, tpy);
		/*if( mchk == FALSE )
		{
			tpx = lpObj->StartX;
			tpy = lpObj->StartY;
			mchk = TRUE;
		}*/
		BYTE attr = MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
		// 경비병일땐 안전지대도 갈수 있다.
		if( (lpObj->Class == 249 || lpObj->Class == 247) && mchk )
		{
			if( ((attr&MAP_ATTR_STAND)!=MAP_ATTR_STAND) )
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				lpObj->m_ActState.Move = 1;
				lpObj->NextActionTime = 3000;
				break;
			}
		}
		else 
		{
			if( ((attr&MAP_ATTR_SAFTYZONE)!=MAP_ATTR_SAFTYZONE) && 
				((attr&MAP_ATTR_STAND)!=MAP_ATTR_STAND) && 
				((attr&MAP_ATTR_BLOCK)!=MAP_ATTR_BLOCK) && 
				((attr&MAP_ATTR_HOLLOW)!=MAP_ATTR_HOLLOW) && 
				mchk )
			{
				lpObj->TargetNumber       = -1;
				lpObj->m_ActState.Attack  = 0;
				lpObj->NextActionTime     = 500;
				lpObj->m_ActState.Emotion = 0;
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				lpObj->m_ActState.Move = 1;
				//lpObj->NextActionTime = 1000;
				break;
			}			
		}
	}
}

void gObjMonsterBaseAct(LPOBJECTSTRUCT lpObj)
{
	LPOBJECTSTRUCT lpTargetObj=NULL;

	if( lpObj->TargetNumber >= 0 )
		lpTargetObj = (LPOBJECTSTRUCT)&gObj[lpObj->TargetNumber];
	else lpObj->m_ActState.Emotion = 0; 

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// 슬립상태면 공격을 못함
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
		return;
#endif
	
	if( lpObj->m_ActState.Emotion == 0 )	// 보통..
	{
		if( lpObj->m_Attribute != 0 )	//
		{
			if( lpObj->m_ActState.Attack )	// 이전에 공격중 이었다면..
			{
				lpObj->m_ActState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
				//if( lpObj->m_Attribute == 100 )	LogAdd("공격중 이었다면 잠깐 쉰다.");
			}
			
			int actcode1 = (rand()%2);

			if( lpObj->m_Attribute == 100 )
			{
				actcode1 = 1;
			}

			// 잠깐 쉬고.. 
			if( actcode1 == 0 )
			{
				//if( lpObj->m_Attribute == 100 )	LogAdd("잠깐 쉰다.");
				lpObj->m_ActState.Rest = 1;
				lpObj->NextActionTime = 500;
				//lpObj->NextActionTime = 1;
			}
			// 다른곳으로 이동 해 본다.

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
			else if( lpObj->m_MoveRange > 0 &&
				( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STONE ) == false ) &&
				( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_STUN ) == false ) 
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
				&& ( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == false ) 
#endif
			       )
	#else
			else if( lpObj->m_MoveRange>0 && lpObj->m_SkillHarden == 0 && lpObj->m_iSkillStunTime == 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#else
	#ifdef NEW_FORSKYLAND2
			else if( lpObj->m_MoveRange>0 && lpObj->m_SkillHarden == 0)
	#else
			else if( lpObj->m_MoveRange>0) 
	#endif		

#endif
			{
				if( lpObj->m_Attribute != 100 )
				{
					gObjMonsterMoveAction(lpObj);
				} 
				else
				{
					int tx=0, ty=0;
					if( lpObj->m_RecallMon >= 0 )
					{
						LPOBJECTSTRUCT lpRecallObj;						
						if( gObj[lpObj->m_RecallMon].Connected > 2 )
						{
							lpRecallObj = &gObj[lpObj->m_RecallMon];
							if( !lpRecallObj->m_Rest )
							{
								if( gObjGetTargetPos(lpObj, lpRecallObj->X, lpRecallObj->Y, tx, ty)==TRUE)
								{
									lpObj->MTX = tx;
									lpObj->MTY = ty;
									lpObj->m_ActState.Move = 1;
									lpObj->NextActionTime = 1000;
									//if( lpObj->m_Attribute == 100 )	LogAdd("주인 곁으로 이동해본다.");
									//lpObj->NextActionTime = 0;
								}
							}
						}
						//gObjMonsterMoveRegen(gObj[lpObj->m_RecallMon].X, gObj[lpObj->m_RecallMon].Y, lpObj);
					}
				}
			}

			// 적을 찾아다닌다.
			if( lpObj->Class == 249 || lpObj->Class == 247 ) // 경비병일땐 PK 찾는다.
			{
				// 수호길드 모집안내.. 행사 안내 
				if( gEvent1 )
				{
					if( (rand()%30) == 0 )
					{
						if( lpObj->m_PK_Count == 0 )
						{
							ChatSend(lpObj, g_GlobalConfig.m_szGuardMessage);
							lpObj->m_PK_Count = 0;
						}
					}
				}

				lpObj->TargetNumber = gObjGuardSearchEnemy(lpObj);
				
				if( lpObj->TargetNumber >= 0 )
				{
					if( gObj[lpObj->TargetNumber].Class >= 248 )
					{
						lpObj->TargetNumber = -1;
					}
				}
			}
			else 
			{
				if( lpObj->m_Attribute == MON_ATTR_100 )
				{
					//lpObj->TargetNumber = gObjCallMonsterSetEnemy(lpObj);
					//if( lpObj->TargetNumber == -1 )
					//{
						lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJTYPE_MONSTER);
						if( lpObj->m_Attribute == 100 )	
						{
							//if( lpObj->TargetNumber>=0) LogAdd("적을 찾아 이동한다.");
						}

					//}
					//else LogAdd("주인 공격 몬스터 세팅 %d", lpObj->TargetNumber);

				}
				else lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJTYPE_CHARACTER);
			}
			
			// 적이 발견되면 열받아서 적을 향해 돌격~~
			if( lpObj->TargetNumber >= 0 )
			{
				lpObj->m_ActState.EmotionCount = 30;
				lpObj->m_ActState.Emotion = 1;
				//if( lpObj->m_Attribute == 100 )	LogAdd("적을 향해 돌격");
			}
		}
	}
	else if( lpObj->m_ActState.Emotion == 1 )	// 열받음..
	{		
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		if( lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0 )	// 공격 대상이 있으면..
		{
#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {				// 현재는 블러드캐슬만 적용됨
				INT iRAND_CHANGE_TARGET = rand()%10;
				if (iRAND_CHANGE_TARGET == 3) {
					lpObj->LastAttackerID = -1;
				}
				if (iRAND_CHANGE_TARGET == 1) {
					if ((lpObj->LastAttackerID != -1) && (lpObj->LastAttackerID != lpObj->TargetNumber)) {
						// 나를 마지막에 공격한 사람이 내가 목표로한 사람과 다르다.
						if ((gObj[lpObj->LastAttackerID].Connected > 2) && (lpObj->MapNumber == gObj[lpObj->LastAttackerID].MapNumber)) {
							// 나와 같은 맵상에 접속해있다면 공격대상을 바꾼다.
							lpObj->TargetNumber = lpObj->LastAttackerID;
							lpTargetObj = &gObj[lpObj->LastAttackerID];
						}
					}
				}
			}
#endif
			int dis = gObjCalDistance(lpObj, lpTargetObj);
			
			int attackRange;
			if( lpObj->m_AttackType >= 100 )
			{
				attackRange = lpObj->m_AttackRange+2;
			}
			else attackRange = lpObj->m_AttackRange;

			//if( lpObj->m_Attribute == 100 )	LogAdd("공격대상 체크1 거리 : %d", dis);

			if(  dis <= attackRange )
			{			//lpObj->m_ActState.Attack = 1;
				int tuser = lpObj->TargetNumber;
				int map = gObj[tuser].MapNumber;
				BYTE attr ;
				if( MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
				{
					attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);
					if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE)// 안전 속성이 아니면
					{
						lpObj->m_ActState.Attack = 1;
					}
					else {	// 안전 속성에 있다면.
						lpObj->TargetNumber = -1;
						lpObj->m_ActState.EmotionCount = 30;
						lpObj->m_ActState.Emotion = 1;					
					}
					lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
					lpObj->NextActionTime = lpObj->m_AttackSpeed;
					if( lpObj->m_Attribute == 100 )	{
						//LogAdd("근처로 가서 공격한다. 다음 액선 시간 %d",lpObj->NextActionTime);
					}
				}
#ifdef MODIFY_0708_BUFFIX_20070820	// 쿤둔일 경우 공격 대상이 벽뒤에 있으면 이동
#ifdef MODIFY_MONSTER_ATTACK_BUGFIX_20080902	// 모든 몬스터에 적용
				else
#else	// MODIFY_MONSTER_ATTACK_BUGFIX_20080902
				else if(lpObj->Class == 275)
#endif	// MODIFY_MONSTER_ATTACK_BUGFIX_20080902
				{
					lpObj->TargetNumber			= -1;
					lpObj->m_ActState.Attack	= 0;
					lpObj->NextActionTime		= 500;
					lpObj->m_ActState.Emotion	= 0;
					lpObj->m_ActState.Move		= 1;
				}
#endif				
			}
			else 
			{
				if( gObjMonsterGetTargetPos(lpObj) == TRUE )
				{
					if( MapC[lpObj->MapNumber].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE )
					{
						lpObj->m_ActState.Move = 1;
						//lpObj->m_ActState.Attack = 1;
						lpObj->NextActionTime = 400;
						//lpObj->NextActionTime = 0;
						lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
						//if( lpObj->m_Attribute == 100 )	LogAdd("몬스터 근처로 이동 %d",lpObj->NextActionTime);
					}
					else
					{
						gObjMonsterMoveAction(lpObj);
						lpObj->m_ActState.Emotion = 3;
						lpObj->m_ActState.EmotionCount = 10;
						//if( lpObj->m_Attribute == 100 )	LogAdd("몬스터 근처로 이동2 %d",lpObj->m_ActState.EmotionCount);
					}
				}
				else 
				{
					/*lpObj->TargetNumber = -1;
					lpObj->m_ActState.Attack = 0;
					lpObj->NextActionTime = 500;
					lpObj->m_ActState.Emotion = 3;
					lpObj->m_ActState.EmotionCount = 5;
					*/
					gObjMonsterMoveAction(lpObj);
					//if( lpObj->m_Attribute == 100 )	LogAdd("몬스터 근처로 이동3 %d",lpObj->m_ActState.EmotionCount);
				}
			}
		}
		else {			
		}
	}
	else if( lpObj->m_ActState.Emotion == 2 )	// 비열해졌다.
	{
		// 잠깐 도망가 본다.
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		lpObj->m_ActState.Move = 1;
		lpObj->NextActionTime = 800;

		if( lpTargetObj!=NULL)
		{
			int tdir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y)*2;
			
			lpObj->MTX += RoadPathTable[tdir]*-3;	// 일단 도망간다.
			lpObj->MTY += RoadPathTable[tdir+1]*-3;
		}
		//if( lpObj->m_Attribute == 100 )	LogAdd("도망간다.");
	}
	else if( lpObj->m_ActState.Emotion == 3 )	// 잠시 데미지를 입어 지연을 준다.
	{
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		lpObj->m_ActState.Move = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->NextActionTime = 400;
		//if( lpObj->m_Attribute == 100 )	LogAdd("데미지후 지연");
	}
}


// 트랩이 공격할때 적을 찾는다.
void gObjTrapAttackEnemySearchX(LPOBJECTSTRUCT lpObj, int count)
{
	int  tObjNum;

	lpObj->TargetNumber = -1;

	int pos = lpObj->X;
	int y   = lpObj->Y;
	for( int n=0; n<count; n++)
	{
		pos++;
		for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].number;
			if( tObjNum >= 0  )
			{
				if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
				{
					if( (y == gObj[tObjNum].Y) && (pos == gObj[tObjNum].X) )
					{
						if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
						{
							lpObj->TargetNumber = tObjNum;
							return;
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearchY(LPOBJECTSTRUCT lpObj, int count)
{
	int  tObjNum;

	lpObj->TargetNumber = -1;

	int pos = lpObj->Y;
	int x   = lpObj->X;
	for( int n=0; n<count; n++)
	{
		pos--;
		for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].number;
			if( tObjNum >= 0) 
			{
				if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
				{
					if( (pos == gObj[tObjNum].Y) && (x == gObj[tObjNum].X) )
					{
						if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
						{
							lpObj->TargetNumber = tObjNum;
							return;
						}
					}
				}
			}
		}
	}
}

void gObjTrapAttackEnemySearch(LPOBJECTSTRUCT lpObj)
{
	int  tObjNum;
	lpObj->TargetNumber = -1;
	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				if( (lpObj->Y == gObj[tObjNum].Y) && (lpObj->X == gObj[tObjNum].X) )
				{
					if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
					{
						lpObj->TargetNumber = tObjNum;
						return;
					}
				}
			}
		}
	}	
}

#ifdef ADD_KANTURU_CONDITION_20060725
void gObjTrapAttackEnemySearchRange(LPOBJECTSTRUCT lpObj, int iRange)
{
	int		tObjNum = -1;
	int		iTargetingRate = 0;
	int		iSuccessRate = 0;

	lpObj->TargetNumber = -1;

	if( lpObj->VPCount <= 0 )
	{
		return;
	}

	iTargetingRate = ( 100 / lpObj->VPCount );

	for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].number;
		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
			{
				if( ( lpObj->Y - iRange <= gObj[tObjNum].Y) && (lpObj->Y + iRange >= gObj[tObjNum].Y) &&
					( lpObj->X - iRange <= gObj[tObjNum].X) && (lpObj->X + iRange >= gObj[tObjNum].X))
				{
					if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
					{
						lpObj->TargetNumber = tObjNum;

						iSuccessRate = rand() % 100;
						if( iSuccessRate < iTargetingRate )
						{
							return;
						}
					}
				}
			}
		}
	}	
}	
#endif	// ADD_KANTURU_CONDITION_20060725

void gObjMonsterTrapAct(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->VPCount2 < 1 ) return;

	if( lpObj->m_AttackRange > 0 )
	{
		if( lpObj->Dir == 3 )
		{
			gObjTrapAttackEnemySearchX(lpObj, lpObj->m_AttackRange+1);
		}
		else if( lpObj->Dir == 1 )
		{
			gObjTrapAttackEnemySearchY(lpObj, lpObj->m_AttackRange+1);
		}
#ifdef ADD_KANTURU_CONDITION_20060725
		else if( lpObj->Dir == 8 )
		{
			// 트랩 몬스터 (인덱스가 100~110상의 몬스터)의 m_AttackRange가 0이상이고 Dir이 8일 경우 범위공격 트랩
			// 트랩 공격 범위에 유저를 찾는다.
			gObjTrapAttackEnemySearchRange( lpObj, lpObj->m_AttackRange );
		}
#endif
	}
	else gObjTrapAttackEnemySearch(lpObj);

	if( lpObj->TargetNumber >= 0 ) {
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	// 공격후는 시간을 좀더 지연시킴
	}
	else lpObj->NextActionTime = lpObj->m_MoveSpeed;	// 공격을 안할때는 빨리 체킹하기 위해.. movespeed 를 사용..
}


BYTE NewOptionRand(int level)
{
	BYTE NOption=0;
	
	NOption = 1<<(rand()%6);

	if( NOption & 0x02 )
	{	// 방어 성공률 확률을 낮춘다
		if( rand()%2 ) 
		{
			NOption = 1<<(rand()%6);
		}
	}

	if( rand()%4 == 0 ) NOption |= 1<<(rand()%6);

	return NOption;
}


void InventoryDropItem(LPOBJECTSTRUCT lpObj, int DropItem)
{

}


#ifdef NEW_SKILL_FORSKYLAND
//천공맵 보스 몬스터의 방어막 공격체크
void	gObjSkylandBossSheildAttack(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->VPCount2 < 1 ) return;

	int tObjNum;

	if( lpObj->m_SkyBossMonSheildLinkIndex == -1 )
	{
		return;
	}

	if( !lpObj->m_SkyBossMonSheild )
		return;

	lpObj->TargetNumber = -1;

	int shieldindex = lpObj->m_SkyBossMonSheildLinkIndex;
	if( gObj[shieldindex].Life < 0 )
	{
		return;
	}

	if( lpObj->m_AttackRange > 0 )
	{
		for( int i=0; i<MAX_MONVIEWPORTOBJECT; i++)
		{
			tObjNum = lpObj->VpPlayer2[i].number;
			if( tObjNum >= 0  )
			{
				if( gObj[tObjNum].Type == OBJTYPE_CHARACTER && gObj[tObjNum].Live )
				{
					if( gObjCheckTileArea(gObj[tObjNum].m_Index, lpObj->X, lpObj->Y, 2) )
					{
						if( (gObj[tObjNum].Authority&AUTHORITY_ADMIN) != AUTHORITY_ADMIN ) 
						{
							lpObj->TargetNumber = tObjNum;
						}
					}
				}
			}
		}
	}

	if( lpObj->TargetNumber >= 0 ) {
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	// 공격후는 시간을 좀더 지연시킴
	}
	else lpObj->NextActionTime = lpObj->m_MoveSpeed;	// 공격을 안할때는 빨리 체킹하기 위해.. movespeed 를 사용..	
}
#endif

BOOL IsCanNotItemDtopInDevilSquare(int ItemType)
{
	int checkitemtype = ItemType/MAX_ITEM_INDEX;
	int	checkitemindex = ItemType%MAX_ITEM_INDEX;

	if( checkitemtype >= 7 && checkitemtype <= 11 )
	{	// 다크피닉스, 그랜드소울, 홀리스프잇 세트
		if( checkitemindex == 17 || checkitemindex == 18 || checkitemindex == 19 )
		{
			return FALSE;
		}
	}
	else if( checkitemtype == 0  )
	{	// 다크브레이커, 선더블레이드
		if( checkitemindex == 17 || checkitemindex == 18 )
		{
			return FALSE;
		}
	}
	else if( checkitemtype == 12  )
	{	// 분노의일격,창찌르기, 스웰라이프, 블러드어택, 관통, 아이스애로우, 일렉스트라이크 구슬
		if( checkitemindex == 12 || checkitemindex == 13 || checkitemindex == 14 || 
			checkitemindex == 16 || checkitemindex == 17 || checkitemindex == 18 || 
			checkitemindex == 19 )
		{
			return FALSE;
		}
	}	
	else if( (ItemType == MAKE_ITEMNUM(4,17)) || (ItemType == MAKE_ITEMNUM(5,9)) )
	{	// 홀리스프릿보우, 드래곤소울지팡이
		return FALSE;
	}
	return TRUE;
}


#ifdef ADD_ABILLITY_KUNDUN_01_20040920
// 몬스터 HP 회복 함수.
void gObjRefillMonsterHP( LPOBJECTSTRUCT lpMonsterObj, INT iRefillHPSec )
{
	INT iMonsterHP = lpMonsterObj->Life;
	INT	iRefillHP  = 0;

	// 6칼리마 쿤둔일 경우
	
#ifdef CHANGE_KUNDUN_SETTING_01_20050221
	// 죽어 있으면 return
	if( lpMonsterObj->Live == 0 )
	{
		return;
	}
#endif
	
	if( lpMonsterObj->Class == 275 )
	{
#ifdef ADD_KUNDUN_CONTROL_20040923
		// 쿤둔 특별한 피회복 시간이 지났으면
		if( lpMonsterObj->iObjectSecTimer > giKundunRefillHPTime )
		{
			// 피회복
			iRefillHP = rand() % ( giKundunRefillHP > 0 ? giKundunRefillHP : 1 );
			
			lpMonsterObj->iObjectSecTimer = 0;

			KUNDUN_EVENT_LOG.Output("[KUNDUN] HP Log -> [%d]회복  [%d]회복시간  [%7.0f]현재HP", 
									iRefillHP, giKundunRefillHPTime, lpMonsterObj->Life);

			lpMonsterObj->iObjectSecTimer = 0;
			
		}
		else
		{
			lpMonsterObj->iObjectSecTimer++;
		}
#endif
		
#ifdef EXTEND_LOG_SYSTEM_20060202
		if( lpMonsterObj->iObjectSecTimer % giKundunHPLogSaveTime == 0 )
		{
			LogAddTD("[KUNDUN][Status Log] HP:%d RefillHP:%d Map:%d X:%d Y:%d",
					iMonsterHP, iRefillHP, lpMonsterObj->MapNumber, lpMonsterObj->X, lpMonsterObj->Y);

			if( gObjIsConnected( lpMonsterObj->LastAttackerID ) )
			{
				LPOBJECTSTRUCT lpAttackObj = &gObj[lpMonsterObj->LastAttackerID];

				LogAddTD("[KUNDUN][Attacker Info] [%s][%s] HP:%d MP:%d X:%d Y:%d",
					lpAttackObj->AccountID, lpAttackObj->Name,
					lpAttackObj->Life, lpAttackObj->Mana, lpAttackObj->X, lpAttackObj->Y );
			}
		}
#endif
		
#ifdef MU_CRYWOLF_BENEFIT_20051215
		// MVP 성공 : 쿤둔 자생 없어짐.
		if( 
			g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_PEACE
#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315		
			&& g_CrywolfSync.GetKundunHPRefillState() == CRYWOLF_BENEFIT_KUNDUN_HP_REFILL_OFF
			&& g_iCrywolfApplyMvpBenefit
#endif // MODIFY_CRYWOLF_BENEFIT_01_20060315
		  )
		{
			iRefillHP		= 0;
			iRefillHPSec	= 0;

#if TESTSERVER == 1
			LogAddTD( "★☆ 크라이울프 혜택 적용 : 쿤둔 자생 없어짐 상태 (%d)", g_CrywolfSync.GetKundunHPRefillState() );
#endif

		}
#endif // MU_CRYWOLF_BENEFIT_20051215

	}

	iMonsterHP = iMonsterHP + iRefillHP + iRefillHPSec;

	if( iMonsterHP > lpMonsterObj->MaxLife )
		lpMonsterObj->Life = lpMonsterObj->MaxLife;
	else
		lpMonsterObj->Life = iMonsterHP;
	
#ifdef ADD_KUNDUN_CONTROL_20040923
	
#ifndef CHANGE_KUNDUN_SETTING_01_20050221
#ifdef FOR_KOREA
	if( rand() % 5 == 0 && (lpMonsterObj->Class == 275) )
	{
		for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
		{
			if( rand()%5 == 0 )
			{
				if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
				{
					if( gObj[n].MapNumber == MAP_INDEX_KALIMA6 )
					{
						TNotice _Notice(TNOTICE_TYPE_LEFTTOP);
						_Notice.SendToUser( n, "쿤둔의 생명이 %7.0f 남았습니다.", lpMonsterObj->Life );
					}
				}
			}
		}

	}
#endif
#endif // CHANGE_KUNDUN_SETTING_01_20050221

#endif // ADD_KUNDUN_CONTROL_20040923

}

#endif


#ifdef ADD_MONSTER_DIE_REWARD_ITEMS_20040915
// 아이템 드랍 10000을 기준으로 드랍률 설정
void gObjMonsterDieRewardItems( 
								   LPOBJECTSTRUCT lpObj, 
								   LPOBJECTSTRUCT lpTargetObj,
								   INT  iCount,
								   INT	iDropRateCommonItem, 
								   INT  iDropRateExcellentItem,
								   INT  iDropRateSetItem,
								   BOOL bMustHaveSkill,
								   BOOL bMustHaveLuck,
								   BOOL bMustHaveAdditionalOption
							  )
{

	int store_count=0;
	int DropItemNum = 0;
	int type, level, x, y;
	float dur=0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	BYTE NOption= 0;
	BOOL item_drop=0;
	BOOL exitem_drop=0;
	CItem * DropItem=NULL;

	// Item Looting 설정
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	// iCount 만큼 드랍 위치 설정
	
	// 세트 아이템
	int iSetDropPer		= iDropRateSetItem;
	int iExtDropPer		= iDropRateExcellentItem;
	int iCommonDropPer	= iDropRateCommonItem;

	for( int i=0; i<iCount; i++ )
	{
		UCHAR cDropX = lpObj->X;
		UCHAR cDropY = lpObj->Y;

		if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
		{
			cDropX = (BYTE)lpObj->X;
			cDropY = (BYTE)lpObj->Y;
		}

		// 액설런트 아이템
		if( rand()%10000 < iExtDropPer )
		{
			DropItem = g_MonsterItemMng.GetItem(lpObj->Level - 25);
			if( DropItem == NULL )
			{
				item_drop = 0;
				exitem_drop = 0;
			}
			else
			{
				item_drop = 1;
				exitem_drop = 1;
			}
		}
		// 일반 아이템
		else
		if( rand()%10000 < iCommonDropPer ) 
		{
			DropItem = g_MonsterItemMng.GetItem(lpObj->Level);
			if( DropItem == NULL )
			{
				item_drop = 0;
			}
			else
			{
				item_drop = 1;
			}
		}

		if( item_drop )
		{
			type = DropItem->m_Type;
			level= DropItem->m_Level;
			dur  = DropItem->m_Durability;
			x    = cDropX;
			y    = cDropY;
			
			int option1rand, option2rand, option3rand, optionc;
			
			// 엑설런트 아이템의 경우 스킬은 무조건 붙으며 기존 옵션들은 정말 희박함
			if( exitem_drop )
			{
				option1rand = 100;
				option2rand = 1;
				option3rand = (rand()%100);
				optionc		= (rand()%3);
				NOption		= NewOptionRand(lpObj->Level);
				level       = 0;
			}
			else
			{
				option1rand = 6;				// 일반적인 옵션1 (스킬) 붙을 확률 6/100
				option2rand = 4;				// 일반적인 옵션2 (행운) 붙을 확률 4/100
				option3rand = (rand()%100);		// 일반적인 옵션3 (추가포인트) 붙을 확률 랜덤/100
				optionc		= (rand()%3);
				NOption     = 0;
			}
			
			// 스킬아이템 확률을 대폭 낮춘다.
			if((rand()%100) < option1rand ) Option1 = 1;
			if((rand()%100) < option2rand ) Option2 = 1;
			
			switch( optionc )
			{
			case 0 :
				if( option3rand < 4 ) Option3 = 3;	// 옵션
				break;
			case 1 :
				if( option3rand < 8 ) Option3 = 2;	// 옵션
				break;
			case 2 :
				if( option3rand < 12 ) Option3 = 1;	// 옵션
				break;
			}
			
			if( DropItem->m_serial )
			{
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser, NOption );
			}
			else 
			{
				MapC[lpObj->MapNumber].MonsterItemDrop(type, level, dur, x, y, Option1, Option2, Option3, NOption, 0, MaxHitUser);			
			}
		}

	}
}

#endif

// 몬스터가 죽어서 아이템을 남긴다.??
void gObjMonsterDieGiveItem(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	int store_count=0;
	int ExtDropPer=0;
	int DropItemNum = 0;
	int type, level, x, y;
	float dur=0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	BYTE NOption= 0;
	BOOL item_drop=0;
	int n;
	CItem * DropItem=NULL;
	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// 중국변경> . 전체 몬스터 아이템 드롭 확률을 조정함.
	if (g_bChnPlayTimeLimitOn) {
		INT iItemDrapRate = 100;
#ifdef MODIFY_CHN_PLAYTIME_LIMIT_SYSTEM_01_20070509
		if( lpTargetObj->m_iVerifyType != PLAYTIME_LIMIT_VERIFY_ADULT )
		{
			if (lpTargetObj->m_iPlayTimeLimit >= 5*60*60)
			{
				iItemDrapRate = g_iChnPlayTimeLimitLevel2ItemDrop;
			}
			else if (lpTargetObj->m_iPlayTimeLimit >= 3*60*60) {
				iItemDrapRate = g_iChnPlayTimeLimitLevel1ItemDrop;
			}
		}
#else
		if (lpTargetObj->m_iPlayTimeLimit >= 5*60*60)
		{
			iItemDrapRate = g_iChnPlayTimeLimitLevel2ItemDrop;
		}
		else if (lpTargetObj->m_iPlayTimeLimit >= 3*60*60) {
			iItemDrapRate = g_iChnPlayTimeLimitLevel1ItemDrop;
		}
#endif // CHN_PLAYTIME_LIMIT_SYSTEM_20050818
		
		if (!iItemDrapRate)
			return;

		if (rand()%100 >= iItemDrapRate) {	// 중국추가> . 돈 드롭액수만 50%

			// 돈을 떨어트린다.	
			if(rand()%2) {
				int x, y;
				float money;
				
				x    = lpObj->X;
				y    = lpObj->Y;
				
				money = (float)lpObj->Money;
				
				// 몬스터 죽였을때 얻는돈
				money += money/100*(lpTargetObj->MonsterDieGetMoney*(iItemDrapRate/100));
				money += 7;

#ifdef FOR_NEW_TESTSERVER			
				money *= 3;		// 젠 드롭 3배
#endif
				
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
				if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
				if( lpObj->MapNumber == 9 )
#endif
				{	// 악마의 광장에서는 돈을 누적한다
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
#ifdef MODIFY_MAXHITUSER_BUG_FIX_20070711
					if(MaxHitUser == -1)
						return;
#endif
					gObj[MaxHitUser].m_nEventMoney += (int)money;				
				}
				else
				{
					MapC[lpObj->MapNumber].MoneyItemDrop((int)money, x ,y);
				}
			}
			return;
		}
	}
#endif
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// 베트남> . 전체 몬스터 아이템 드롭 확률을 조정함.
	if (g_bVtmPlayTimeLimitOn) 
	{
		INT iItemDrapRate = 100;
		if (lpTargetObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel2Time * 60) 
		{
			iItemDrapRate = g_iVtmPlayTimeLimitLevel2ItemDrop;
		}
		else if (lpTargetObj->m_iPlayTimeLimit >= g_iVtmPlayTimeLimitLevel1Time * 60) 
		{
			iItemDrapRate = g_iVtmPlayTimeLimitLevel1ItemDrop;
		}
		
		if (!iItemDrapRate)
			return;

		if (rand()%100 >= iItemDrapRate) 
		{	// 중국추가> . 돈 드롭액수만 50%

			// 돈을 떨어트린다.	
			if(rand()%2) {
				int x, y;
				float money;
				
				x    = lpObj->X;
				y    = lpObj->Y;
				
				money = (float)lpObj->Money;
				
				// 몬스터 죽였을때 얻는돈
				money += money/100*(lpTargetObj->MonsterDieGetMoney*(iItemDrapRate/100));
				money += 7;

#ifdef FOR_NEW_TESTSERVER			
				money *= 3;		// 젠 드롭 3배
#endif
				
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
				if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
				if( lpObj->MapNumber == 9 )
#endif
				{	// 악마의 광장에서는 돈을 누적한다
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
#ifdef MODIFY_MAXHITUSER_BUG_FIX_20070711
					if(MaxHitUser == -1)
						return;
#endif
					gObj[MaxHitUser].m_nEventMoney += (int)money;				
				}
				else
				{
					MapC[lpObj->MapNumber].MoneyItemDrop((int)money, x ,y);
				}
			}
			return;
		}
	}
#endif
	
#ifdef ADD_NEWPVP_PKFIELD
	if (g_NewPVP.IsVulcanusMonster(lpObj->Class))
	{
		// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		if (g_NewPVP.DropItem(lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y, iMaxHitUser))
			return;
	}
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// 설날복주머니일 경우 아이템 드럽
	if( lpObj->Class == MON_NEWYEAR_LUCKYBAG )
	{
		// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
		int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );	

		//LogAddTD();

		UCHAR cDropX = lpObj->X;
		UCHAR cDropY = lpObj->Y;

		//if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 2, 2, 2 ) )
		//{
		//	cDropX = (BYTE)lpObj->X;
		//	cDropY = (BYTE)lpObj->Y;
		//}
		
		NewYearLuckyBagItemBagOpen(lpTargetObj, lpObj->MapNumber, cDropX, cDropY);
		return;
	}
#endif

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// 크라이울프 다크엘프 아이템 드랍
	if( lpObj->Class == CRYWOLF_MON_DARKELF )
	{
		if( g_bCrywolfMonsterDarkElfItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iCrywolfMonsterDarkElfItemDropRate )
			{			
				// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
				int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );		
				
				LogAddTD("[ Crywolf ][Reward] Monster Dark Elf ItemDrop MaxHitUser [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

				UCHAR cDropX = lpObj->X;
				UCHAR cDropY = lpObj->Y;

				if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
				{
					cDropX = (BYTE)lpObj->X;
					cDropY = (BYTE)lpObj->Y;
				}

				CrywolfDarkElfItemBagOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY );
			}
		}
		
		return;
	}
#endif	// #ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// 크라이울프 보스몬스터 발가스 아이템 드랍
	if( lpObj->Class == CRYWOLF_MON_BALGARS )
	{
		if( g_bCrywolfBossMonsterItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iCrywolfBossMonsterItemDropRate )
			{
				// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
				int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );		
				
				LogAddTD("[ Crywolf ][Reward] Boss Monster ItemDrop MaxHitUser [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

				UCHAR cDropX = lpObj->X;
				UCHAR cDropY = lpObj->Y;

				if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
				{
					cDropX = (BYTE)lpObj->X;
					cDropY = (BYTE)lpObj->Y;
				}

				CrywolfBossMonsterItemBagOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY );
			}
		}
		return;
	}
#endif	// #ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404

#ifdef KANTURU_MONSTER_ITEMDROP_20060807	// 칸투르 몬스터 아이템 드랍
	// 마야 손
	if( lpObj->Class == KANTURU_MONSTER_MAYA_LEFTHAND || lpObj->Class == KANTURU_MONSTER_MAYA_RIGHTHAND )
	{
		if( g_bKanturuMayaHandItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iKanturuMayaHandItemDropRate )
			{			
				// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
				int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );		
				
				LogAddTD("[ KANTURU ][ Reward ] MayaHand ItemDrop MaxHitUser [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

				UCHAR cDropX = lpObj->X;
				UCHAR cDropY = lpObj->Y;

				if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
				{
					cDropX = (BYTE)lpObj->X;
					cDropY = (BYTE)lpObj->Y;
				}

				KanturuMayaHandItemBagOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY );
			}
		}
		
		return;
	}

	// 나이트메어
	if( lpObj->Class == KANTURU_MONSTER_NIGHTMARE )
	{
		if( g_bKanturuNightmareItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iKanturuNightmareItemDropRate )
			{			
				// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
				int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );		
				
				LogAddTD("[ KANTURU ][ Reward ] Nightmare ItemDrop MaxHitUser [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

				UCHAR cDropX = lpObj->X;
				UCHAR cDropY = lpObj->Y;

				if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
				{
					cDropX = (BYTE)lpObj->X;
					cDropY = (BYTE)lpObj->Y;
				}

				KanturuNightmareItemBagOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY );
			}
		}
		
		return;
	}
#endif	// #ifdef KANTURU_MONSTER_ITEMDROP_20060807

#ifdef ADD_RAKLION_20080408
	if( lpObj->Class == RAKLION_SELUPAN && lpObj->Connected==CSS_GAMEPLAYING )
	{
		if( g_bRaklionSelupanItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iRaklionSelupanItemDropRate )
			{
				// 아이템은 누적 데미지의 합이 가장 큰 캐릭터에게 지급된다.
				int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );		
				
				LogAddTD("[ RAKLION ][ Reward ] Raklion ItemDrop MaxHitUser [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );
				
				UCHAR cDropX = lpObj->X;
				UCHAR cDropY = lpObj->Y;
				
				for( INT iCount=0; iCount<10; ++iCount )
				{
					if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 2, 2, 10 ) )
					{
						cDropX = (BYTE)lpObj->X;
						cDropY = (BYTE)lpObj->Y;
					}
					
					RaklionSelupanItemBagOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY );
				}
			}
		}
		
		return;
	}
#endif // ADD_RAKLION_20080408
	
#ifdef CASTLE_NPC_DONT_DROP_ITEM_20050517	// 공성전 NPC들은 아이템을 떨구지 않는다.
	if (lpObj->m_btCsNpcType != CS_NPC_TYPE_NONE)
		return;
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
	if( lpObj->Class == 295 ) // 성사냥터 보스몹 에로힘.
	{
		// Item Looting 설정
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

		INT iMaxNumOfRewardItem = 1;
		for( int i=0; i<iMaxNumOfRewardItem; i++)
		{
			UCHAR cDropX = lpObj->X;
			UCHAR cDropY = lpObj->Y;

			LogAddTD("[Castle HuntZone] Boss Monster ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );

			if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
			{
				cDropX = (BYTE)lpObj->X;
				cDropY = (BYTE)lpObj->Y;
			}

			if( i == 0 ) // 한개만 떨어질 경우
			{
				cDropX = (BYTE)lpObj->X;
				cDropY = (BYTE)lpObj->Y;
			}
			
			CastleHuntZoneBossRewardOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY);

		}

		return;
	}
#endif

#ifdef MODIFY_KUNDUN_REWARD_01_20040915
	// 쿤둔이 죽었을때 아이템을 떨군다.
	if( lpObj->Class == 275 )
	{
		// 0. 쿤둔이 죽었다는 로그를 기록한다.
		LogAddTD("[★★Kundun EVENT] Kundun die, Killer [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );
		KUNDUN_EVENT_LOG.Output("[★★Kundun EVENT] Kundun die, Killer [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

		// 1. 현재 칼리마6에 있는 모든 유저들을 로그로 기록한다.
		// MODIFY_KUNDUN_LOG_20060201
		// 20060201 로그 주석 처리 (RKFKA)
//		for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
//		{
//			if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
//			{
//				if( gObj[n].MapNumber == MAP_INDEX_KALIMA6 )
//				{
//					LogAddTD("[★Kundun EVENT] In KALIMA(6), User List [%s][%s]", gObj[n].AccountID, gObj[n].Name );
//					KUNDUN_EVENT_LOG.Output("[★Kundun EVENT] In KALIMA(6), User List [%s][%s]", gObj[n].AccountID, gObj[n].Name );
//				}
//			}
//		}


		// Item Looting 설정
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
#ifdef CHANGE_KUNDUN_SETTING_01_20050221
		INT iMaxNumOfRewardItem = 3;
		for( int i=0; i<iMaxNumOfRewardItem; i++)
#else
		for( int i=0; i<5; i++ )
#endif
		{
			UCHAR cDropX = lpObj->X;
			UCHAR cDropY = lpObj->Y;
			
			if( CHECK_LIMIT(MaxHitUser, MAX_OBJECT) )
			{
				LogAddTD("[★Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
				KUNDUN_EVENT_LOG.Output("[★Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );

				// MODIFY_KUNDUN_LOG_20060201
				//LogAddTD("[★Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
				//KUNDUN_EVENT_LOG.Output("[★Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
			}
			else
			{
				LogAddTD("[★Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				KUNDUN_EVENT_LOG.Output("[★Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );

				// MODIFY_KUNDUN_LOG_20060201
				//LogAddTD("[★Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				//KUNDUN_EVENT_LOG.Output("[★Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
			}
			
			if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 4, 4, 10 ) )
			{
				cDropX = (BYTE)lpObj->X;
				cDropY = (BYTE)lpObj->Y;
			}
			
			if( lpObj->Class == 275 )
			{
				if( rand()%10000 < (g_GlobalConfig.m_iAncientDrop*(lpObj->m_wAncientDropRate/100.0f)) )
				{
					// Type 1일 경우 쿤둔을 죽인것에 대한 보상!
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
					MakeRewardSetItem( MaxHitUser, cDropX, cDropY, 1, lpObj->MapNumber );
#else
					MakeRewardSetItem( MaxHitUser, cDropX, cDropY, 1 );
#endif // #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
					LogAddTD("[○Kundun EVENT] Drop SetItem ");
					KUNDUN_EVENT_LOG.Output("[○Kundun EVENT] Drop SetItem ");
					continue;
				}
			}
			
			// 쿤둔 (칼리마7) 일 경우는 무조건 아이템이 드롭된다.
			if( CHECK_LIMIT(MaxHitUser, MAX_OBJECT) )
			{
				// ADD_KUNDUN_ITEM_DROP_LOG_20060309
				LogAddTD("[○Kundun EVENT] Drop Item [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );				
				KUNDUN_EVENT_LOG.Output("[○Kundun EVENT] Drop Item [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
			}
			else
			{
				// ADD_KUNDUN_ITEM_DROP_LOG_20060309
				LogAddTD("[○Kundun EVENT] Drop Item [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				KUNDUN_EVENT_LOG.Output("[○Kundun EVENT] Drop Item [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
			}

			// MODIFY_ITEM_DROP_MAP_OF_EROHIM_KUNDUN_20050610
			KundunEventItemBoxOpen( lpTargetObj, lpObj->MapNumber, cDropX, cDropY);
		}

		return;
	}
#endif


	if( lpObj->Class == 249 || lpTargetObj->Class == 249 || 
		lpObj->Class == 247 || lpTargetObj->Class == 247 ) 
	{		
		return;
	}
	
	if( lpObj->m_RecallMon >= 0 )						// 소환 몬스터는 아이템이나 돈을 떨구지 않는다.
	{	
		
		return;
	}

#ifdef FOR_BLOODCASTLE
	if(lpObj->Class == 131)								// 성문일 경우에는 아무것도 떨구지 않는다.
	{
		return;
	}

	if (CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))	// 성자의석상 시리즈 일때도 아무것도 떨구지 않는다. (=> 리젠될 때 떨군다.)
	{
		return;
	}
#endif

#ifdef ADD_RAKLION_20080408								// 라클리온 거미알은 아이템 드랍을 하지 않는다
	if( lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 )
	{
		return;
	}
#endif // ADD_RAKLION_20080408
	

#ifdef CHAOSCASTLE_SYSTEM_20040408						// 몬스터가 죽어서 아이템을 드롭한다 -> 카오스 캐슬의 몬스터는 지정된 아이템만 드롭한다.
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) 
	{
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// 카오스캐슬 번호 구하기
		g_ChaosCastle.SearchNDropMonsterItem ( g_ChaosCastle.GetChaosCastleByMapNum( lpObj->MapNumber ), lpObj->m_Index, lpTargetObj->m_Index );
#else
		g_ChaosCastle.SearchNDropMonsterItem (lpObj->MapNumber-MAP_INDEX_CHAOSCASTLE1, lpObj->m_Index, lpTargetObj->m_Index);
#endif
		// 여기에서 리젠 시키지 말고 지운다.
		gObjDel (lpObj->m_Index);
		
#ifdef FOR_KOREA		
		// 특정 확률에 따라 카오스캐슬 오프라인 경품에 당첨될 수 있다.
		g_ChaosCastle.CheckRegisterOfflineGift(lpTargetObj->m_Index);
#endif
		return;
	}

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908				// 중국 경품 이벤트 (어느 맵이나 받을 수 있다.)
	g_ChaosCastle.CheckRegisterOfflineGift(lpTargetObj->m_Index);
#endif

#endif
	

#ifdef MONSTER_HERD_SYSTEM_20031120
	if (lpObj->m_bIsInMonsterHerd) 
	{
		MonsterHerd * lpMH = lpObj->m_lpMonsterHerd;

		if (lpMH) {
			if (lpMH->MonsterHerdItemDrop(lpObj))
				return;
		}
	}
#endif
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// 환영 사원에서 몬스터 아이템 드랍
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
	{
		int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );	
		
		g_IllusionTempleEvent.MonsterDieItemDrop(lpTargetObj);
		return;
	}
#endif
	
#ifdef CHUSEOK_MONSTER_EVENT_20070823	// 추석 몬스터이벤트 - 달토끼 아이템 드랍
	if( lpObj->Class == 413 )
	{
		ChuseokMonsterEventItemBagOpen( lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y );		
		return;	
	}
#endif

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// 여름 불꽃 유령 이벤트
	if( lpObj->Class == 463 )	// 불꽃 유령
	{
		FireCrackerMonsterEventItemBagOpen( lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y );		
		return;	
	}
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

	int itemrate=lpObj->m_ItemRate;
	int moneyrate=lpObj->m_MoneyRate;

	if( itemrate < 1 ) itemrate = 1;
	if( moneyrate < 1 ) moneyrate = 1;

	if( lpObj->Class == 44 )	// 환수 드래곤 일 경우
	{
		
#ifdef	MODIFY_JEWELOFBLESS_SELL_BUGFIX
		dur = 0;
#else
		dur  = 255;
#endif
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 0;
		if( (rand()%4) > 0 )	// 1/5 로 아이템 나올 활률
		{
			for( int n=0; n<4; n++)
			{
				int x, y;
				x    = lpObj->X-2;
				y    = lpObj->Y-2;
				x   += rand()%3;
				y   += rand()%3;			
				MapC[lpObj->MapNumber].MoneyItemDrop(10000, x ,y);
			}			
			return;
		}
		if( (rand()%3) < 2 )
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			type = ItemGetNumberMake(14, 13);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return;
		}
		else
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			type = ItemGetNumberMake(14, 14);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return;
		}		
	}
	else if( lpObj->Class == 43 )	// 황금버지드래곤 일 경우
	{
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 0;
		type = ItemGetNumberMake(14, 11);
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
#ifndef ADD_GOLDEN_EVENT_RENEWAL_20090311
	else if( lpObj->Class == 53 )
	{	// 타이탄
#ifdef ELEGORADO_EVENT
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 9;	// 엘도라도의 상자
		type = ItemGetNumberMake(14, 11);
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
#else
		if( AttackEvent53BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
#endif
	}
	else if( lpObj->Class == 55 )
	{	// 데쓰킹
		if( AttackEvent55BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
	}
#ifdef ELEGORADO_EVENT
	else if( lpObj->Class == 78 )
	{	// 황금 고블린		
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;		
		dur  = 255;
		level= 8;
		type = ItemGetNumberMake(14, 11);
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 79 )
	{	// 골드 데르콘	
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;

		dur  = 255;		
		level= 10;
		type = ItemGetNumberMake(14, 11);		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 80 )
	{	// 데블 리자드킹
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;

		dur  = 255;	
		level= 11;
		type = ItemGetNumberMake(14, 11);		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 82 )
	{	// 칸투르 1		
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

		x    = lpObj->X;
		y    = lpObj->Y;

		dur  = 255;		
		level= 12;				
		type = ItemGetNumberMake(14, 11);	

		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
#endif
#else //ADD_GOLDEN_EVENT_RENEWAL_20090311
	else if(lpObj->Class == 78 || lpObj->Class == 502 || lpObj->Class == 493)	
	{
		
		// 황금버지드래곤, 황금고블린 , 황금토끼, 황금다크나이트 (황금타이탄은 아래로 뺏다)
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 8;
		type = ItemGetNumberMake(14, 11);
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 53 ||  lpObj->Class == 54 )
	{	// 타이탄, 황금 솔져 
#ifdef ELEGORADO_EVENT
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 8;	// 엘도라도의 상자
		type = ItemGetNumberMake(14, 11);
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
#else
		if( AttackEvent53BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
#endif
	}
	
	
	else if( lpObj->Class == 80 || lpObj->Class == 81 || lpObj->Class == 494)
	{	// 데블 리자드킹 , 황금베파르 , 황금 데빌
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;
		
		dur  = 255;	
		level= 9;
		type = ItemGetNumberMake(14, 11);		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 82 || lpObj->Class == 83 || lpObj->Class == 495 || lpObj->Class == 496)
	{	// 황금탄탈로스 , 황금휠 , 황금돌괴물, 황금크러스트
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;
		
		dur  = 255;		
		level= 10;				
		type = ItemGetNumberMake(14, 11);	
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 497 || lpObj->Class == 498)
	{
		// 황금 사티로스, 황금트윈테일
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;
		
		dur  = 255;		
		level= 11;				
		type = ItemGetNumberMake(14, 11);	
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 499 || lpObj->Class == 500)
	{
		//황금아이언나이트, 황금네이핀
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		x    = lpObj->X;
		y    = lpObj->Y;
		
		dur  = 255;		
		level= 12;				
		type = ItemGetNumberMake(14, 11);	
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return;
	}
	else if( lpObj->Class == 79 )
	{	// 골드 데르콘	
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		UCHAR cDropX = lpObj->X;
		UCHAR cDropY = lpObj->Y;

		dur  = 255;		
		int nLevel[3] = {8,9,10};

				
		for( INT iCount = 0; iCount < 5; iCount++ )
		{
			if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 2, 2, 10 ) )
			{
				cDropX = lpObj->X;
				cDropY = lpObj->Y;
			}
			level= nLevel[rand()%3];
			type = ItemGetNumberMake(14, 11);		
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, cDropX, cDropY, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		}

		
		return;
	}
	else if(lpObj->Class == 501)
	{
		// 그레이트 황금 드래곤 
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		
		UCHAR cDropX = lpObj->X;
		UCHAR cDropY = lpObj->Y;

		dur  = 255;		
		int nLevel[2] = {11,12};
		
		
		for( INT iCount = 0; iCount < 5; iCount++ )
		{
			if( !gObjGetRandomItemDropLocation( lpObj->MapNumber, cDropX, cDropY, 2, 2, 10 ) )
			{
				cDropX = lpObj->X;
				cDropY = lpObj->Y;
			}
			level= nLevel[rand()%2];
			type = ItemGetNumberMake(14, 11);		
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, cDropX, cDropY, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		}
		
		
		return;
	}

	else if( lpObj->Class == 55 )
	{	// 데쓰킹
		if( AttackEvent55BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
	}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311


	// 이벤트 아이템 드랍을 설정한다
	if( gEventMonsterItemDrop(lpObj, lpTargetObj) )
	{		
		return;	
	}


#ifdef HAPPY_POUCH	//happycat@20050201 (복주머니) - 국내 설 이벤트
	CMobDropItemInfo ItemInfo;

	if (g_GameEvent.GetEventItem(lpObj->MapNumber, ItemInfo))
	{
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, 
			ItemInfo.m_wType, ItemInfo.m_level, ItemInfo.m_fDurability, ItemInfo.m_option1, ItemInfo.m_option2, ItemInfo.m_option3, gObjMonsterTopHitDamageUser(lpObj));
		return; // apple★20050203
	}

#endif

#ifdef NEW_SKILL_FORSKYLAND
	// 퀘스트 아이템 드랍을 설정한다
	if( g_QuestInfo.MonsterItemDrop(lpObj) )
	{		
		return;	
	}
#endif

	// 레벨 20 이하이면 사과 드롭 확률이 높아진다.
	if (lpTargetObj->Level <= 20) {
		if (rand()%10000 < APPLE_DROPRATE) 
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			x    = lpObj->X;
			y    = lpObj->Y;
			dur  = 1;		
			level= 0;		
			type = ItemGetNumberMake(14, 0);	
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		}
	}

	
	
#ifdef CHANGE_EX_ITEM_DROP_RATE_20040524	
	ExtDropPer = rand()%lpTargetObj->m_wExcellentDropRate;
#else
	// ADD_REWARD_ABOUT_KALIMA_BOSS_01_20040907 
	// 1200->2000으로 수정 - 2004.09.08 (apple)
	ExtDropPer = rand()%lpTargetObj->m_wExcellentDropRate;
#endif
	

#ifdef FOR_BLOODCASTLE
	// 블러드캐슬 몬스터일 경우 새로운 엑셀 드롭율을 적용한다.
//	if (CHECK_BLOODCASTLE(lpObj->MapNumber))
//		ExtDropPer = rand() % g_BloodCastle.GetExcelItemDropRate();
#endif

	int	ItemDropPer = gItemDropPer;

	ItemDropPer += gItemDropPer*lpTargetObj->SetOpImproveItemDropRate/100;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_CASHITEM_4RD_MARK_20080723
	{
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		if( iMaxHitUser >= 0 )
		{
			int iItemDropRate = gObjGetTotalValueOfEffect( &gObj[iMaxHitUser], EFFECTTYPE_ITEMDROPRATE );
			if( iItemDropRate > 0 )
			{
				ItemDropPer = ItemDropPer * iItemDropRate / 100;
				ExtDropPer = rand() % ( 2000 / iItemDropRate * 100 );
			}
		}
	}
#endif // ADD_CASHITEM_4RD_MARK_20080723

#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC	
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205	// PC방 아이템 획득 확률 증가
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
	int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
	if( iMaxHitUser >= 0 )
	{
		int iItemDropRate = gObjGetTotalValueOfEffect( &gObj[iMaxHitUser], EFFECTTYPE_ITEMDROPRATE );
		
		if( iItemDropRate > 0 )
		{
			ItemDropPer = ItemDropPer * iItemDropRate / 100;
			ExtDropPer = rand() % ( 2000 / iItemDropRate * 100 );
		}		
	}
#ifndef MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#endif
#endif // MODIFY_BUFF_EFFECT_SYSTEM_CHECKLOGIC
#else

#ifdef ADD_PCS_MARK_OF_EXP_20070205
	ItemDropPer = (int)(ItemDropPer * ((float) lpTargetObj->m_wItemDropRate / 100 ));
#endif // ADD_PCS_MARK_OF_EXP_20070205
	
#ifdef PCBANG_POINT_SYSTEM_20070206		// 아이템 획득 확률 증가
	// PC방 포인트 아이템 획득확률이 적용된 유저 처리
	int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
#ifdef MODIFY_MAXHITUSER_BUG_FIX_20070711
	if(iMaxHitUser >= 0)
	{
		if( gObj[iMaxHitUser].m_iPCBangPointItemTime > 0 && gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer > 0 )

		{
			if( ItemDropPer > 0 )
			{
				ItemDropPer = ItemDropPer * gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer / 100;
				ExtDropPer = rand() % ( 2000 / gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer * 100 );
			}
		}
	}
#else	// MODIFY_MAXHITUSER_BUG_FIX_20070711
	if( gObj[iMaxHitUser].m_iPCBangPointItemTime > 0 && gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer > 0 )

	{
		if( ItemDropPer > 0 )
		{
			ItemDropPer = ItemDropPer * gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer / 100;
			ExtDropPer = rand() % ( 2000 / gObj[iMaxHitUser].m_iPCBangPointItemGiveItemPer * 100 );
		}
	}
#endif	// MODIFY_MAXHITUSER_BUG_FIX_20070711
#endif	

#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	// 1/100 로 엑설런트 아이템을 떨어트린다.
	if( ExtDropPer == 1 )
	{
		
#ifdef UPGRADE_GAMESERVER_01_20041006
		DropItem = g_MonsterItemMng.GetItem(lpObj->Level-25);

#ifdef MODIFY_380_EXCELENT_ITEM_DISABLE_20080212	// 380 아이템 엑 안떨어지게
		if( DropItem != NULL && g_kItemSystemFor380.Is380Item( DropItem ))
		{
			DropItem = NULL;
		}
#endif	// MODIFY_380_EXCELENT_ITEM_DISABLE_20080212
		
		if( DropItem == NULL )
		{
			item_drop = 0;
		}
		else
		{
			BOOL foundChangeupitem = FALSE;
			for( int i = 0; i < 4; i++ )
			{	// 전직용 아이템들은 엑설런트 아이템이 떨어질 확률을 더 낮게 한다
				if( DropItem->m_RequireClass[i] > 1 )
				{
					foundChangeupitem = TRUE;
					break;
				}		
			}
			
			if( foundChangeupitem )
			{
				ExtDropPer = rand()%100;			
			}		
			
			if( ExtDropPer )
			{
				if( (rand()%itemrate) < ItemDropPer )	
				{
					item_drop = 1;
				}
			}
		}
#else // UPGRADE_GAMESERVER_01_20041006
		
		// 몬스터 창고에서 엑설런트 아이템을 꺼낸다.
		store_count = lpObj->WarehouseCount;
		if( CHECK_LIMIT_NONZERO( store_count, MAX_WAREHOUSEITEMS) == FALSE) 
		{			
			return;
		}
		DropItemNum = rand()%store_count;
		DropItem = &lpObj->pWarehouse[DropItemNum];

#ifdef NEW_SKILL_FORSKYLAND		
		BOOL	foundChangeupitem = FALSE;
		for( int i = 0; i < 4; i++ )
		{	// 전직용 아이템들은 엑설런트 아이템이 떨어질 확률을 더 낮게 한다
			if( DropItem->m_RequireClass[i] > 1 )
			{
				foundChangeupitem = TRUE;
				break;
			}		
		}

		if( foundChangeupitem )
		{
			ExtDropPer = rand()%100;			
		}		

		if( ExtDropPer )
		{
			if( (rand()%itemrate) < ItemDropPer )
			{
				item_drop = 1;
			}
		}
#else // NEW_SKILL_FORSKYLAND
		if( (rand()%itemrate) < ItemDropPer )
		{
			item_drop = 1;
		}
#endif // NEW_SKILL_FORSKYLAND
#endif // UPGRADE_GAMESERVER_01_20041006


	}
	else
	{
		if( (rand()%itemrate) < ItemDropPer )
		{			
			DropItem = g_MonsterItemMng.GetItem(lpObj->Level);
			if( DropItem == NULL )
			{
				item_drop = 0;
			}
			else
			{
				item_drop = 1;
			}
		}
	}

	// 아이템이 없으면 아이템 드롭 없다.
	if( item_drop )
	{
		if( DropItem->IsItem()==FALSE ) item_drop = 0;

		if( (DropItem->m_Type >= ITEM_ETC) && (DropItem->m_Type <= ITEM_ETC + MAX_ITEM_INDEX)) {
			INT I = 0;
		}
				
#ifdef NEW_FORSKYLAND2
		// 데빌스퀘어 드롭 아이템 제한(극히 드물게)
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
		if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
		if( lpObj->MapNumber == MAP_INDEX_DEVILSQUARE )
#endif
		{	
			if( !IsCanNotItemDtopInDevilSquare(DropItem->m_Type) )
			{
				if( rand()%10 != 0 )
				{
					item_drop = 0;
				}
			}
		}
#endif
		
#ifdef MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825
	// 소켓 아이템은 라클리온 맵에서만 드롭 되도록 수정
		if( g_SocketOptionSystem.IsEnableSocketItem( DropItem->m_Type ) == true )
		{
			if( g_SocketOptionSystem.IsEnableDropSocketItemMap( lpObj->MapNumber ) == false )
			{
				item_drop = 0;
			}
		}
#endif // MODIFY_LIMIT_SOCKET_ITEM_DROP_MAP_RAKLION_20080825

#ifdef FEATHEROFLOKE_ITEMDROP_CHECK
		if( g_GlobalConfig.m_bFeatherOnlyIcarus == TRUE && DropItem->m_Type == MAKE_ITEMNUM(13,14) && lpObj->MapNumber != MAP_INDEX_ICARUS )
		{	// 로크의 깃털이 이카루스가 아닌곳이면 떨어지지 않는다
			item_drop = 0;
		}
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
		// MVP 실패 : 보석 드랍률 하락.
		if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
			&& g_iCrywolfApplyMvpPenalty
		  )
		{
			if(	DropItem->m_Type == MAKE_ITEMNUM(14, 13)	// 축석
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 14)	// 영석
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 16)	// 생석
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 22)	// 창석
				|| DropItem->m_Type == MAKE_ITEMNUM(12, 15)	// 혼석
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 31)	// 수호 보석) 
			  )
			{
				if( rand()%100 > g_CrywolfSync.GetGemDropPenaltiyRate() )
				{
					item_drop = 0;
				}
			}

		}
#endif

	}
	
//#if (TESTSERVER==0)
	if( item_drop )
//#endif
	{
		type = DropItem->m_Type;
		level= DropItem->m_Level;

#ifdef MODIFY_EXCELLENT_ITEM_DUR_01_20041202
		if( ExtDropPer == 1 )
	#ifdef MODIFY_EXCELLENT_DROP_DURATION_BUG_20060524
			// 엑설런트 아이템인 경우 아이템의 레벨을 0으로 한다.
			// DropItem->m_Level값으로 dur을 구하기 때문에 실제 생성시 값 차이가 난다.
			dur  = ItemGetDurability( DropItem->m_Type, 0, TRUE, FALSE );
	#else
			dur  = ItemGetDurability( DropItem->m_Type, DropItem->m_Level, TRUE, FALSE );
	#endif // MODIFY_EXCELLENT_DROP_DURATION_BUG_20060524
		else
			dur  = DropItem->m_Durability;
#else
		dur  = DropItem->m_Durability;
#endif

		x    = lpObj->X;
		y    = lpObj->Y;
				
		
		int option1rand, option2rand, option3rand, optionc;
		

		// 엑설런트 아이템의 경우 스킬은 무조건 붙으며 기존 옵션들은 정말 희박함
		if( ExtDropPer == 1 )
		{
			option1rand = 100;
			option2rand = 1;
			option3rand = (rand()%100);
			optionc		= (rand()%3);
			NOption		= NewOptionRand(lpObj->Level);
			level       = 0;
		}
		else
		{
			option1rand = 6;				// 일반적인 옵션1 (스킬) 붙을 확률 6/100
			option2rand = 4;				// 일반적인 옵션2 (행운) 붙을 확률 4/100
			option3rand = (rand()%100);		// 일반적인 옵션3 (추가포인트) 붙을 확률 랜덤/100
			optionc		= (rand()%3);
			NOption     = 0;
		}

		// 스킬아이템 확률을 대폭 낮춘다.
		if((rand()%100) < option1rand ) Option1 = 1;
		if((rand()%100) < option2rand ) Option2 = 1;

		switch( optionc )
		{
		case 0 :
			if( option3rand < 4 ) Option3 = 3;	// 옵션
			break;
		case 1 :
			if( option3rand < 8 ) Option3 = 2;	// 옵션
			break;
		case 2 :
			if( option3rand < 12 ) Option3 = 1;	// 옵션
			break;
		}

//#if (TESTSERVER==0)
		if( lpObj->Class == 43  )
//#endif
		{
			Option1 = DropItem->m_Option1;
			Option2 = DropItem->m_Option2;
			Option3 = DropItem->m_Option3;
		}
		
		// 영석/축석/혼석 에는 옵션이나 스킬들이 안붙게..
		if( type == MAKE_ITEMNUM(12,15) ||
			type == MAKE_ITEMNUM(14,13) ||
			type == MAKE_ITEMNUM(14,14) ||
#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
			type == MAKE_ITEMNUM(14,31) ||
#endif
			type == MAKE_ITEMNUM(14,16) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			NOption = 0;
		}
		
		
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

		if( DropItem->m_serial )
		{
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser, NOption );
		}
		else 
		{
			MapC[lpObj->MapNumber].MonsterItemDrop(type, level, dur, x, y, Option1, Option2, Option3, NOption, 0, MaxHitUser);			
		}

		cexDrop.DropItem(lpObj,&gObj[MaxHitUser]);
	}
//#if (TESTSERVER==0)
	else 
	{
		if( lpObj->Money < 1 ) 
		{			
			return;
		}
		
		if( (rand()%moneyrate) < 10 )
		{
			// 돈을 떨어트린다.			
			int x, y;
			float money;
			
			x    = lpObj->X;
			y    = lpObj->Y;
			
			money = (float)lpObj->Money;
			
#ifdef UPDATE_ZEN_DROP_DOWN_20080806	// 몬스터 죽였을 시 드롭젠 (경험치 / 2)
#ifndef FOR_NEW_TESTSERVER			
			money /= 2;
#endif
			// 몬스터 죽였을때 얻는돈
			money += money/100*lpTargetObj->MonsterDieGetMoney;
#else	// UPDATE_ZEN_DROP_DOWN_20080806
			// 몬스터 죽였을때 얻는돈
			money += money/100*lpTargetObj->MonsterDieGetMoney;

			money += 7;
#endif	// UPDATE_ZEN_DROP_DOWN_20080806

#ifdef FOR_NEW_TESTSERVER			
//			money *= 3;		// 젠 드롭 3배
#endif
			
#ifdef ADD_ZEN_OPTION_20070903	// 추가 젠
			money *= gAddZen;
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// 패시브 1차 - 젠 증가
			if( lpTargetObj->m_MPSkillOpt.iMpsPlusZen > 0 )
			{
				// 드롭젠 (경험치 / 2) 젠증가 스킬에 적용 (20081107 - grooving)
				money += ( money * lpTargetObj->m_MPSkillOpt.iMpsPlusZen ) / 100;
			}
#endif
			
#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
			if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
			if( lpObj->MapNumber == 9 )
#endif
			{	// 악마의 광장에서는 돈을 누적한다
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
#ifdef MODIFY_MAXHITUSER_BUG_FIX_20070711
				if(MaxHitUser == -1)
					return;
#endif
				gObj[MaxHitUser].m_nEventMoney += (int)money;				
			}
			else
			{
				MapC[lpObj->MapNumber].MoneyItemDrop((int)money, x ,y);
			}
		}		
	}
//#endif
	if( lpObj->Money < 1 ) 
	{		
		return;
	}

	if( lpObj->m_stateEngagePVP == PVP_USER_PLAYING )
		return;

#ifdef DEVILSQUARE_EXTEND_20050221		// 데빌스퀘어 맵번호 체크 수정
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
	if( lpObj->MapNumber == 9 )
#endif
		return;
		
	// 400분의 1로 돈을 4개 뭉태기로 떨어트린다.
	if( (rand()%400) == 1 )
	{		
		for( n=0; n<4; n++)
		{
			int x, y;
			x   = lpObj->X-2;
			y   = lpObj->Y-2;
			x   += rand()%3;
			y   += rand()%3;
			MapC[lpObj->MapNumber].MoneyItemDrop(lpObj->Money, x ,y);
		}
	}
	if( gEvent1ItemDropTodayPercent > 0 )
	{
		if( (rand()%gEvent1ItemDropTodayPercent) == 0 )
		{
			if( gEvent1ItemDropTodayCount < gEvent1ItemDropTodayMax ) 
			{
				gEvent1ItemDropTodayCount++;
				type = ItemGetNumberMake(14, 12);
				
				// 러브팡팡 아이템 
				dur		= 0;
				x		= lpObj->X;
				y		= lpObj->Y;
				level	= 1;
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, 0, 0, 0);
#ifdef REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
				/*
				 *	러브러브 이벤트와 인증서버와의 커플링 제거
				 *		< commonserver.cfg >
				 *		EVENT1						= 0	; 0 : 러브러브 이벤트 안함 1:  러브러브 이벤트 함
				 *		Event1ItemDropTodayMax		= 0	; 오늘 이벤트로 떨어질 아이템 수
				 * 		Event1ItemDropTodayPercent	= 0
				 *
				 */
#else
				LoveHeartCreateSend();
#endif
			}
		}
	}
}

BOOL gEventMonsterItemDrop(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj)
{
	
	int type, level, x, y;
	float dur=0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;


#ifdef ADD_ITEM_FENRIR_01_20051110
	// 펜릴 재료 아이템 드랍
	if( g_bFenrirStuffItemDrop )
	{
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;

		BOOL bDropStuff = FALSE;

		// 1. > 갑옷 파편 드랍
		if( lpObj->Level >= g_iFenrirStuff_01_DropLv_Min 
			&& lpObj->Level <= g_iFenrirStuff_01_DropLv_Max 
			&& lpObj->MapNumber == g_iFenrirStuff_01_DropMap 
			&& !bDropStuff )
		{
			if( (rand()%10000) < g_iFenrirStuff_01_DropRate )
			{
				bDropStuff	= TRUE;
				type		= ItemGetNumberMake(13,32);
				
			}
		}

		// 2. > 여신의 가호 드랍
		if( lpObj->Level >= g_iFenrirStuff_02_DropLv_Min 
			&& lpObj->Level <= g_iFenrirStuff_02_DropLv_Max
			&& lpObj->MapNumber == g_iFenrirStuff_02_DropMap
			&& !bDropStuff )
		{
			if( (rand()%10000) < g_iFenrirStuff_02_DropRate )
			{
				bDropStuff	= TRUE;
				type		= ItemGetNumberMake(13,33);
			}
		}

		// 3. > 맹수의 발톱 드랍
		if( lpObj->Level >= g_iFenrirStuff_03_DropLv_Min 
			&& lpObj->Level <= g_iFenrirStuff_03_DropLv_Max
			&& lpObj->MapNumber == g_iFenrirStuff_03_DropMap
			&& !bDropStuff )
		{
			if( (rand()%10000) < g_iFenrirStuff_03_DropRate )
			{
				bDropStuff	= TRUE;
				type		= ItemGetNumberMake(13,34);
			}
		}

		if( bDropStuff )
		{
			level   = 0;
			x		= lpObj->X;
			y		= lpObj->Y;
			dur		= 1;
			
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
		
	}
#endif // ADD_ITEM_FENRIR_01_20051110
	
#ifdef MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	int iSphereItemCode = 0;
	iSphereItemCode = g_SocketOptionSystem.GetSphereDropInfo( lpObj->Level );

	if( iSphereItemCode != -1 )
	{
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, iSphereItemCode, 0, 1, 0, 0, 0, iMaxHitUser);
		return TRUE;
	}
#endif // MODIFY_SET_SEED_SPHERE_DROP_RATE_20080903
	
#ifdef ADD_SHIELD_POINT_01_20060403
	if( g_CompoundPotionDropOn == TRUE )
	{
		BOOL bDropStuff = FALSE;
		int iType		= 0;

		if( lpObj->Level >= g_iCompoundPotionLv1DropLevel )
		{
			if( ( (rand()%10000) <= g_iCompoundPotionLv1DropRate )
				&& ( bDropStuff == FALSE )
				)
			{
				bDropStuff	= TRUE;
				iType		=ItemGetNumberMake(14,38);		// 작은 복합 물약
			}
		}
		
		if ( bDropStuff == FALSE && lpObj->Level >= g_iCompoundPotionLv2DropLevel )
		{
			if( (rand()%10000) <= g_iCompoundPotionLv2DropRate )
			{
				bDropStuff = TRUE;
				iType		=ItemGetNumberMake(14,39);		// 중간 복합 물약
			}
		}

		if ( bDropStuff == FALSE && lpObj->Level >= g_iCompoundPotionLv3DropLevel )
		{
			if( (rand()%10000) <= g_iCompoundPotionLv3DropRate )
			{
				bDropStuff = TRUE;
				iType		=ItemGetNumberMake(14,40);		// 큰 복합 물약
			}
		}

		if( bDropStuff == TRUE )
		{
			int level		= 0;
			int dur			= 1;
			int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)lpObj->X, (BYTE)lpObj->Y, iType, level, (BYTE)dur, 0, 0, 0, iMaxHitUser);
			return TRUE;
		}
	}
#endif // ADD_SHIELD_POINT_01_20060403

#ifdef ITEMDROP_RINGOFTRANSFORM
	if( gIsItemDropRingOfTransform )
	{	
		// 변신 반지 드롭
		if( (rand()%10000) < gItemDropRingOfTransform )
		{
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
			if( lpattr )
			{
				int level = lpattr->m_Level;												
				int callbead_level=0;
				if( level < 0 ) level = 0;
				callbead_level = level/10;
				if( callbead_level > 0 ) callbead_level -= 1;
				if( callbead_level > 5 ) callbead_level = 5;				

				dur  = (float)100+(rand()%100);	// 내구도 설정
				x    = lpObj->X;
				y    = lpObj->Y;
				level= callbead_level;			// 변신 반지 레벨
				type = ItemGetNumberMake(13, 10);			
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;				
			}	
		}
	}
#endif


	if( gFireCrackerEvent )
	{	
		// 폭죽 이벤트가 진행줄일때
		if( gLanguage == 0 )
		{	
			// 한국일때는 다르게 처리 된다

			if( lpObj->MapNumber < 7 )
			{	
				// 아틀란스 이하에서만 나온다
				LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
				if( lpattr )
				{
					if( lpattr->m_Level >= 17 )
					{	
						// 자이언트 이상일때만
						if( (rand()%10000) < gFireCrackerDropRate )
						{
							dur  = 255;
							x    = lpObj->X;
							y    = lpObj->Y;
							level= 2;	// 폭죽
							type = ItemGetNumberMake(14, 11);			
							int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
							ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
							return TRUE;
						}
					}
				}
			}
		}
		else
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			if( MaxHitUser != -1 )
			{
				int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);

				if( partycount > 0 )
				{	// 파티를 맺었을때만 
					LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);	

					if( lpattr )
					{
						if( lpattr->m_Level >= 17 )
						{	// 자이언트 이상일때만
							if( (rand()%10000) < gFireCrackerDropRate )
							{
								dur  = 255;
								x    = lpObj->X;
								y    = lpObj->Y;
								level= 2;	// 폭죽
								type = ItemGetNumberMake(14, 11);			
								ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
								return TRUE;
							}
						}
					}
				}
			}
		}		
	}
	
	
	if( gXMasEvent )
	{	
		// 크리스마스 이벤트 일때만
#ifdef EVENT_ITEMBAG_UPGRADE		
		if( !StarOfXMasItemBag )
			return FALSE;
		
		if( StarOfXMasItemBag->DropEventItem(lpObj->m_Index) )
		{
			return TRUE;
		}
#else	
	#ifdef XMAS_EVENT_CHANGE		
			if( lpObj->MapNumber == MAP_INDEX_DEVIAS )
			{	// 데비아스에서만
				if( (rand()%10000) < g_XMasEvent_StarOfXMasDropRate )
				{
					dur  = 255;
					x    = lpObj->X;
					y    = lpObj->Y;
					level= 1;
					type = ItemGetNumberMake(14, 11);

					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
	#else	
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);	

			if( lpattr )	
			{
				if( lpattr->m_Level >= 17 )
				{	// 자이언트 이상일때만
					if( (rand()%10000) < g_XMasEvent_StarOfXMasDropRate )
					{
						dur  = 255;
						x    = lpObj->X;
						y    = lpObj->Y;
						level= 1;
						type = ItemGetNumberMake(14, 11);

						int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
						return TRUE;
					}
				}
			}
	#endif
#endif
	}

	if( gHeartOfLoveEvent )
	{	
		// 폭죽 이벤트가 진행줄일때		
		if( gLanguage == 2 )
		{
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
			if( lpattr )
			{
				if( lpattr->m_Level >= 15 )
				{	
					// 
					if( (rand()%10000) < gHeartOfLoveDropRate )
					{
						dur  = 255;
						x    = lpObj->X;
						y    = lpObj->Y;
						level= 3;	// 사랑의 하트
						type = ItemGetNumberMake(14, 11);			
						int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
						return TRUE;
					}
				}
			}
		}
		else
		{
			if( lpObj->MapNumber < 7 )
			{	
				// 아틀란스 이하에서만 나온다
				LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
				if( lpattr )
				{
					if( lpattr->m_Level >= 17 )
					{	
						// 자이언트 이상일때만
						if( (rand()%10000) < gHeartOfLoveDropRate )
						{
							dur  = 255;
							x    = lpObj->X;
							y    = lpObj->Y;
							level= 3;	// 사랑의 하트
							type = ItemGetNumberMake(14, 11);			
							int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
							ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
							return TRUE;
						}
					}
				}
			}
		}		
	}


	if( gMedalEvent )
	{	
		// 메달 이벤트가 진행중일때
		if( lpObj->MapNumber == 1 || lpObj->MapNumber == 2 )
		{	// 데비아스 및 던전에서는 은메달이 나와야 된다			
			if( (rand()%10000) < gSilverMedalDropRate )
			{
				dur  = 255;
				x    = lpObj->X;
				y    = lpObj->Y;
				level= 5;	// 은메달
				type = ItemGetNumberMake(14, 11);			
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}			
		}
		else if( lpObj->MapNumber == 4 || lpObj->MapNumber == 7 || lpObj->MapNumber == 8 )
		{	// 로스트타워, 아틀란스, 타르칸에서는 금메달이 나와야 된다
			if( (rand()%10000) < gGoldMedalDropRate )
			{
				dur  = 255;
				x    = lpObj->X;
				y    = lpObj->Y;
				level= 6;	// 금메달
				type = ItemGetNumberMake(14, 11);			
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}			
		}
	}	

	if( gEventChipEvent )
	{	// 이벤트칩 이벤트가 진행중일때				
		if( (rand()%10000) < gBoxOfGoldDropRate )
		{
			dur  = 255;
			x    = lpObj->X;
			y    = lpObj->Y;
			level= 7;	// 황금의 상자
			type = ItemGetNumberMake(14, 11);			
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}		
	}

#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[16] != AUTH_CHECK_16 )
		DestroyGIocp();
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518	
	// 쿤둔의 표식 드롭처리
	if (rand()%10000 < g_iKundunMarkDropRate) 
	{
		
#ifndef HIDDEN_KALIMA_20050706
	// 쿤둔의 표식 드랍지역 제한 삭제 - hnine
	#ifndef UPDATE_LOWLEVEL_SUPPORT_1_20051215
		if (CHECK_KALIMAMAP(lpObj->MapNumber)) 
		{
			// 칼리마 내부에서는 쿤둔의 표식이 떨어지지 않는다.
			return FALSE;
		}
	#endif
#endif

		Option1 = 0;
		Option2 = 0;
		Option3 = 0;
		dur		= 1;
		x		= lpObj->X;
		y		= lpObj->Y;

		// ### 20040906 (apple) 
		level = 0;

#ifdef MODIFY_KALIMA_KUNDUNMARK_DROP_01_20040906
		if( lpObj->Level < 25 )
		{
			level = 0;
		}
		else if( lpObj->Level < 47)								// +1 (25~ 46)
		{
			level = 1;
		}
		else if( lpObj->Level < 66)								// +2 (47~ 65)
		{
			level = 2;
		}
		else if( lpObj->Level < 78)								// +3 (66~ 79)
		{
			level = 3;
		}
		else if( lpObj->Level < 84)								// +4 (80~ 85)
		{
			level = 4;
		}
		else if( lpObj->Level < 92)								// +5 (86~ 95)
		{
			level = 5;
		}
#ifdef HIDDEN_KALIMA_20050706
		else if( lpObj->Level < 114)							// +6 (92~ 114)
		{
			level = 6;
		}
	#ifdef UPDATE_LOWLEVEL_SUPPORT_1_20051215
		// 쿤둔의 표식 +7의 드랍 레벨과 지역을 수정 : 레벨 115이상, 전지역 - hnine
		else if( lpObj->Level > 115 && lpObj->Level < g_GlobalConfig.m_iMaxLevel+1 )		// +7 (115~ )
		{
			level = 7;
		}
	#else
		else													// +7 (115~ )
		{
			if (CHECK_KALIMAMAP(lpObj->MapNumber))
				level = 7;
			else
				level = 0;
		}
	#endif
#else
		else													// +6 (96~ )
		{
			level = 6;
		}
#endif

#ifdef HIDDEN_KALIMA_20050706
	#ifndef UPDATE_LOWLEVEL_SUPPORT_1_20051215
		if (level != 7 &&
			CHECK_KALIMAMAP(lpObj->MapNumber)
			) 
		{
			return FALSE;
		}
	#endif
#endif

		if (level == 0)
			return FALSE;
#endif
		
		type = ItemGetNumberMake(14, 29);						// 쿤둔의 표식
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return TRUE;
	}
#endif
	

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214							// 성주의 표식 드롭처리
	if (rand()%10000 < g_iMarkOfTheLord) {
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;
		level	= 3;
		dur		= 1;
		x		= lpObj->X;
		y		= lpObj->Y;

		type = ItemGetNumberMake(14, 21);						// 성주의 표식 (레나 +3)
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return TRUE;
	}
#endif


#ifdef JAPAN_1ST_ANIVERSARY_BOX_20040531
	if (rand()%10000 < g_iJapan1StAnivItemDropRate) {
		
		if( lpTargetObj->PartyNumber >= 0 )
		{
			dur  = 255;
			x    = lpObj->X;
			y    = lpObj->Y;
			type = ItemGetNumberMake(14, 20);			

			// 일본 1주년 기념 폭죽상자는 맵에 따라 다른 레벨이 떨어진다.
			switch(lpObj->MapNumber) {
			case 0 :		// 로랜시아		(+1)
				{
					level= 1;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 1 :		// 던전			(+2)
				{
					level= 2;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 2 :		// 데비아스		(+2)
				{
					level= 2;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 3 :		// 노리아		(+1)
				{
					level= 1;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 4 :		// 로스트타워	(+3)
				{
					level= 3;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 7 :		// 아틀란스		(+4)
				{
					level= 4;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 8 :		// 타르칸		(+5)
				{
					level= 5;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			default:
				break;
			}
		}
	}
#endif

#ifdef DARKLORD_WORK
	if( gIsDropDarkLordItem )
	{
		// 군주의 소매 드롭 
		if( lpObj->Level >= gSleeveOfLordDropLevel )
		{
			if( rand()%10000 < gSleeveOfLordDropRate && (g_GlobalConfig.m_bMonarchOnlyIcarus == false || lpObj->MapNumber == MAP_INDEX_ICARUS) )
			{	
				type = ItemGetNumberMake(13, 14);
				level = 1;
				x    = lpObj->X;
				y    = lpObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}
		}

		// 다크호스의 영혼 드롭
		if( lpObj->Level >= gSoulOfDarkHorseropLevel )
		{
			if( rand()%10000 < gSoulOfDarkHorseDropRate && (g_GlobalConfig.m_bHorsePotOnlyIcarus == false || lpObj->MapNumber == MAP_INDEX_ICARUS) )
			{	
				type = ItemGetNumberMake(13, 31);
				level = 0;
				x    = lpObj->X;
				y    = lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}
		}

		// 다크스피릿의 영혼 드롭
		if( lpObj->Level >= gSoulOfDarkSpiritDropLevel )
		{
			if( rand()%10000 < gSoulOfDarkSpiritDropRate && (g_GlobalConfig.m_bSpiritPotOnlyIcarus == false || lpObj->MapNumber == MAP_INDEX_ICARUS) )
			{	
				type = ItemGetNumberMake(13, 31);
				level = 1;
				x    = lpObj->X;
				y    = lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}
		}
	}

#endif

#ifdef ADD_NEW_GEM_OF_DEFEND_FOR_CS_01_20050301
	// 수호보석 드랍.
	if( gIsDropGemOfDefend )
	{
		if( ( lpObj->MapNumber != MAP_INDEX_KALIMA7 ) && ( lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		{	// 수호의 보석은 칼리마7 과 성 사냥터에서만 떨어진다.
			
		}
		else
		{
			if( lpObj->Level >= gGemOfDefendDropLevel )
			{
				if( rand()%10000 < gGemOfDefendDropRate )
				{	
					type = ItemGetNumberMake(14, 31);
					level = 0;
					x    = lpObj->X;
					y    = lpObj->Y;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
		}
	}
#endif
	
#ifdef KANTURU_SPECIAL_ITEMDROP_20060808
if( g_bKanturuSpecialItemDropOn )
{
	// 문스톤펜던트
	if( lpObj->MapNumber == MAP_INDEX_KANTURU2 )
	{
		if( rand()%10000 < g_iKanturuMoonStoneDropRate )
		{	
			type = ItemGetNumberMake(13, 38);
			level = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
	}

	// 조화의 보석
	if( lpObj->MapNumber == MAP_INDEX_KANTURU1 
		|| lpObj->MapNumber == MAP_INDEX_KANTURU2 
		|| lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS 
	  )
	{
		if( rand()%10000 < g_iKanturuJewelOfHarmonyDropRate )
		{	
			type = ItemGetNumberMake(14, 41);
			level = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
	}

}


#endif // KANTURU_SPECIAL_ITEMDROP_20060808

#ifdef ADD_THIRD_WING_20070525	// 콘도르의불꽃 드롭 처리
	if( lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_QUEST )
	{
		if( rand() % 10000 < g_iCondorFlameDropRate )
		{	
			type = ItemGetNumberMake( 13, 52 );
			level = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser );
			return TRUE;
		}
	}
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_02_20050314
	if( gIsDropSetItemInCastleHuntZone )
	{
		// 시련의 땅에서만 받을 수 있다.
		if( lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE )
		{	
			
		}
		else
		{
			if( lpObj->Level >= gSetItemInCastleHuntZoneDropLevel )
			{
				if( rand()%10000 < gSetItemInCastleHuntZoneDropRate )
				{
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

					if( CHECK_LIMIT(MaxHitUser, MAX_OBJECT) )
					{
						LogAddTD("[Castle HuntZone] Drop SetItem [%s][%s] ", gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name);
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216 
						MakeRewardSetItem( MaxHitUser, lpObj->X, lpObj->Y, 0, lpObj->MapNumber );
#else
						MakeRewardSetItem( MaxHitUser, lpObj->X, lpObj->Y, 0 );
#endif // #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
						return TRUE;
					}

				}
			}
		}
	}
#endif


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// 신비의 구슬 드롭처리
	if( lpObj->MapNumber == 33 ||		// 아이다
		lpObj->MapNumber == 34			// 크라이울프 1차맵
		)
	{
		bool bDropMysteriousBead = false;
		switch(lpObj->Class) 
		{
		case 304:	// 위치퀸
		case 305:	// 블루골렘
		case 306:	// 데스라이더
		case 307:	// 포레스트오크
		case 308:	// 데스트리
		case 309:	// 헬마이네
			{
				if (rand()%10000 < g_iMysteriousBeadDropRate1) 
				{
					bDropMysteriousBead = true;	
				}
			}
			break;
		case 310:	// 척후병(근거리)
		case 311:	// 척후병(중거리)
		case 312:	// 척후병(원거리)
		case 313:	// 웨어울프
			{
				if (rand()%10000 < g_iMysteriousBeadDropRate2) 
				{
					bDropMysteriousBead = true;	
				}
			}
			break;
		}

		if (bDropMysteriousBead == true) 
		{
			type = ItemGetNumberMake(12, 26);
			level = 0;
			x    = lpObj->X;
			y    = lpObj->Y;
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
	}
#endif

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 여기서 마스터레벨 이벤트맵 이장아이템 재료를 드랍
	// 광장 재료
	if( gDevilSquareEvent )
	{
		if (!CHECK_DEVILSQUARE(lpObj->MapNumber)) 
		{
			// 7 악마의눈
			if (rand()%10000 < gML_EyesOfDevilSquareDropRate) 
			{
				if(lpObj->Level >= 90)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 128;
					level	= 7;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(14, 17);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}

			// 7 악마의열쇠
			if (rand()%10000 < gML_KeyOfDevilSquareDropRate) 
			{
				if(lpObj->Level >= 90)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 128;
					level	= 7;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(14, 18);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
		}
	}

	// 사원 재료
	if(g_bIllusionTampleEvent)
	{
		if (!CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) )
		{
			// 6 낡은 두루마리
			if (rand()%10000 < gML_OldScrollDropRate) 
			{
				if(lpObj->Level >= 96)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 0;
					level	= 6;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(13, 49);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}

			// 6 환영교단의 서약
			if (rand()%10000 < gML_CovenantOfIllusionDropRate) 
			{
				if(lpObj->Level >= 96)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 0;
					level	= 6;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(13, 50);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
		}
	}

	// 블캐 재료
	if (g_bBloodCastle) 
	{
		if (!CHECK_BLOODCASTLE(lpObj->MapNumber)) 
		{
			// 8 대천사의 서
			if (rand()%10000 < gML_AngelKingsPaperDropRate) 
			{
				if(lpObj->Level >= 90)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 128;
					level	= 8;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(13, 16);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}

			// 8 블러드 본
			if (rand()%10000 < gML_BloodBoneDropRate) 
			{
				if(lpObj->Level >= 90)
				{
					Option1 = 0;
					Option2 = 0;
					Option3 = 0;
					dur		= 128;
					level	= 8;
					x		= lpObj->X;
					y		= lpObj->Y;
					
					type = ItemGetNumberMake(13, 17);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
		}
	}
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123

#ifdef FOR_BLOODCASTLE
	if (g_bBloodCastle) 
	{
		if (!CHECK_BLOODCASTLE(lpObj->MapNumber)) 
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;

			// 1 . 스톤이 떨어질 확률
/*			if( g_bStoneItemDrop )
			{			
				if (rand()%10000 < g_iStoneDropRate) {
#ifdef FOR_BLOODCASTLE_TEST
					dur  = 0;
#else
					dur  = 128;
#endif
					x    = lpObj->X;
					y    = lpObj->Y;
					level= 1;	
					type = ItemGetNumberMake(14, 21);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
			}
*/
			// 2 . 대천사의 서가 떨어질 확률
			if (rand()%10000 < g_iAngelKingsPaperDropRate) 
			{
#ifdef FOR_BLOODCASTLE_TEST
				dur  = 0;
#else
				dur  = 128;
#endif
				x    = lpObj->X;
				y    = lpObj->Y;

				
				if( lpObj->Level < 32)			// +1 ( 0~ 31)
				{
					level = 1;
				}
				else if( lpObj->Level < 45)		// +2 (31~ 44)
				{
					level = 2;
				}
				else if( lpObj->Level < 57)		// +3 (45~ 56)
				{
					level = 3;
				}
				else if( lpObj->Level < 68)		// +4 (57~ 67)
				{
					level = 4;
				}
				else if( lpObj->Level < 76)		// +5 (68~ 75)
				{
					level = 5;
				}
				else if( lpObj->Level < 84)		// +6 (64~ 83)
				{
					level = 6;
				}
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 대천사의서 +8 드롭 확률
				else if( lpObj->Level < 90 )	// +7 (75~ 89)
				{
					level = 7;
				}
				else							// +8 (90~ )
				{
					level = 8;
				}
#else
				else							// +7 (75~ )
				{
					level = 7;
				}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 마스터레벨 이벤트맵 재료면 처리안함
				if(level != 8)
				{
					type = ItemGetNumberMake(13, 16);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
#else	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
				type = ItemGetNumberMake(13, 16);
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
			}
					
			// 3 . 블러드본이 떨어질 확률
			if (rand()%10000 < g_iBloodBoneDropRate) 
			{
#ifdef FOR_BLOODCASTLE_TEST
				dur  = 0;
#else
				dur  = 128;
#endif
				x    = lpObj->X;
				y    = lpObj->Y;

				if( lpObj->Level < 32)			// +1 ( 0~ 31)
				{
					level = 1;
				}
				else if( lpObj->Level < 45)		// +2 (31~ 44)
				{
					level = 2;
				}
				else if( lpObj->Level < 57)		// +3 (45~ 56)
				{
					level = 3;
				}
				else if( lpObj->Level < 68)		// +4 (57~ 67)
				{
					level = 4;
				}
				else if( lpObj->Level < 76)		// +5 (68~ 75)
				{
					level = 5;
				}
				else if( lpObj->Level < 84)		// +6 (64~ 83)
				{
					level = 6;
				}
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// 블러드본 +8 드롭 확률
				else if( lpObj->Level < 90 )	// +7 (75~ 89)
				{
					level = 7;
				}
				else							// +8 (90~ )
				{
					level = 8;
				}
#else
				else							// +7 (75~ )
				{
					level = 7;
				}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
				
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 마스터레벨 이벤트맵 재료면 처리안함
				if(level != 8)
				{
					type = ItemGetNumberMake(13, 17);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
#else	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
				type = ItemGetNumberMake(13, 17);
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
			}
		}
	}
#endif
	

#ifdef DARK_LORD_HEART_EVENT_20040810			// 다크로드의 마음 이벤트 아이템 드롭
	if (rand()%10000 < g_iDarkLordHeartDropRate) 
	{
		int iDropRate = 0;

		// 세 맵에서만 떨어진다.
		if (lpTargetObj->MapNumber == MAP_INDEX_RORENCIA ||
			lpTargetObj->MapNumber == MAP_INDEX_NORIA ||
			lpTargetObj->MapNumber == MAP_INDEX_DEVIAS
			)
		{
			type	= ItemGetNumberMake(14, 11);
			level	= 13;
			x		= lpObj->X;
			y		= lpObj->Y;
			dur		= 0;
			
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
	}	
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908
	if( ChnThanksCookieEvent->DropEventItem(lpObj->m_Index) )
	{
		return TRUE;
	}
#endif
	
#ifdef EVENT_SONGKRAN_2005_20050316
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
	if( MaxHitUser != -1 )
	{
		int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);

		if( partycount > 0 )
		{	// 파티를 맺었을때만 
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);

			if( lpattr )
			{
				if( lpattr->m_Level >= 17 )
				{
					if( (rand()%10000) < gHeartOfLoveDropRate )
					{
						// 자이언트 이상일때만
						dur  = 255;
						x    = lpObj->X;
						y    = lpObj->Y;
						level= 2;	// 폭죽
						type = ItemGetNumberMake(14, 11);			
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
						return TRUE;
					}
				}
			}
		}
	}
#endif // EVENT_SONGKRAN_2005_20050316
	
#ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	if( g_bRibbonBoxEvent )	// 크리스마스 리본상자 이벤트가 진행중일 때
	{
		// 몬스터 레벨별로 드랍된다.		
		if( lpObj->Level >= g_iRedRibbonBoxDropLevelMin && lpObj->Level <= g_iRedRibbonBoxDropLevelMax )
		{
			// 빨간리본상자
			if( ( rand() % 10000 ) < g_iRedRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 32);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 빨간리본상자
				x		= lpObj->X;
				y		= lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iGreenRibbonBoxDropLevelMin && lpObj->Level <= g_iGreenRibbonBoxDropLevelMax )
		{
			// 초록리본상자
			if( ( rand() % 10000 ) < g_iGreenRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 33);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 초록리본상자
				x		= lpObj->X;
				y		= lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iBlueRibbonBoxDropLevelMin && lpObj->Level <= g_iBlueRibbonBoxDropLevelMax ) // 70 ~ 97
		{
			// 파란리본상자
			if( ( rand() % 10000 ) < g_iBlueRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 34);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 파란리본상자
				x		= lpObj->X;
				y		= lpObj->Y;							
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;							
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif
			}
		}				
	}
#endif // #ifdef CHRISTMAS_RIBBONBOX_EVENT_20051212
	
#ifdef VALENTINES_DAY_BOX_EVENT_20060124
	if( g_bChocolateBoxEvent )	// 발렌타인데이 초콜릿상자 이벤트가 진행중일 때
	{
		// 몬스터 레벨별로 드랍된다.		
		if( lpObj->Level >= g_iPinkChocolateBoxDropLevelMin && lpObj->Level <= g_iPinkChocolateBoxDropLevelMax )
		{
			// 분홍초콜릿상자 
			if( ( rand() % 10000 ) < g_iPinkChocolateBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 32);
				level	= 0;
#ifdef MODIFY_VALENTINES_DAY_BOX_DROP_20070202
				x		= lpObj->X;
				y		= lpObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iRedChocolateBoxDropLevelMin && lpObj->Level <= g_iRedChocolateBoxDropLevelMax )
		{
			// 빨간초콜릿상자
			if( ( rand() % 10000 ) < g_iRedChocolateBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 33);
				level	= 0;
#ifdef MODIFY_VALENTINES_DAY_BOX_DROP_20070202
				x		= lpObj->X;
				y		= lpObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iBlueChocolateBoxDropLevelMin && lpObj->Level <= g_iBlueChocolateBoxDropLevelMax ) // 70 ~ 97
		{
			// 파란초콜릿상자
			if( ( rand() % 10000 ) < g_iBlueChocolateBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 34);
				level	= 0;
#ifdef MODIFY_VALENTINES_DAY_BOX_DROP_20070202
				x		= lpObj->X;
				y		= lpObj->Y;

				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;
								
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif	
			}
		}				
	}
#endif // #ifdef VALENTINES_DAY_BOX_EVENT_20060124

#ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	if( g_bCandyBoxEvent )	// 화이트데이 사탕상자 이벤트가 진행중일 때
	{
		// 몬스터 레벨별로 드랍된다.		
		if( lpObj->Level >= g_iLightPurpleCandyBoxDropLevelMin && lpObj->Level <= g_iLightPurpleCandyBoxDropLevelMax )
		{
			// 연보라사탕상자 
			if( ( rand() % 10000 ) < g_iLightPurpleCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 32);	
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 연보라사탕상자
				x		= lpObj->X;
				y		= lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend( lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iVermilionCandyBoxDropLevelMin && lpObj->Level <= g_iVermilionCandyBoxDropLevelMax )
		{
			// 주홍초콜릿상자
			if( ( rand() % 10000 ) < g_iVermilionCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 33);
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 주홍초콜릿상자
				x		= lpObj->X;
				y		= lpObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif
			}
		}
		else if( lpObj->Level >= g_iDeepBlueCandyBoxDropLevelMin && lpObj->Level <= g_iDeepBlueCandyBoxDropLevelMax ) // 70 ~ 97
		{
			// 감청초콜릿상자
			if( ( rand() % 10000 ) < g_iDeepBlueCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 34);
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// 감청초콜릿상자
				x		= lpObj->X;
				y		= lpObj->Y;							
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#else
				x		= lpTargetObj->X;
				y		= lpTargetObj->Y;							
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);				
				return TRUE;
#endif
			}
		}				
	}
#endif // #ifdef WHITEDAY_CANDY_BOX_EVENT_20060222
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	if( g_bHallowinDayEventOn )
	{
		bool bIsBossMonster = false;

		// 보스는 드롭하지 않음
		if( lpObj->m_Index == 349 ||		// 발가스
			lpObj->m_Index == 364 ||		// 마야
			lpObj->m_Index == 361 ||		// 나이트메어
			lpObj->m_Index == 362 ||		// 마야의 손
			lpObj->m_Index == 363			// 마야의 손
			)
		{
			bIsBossMonster = true;
		}

#ifdef UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026	// 호박 드랍 레벨 설정_item(kor).txt의 레벨 값을 얻어와서 레벨 설정
		if( bIsBossMonster == false && ( rand() % 10000 ) < g_iHallowinDayEventItemDropRate 
			&& zzzItemLevel(14, 45, lpObj->Level+1))
#else
		if( bIsBossMonster == false && ( rand() % 10000 ) < g_iHallowinDayEventItemDropRate )
#endif	// UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026
		{
			type	= ItemGetNumberMake( 14, 45 );
			level	= 0;
			x		= lpObj->X;
			y		= lpObj->Y;
			int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );
			ItemSerialCreateSend( lpTargetObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, iMaxHitUser );
			return TRUE;
		}
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_EVENT_20061017
	
	
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( g_bSantaPolymorphRingDropOn )
	{
		bool bIsBossMonster = false;

		// 보스는 드롭하지 않음
		if( lpObj->m_Index == 349 ||		// 발가스
			lpObj->m_Index == 364 ||		// 마야
			lpObj->m_Index == 361 ||		// 나이트메어
			lpObj->m_Index == 362 ||		// 마야의 손
			lpObj->m_Index == 363			// 마야의 손
			)
		{
			bIsBossMonster = true;
		}

		if( bIsBossMonster == false &&
			( ( rand() % 10000 ) < g_iSantaPolymorphRingDropRate )
		  )
		{
			type	= ItemGetNumberMake( 13, 41 );
			level	= 0;
			dur		= 100;
			x		= lpObj->X;
			y		= lpObj->Y;
			int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );
			ItemSerialCreateSend( lpTargetObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, iMaxHitUser );
		}
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
	if(g_bIllusionTampleEvent)
	{
		if (!CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;

			// 낡은 두루마리 나올 확률
			if (rand()%10000 < g_iOldScrollDropRate) 
			{
#ifdef MODIFY_ILLUSIONTEMPLE_ITEMVALUE_BUFGIX_20070904
				dur	 = 0;
#else
				dur  = 128;
#endif
				x    = lpObj->X;
				y    = lpObj->Y;

				if(lpObj->Level >= 66)
				{
					if( lpObj->Level < 72)			// +1 ( 66 ~ 71)
					{
						level = 1;
					}
					else if( lpObj->Level < 78)		// +2 (71 ~ 77)
					{
						level = 2;
					}
					else if( lpObj->Level < 84)		// +3 (78~ 83)
					{
						level = 3;
					}
					else if( lpObj->Level < 90)		// +4 (84 ~ 89)
					{
						level = 4;
					}
					else if( lpObj->Level < 96)		// +5 (90~ 96)
					{
						level = 5;
					}
					else							// +6 (96~)
					{
						if(g_bUseMaxLevelIllusionTemple)
						{
							// 400 레벨 전용 환영 사원 사용
							level = 6;
						}
						else
						{
							level = 5;
						}
					}


#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 마스터레벨 이벤트맵 재료면 처리안함
					if( level != 6 )
					{
						type = ItemGetNumberMake(13, 49);
						int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
						return TRUE;
					}
#else	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
					type = ItemGetNumberMake(13, 49);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
				}
			}

			// 환영교단의 서약 나올 확률
			if (rand()%10000 < g_iCovenantOfIllusionDropRate) 
			{
#ifdef MODIFY_ILLUSIONTEMPLE_ITEMVALUE_BUFGIX_20070904
				dur	 = 0;
#else
				dur  = 128;
#endif
				x    = lpObj->X;
				y    = lpObj->Y;

				if( lpObj->Level >= 70 )
				{
					if( lpObj->Level < 76)		// +1 (70 ~ 75)
					{
						level = 1;
					}
					else if( lpObj->Level < 82)		// +2 (76 ~ 81)
					{
						level = 2;
					}
					else if( lpObj->Level < 88)		// +3 (82 ~ 87)
					{
						level = 3;
					}
					else if( lpObj->Level < 94)		// +4 (88 ~ 93)
					{
						level = 4;
					}
					else if( lpObj->Level < 100)	// +5 (94 ~ 99)
					{
						level = 5;
					}
					else							// +6 (100 ~)
					{
						if(g_bUseMaxLevelIllusionTemple)
						{
							// 400 레벨 전용 환영 사원 사용
							level = 6;
						}
						else
						{
							level = 5;
						}
					}
				
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// 마스터레벨 이벤트맵 재료면 처리안함
					if( level != 6 )
					{
						type = ItemGetNumberMake(13, 50);
						int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
						ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
						return TRUE;
					}
#else	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
					type = ItemGetNumberMake(13, 50);
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
#endif	// UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123
				}
			}
		}
	}
#endif	// ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328

#ifdef ADD_EVENT_CHERRYBLOSSOM_20080312
	if (g_bCherryBlossomEventOn) 
	{
		bool bIsBossMonster = false;
		
		// 보스는 드롭하지 않음
		if( lpObj->m_Index == 349 ||		// 발가스
			lpObj->m_Index == 364 ||		// 마야
			lpObj->m_Index == 361 ||		// 나이트메어
			lpObj->m_Index == 362 ||		// 마야의 손
			lpObj->m_Index == 363			// 마야의 손
			)
		{
			bIsBossMonster = true;
		}
		
#ifdef MODIFY_CHERRYBLOSSOM_DROP_VULCANUS_20090324
		if( lpObj->MapNumber != MAP_INDEX_VULCANUS )	// 불카노스가 아니면 드랍하지 않는다.
		{
			return FALSE;
		}
#endif // MODIFY_CHERRYBLOSSOM_DROP_VULCANUS_20090324
		
		//#ifdef UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026	// 벚꽃 드랍 레벨 설정_item(kor).txt의 레벨 값을 얻어와서 레벨 설정
		//		if( bIsBossMonster == false && ( rand() % 10000 ) < g_bCherryBlossomEventItemDropRate 
		//			&& zzzItemLevel(14, 84, lpObj->Level+1))
		//#else
		if( bIsBossMonster == false && ( rand() % 10000 ) < g_bCherryBlossomEventItemDropRate )
			//#endif	// UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026
		{
			type	= ItemGetNumberMake( 14, 84 );	//벚꽃놀이 상자 //45
			level	= 0;
			x		= lpObj->X;
			y		= lpObj->Y;
			int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );
			ItemSerialCreateSend( lpTargetObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, iMaxHitUser );
			return TRUE;
		}
	}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// 행운의 동전 드롭처리
	if(g_bLuckyCoinEventOn)
	{
		if (rand()%10000 < g_iLuckyCoinDropRate) {
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level	= 0;
			dur		= 1;
			x		= lpObj->X;
			y		= lpObj->Y;

			type = ItemGetNumberMake(14, 100);						// 행운의 동전
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
			return TRUE;
		}
	}
#endif	// UPDATE_LUCKY_COIN_EVENT_20081029
	
	return FALSE;
}

#ifdef SECOND_QUEST

void CQeustNpcTeleport::Run(int aIndex)
{
	if( RefCount > 0 )
	{	// 말걸고 있으면... 
		return;
	}

	TimeCount++;
	if( TimeCount > gQuestNPCTeleportTime )
	{	// 텔레포트 		
		TimeCount = 0;		
		
		PMSG_MAGICATTACK_RESULT	pAttack;
	
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// 스킬번호 BYTE -> WORD 
		pAttack.MagicNumberH   = HIBYTE( AT_SKILL_TELEPORT );
		pAttack.MagicNumberL   = LOBYTE( AT_SKILL_TELEPORT );
#else
		pAttack.MagicNumber   = AT_SKILL_TELEPORT;
#endif
		pAttack.SourceNumberH = HIBYTE(aIndex);
		pAttack.SourceNumberL = LOBYTE(aIndex);	
		pAttack.TargetNumberH = HIBYTE(aIndex);
		pAttack.TargetNumberL = LOBYTE(aIndex);	

		MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);			

		gObjViewportListProtocolDestroy(&gObj[aIndex]);
		gObjClearViewport(&gObj[aIndex]);

		int		tableindex;

		while(1)
		{
			tableindex = rand()%4;
			if( LastMapNumber != tableindex )
			{
				LastMapNumber = tableindex;
				break;
			}
		}			
		
		gObj[aIndex].X			= m_QuestNPCTeleportPos[tableindex].x;
		gObj[aIndex].Y			= m_QuestNPCTeleportPos[tableindex].y;

		gObj[aIndex].TX = gObj[aIndex].X;
		gObj[aIndex].TY = gObj[aIndex].Y;
		gObj[aIndex].MTX = gObj[aIndex].X;
		gObj[aIndex].MTY = gObj[aIndex].Y;
		

		gObj[aIndex].m_OldX		= gObj[aIndex].TX;
		gObj[aIndex].m_OldY		= gObj[aIndex].TY;

		gObj[aIndex].MapNumber = m_QuestNPCTeleportPos[tableindex].mapnum;
		gObj[aIndex].Dir = m_QuestNPCTeleportPos[tableindex].dir;

		gObj[aIndex].StartX = (BYTE)gObj[aIndex].X;
		gObj[aIndex].StartY = (BYTE)gObj[aIndex].Y;

		gObj[aIndex].m_State	= OBJST_CREATE;
		gObj[aIndex].PathCount = 0;	

		LogAdd("NPC %d로 이동", tableindex);			
	}

}
#endif