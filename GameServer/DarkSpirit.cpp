// DarkSpirit.cpp: implementation of the CDarkSpirit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DarkSpirit.h"

#ifdef DARKLORD_WORK

#include "..\\Include\\Public.h"
#include "..\\Include\\ProDef.h"
#include "..\\Include\\SproDef.h"
#include "..\\Include\\GMProtocol.h"
#include "..\\Common\\Winutil.h"
#include "..\\Common\\zzzItem.h"
#include "..\\Common\\Classdef.h"
#include "..\\Include\\ReadScript.h"

#include "User.h"

#include "protocol.h"
#include "BattleGround.h"
#include "Gamemain.h"
#include "gObjMonster.h"
#include "MagicDamage.h"

#include "BattleSoccerManager.h"

#include "ObjAttack.h"

#ifdef DARKLORD_WORK			
#include "PartyClass.h"
extern PartyClass gParty;
#endif

#ifdef FOR_BLOODCASTLE
#include "BloodCastle.h"
#endif

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
#include "Crywolf.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif


CDarkSpirit	gDarkSpirit[MAX_OBJECT];

#ifdef ADD_SHIELD_POINT_01_20060403
extern int g_ShieldSystemOn;
extern int g_iDamageDevideToSDRate;
extern int g_iDamageDevideToHPRate;
extern float g_fSuccessAttackRateOption;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDarkSpirit::CDarkSpirit():m_ActionMode(CDarkSpirit::PetItem_Mode_Normal)
{
	Init();
}

CDarkSpirit::~CDarkSpirit()
{
	
}

void CDarkSpirit::Init()
{
	m_AttackDamageMin = 0;
	m_AttackDamageMax = 0;
	m_AttackSpeed = 0;
	m_SuccessAttackRate = 0;

	m_dwLastAttackTime = 0;	

	m_iMasterIndex = -1;
	m_iTargetIndex = -1;
	m_pPetItem = NULL;
}

void CDarkSpirit::Run()
{
	if( m_iMasterIndex == -1 )
		return;	// �����Ͱ� ������ ó�� ����

	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	if( !m_pPetItem )	// ��������� ����
		return;

	if( m_pPetItem->m_Durability <= 0.f )
		return;		// �׾���.

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if( (attr & MAP_ATTR_SAFTYZONE) )
	{	// ��������...
		return;
	}

#ifdef MODIFY_SETITEM_LEADERSHIP_BUGFIX
	//��� �䱸ġ ����(��ũ���Ǹ���)
	if (m_pPetItem->m_RequireLeaderShip)
	{
		if (lpObj->Leadership+lpObj->AddLeadership < m_pPetItem->m_RequireLeaderShip)
			return;
	}
#endif // MODIFY_SETITEM_LEADERSHIP_BUGFIX

	if( m_dwLastAttackTime > GetTickCount() )
		return;
	
	m_dwLastAttackTime = GetTickCount()+(1500)-(m_AttackSpeed*10);

	switch(m_ActionMode)
	{
		case CDarkSpirit::PetItem_Mode_Normal:			
			ModeNormal();
		break;

		case CDarkSpirit::PetItem_Mode_Attack_Random:			
			ModeAttackRandom();
		break;

		case CDarkSpirit::PetItem_Mode_Attack_WithMaster:			
			ModeAttackWithMaster();
		break;

		case CDarkSpirit::PetItem_Mode_Attack_Target:			
			ModeAttakTarget();
		break;
	}
}

void CDarkSpirit::ModeNormal()
{
	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];
}

void CDarkSpirit::ModeAttackRandom()
{
	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];

	int tObjNum;
	int count = 0;	
	int	FindObj[MAXVIEWPORTOBJECT];
	int FindObjCount = 0;
	int dis;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif
	
	BOOL	EnableAttack;

	int criticaldamage = 0;
	
#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	if( (rand()%100) < lpObj->m_CriticalDamage )	// ũ��Ƽ��(���) ������
		criticaldamage = 1;
	
	if( (rand()%100) < lpObj->m_ExcelentDamage )	// ������Ʈ ������
		criticaldamage = 2;
#else
	if( (rand()%10) < 3 )
	{	// ������ ũ��Ƽ�� ������ ����
		criticaldamage = 1;
	}
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					EnableAttack = FALSE;

					if( gObj[tObjNum].Life > 0 &&  
						(gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110) ) 
					{	// ��� �־�� �ȴ�
						if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
						{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
							EnableAttack = TRUE;
						}
	#ifdef DUEL_SYSTEM_20031028
						else if( DuelIndex == tObjNum )
						{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
							EnableAttack = TRUE;
						}
	#endif
						else
						{	// ����� üũ
							int CallMonIndex = gObj[tObjNum].m_Index;
							if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
							{
								if( gObj[tObjNum].m_RecallMon >= 0 ) 
								{
									CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
								}
							}
							if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
							{	// ����� ��������̸� ���ݰ���
								EnableAttack = TRUE;
							}
						}

						if( EnableAttack )
						{
							if( lpObj->MapNumber == gObj[tObjNum].MapNumber) 
							{
								dis = gObjCalDistance(lpObj, &gObj[tObjNum]);

								if( dis < MAX_DARKSPIRIT_ATTACK_RANGE )
								{
									FindObj[FindObjCount] = tObjNum;
									FindObjCount++;
								}
							}
						}
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}

	if( FindObjCount )
	{
		if( criticaldamage )
		{
			int targetindex = FindObj[rand()%FindObjCount];
			SendAttackMsg(lpObj->m_Index, targetindex, criticaldamage);
#if TESTSERVER == 1
		DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s ��������", gObj[targetindex].Name);
#endif
		}
		else
		{
			int targetindex = FindObj[rand()%FindObjCount];
			RangeAttack(lpObj->m_Index, targetindex);
			
#if TESTSERVER == 1
			DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s ��������", gObj[targetindex].Name);
#endif
		}
		//CDarkSpirit::Attack(lpObj, &gObj[targetindex], NULL, 1);
	}
}

void CDarkSpirit::ModeAttackWithMaster()
{
	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];	
	int count = 0;		
	int dis;

	if( m_iTargetIndex == -1 )
		return;
	
	if( lpObj->MapNumber == gObj[m_iTargetIndex].MapNumber) 
	{
		if( gObj[m_iTargetIndex].Life > 0 )
		{
			dis = gObjCalDistance(lpObj, &gObj[m_iTargetIndex]);
			if( dis < MAX_DARKSPIRIT_ATTACK_RANGE+2 )
			{	
				int criticaldamage = 0;
#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
				if( (rand()%100) < lpObj->m_CriticalDamage )	// ũ��Ƽ��(���) ������
					criticaldamage = 1;
				
				if( (rand()%100) < lpObj->m_ExcelentDamage )	// ������Ʈ ������
					criticaldamage = 2;
#else
				if( (rand()%10) < 3 )
				{	// ������ ũ��Ƽ�� ������ ����
					criticaldamage = 0;
				}
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

				if( criticaldamage )
				{					
					SendAttackMsg(lpObj->m_Index, m_iTargetIndex, criticaldamage);

#if TESTSERVER == 1
					DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s Ÿ��(ĳ��)����", gObj[m_iTargetIndex].Name);
#endif	
				}
				else
				{
					RangeAttack(lpObj->m_Index, m_iTargetIndex);
#if TESTSERVER == 1
					DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s Ÿ��(ĳ��)����", gObj[m_iTargetIndex].Name);
#endif				
				}				
				//CDarkSpirit::Attack(lpObj, &gObj[m_iTargetIndex], NULL, 1);
			}
		}
		else	// ������ Ÿ���� ���ش�
			ReSetTarget(m_iTargetIndex);
	}
}

void CDarkSpirit::ModeAttakTarget()
{
	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];	
	int count = 0;		
	int dis;

	if( m_iTargetIndex == -1 )
		return;
	
	if( lpObj->MapNumber == gObj[m_iTargetIndex].MapNumber) 
	{
		if( gObj[m_iTargetIndex].Life > 0 )
		{
			dis = gObjCalDistance(lpObj, &gObj[m_iTargetIndex]);
			if( dis < MAX_DARKSPIRIT_ATTACK_RANGE+2 )
			{	
				
#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
				// Ÿ�� ���� ������ �׻� ���� �������� ó���Ѵ�.
				int	criticaldamage = 0;
				int iActionType = 1;
				
				if( (rand()%100) < lpObj->m_CriticalDamage )	// ũ��Ƽ��(���) ������
					criticaldamage = 1;
				
				if( (rand()%100) < lpObj->m_ExcelentDamage )	// ������Ʈ ������
					criticaldamage = 2;
#else
	#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
				// Ÿ�� ���� ������ �׻� ���� �������� ó���Ѵ�.
				int	criticaldamage = 1;
				int iActionType = 1;
				
	#else
				int criticaldamage = 0;
				if( (rand()%10) < 3 )
				{	// ������ ũ��Ƽ�� ������ ����
					criticaldamage = 1;
				}
	#endif
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

				if( criticaldamage )
				{			

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
					SendAttackMsg(lpObj->m_Index, m_iTargetIndex, criticaldamage, iActionType);
#else
					SendAttackMsg(lpObj->m_Index, m_iTargetIndex, criticaldamage);
#endif

#if TESTSERVER == 1
					DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s Ÿ��(����)����", gObj[m_iTargetIndex].Name);
#endif	
				}
				else
				{
					RangeAttack(lpObj->m_Index, m_iTargetIndex);
#if TESTSERVER == 1
					DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s Ÿ��(����)����", gObj[m_iTargetIndex].Name);
#endif				
				}
				//CDarkSpirit::Attack(lpObj, &gObj[m_iTargetIndex], NULL, 1);			
#if TESTSERVER == 1
				DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] %s Ÿ��(����)����", gObj[m_iTargetIndex].Name);
#endif
			}
		}
		else
		{	// ����� �׾� ������ Ÿ���� ���ش�
			ReSetTarget(m_iTargetIndex);
		}
	}
}

void CDarkSpirit::RangeAttack(int aIndex, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int StartDis = 1;
	int tObjNum;

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	BOOL	EnableAttack;	
	int		HitCount = 0;

	SendAttackMsg(lpObj->m_Index, aTargetIndex, 0);

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 && aTargetIndex != tObjNum )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}	
				else
				{	// ����� üũ
					int CallMonIndex = gObj[tObjNum].m_Index;
					if( gObj[tObjNum].Type == OBJTYPE_MONSTER )
					{
						if( gObj[tObjNum].m_RecallMon >= 0 ) 
						{
							CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}
					if( gObjTargetGuildWarCheck(lpObj, &gObj[CallMonIndex]) == TRUE )
					{	// ����� ��������̸� ���ݰ���
						EnableAttack = TRUE;
					}
				}

				if( EnableAttack )
				{	
					attackcheck = FALSE;

					if( loopcount == 0 )
					{
						attackcheck = TRUE;
					}
					else
					{
						if( !(rand()%3) )
						{
							attackcheck = TRUE;
						}
					}
					
					if( attackcheck )
					{
						if( gObjCalDistance(&gObj[aTargetIndex], &gObj[tObjNum]) < 4)
						{	
#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
							SendAttackMsg(lpObj->m_Index, tObjNum, 3);
#else
							SendAttackMsg(lpObj->m_Index, tObjNum, 2);
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
							HitCount++;

							if( HitCount > 3)
								break;
							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0);
						}
					}
				}
			}
		}	
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{	
			break;
		}
	}
}

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
void CDarkSpirit::SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType)
#else
void CDarkSpirit::SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage)
#endif
{
	PMSG_PET_ITEM_ATTACK_COMMAND pMsg;

#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	if( criticaldamage != 3 )
#else
	if( criticaldamage != 2 )
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	{
		pMsg.PetType = 0;
		pMsg.SkillType = !criticaldamage;

#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
		if( iActionType == 1 )		// �׼� ��ȣ�� 1�ϰ�쿣 ������ ���ݸ��
			pMsg.SkillType = 0;
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

		pMsg.NumberH = HIBYTE(aIndex);
		pMsg.NumberL = LOBYTE(aIndex);

		pMsg.TargetNumberH = HIBYTE(aTargetIndex);
		pMsg.TargetNumberL = LOBYTE(aTargetIndex);

		pMsg.h.set((LPBYTE)&pMsg, 0xa8, sizeof(pMsg));

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);

#ifndef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
		// 0. ���� ��Ŀ� ���� ������ ������ Ʋ���� �Ѵ�.
		// ActionType �� 1�̸� ���Ǹ��� ���� �뽬 ������ �Ѵ�.
		if( iActionType == 1 )
		{
			criticaldamage = 0;
			if( (rand()%10) < 3 )
			{	// ������ ũ��Ƽ�� ������ ����
				criticaldamage = 1;
			}
		}
#endif
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	}
	else
	{
		criticaldamage = 0;
	}
	
#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
	gObjAddAttackProcMsgSendDelay(&gObj[aIndex], 
								  MSG_ATTACKPROC_DARKSPRITATTACK, 
								  aTargetIndex,
								  600, criticaldamage, iActionType);
#else
	gObjAddAttackProcMsgSendDelay(&gObj[aIndex], 
								  MSG_ATTACKPROC_DARKSPRITATTACK, 
								  aTargetIndex,
								  600, criticaldamage, 0);
#endif

}

void CDarkSpirit::SetTarget(int aTargetIndex)
{
	if( m_iMasterIndex == -1 )
		return;	// �����Ͱ� ������ ó�� ����

	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	m_iTargetIndex = aTargetIndex;
}

void CDarkSpirit::ReSetTarget(int aTargetIndex)
{
	if( m_iMasterIndex == -1 )
		return;	// �����Ͱ� ������ ó�� ����

	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	if( aTargetIndex == m_iTargetIndex )
	{
		m_iTargetIndex = -1;
		if( m_ActionMode == CDarkSpirit::PetItem_Mode_Attack_Target )
			SetMode(CDarkSpirit::PetItem_Mode_Normal);
	}
}

void CDarkSpirit::SetMode(ePetItem_Mode mode, int iTargetindex)
{
	if( m_iMasterIndex == -1 )
		return;	// �����Ͱ� ������ ó�� ����

	LPOBJECTSTRUCT lpObj = &gObj[m_iMasterIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	if( mode == CDarkSpirit::PetItem_Mode_Attack_Target || m_ActionMode != mode  )
	{	// �̹� ���� ����.
		if( mode == CDarkSpirit::PetItem_Mode_Attack_Target )
		{
			if(CHECK_LIMIT(iTargetindex, MAX_OBJECT)) 
			{
				m_iTargetIndex = iTargetindex;
			}
			else
			{
				return;
			}
		}

		m_ActionMode = mode;

		PMSG_SEND_PET_ITEM_COMMAND pMsg;
		pMsg.h.set((LPBYTE)&pMsg, 0xa7, sizeof(pMsg));

		pMsg.PetType = 0;
		pMsg.Command = mode;

		pMsg.NumberH = HIBYTE(m_iTargetIndex);
		pMsg.NumberL = LOBYTE(m_iTargetIndex);

		DataSend(m_iMasterIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		char strMode[][255] = 
		{
			"[�Ϲ�]", 
			"[��������]",
			"[���ΰ����̰���]",
			"[���������]"
		};
#if TESTSERVER == 1
		DebugOutput(lpObj->m_Index, "[��ũ���Ǹ�] ��� ��ȯ %s", strMode[m_ActionMode]);
#endif
	}
}


void CDarkSpirit::Set(int aIndex, CItem* pPetItem)
{
	LPOBJECTSTRUCT lpObj = &gObj[aIndex];
	if(lpObj->Class != CLASS_DARKLORD )
		return;

	BOOL	bChange = FALSE;

	if( m_iMasterIndex == -1 )	// ��� �����ϰ� �ִ� �������� �ƴϸ�
	{
		//CDarkSpirit::m_ActionMode = PetItem_Mode_Normal;	
		bChange = TRUE;
	}

	Init();

 	if( pPetItem->m_Type != MAKE_ITEMNUM(13,5) )	
		return;

	if( pPetItem->m_Durability <= 0.f )
		return;	// ��ũ���Ǹ� �׾���	

	int petitemlevel = pPetItem->m_PetItem_Level;

	int leadership = lpObj->Leadership+lpObj->AddLeadership;

	m_AttackDamageMin = 180+petitemlevel*15+leadership/8;
	m_AttackDamageMax = 200+petitemlevel*15+leadership/4;

	m_AttackSpeed = 20+petitemlevel*4/5+leadership/50;
	
	m_SuccessAttackRate = 1000+petitemlevel+(petitemlevel/15);

	m_iMasterIndex = aIndex;
	m_pPetItem = pPetItem;

	if( bChange )
		SetMode(CDarkSpirit::PetItem_Mode_Normal);

#if TESTSERVER == 1
	DebugOutput(aIndex, "[%s]���� Lv:%d Mi:%d Ma:%d Sp:%d R:%d M:%d", 
						pPetItem->GetName(),				
						pPetItem->m_PetItem_Level, 						
						m_AttackDamageMin,
						m_AttackDamageMax,
						m_AttackSpeed,
						m_SuccessAttackRate,
						m_iMasterIndex);
#endif
}


// ��ũ ���Ǹ� ������ ������ ���� ó���� ó�� ����� ����...

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
BOOL CDarkSpirit::Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int criticaldamage, int iActionType)
#else
BOOL CDarkSpirit::Attack(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf *lpMagic, int criticaldamage)
#endif
{
	int				skillSuccess=0;

	LPOBJECTSTRUCT	lpCallObj;
	LPOBJECTSTRUCT	lpCallTargetObj;
	BYTE			MsgDamage = 0;

	int				ManaChange = FALSE;		

#ifdef ADD_SHIELD_POINT_01_20060403
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;
#endif

	if( (lpTargetObj->Authority&AUTHORITY_ADMIN) == AUTHORITY_ADMIN ) 
	{	// ��� ĳ���� ������ ���� �ʴ´�
		return FALSE;
	}

	if( lpObj->MapNumber != lpTargetObj->MapNumber )
	{	// ���� ���� �ƴϸ�, 
		return FALSE;
	}
	

#ifdef CRYWOLF_MAIN_SCHEDULER_20050505
	// ũ���̿��� �غ�, ���� �ð����� ���Ϳ��� ���ظ� ���� �� ����.
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY 
		|| g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_END )
	{
		if( CHECK_CRYWOLF_FIRSTZONE(lpTargetObj->MapNumber) )
		{
			if( lpTargetObj->Type == OBJTYPE_MONSTER )
				return FALSE;
		}	
	}
#endif

	int skill=0;
	if( lpMagic != NULL )
	{
		skill = lpMagic->m_Skill;
	}

	skillSuccess = 1;

	if( lpObj->GuildNumber > 0 )
	{
		if( lpObj->lpGuild != NULL )
		{
			if( lpObj->lpGuild->WarState && lpObj->lpGuild->WarType == BT_SOCCER )
			{
				if( GetBattleSoccerGoalMove(0) == 0 )
				{
					return TRUE;
				}				
			}
		}
	}

	// �౸�� �� ��쿣 ƨ��� �Ѵ�.
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		if( lpTargetObj->Class == 200 )	
		{
#ifdef FOR_ONAIR
			return TRUE;
#else
			if( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index);
			}
			else gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index);			
			return TRUE;
#endif
		}
	}
		
	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( gObjIsConnected(lpObj) == FALSE ) return FALSE;		
	}
	
	if( lpTargetObj->Type == OBJTYPE_CHARACTER )
	{
		if( gObjIsConnected(lpTargetObj) == FALSE ) return FALSE;
	}
	
	if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_MONSTER)
	{
		// �����ڰ� �÷��̾�� ��밡 ���Ͷ�� ��ȯ������ üũ�Ѵ�.
		// �̶� �ڽ��� ��ȯ�� �����ϰ�� ������ �ȵȴ�.
		if( lpObj->m_RecallMon >= 0 )
		{
			if( lpObj->m_RecallMon == lpTargetObj->m_Index ) return FALSE;
		}
	}
	
	// �����ص� �Ǵ��� �����.
	if( gObjAttackQ(lpTargetObj) == FALSE ) return FALSE;
	
#ifdef CASTLE_MAIN_SCHEDULER_20041111		// �������� ����� ���ݴ��� ���� -> ������ �߿� ���� ���� ���ݹ����� �ʵȴ�.
	if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
		if ((lpObj->m_btCsJoinSide > CS_SIEGESIDE_NONE) &&
			(lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
			) 
		{
			return FALSE;
		}
	}
#endif

	lpObj->m_TotalAttackCount++;


	// ���� ������ üũ �Ѵ�
	if( !CheckAttackArea(lpObj, lpTargetObj) )
		return FALSE;
	
	
	lpCallObj = lpObj;
	
	// ��ȯ������ ��� ���μ���
	lpCallTargetObj = lpTargetObj;
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		if( lpTargetObj->m_RecallMon >= 0 ) 
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	// ������ üũ
	if( !PkCheck(lpCallObj, lpTargetObj) )
		return FALSE;

	int	Strength	= lpObj->Strength+lpObj->AddStrength;
	int	Dexterity	= lpObj->Dexterity+lpObj->AddDexterity;
	int	Vitality	= lpObj->Vitality+lpObj->AddVitality;
	int	Energy		= lpObj->Energy+lpObj->AddEnergy;

#ifdef DUEL_SYSTEM_20031028
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

#ifdef DUEL_SYSTEM_FIX_20031224		// �ο�� ������ ���� ���� ���̶�� ����ī��Ʈ�� �����Ѵ�.
	if (bIsOnDuel) {
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}
#endif
#endif

	int MSBFlag = 0;
	
#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	if( criticaldamage == 1 )
		MsgDamage = DT_CRITICAL;
	else if( criticaldamage == 2 )
		MsgDamage = DT_EXCELLENT;
#else
	if( criticaldamage )
		MsgDamage = DT_CRITICAL;
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	else
		MsgDamage = 0;
	
	// �������� ���ݵ����� ���� ��´�.
#ifdef NEW_FORSKYLAND3
	int bAllMiss = FALSE;
#endif	

	int AttackDamage = 0;

	// �̽� üũ
#ifdef ADD_SHIELD_POINT_01_20060403
	// CODEREVIEW_SHIELD : ���� �ý��� ON/OFF üũ �߰�
	// CODEREVIEW_SHIELD2 : 
	if( g_ShieldSystemOn == TRUE )
	{
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			if( !MissCheckPvP( lpObj, lpTargetObj, skill, skillSuccess, bAllMiss ) )
			{
				return FALSE;
			}
		}
		else
		{
			if( !MissCheck( lpObj, lpTargetObj, skill, skillSuccess, bAllMiss ) )
			{
				return FALSE;
			}
		}
	}
	else
	{
		if( !MissCheck( lpObj, lpTargetObj, skill, skillSuccess, bAllMiss ) )
		{
			return FALSE;
		}
	}
#else
	if( !MissCheck(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
		return FALSE;
#endif

	// ����� ������ ��´�
#ifdef ADD_380ITEM_NEWOPTION_20060711	
	int targetdefense = GetTargetDefense(lpObj, lpTargetObj, MsgDamage);
#else
	int skilldefense = lpTargetObj->m_SkillDefense;
	int targetdefense = lpTargetObj->m_Defense+skilldefense;
	targetdefense -= targetdefense*lpTargetObj->m_SkillMagumReduceDefense/100;
#endif // ADD_380ITEM_NEWOPTION_20060711
	

	//int targetdefense = GetTargetDefense(lpObj, lpTargetObj, MsgDamage);		
	
#ifdef DUEL_SYSTEM_20031028		
	AttackDamage = GetAttackDamage(lpObj, targetdefense, bIsOnDuel, criticaldamage);
#else		
	AttackDamage = GetAttackDamage(lpObj, targetdefense, criticaldamage);	
#endif	

	//- ��ũ ���Ǹ� ��ü ������ �� ������ �ش�.(apple)
	// ActionType == 0 : ��ü����
	// ActionType == 1 : 

#ifdef MODIFY_DARKSPIRIT_TAGET_ATTACK_01_20040906
	if( !criticaldamage && !iActionType )
		AttackDamage = AttackDamage/(1.5);	
#else
	if( !criticaldamage )
		AttackDamage = AttackDamage/(1.5);	
#endif
	

#ifdef NEW_FORSKYLAND3
	if( bAllMiss )
	{
#ifdef MODIFY_SUCCESSBLOCKING_ALLMISS_20040408
		AttackDamage =  AttackDamage*30/100;
#else
		AttackDamage =  AttackDamage*20/100;
#endif
	}
#endif

	// �������� ���� ��Ų��.
	if( lpTargetObj->DamageMinus )
	{
		int beforeDamage=AttackDamage;
		AttackDamage -=  AttackDamage*lpTargetObj->DamageMinus/100;
		//if( AttackDamage < 0 ) AttackDamage = 0;
		//LogAdd("������ ���� : %d -> %d (%d)",beforeDamage, AttackDamage,lpTargetObj->DamageMinus);
	}

	// �����༮�Ƿ���/10 ���� ���� ��� AttackDamage ���� 
	int tlevel = lpObj->Level/10;
	if( AttackDamage < tlevel )
	{
		if( tlevel < 1 ) tlevel = 1;
		AttackDamage = tlevel;
	}

	// ��ų ����
	if( lpTargetObj->m_SkillNumber == AT_SKILL_BLOCKING )
		if( AttackDamage > 1 ) AttackDamage >>= 1;

	// ��ũ���Ǹ� ������ ����
	lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_DurabilitySmall += (AttackDamage*2)/100;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �� ���� �Ҹ� �ӵ� ���� (��ũ���Ǹ�)
	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_DurabilitySmall >= 800 + lpObj->m_MPSkillOpt.iMpsPetDurDownSpeed)
#else // UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_DurabilitySmall >= 800 )
#endif // UPDATE_MASTER_LEVEL_4RANK_20080428
	{
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_DurabilitySmall = 0;
		lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Durability--;
		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Durability < 1 )	
		{
			lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Durability = 0.f;			
			int	exp = lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp;
			if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].DecPetItemExp(10) )	// ����ġ 10% ����
			{	// ����ġ ���ҿ� ���� ���� �ٿ��� �����
				LogAddTD("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]", 
						lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(),
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level,
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp,
						exp-lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp);
				
				Set(lpObj->m_Index, &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT]);		
				CDarkSpirit::SendLevelmsg(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, 0, 255);
			}

#if TESTSERVER == 1
			DebugOutput(lpObj->m_Index, "[%s] LevelDown Lv:%d Exp:%d", 
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].GetName(),				
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Level, 
						lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_PetItem_Exp);
#endif

#ifdef MODIFY_LOG_SYSTEM_2_3_20060828
	#ifdef MODIFY_ITEM_SERIAL_EXPAND_20080227
				LogAddTD("[%s][%s] ��ũ���Ǹ��ǹ��� Item is Broken because durability is exhausted [%u]",
	#else
				LogAddTD("[%s][%s] ��ũ���Ǹ��ǹ��� Item is Broken because durability is exhausted [%d]",
	#endif // MODIFY_ITEM_SERIAL_EXPAND_20080227
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Number );
#else
	#ifdef EXTEND_LOG_SYSTEM_03_20060816
				LogAddTD("��ũ���Ǹ��ǹ��� Item is Broken because durability is exhausted");
	#endif // EXTEND_LOG_SYSTEM_03_20060816
#endif // MODIFY_LOG_SYSTEM_2_3_20060828
		}
		GCItemDurSend(lpObj->m_Index, EQUIPMENT_WEAPON_LEFT, (BYTE)lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Durability, 0);		
	}
	
	// ��밡 õ�������� ������ �ִٸ�.. 
	if( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if( AttackDamage > 1 ) 
		{
#ifdef NEW_FORSKYLAND3	// ����õ�� ������ ��� 30%���� 20%�� ����
			float damage = (float)(AttackDamage*8)/10;
#else
			float damage = (float)(AttackDamage*7)/10;
#endif
			AttackDamage = (int)damage;// 10���� 7�� ����ڰ�..  
		}
	}

	if( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem	*Wing = &lpTargetObj->pInventory[EQUIPMENT_WING];
		if( AttackDamage > 1 ) 
		{
#ifdef ADD_THIRD_WING_20070525	// 3������ ������ ��� ����
				if( Wing->m_Type == MAKE_ITEMNUM( 12, 36 )		// ��ǳ�ǳ���
					|| Wing->m_Type == MAKE_ITEMNUM( 12, 37 )	// �ð��ǳ���  
					|| Wing->m_Type == MAKE_ITEMNUM( 12, 38 )	// ȯ���ǳ���
					|| Wing->m_Type == MAKE_ITEMNUM( 12, 39 )	// �ĸ��ǳ���
					)
				{
					// 39% + ( level * 2 )
					float damage = ( float )( AttackDamage * ( 61 - ( Wing->m_Level * 2 )) ) / 100;
					AttackDamage = ( int )damage;
				}
				else if( Wing->m_Type == MAKE_ITEMNUM( 12, 40 ) )	// �����Ǹ���
				{
					// 24% + ( level * 2 )
					float damage = ( float )( AttackDamage * ( 76 - ( Wing->m_Level * 2 )) ) / 100;
					AttackDamage = ( int )damage;
				}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// �ű�ĳ���� 1������(����� ����) ��������� 12% + level*2
				else if( Wing->m_Type == MAKE_ITEMNUM( 12, 41 ) )	// �ű�ĳ���� 1�� ����(����� ����)
				{
					float damage = (float)(AttackDamage*(88-(Wing->m_Level*2)))/100;
					AttackDamage = (int)damage;	
				}
#endif // ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
#ifdef ADD_SUMMONER_SECOND_THIRD_WING_20071130	// ������ ���� ������ ���
				else if( Wing->m_Type == MAKE_ITEMNUM( 12, 42 ) )	// �ű�ĳ���� 2�� ����(������ ����)
				{
					float damage = (float)(AttackDamage*(75-(Wing->m_Level*2)))/100;
					AttackDamage = (int)damage;	
				}
				else if( Wing->m_Type == MAKE_ITEMNUM( 12, 43 ) )	// �ű�ĳ���� 3�� ����(������ ����)
				{
					float damage = (float)(AttackDamage*(61-(Wing->m_Level*2)))/100;
					AttackDamage = (int)damage;	
				}
#endif				
				else
#endif	// ADD_THIRD_WING_20070525			
#ifdef NEW_FORSKYLAND3
				if( Wing->m_Type > MAKE_ITEMNUM(12,2) )
				{	// ���� ������ ��� �߰� ������ 25% + level
					float damage = (float)(AttackDamage*(75-(Wing->m_Level*2)))/100;
					AttackDamage = (int)damage;
				}
				else
#endif			
				{	// ���� ������ ��� ���� 12% + level*2
					float damage = (float)(AttackDamage*(88-(Wing->m_Level*2)))/100;
					AttackDamage = (int)damage;	
				}
			//LogAdd("[%s][%s] ���� ���� ȿ��2 %d", lpObj->AccountID, lpObj->Name, AttackDamage);
		}		
	}

#ifdef NEW_FORSKYLAND3
	if( gObjDenorantSprite(lpTargetObj) )
	{	// ����Ʈ ������ ��� 10%
		CItem	*Dinorant = &lpTargetObj->pInventory[EQUIPMENT_HELPER];

		// 5%�߰� ������ ��� �ɼ� ����
		int	dinorantdecdamage = 90-Dinorant->IsDinorantReduceAttackDamaege();			
		lpObj->Life -= 1;
		
		if( lpObj->Life < 0 ) 
		{
			lpObj->Life = 0;
		}
		else
		{
			AttackDamage = AttackDamage*dinorantdecdamage/100;
		}
		
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0, (WORD)lpObj->iShield);
#else
		GCReFillSend(lpObj->m_Index, (WORD)lpObj->Life, 0xFF, 0);
#endif
		
	}
#endif

#ifdef DARKLORD_WORK
	if( gObjDarkHorse(lpTargetObj) )
	{	// ��ũȣ�� ������ ��� (10+(���/100))
		CItem	*Darkhorse = &lpTargetObj->pInventory[EQUIPMENT_HELPER];	

		int	decdamage = 100-((30+Darkhorse->m_PetItem_Level)/2);
		

		//int	decdamage = (90-(lpTargetObj->Leadership+lpTargetObj->AddLeadership)/200);
		lpTargetObj->Life -= 1;
		
		if( lpTargetObj->Life < 0 ) 
		{
			lpTargetObj->Life = 0;
		}
		else
		{
			AttackDamage = AttackDamage*decdamage/100;
		}

#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0);
#endif // ADD_SHIELD_POINT_01_20060403

	}
#endif
	
	if( lpTargetObj->Live ) 
	{			
		//#BUGFIX_DARKSPRITE_DINORANT_DAMAGE_20041005
//		if( lpObj->pInventory[EQUIPMENT_HELPER].m_Type == MAKE_ITEMNUM(13,3) )
//		{	// ����Ʈ �Ϲ� ���ݵ� ������ �߰�
//			AttackDamage = AttackDamage*130/100;
//		}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ҿ�ٸ��� : �������� ������ ���(CDarkSpirit)
		if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
		{	// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
			// �������� ������
			int iWizardSkillDefense = 0;
			int iManaRate			= 0;
			gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense, &iManaRate );
			// ���� ����			
			int iReplacementMana = 0;
			if( iManaRate > 0 )
				iReplacementMana = (WORD)lpTargetObj->Mana * iManaRate / 1000;	
			else
				iReplacementMana = (WORD)lpTargetObj->Mana * 2 / 100;	

			if( lpTargetObj->Mana > iReplacementMana )
			{	
				// ��밡���� ������ �־�� �ȴ�
				lpTargetObj->Mana -= iReplacementMana;				

				int decattackdamage = 0;
				if( iWizardSkillDefense > 0 )
					decattackdamage = AttackDamage * iWizardSkillDefense / 100;	

				// ������ ����
				AttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}
		}

#else
		if( ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC ) == true ) && AttackDamage > 0 )
		{	// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
			int replacemana = (WORD)lpTargetObj->Mana*2/100;		// ���� 2%����
			if( lpTargetObj->Mana > replacemana )
			{	// ��밡���� ������ �־�� �ȴ�
				lpTargetObj->Mana -= replacemana;				// �������� ������
				int iWizardSkillDefense = 0;
				gObjGetValueOfBuffIndex( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, &iWizardSkillDefense );
				int decattackdamage = AttackDamage * iWizardSkillDefense / 100;	// ������ ����

				AttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}
		}
#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
#else
		if( lpTargetObj->m_WizardSkillDefense && AttackDamage > 0 )
		{	// ���� �ҿ�ٸ���(�������� ������ ��� ��Ų��)
			int replacemana = (WORD)lpTargetObj->Mana*2/100;		// ���� 2%����
			if( lpTargetObj->Mana > replacemana )
			{	// ��밡���� ������ �־�� �ȴ�
				lpTargetObj->Mana -= replacemana;				// �������� ������
					int decattackdamage = AttackDamage*lpTargetObj->m_WizardSkillDefense/100;	// ������ ����

				AttackDamage -= decattackdamage;
				ManaChange = TRUE;
			}
		}
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�� �ȿ��� ����� ���� ���� �� ������ ����
	#ifdef ADD_SHIELD_POINT_01_20060403
		if( g_ShieldSystemOn == FALSE )
	#endif // ADD_SHIELD_POINT_01_20060403
		{
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber) && CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}
#endif
		
#ifdef CASTLE_MAIN_SCHEDULER_20041111	// ������ �߿� ������ �ȿ����� ����� ���� ���� �� ������ ������ 80% ����, �ٸ��� 60% ����
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
			if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
			{
				if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE) {
					if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide) {
						AttackDamage = AttackDamage * 20 / 100;
					}
					else 
					{
	#ifdef ADD_SHIELD_POINT_01_20060403
						if( g_ShieldSystemOn == FALSE )
	#endif	// ADD_SHIELD_POINT_01_20060403
						{
							AttackDamage = AttackDamage * 40 / 100;
						}
					}
				}
			}
		}
#endif

#ifdef ADD_NEW_ITEM_FOR_CASTLE_01_20041116 // �ູ�� ���� ��� ȿ��!
		if( lpObj->Type == OBJTYPE_CHARACTER 
			&& lpTargetObj->Type == OBJTYPE_MONSTER )
		{
			// ��ȣ ���� ���� ��
			if( lpTargetObj->Class == 283 )
			{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_BLESS_POTION )
					|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION )
				  )
	#else
				if( lpObj->m_iPotionBlessTime > 0 
					|| lpObj->m_iPotionSoulTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				{
					// �߰� ������ ���� ����.
					AttackDamage = AttackDamage + AttackDamage * 20 / 100;
				}
				else
				{
					if( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle( lpObj, lpTargetObj, 1 );	
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					// ������ 20%�� ����
					AttackDamage = AttackDamage * 20 / 100;

				}

			}

			// ���� ���� ��
			if( lpTargetObj->Class == 277 )
			{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_BLESS_POTION )
					|| gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_SOUL_POTION )
				  )
	#else
				if( lpObj->m_iPotionBlessTime > 0 
					|| lpObj->m_iPotionSoulTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				{
					// �߰� ������ ���� ����.
					AttackDamage = AttackDamage + AttackDamage * 20 / 100;
				}
				else
				{
					if( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle( lpObj, lpTargetObj, 1 );	
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					// ������ 20%�� ����
					AttackDamage = AttackDamage * 20 / 100;

				}
			}

		}
#endif
					
#ifdef ADD_SHIELD_POINT_01_20060403
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
			iTempShieldDamage		= GetShieldDamage( lpObj, lpTargetObj, AttackDamage );
			lpTargetObj->iShield	= lpTargetObj->iShield - iTempShieldDamage;
			lpTargetObj->Life		= lpTargetObj->Life - (AttackDamage - iTempShieldDamage);
			iTotalShieldDamage		= iTotalShieldDamage + iTempShieldDamage;

			if( lpTargetObj->Life < 0 )
			{
				lpTargetObj->Life = 0;
			}
		}
		else
		{
			lpTargetObj->Life = lpTargetObj->Life - AttackDamage;

			if( lpTargetObj->Life < 0 )
				lpTargetObj->Life = 0;
		}
#else
		lpTargetObj->Life -= AttackDamage;

		if( lpTargetObj->Life < 0 ) lpTargetObj->Life = 0;
#endif
	}	
	
	// ���ϴ°� �����϶�..
	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		// ���ݴ��ߴ�..
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100);
		
#ifdef FOR_BLOODCASTLE
		lpTargetObj->LastAttackerID = lpObj->m_Index;	// ������ ������ ����� �ε����� ����
#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// ���⼭ Agro�� ���� �����ش�.
		if( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage/100);
		}
#endif
		
	}
	
	BOOL			selfdefense=FALSE;

	lpCallObj = lpTargetObj;

	if( lpTargetObj->Type == OBJTYPE_MONSTER )
	{
		if( lpTargetObj->m_RecallMon >= 0 ) 
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if( AttackDamage >= 1 )
	{	
		// �÷��̾�鳢��..
		// ������ �Ͽ� �����ϸ� ��������� �����Ѵ�.
		if( lpObj->Type	== OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_CHARACTER )
		{
#ifdef DUEL_SYSTEM_20031028
			if (gObjDuelCheck(lpObj, lpTargetObj)) {	// ���� ���� ���̶�� ������� �����ʵ�
				selfdefense = FALSE;
			}
	#ifdef CHAOSCASTLE_SYSTEM_20040408						// ī���� ĳ�� �ȿ����� ��������� �������� �ʴ´�.
			else if (CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_CHAOSCASTLE(lpTargetObj->MapNumber)) {
				selfdefense = FALSE;
			}
	#endif
			else {
				selfdefense = TRUE;
			}
#else
			selfdefense = TRUE;
#endif


#ifdef ADD_NEW_UNION_01_20041006
			// ���� ��� �̸� ���� ���� ������ ���� �ʴ´�.
			// �ƹ����� ���� ����. (���� ���� �϶���..)
			if( gObjGetRelationShip( lpObj, lpTargetObj ) == G_RELATIONSHIP_RIVAL )
			{
				selfdefense = FALSE;
			}
#endif
			
#ifdef CASTLE_MAIN_SCHEDULER_20041111					// ������ �� ������ �������� ������� �����ʵ�
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) {
				if (lpObj->m_btCsJoinSide > CS_SIEGESIDE_NONE) {
					selfdefense	= FALSE;
				}
			}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328	// ��� �ȿ����� ��������� �������� �ʴ´�.
			else if( CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) )
			{
				selfdefense = FALSE;
			}
#endif
		}
		else if( lpTargetObj->Type == OBJTYPE_MONSTER && lpObj->Type == OBJTYPE_CHARACTER)
		{
			if( lpTargetObj->m_RecallMon >= 0 ) selfdefense = TRUE;
		}
		if( lpTargetObj->Type == OBJTYPE_CHARACTER )
		{	// ������ �����ϸ� �������� ���� ��Ų��			
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}

#ifdef NEW_FORSKYLAND2						
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_STONE ) == true )
		{
			gObjRemoveBuffEffect( lpTargetObj, BUFFTYPE_STONE );
			GCMagicCancelSend(lpTargetObj, AT_SKILL_ELFHARDEN);
		}
	#else
		if( lpTargetObj->m_SkillHarden != 0 )
		{	// ������ �������� ��ȭ�� �ɷ� ������, ������ Ǯ�� �ش�

			lpTargetObj->m_SkillHarden--;

			if( lpTargetObj->m_SkillHarden <= 0 )
			{
				lpTargetObj->m_SkillHardenTime = 0;
				lpTargetObj->m_SkillHarden = 0;			 
				lpTargetObj->m_ViewSkillState &= ~STATE_HARDEN;
				GCMagicCancelSend(lpTargetObj, AT_SKILL_ELFHARDEN);
			}			
		}
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
#endif
	}	

#ifdef CHINA_GM_EVENT
		if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
		{
			selfdefense = FALSE;
		}
#endif

#ifdef MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906
		if( lpTargetObj->Authority&AUTHORITY_EVENT_GM )
		{
			selfdefense = FALSE;
		}
#endif // MODIFY_CHN_GMEVENT_TO_TAIWAN_GM_EVENT_20070906		

#ifdef MODIFY_0708_BUFFIX_20070820	// ���� ��Ƽ�������� ������� ���� ����
#ifdef MODIFY_SELFDEFENSE_20070904
	if( lpObj->PartyNumber >= 0 
		&& lpCallObj->PartyNumber >= 0
		&& lpObj->PartyNumber == lpCallObj->PartyNumber )
#else
	if( lpObj->PartyNumber == lpCallObj->PartyNumber)
#endif	// MODIFY_SELFDEFENSE_20070904
	{
		selfdefense = FALSE;
	}
#endif
	
	if( selfdefense == TRUE  )
	{
		if( gObjTargetGuildWarCheck(lpObj, lpCallObj) == FALSE )
		{
#ifdef UPDATE_PK_SYSTEM_20070302	// ��Ƽ�� ��Ƽ�� ��ü�� ������� ����
#ifdef MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022	// ���濡 �ɸ� ������ SelfDefense�� �ִ´�
			if( lpCallObj->PartyNumber >= 0 )
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK �г�Ƽ ����� ����
				{
					gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
				}
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
#else	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022
			if( lpCallObj->PartyNumber >= 0 )
			{
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( gParty.GetPKPartyPenalty(partynum) < PK_LEVEL_DEFAULT+2)		// PK �г�Ƽ ����� ����
				{
					for(int n = 0; n < MAX_PARTYUSER; n++)
					{
						number = gParty.m_PartyS[partynum].Number[n];
						if( number >= 0 )
						{
							gObjCheckSelfDefense(lpObj, number);
						}
					}
				}
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
#endif	// MODIFY_PARTY_SELFDEFENSE_BUG_FIX_20071022
#else	// UPDATE_PK_SYSTEM_20070302
			gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
#endif	// UPDATE_PK_SYSTEM_20070302
		}
	}

	if( AttackDamage >= 5 )
	{		
		if( lpTargetObj->Type == OBJTYPE_MONSTER ) // ����ڰ� ����
		{
			if( (rand()%26) == 0 )	
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100);
			}
		}
		else 
		{
			if( (rand()%4) == 0 )	
			{
				if( gObjUniriaSprite(lpTargetObj) == FALSE )
				{
					MSBFlag = 1;
				}
			}
		}
	}

#ifdef NEW_FORSKYLAND2
	if( ManaChange )
	{
		GCManaSend(lpTargetObj->m_Index, (short)lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);				
	}
#endif	

	if( lpObj->Type == OBJTYPE_CHARACTER )
	{
		if( lpObj->m_Change == 9 )
		{
			GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);	
		}
	}
	
	lpObj->m_Rest = 0;
	if( AttackDamage > 0 )
	{
#ifdef FOR_ANTI_HACK
		if( lpObj->Type == OBJTYPE_CHARACTER && lpTargetObj->Type == OBJTYPE_MONSTER )
		{
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}		
#endif

		int atd_reflect;
		atd_reflect = (int)((float)AttackDamage*lpTargetObj->DamageReflect/100);

		if( atd_reflect )
		{
			//LogAdd("������ �ݻ� %x ", atd_reflect);
			// ������ �޴� ���(�������ݻ�ɷ����ִ´��)�� �޼���ť�� ������.
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, atd_reflect);
		}

		if( rand()%100 < lpObj->SetOpReflectionDamage )
		{	// ��Ʈ �ɼ� 100% ������ �ݻ� 
			gObjAddMsgSendDelay(lpTargetObj, MSG_REFLECT, lpObj->m_Index, 10, AttackDamage);
		}

#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826	// ���� ������ �߰�
		if( rand()%100 < lpObj->SetOpDoubleDamage )
		{
			AttackDamage *= 2;
			MsgDamage |= DT_DOUBLEDAMAGE;
		}
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

#ifdef ADD_SHIELD_POINT_01_20060403
		AttackDamage = AttackDamage - iTotalShieldDamage;

	#ifdef ADD_SKILL_WITH_COMBO
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
	#else
		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, iTotalShieldDamage);
	#endif

#else

	#ifdef ADD_SKILL_WITH_COMBO
			gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill);
	#else
			gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage);
	#endif

#endif // ADD_SHIELD_POINT_01_20060403
			
	}	
	else 
	{

#ifdef ADD_SHIELD_POINT_01_20060403
		GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage, 0);
#else
		GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);
#endif

	}

#ifdef NEW_FORSKYLAND3
	if( lpObj->Life <= 0 && lpObj->Type == OBJTYPE_CHARACTER )
	{	// ������ ���� �ɼǿ� ���� ĳ���� ü���� ���̸� 3���Ŀ� �ٽ� ���� üũ�� �ؼ� ���δ�
		if( lpObj->m_CheckLifeTime <= 0 )
		{
#ifdef USER_DIE_MAKE_OPPOSIT_PK_20040329
			lpObj->lpAttackObj = lpTargetObj;

			if (lpTargetObj->Type == OBJTYPE_CHARACTER) {
				lpObj->m_bAttackerKilled = true;		// ���� ������ �����ϴ� �׾����Ƿ� ������ ���� ���� �����ڴ� �ƴϴ�.
			}
			else {
				lpObj->m_bAttackerKilled = false;
			}
#else
			lpObj->lpAttackObj = lpTargetObj;
#endif			
			lpObj->m_CheckLifeTime = 3;
		}
	}
#endif
	
	return TRUE;
}



#ifdef DUEL_SYSTEM_20031028
	int  CDarkSpirit::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, BOOL bIsOnDuel, int criticaldamage)	
#else
	int  CDarkSpirit::GetAttackDamage(LPOBJECTSTRUCT lpObj, int targetDefense, int criticaldamage)	
#endif
{
#ifdef DUEL_SYSTEM_20031028
	#ifdef ADD_SHIELD_POINT_01_20060403
		if ( g_ShieldSystemOn == TRUE )
		{
			if( bIsOnDuel == TRUE )
			{
				bIsOnDuel = FALSE;
			}
		}
	#endif
#endif
		
	int ad;
	int sub;
	int AttackDamage;

	sub = (m_AttackDamageMax)-(m_AttackDamageMin);

#ifdef MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826
	if( criticaldamage == 1 )	// ũ��Ƽ��(���) ������
	{
		AttackDamage = m_AttackDamageMin+sub;
	}
	else if( criticaldamage == 2 )	// ������Ʈ ������
	{
		AttackDamage = m_AttackDamageMin+sub;
		AttackDamage += AttackDamage*20/100;
	}
	else						// �Ϲ� ������
	{		
		AttackDamage = m_AttackDamageMin+(rand()%(sub+1));
	}
#else
	if( criticaldamage )
	{
		AttackDamage = m_AttackDamageMin+sub;
	}
	else
	{		
		AttackDamage = m_AttackDamageMin+(rand()%(sub+1));
	}
#endif // MODIFY_EX_DAMAGE_DARKSPRIT_BUGFIX_20080826

	if( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type >= MAKE_ITEMNUM(2,8) && 
		lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type <= MAKE_ITEMNUM(2,12)
#ifdef ADD_WEAPON_OF_ARCHANGEL_SCEPTER_20050706
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,13) )
#endif
#ifdef ADD_NEW_ITEM_01_20050823
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,14) )
#endif	
#ifdef MODIFY_350ITEM_BUGFIX2_20060823
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,15) )	// 350������.���̴׼���
#endif
#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,17) )	// ���ַ�Ʈ���� �߰�
#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
#ifdef ADD_STRIKER_SCEPTER_20090317
		|| ( lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Type == MAKE_ITEMNUM(2,18) )	// ��Ʈ����Ŀ���� �߰�
#endif // ADD_STRIKER_SCEPTER_20090317
	  )
	{	//���͸� ����
		int addpetdamage = lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_Magic/2;
		// ������ ���ҿ� ���� �г�Ƽ�� ����ȴ�
		addpetdamage -= (addpetdamage*lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT].m_CurrentDurabilityState);	
		addpetdamage = AttackDamage*addpetdamage/100;
		AttackDamage += addpetdamage;
	}
	
#ifdef DUEL_SYSTEM_20031028
// 	if (bIsOnDuel)
// 		ad = (AttackDamage) * 60 /100 - targetDefense;
// 	else
#endif
		ad = (AttackDamage)-targetDefense;	

	return ad;
}

BOOL CDarkSpirit::MissCheck(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss )
{
	int SuccessAttackRate = 0;
	int TargetSuccessfulBlocking = lpTargetObj->m_SuccessfulBlocking;
	int	MsgDamage = 0;
	
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �нú� 1�� - ���ݼ����� ���(��ũ���Ǹ�)
	SuccessAttackRate += lpObj->m_MPSkillOpt.iMpsAttackSuccessRate;		
#endif

	if(TargetSuccessfulBlocking > g_GlobalConfig.m_iMaxMissRate)
		TargetSuccessfulBlocking = g_GlobalConfig.m_iMaxMissRate;

#ifdef NEW_FORSKYLAND3
	if( m_SuccessAttackRate < TargetSuccessfulBlocking )
	{
		bAllMiss = TRUE;
	}
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ��� ���뽺ų ��ȣ�� �ֹ� ����
	if(CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		if(lpTargetObj->Type == OBJTYPE_CHARACTER 
			&& g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber))
#else
		if((lpTargetObj->m_ViewSkillState & STATE_ILLUSION_TEMPLE_PRODECTION) == STATE_ILLUSION_TEMPLE_PRODECTION)
#endif
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}

		// ���� ��Ƽ�������� ������ ����
		if(lpObj->PartyNumber == lpTargetObj->PartyNumber)
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}
	}
#endif

#ifdef NEW_FORSKYLAND3
	if( bAllMiss )
	{	
#ifdef MODIFY_SUCCESSBLOCKING_ALLMISS_20040408
		if( rand()%100 >= 5 )
#else
		if( rand()%100 >= 3 )
#endif
		{	// all �̽��� 3%�� Ȯ���� �������� ��� �´�

#ifdef ADD_SKILL_WITH_COMBO
#else
			MsgDamage = 0x20;
#endif
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);				
			return FALSE;
		}
	}
	else 
#endif

	if( m_SuccessAttackRate )
	{
		if( (rand()%m_SuccessAttackRate) < TargetSuccessfulBlocking) 
		{

	#ifdef ADD_SKILL_WITH_COMBO
	#else
			int MsgDamage = 0x20;
	#endif
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, MsgDamage);			
			return FALSE;
		}
	}
	return TRUE;
}

#ifdef ADD_SHIELD_POINT_01_20060403
BOOL CDarkSpirit::MissCheckPvP(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int skill, int skillSuccess, int& bAllMiss)
{
	float iAttackRate = 0;
	float iDefenseRate = 0;
	int iAttackSuccessRate = 0;
	// �� ������ AttackRating/DefenseRating

	// CODEREVIEW_SHIELD : �Ҽ��� ������

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328		// ȯ�� ��� ���뽺ų ��ȣ�� �ֹ� ����
	if(CHECK_ILLUSION_TEMPLE(lpTargetObj->MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(lpTargetObj->MapNumber) == ILLUSION_TEMPLE_STATE_PLAYING)
	{
#ifdef MODIFY_ILLUSIONTEMPLE_BUGFIX_4_20070802
		if(lpTargetObj->Type == OBJTYPE_CHARACTER 
			&& g_IllusionTempleEvent.CheckSkillProdection(lpTargetObj->m_iIllusionTempleIndex, lpTargetObj->MapNumber))
#else
		if((lpTargetObj->m_ViewSkillState & STATE_ILLUSION_TEMPLE_PRODECTION) == STATE_ILLUSION_TEMPLE_PRODECTION)
#endif
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}

		// ���� ��Ƽ�������� ������ ����
		if(lpObj->PartyNumber == lpTargetObj->PartyNumber)
		{
			GCDamageSend(lpObj->m_Index,lpTargetObj->m_Index , 0, 0, 0, 0);
			return FALSE;
		}
	}
#endif

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ���� ���� ������/���� ��� ������1
	int iTotalLevel = lpObj->Level + lpObj->m_nMasterLevel;
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 45 / 10 );
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 4 );
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 6 / 10 );
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���� ���� ������
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = ( iTotalLevel * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	int iTargetTotalLevel = lpTargetObj->Level + lpTargetObj->m_nMasterLevel;
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 10 );
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���� ��� ������
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = ( iTargetTotalLevel * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	// ���׼���
	if( iAttackRate <= 0 || iDefenseRate <= 0 || lpObj->Level <= 0 || lpTargetObj->Level <= 0 )
	{
		// iAttackRate�� iDefenseRate�� 0�� ���°���� ó��
		return FALSE;
	}
#else	
	if( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 45 / 10 );
	}
	else if( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 4 );
	}
	else if( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 6 / 10 );
	}
	else if( lpObj->Class == CLASS_MAGUMSA )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
	else if( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���� ���� ������
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 35 / 10 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
	else if( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 10 );
	}
	else if( lpTargetObj->Class == CLASS_MAGUMSA )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
	else if( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 4 );
	}
#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ���� ��� ������
	else if( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = ( lpTargetObj->Level * 2 ) + ( lpTargetObj->Dexterity / 2 );
	}
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

	if( iAttackRate <= 0 || iDefenseRate <= 0 || lpObj->Level <= 0 || lpTargetObj <= 0 )
	{
		// iAttackRate�� iDefenseRate�� 0�� ���°���� ó��
		return FALSE;
	}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

	// ���� ���� ������
	// iAttackSuccessRate = 100 * iAttackRate / ( iAttackRate + iDefenseRate ) * 2 * lpObj->Level / ( lpObj->Level + lpTargetObj->Level );

#ifdef ADD_380ITEM_NEWOPTION_20060711
	// 380 �߰� ������ �ɼ� : ���ݼ�����, ��� ������ ���
	iAttackRate		+= lpObj->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;
#endif // ADD_380ITEM_NEWOPTION_20060711

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ���� ��ȭ ������ �ɼ� : ���� ������, ��� ������ ��� 
	iAttackRate		+= lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate	+= lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// �нú� 1�� - ���ΰ��ݼ�����, ���ι�� ������ ���
		iAttackRate		+= lpObj->m_MPSkillOpt.iMpsPVPAttackDmgRate;
		iDefenseRate	+= lpTargetObj->m_MPSkillOpt.iMpsPVPBlockingRate;
#endif
		
#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ���� ���� ������/���� ��� ������2
	// CODEREVIEW_SHIELD : �ڵ� ���� ���� �ϱ�
	float iExpressionA = ( ( iAttackRate * 10000 ) / ( iAttackRate + iDefenseRate ) );
	float iExpressionB = ( ( iTotalLevel * 10000 ) / ( iTotalLevel + iTargetTotalLevel ) );
#else
	// CODEREVIEW_SHIELD : �ڵ� ���� ���� �ϱ�
	float iExpressionA = ( ( iAttackRate * 10000 ) / ( iAttackRate + iDefenseRate ) );
	float iExpressionB = ( ( lpObj->Level * 10000 ) / ( lpObj->Level + lpTargetObj->Level ) );
#endif

	iExpressionA = iExpressionA / 10000;
	iExpressionB = iExpressionB / 10000;

	iAttackSuccessRate = 100 * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;

#ifdef MASTER_LEVEL_UP_SYSTEM_20070912	// ���շ��� ���� - ���� ���� ���� ���� ������ ����
	// ���� ���� ���� ���� ������ ����
	if( iTargetTotalLevel - iTotalLevel >= 100  )
	{
		iAttackSuccessRate = iAttackSuccessRate - 5;
	}
	else if( iTargetTotalLevel - iTotalLevel >= 200 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 10;
	}
	else if( iTargetTotalLevel - iTotalLevel >= 300 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 15;
	}
#else
	// ���� ���� ���� ���� ������ ����
	if( lpTargetObj->Level - lpObj->Level >= 100  )
	{
		iAttackSuccessRate = iAttackSuccessRate - 5;
	}
	else if( lpTargetObj->Level - lpObj->Level >= 200 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 10;
	}
	else if( lpTargetObj->Level - lpObj->Level >= 300 )
	{
		iAttackSuccessRate = iAttackSuccessRate - 15;
	}
#endif	// MASTER_LEVEL_UP_SYSTEM_20070912

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[SD Attack] AR:%5.1f/DR:%5.1f/ASR:%d", iAttackRate, iDefenseRate, iAttackSuccessRate);
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
	GCServerMsgStringSend( Msg, lpTargetObj->m_Index, 1 );
#endif

	// ���� ����?
	DWORD dwRate = rand() % 100;
	if( dwRate > iAttackSuccessRate )
	{
		// Miss ������ ������.
		GCDamageSend( lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return FALSE;
	}

	return TRUE;
}
#endif

// 
void CDarkSpirit::ChangeCommand(int command, int targetindex)
{
	CDarkSpirit::ePetItem_Mode setmode = CDarkSpirit::PetItem_Mode_Normal;
	
	if( command == CDarkSpirit::PetItem_Mode_Normal )			
		setmode = CDarkSpirit::PetItem_Mode_Normal;
	else if( command == CDarkSpirit::PetItem_Mode_Attack_Random )
		setmode = CDarkSpirit::PetItem_Mode_Attack_Random;
	else if( command == CDarkSpirit::PetItem_Mode_Attack_WithMaster )
		setmode = CDarkSpirit::PetItem_Mode_Attack_WithMaster;
	else if( command == CDarkSpirit::PetItem_Mode_Attack_Target )
		setmode = CDarkSpirit::PetItem_Mode_Attack_Target;

	CDarkSpirit::SetMode(setmode, targetindex);
}

void CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType)
{
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));

	pMsg.PetType = PetType;
	pMsg.InvenType = 254;
	pMsg.nPos = nPos;

	pMsg.Level = gObj[aIndex].pInventory[nPos].m_PetItem_Level;
	pMsg.Exp = gObj[aIndex].pInventory[nPos].m_PetItem_Exp;
	
#ifdef MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626
	pMsg.Life = (BYTE)(gObj[aIndex].pInventory[nPos].m_Durability);
#endif // MODIFY_DARKLORD_SEND_INFO_PROTOCOL_EXTEND_20080626

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

#endif	// #ifdef DARKLORD_WORK

#ifdef ADD_SHIELD_POINT_01_20060403
int CDarkSpirit::GetShieldDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;

	if( g_ShieldSystemOn == FALSE )
	{
		return 0;
	}

	if( iAttackDamage <= 0 )
	{
		return 0;
	}

	// PVP�� ��
	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0;
	bool bReduceShieldGage = false;

// CODEREVIEW_SHIELD :  SD �������� Life ���� ����
#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	// ��ȭ�� ���� ��ȭ ������ �ɼ� : SD������ ��� / SD ���� ��� / SD ���� Ȯ�� ���

	// 0. SD ���� �� �ʱ� ����
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;

	// 1. ������ : SD ������ ��� �ɼ�. ������ SD������ ���ߴ� ����.
	iDamageDevideToSDRate -= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;

	// 2. ����� : SD ���� ��� �ɼ�. SD�� ������ ����.
	iDamageDevideToSDRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;
	
	// 3. ���� Ȯ��
	if ( iDamageDevideToSDRate < 0 ) 
		iDamageDevideToSDRate = 0;
	if ( iDamageDevideToSDRate > 100 ) 
		iDamageDevideToSDRate = 100;
	
	// 4. ������ : SD���� Ȯ���� �ִ� ��� : 0713 ����
	if ( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;
		if ( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}

	// 5. �α�
	if ( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate != 0 
		|| lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate != 0
		|| lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
	{
			LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
				lpObj->AccountID, lpObj->Name,
				lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate,
				lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate,
				lpTargetObj->AccountID, lpTargetObj->Name,
				lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate,
				iDamageDevideToSDRate);
	}

	// 6. ���� ���
	iReduceShield = ( iAttackDamage * iDamageDevideToSDRate ) / 100;
#else
	iReduceShield = ( iAttackDamage * g_iDamageDevideToSDRate ) / 100;
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

	iReduceLife = iAttackDamage - iReduceShield;

	if( lpTargetObj->iShield - iReduceShield < 0 )
	{
		iReduceLife += ( iReduceShield - lpTargetObj->iShield );
		iReduceShield = lpTargetObj->iShield;
		if( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

#ifdef SHIELD_POINT_DEBUGLOG
	char Msg[256];
	sprintf(Msg, "[Shield] Damage : %d, HP : %d, SD :%d", iAttackDamage, iReduceLife, iReduceShield );
	GCServerMsgStringSend( Msg, lpObj->m_Index, 1 );
	GCServerMsgStringSend( Msg, lpTargetObj->m_Index, 1 );
#endif

#ifdef ADD_JEWEL_OF_HARMONY_SYSTEM_20060530
	#if TESTSERVER == 1
		LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s] Defender:[%s][%s] Damage : %d, HP : %d, SD :%d", 
			lpObj->AccountID, lpObj->Name,
			lpTargetObj->AccountID, lpTargetObj->Name,
			iAttackDamage, iReduceLife, iReduceShield );

	#endif
#endif // ADD_JEWEL_OF_HARMONY_SYSTEM_20060530

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20 / 100;
	if( bReduceShieldGage == true 
		&& ( iReduceLife > iReduceLifeForEffect )
		)
	{
	#ifdef MODIFY_NO_SHOW_SHIELDBROKE_EFFECT_IN_CHAOSCASTLE_20060523
		if( CHECK_CHAOSCASTLE(lpTargetObj->MapNumber) == FALSE )
	#endif
		{
			// �� �ı� ����Ʈ
			GCSendEffectInfo( lpTargetObj->m_Index, EFFECT_SHIELDBROKE );
		}
	}

	return iReduceShield;
}
#endif