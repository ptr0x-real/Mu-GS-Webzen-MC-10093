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

#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �������� NPC �� ó���� ����
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
CLogToFile	KUNDUN_EVENT_LOG ("KUNDUN_EVENT_LOG", ".\\KUNDUN_EVENT_LOG");		// �α����� ��ü
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

#ifdef ADD_NEW_MAP_KALIMA_20040518				// ���� ó���Լ� ��ü
#include "KalimaGate.h"
#endif

#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207	// ��ȣ������ ����
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
CQeustNpcTeleport	gQeustNpcTeleport;			// NPC���� ���� ����� ���� ����Ʈ
#endif

#ifdef EVENT_ITEMBAG_UPGRADE
extern CItemBagEx	*   StarOfXMasItemBag;		// ��ź�� �� ������ ����(?)
#endif

#ifdef MU_CHN_THANKS_COOKIE_EVENT_20040908 
extern CItemBagEx *	ChnThanksCookieEvent;		// �߱� ���� �̺�Ʈ
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

#define MON_ATTR_0		0	// ���� ���ϱ⸸ �ϴ� ����
#define MON_ATTR_1		1	//
#define MON_ATTR_2		2	//
#define MON_ATTR_100	100	// ��ȯ����


void gObjMonsterMoveRegen(int x, int y, LPOBJECTSTRUCT lpObj)
{
	gMSetBase.GetBoxPosition(lpObj->MapNumber, x-1, y-1, x+1, y+1, lpObj->X, lpObj->Y);
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	//LogAdd("��ġ �̵� %d %d", x, y);
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
		/* /* (�����ض�)
		lpObj->Live	   = 0;
		lpObj->m_State   = OBJST_DYING;	
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen  = 1;			
		*/
		return FALSE;
	}
	else if( lpObj->Class >= 78 && lpObj->Class <= 83 )
	{	// �̺�Ʈ ���ʹ� ��Ȱ��Ű�� �ʴ´�		
		/*/* (�����ض�)
		lpObj->Live	   = 0;
		lpObj->m_State   = OBJST_DYING;	
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen  = 1;			
		*/
		return FALSE;		
	}
#ifdef ADD_GOLDEN_EVENT_RENEWAL_20090311	
	else if( lpObj->Class >= 493 && lpObj->Class <= 502 )
	{	// �̺�Ʈ ���ʹ� ��Ȱ��Ű�� �ʴ´�		
		
		return FALSE;		
	}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311
#ifdef BUGFIX_KALIMAGATE_VANISHING_20041006
	else if( CHECK_KALIMAGATE(lpObj->m_Attribute) ) {
		// Ȥ�ö� �̰��� ������ �������� �����Ѵ�. (������ ���⿡ ���� ������ ����.)
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
		// ��Ŭ���� �Ź̾��� �̰����� ��Ȱ��Ű�� �ʴ´�		
		if( g_Raklion.GetRaklionState() != RAKLION_STATE_END )
			return FALSE;
	}
	else if( lpObj->Class == 459 )
	{
		// ��Ŭ���� ������ �̰����� ��Ȱ ��Ű�� �ʴ´�
		if( g_Raklion.GetRaklionState() != RAKLION_STATE_STANDBY
			|| g_Raklion.GetRaklionState() != RAKLION_STATE_READY )
			return FALSE;
	}
#ifdef MODIFY_RAKLION_COOLUTIN_AND_IRON_KNIGHT_REGEN_BUGFIX_20080828
	else if( lpObj->Class == RAKLION_COOLUTIN && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS
			|| lpObj->Class == RAKLION_IRON_KNIGHT && lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS
			)
	{
		// ��Ŭ���� ������ ��ȯ�� ���ʹ� ���⼭ ��Ȱ ��Ű�� �ʴ´�
		return FALSE;
	}
#else
	else if( lpObj->Class == RAKLION_COOLUTIN || lpObj->Class == RAKLION_IRON_KNIGHT )
	{
		// ��Ŭ���� ������ ��ȯ�� ���ʹ� ���⼭ ��Ȱ ��Ű�� �ʴ´�
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
				// �η����� ���ʹ� �ڽ��� ��������ġ �ֺ��� �����ϰ� ������Ų��.
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
		// MVP ���� ü�� �϶��� ������� �ʴ´�.
	}
	else
	{
		// MVP ���� : ���� ü�� ����
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
			LogAddTD( "�ڡ� ũ���̿��� ���� ���� ���� (%d)[%s] : %f / %f / %d", 
				lpObj->m_Index, lpObj->Name, lpObj->Life, lpObj->MaxLife, lpObj->m_iScriptMaxLife );
#endif // TESTSERVER == 1
	
		}
	}
#endif // MU_CRYWOLF_BENEFIT_20051215


	return TRUE;
	//gObjViewportListProtocolCreate(lpObj);
}

// ������ ����Ʈ�� ĳ���Ͱ� �����ϴ���..
int gObjMonsterViewportIsCharacter(LPOBJECTSTRUCT lpObj)
{
	int tObjNum;

	for(int n=0; n<MAX_MONVIEWPORTOBJECT; n++)
	{
		if( lpObj->VpPlayer2[n].state )
		{
			tObjNum = lpObj->VpPlayer2[n].number;
			if( tObjNum == lpObj->TargetNumber ) // ��� ������ ���ٸ�..
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

// -Ÿ���� ���� ������ ����
// -������ ������ �����
// -�ٸ� �ʿ��ִ� �����
// -�Ÿ��� �� ����ڸ� �����Ѵ�.
int gObjMonsterHitDamageUserDel(LPOBJECTSTRUCT lpObj)
{
	int delok=0;
	int delindex=-1;

	for(int n=0; n<MAX_HITDAMAGE; n++)
	{
		if( lpObj->sHD[n].number >= 0 )
		{
			delok = 0;
			// ������ �ְ�..
			if( gObjIsConnected(lpObj->sHD[n].number) == TRUE )
			{
				// ������ Ÿ���� 20�� ���� ����ڴ� �ؾ������.
				if( (GetTickCount()-(lpObj->sHD[n].LastHitTime)) > (1000*20) )
				{
					delok = 1;
				}
				// ���� �ʿ� ��ġ���� �ʾҴٸ�..
				if( gObj[lpObj->sHD[n].number].MapNumber != lpObj->MapNumber )
				{
					delok = 1;
				}
				else	// ���� �ʿ� �ִٸ�.. ��ġ�� üũ�Ѵ�.
				{
					if( gObjCheckTileArea(	lpObj->m_Index, gObj[lpObj->sHD[n].number].X, gObj[lpObj->sHD[n].number].Y, 20) == FALSE )
					{
						delok = 1;	// �� ��Ʈ�� ����.
					}
				}
			}
			else delok = 1;	// ������ ���� �ʴٸ�			
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
// ����ġ ����� ���� ���Ϳ� Ÿ���� ���� ������ Ÿ�� ���� ����Ѵ�.
// lpObj		: ����
// hit_player	: Ÿ���� ���� ����
// hit_damage	: Ÿ�� ��
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
		// �� ������ �����ϴ�.. ������ Ȯ���ϱ� ���� �����.
		blank = gObjMonsterHitDamageUserDel(lpObj);
	}
	
	// ���� ����ڰ� �ƴҶ� ������� ���� �߰��Ѵ�...
	// �̶�.. �� ������ ������ �߰��Ҽ� ���⶧����.. �ð��� ������ ������ �����Ѵ�.
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


// ���������� ���� ĳ���͸� ã�´�
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


// ���Ͱ� �ش� ��ġ�� �̵��ص� �Ǵ��� üũ�Ͽ� �̵��ص� �Ǹ� TRUE�� 
// �̵��� �� ���ٸ� FALSE�� �����Ѵ�.
BOOL gObjMonsterMoveCheck(LPOBJECTSTRUCT lpObj, int tx, int ty)
{
	// ���޾����� �̵��ص� �ȴ�.
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

// ����� ���� ����� ��ġ�� ��´�.
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������� ����� ��ġ�� ������
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
	
	// Ÿ�� ��ó�� ��ġ�� ��´�.
	if( lpObj->X < mtx ) tpx -= dis;
	if( lpObj->X > mtx ) tpx += dis;
	if( lpObj->Y < mty ) tpy -= dis;
	if( lpObj->Y > mty ) tpy += dis;

	searchp = GetPathPacketDirPos(lpTargetObj->X-tpx, lpTargetObj->Y-tpy)*2;
	if( MapC[lpObj->MapNumber].GetStandAttr( tpx, tpy) == FALSE )	// ���� ���ٸ�.
	{
		while(searchcount--)
		{
			mtx = lpTargetObj->X+RoadPathTable[searchp];
			mty = lpTargetObj->Y+RoadPathTable[searchp+1];
			
			//  �ڸ��� ���������..
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
				(result==TRUE) ) // ��������� �� ���±��̸�..
			{
				lpObj->MTX = tpx;
				lpObj->MTY = tpy;
				return TRUE;
			}
		}
	}
	return FALSE;
}


// ��뿡 ���� ����� ��ġ�� ��´�.
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

	// ���� ���°� 100�� ������.. ���ݱ��̸� 2���..
	if( lpObj->m_AttackType >= 100 )
	{
		dis = lpObj->m_AttackRange+2;
	}
	else dis = lpObj->m_AttackRange;
	
	// Ÿ�� ��ó�� ��ġ�� ��´�.
	if( lpObj->X < mtx ) tpx -= dis;
	if( lpObj->X > mtx ) tpx += dis;
	if( lpObj->Y < mty ) tpy -= dis;
	if( lpObj->Y > mty ) tpy += dis;

	searchp = GetPathPacketDirPos(sx-tpx, sy-tpy)*2;
	if( MapC[lpObj->MapNumber].GetStandAttr( tpx, tpy) == FALSE )	// ���� ���ٸ�.
	{
		while(searchcount--)
		{
			mtx = sx+RoadPathTable[searchp];
			mty = sy+RoadPathTable[searchp+1];
			
			//  �ڸ��� ���������..
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
				//(result==TRUE) ) // ��������� �� ���±��̸�..
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
				//(result==TRUE) ) // ��������� �� ���±��̸�..
			{
				tx = tpx;
				ty = tpy;
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ������ ���� ����� ���� ������ �����Ѵ�.
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
		//LogAdd("���� ���� ���� ���� %d", Target);
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
				gObj[tObjNum].Live)	// ������ �Ҵ�Ǿ� �ִٸ�..
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
					
#ifdef UPDATE_GM_FUNCTION_20070228	// EVENT_GM �� ��쵵 ���� �ȵǰ�
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
							//if( dis <= 3 )	// �ּ� ������ ������..
							{	// �ٸ� �ֵ麸�� ������ �ִٸ�..
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
			//if( gObj[tObjNum].Type == objtype && gObj[tObjNum].Live)	// ������ �Ҵ�Ǿ� �ִٸ�..
			//if( gObj[tObjNum].Live)	// ������ �Ҵ�Ǿ� �ִٸ�..

#ifdef UPDATE_PK_SYSTEM_20070302		// ���θ��� ��Ƽ�� ������� �������� ���� ����
			BOOL	PKFlag = FALSE;
			if(gObj[tObjNum].PartyNumber >= 0)	// ��Ƽ�� ���Ұ��
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
					//if( dis <= 3 )	// �ּ� ������ ������..
					{	// �ٸ� �ֵ麸�� ������ �ִٸ�..
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
		// �ӹ��Ÿ��� ������ �ƹ� �ൿ�� ���� �ʴ´�.
		return;
	}
#endif

	switch( aMsgCode )
	{
	case 0 :	// ���ݴ��ߴ�.
		if( lpObj->m_Attribute == 0 ) return;	// ���ݴ��ϱ⸸ �ϴ� ĳ���Ͷ��..
		
		if( gObj[aIndex].Live == 0 ||
			gObj[aIndex].m_State != OBJST_PLAYING ) return;	// ��밡 �׾��ִٸ�..

		if( lpObj->m_ActState.Emotion == 0 ) 
		{
			lpObj->m_ActState.Emotion = 1;
			lpObj->m_ActState.EmotionCount = 10;
		}
		else if( lpObj->m_ActState.Emotion == 1 ) 
		{
			lpObj->m_ActState.EmotionCount = 10;
		}
		if( !lpObj->m_ActState.Attack && lpObj->PathStartEnd == 0 )	// �������̾ƴϸ�..
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
							// ��ȯ������ ��� ���͸� ���� ����
							if( gObj[aIndex].Type == OBJTYPE_MONSTER)
							{
								lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
							}
						}
						else
						{
							lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
						}
					}
					else {
#ifdef MONSTER_HERD_SYSTEM_20031120
						if( rand()%100 < 90 )				// ���Ͱ� ���ݿ� ���� ����
						{
							
							if( lpObj->m_RecallMon >= 0 )
							{
								// ��ȯ������ ��� ���͸� ���� ����
								if( gObj[aIndex].Type == OBJTYPE_MONSTER)
								{
									lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
								}
							}
							else
							{
								lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
							}
						}
#else
						if( (rand()%3) == 0 )
						{
							
							if( lpObj->m_RecallMon >= 0 )
							{
								// ��ȯ������ ��� ���͸� ���� ����
								if( gObj[aIndex].Type == OBJTYPE_MONSTER)
								{
									lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
								}
							}
							else
							{
								lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
							}
						}
#endif
	
					}
				}
				// ���� �ִٸ� ������ ���Ѵ�.
				else 
				{
					BYTE wall=0;
					wall = MapC[map].CheckWall2(lpObj->X, lpObj->Y, gObj[aIndex].X, gObj[aIndex].Y);

					if( wall == 0x01 )
					{
						attr = MapC[map].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);
						if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE)
						{
							if( lpObj->TargetNumber < 0 ) // ���ݻ�밡 ���ٸ�..
							{
								lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
							}
						}
					}
					else if( wall == 0x02 ) // ĳ���Ͱ� �տ� ������..
					{
					}
					else {	// ���̰ų� �߰��� ���� �ִٸ�..
						//lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�	
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
		{	// �������϶��� ��� �����Ѵ�.. � ���� ��븦 �ٲ۴�..
			if( ((rand()%2) == 1) && lpObj->PathStartEnd == 0 )
			{
				int tuser = lpObj->TargetNumber;
				
				if (!CHECK_LIMIT(tuser, MAX_OBJECT))		return;

				int map = gObj[tuser].MapNumber;
				// ���� �ִٸ� ������ ���Ѵ�.

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
					lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
				}
				else if( MapC[map].CheckWall2(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
				{
					lpObj->m_ActState.Attack = 1;
					lpObj->TargetNumber = aIndex;	// ���ݻ��� �����ϰ�
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
						lpObj->m_ActState.Emotion = 2;	// ��������.
						lpObj->m_ActState.EmotionCount = 2;
					}
				}
			}
		}
		break;
	case 1 :	// ����(���Ͱ�) �׾���
#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �������� NPC ���� �׾��� ��� ���⼭ ó���Ѵ�.
		if (lpObj->m_btCsNpcType != CS_NPC_TYPE_NONE) {
			switch (lpObj->m_btCsNpcType) {
			case CS_NPC_TYPE_DB_DFN :		// ������ NPC - ������ DB
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal, TRUE);
				break;
			case CS_NPC_TYPE_INS_DFN :		// ������ NPC - ������
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal);
				break;
			case CS_NPC_TYPE_INS_ATK :		// ������ NPC - ������
				g_CastleSiege.DelNPC(lpObj->m_Index, lpObj->Class, lpObj->m_iCsNpcExistVal);
				break;
			}
#ifdef ADD_NEW_MERCENARY_FOR_CASTLE_01_20041214		
			if (lpObj->Class == NPC_INDEX_SPEARMAN || lpObj->Class == NPC_INDEX_BOWMAN) 
			{
				g_CsNPC_Mercenary.DeleteMercenary( lpObj->m_Index );
			}
#endif
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208	// 2005.05.23 �߰� (b4nfter)
			if(lpObj->Class == NPC_INDEX_LIFESTONE)
			{
				g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
			}
#endif
			gObjDel(lpObj->m_Index);
		}
#endif
		
#ifdef HIDDEN_KALIMA_20050706				// ����� �ܿ��� ������ �� �ڸ��� ������ ��ȯ�Ѵ�.
		if (CHECK_KALIMAMAP(lpObj->MapNumber)) {
			if (
//			lpObj->Class == 147	||			// �׽�Ʈ
//			lpObj->Class == 177 ||			// �׽�Ʈ
//			lpObj->Class == 185 ||			// �׽�Ʈ
//			lpObj->Class == 193 ||			// �׽�Ʈ
//			lpObj->Class == 263				// �׽�Ʈ
			lpObj->Class == 161 ||			// ����� �ܿ�1
			lpObj->Class == 181 || 			// ����� �ܿ�2
			lpObj->Class == 189 || 			// ����� �ܿ�3
			lpObj->Class == 197 || 			// ����� �ܿ�4
			lpObj->Class == 267				// ����� �ܿ�5
			)
			{
				g_KalimaGate.CreateKalimaGate2(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
		}
#endif
		
#ifdef MODIFY_QUEST_SYSTEM_20070525	// ����Ʈ �Ϸ����� ����Kill���� Ȯ���Ѵ�.
		// ����Ʈ ���͸� �̰��� �̸� ����Ѵ�.
		// ���� ����Ʈ�� �þ�� �ٸ� ����� ã����.
		if( lpObj->Class == 409		// �߶�(�Ʒú�)
			|| lpObj->Class == 410	// �������Ǹ�(�Ʒú�)
			|| lpObj->Class == 411	// �Ҷ�(�Ʒú�)
			|| lpObj->Class == 412	// ��ũ����(�Ʒú�) 
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

#ifdef FOR_BLOODCASTLE		// 2004.02.02 ����ĳ�� ���� �� ���� ���� - ���� �� üũ�κ� (�� ��ƾ�� �߰� ��)
		if( CHECK_BLOODCASTLE(lpObj->MapNumber) && lpObj->Type >= OBJTYPE_MONSTER )
		{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����
			int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum( lpObj->MapNumber );
#endif			
			if (
				lpObj->Class == 89 ||		// �����ذ�1
				lpObj->Class == 95 ||		// �����ذ�2
				lpObj->Class == 112 ||		// �����ذ�3
				lpObj->Class == 118 ||		// �����ذ�4
				lpObj->Class == 124 ||		// �����ذ�5
				lpObj->Class == 130			// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314			// ����ĳ�� �������� üũ�� �ø� (���� �׿��� ��)
				|| lpObj->Class == 143		// �����ذ�7
#endif				
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010		// ����ĳ��8 �������� üũ�� �ø� (���� �׿��� ��)
				|| lpObj->Class == 433		// �����ذ�8
#endif				
				)
			{
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����				
				g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT++;
#else
				g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT++;
#endif
			}
			else {
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����	
				g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT++;
#else
				g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_KILL_COUNT++;

#endif
			}

#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ�� �ٸ� �ε��� ����	
			if (g_BloodCastle.CheckMonsterKillCount(iBridgeIndex)) {
				// 1 . �Ϲݸ��� �Ҵ緮 �Ϸ�üũ				
				if (!g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE) {
					// ���� �Ҵ緮�� ä���ٸ� �Ҵ緮�� �Ϸ�Ǿ����� üũ
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = -1;
					// ����鿡�� �ٸ��� ���������� �˸� -> Ŭ���̾�Ʈ�� �ٸ� �������� �Ӽ�
					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_MONSTEREND;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;
					g_BloodCastle.SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, iBridgeIndex);
					// �ϴ� ������ �ٸ��� ������ �Ӽ��� �ٲ۴�.
					g_BloodCastle.ReleaseCastleBridge(iBridgeIndex);
					// �������� �ٸ��� �Ӽ��� ������ �ð��� ����
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = GetTickCount() + BC_MAX_TICK_DOOR_OPEN;

					LogAddTD("[Blood Castle] (%d) All of the Monster Terminated -> %d", 
						iBridgeIndex+1, 
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT
						);
					
					// �������� �Ҵ緮�� ����Ѵ�.
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(iBridgeIndex) * BC_PERUSER_BOSS_MONSTER_KILL;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

					if( g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
					{
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
					}
				}

				if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT++;
					// ���� �Ҵ緮�� ä�����ٰ� �ش� �ٸ� ����鿡�� �����Ѵ�.
					g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1168));							// "���� ���ſϷ�!! ��������"
				}
			}


			if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE &&
				g_BloodCastle.CheckBossKillCount(iBridgeIndex)
				) 
			{
				// 2 . �������� �Ҵ緮 �Ϸ�üũ
				if (!g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE) {
					// ���� �Ҵ緮�� ä���ٸ� �Ҵ緮�� �Ϸ�Ǿ����� üũ
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = -1;

					g_BloodCastle.SetSaintStatue(iBridgeIndex);

					LogAddTD("[Blood Castle] (%d) All of the Boss Monster Terminated -> %d", 
						iBridgeIndex + 1, 
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
				}

				if (g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT++;
					// �������� �Ҵ緮�� ä�����ٰ� �ش� �ٸ� ����鿡�� �����Ѵ�.
					g_BloodCastle.SendNoticeMessage(iBridgeIndex, lMsg.Get(1180));		// "�������� ���ſϷ�!! ������ �ı��ϼ���!"
				}
			}
#else	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010
			if (g_BloodCastle.CheckMonsterKillCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)) {
				// 1 . �Ϲݸ��� �Ҵ緮 �Ϸ�üũ
				if (!g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE) {
					// ���� �Ҵ緮�� ä���ٸ� �Ҵ緮�� �Ϸ�Ǿ����� üũ
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_MAX_COUNT = -1;
					// ����鿡�� �ٸ��� ���������� �˸� -> Ŭ���̾�Ʈ�� �ٸ� �������� �Ӽ�
					PMSG_STATEBLOODCASTLE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(pMsg));
					pMsg.btPlayState		= BC_STATE_MONSTEREND;
					pMsg.wRemainSec			= 0;
					pMsg.wMaxKillMonster	= 0;
					pMsg.wCurKillMonster	= 0;
					pMsg.wUserHaveWeapon	= 0;
					pMsg.btWeaponNum		= -1;
					g_BloodCastle.SendBridgeAnyMsg((LPBYTE)&pMsg, pMsg.h.size, lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					// �ϴ� ������ �ٸ��� ������ �Ӽ��� �ٲ۴�.
					g_BloodCastle.ReleaseCastleBridge(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);
					// �������� �ٸ��� �Ӽ��� ������ �ð��� ����
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_dwBC_TICK_DOOR_OPEN = GetTickCount() + BC_MAX_TICK_DOOR_OPEN;

					LogAddTD("[Blood Castle] (%d) All of the Monster Terminated -> %d", 
						lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_KILL_COUNT
						);
					
					// �������� �Ҵ緮�� ����Ѵ�.
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1) * BC_PERUSER_BOSS_MONSTER_KILL;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;

					if( g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT > 10 )
					{
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
					}
				}

				if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_MONSTER_SUCCESS_MSG_COUNT++;
					// ���� �Ҵ緮�� ä�����ٰ� �ش� �ٸ� ����鿡�� �����Ѵ�.
					g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1168));							// "���� ���ſϷ�!! ��������"
				}
			}


			if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_MONSTER_KILL_COMPLETE &&
				g_BloodCastle.CheckBossKillCount(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1)
				) 
			{
				// 2 . �������� �Ҵ緮 �Ϸ�üũ
				if (!g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_BOSS_MONSTER_KILL_COMPLETE) {
					// ���� �Ҵ緮�� ä���ٸ� �Ҵ緮�� �Ϸ�Ǿ����� üũ
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_bBC_BOSS_MONSTER_KILL_COMPLETE = TRUE;
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_MAX_COUNT = -1;

					g_BloodCastle.SetSaintStatue(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1);

					LogAddTD("[Blood Castle] (%d) All of the Boss Monster Terminated -> %d", 
						lpObj->MapNumber-MAP_INDEX_BLOODCASTLE1+1, 
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_KILL_COUNT);
				}

				if (g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT < BC_MAX_MONSTER_SUCCESS_MSG) {
					g_BloodCastle.m_BridgeData[lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT++;
					// �������� �Ҵ緮�� ä�����ٰ� �ش� �ٸ� ����鿡�� �����Ѵ�.
					g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - MAP_INDEX_BLOODCASTLE1, lMsg.Get(1180));		// "�������� ���ſϷ�!! ������ �ı��ϼ���!"
				}
			}
#endif	// ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010			
		}
#endif
		break;
	case 2 :	// ���� ������ ���ߴ�. (�ڷ� ��������)
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
	case 3 :	// ���� ����ڰ� �׾���..
		lpObj->TargetNumber = -1;
#ifdef FOR_BLOODCASTLE
		lpObj->LastAttackerID = -1;
#endif
		lpObj->m_ActState.Emotion = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->m_ActState.Move = 0;
		lpObj->NextActionTime = 1000;
		break;
	case 4 :	// ������ ���� ��� �ӹ��Ÿ���.
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
	case MSG_ATTACK_NORMAL :  // �������ִ� �Ϲ� ���� ����
		gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0);		
		//MsgOutput(aIndex, "[MSG_ATTACKPROC][NORMAL ATTACK] %s... ", lpObj->Name);
		break;
	case MSG_MONSTER_SKILL_POISON :	// ������ �ִ� ��ų ���� ����
		{
			LPOBJECTSTRUCT lpTargetObj = &gObj[aIndex];
			// �� ��ų
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
			if( lpTargetObj->m_PoisonType == 0 ) // ���� �ߵ� ������ �ɷ��ִٸ� �ٽ� �ɸ��� �ʰ�..
		#else
			if( lpTargetObj->m_PoisonBeattackCount == 0 ) // ���� �ߵ� ������ �ɷ��ִٸ� �ٽ� �ɸ��� �ʰ�..
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
			{
				if( retResistance(lpTargetObj, RESISTANCE_POISON) == FALSE ) 
				{
		#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonType = AT_SKILL_POISON;
		#endif	//#ifdef ADD_SKILL_WITH_COMBO
					lpTargetObj->m_PoisonBeattackCount	= aMsgSubCode;	// �� ����
					lpTargetObj->lpAttackObj			= lpObj;
					lpTargetObj->m_ViewSkillState		|= 0x01; //	1��° ��Ʈ
					GCStateInfoSend(lpTargetObj, 1, lpTargetObj->m_ViewSkillState);
				}
			}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004		
		}
		break;
	case MSG_MONSTER_SKILL_SPRING: // ������ �ִ� �б� ����
		{
			// aMsgSubCode : �Ÿ�
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

#ifdef ADD_NEW_MAP_KALIMA_20040518						// Į���� ���������� �߰� ���ݵ�
	if (lpObj->m_iMonsterBattleDelay > 0) {	// ������ �ð��� �ֱ������� ��� ��´�.
		lpObj->m_iMonsterBattleDelay --;
	}
#endif

	if( (GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime) ) return;
	lpObj->CurActionTime = GetTickCount();	
	
#ifdef FOR_BLOODCASTLE
	if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {
		if ( (lpObj->Class == 131) || (CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))) {	// �����̳� �����Ǽ����� �ʿ����δ�.
			return;
		}
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518						// ������ �������� �ʴ´�. -> ���ݵ� ���Ѵ�. ��� �ڽ����� �� �ִ� ����ڸ� ã�Ƽ� �̵���Ű�� �ð��� ���� ���� �پ���.
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


#ifdef CASTLE_NPC_GUARDIANSTATUE_WORK_20041207			// ��ȣ������ �������� �ʴ´�.
	if (lpObj->Class == 283) {
		return;
	}
#endif

#ifdef CASTLE_NPC_CANNON_TOWER_20041223					// ĳ��Ÿ���� ������ �����Ѵ�.
	if (lpObj->Class == 288) {
		return;
	}
#endif
	
#ifdef CASTLE_NPC_LIFESTONE_WORK_20041208				// ������������ �������� �ʴ´�.
	if( lpObj->Class ==  NPC_INDEX_LIFESTONE )
	{
		return;
	}
#endif
	
#ifdef FOR_BLOODCASTLE
	if( lpObj->Class >= 100 && lpObj->Class <= 110 )	// Ʈ���϶�
#else
	if( lpObj->Class >= 100 && lpObj->Class < 110 )		// Ʈ���϶�
#endif
	{
		gObjMonsterTrapAct(lpObj);
	}
	else if( lpObj->Class == 200 )		// ��Ʋ��Ŀ �౸���� ��
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
	// ������ �뺴�� �ٸ��� �ൿ�Ѵ�.
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
	
	// ��ȯ ���� �� �� -> ���ΰ��� ��ġ Ȯ�� �� �ָ� �ڷ���Ʈ
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
				//LogAdd("��ġ �̵� %d %d", lpCallObj->X+1, lpCallObj->Y);
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
#ifdef ADD_RAKLION_20080408		// ��Ŭ���� �������� ������ ��� ���� ó���ϰ� �ִ�.
		if( lpObj->Connected==3 && lpObj->Type==OBJTYPE_MONSTER && lpObj->Class==RAKLION_SELUPAN )
			return;
#endif // ADD_RAKLION_20080408

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
		// ���̴� �� �̻� ���� ����Ǵ� ���� ���� �ý���.(��ų ���)
		if( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
		{
			BOOL bEnableAttack = TRUE;
			// 1) Ÿ���� ������.
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

			// 1) �׾���
			// 2) �ڷ���Ʈ ���� ��
			if( !gObj[lpObj->TargetNumber].Live 
				|| gObj[lpObj->TargetNumber].Teleport)
			{
				bEnableAttack = FALSE;
			}

			// 1) ���� ���� ���� �ƴϰ�,
			// 2) ���� ���̰�,
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

			// ���⼭ ��ų�Ŵ����� ó��

			// AI���� ��ų ó��!!!!!!!

			// 1. �Ϲ� ����
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
				// 2. ���� ��ų ����
				TMonsterSkillManager::UseMonsterSkill( lpObj->m_Index, lpObj->TargetNumber, 0 );
			}
	
			lpObj->m_ActState.Attack = 0;

			return;
		}
		// ������ ���� �κ�.
#endif

		int AttackType = lpObj->m_AttackType;
		int MagicAttack=0;

		if( AttackType >= 100 )
		{
			if( (rand()%5) == 0 )	// 5 ���� 1 Ȯ���� ��������
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
						//if( lpObj->m_Attribute == 100 )	LogAdd("��밡 �׾���.. ������ ��ٸ���.");
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
								if (lpObj->m_lpMonsterHerd != NULL)		// �������� ���� �� �ൿ�� ������ �� �� �޼��带 �������̵�
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
						//if( lpObj->m_Attribute == 100 )	LogAdd("��밡 �׾���.. ������ ��ٸ���.");
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
								if (lpObj->m_lpMonsterHerd != NULL)		// �������� ���� �� �ൿ�� ������ �� �� �޼��带 �������̵�
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


// ���� �������� (�Ǹ��� �Ҳ�)�� ó���ϴ� �κ�
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

	// �� ���� ũ�� �����ؾ� �Ѵ�. 
	// ���� ������ ����Ʈ������ ���������� �����Ǿ���! (apple)
	BYTE AttackSendBuff[256];
	int  ASBOfs=0;
	
	PMSG_DURATION_MAGIC_RECV pDuration;
	
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
	pCount.MagicNumberH	= HIBYTE( 0 );
	pCount.MagicNumberL	= LOBYTE( 0 );
#else
	pCount.MagicNumber = 0;
#endif
	pDuration.Dir = 0;
	pDuration.X = (BYTE)lpObj->X;
	pDuration.Y = (BYTE)lpObj->Y;

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
	// ����� ��쿡�� ������ ������ �Ѵ� - apple
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
	// ���(275) �⺻ ������ ��ü ��������. (apple)
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
						// ��� �� ��� ��ü ������ �� �Ÿ� ����.
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
		// ����� ȯ���� ���
		if (
			lpObj->Class == 161 ||		// ����� ȯ��1
			lpObj->Class == 181 || 		// ����� ȯ��2
			lpObj->Class == 189 || 		// ����� ȯ��3
			lpObj->Class == 197 || 		// ����� ȯ��4
			lpObj->Class == 267 || 		// ����� ȯ��5
			lpObj->Class == 275 		// ����� ȯ��6
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
	// *. 6Į������ ����϶� 
	if( lpMonsterObj->Class == 275 )
	{
		PMSG_MAGICATTACK pAttackMsg;

		// ��ȯ ���� �ε��� = 1
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		pAttackMsg.MagicNumberH	= HIBYTE( 1 );				
		pAttackMsg.MagicNumberL	= LOBYTE( 1 );				
#else
		pAttackMsg.MagicNumber	= 1;				
#endif		
		pAttackMsg.NumberH		= HIBYTE(lpMonsterObj->TargetNumber);
		pAttackMsg.NumberL		= LOBYTE(lpMonsterObj->TargetNumber);
		pAttackMsg.Dis			= 0;
	
		// �ǰ� �� ���� �ʾ����� ���� �� ��ȯ!
		// ���� �ε��� 1 ���� ��ȯ ����

		
		// 2. ���� ��ȯ�� 5�� ���� �Ѵ�.
		if( lpMonsterObj->Life < (lpMonsterObj->MaxLife / 25) )
		{

			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
		}
		// 1. ���� ��ȯ�� 3�� ���� �Ѵ�.
		else if( lpMonsterObj->Life < (lpMonsterObj->MaxLife / 5) )
		{
			CGMagicAttack(&pAttackMsg, lpMonsterObj->m_Index);
		}
	}

}
#endif

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
// ���� ���� ������ ó���� ���� �Լ��̴�. (apple)
// CGBeattackRecv�� ���͸��� ���� ó�� �Լ��̴�.!
void gObjMonsterBeattackRecv( LPBYTE lpRecv, int aIndex )
{
	LPPMSG_BEATTACK_COUNT	lpCount = (LPPMSG_BEATTACK_COUNT)lpRecv;
	
	int	lOfs;
	int tNumber;
	CMagicInf *lpMagic;

	lOfs = sizeof(PMSG_BEATTACK_COUNT);
	LPPMSG_BEATTACK	lpMsg;

	// ������ ��� ���� ��ȣ ��� ó��
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
//		pAttackMsg.MagicNumber = 3;// ����� ó��.
//		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
//		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
//		pAttackMsg.Dis     = 0;
//		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
	}
#endif

#ifdef ADD_NEW_MAP_KALIMA_20040518	// Į���� ���������� �߰� ���ݵ�
	if (lpObj->m_iMonsterBattleDelay > 0) {	// �ӹ��Ÿ��� ���� (Ŭ���̾�Ʈ ���� �� ��) ���� ������ ���� �ʴ´�.
		return;
	}

	if (
		lpObj->Class == 144 ||		// ���� ����1
		lpObj->Class == 174 ||		// ���� ����2
		lpObj->Class == 182 ||		// ���� ����3
		lpObj->Class == 190 ||		// ���� ����4
		lpObj->Class == 260 ||		// ���� ����5
		lpObj->Class == 268			// ���� ����6
		)
	{
		// 1/2 Ȯ���� �������
		if (rand()%2) {
			PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
			pAttackMsg.MagicNumberH = HIBYTE( 0 );	// �⺻ AttackType�� 3 ��
			pAttackMsg.MagicNumberL = LOBYTE( 0 );
#else
			pAttackMsg.MagicNumber = 0;				// �⺻ AttackType�� 3 ��
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
		lpObj->Class == 161 ||		// ����� ȯ��1
		lpObj->Class == 181 || 		// ����� ȯ��2
		lpObj->Class == 189 || 		// ����� ȯ��3
		lpObj->Class == 197 || 		// ����� ȯ��4
		lpObj->Class == 267 || 		// ����� ȯ��5
		lpObj->Class == 275 		// ����� ȯ��6
		)
	{
		// ���
		PMSG_MAGICATTACK pAttackMsg;

		// ��ü���� 0���� (attacktype�� 100 �̻��θ���) �߰� �Ǿ� �ֱ� ������
		// rand()%6 + 1�� �ȴ�. (apple)
		
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		if( wRandValue == 1 || wRandValue == 2 || wRandValue == 0)
			gObjUseMonsterSpecialAbillity( lpObj );
#else
#ifdef ADD_ABILLITY_KUNDUN_01_20040920	
		if( pAttackMsg.MagicNumber == 1 || pAttackMsg.MagicNumber == 2 || pAttackMsg.MagicNumber == 0)
			gObjUseMonsterSpecialAbillity( lpObj );
#endif
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107

#ifdef MODIFY_MONSTER_ENTIRE_ATTACK_20040915
		gObjMonsterMagicAttack(lpObj, lpObj->Class);		// ����� ȯ���� ��� ���� ���� (������ �Ҳ�) �� �׻� ������.
#else
		gObjMonsterMagicAttack(lpObj);						// ����� ȯ���� ��� ���� ���� (������ �Ҳ�) �� �׻� ������.
#endif
		return;
	}

	if (
		lpObj->Class == 149 ||		// ��ũ��1
		lpObj->Class == 179 ||		// ��ũ��2
		lpObj->Class == 187 ||		// ��ũ��3
		lpObj->Class == 195 ||		// ��ũ��4
		lpObj->Class == 265 ||		// ��ũ��5
		lpObj->Class == 273			// ��ũ��6
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wRandValue = ( rand() % 2 )+ 1;
		pAttackMsg.MagicNumberH = HIBYTE( wRandValue );		// ��ũ���� ���� ���������� ������ �� �� �ϳ��� ������.
		pAttackMsg.MagicNumberL = LOBYTE( wRandValue );		
#else
		pAttackMsg.MagicNumber = rand()%2+1;				// ��ũ���� ���� ���������� ������ �� �� �ϳ��� ������.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}


	if (
		lpObj->Class == 145 ||		// �ε� ��Ʃ����1
		lpObj->Class == 175 ||		// �ε� ��Ʃ����2
		lpObj->Class == 183 ||		// �ε� ��Ʃ����3
		lpObj->Class == 191 ||		// �ε� ��Ʃ����4
		lpObj->Class == 261 ||		// �ε� ��Ʃ����5
		lpObj->Class == 269 ||		// �ε� ��Ʃ����6
		lpObj->Class == 146 ||		// ���� ����1
		lpObj->Class == 176 || 		// ���� ����2
		lpObj->Class == 184 ||		// ���� ����3
		lpObj->Class == 192 ||		// ���� ����4
		lpObj->Class == 262 ||		// ���� ����5
		lpObj->Class == 270 ||		// ���� ����6
		lpObj->Class == 147 ||		// ���� ��1
		lpObj->Class == 177 ||		// ���� ��2
		lpObj->Class == 185 ||		// ���� ��3
		lpObj->Class == 193 ||		// ���� ��4
		lpObj->Class == 263 ||		// ���� ��5
		lpObj->Class == 271 ||		// ���� ��6
		lpObj->Class == 148 ||		// ���� ��Ʃ����1
		lpObj->Class == 178 ||		// ���� ��Ʃ����2
		lpObj->Class == 186 ||		// ���� ��Ʃ����3
		lpObj->Class == 194 ||		// ���� ��Ʃ����4
		lpObj->Class == 264 ||		// ���� ��Ʃ����5
		lpObj->Class == 272 ||		// ���� ��Ʃ����6
//			lpObj->Class == 149 ||		// ��ũ��1
//			lpObj->Class == 179 ||		// ��ũ��2
//			lpObj->Class == 187 ||		// ��ũ��3
//			lpObj->Class == 195 ||		// ��ũ��4
//			lpObj->Class == 265 ||		// ��ũ��5
//			lpObj->Class == 273 ||		// ��ũ��6
		lpObj->Class == 160 ||		// ����� �ܿ�1
		lpObj->Class == 180 ||		// ����� �ܿ�2
		lpObj->Class == 188 ||		// ����� �ܿ�3
		lpObj->Class == 196 ||		// ����� �ܿ�4
		lpObj->Class == 266 ||		// ����� �ܿ�5
		lpObj->Class == 274			// ����� �ܿ�6
		)
	{
		// 1/2 Ȯ���� ������ �� ����
		if (rand()%2) {
			PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
			pAttackMsg.MagicNumberH = HIBYTE( 1 );		// �⺻ AttackType�� 150�� ���¿��� ������ �߰������Ƿ� �ε����� 1
			pAttackMsg.MagicNumberL = LOBYTE( 1 );	
#else
			pAttackMsg.MagicNumber = 1;				// �⺻ AttackType�� 150�� ���¿��� ������ �߰������Ƿ� �ε����� 1
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
		lpObj->Class == 89 ||		// �����ذ�1
		lpObj->Class == 95 ||		// �����ذ�2
		lpObj->Class == 112 ||		// �����ذ�3
		lpObj->Class == 118 ||		// �����ذ�4
		lpObj->Class == 124 ||		// �����ذ�5
		lpObj->Class == 130			// �����ذ�6
#ifdef BLOODCASTLE_EXTEND_20040314	// ����ĳ�� �������� üũ�� �ø�
		|| lpObj->Class == 143		// �����ذ�7
#endif		
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ����ĳ��8 �������� üũ�� �ø�
		|| lpObj->Class == 433		// �����ذ�8
#endif		
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		WORD wRandValue = ( rand() % 2 ) + 1;
		pAttackMsg.MagicNumberH = HIBYTE( wRandValue );		// �����ذ��� ���� ����� �������� �� �߿� �ϳ��� ������.
		pAttackMsg.MagicNumberL = LOBYTE( wRandValue );		
#else
		pAttackMsg.MagicNumber = rand()%2+1;				// �����ذ��� ���� ����� �������� �� �߿� �ϳ��� ������.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ���� �ü����� ������ ��� ������ ���̴�.
	if( 
		lpObj->Class == 163 ||		// ī���� ĳ�� - �ü�1
		lpObj->Class == 165 ||		// ī���� ĳ�� - �ü�2
		lpObj->Class == 167 ||		// ī���� ĳ�� - �ü�3
		lpObj->Class == 169 ||		// ī���� ĳ�� - �ü�4
		lpObj->Class == 171 ||		// ī���� ĳ�� - �ü�5
		lpObj->Class == 173			// ī���� ĳ�� - �ü�6
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī�����ü�7 �߰�
		|| lpObj->Class == 427			// ī���� ĳ�� - �ü�7
#endif		
		)
	{
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
		pAttackMsg.MagicNumberH = HIBYTE( 1 );							// ī����ĳ�� �ü��� ���� �׻� ������ ���� ���.
		pAttackMsg.MagicNumberL = LOBYTE( 1 );							// ī����ĳ�� �ü��� ���� �׻� ������ ���� ���.
#else
		pAttackMsg.MagicNumber = 1;							// ī����ĳ�� �ü��� ���� �׻� ������ ���� ���.
#endif
		pAttackMsg.NumberH = HIBYTE(lpObj->TargetNumber);
		pAttackMsg.NumberL = LOBYTE(lpObj->TargetNumber);
		pAttackMsg.Dis     = 0;
		CGMagicAttack(&pAttackMsg, lpObj->m_Index);
		return;
	}
#endif
	
/*	if (
		lpObj->Class == 87 ||		// ���̾�Ʈ����1
		lpObj->Class == 93 ||		// ���̾�Ʈ����2
		lpObj->Class == 99 ||		// ���̾�Ʈ����3
		lpObj->Class == 116 ||		// ���̾�Ʈ����4
		lpObj->Class == 122 ||		// ���̾�Ʈ����5
		lpObj->Class == 128			// ���̾�Ʈ����6
		)
	{
		if(rand()%2) {
			// ���̾�Ʈ������ ���� 1/2 Ȯ���� �������ݰ� �Ϲݰ����� ������ �Ѵ�.
			PMSG_MAGICATTACK pAttackMsg;
			pAttackMsg.MagicNumber = 1;		// �߰��Ǿ��� ��������
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
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
	{	// ���ֹ��� ��
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
	{	// ���ֹ��� ��
		PMSG_MAGICATTACK pAttackMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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
	case 0 :	// �Ϲ� ����
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
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
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


// ���Ͱ� Ÿ���� �ִٸ� �� ��ó���� �̵��Ѵ�.
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
	
	// �ڽſ��� ����� ��ġ�� ��´�.
	if( lpObj->X < mtx ) tpx = mtx-1;
	if( lpObj->X > mtx ) tpx = mtx+1;
	if( lpObj->Y < mty ) tpy = mty-1;
	if( lpObj->Y > mty ) tpy = mty+1;

	//searchp = GetPathPacketDirPos(gObj[tObjNum].X-tpx, gObj[tObjNum].Y-tpy)*2;

	while(searchcount--)
	{
		mtx = gObj[tObjNum].X+RoadPathTable[searchp];
		mty = gObj[tObjNum].Y+RoadPathTable[searchp+1];
		//  �ڸ��� ���������..
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

// �̵��ϰ��� �ϴ°������� �̵��н��� ã�� �̵��ߴٸ� �̵� ��Ŷ�� ����� ������.
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� �����̵��� �� �� ����
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
		
		// ADD_NEW_MONSTER_SYSTEM_02_20050725 -- ���ʿ��� �ε����� ���Ⲭ ���Ƴ��Ҵ�.
		// �ϴ� Ǯ�����.
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
		//vChatSend(lpObj, "�̵����� %d / %d  -> %d / %d", lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY);
		return TRUE;
	}
	return FALSE;
}


// ���Ͱ� �ٸ������� �̵��� ����.
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¿����� �̵��� �� �� ����
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
		
		// �ʹ� �ָ� ���� �ʾҳ�?? üũ
		BOOL mchk = gObjMonsterMoveCheck(lpObj, tpx, tpy);
		/*if( mchk == FALSE )
		{
			tpx = lpObj->StartX;
			tpy = lpObj->StartY;
			mchk = TRUE;
		}*/
		BYTE attr = MapC[lpObj->MapNumber].GetAttr(tpx, tpy);
		// ����϶� �������뵵 ���� �ִ�.
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

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �������¸� ������ ����
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SLEEP ) == true )
		return;
#endif
	
	if( lpObj->m_ActState.Emotion == 0 )	// ����..
	{
		if( lpObj->m_Attribute != 0 )	//
		{
			if( lpObj->m_ActState.Attack )	// ������ ������ �̾��ٸ�..
			{
				lpObj->m_ActState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
				//if( lpObj->m_Attribute == 100 )	LogAdd("������ �̾��ٸ� ��� ����.");
			}
			
			int actcode1 = (rand()%2);

			if( lpObj->m_Attribute == 100 )
			{
				actcode1 = 1;
			}

			// ��� ����.. 
			if( actcode1 == 0 )
			{
				//if( lpObj->m_Attribute == 100 )	LogAdd("��� ����.");
				lpObj->m_ActState.Rest = 1;
				lpObj->NextActionTime = 500;
				//lpObj->NextActionTime = 1;
			}
			// �ٸ������� �̵� �� ����.

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
									//if( lpObj->m_Attribute == 100 )	LogAdd("���� ������ �̵��غ���.");
									//lpObj->NextActionTime = 0;
								}
							}
						}
						//gObjMonsterMoveRegen(gObj[lpObj->m_RecallMon].X, gObj[lpObj->m_RecallMon].Y, lpObj);
					}
				}
			}

			// ���� ã�ƴٴѴ�.
			if( lpObj->Class == 249 || lpObj->Class == 247 ) // ����϶� PK ã�´�.
			{
				// ��ȣ��� �����ȳ�.. ��� �ȳ� 
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
							//if( lpObj->TargetNumber>=0) LogAdd("���� ã�� �̵��Ѵ�.");
						}

					//}
					//else LogAdd("���� ���� ���� ���� %d", lpObj->TargetNumber);

				}
				else lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJTYPE_CHARACTER);
			}
			
			// ���� �߰ߵǸ� ���޾Ƽ� ���� ���� ����~~
			if( lpObj->TargetNumber >= 0 )
			{
				lpObj->m_ActState.EmotionCount = 30;
				lpObj->m_ActState.Emotion = 1;
				//if( lpObj->m_Attribute == 100 )	LogAdd("���� ���� ����");
			}
		}
	}
	else if( lpObj->m_ActState.Emotion == 1 )	// ������..
	{		
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		if( lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0 )	// ���� ����� ������..
		{
#ifdef FOR_BLOODCASTLE
			if (CHECK_BLOODCASTLE(lpObj->MapNumber)) {				// ����� ����ĳ���� �����
				INT iRAND_CHANGE_TARGET = rand()%10;
				if (iRAND_CHANGE_TARGET == 3) {
					lpObj->LastAttackerID = -1;
				}
				if (iRAND_CHANGE_TARGET == 1) {
					if ((lpObj->LastAttackerID != -1) && (lpObj->LastAttackerID != lpObj->TargetNumber)) {
						// ���� �������� ������ ����� ���� ��ǥ���� ����� �ٸ���.
						if ((gObj[lpObj->LastAttackerID].Connected > 2) && (lpObj->MapNumber == gObj[lpObj->LastAttackerID].MapNumber)) {
							// ���� ���� �ʻ� �������ִٸ� ���ݴ���� �ٲ۴�.
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

			//if( lpObj->m_Attribute == 100 )	LogAdd("���ݴ�� üũ1 �Ÿ� : %d", dis);

			if(  dis <= attackRange )
			{			//lpObj->m_ActState.Attack = 1;
				int tuser = lpObj->TargetNumber;
				int map = gObj[tuser].MapNumber;
				BYTE attr ;
				if( MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
				{
					attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);
					if( (attr&MAP_ATTR_SAFTYZONE) != MAP_ATTR_SAFTYZONE)// ���� �Ӽ��� �ƴϸ�
					{
						lpObj->m_ActState.Attack = 1;
					}
					else {	// ���� �Ӽ��� �ִٸ�.
						lpObj->TargetNumber = -1;
						lpObj->m_ActState.EmotionCount = 30;
						lpObj->m_ActState.Emotion = 1;					
					}
					lpObj->Dir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
					lpObj->NextActionTime = lpObj->m_AttackSpeed;
					if( lpObj->m_Attribute == 100 )	{
						//LogAdd("��ó�� ���� �����Ѵ�. ���� �׼� �ð� %d",lpObj->NextActionTime);
					}
				}
#ifdef MODIFY_0708_BUFFIX_20070820	// ����� ��� ���� ����� ���ڿ� ������ �̵�
#ifdef MODIFY_MONSTER_ATTACK_BUGFIX_20080902	// ��� ���Ϳ� ����
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
						//if( lpObj->m_Attribute == 100 )	LogAdd("���� ��ó�� �̵� %d",lpObj->NextActionTime);
					}
					else
					{
						gObjMonsterMoveAction(lpObj);
						lpObj->m_ActState.Emotion = 3;
						lpObj->m_ActState.EmotionCount = 10;
						//if( lpObj->m_Attribute == 100 )	LogAdd("���� ��ó�� �̵�2 %d",lpObj->m_ActState.EmotionCount);
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
					//if( lpObj->m_Attribute == 100 )	LogAdd("���� ��ó�� �̵�3 %d",lpObj->m_ActState.EmotionCount);
				}
			}
		}
		else {			
		}
	}
	else if( lpObj->m_ActState.Emotion == 2 )	// ��������.
	{
		// ��� ������ ����.
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		lpObj->m_ActState.Move = 1;
		lpObj->NextActionTime = 800;

		if( lpTargetObj!=NULL)
		{
			int tdir = GetPathPacketDirPos( lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y)*2;
			
			lpObj->MTX += RoadPathTable[tdir]*-3;	// �ϴ� ��������.
			lpObj->MTY += RoadPathTable[tdir+1]*-3;
		}
		//if( lpObj->m_Attribute == 100 )	LogAdd("��������.");
	}
	else if( lpObj->m_ActState.Emotion == 3 )	// ��� �������� �Ծ� ������ �ش�.
	{
		if( lpObj->m_ActState.EmotionCount > 0 ) lpObj->m_ActState.EmotionCount--;
		else {
			lpObj->m_ActState.Emotion = 0;
		}
		lpObj->m_ActState.Move = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->NextActionTime = 400;
		//if( lpObj->m_Attribute == 100 )	LogAdd("�������� ����");
	}
}


// Ʈ���� �����Ҷ� ���� ã�´�.
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
			// Ʈ�� ���� (�ε����� 100~110���� ����)�� m_AttackRange�� 0�̻��̰� Dir�� 8�� ��� �������� Ʈ��
			// Ʈ�� ���� ������ ������ ã�´�.
			gObjTrapAttackEnemySearchRange( lpObj, lpObj->m_AttackRange );
		}
#endif
	}
	else gObjTrapAttackEnemySearch(lpObj);

	if( lpObj->TargetNumber >= 0 ) {
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	// �����Ĵ� �ð��� ���� ������Ŵ
	}
	else lpObj->NextActionTime = lpObj->m_MoveSpeed;	// ������ ���Ҷ��� ���� üŷ�ϱ� ����.. movespeed �� ���..
}


BYTE NewOptionRand(int level)
{
	BYTE NOption=0;
	
	NOption = 1<<(rand()%6);

	if( NOption & 0x02 )
	{	// ��� ������ Ȯ���� �����
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
//õ���� ���� ������ �� ����üũ
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
		lpObj->NextActionTime = lpObj->m_AttackSpeed;	// �����Ĵ� �ð��� ���� ������Ŵ
	}
	else lpObj->NextActionTime = lpObj->m_MoveSpeed;	// ������ ���Ҷ��� ���� üŷ�ϱ� ����.. movespeed �� ���..	
}
#endif

BOOL IsCanNotItemDtopInDevilSquare(int ItemType)
{
	int checkitemtype = ItemType/MAX_ITEM_INDEX;
	int	checkitemindex = ItemType%MAX_ITEM_INDEX;

	if( checkitemtype >= 7 && checkitemtype <= 11 )
	{	// ��ũ�Ǵн�, �׷���ҿ�, Ȧ�������� ��Ʈ
		if( checkitemindex == 17 || checkitemindex == 18 || checkitemindex == 19 )
		{
			return FALSE;
		}
	}
	else if( checkitemtype == 0  )
	{	// ��ũ�극��Ŀ, �������̵�
		if( checkitemindex == 17 || checkitemindex == 18 )
		{
			return FALSE;
		}
	}
	else if( checkitemtype == 12  )
	{	// �г����ϰ�,â���, ����������, �������, ����, ���̽��ַο�, �Ϸ���Ʈ����ũ ����
		if( checkitemindex == 12 || checkitemindex == 13 || checkitemindex == 14 || 
			checkitemindex == 16 || checkitemindex == 17 || checkitemindex == 18 || 
			checkitemindex == 19 )
		{
			return FALSE;
		}
	}	
	else if( (ItemType == MAKE_ITEMNUM(4,17)) || (ItemType == MAKE_ITEMNUM(5,9)) )
	{	// Ȧ������������, �巡��ҿ�������
		return FALSE;
	}
	return TRUE;
}


#ifdef ADD_ABILLITY_KUNDUN_01_20040920
// ���� HP ȸ�� �Լ�.
void gObjRefillMonsterHP( LPOBJECTSTRUCT lpMonsterObj, INT iRefillHPSec )
{
	INT iMonsterHP = lpMonsterObj->Life;
	INT	iRefillHP  = 0;

	// 6Į���� ����� ���
	
#ifdef CHANGE_KUNDUN_SETTING_01_20050221
	// �׾� ������ return
	if( lpMonsterObj->Live == 0 )
	{
		return;
	}
#endif
	
	if( lpMonsterObj->Class == 275 )
	{
#ifdef ADD_KUNDUN_CONTROL_20040923
		// ��� Ư���� ��ȸ�� �ð��� ��������
		if( lpMonsterObj->iObjectSecTimer > giKundunRefillHPTime )
		{
			// ��ȸ��
			iRefillHP = rand() % ( giKundunRefillHP > 0 ? giKundunRefillHP : 1 );
			
			lpMonsterObj->iObjectSecTimer = 0;

			KUNDUN_EVENT_LOG.Output("[KUNDUN] HP Log -> [%d]ȸ��  [%d]ȸ���ð�  [%7.0f]����HP", 
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
		// MVP ���� : ��� �ڻ� ������.
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
			LogAddTD( "�ڡ� ũ���̿��� ���� ���� : ��� �ڻ� ������ ���� (%d)", g_CrywolfSync.GetKundunHPRefillState() );
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
						_Notice.SendToUser( n, "����� ������ %7.0f ���ҽ��ϴ�.", lpMonsterObj->Life );
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
// ������ ��� 10000�� �������� ����� ����
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

	// Item Looting ����
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	// iCount ��ŭ ��� ��ġ ����
	
	// ��Ʈ ������
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

		// �׼���Ʈ ������
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
		// �Ϲ� ������
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
			
			// ������Ʈ �������� ��� ��ų�� ������ ������ ���� �ɼǵ��� ���� �����
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
				option1rand = 6;				// �Ϲ����� �ɼ�1 (��ų) ���� Ȯ�� 6/100
				option2rand = 4;				// �Ϲ����� �ɼ�2 (���) ���� Ȯ�� 4/100
				option3rand = (rand()%100);		// �Ϲ����� �ɼ�3 (�߰�����Ʈ) ���� Ȯ�� ����/100
				optionc		= (rand()%3);
				NOption     = 0;
			}
			
			// ��ų������ Ȯ���� ���� �����.
			if((rand()%100) < option1rand ) Option1 = 1;
			if((rand()%100) < option2rand ) Option2 = 1;
			
			switch( optionc )
			{
			case 0 :
				if( option3rand < 4 ) Option3 = 3;	// �ɼ�
				break;
			case 1 :
				if( option3rand < 8 ) Option3 = 2;	// �ɼ�
				break;
			case 2 :
				if( option3rand < 12 ) Option3 = 1;	// �ɼ�
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

// ���Ͱ� �׾ �������� �����.??
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
	
#ifdef CHN_PLAYTIME_LIMIT_SYSTEM_20050818	// �߱�����> . ��ü ���� ������ ��� Ȯ���� ������.
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

		if (rand()%100 >= iItemDrapRate) {	// �߱��߰�> . �� ��Ӿ׼��� 50%

			// ���� ����Ʈ����.	
			if(rand()%2) {
				int x, y;
				float money;
				
				x    = lpObj->X;
				y    = lpObj->Y;
				
				money = (float)lpObj->Money;
				
				// ���� �׿����� ��µ�
				money += money/100*(lpTargetObj->MonsterDieGetMoney*(iItemDrapRate/100));
				money += 7;

#ifdef FOR_NEW_TESTSERVER			
				money *= 3;		// �� ��� 3��
#endif
				
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
				if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
				if( lpObj->MapNumber == 9 )
#endif
				{	// �Ǹ��� ���忡���� ���� �����Ѵ�
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
	
#ifdef VTM_PLAYTIME_LIMIT_SYSTEM_20060626	// ��Ʈ��> . ��ü ���� ������ ��� Ȯ���� ������.
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
		{	// �߱��߰�> . �� ��Ӿ׼��� 50%

			// ���� ����Ʈ����.	
			if(rand()%2) {
				int x, y;
				float money;
				
				x    = lpObj->X;
				y    = lpObj->Y;
				
				money = (float)lpObj->Money;
				
				// ���� �׿����� ��µ�
				money += money/100*(lpTargetObj->MonsterDieGetMoney*(iItemDrapRate/100));
				money += 7;

#ifdef FOR_NEW_TESTSERVER			
				money *= 3;		// �� ��� 3��
#endif
				
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
				if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
				if( lpObj->MapNumber == 9 )
#endif
				{	// �Ǹ��� ���忡���� ���� �����Ѵ�
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
		// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
		int iMaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		if (g_NewPVP.DropItem(lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y, iMaxHitUser))
			return;
	}
#endif // ADD_NEWPVP_PKFIELD
	
#ifdef UPDATE_NEWYEAR_LUCKYBAG_MONSTER_20070131	// �������ָӴ��� ��� ������ �巴
	if( lpObj->Class == MON_NEWYEAR_LUCKYBAG )
	{
		// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404	// ũ���̿��� ��ũ���� ������ ���
	if( lpObj->Class == CRYWOLF_MON_DARKELF )
	{
		if( g_bCrywolfMonsterDarkElfItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iCrywolfMonsterDarkElfItemDropRate )
			{			
				// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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

#ifdef ADD_CRYWOLF_MONSTER_ITEMDROP_20060404		// ũ���̿��� �������� �߰��� ������ ���
	if( lpObj->Class == CRYWOLF_MON_BALGARS )
	{
		if( g_bCrywolfBossMonsterItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iCrywolfBossMonsterItemDropRate )
			{
				// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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

#ifdef KANTURU_MONSTER_ITEMDROP_20060807	// ĭ���� ���� ������ ���
	// ���� ��
	if( lpObj->Class == KANTURU_MONSTER_MAYA_LEFTHAND || lpObj->Class == KANTURU_MONSTER_MAYA_RIGHTHAND )
	{
		if( g_bKanturuMayaHandItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iKanturuMayaHandItemDropRate )
			{			
				// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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

	// ����Ʈ�޾�
	if( lpObj->Class == KANTURU_MONSTER_NIGHTMARE )
	{
		if( g_bKanturuNightmareItemDrop )
		{		
			int ItemDropRate = rand() % 10000;
			if( ItemDropRate <= g_iKanturuNightmareItemDropRate )
			{			
				// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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
				// �������� ���� �������� ���� ���� ū ĳ���Ϳ��� ���޵ȴ�.
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
	
#ifdef CASTLE_NPC_DONT_DROP_ITEM_20050517	// ������ NPC���� �������� ������ �ʴ´�.
	if (lpObj->m_btCsNpcType != CS_NPC_TYPE_NONE)
		return;
#endif

#ifdef ADD_NEW_CASTLE_HUNTZONE_REWARD_01_20050303
	if( lpObj->Class == 295 ) // ������� ������ ������.
	{
		// Item Looting ����
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

			if( i == 0 ) // �Ѱ��� ������ ���
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
	// ����� �׾����� �������� ������.
	if( lpObj->Class == 275 )
	{
		// 0. ����� �׾��ٴ� �α׸� ����Ѵ�.
		LogAddTD("[�ڡ�Kundun EVENT] Kundun die, Killer [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );
		KUNDUN_EVENT_LOG.Output("[�ڡ�Kundun EVENT] Kundun die, Killer [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name );

		// 1. ���� Į����6�� �ִ� ��� �������� �α׷� ����Ѵ�.
		// MODIFY_KUNDUN_LOG_20060201
		// 20060201 �α� �ּ� ó�� (RKFKA)
//		for( int n=ALLOC_USEROBJECTSTART; n<MAX_OBJECT; n++)
//		{
//			if( gObj[n].Connected == 3 && (gObj[n].Type == OBJTYPE_CHARACTER) )
//			{
//				if( gObj[n].MapNumber == MAP_INDEX_KALIMA6 )
//				{
//					LogAddTD("[��Kundun EVENT] In KALIMA(6), User List [%s][%s]", gObj[n].AccountID, gObj[n].Name );
//					KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] In KALIMA(6), User List [%s][%s]", gObj[n].AccountID, gObj[n].Name );
//				}
//			}
//		}


		// Item Looting ����
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
				LogAddTD("[��Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
				KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );

				// MODIFY_KUNDUN_LOG_20060201
				//LogAddTD("[��Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
				//KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
			}
			else
			{
				LogAddTD("[��Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] In KALIMA(7), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );

				// MODIFY_KUNDUN_LOG_20060201
				//LogAddTD("[��Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				//KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] In KALIMA(6), ItemDrop MaxHitUser [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
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
					// Type 1�� ��� ����� ���ΰͿ� ���� ����!
#ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
					MakeRewardSetItem( MaxHitUser, cDropX, cDropY, 1, lpObj->MapNumber );
#else
					MakeRewardSetItem( MaxHitUser, cDropX, cDropY, 1 );
#endif // #ifdef MODIFY_KUNDUN_ITEM_DROP_MAP_20060216
					LogAddTD("[��Kundun EVENT] Drop SetItem ");
					KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] Drop SetItem ");
					continue;
				}
			}
			
			// ��� (Į����7) �� ���� ������ �������� ��ӵȴ�.
			if( CHECK_LIMIT(MaxHitUser, MAX_OBJECT) )
			{
				// ADD_KUNDUN_ITEM_DROP_LOG_20060309
				LogAddTD("[��Kundun EVENT] Drop Item [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );				
				KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] Drop Item [%d][%s][%s]", i, gObj[MaxHitUser].AccountID, gObj[MaxHitUser].Name );
			}
			else
			{
				// ADD_KUNDUN_ITEM_DROP_LOG_20060309
				LogAddTD("[��Kundun EVENT] Drop Item [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
				KUNDUN_EVENT_LOG.Output("[��Kundun EVENT] Drop Item [%d][%s][%s]", i, lpTargetObj->AccountID, lpTargetObj->Name );
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
	
	if( lpObj->m_RecallMon >= 0 )						// ��ȯ ���ʹ� �������̳� ���� ������ �ʴ´�.
	{	
		
		return;
	}

#ifdef FOR_BLOODCASTLE
	if(lpObj->Class == 131)								// ������ ��쿡�� �ƹ��͵� ������ �ʴ´�.
	{
		return;
	}

	if (CHECK_LIMIT(lpObj->Class-BC_SAINT_STATUE_1, 3))	// �����Ǽ��� �ø��� �϶��� �ƹ��͵� ������ �ʴ´�. (=> ������ �� ������.)
	{
		return;
	}
#endif

#ifdef ADD_RAKLION_20080408								// ��Ŭ���� �Ź̾��� ������ ����� ���� �ʴ´�
	if( lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 )
	{
		return;
	}
#endif // ADD_RAKLION_20080408
	

#ifdef CHAOSCASTLE_SYSTEM_20040408						// ���Ͱ� �׾ �������� ����Ѵ� -> ī���� ĳ���� ���ʹ� ������ �����۸� ����Ѵ�.
	if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) 
	{
#ifdef ADD_CHAOSCASTLE_FOR_MASTER_LEVEL_20071010	// ī����ĳ�� ��ȣ ���ϱ�
		g_ChaosCastle.SearchNDropMonsterItem ( g_ChaosCastle.GetChaosCastleByMapNum( lpObj->MapNumber ), lpObj->m_Index, lpTargetObj->m_Index );
#else
		g_ChaosCastle.SearchNDropMonsterItem (lpObj->MapNumber-MAP_INDEX_CHAOSCASTLE1, lpObj->m_Index, lpTargetObj->m_Index);
#endif
		// ���⿡�� ���� ��Ű�� ���� �����.
		gObjDel (lpObj->m_Index);
		
#ifdef FOR_KOREA		
		// Ư�� Ȯ���� ���� ī����ĳ�� �������� ��ǰ�� ��÷�� �� �ִ�.
		g_ChaosCastle.CheckRegisterOfflineGift(lpTargetObj->m_Index);
#endif
		return;
	}

#ifdef MU_CHN_OFFLINE_GIFT_EVENT_20040908				// �߱� ��ǰ �̺�Ʈ (��� ���̳� ���� �� �ִ�.)
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
	
#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ȯ�� ������� ���� ������ ���
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
	{
		int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );	
		
		g_IllusionTempleEvent.MonsterDieItemDrop(lpTargetObj);
		return;
	}
#endif
	
#ifdef CHUSEOK_MONSTER_EVENT_20070823	// �߼� �����̺�Ʈ - ���䳢 ������ ���
	if( lpObj->Class == 413 )
	{
		ChuseokMonsterEventItemBagOpen( lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y );		
		return;	
	}
#endif

#ifdef ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610	// ���� �Ҳ� ���� �̺�Ʈ
	if( lpObj->Class == 463 )	// �Ҳ� ����
	{
		FireCrackerMonsterEventItemBagOpen( lpTargetObj, lpObj->MapNumber, lpObj->X, lpObj->Y );		
		return;	
	}
#endif // ADD_SUMMER_FIRECRACKER_MONSTER_EVENT_20090610

	int itemrate=lpObj->m_ItemRate;
	int moneyrate=lpObj->m_MoneyRate;

	if( itemrate < 1 ) itemrate = 1;
	if( moneyrate < 1 ) moneyrate = 1;

	if( lpObj->Class == 44 )	// ȯ�� �巡�� �� ���
	{
		
#ifdef	MODIFY_JEWELOFBLESS_SELL_BUGFIX
		dur = 0;
#else
		dur  = 255;
#endif
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 0;
		if( (rand()%4) > 0 )	// 1/5 �� ������ ���� Ȱ��
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
	else if( lpObj->Class == 43 )	// Ȳ�ݹ����巡�� �� ���
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
	{	// Ÿ��ź
#ifdef ELEGORADO_EVENT
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 9;	// �������� ����
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
	{	// ����ŷ
		if( AttackEvent55BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
	}
#ifdef ELEGORADO_EVENT
	else if( lpObj->Class == 78 )
	{	// Ȳ�� ���		
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
	{	// ��� ������	
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
	{	// ���� ���ڵ�ŷ
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
	{	// ĭ���� 1		
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
		
		// Ȳ�ݹ����巡��, Ȳ�ݰ�� , Ȳ���䳢, Ȳ�ݴ�ũ����Ʈ (Ȳ��Ÿ��ź�� �Ʒ��� ����)
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
	{	// Ÿ��ź, Ȳ�� ���� 
#ifdef ELEGORADO_EVENT
		dur  = 255;
		x    = lpObj->X;
		y    = lpObj->Y;
		level= 8;	// �������� ����
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
	{	// ���� ���ڵ�ŷ , Ȳ�ݺ��ĸ� , Ȳ�� ����
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
	{	// Ȳ��źŻ�ν� , Ȳ���� , Ȳ�ݵ�����, Ȳ��ũ����Ʈ
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
		// Ȳ�� ��Ƽ�ν�, Ȳ��Ʈ������
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
		//Ȳ�ݾ��̾���Ʈ, Ȳ�ݳ�����
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
	{	// ��� ������	
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
		// �׷���Ʈ Ȳ�� �巡�� 
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
	{	// ����ŷ
		if( AttackEvent55BagOpen(lpObj) == TRUE ) return;
		itemrate = 1;
	}
#endif //ADD_GOLDEN_EVENT_RENEWAL_20090311


	// �̺�Ʈ ������ ����� �����Ѵ�
	if( gEventMonsterItemDrop(lpObj, lpTargetObj) )
	{		
		return;	
	}


#ifdef HAPPY_POUCH	//happycat@20050201 (���ָӴ�) - ���� �� �̺�Ʈ
	CMobDropItemInfo ItemInfo;

	if (g_GameEvent.GetEventItem(lpObj->MapNumber, ItemInfo))
	{
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, 
			ItemInfo.m_wType, ItemInfo.m_level, ItemInfo.m_fDurability, ItemInfo.m_option1, ItemInfo.m_option2, ItemInfo.m_option3, gObjMonsterTopHitDamageUser(lpObj));
		return; // apple��20050203
	}

#endif

#ifdef NEW_SKILL_FORSKYLAND
	// ����Ʈ ������ ����� �����Ѵ�
	if( g_QuestInfo.MonsterItemDrop(lpObj) )
	{		
		return;	
	}
#endif

	// ���� 20 �����̸� ��� ��� Ȯ���� ��������.
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
	// 1200->2000���� ���� - 2004.09.08 (apple)
	ExtDropPer = rand()%lpTargetObj->m_wExcellentDropRate;
#endif
	

#ifdef FOR_BLOODCASTLE
	// ����ĳ�� ������ ��� ���ο� ���� ������� �����Ѵ�.
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
#ifdef BUGFIX_BUFF_EFFECT_SYSTEM_FOR_PCBANG_ITEM_20071205	// PC�� ������ ȹ�� Ȯ�� ����
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
	
#ifdef PCBANG_POINT_SYSTEM_20070206		// ������ ȹ�� Ȯ�� ����
	// PC�� ����Ʈ ������ ȹ��Ȯ���� ����� ���� ó��
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
	
	// 1/100 �� ������Ʈ �������� ����Ʈ����.
	if( ExtDropPer == 1 )
	{
		
#ifdef UPGRADE_GAMESERVER_01_20041006
		DropItem = g_MonsterItemMng.GetItem(lpObj->Level-25);

#ifdef MODIFY_380_EXCELENT_ITEM_DISABLE_20080212	// 380 ������ �� �ȶ�������
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
			{	// ������ �����۵��� ������Ʈ �������� ������ Ȯ���� �� ���� �Ѵ�
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
		
		// ���� â���� ������Ʈ �������� ������.
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
		{	// ������ �����۵��� ������Ʈ �������� ������ Ȯ���� �� ���� �Ѵ�
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

	// �������� ������ ������ ��� ����.
	if( item_drop )
	{
		if( DropItem->IsItem()==FALSE ) item_drop = 0;

		if( (DropItem->m_Type >= ITEM_ETC) && (DropItem->m_Type <= ITEM_ETC + MAX_ITEM_INDEX)) {
			INT I = 0;
		}
				
#ifdef NEW_FORSKYLAND2
		// ���������� ��� ������ ����(���� �幰��)
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
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
	// ���� �������� ��Ŭ���� �ʿ����� ��� �ǵ��� ����
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
		{	// ��ũ�� ������ ��ī�罺�� �ƴѰ��̸� �������� �ʴ´�
			item_drop = 0;
		}
#endif

#ifdef MU_CRYWOLF_PENALTY_20051215
		// MVP ���� : ���� ����� �϶�.
		if( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_STATE_OCCUPIED 
			&& g_iCrywolfApplyMvpPenalty
		  )
		{
			if(	DropItem->m_Type == MAKE_ITEMNUM(14, 13)	// �༮
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 14)	// ����
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 16)	// ����
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 22)	// â��
				|| DropItem->m_Type == MAKE_ITEMNUM(12, 15)	// ȥ��
				|| DropItem->m_Type == MAKE_ITEMNUM(14, 31)	// ��ȣ ����) 
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
			// ������Ʈ �������� ��� �������� ������ 0���� �Ѵ�.
			// DropItem->m_Level������ dur�� ���ϱ� ������ ���� ������ �� ���̰� ����.
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
		

		// ������Ʈ �������� ��� ��ų�� ������ ������ ���� �ɼǵ��� ���� �����
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
			option1rand = 6;				// �Ϲ����� �ɼ�1 (��ų) ���� Ȯ�� 6/100
			option2rand = 4;				// �Ϲ����� �ɼ�2 (���) ���� Ȯ�� 4/100
			option3rand = (rand()%100);		// �Ϲ����� �ɼ�3 (�߰�����Ʈ) ���� Ȯ�� ����/100
			optionc		= (rand()%3);
			NOption     = 0;
		}

		// ��ų������ Ȯ���� ���� �����.
		if((rand()%100) < option1rand ) Option1 = 1;
		if((rand()%100) < option2rand ) Option2 = 1;

		switch( optionc )
		{
		case 0 :
			if( option3rand < 4 ) Option3 = 3;	// �ɼ�
			break;
		case 1 :
			if( option3rand < 8 ) Option3 = 2;	// �ɼ�
			break;
		case 2 :
			if( option3rand < 12 ) Option3 = 1;	// �ɼ�
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
		
		// ����/�༮/ȥ�� ���� �ɼ��̳� ��ų���� �Ⱥٰ�..
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
			// ���� ����Ʈ����.			
			int x, y;
			float money;
			
			x    = lpObj->X;
			y    = lpObj->Y;
			
			money = (float)lpObj->Money;
			
#ifdef UPDATE_ZEN_DROP_DOWN_20080806	// ���� �׿��� �� ����� (����ġ / 2)
#ifndef FOR_NEW_TESTSERVER			
			money /= 2;
#endif
			// ���� �׿����� ��µ�
			money += money/100*lpTargetObj->MonsterDieGetMoney;
#else	// UPDATE_ZEN_DROP_DOWN_20080806
			// ���� �׿����� ��µ�
			money += money/100*lpTargetObj->MonsterDieGetMoney;

			money += 7;
#endif	// UPDATE_ZEN_DROP_DOWN_20080806

#ifdef FOR_NEW_TESTSERVER			
//			money *= 3;		// �� ��� 3��
#endif
			
#ifdef ADD_ZEN_OPTION_20070903	// �߰� ��
			money *= gAddZen;
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122		// �нú� 1�� - �� ����
			if( lpTargetObj->m_MPSkillOpt.iMpsPlusZen > 0 )
			{
				// ����� (����ġ / 2) ������ ��ų�� ���� (20081107 - grooving)
				money += ( money * lpTargetObj->m_MPSkillOpt.iMpsPlusZen ) / 100;
			}
#endif
			
#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
			if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
			if( lpObj->MapNumber == 9 )
#endif
			{	// �Ǹ��� ���忡���� ���� �����Ѵ�
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

#ifdef DEVILSQUARE_EXTEND_20050221		// ���������� �ʹ�ȣ üũ ����
	if( CHECK_DEVILSQUARE(lpObj->MapNumber) )
#else
	if( lpObj->MapNumber == 9 )
#endif
		return;
		
	// 400���� 1�� ���� 4�� ���±�� ����Ʈ����.
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
				
				// �������� ������ 
				dur		= 0;
				x		= lpObj->X;
				y		= lpObj->Y;
				level	= 1;
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, 0, 0, 0);
#ifdef REMOVE_LOVELOVE_EVENT_COUPLING_WITH_JOINSERVER_20060319
				/*
				 *	���귯�� �̺�Ʈ�� ������������ Ŀ�ø� ����
				 *		< commonserver.cfg >
				 *		EVENT1						= 0	; 0 : ���귯�� �̺�Ʈ ���� 1:  ���귯�� �̺�Ʈ ��
				 *		Event1ItemDropTodayMax		= 0	; ���� �̺�Ʈ�� ������ ������ ��
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
	// �渱 ��� ������ ���
	if( g_bFenrirStuffItemDrop )
	{
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;

		BOOL bDropStuff = FALSE;

		// 1. > ���� ���� ���
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

		// 2. > ������ ��ȣ ���
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

		// 3. > �ͼ��� ���� ���
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
				iType		=ItemGetNumberMake(14,38);		// ���� ���� ����
			}
		}
		
		if ( bDropStuff == FALSE && lpObj->Level >= g_iCompoundPotionLv2DropLevel )
		{
			if( (rand()%10000) <= g_iCompoundPotionLv2DropRate )
			{
				bDropStuff = TRUE;
				iType		=ItemGetNumberMake(14,39);		// �߰� ���� ����
			}
		}

		if ( bDropStuff == FALSE && lpObj->Level >= g_iCompoundPotionLv3DropLevel )
		{
			if( (rand()%10000) <= g_iCompoundPotionLv3DropRate )
			{
				bDropStuff = TRUE;
				iType		=ItemGetNumberMake(14,40);		// ū ���� ����
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
		// ���� ���� ���
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

				dur  = (float)100+(rand()%100);	// ������ ����
				x    = lpObj->X;
				y    = lpObj->Y;
				level= callbead_level;			// ���� ���� ����
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
		// ���� �̺�Ʈ�� �������϶�
		if( gLanguage == 0 )
		{	
			// �ѱ��϶��� �ٸ��� ó�� �ȴ�

			if( lpObj->MapNumber < 7 )
			{	
				// ��Ʋ���� ���Ͽ����� ���´�
				LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
				if( lpattr )
				{
					if( lpattr->m_Level >= 17 )
					{	
						// ���̾�Ʈ �̻��϶���
						if( (rand()%10000) < gFireCrackerDropRate )
						{
							dur  = 255;
							x    = lpObj->X;
							y    = lpObj->Y;
							level= 2;	// ����
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
				{	// ��Ƽ�� �ξ������� 
					LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);	

					if( lpattr )
					{
						if( lpattr->m_Level >= 17 )
						{	// ���̾�Ʈ �̻��϶���
							if( (rand()%10000) < gFireCrackerDropRate )
							{
								dur  = 255;
								x    = lpObj->X;
								y    = lpObj->Y;
								level= 2;	// ����
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
		// ũ�������� �̺�Ʈ �϶���
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
			{	// ����ƽ�������
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
				{	// ���̾�Ʈ �̻��϶���
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
		// ���� �̺�Ʈ�� �������϶�		
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
						level= 3;	// ����� ��Ʈ
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
				// ��Ʋ���� ���Ͽ����� ���´�
				LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);
				if( lpattr )
				{
					if( lpattr->m_Level >= 17 )
					{	
						// ���̾�Ʈ �̻��϶���
						if( (rand()%10000) < gHeartOfLoveDropRate )
						{
							dur  = 255;
							x    = lpObj->X;
							y    = lpObj->Y;
							level= 3;	// ����� ��Ʈ
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
		// �޴� �̺�Ʈ�� �������϶�
		if( lpObj->MapNumber == 1 || lpObj->MapNumber == 2 )
		{	// ����ƽ� �� ���������� ���޴��� ���;� �ȴ�			
			if( (rand()%10000) < gSilverMedalDropRate )
			{
				dur  = 255;
				x    = lpObj->X;
				y    = lpObj->Y;
				level= 5;	// ���޴�
				type = ItemGetNumberMake(14, 11);			
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}			
		}
		else if( lpObj->MapNumber == 4 || lpObj->MapNumber == 7 || lpObj->MapNumber == 8 )
		{	// �ν�ƮŸ��, ��Ʋ����, Ÿ��ĭ������ �ݸ޴��� ���;� �ȴ�
			if( (rand()%10000) < gGoldMedalDropRate )
			{
				dur  = 255;
				x    = lpObj->X;
				y    = lpObj->Y;
				level= 6;	// �ݸ޴�
				type = ItemGetNumberMake(14, 11);			
				int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
				ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
				return TRUE;
			}			
		}
	}	

	if( gEventChipEvent )
	{	// �̺�ƮĨ �̺�Ʈ�� �������϶�				
		if( (rand()%10000) < gBoxOfGoldDropRate )
		{
			dur  = 255;
			x    = lpObj->X;
			y    = lpObj->Y;
			level= 7;	// Ȳ���� ����
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
	// ����� ǥ�� ���ó��
	if (rand()%10000 < g_iKundunMarkDropRate) 
	{
		
#ifndef HIDDEN_KALIMA_20050706
	// ����� ǥ�� ������� ���� ���� - hnine
	#ifndef UPDATE_LOWLEVEL_SUPPORT_1_20051215
		if (CHECK_KALIMAMAP(lpObj->MapNumber)) 
		{
			// Į���� ���ο����� ����� ǥ���� �������� �ʴ´�.
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
		// ����� ǥ�� +7�� ��� ������ ������ ���� : ���� 115�̻�, ������ - hnine
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
		
		type = ItemGetNumberMake(14, 29);						// ����� ǥ��
		int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
		return TRUE;
	}
#endif
	

#ifdef MU_CASTLESIEGE_CS_MARKS_20041214							// ������ ǥ�� ���ó��
	if (rand()%10000 < g_iMarkOfTheLord) {
		Option1 = 0;
		Option2 = 0;
		Option3 = 0;
		level	= 3;
		dur		= 1;
		x		= lpObj->X;
		y		= lpObj->Y;

		type = ItemGetNumberMake(14, 21);						// ������ ǥ�� (���� +3)
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

			// �Ϻ� 1�ֳ� ��� ���׻��ڴ� �ʿ� ���� �ٸ� ������ ��������.
			switch(lpObj->MapNumber) {
			case 0 :		// �η��þ�		(+1)
				{
					level= 1;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 1 :		// ����			(+2)
				{
					level= 2;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 2 :		// ����ƽ�		(+2)
				{
					level= 2;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 3 :		// �븮��		(+1)
				{
					level= 1;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 4 :		// �ν�ƮŸ��	(+3)
				{
					level= 3;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 7 :		// ��Ʋ����		(+4)
				{
					level= 4;
					int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
					ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, MaxHitUser);
					return TRUE;
				}
				return TRUE;
			case 8 :		// Ÿ��ĭ		(+5)
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
		// ������ �Ҹ� ��� 
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

		// ��ũȣ���� ��ȥ ���
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

		// ��ũ���Ǹ��� ��ȥ ���
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
	// ��ȣ���� ���.
	if( gIsDropGemOfDefend )
	{
		if( ( lpObj->MapNumber != MAP_INDEX_KALIMA7 ) && ( lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		{	// ��ȣ�� ������ Į����7 �� �� ����Ϳ����� ��������.
			
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
	// ���������Ʈ
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

	// ��ȭ�� ����
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

#ifdef ADD_THIRD_WING_20070525	// �ܵ����ǺҲ� ��� ó��
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
		// �÷��� �������� ���� �� �ִ�.
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


#ifdef HIDDEN_TREASUREBOX_EVENT_20050804	// �ź��� ���� ���ó��
	if( lpObj->MapNumber == 33 ||		// ���̴�
		lpObj->MapNumber == 34			// ũ���̿��� 1����
		)
	{
		bool bDropMysteriousBead = false;
		switch(lpObj->Class) 
		{
		case 304:	// ��ġ��
		case 305:	// ����
		case 306:	// �������̴�
		case 307:	// ������Ʈ��ũ
		case 308:	// ����Ʈ��
		case 309:	// �︶�̳�
			{
				if (rand()%10000 < g_iMysteriousBeadDropRate1) 
				{
					bDropMysteriousBead = true;	
				}
			}
			break;
		case 310:	// ô�ĺ�(�ٰŸ�)
		case 311:	// ô�ĺ�(�߰Ÿ�)
		case 312:	// ô�ĺ�(���Ÿ�)
		case 313:	// �������
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

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// ���⼭ �����ͷ��� �̺�Ʈ�� ��������� ��Ḧ ���
	// ���� ���
	if( gDevilSquareEvent )
	{
		if (!CHECK_DEVILSQUARE(lpObj->MapNumber)) 
		{
			// 7 �Ǹ��Ǵ�
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

			// 7 �Ǹ��ǿ���
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

	// ��� ���
	if(g_bIllusionTampleEvent)
	{
		if (!CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) )
		{
			// 6 ���� �η縶��
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

			// 6 ȯ�������� ����
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

	// ��ĳ ���
	if (g_bBloodCastle) 
	{
		if (!CHECK_BLOODCASTLE(lpObj->MapNumber)) 
		{
			// 8 ��õ���� ��
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

			// 8 ���� ��
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

			// 1 . ������ ������ Ȯ��
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
			// 2 . ��õ���� ���� ������ Ȯ��
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
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ��õ���Ǽ� +8 ��� Ȯ��
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

#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// �����ͷ��� �̺�Ʈ�� ���� ó������
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
					
			// 3 . ���庻�� ������ Ȯ��
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
#ifdef ADD_BLOODCASTLE_FOR_MASTER_LEVEL_20071010	// ���庻 +8 ��� Ȯ��
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
				
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// �����ͷ��� �̺�Ʈ�� ���� ó������
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
	

#ifdef DARK_LORD_HEART_EVENT_20040810			// ��ũ�ε��� ���� �̺�Ʈ ������ ���
	if (rand()%10000 < g_iDarkLordHeartDropRate) 
	{
		int iDropRate = 0;

		// �� �ʿ����� ��������.
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
		{	// ��Ƽ�� �ξ������� 
			LPMONSTER_ATTRIBUTE lpattr = gMAttr.GetAttr(lpObj->Class);

			if( lpattr )
			{
				if( lpattr->m_Level >= 17 )
				{
					if( (rand()%10000) < gHeartOfLoveDropRate )
					{
						// ���̾�Ʈ �̻��϶���
						dur  = 255;
						x    = lpObj->X;
						y    = lpObj->Y;
						level= 2;	// ����
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
	if( g_bRibbonBoxEvent )	// ũ�������� �������� �̺�Ʈ�� �������� ��
	{
		// ���� �������� ����ȴ�.		
		if( lpObj->Level >= g_iRedRibbonBoxDropLevelMin && lpObj->Level <= g_iRedRibbonBoxDropLevelMax )
		{
			// ������������
			if( ( rand() % 10000 ) < g_iRedRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 32);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// ������������
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
			// �ʷϸ�������
			if( ( rand() % 10000 ) < g_iGreenRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 33);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// �ʷϸ�������
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
			// �Ķ���������
			if( ( rand() % 10000 ) < g_iBlueRibbonBoxDropRate )
			{
				type	= ItemGetNumberMake(12, 34);
				level	= 0;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// �Ķ���������
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
	if( g_bChocolateBoxEvent )	// �߷�Ÿ�ε��� ���ݸ����� �̺�Ʈ�� �������� ��
	{
		// ���� �������� ����ȴ�.		
		if( lpObj->Level >= g_iPinkChocolateBoxDropLevelMin && lpObj->Level <= g_iPinkChocolateBoxDropLevelMax )
		{
			// ��ȫ���ݸ����� 
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
			// �������ݸ�����
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
			// �Ķ����ݸ�����
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
	if( g_bCandyBoxEvent )	// ȭ��Ʈ���� �������� �̺�Ʈ�� �������� ��
	{
		// ���� �������� ����ȴ�.		
		if( lpObj->Level >= g_iLightPurpleCandyBoxDropLevelMin && lpObj->Level <= g_iLightPurpleCandyBoxDropLevelMax )
		{
			// ������������� 
			if( ( rand() % 10000 ) < g_iLightPurpleCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 32);	
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// �������������
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
			// ��ȫ���ݸ�����
			if( ( rand() % 10000 ) < g_iVermilionCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 33);
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// ��ȫ���ݸ�����
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
			// ��û���ݸ�����
			if( ( rand() % 10000 ) < g_iDeepBlueCandyBoxDropRate )
			{
				type	= ItemGetNumberMake(14, 34);
				level	= 1;
#ifdef MODIFY_BOX_DROP_POSITION_20070308	// ��û���ݸ�����
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

		// ������ ������� ����
		if( lpObj->m_Index == 349 ||		// �߰���
			lpObj->m_Index == 364 ||		// ����
			lpObj->m_Index == 361 ||		// ����Ʈ�޾�
			lpObj->m_Index == 362 ||		// ������ ��
			lpObj->m_Index == 363			// ������ ��
			)
		{
			bIsBossMonster = true;
		}

#ifdef UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026	// ȣ�� ��� ���� ����_item(kor).txt�� ���� ���� ���ͼ� ���� ����
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

		// ������ ������� ����
		if( lpObj->m_Index == 349 ||		// �߰���
			lpObj->m_Index == 364 ||		// ����
			lpObj->m_Index == 361 ||		// ����Ʈ�޾�
			lpObj->m_Index == 362 ||		// ������ ��
			lpObj->m_Index == 363			// ������ ��
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

			// ���� �η縶�� ���� Ȯ��
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
							// 400 ���� ���� ȯ�� ��� ���
							level = 6;
						}
						else
						{
							level = 5;
						}
					}


#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// �����ͷ��� �̺�Ʈ�� ���� ó������
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

			// ȯ�������� ���� ���� Ȯ��
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
							// 400 ���� ���� ȯ�� ��� ���
							level = 6;
						}
						else
						{
							level = 5;
						}
					}
				
#ifdef UPDATE_MASTER_LEVEL_EVENTMAP_TICKET_METERIAL_DROPRATE_20080123	// �����ͷ��� �̺�Ʈ�� ���� ó������
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
		
		// ������ ������� ����
		if( lpObj->m_Index == 349 ||		// �߰���
			lpObj->m_Index == 364 ||		// ����
			lpObj->m_Index == 361 ||		// ����Ʈ�޾�
			lpObj->m_Index == 362 ||		// ������ ��
			lpObj->m_Index == 363			// ������ ��
			)
		{
			bIsBossMonster = true;
		}
		
#ifdef MODIFY_CHERRYBLOSSOM_DROP_VULCANUS_20090324
		if( lpObj->MapNumber != MAP_INDEX_VULCANUS )	// ��ī�뽺�� �ƴϸ� ������� �ʴ´�.
		{
			return FALSE;
		}
#endif // MODIFY_CHERRYBLOSSOM_DROP_VULCANUS_20090324
		
		//#ifdef UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026	// ���� ��� ���� ����_item(kor).txt�� ���� ���� ���ͼ� ���� ����
		//		if( bIsBossMonster == false && ( rand() % 10000 ) < g_bCherryBlossomEventItemDropRate 
		//			&& zzzItemLevel(14, 84, lpObj->Level+1))
		//#else
		if( bIsBossMonster == false && ( rand() % 10000 ) < g_bCherryBlossomEventItemDropRate )
			//#endif	// UPDATE_PUMPKIN_OF_LUCK_DROP_LEVEL_20071026
		{
			type	= ItemGetNumberMake( 14, 84 );	//���ɳ��� ���� //45
			level	= 0;
			x		= lpObj->X;
			y		= lpObj->Y;
			int iMaxHitUser = gObjMonsterTopHitDamageUser( lpObj );
			ItemSerialCreateSend( lpTargetObj->m_Index, lpObj->MapNumber, (BYTE)x, (BYTE)y, type, level, (BYTE)dur, Option1, Option2, Option3, iMaxHitUser );
			return TRUE;
		}
	}
#endif // ADD_EVENT_CHERRYBLOSSOM_20080312

#ifdef UPDATE_LUCKY_COIN_EVENT_20081029							// ����� ���� ���ó��
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

			type = ItemGetNumberMake(14, 100);						// ����� ����
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
	{	// ���ɰ� ������... 
		return;
	}

	TimeCount++;
	if( TimeCount > gQuestNPCTeleportTime )
	{	// �ڷ���Ʈ 		
		TimeCount = 0;		
		
		PMSG_MAGICATTACK_RESULT	pAttack;
	
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof( pAttack));
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
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

		LogAdd("NPC %d�� �̵�", tableindex);			
	}

}
#endif