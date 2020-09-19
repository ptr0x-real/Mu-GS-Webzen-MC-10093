// ObjUseSkill.cpp: implementation of the CObjUseSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Include\Prodef.h"
#include "Include\SProdef.h"
#include "Common\winutil.h"
#include "Include\GMProtocol.h"
#include "Include\Public.h"
#include "Include\cb_protocol.h"

#include "zzzmathlib.h"

#include "User.h"
#include "protocol.h"
#include "Gamemain.h"

#include "ObjUseSkill.h"
#include "MagicDamage.h"
#include "PartyClass.h"
#include "Math.h"

#ifdef CHAOSCASTLE_SYSTEM_20040408
#include "ChaosCastle.h"
#endif

#ifdef CASTLE_MAIN_SCHEDULER_20041111
#include "CastleSiege.h"
#endif

#ifdef CHINA_HACKUSER_KICK_SYSTEM
#include "ChinaHackUserKick.h"
#endif

#ifdef DARKLORD_WORK
#include "SkillHitBox.h"
#include "MoveCommand.h"

int		DecPartyMemberHPandMP(LPOBJECTSTRUCT lpObj);
#endif

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
#include "SkillAdditionInfo.h"
#endif

#ifdef ADD_KANTURU_20060627
#include "Kanturu.h"
#endif

#ifdef ADD_RAKLION_20080408
#include "Raklion.h"
#endif // ADD_RAKLION_20080408

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830
#include "ObjCalCharacter.h"
#endif

#ifdef ADD_EVENT_MAP_ILLUSION_TEMPLE_20070328
#include "IllusionTempleEvent.h"
#endif

#ifdef ADD_NEWPVP_PKFIELD
#include "NewPVP.h"
#endif // ADD_NEWPVP_PKFIELD

extern PartyClass	gParty;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CObjUseSkill gObjUseSkill;

CObjUseSkill::CObjUseSkill()
{

}

CObjUseSkill::~CObjUseSkill()
{

}

int CObjUseSkill::GetTargetLinePath(int sx, int sy, int tx, int ty, int* PathX, int* PathY, int distance)
{
	int x, y;
	int delta_x, delta_y, half, error = 0;
	int incx = 1, incy = 1;

	if( sx > tx )
	{
		incx = -1;
		delta_x = sx - tx;
	}
	else
	{
		delta_x = tx - sx;
	}

	if( sy > ty )
	{
		incy = -1;
		delta_y = sy - ty;
	}	
	else
	{
		delta_y = ty - sy;
	}

	int dis = (int)(sqrt((double)(delta_x*delta_x)+(delta_y*delta_y)));
	dis = distance-dis;

	int count = 0;
	
	PathX[count] = sx;	PathY[count] = sy;	count++;

	if( delta_x > delta_y )
	{
		y = sy;
		half = (int)( delta_x / 2 );

		if( incx > 0 )
		{
			for( x = sx+1; x <= tx+dis; x ++ )
			{
				error += delta_y;
				if( error > half )
				{
					y += incy;
					error -= delta_x;
				}

				PathX[count] = x;	PathY[count] = y;	count++;				
			}
		}
		else
		{
			for( x = sx-1; x >= tx-dis; x -- )
			{
				error += delta_y;
				if( error > half )
				{
					y += incy;
					error -= delta_x;
				}
				PathX[count] = x;	PathY[count] = y;	count++;
			}
		}
	} 
	else 
	{
		x = sx;
		half = (int)( delta_y / 2 );
		
		if( incy > 0 )
		{
			for( y = sy+1; y <= ty+dis; y ++ )
			{
				error += delta_x;		
				if( error > half )
				{
					x += incx;
					error -= delta_y;
				}
				PathX[count] = x;	PathY[count] = y;	count++;
			}	
		}
		else
		{
			for( y = sy-1; y >= ty-dis; y -- )
			{
				error += delta_x;		
				if( error > half )
				{
					x += incx;
					error -= delta_y;
				}
				PathX[count] = x;	PathY[count] = y;	count++;
			}	
		}
	}
	return count;
}

BOOL CObjUseSkill::SpeedHackCheck(int aIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int iTimeCalc = GetTickCount()-lpObj->m_LastAttackTime;

	if(lpObj->Type != OBJTYPE_CHARACTER)	// ������ �ƴϸ�, �� ���͸� üũ�ϰ� ������..
	{
		return(TRUE);
	}

/*
#ifdef FOR_CHINA
	if( iTimeCalc < lpObj->m_DetectSpeedHackTime ) 
#else
	if( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc != 0 ) 
#endif
*/
	if( iTimeCalc < lpObj->m_DetectSpeedHackTime ) 
	{
		lpObj->m_DetectCount++;			

		lpObj->m_SumLastAttackTime += iTimeCalc;

		if( lpObj->m_DetectCount > gHackCheckCount )
		{
			lpObj->m_DetectedHackKickCount++;
			if( gIsKickDetecHackCountLimit ) 
			{
				if( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
				{
#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518			
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
											 lpObj->AccountID, 
											 lpObj->Name, 
											 lMsg.Get(1900+lpObj->Class),
											 lpObj->m_DetectedHackKickCount,
											 lpObj->MapNumber);						     
	#else
					LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d) @%d",
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lMsg.Get(1900+lpObj->Class),
							 lpObj->m_DetectedHackKickCount,
							 lpObj->MapNumber);						     					
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
					ChinaHackLogFile.Output("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
											 lpObj->AccountID, 
											 lpObj->Name, 
											 lMsg.Get(1900+lpObj->Class),
											 lpObj->m_DetectedHackKickCount);						     
	#else
					LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)",
							 lpObj->AccountID, 
							 lpObj->Name, 
							 lMsg.Get(1900+lpObj->Class),
							 lpObj->m_DetectedHackKickCount);						     					
	#endif
#endif
					CloseClient(aIndex);
					return FALSE;
				}
			}

			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;


#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
			ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong MagicSkill (%d)(%d) Penalty %d @%d", 
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectSpeedHackTime,
									 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
									 lpObj->m_SpeedHackPenalty,
									 lpObj->MapNumber);
			ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);
	#else
			LogAddTD("[%s][%s] %s Attack Speed Is Wrong MagicSkill (%d)(%d) Penalty %d @%d", 
					 lpObj->AccountID, 
					 lpObj->Name, 
					 lMsg.Get(1900+lpObj->Class),
					 lpObj->m_DetectSpeedHackTime,
					 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
					 lpObj->m_SpeedHackPenalty,
					 lpObj->MapNumber);
	#endif
#else
	#ifdef CHINA_HACKUSER_KICK_SYSTEM
			ChinaHackLogFile.Output("[%s][%s] %s Attack Speed Is Wrong MagicSkill (%d)(%d) Penalty %d", 
									 lpObj->AccountID, 
									 lpObj->Name, 
									 lMsg.Get(1900+lpObj->Class),
									 lpObj->m_DetectSpeedHackTime,
									 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
									 lpObj->m_SpeedHackPenalty);
			ChinaHackUserKick.InsertSpeedNCount(lpObj->m_Index);
	#else
			LogAddTD("[%s][%s] %s Attack Speed Is Wrong MagicSkill (%d)(%d) Penalty %d", 
					 lpObj->AccountID, 
					 lpObj->Name, 
					 lMsg.Get(1900+lpObj->Class),
					 lpObj->m_DetectSpeedHackTime,
					 lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
					 lpObj->m_SpeedHackPenalty);
	#endif
#endif
		}
		//lpObj->m_LastAttackTime = GetTickCount();
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}
	lpObj->m_LastAttackTime = GetTickCount();
	//LogAdd("Normal Magic2 Attack Speed = [%d,%d] %d", lpObj->m_AttackSpeed, lpObj->m_MagicSpeed, iTimeCalc);

	if( bIsIgnorePacketSpeedHackDetect )
	{
		if( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;

#ifdef MODIFY_HACKLOG_OF_ATTACK_20050518
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left) @%d", 
					  lpObj->AccountID, 
					  lpObj->Name, 
					  lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty,
					  lpObj->MapNumber);	
#else
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left)", 
					  lpObj->AccountID, 
					  lpObj->Name, 
					  lMsg.Get(1900+lpObj->Class),
					  lpObj->m_SpeedHackPenalty);		
#endif
			return FALSE;
		}
	}
	
	return TRUE;
}

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
BOOL CObjUseSkill::EnableSkill( int Skill, int SkillLevel )
#else
BOOL CObjUseSkill::EnableSkill(BYTE Skill)
#endif
{
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��� ������ ��ų���� �Ǵ��� �� �����ͷ��� ��ų �߰�	
	if( g_MasterSkillSystem.CheckMasterLevelSkill( Skill ) )
	{
		int iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill( Skill );
		if(	iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE				// �ҿ�ٸ��� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_HELL					// �����̾� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_WHEEL_KNIGHT			// ȸ�������� ��ȭ1 (���)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_HEALING				// ġ�� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_CROSSBOW				// �ٹ� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_WHEEL_MAGUMSA		// ȸ�������� ��ȭ2 (���˻�)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE		// ���������� ��ȭ	
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_BLAST				// ����Ʈ ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_DARKHORSE_ATTACK		// �����ũ ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_FIREBURST			// ���̾����Ʈ ��ȭ

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �����ͷ��� 4��ũ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_EVIL					// �Ƿ� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_EXPICE				// ���� ���̽� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_STRIKE				// ��ο� ��ȭ(���)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_BLOWOFFURY			// �г��� �ϰ� ��ȭ(���)
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_DEFENSE				// ���� ���
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_ATTACK				// ���ݷ� ���	
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_DEFENSEDOWN			// ���� ���� ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_SWORD6				// �Ŀ� ������ ��ȭ
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_FIRESCREAM			// ���̾� ��ũ�� ��ȭ
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428	
			
#ifdef UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
			&& iBaseMLS !=	AT_SKILL_ML_STRENGTHEN_MAGUMSA_EVIL			// �Ƿ� ��ȭ(���˻�)
#endif	// UPDATE_MASTER_SKILL_4RANK_ADDITION_20080808
			)
		{
			return FALSE;
		}

		return TRUE;		
	}
#endif
	
	if( Skill != AT_SKILL_STORM		&&		// ȸ����
		Skill != AT_SKILL_EVIL		&&		// �Ƿ�
		Skill != AT_SKILL_FLAME		&&		// �ұ��
		Skill != AT_SKILL_HELL		&&		// �����̾�
		Skill != AT_SKILL_BLAST		&&		// ����Ʈ
		Skill != AT_SKILL_INFERNO	&&		// ���丣�� 
		Skill != AT_SKILL_CROSSBOW	&&		// �ٹ�
		Skill != AT_SKILL_DEVILFIRE &&		// ���ǺҲ�
		Skill != AT_SKILL_FLASH		&&		// ������÷���
		Skill != AT_SKILL_WHEEL 			// ȸ����

#ifdef ADD_SKILL_20031022			
		&& Skill != AT_SKILL_SWORD6	
#endif

#ifdef NEW_SKILL_FORSKYLAND		
		&& 		
		Skill != AT_SKILL_KNIGHTSPEAR		&&		// ���â
		Skill != AT_SKILL_KNIGHTADDLIFE		&&		// �������
		Skill != AT_SKILL_STRIKE			&& 		// �ϴ���
		Skill != AT_SKILL_KNIGHTDINORANT	&& 		// ����Ʈ ����		

		Skill != AT_SKILL_ELFHARDEN			&&		// ��ȭ
		Skill != AT_SKILL_PENETRATION		&&		// ����		

		Skill != AT_SKILL_DEFENSEDOWN		&& 		// ����
		Skill != AT_SKILL_MAGICDEFENSE				// �ҿ�ٸ���
#endif

#ifdef ADD_SKILL_WITH_COMBO
		&& 		
		Skill != AT_SKILL_BLOWOFFURY		&&		// �г��� �ϰ�		
		Skill != AT_SKILL_EXPPOISON			&&		// Ȯ�嵶
		Skill != AT_SKILL_EXPICE			&&		// Ȯ����̽�
		Skill != AT_SKILL_EXPHELL					// Ȯ�������̾�
#elif GAME_VERSION == G_V_97D
		&& 		
		Skill != AT_SKILL_BLOWOFFURY		&&		// �г��� �ϰ�		
		Skill != AT_SKILL_EXPPOISON			&&		// Ȯ�嵶
		Skill != AT_SKILL_EXPICE			&&		// Ȯ����̽�
		Skill != AT_SKILL_EXPHELL					// Ȯ�������̾�
#endif

#ifdef DARKLORD_WORK		
		&& Skill != AT_SKILL_ELECTRICSPARK			// �Ϸ�Ʈ�� ����ũ
		&& Skill != AT_SKILL_DARKHORSE_ATTACK
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
		&& Skill != AT_SKILL_BRAND_OF_SKILL			// ��ũ�ε� ���� ��ų
		&& Skill != AT_SKILL_STUN					// ����
		&& Skill != AT_SKILL_REMOVAL_STUN			// ���� ����
		&& Skill != AT_SKILL_ADD_MANA				// ���� ����
		&& Skill != AT_SKILL_INVISIBLE				// ����
		&& Skill != AT_SKILL_REMOVAL_INVISIBLE		// ���� ����
		&& Skill != AT_SKILL_REMOVAL_MAGIC			// ���� ����
#endif
		
#ifdef ADD_ITEM_FENRIR_01_20051110
		&& Skill != AT_SKILL_FENRIR_ATTACK			// �渱 ��ų - �ö������
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
		&& Skill != AT_SKILL_INFINITY_ARROW			// ����� ���Ǵ�Ƽ �ַο�
#endif
		
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601		// ��ũ�ε� ���̾� ��ũ�� Ȱ��ȭ
		&& Skill != AT_SKILL_FIRESCREAM	
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ȯ���� ��ų
		// ����
		&& Skill != AT_SKILL_DRAINLIFE				// �巹�ζ�����
		&& Skill != AT_SKILL_CHAINLIGHTNING			// ü�� ����Ʈ��
		&& Skill != AT_SKILL_LIGHTNINGORB			// ����Ʈ�׿���
		
		&& Skill != AT_SKILL_THORNS					// ����(�������ݻ�)
		
		&& Skill != AT_SKILL_SLEEP					// ����
		&& Skill != AT_SKILL_BLIND					// ����ε�
		
		// ȯ�� ��ų
		&& Skill != AT_SKILL_EXPLOSION_2			// �ͽ��÷���
		&& Skill != AT_SKILL_REQUIEM				// ������
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912		

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
		&& Skill != AT_SKILL_WEAKNESS				// ��ũ�Ͻ�
		&& Skill != AT_SKILL_ENERVATION				// �̳ʺ��̼�
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
		&& Skill != AT_SKILL_PULLUTION				// �޷��
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425		

#ifdef SEASON4_PATCH_GROOVING
		// --->
		// ���� 4.0 �߰� ��ų  grooving 2008/04/24
		&& Skill !=	AT_SKILL_LIGHTING_SHOCK			// ����Ʈ�� ��ũ
		&& Skill !=	AT_SKILL_BERSERKER				// ����Ŀ
		// <---
#endif

#ifdef ADD_SEASON4_NEW_SKILL_20080522	// ��� ���ɽ�ų
		&& Skill != AT_SKILL_DESTRUCTION			// �ı��� �ϰ�
		&& Skill != AT_SKILL_MAGICPOWRER			// ���� ����
		&& Skill != AT_SKILL_RECOVERY				// ȸ��
		&& Skill != AT_SKILL_MULTISHOT				// ��Ƽ��
		&& Skill != AT_SKILL_FLAMESTRIKE			// �÷��� ��Ʈ����ũ
		&& Skill != AT_SKILL_GIGANTICSTORM			// �Ⱓƽ ����
		&& Skill != AT_SKILL_CHAOTICDESAIR			// ī��ƽ ���̾�
#endif	// ADD_SEASON4_NEW_SKILL_20080522		
		)
	{
		return FALSE;
	}

	return TRUE;
}

int CObjUseSkill::GetUseMana(int aIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	float mana = lpObj->Mana;
	
	// ���ʹ� MP�� ���� �ʴ´�.
	if( lpObj->Type != OBJTYPE_CHARACTER ) return 0;
	if( lpMagic == NULL ) return FALSE;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ���� ���� ��ȭ
	float DecreaseMana = 0.0f;

	DecreaseMana = MagicDamageC.SkillGetMana(lpMagic->m_Skill) + GetAddUseMana( lpObj, lpMagic );
	DecreaseMana -= (DecreaseMana * lpObj->m_MPSkillOpt.iMpsDecreaseMana) / 100;
	
	if(DecreaseMana < 1.0f)	DecreaseMana = 1.0f;
	mana -= DecreaseMana;
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	mana -= MagicDamageC.SkillGetMana(lpMagic->m_Skill);

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
	// ������ �߰��� ��´�.
	mana -= GetAddUseMana( lpObj, lpMagic );
#endif
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
	// ������ ���� ����� �� ���ٸ� �׳� ����
	if( mana < 0 ) return -1;

	return (int)mana;
}

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
int	CObjUseSkill::GetAddUseMana( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic )
{
	int iRetValue = 0;

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// ������� ���Ǵ�Ƽ ���ο콺ų�� ����ϰ� �ٹ߽�ų�� ����� ��� ���� �Ҹ� ó��
	// ȭ��, ���� +0 :0, +1:1�߰� ����, +2:2 �߰� ����
	if( lpObj->Class == CLASS_ELF && lpObj->Type == OBJTYPE_CHARACTER && lpObj->ChangeUP == 1 ) 
	{
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW ) == true )
	#else
		if( lpObj->m_iMuseElfInfinityArrowSkillTime > 0  )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		{
			if( lpMagic->m_Skill == AT_SKILL_CROSSBOW		|| 
				lpMagic->m_Skill == AT_SKILL_BOW			||
				lpMagic->m_Skill == AT_SKILL_ELFHARDEN		||
				lpMagic->m_Skill ==	AT_SKILL_PENETRATION	||
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ٹ� ��ȭ ��ų �߰�
				lpMagic->m_Skill == AT_SKILL_5CROSSBOW		||
				g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill ) == AT_SKILL_ML_STRENGTHEN_CROSSBOW
				)
#else
				lpMagic->m_Skill == AT_SKILL_5CROSSBOW )
#endif
			{
				int		iArrowLevel =0;
				CItem	*Right = &lpObj->pInventory[EQUIPMENT_WEAPON_RIGHT];
				CItem	*Left  = &lpObj->pInventory[EQUIPMENT_WEAPON_LEFT];
				
				// ������ �����ϰ� �ִٸ�..
				if( ( Right->m_Type >= MAKE_ITEMNUM(4,8)) && (Right->m_Type <= MAKE_ITEMNUM(4,14)) ||
					( Right->m_Type == MAKE_ITEMNUM(4,16)) || (Right->m_Type == MAKE_ITEMNUM(4,18)) || 
					( Right->m_Type == MAKE_ITEMNUM(4,19)) )
				{
					if( ( Left->m_Type == MAKE_ITEMNUM(4,7)) )
					{
						iArrowLevel = Left->m_Level;
					}			
				}
				else if( (Left->m_Type >= MAKE_ITEMNUM(4,0)) && (Left->m_Type <= MAKE_ITEMNUM(4,6)) || 
						 (Left->m_Type == MAKE_ITEMNUM(4,17)) 
		#ifdef ADD_NEW_MAP_KALIMA_20040518
						 || (Left->m_Type == MAKE_ITEMNUM(4,20))		// Į������ �߰��� ����Ȱ
		#endif				 
						 
		#ifdef ADD_NEW_ITEM_01_20050823
						 || (Left->m_Type == MAKE_ITEMNUM(4,21))		// ������ Ȯ��� �߰��� ����Ȱ
		#endif

		#ifdef MODIFY_350ITEM_BUGFIX_20060811
						 || (Left->m_Type == MAKE_ITEMNUM(4,22))		// �˹�Ʈ�ν����� Ȱ �߰�
		#endif
						 
		#ifdef MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
						 || (Left->m_Type == MAKE_ITEMNUM(4,23))		// ��ũ���ð� Ȱ �߰�
		#endif // MODIFY_SEASON4_UPDATE_BUGFIX_01_20080722
		#ifdef ADD_AERINBOW_20090309
						 || (Left->m_Type == MAKE_ITEMNUM(4,24))		// ������� �߰�
		#endif // ADD_AERINBOW_20090309						 
						 )	// Ȱ�� �����ϰ� �ִٸ�..
				{
					if( (Right->m_Type == MAKE_ITEMNUM(4,15)))	// �ݴ��� �տ��� ȭ��
					{
						iArrowLevel = Right->m_Level;
					}			
				}

				// ȭ�� ������ ���� ������ ���
				if( iArrowLevel == 0 )
				{
					iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0();
				}
				else if( iArrowLevel == 1 )
				{
					iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1();
				}
				else if( iArrowLevel == 2 )
				{
					iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2();
				}
#ifdef UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
				else if( iArrowLevel == 3 )
				{
					iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus3();
				}
#endif // UPDATE_ADD_ARROW_TYPE_PLUS_3_20081106
			}
		}
	}	
#endif	// #ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	
	return iRetValue;	
}
#endif	// #ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407

#ifdef NEW_FORSKYLAND2
int CObjUseSkill::GetUseBP(int aIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int bp = lpObj->BP;	
	// ���ʹ� ���� �ʴ´�.
	if( lpObj->Type != OBJTYPE_CHARACTER ) return 0;
	if( lpMagic == NULL ) return FALSE;

	int usingbp = MagicDamageC.SkillGetBP(lpMagic->m_Skill);	
	usingbp -=  usingbp*lpObj->SetOpDecreaseAG/100;

#ifdef MODIFY_SOCKET_ITEM_NEW_OPTION_20080818
	if( lpObj->m_btAGReduceRate > 0 )
	{
		usingbp -= ( usingbp * lpObj->m_btAGReduceRate ) / 100;
	}
#endif // MODIFY_SOCKET_ITEM_NEW_OPTION_20080818

	bp -= usingbp;
	
	// ������ ���� ����� �� ���ٸ� �׳� ����
	if( bp < 0 ) return -1;

	return (int)bp;
}
#endif

#ifdef EXPAND_ATTACK_PACKET_0X1E	
void CObjUseSkill::UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
#else
void CObjUseSkill::UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos)
#endif
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	
#ifdef ADD_SKILL_WITH_COMBO
	BOOL	bCombo = FALSE;
	if( gComboAttack.CheckCombo(aIndex, lpMagic->m_Skill) )
	{
		bCombo = TRUE;		
		//GCMagicAttackNumberSend(&gObj[aIndex], AT_SKILL_COMBO, aIndex, 1);		
	}

	if( lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_EXPHELL )
	{	// �����̾�2 �ɰ� �ִµ� �ٸ� ��ų�� ��� ���� ��ų�� ���� �ǰ�... ��ų�� �����̾ �ߵ��Ѵ�
		CMagicInf *		lpMagic;
		lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

		if( lpMagic )
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic);	
		}
		return;
	}

#endif

	int usemana;
	usemana = GetUseMana(aIndex, lpMagic);

#ifdef MODIFY_BUGFIX_20081119	// ��ũȣ�� �����ũ ��ų ������ ���� �Һ�Ǵ� ���� ����
	if(lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK)
	{
		usemana = gObj[aIndex].Mana;
	}
#endif	// MODIFY_BUGFIX_20081119
	
	PMSG_DURATION_MAGIC_SEND	pMsg;
	
	if( usemana >= 0 )
	{
		
#ifdef DARKLORD_WORK
		if( !gObj[aIndex].SkillDelay.Check(lpMagic->m_Skill) )
		{
			LogAddTD("[%] ��ų �����̽ð� ����", gObj[aIndex].Name);
			return;
		}
#endif

#ifdef NEW_FORSKYLAND2
		int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
		if( usebp >= 0 )
		{				
			lpObj->UseMagicNumber = lpMagic->m_Skill;
			lpObj->UseMagicTime   = GetTickCount();
			lpObj->UseMagicCount  = 0;
			lpObj->Mana = (float)usemana;

			lpObj->BP = usebp;
			GCManaSend(lpObj->m_Index, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
			PHeadSetBE((LPBYTE)&pMsg, 0x1E, sizeof( pMsg ));

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD
			pMsg.MagicNumberH = HIBYTE( lpMagic->m_Skill );
			pMsg.MagicNumberL = LOBYTE( lpMagic->m_Skill );
#else
			pMsg.MagicNumber = lpMagic->m_Skill;
#endif
			pMsg.X = x;
			pMsg.Y = y;
			pMsg.Dir = dir;
			pMsg.NumberH = HIBYTE(aIndex);
			pMsg.NumberL = LOBYTE(aIndex);
			if( lpObj->Type == OBJTYPE_CHARACTER )
				DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

#ifdef WORLD_TOURNAMENT_EVENT_SETTING			
				MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
#else
	#ifdef CHAOSCASTLE_SYSTEM_20040408		// !!> . ���⼭ ī���� ĳ������ �������� �ʵǴ� ��ų���� �ߵ��Ǹ� ������ �ʴ´�.
				if (CHECK_CHAOSCASTLE(lpObj->MapNumber)) 
				{
	#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ī���� ĳ������ �������� �ʵǴ� ��ų �߰�
					if( g_MasterSkillSystem.CheckMasterLevelSkill( lpMagic->m_Skill ) )
					{
						INT iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill( lpMagic->m_Skill );
						switch( iBaseMLS )
						{
						case AT_SKILL_ML_STRENGTHEN_MAGICDEFENSE:	// �ҿ�ٸ��� ��ȭ
						case AT_SKILL_ML_STRENGTHEN_KNIGHTADDLIFE:	// ���������� ��ȭ
						case AT_SKILL_ML_STRENGTHEN_HEALING:		// ġ�� ��ȭ
			#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
						case AT_SKILL_ML_STRENGTHEN_DEFENSE:		// ����� ��� ��ȭ
						case AT_SKILL_ML_STRENGTHEN_ATTACK:			// ���ݷ� ��� ��ȭ
			#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
							break;
						default:
							MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
							break;							
						}
					}
					else
	#endif	// ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
					switch(lpMagic->m_Skill) 
					{
					case AT_SKILL_MAGICDEFENSE :	// �ҿ�ٸ���
					case AT_SKILL_KNIGHTADDLIFE :	// ����������
					case AT_SKILL_DEFENSE :			// �������
					case AT_SKILL_ATTACK :			// ���ݷ����
					case AT_SKILL_HEALING :			// ġ��
						break;
					default :
						MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
						break;
					}
				}
				else 
				{
					MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
				}
	#else
				MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
	#endif
#endif // WORLD_TOURNAMENT_EVENT_SETTING
		}
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// UseSkill -> g_MasterSkillSystem.RunningSkill_MLS
		if( g_MasterSkillSystem.CheckMasterLevelSkill( lpMagic->m_Skill ) )
		{
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428
			g_MasterSkillSystem.RunningSkill_MLS( lpObj, aTargetIndex, lpMagic, bCombo, x, y, TargetPos );
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
			g_MasterSkillSystem.RunningSkill_MLS( lpObj, aTargetIndex, lpMagic, bCombo );
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
			return;
		}
#endif
	
#ifdef ADD_SKILL_20031022		
		if( lpMagic->m_Skill == AT_SKILL_SWORD6 )
		{	// �Ŀ� ������ (���� ��ų)			
	#ifdef EXPAND_ATTACK_PACKET_0X1E
				SkillPowerSlash(aIndex, lpMagic, x, y, dir, TargetPos, aTargetIndex);
	#else
				SkillPowerSlash(aIndex, lpMagic, x, y, dir, TargetPos);
	#endif
		}		
#endif

#ifdef ADD_SKILL_WITH_COMBO	
		if( lpMagic->m_Skill == AT_SKILL_EXPPOISON )
		{	// ���� ������
			SkillDeathPoison(aIndex, lpMagic, x, y, dir, TargetPos, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_EXPICE )
		{	// ���� ���̽�
			SkillSuddenIce(aIndex, lpMagic, x, y, dir, TargetPos, aTargetIndex);
		}		
		else if( lpMagic->m_Skill == AT_SKILL_BLOWOFFURY )
		{	// �г��� �ϰ�
			SkillBlowOfFury(aIndex, lpMagic, x, y, dir, TargetPos, aTargetIndex, bCombo);
		}
		else if( lpMagic->m_Skill == AT_SKILL_WHEEL )
		{	// ȸ��������
			SkillWheel(aIndex, lpMagic, aTargetIndex, bCombo);
		}			
#endif
		
#ifdef DARKLORD_WORK		
		else if( lpMagic->m_Skill == AT_SKILL_ELECTRICSPARK )
		{	// �Ϸ�Ʈ������ũ
			SkillElectricSpark(aIndex, lpMagic, x, y, dir, TargetPos, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK )
		{	// ��ũȣ��			
			SkillDarkHorseAttack(lpObj->m_Index, aTargetIndex, lpMagic);
		}
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
		else if( lpMagic->m_Skill == AT_SKILL_STUN)
		{	// ����
			SkillStun(aIndex, aTargetIndex, lpMagic, x, y, dir, TargetPos);
		}		
#endif

#ifdef ADD_ITEM_FENRIR_01_20051110
		else if( lpMagic->m_Skill == AT_SKILL_FENRIR_ATTACK)
		{
			SkillFenrirAttack(aIndex, aTargetIndex, lpMagic);
		}
#endif

#ifdef NEW_ATTACK_PROC
		if( lpMagic->m_Skill == AT_SKILL_BLOWOFFURY )
		{	// �г��� �ϰ�
			SkillBlowOfFury(aIndex, lpMagic, x, y, dir, TargetPos);
		}
		/*
		*/
		else if( lpMagic->m_Skill == AT_SKILL_PENETRATION )
		{	// ����
			SkillPenetration(aIndex, lpMagic, x, y, dir, TargetPos);
		}
		else if( lpMagic->m_Skill == AT_SKILL_DEFENSEDOWN )
		{	// ���˻� ��� ����
			SkillDefenseDown(aIndex, lpMagic, x, y, dir, TargetPos);
		}		
		else if( lpMagic->m_Skill == AT_SKILL_EVIL )
		{	// �Ƿ�
			SkillEvil(aIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_HELL )
		{	// �����̾�
			SkillHellFire(aIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_INFERNO )
		{	// ���丣��
			SkillHellFire(aIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_WHEEL )
		{	// ȸ��������
			SkillWheel(aIndex, lpMagic);
		}	
#endif

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117
		// �Ƿ�
		else if( lpMagic->m_Skill == AT_SKILL_EVIL )
		{
			SkillNewEvil( aIndex, aTargetIndex, lpMagic );
		}
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
		else if( lpMagic->m_Skill == AT_SKILL_LIGHTNINGORB )
		{
			SkillLightningOrb( aIndex, aTargetIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_DRAINLIFE )
		{
			SkillDrainLife(aIndex, aTargetIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_CHAINLIGHTNING )
		{
			SkillChainLightning(aIndex, aTargetIndex, lpMagic);
		}
		else if( lpMagic->m_Skill == AT_SKILL_EXPLOSION_2	// ȯ�� ��ų
			|| lpMagic->m_Skill == AT_SKILL_REQUIEM )
		{
			SkillSummons(aIndex, lpMagic, x, y, aTargetIndex);
		}
#endif

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
		else if( lpMagic->m_Skill == AT_SKILL_WEAKNESS	)
		{
			SkillWeakness(aIndex, lpMagic, x, y, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_ENERVATION	)
		{
			SkillEnervation(aIndex, lpMagic, x, y, aTargetIndex);
		}
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425
		else if( lpMagic->m_Skill == AT_SKILL_PULLUTION )
		{
			UseSkillPollution(aIndex, lpMagic, x, y, aTargetIndex);
		}
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425

#ifdef SEASON4_PATCH_GROOVING
		// --->
		// ���� 4.0 �߰� ��ų  grooving 2008/04/24
		else if( lpMagic->m_Skill == AT_SKILL_LIGHTING_SHOCK )
		{
			SkillLightningShock( aIndex, aTargetIndex, lpMagic );
		}
		// <---
#endif

#ifdef ADD_SEASON4_NEW_SKILL_20080522	// ��ų ���
		else if( lpMagic->m_Skill == AT_SKILL_DESTRUCTION )
		{
			SkillDestruction(aIndex, lpMagic, x, y, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_FLAMESTRIKE)
		{
			SkillFlameStrike(aIndex, lpMagic, TargetPos, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_GIGANTICSTORM)
		{
			SkillGiganticStorm(aIndex, lpMagic, x, y, aTargetIndex);
		}
		else if( lpMagic->m_Skill == AT_SKILL_MULTISHOT)
		{
			SkillMultiShot(aIndex, lpMagic, TargetPos, aTargetIndex);
		}
		else if(lpMagic->m_Skill == AT_SKILL_CHAOTICDESAIR)
		{
			SkillchaoticDesair(aIndex, lpMagic, TargetPos, aTargetIndex);
		}
#endif	// ADD_SEASON4_NEW_SKILL_20080522
	}
}

void CObjUseSkill::UseSkill(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	BOOL	bCombo = FALSE;
#ifdef ADD_SKILL_WITH_COMBO
	if( gComboAttack.CheckCombo(aIndex, lpMagic->m_Skill) )
	{
		bCombo = TRUE;
		//GCMagicAttackNumberSend(&gObj[aIndex], AT_SKILL_COMBO, 0, 1);
	}

	if( lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_EXPHELL )
	{	// �����̾�2 �ɰ� �ִµ� �ٸ� ��ų�� ��� ���� ��ų�� ���� �ǰ�... ��ų�� �����̾ �ߵ��Ѵ�
		CMagicInf *		lpMagic;
		lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

		if( lpMagic )
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic);	
		}
		return;
	}
#endif	

#ifdef ADD_SKILL_WITH_COMBO	
	if( lpMagic->m_Skill == AT_SKILL_EXPHELL && aTargetIndex != AT_SKILL_EXPHELL_START )
	{	// �����̾�2 �ߵ��� ������ ������ �ȵȴ�
		RunningSkill(aIndex, aTargetIndex, lpMagic, bCombo);		
		lpObj->UseMagicNumber = lpMagic->m_Skill;
		lpObj->UseMagicTime   = GetTickCount();
		lpObj->UseMagicCount  = 0;
		return;
	}
#endif

	int usemana = GetUseMana(aIndex, lpMagic);
	if( usemana >= 0 )
	{
#ifdef DARKLORD_WORK
		if( !gObj[aIndex].SkillDelay.Check(lpMagic->m_Skill) )
		{
			LogAddTD("[%] ��ų �����̽ð� ����", gObj[aIndex].Name);
			return;
		}
#endif
#ifdef NEW_FORSKYLAND2
		int usebp = GetUseBP(aIndex, lpMagic);
		if( usebp >= 0 )
		{
			if( RunningSkill(aIndex, aTargetIndex, lpMagic, bCombo) == TRUE )
			{
				lpObj->UseMagicNumber = lpMagic->m_Skill;
				lpObj->UseMagicTime   = GetTickCount();
				lpObj->UseMagicCount  = 0;
				lpObj->Mana = (float)usemana;
				lpObj->BP = usebp;
				GCManaSend(aIndex, (short)lpObj->Mana, 0xFF, 0, lpObj->BP);
			}
		}
#endif
	}
}

BOOL CObjUseSkill::RunningSkill(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL bCombo)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	INT MagicNumber;
#else
	BYTE MagicNumber;
#endif

	MagicNumber			 = lpMagic->m_Skill;
	lpObj->m_SkillNumber = MagicNumber;

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	// �����ڰ� ���� ���¿� �ִٸ� ������¸� �����Ѵ�.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INVISABLE ) == true )
	#else
	if( lpObj->m_iSkillInvisibleTime > 0 )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
		if( ( lpObj->Authority & AUTHORITY_EVENT_GM ) != AUTHORITY_EVENT_GM )
		{
			this->RemoveCloakingEffect(lpObj->m_Index);
		}
	#else
		this->RemoveCloakingEffect(lpObj->m_Index);
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_05_20071220
	}
#endif

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �����ͷ��� ��ų ó���� ���� ����.
	if( g_MasterSkillSystem.CheckMasterLevelSkill( lpMagic->m_Skill ) )
	{
		return g_MasterSkillSystem.RunningSkill_MLS( lpObj, aTargetIndex, lpMagic, bCombo );
	}
#endif
	
	switch(MagicNumber)
	{
	case AT_SKILL_BLOCKING :
		// �������� �ִ��� Ȯ��..
		if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].IsItem() == TRUE )
		{
			if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type >= ITEM_SHIELD+4 &&
				lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Type < ITEM_SHIELD+MAX_ITEM_INDEX )
			{
				if( lpObj->pInventory[EQUIPMENT_WEAPON_LEFT].m_Option1 )
				{
					lpObj->m_SkillTime   = GetTickCount()+(4000);
#ifdef ADD_TRAP_OBJECT_EXTEND_20060731
					GCActionSend(lpObj, AT_SKILL_BLOCKING, lpObj->m_Index, aTargetIndex );
#else
					GCActionSend(lpObj, AT_SKILL_BLOCKING, lpObj->m_Index);
#endif // ADD_TRAP_OBJECT_EXTEND_20060731
				}
			}
		}
		break;
	case AT_SKILL_POISON :
	case AT_SKILL_METEO  :
	case AT_SKILL_THUNDER:
	case AT_SKILL_ENERGYBALL :
	case AT_SKILL_FLAME :
	case AT_SKILL_SWORD1 :	// ������� ����� ��ų
	case AT_SKILL_SWORD2 :
	case AT_SKILL_SWORD3 :
	case AT_SKILL_SWORD4 :
	case AT_SKILL_SWORD5 :
#ifdef ADD_SKILL_20031022
	case AT_SKILL_SWORD6 :
#endif			
	case AT_SKILL_SLOW :
	case AT_SKILL_BOW :
	case AT_SKILL_FIREBALL :
	case AT_SKILL_CROSSBOW :	
	case AT_SKILL_WHEEL :
	case AT_SKILL_BLOWOFFURY :

#ifdef NEW_SKILL_FORSKYLAND
	//case AT_SKILL_SWORDAURORA :
	case AT_SKILL_KNIGHTSPEAR :	
	case AT_SKILL_PENETRATION :
#endif
		gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0);
		break;
	case AT_SKILL_HEALING :	// ġ��
		SkillHealing(aIndex, aTargetIndex, lpMagic->m_Level);
		break;

#ifdef NEW_SKILL_FORSKYLAND
	case AT_SKILL_MAGICDEFENSE :	// ���� ����
#ifdef NEW_FORSKYLAND3
		WizardMagicDefense(aIndex, aTargetIndex, lpMagic->m_Level);
#else
		WizardMagicDefense(aIndex, lpMagic->m_Level);
#endif
		break;

	case AT_SKILL_KNIGHTADDLIFE :		// ����� ����
		KnightSkillAddLife(aIndex, lpMagic->m_Level);
		break;
#endif
	case AT_SKILL_DEFENSE :	// ���� ����
		SkillDefense(aIndex, aTargetIndex, lpMagic->m_Level);
		break;
	case AT_SKILL_ATTACK :
		SkillAttack(aIndex, aTargetIndex, lpMagic->m_Level);
		break;
	case AT_SKILL_CALLMON1 : return SkillMonsterCall(lpObj->m_Index, 26, lpObj->X-1, lpObj->Y+1);	// ���
	case AT_SKILL_CALLMON2 : return SkillMonsterCall(lpObj->m_Index, 32, lpObj->X-1, lpObj->Y+1);	// ������
	case AT_SKILL_CALLMON3 : return SkillMonsterCall(lpObj->m_Index, 21, lpObj->X-1, lpObj->Y+1);	// �ϻ���
	case AT_SKILL_CALLMON4 : return SkillMonsterCall(lpObj->m_Index, 20, lpObj->X-1, lpObj->Y+1);	// ���δ���
	case AT_SKILL_CALLMON5 : return SkillMonsterCall(lpObj->m_Index, 10, lpObj->X-1, lpObj->Y+1);	// ��ũ����Ʈ
	case AT_SKILL_CALLMON6 : return SkillMonsterCall(lpObj->m_Index, 150, lpObj->X-1, lpObj->Y+1);	// �߸�
	case AT_SKILL_CALLMON7 : return SkillMonsterCall(lpObj->m_Index, 151, lpObj->X-1, lpObj->Y+1);	// ����

#ifdef ADD_SKILL_WITH_COMBO
	//case AT_SKILL_EXPPOISON : return SkillDeathPoison(lpObj->m_Index, aTargetIndex, lpMagic);
	//case AT_SKILL_EXPICE : return SkillSuddenIce(lpObj->m_Index, aTargetIndex, lpMagic);
	case AT_SKILL_EXPHELL : return SkillHellFire2(lpObj->m_Index, aTargetIndex, lpMagic);

	case AT_SKILL_STRIKE : return SkillKnightBlow(lpObj->m_Index, aTargetIndex, lpMagic, bCombo);	// ��ο�(���)	

	//case AT_SKILL_EXPHELL_START : return SkillHellFire2Start(lpObj->m_Index, lpMagic);	// �����̾�2 ����Ѵٰ� �˸�
#endif

#ifdef DARKLORD_WORK
	case AT_SKILL_SPEAR :			return SkillSpear(lpObj->m_Index, aTargetIndex, lpMagic);	// ���Ǿ�(��ũ�ε�)
	case AT_SKILL_FIREBURST :		return SkillFireBurst(lpObj->m_Index, aTargetIndex, lpMagic);	// ���̾����Ʈ(��ũ�ε�)
	case AT_SKILL_DARKHORSE_ATTACK :	return SkillDarkHorseAttack(lpObj->m_Index, aTargetIndex, lpMagic);	// ��ũȣ�� ����(��ũ�ε�)

	case AT_SKILL_RECALL_PARTY:		    SkillRecallParty(aIndex, lpMagic->m_Level);			break;		// ��Ƽ ��ȯ(��ũ�ε�)
	case AT_SKILL_ADD_CRITICALDAMAGE :	SkillAddCriticalDamage(aIndex, lpMagic->m_Level);	break;		// ũ��Ƽ�� �������߰�(��ũ�ε�)	
#endif
#ifdef ADD_NEW_MAP_KALIMA_20040518
	case AT_SKILL_DEFENSEDOWN :
		{
			MaGumSkillDefenseDown		(lpObj->m_Index, aTargetIndex, 1);
		}
		break;
	case AT_SKILL_SUMMON :
		{
			SkillSummon			(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_IMMUNE_TO_MAGIC :
		{
			SkillImmuneToMagic	(lpObj->m_Index, lpMagic);
		}
		break;
	case AT_SKILL_IMMUNE_TO_HARM :
		{
			SkillImmuneToHarm	(lpObj->m_Index, lpMagic);
		}
		break;
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	case AT_SKILL_RUSH :
		{	// ���̵峪��Ʈ
			SkillKnightRush			(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_JAVALIN :
		{	// �ҿ� ������
			SkillWizardJavalin		(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_DEEPIMPACT :
		{	// ���� ����
			SkillElfRush			(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_ONE_FLASH :
		{	// ����� ���˻�
			SkillMagumOneFlash		(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_DEATH_CANNON :
		{	// ������ ���˻�
			SkillMagumDeathCannon	(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;
	case AT_SKILL_SPACE_SPLIT :
		{	// ��ũ�ε� ���� ��ų
			SkillDarklordSpaceSplit	(lpObj->m_Index, aTargetIndex, lpMagic);
		}
		break;

	case AT_SKILL_BRAND_OF_SKILL :
		{	// ��ũ�ε� ���� ��ų
			SkillBrandOfSkill(aIndex, aTargetIndex, lpMagic);
		}
		break;
	
	case AT_SKILL_REMOVAL_STUN :
		{	// ���� ���� - ����å��ų	
			SkillRemoveStun(aIndex, aTargetIndex, lpMagic);
		}
		break;

	case AT_SKILL_ADD_MANA :
		{	// ���� ���� - ����å��ų
			SkillAddMana(aIndex, aTargetIndex, lpMagic);
		}
		break;

	case AT_SKILL_INVISIBLE :
		{	// ���� - ����å��ų
			SkillCloaking(aIndex, aTargetIndex, lpMagic);
		}
		break;

	case AT_SKILL_REMOVAL_INVISIBLE :
		{	// ���� ���� - ����å��ų
			SkillRemoveCloaking(aIndex, aTargetIndex, lpMagic);
		}
		break;

	case AT_SKILL_REMOVAL_MAGIC :
		{	// ���� ���� - ����å��ų
			SkillRemoveMagic(aIndex, aTargetIndex, lpMagic);
		}
		break;
#endif

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	case AT_SKILL_INFINITY_ARROW:
		{	// ����� - ���Ǵ�Ƽ �ַο�
			SkillInfinityArrow( aIndex, aTargetIndex, lpMagic );
		}
		break;
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ȯ���� ���ֽ�ų
	case AT_SKILL_THORNS:
		{
			SkillThorns( aIndex, aTargetIndex, lpMagic );
		}
		break;

	case AT_SKILL_SLEEP:
		{
			SkillSleep( aIndex, aTargetIndex, lpMagic );	
		}
		break;
		
	case AT_SKILL_BLIND:
		{
			SkillBlind( aIndex, aTargetIndex, lpMagic );	
		}
		break;
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef SEASON4_PATCH_GROOVING
		//-->
		// ���� 4.0 �߰� ��ų  grooving 2008/04/24 : ����Ŀ
	case AT_SKILL_BERSERKER:
		{
			SkillBerserker(lpObj->m_Index, lpMagic);
		}
		break;
		//<--
#endif

#ifdef ADD_SEASON4_NEW_SKILL_20080522	// ȸ��, ��������
	case AT_SKILL_RECOVERY:	// ȸ�� ��ų ���
		{
			SkillRecovery( aIndex, aTargetIndex, lpMagic );
		}
		break;
				
	case AT_SKILL_MAGICPOWRER:	//  �������� ��ų ���
		{
			SkillMagicPower( aIndex, aTargetIndex, lpMagic );
		}
		break;
#endif	// ADD_SEASON4_NEW_SKILL_20080522

	default :
		gObjAttack(lpObj, lpTargetObj, lpMagic, 1, 0);	
		break;
	}
	return TRUE;
}

#ifdef NEW_SKILL_FORSKYLAND
// ���˻���°���
void CObjUseSkill::MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

#ifdef ADD_NEW_MAP_KALIMA_20040518								// ���͵� ��������� �� �� �־�� �Ѵ�.
	if( lpObj->Type == OBJTYPE_CHARACTER) {
		// ����� �߿����� ���˻縸 �� �� �ִ�.
		if( lpObj->Class != CLASS_MAGUMSA ) return;
	}
#else
	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_MAGUMSA ) return;
#endif

	LogAdd("���˻� ���͹��°��� : %d", 10);

	if( retCalcSkillResistance(lpTargetObj, TRUE)  )
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_DEC, EFFECTTYPE_DECREASE_DEFENSE, 10, 0, 0, 10 );
#else
		lpTargetObj->m_SkillMagumReduceDefense = 10;
		lpTargetObj->m_SkillMagumReduceDefenseTime = 10;
		lpTargetObj->m_ViewSkillState |= STATE_REDUCE_DEFENSE;	//	7��° ��Ʈ : ���˻� ���� ����
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		GCMagicAttackNumberSend(lpObj, AT_SKILL_DEFENSEDOWN, lpTargetObj->m_Index, skillSuccess);
	}
}

// ����������(��� ����� ����)
void CObjUseSkill::KnightSkillAddLife(int aIndex, int skill_level)
{
	int skillSuccess = 1;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];


#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[14] != AUTH_CHECK_14 )
		DestroyGIocp();
#endif

#ifdef NEW_FORSKYLAND2
	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGUMSA ) return;

	int number;
	int partynum=0;
	int totallevel=0;
	int partycount;
	int dis;
	int ApplyPartyIndex[MAX_PARTYUSER];	

	int viewplayer=0;
	int viewpercent = 0;
	
	LPOBJECTSTRUCT lpPartyObj;

	memset(ApplyPartyIndex, -1, sizeof(int)*MAX_PARTYUSER);
	partynum = lpObj->PartyNumber;	

	if( partynum != -1 )
	{
		partycount = gParty.m_PartyS[partynum].Count;
		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];			
				// ���� �ʿ� �����ϰų�..
				if( lpObj->MapNumber == lpPartyObj->MapNumber) 
				{
					dis = gObjCalDistance(lpObj, &gObj[number]);
					if( dis < 10 )
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}	

	if( viewplayer > 1 )
	{		
		if( viewplayer == 2 )	viewpercent = 1;
		else if( viewplayer == 3 )	viewpercent = 2;
		else if( viewplayer == 4 )	viewpercent = 3;		
		else viewpercent = 4;
	}	

	int iaddLifepower = 12+(lpObj->Vitality+lpObj->AddVitality)/g_GlobalConfig.m_iInnerVitality + 
							   (lpObj->Energy+lpObj->AddEnergy)/g_GlobalConfig.m_iInnerEnergy+viewpercent;

	if(iaddLifepower > g_GlobalConfig.m_iInnerMaxHP) iaddLifepower = g_GlobalConfig.m_iInnerMaxHP;

	int iLifeTime  = 60+(lpObj->Energy+lpObj->AddEnergy)/g_GlobalConfig.m_iInnerTime;
	int iaddlife;

	//iaddLifepower += (int)(lpObj->MaxLife*viewpercent/100);	

	if( partynum == -1 )
	{	// ��Ƽ�� �ȵǾ� �ִ� �����̴�
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		iaddlife = (int)lpObj->MaxLife * iaddLifepower / 100;

		gObjAddBuffEffect( lpObj, BUFFTYPE_HP_INC, EFFECTTYPE_HP, iaddlife, 0, 0, iLifeTime );
#else
		iaddlife = (int)lpObj->MaxLife*iaddLifepower/100;

		lpObj->AddLife -= lpObj->m_SkillAddLife;
		
		lpObj->AddLife += iaddlife;	// �ִ� ����� ������ ������ �ش�
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0);
#endif

		lpObj->m_SkillAddLife = iaddlife;
		lpObj->m_SkillAddLifeTime = iLifeTime;

		lpObj->m_ViewSkillState |= STATE_ADD_LIFE;  //����� ����.
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		GCMagicAttackNumberSend(lpObj, AT_SKILL_KNIGHTADDLIFE, lpObj->m_Index, skillSuccess);		
	}
	else
	{	// ��Ƽ ����
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			if( ApplyPartyIndex[n] != -1 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[ApplyPartyIndex[n]];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				iaddlife = (int)lpPartyObj->MaxLife * iaddLifepower / 100;
				gObjAddBuffEffect( lpPartyObj, BUFFTYPE_HP_INC, EFFECTTYPE_HP, iaddlife, 0, 0, iLifeTime );
#else

				iaddlife = (int)lpPartyObj->MaxLife*iaddLifepower/100;

				lpPartyObj->AddLife -= lpPartyObj->m_SkillAddLife;
				lpPartyObj->AddLife += iaddlife;	// �ִ� ����� ������ ������ �ش�

#ifdef ADD_SHIELD_POINT_01_20060403
				GCReFillSend(lpPartyObj->m_Index, (WORD)(lpPartyObj->MaxLife+lpPartyObj->AddLife), 0xFE, 0, (WORD)(lpPartyObj->iMaxShield+lpPartyObj->iAddShield));
#else
				GCReFillSend(lpPartyObj->m_Index, (WORD)(lpPartyObj->MaxLife+lpPartyObj->AddLife), 0xFE, 0);
#endif

				lpPartyObj->m_SkillAddLife = iaddlife;

#ifdef MODIFY_ACCUMULATED_BUG_SWELLLIFE_ON_PARTY_01_20050531
				lpPartyObj->m_SkillAddLifeTime = iLifeTime;
#else
				lpPartyObj->m_SkillAddLifeTime = 60;
#endif

				lpPartyObj->m_ViewSkillState |= STATE_ADD_LIFE;  //����� ����.
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				GCMagicAttackNumberSend(lpObj, AT_SKILL_KNIGHTADDLIFE, lpPartyObj->m_Index, skillSuccess);
			}
		}
	}
#endif
}

// �����������(�ҿ�ٸ���)
#ifdef NEW_FORSKYLAND3
void CObjUseSkill::WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level)
#else
void CObjUseSkill::WizardMagicDefense(int aIndex, int skill_level)
#endif
{
#ifdef NEW_FORSKYLAND3
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
	
	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA ) return;

	if( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{	// ��Ƽ�� �ƴϸ� ����Ҽ� ����
		return;
	}

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iDefenseValue = 0;
	int iSkillValidTime = 0;

	iDefenseValue = 10 + ( ( lpObj->Dexterity + lpObj->AddDexterity ) / 50 ) + ( ( lpObj->Energy + lpObj->AddEnergy ) / 200 );
	iSkillValidTime = 60 + ( ( lpObj->Energy + lpObj->AddEnergy ) / 40 );

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// �ҿ�ٸ��� �Ϲݽ�ų : �����Ҹ� ����( �����ý��ۿ� �߰� )
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, iDefenseValue, EFFECTTYPE_MELEE_DEFENSE_DOWN_MANA, 20, iSkillValidTime );
#else
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_MELEE_DEFENSE_INC, EFFECTTYPE_IMPROVE_MELEE_DEFENSE, iDefenseValue, 0, 0, iSkillValidTime );
#endif
	GCMagicAttackNumberSend(lpObj, AT_SKILL_MAGICDEFENSE, lpTargetObj->m_Index, skillSuccess);
#else
	#ifdef WIZARD_UPGRADE
		lpTargetObj->m_WizardSkillDefense	= 10+(lpObj->Dexterity+lpObj->AddDexterity)/g_GlobalConfig.m_iShieldDexterity+(lpObj->Energy+lpObj->AddEnergy)/g_GlobalConfig.m_iShieldEnergy;
	#else
		lpTargetObj->m_WizardSkillDefense	= 10+(lpObj->Dexterity+lpObj->AddDexterity)/g_GlobalConfig.m_iShieldDexterity+(lpObj->Energy+lpObj->AddEnergy)/g_GlobalConfig.m_iShieldEnergy;
	#endif

		if(lpTargetObj->m_WizardSkillDefense > g_GlobalConfig.m_iShieldMaxDefense) lpTargetObj->m_WizardSkillDefense = g_GlobalConfig.m_iShieldMaxDefense;

		lpTargetObj->m_WizardSkillDefenseTime = 60+(lpObj->Energy+lpObj->AddEnergy)/g_GlobalConfig.m_iShieldTime;	

	#ifdef CHANGE_VIEWSTATE
		lpTargetObj->m_ViewSkillState |= STATE_REDUCE_MAGICDEFENSE; //	9��° ��Ʈ : ���� ���� ����
	#else
		lpTargetObj->m_ViewSkillState |= 0;						  //	4��° ��Ʈ : ���� ���� ���� ǥ��
	#endif;
		GCMagicAttackNumberSend(lpObj, AT_SKILL_MAGICDEFENSE, lpTargetObj->m_Index, skillSuccess);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004	
	
#else
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	#ifdef NEW_FORSKYLAND2

		if( lpObj->Type != OBJTYPE_CHARACTER) 
			if( lpObj->m_RecallMon == -1 ) return;

		if( lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGUMSA ) return;

		lpObj->m_WizardSkillDefense	= 7;
		lpObj->m_WizardSkillDefenseTime = 30;	

		#ifdef CHANGE_VIEWSTATE
			lpObj->m_ViewSkillState |= STATE_REDUCE_MAGICDEFENSE; //	9��° ��Ʈ : ���� ���� ����
		#else
			lpObj->m_ViewSkillState |= 0;						  //	4��° ��Ʈ : ���� ���� ���� ǥ��
		#endif;
			GCMagicAttackNumberSend(lpObj, AT_SKILL_MAGICDEFENSE, lpObj->m_Index, skillSuccess);
		#endif
	#endif
}
#endif



//�������� (��30�ʰ� (�ƾ��۷���+1)*3��ŭ ����)
void CObjUseSkill::SkillDefense(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	float skill_defense;

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;

	if( lpObj->m_bBuffHelper == false && lpObj->Class != CLASS_ELF ) return;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ������ ��������� �ڽſ��Ը� ���� �� �ִ�.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if( skill_level < 0 || skill_level > 7 ) skill_level = 0;

	skill_defense = (float)((skill_level+1)*2+((lpObj->Energy+lpObj->AddEnergy)/8));
	skill_defense *= g_GlobalConfig.m_fPlusDef;

	// ����� ��� -10%, +10% �� ������ �ش�.
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		skill_defense += (skill_defense/10);
	}
	//LogAdd("�������� ���� ��� 2 : %f", skill_defense);
	
#ifdef MODIFY_ELF_DEFENCE_ATTACK_VALUE_UP_SKILL_20070110
	// �̹� ����� ��ġ���� ������ ����/�������� �ʴ´�.
	#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	#ifdef MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
		if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, skill_defense, 0 ) == true )
	#else
		if( gObjCheckPowerfulEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC, skill_defense, 0 ) == true )
	#endif // MODIFY_BUFFSYSTEM_BUGFIX_07_20080109
	#else
	if( lpTargetObj->m_SkillDefense > skill_defense )
	#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
		GCMagicAttackNumberSend( lpObj, AT_SKILL_DEFENSE, lpTargetObj->m_Index, 0 );	// ��ų��� ����		
		return;
	}
#endif

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_DEFENSE_POWER_INC, EFFECTTYPE_IMPROVE_DEFENSE, skill_defense, 0, 0, 60 );
#else
	lpTargetObj->m_SkillDefense = (int)skill_defense;

	if(lpObj->m_bBuffHelper == true)
		lpTargetObj->m_SkillDefenseTime = GetTickCount()+(g_BuffHelper.m_iAuraTime*1000);
	else
		lpTargetObj->m_SkillDefenseTime = GetTickCount()+(g_GlobalConfig.m_iDefenseAuraTime*1000);
#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_DEFENSE );
#else
	lpTargetObj->m_ViewSkillState |= 0x08; //	4��° ��Ʈ : ���� ���� ���� ǥ��
#endif
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// ���� ���� ��� �� Target�� ���� �ִ� ������ Agro�� ���� �����ش�.
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if( lpMonster->m_iCurrentAI )
					lpMonster->m_Agro.IncAgro(lpObj->m_Index, (int)skill_defense / 10 );
			}
		}
	}
#endif

	GCMagicAttackNumberSend(lpObj, AT_SKILL_DEFENSE, lpTargetObj->m_Index, skillSuccess);
}

//���ݷ����� (��30�ʰ� (�ƾ��۷���+1)*3��ŭ ����)
void CObjUseSkill::SkillAttack(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
	
	float skill_attack;

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;

	if( lpObj->m_bBuffHelper == false && lpObj->Class != CLASS_ELF ) return;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ������ ���ݷ� ����� �ڽſ��Ը� ���� �� �ִ�.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if( skill_level < 0 || skill_level > 7 ) skill_level = 0;

	skill_attack = ((skill_level+1)*3+((float)(lpObj->Energy+lpObj->AddEnergy)/7))*g_GlobalConfig.m_fPlusDmg;

	//LogAdd("���ݷ����� ���� ��� 1 : %f", skill_attack);
	// ����� ��� -10%, +10% �� ������ �ش�.
	if( lpTargetObj->Class == CLASS_KNIGHT )
	{
		skill_attack += (skill_attack/10);
	}
	//LogAdd("���ݷ����� ���� ��� 2 : %f", skill_attack);

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, skill_attack, 0 ) == true )
	{
		GCMagicAttackNumberSend( lpObj, AT_SKILL_ATTACK, lpTargetObj->m_Index, 0 );	// ��ų��� ����
		return;
	}

	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_ATTACK_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, skill_attack, 0, 0, 60 );
#else
	#ifdef MODIFY_ELF_DEFENCE_ATTACK_VALUE_UP_SKILL_20070110
	// �̹� ����� ��ġ���� ������ ����/�������� �ʴ´�.
	if( lpTargetObj->m_SkillAttack > skill_attack )
	{
		GCMagicAttackNumberSend( lpObj, AT_SKILL_ATTACK, lpTargetObj->m_Index, 0 );	// ��ų��� ����
		return;
	}
	#endif

	lpTargetObj->m_SkillAttack = (int)skill_attack;

	if(lpObj->m_bBuffHelper == true)
		lpTargetObj->m_SkillAttackTime = GetTickCount()+(g_BuffHelper.m_iAuraTime*1000);
	else
		lpTargetObj->m_SkillAttackTime = GetTickCount()+(g_GlobalConfig.m_iDamageAuraTime*1000);

#ifdef EXPAND_VIEWSKILLSTATE_WORD_TO_INT_20041122
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_ATTACK );
#else
	lpTargetObj->m_ViewSkillState |= 0x04;	//	3��° ��Ʈ : ���� ���ݷ� ���� ǥ��
#endif

#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	
#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// ���ݷ� ���� ��� �� Target�� ���� �ִ� ������ Agro�� ���� �����ش�.
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if( lpMonster->m_iCurrentAI )
					lpMonster->m_Agro.IncAgro(lpObj->m_Index, (int)skill_attack / 10);
			}
		}
	}


#endif

	GCMagicAttackNumberSend(lpObj, AT_SKILL_ATTACK, lpTargetObj->m_Index, skillSuccess);
}

// ���� ���� ���
BOOL CObjUseSkill::SkillChangeUse(int aIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int skill_level = -1;
	if( lpObj->Type != OBJTYPE_CHARACTER) return FALSE;
			
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM(13,10) )
		{			
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else skill_level = lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Level;
		}
	}
	if( (lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE) && skill_level == -1 )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM(13,10) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else skill_level = lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Level;

		}		
	}
	if( skill_level > 7 ) return FALSE;
	
#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ����Ʈ�ذ����纯�Ź��� ���� ó��
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{			
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else skill_level = 372;
		}
	}
	if( (lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE) && skill_level == -1 )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 39 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else skill_level = 372;
		}		
	}
#endif
	
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 373;
			}
		}
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 40 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 373;
			}
		}
	}
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
	
#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 374;	// �ӽ�
			}
		}
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 41 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 374;	// �ӽ�
			}
		}
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	
#ifdef UPDATE_GM_FUNCTION_20070228
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
			{
				return FALSE;
			}
			else
			{
				skill_level = NEW_GM_CHARACTER;
			}
		}
	}

	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 42 ) )
		{
			if( (lpObj->Authority&AUTHORITY_EVENT_GM) != AUTHORITY_EVENT_GM )
			{
				return FALSE;
			}
			else
			{
				skill_level = NEW_GM_CHARACTER;
			}
		}
	}
#endif // UPDATE_GM_FUNCTION_20070228

#ifdef MODIFY_ILLUSION_TEMPLE_20070629	// ȯ����������� ������ ��Ǯ����
	if(CHECK_ILLUSION_TEMPLE(lpObj->MapNumber) 
		&& g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) != ILLUSION_TEMPLE_STATE_NONE)
	{
		int team = g_IllusionTempleEvent.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);
		if(team == TEAM_ALLIED)
		{
			skill_level = ALLIED_CHARACTER;
		}
		else if(team == TEAM_ILLUSION)
		{
			skill_level = ILLUSION_CHARACTER;
		}
	}
#endif
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 477;//Monster ��ũ��Ʈ �ε���
			}
		}
	}
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 68 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 477;//Monster ��ũ��Ʈ �ε���
			}
		}
	}
#endif //ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
	if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_RIGHT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 503;//Monster ��ũ��Ʈ �ε���
			}
		}
	}
	if( lpObj->pInventory[EQUIPMENT_RING_LEFT].IsItem() == TRUE )
	{
		if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Type == MAKE_ITEMNUM( 13, 76 ) )
		{
			if( lpObj->pInventory[EQUIPMENT_RING_LEFT].m_Durability < 1 )
			{
				skill_level = -1;
			}
			else
			{
				skill_level = 503;//Monster ��ũ��Ʈ �ε���
			}
		}
	}
#endif //ADD_PREMIUMITEM_PANDA
	
#ifdef MODIFY_UPDATING_VIEWPORT_20050118
	// ���� �� ������ ������ ��� ĳ���� �ٽ� �׸��� �ʴ´�.(����/â����)

	// �ڽ��� ���� ������ �ٽ� ������ ����/â�� �ٽ� �׸����� �Ѵ�. 
	#ifndef MODIFY_VIEWPORT_REMOVE_CHANGE_RING_20051013
		if( skill_level == -1 ) return FALSE;		
	#endif
		
#endif

	int change=-1;
	switch( skill_level )
	{
		case 0 :  change = 2;	break;		// �����巡��
		case 1 :  change = 7;	break;		// ���̾�Ʈ 
		case 2 :  change = 14;	break;		// �ذ�����
		case 3 :  change = 8;	break;		// ������ �һ�����
		case 4 :  change = 9;	break;		// ��� ��ġ
		case 5 :  change = 41;	break;		// ����ī��

#ifdef EVENT_SERVER_OPEN_COMMEMORATION_20050510
		case 6 :
		{
			if(lpObj->Authority & AUTHORITY_EVENT_GM) // ������ ������ ���
				change = 275; // ��� ����
			else // ������ ������ ���ٸ� ���� ĳ���͸� ���� ���� �ʴ´�.
				return FALSE;
		}
		break;
#endif // EVENT_SERVER_OPEN_COMMEMORATION_20050510

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ����Ʈ�ذ����纯�Ź��� ���� ó��
		case 372:					// ����Ʈ�ذ����纯�Ź���
			change = skill_level;
			break;
#endif
			
#ifdef HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017
		case 373:
			change = skill_level;
			break;
#endif // HALLOWINDAY_JACK_O_LANTERN_MORPH_RING_20061017


#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		case 374:
			change = skill_level;
			break;
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117

#ifdef UPDATE_GM_FUNCTION_20070228
		case NEW_GM_CHARACTER:
			change = skill_level;
			break;
#endif 

#ifdef MODIFY_ILLUSION_TEMPLE_20070629	// ȯ����������� ������ ��Ǯ����
		case ALLIED_CHARACTER:
		case ILLUSION_CHARACTER:
			change = skill_level;
			break;
#endif
//------------->
//20081013 �ؿ� ũ�������� �̺�Ʈ - ���Ź���(grooving)
#ifdef ADD_XMASEVENT_POLYMORPH_RING_20080930
		case 477:
			change = skill_level;
			break;
			
#endif //ADD_XMASEVENT_POLYMORPH_RING_20080930
//<-------------
#ifdef ADD_PREMIUMITEM_PANDA
		case 503:
			change = skill_level;
			break;
#endif // ADD_PREMIUMITEM_PANDA
		default : change = -1; break;
	}

	if( lpObj->m_Change >= 0 && lpObj->m_Change == change) return FALSE;
	if( lpObj->m_Change == 41 && change != 41 ) 
	{
		lpObj->AddLife =  0;

#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0, (WORD)lpObj->iShield);
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0, (WORD)(lpObj->iMaxShield+lpObj->iAddShield));
#else
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->Life), 0xFF, 0);
		GCReFillSend(lpObj->m_Index, (WORD)(lpObj->MaxLife+lpObj->AddLife), 0xFE, 0);
#endif
	}

#ifdef ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830	// ���� �� �߰� HP, ���� ����
	if( lpObj->m_Change == 372 && change != 372 )
	{
		lpObj->AddLife =  0;
		gObjCalCharacter( lpObj->m_Index );

#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend( lpObj->m_Index, ( WORD )( lpObj->Life ), 0xFF, 0, ( WORD )lpObj->iShield );
		GCReFillSend( lpObj->m_Index, ( WORD )( lpObj->MaxLife+lpObj->AddLife ), 0xFE, 0, ( WORD )( lpObj->iMaxShield + lpObj->iAddShield ) );
#else
		GCReFillSend( lpObj->m_Index, ( WORD )( lpObj->Life ), 0xFF, 0 );
		GCReFillSend( lpObj->m_Index, ( WORD )( lpObj->MaxLife + lpObj->AddLife ), 0xFE, 0 );
#endif
	}
#endif	// ADD_ELITE_SKELETON_WORRIOR_CHANGE_RING_20060830

#ifdef CHRISTMAS_SANTA_POLYMORPH_RING_20061117
	if( lpObj->m_Change == 374 && change != 374 )
	{
		lpObj->m_AttackDamageMaxLeft	-= 20;
		lpObj->m_AttackDamageMinLeft	-= 20;
		lpObj->m_AttackDamageMaxRight	-= 20;
		lpObj->m_AttackDamageMinRight	-= 20;
		lpObj->m_MagicDamageMin			-= 20;
		lpObj->m_MagicDamageMax			-= 20;
	}
#endif // CHRISTMAS_SANTA_POLYMORPH_RING_20061117
		 
	lpObj->m_Change = change;	
	gObjViewportListProtocolCreate(lpObj);

#ifdef MODIFY_VIEWPORT_REMOVE_CHANGE_RING_20051013
	#ifdef CASTLE_SIEGE_ONLY_PATCH_20041018
		g_CastleSiege.NotifySelfCsJoinSide (lpObj->m_Index);
	#endif
#endif

#ifdef MODIFY_KILL_COUNT_BUGFIX_20060908
	// ųī��Ʈ ������ �����ش�.(Ŭ���̾�Ʈ ����)
	PMSG_KILLCOUNT pMsg = { 0, };
	PHeadSubSetB( ( LPBYTE )&pMsg, 0xB8, 0x01, sizeof( pMsg ) );

	pMsg.btKillCount = lpObj->m_btKillCount;
	DataSend( aIndex, ( LPBYTE )&pMsg, sizeof( pMsg ) );
#endif

	return TRUE;
}

// ������ ���� ������ ����Ѵ�.
void CObjUseSkill::SkillHealing(int aIndex, int aTargetIndex, int skill_level)
{	
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;
	
	if( lpObj->Class != CLASS_ELF ) return;
	
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ������ ���� �ڽſ��Ը� ���� �� �ִ�.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if( skill_level < 0 || skill_level > 7 ) skill_level = 0;

	lpTargetObj->Life += ((skill_level+1)*5+((lpObj->Energy+lpObj->AddEnergy)/5))*g_GlobalConfig.m_fPlusHeal;
	
	if( lpTargetObj->Life > (lpTargetObj->MaxLife+lpTargetObj->AddLife) ) 
	{
		lpTargetObj->Life = (lpTargetObj->MaxLife+lpTargetObj->AddLife);
	}
	if( lpTargetObj->Type == OBJTYPE_CHARACTER ) 
	{
#ifdef ADD_SHIELD_POINT_01_20060403
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);
#else
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0);
#endif
		GCMagicAttackNumberSend(lpObj, AT_SKILL_HEALING, lpTargetObj->m_Index, skillSuccess);

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	// ġ�� ���� ��� �� Target�� ���� �ִ� ������ Agro�� ���� �����ش�.
	for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
		{
			if( lpObj->VpPlayer2[n].state )
			{
				LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

				if( lpMonster->m_iCurrentAI )
					lpMonster->m_Agro.IncAgro(lpObj->m_Index, lpObj->Energy/5/40 );
			}
		}
	}


#endif

	}
	else if( lpTargetObj->m_RecallMon >= 0 )
	{	// ��ȯ ������ ��쿡�� �ڽ��� HP�� �˷��ش�
		GCRecallMonLife(lpTargetObj->m_RecallMon, (int)lpTargetObj->MaxLife, (int)lpTargetObj->Life);		
	}
}

// ���͸� ��ȯ�Ѵ�.
BOOL CObjUseSkill::SkillMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	int result;

#ifdef NEW_SKILL_FORSKYLAND
	if( gObj[aIndex].MapNumber == 10 )
	{	// õ�������� ��ȯ�Ҽ� ����		
		return FALSE;
	}
#endif
#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ���͸� ��ȯ�� �� ����.
	if( CHECK_CHAOSCASTLE(gObj[aIndex].MapNumber) )
	{
		return FALSE;
	}
#endif

	if( gObj[aIndex].m_RecallMon >= 0 ) 
	{
		GCRecallMonLife(aIndex, 60, 0);
		gObjMonsterCallKill(aIndex);
		return FALSE;
		//return;
	}
	result = gObjAddCallMon();
		
	if( result >= 0 )
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		
		gObjSetMonster(result, MonsterType);
		gObj[result].m_RecallMon = aIndex;

		gObj[result].m_Attribute	= 100;
		//gObj[result].m_MoveSpeed	= 100;
		gObj[result].TargetNumber	= -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount		= 0;				
		gObj[aIndex].m_RecallMon	= result;
		
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon, (int)gObj[result].MaxLife, (int)gObj[result].Life);
		return TRUE;
	}
	return FALSE;
}

#ifdef ADD_SKILL_WITH_COMBO
void CObjUseSkill::SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	int	HitCount = 0;
	BOOL bAttack = FALSE;

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// �г����ϰ��� ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
//					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 4 )		// �г��� �ϰ� ���� 2 -> 3ĭ���� ���� (2004.10.26)
					{
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
						{
							continue;
						}

						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  500, lpMagic->m_Skill, isCombo);							

							if(isCombo)
								GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aIndex, 1);
						}
						//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
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

// ȸ����
void CObjUseSkill::SkillWheel(int aIndex, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo)
{
	//return;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;


#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ȸ��������� ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					{	
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
						{
							continue;
						}
						
						if( bAttack )
						{
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo);

							if(isCombo)
								GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, gObj[tObjNum].m_Index, 1);
						}
						//LogAdd("ȸ���� ����");
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
#endif

#ifdef NEW_ATTACK_PROC
void CObjUseSkill::SkillPenetration(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int	TargetPathX[20];
	int	TargetPathY[20];
	int PathCount;

	int TargePosx = lpObj->X-(8-(TargetPos&0x0F));
	int TargePosy = lpObj->Y-(8-((TargetPos&0xF0)>>4));

	int tObjNum;
	int count = 0;

	// �������� ���� ��θ� ��� �´�
	PathCount = GetTargetLinePath(lpObj->X, lpObj->Y, TargePosx, TargePosy, TargetPathX, TargetPathY, 6);

	//if( PathCount > 0 )
	//	gObjSetPosition(aIndex, TargetPathX[PathCount-1], TargetPathY[PathCount-1]);	

	//return;
	
	//BYTE	TargetPos = 0;
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 6 )
					{
						BOOL FoundEnemy = FALSE;
						for( int i = 0; i < PathCount; i++ )
						{	
							if( TargetPathX[i] == gObj[tObjNum].X && 
								TargetPathY[i] == gObj[tObjNum].Y )
							/*
							if( abs(TargetPathX[i]-gObj[tObjNum].X) <= 1 && 
								abs(TargetPathY[i]-gObj[tObjNum].Y) <= 1 )
								*/
							{
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
								break;
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
}

void CObjUseSkill::SkillDefenseDown(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int	TargetPathX[20];
	int	TargetPathY[20];
	int PathCount;

	int TargePosx = lpObj->X+(8-TargetPos&0xF0);
	int TargePosy = lpObj->Y+(8-TargetPos&0x0F);

	int tObjNum;
	int count = 0;

	// �������� ���� ��θ� ��� �´�
	PathCount = GetTargetLinePath(lpObj->X, lpObj->Y, TargePosx, TargePosy, TargetPathX, TargetPathY, 4);
	
	//BYTE	TargetPos = 0;
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
#ifndef ADD_NEW_MAP_KALIMA_20040518		// ���� -> ����� ������ �� �־�� �ϹǷ� ������. (��� -> ����� ���� ?)
			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
#endif
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					{
						BOOL FoundEnemy = FALSE;
						for( int i = 0; i < PathCount; i++ )
						{							
							//if( TargetPathX[i] == gObj[tObjNum].X && TargetPathY[i] == gObj[tObjNum].Y )
							if( abs(TargetPathX[i]-gObj[tObjNum].X) <= 2 && 
								abs(TargetPathY[i]-gObj[tObjNum].Y) <= 2 )
							{
								MaGumSkillDefenseDown(aIndex, tObjNum, lpMagic->m_Level);
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
								break;
							}
						}						
					}
				}
#ifndef ADD_NEW_MAP_KALIMA_20040518
			}
#endif
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
}

// �ұ��
void CObjUseSkill::SkillFlame(int aIndex, CMagicInf *lpMagic)
{	
}

// �Ƿ�
void CObjUseSkill::SkillEvil(int aIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

	int HitCount = 0;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 10 )
					{
						if( rand()%3 < 2 )
						{
							int delay = rand()%2000;
							
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  delay, lpMagic->m_Skill);							
							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
							//LogAdd("�Ƿ� ���� %d", delay);
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

// �����̾�
void CObjUseSkill::SkillHellFire(int aIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

	int HitCount = 0;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 4 )
					{	
						gObjAddAttackProcMsgSendDelay(lpObj, 
													  MSG_ATTACKPROC_ENTIREMAGIC, 
													  tObjNum,
													  200, lpMagic->m_Skill);							
						//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
						//LogAdd("�����̾� ����");
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

// ����Ʈ
void CObjUseSkill::SkillBlast(int aIndex, CMagicInf *lpMagic)
{
}

void CObjUseSkill::SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

	int	HitCount = 0;
	BOOL bAttack = FALSE;

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
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
					//					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 4 )		// �г��� �ϰ� ���� 2 -> 3ĭ���� ���� (2004.10.26)
					{
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
								MSG_ATTACKPROC_ENTIREMAGIC, 
								tObjNum,
								500, lpMagic->m_Skill);		
						}
						//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
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

// ȸ����
void CObjUseSkill::SkillWheel(int aIndex, CMagicInf *lpMagic)
{
	//return;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
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
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 3 )
					{	
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( bAttack )
						{
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0);
						}
						//LogAdd("ȸ���� ����");
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

#endif


#ifdef ADD_SKILL_20031022		
// �Ŀ� ������ (���� ��ų)
#ifdef EXPAND_ATTACK_PACKET_0X1E
void CObjUseSkill::SkillPowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex)
#else
void CObjUseSkill::SkillPowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle)
#endif
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;	
	::SkillFrustrum(Targetangle, aIndex);

	int tObjNum;
	int count = 0;

	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������������ ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
					{
						bAttack = FALSE;
						if( HitCount > 10 )
						{
							bAttack = FALSE;
						}
						else if( HitCount >= 5 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( bAttack )
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);					

						HitCount++;
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
#endif

#ifdef ADD_SKILL_WITH_COMBO	
BOOL CObjUseSkill::SkillDeathPoison(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;

	int FirstHit = 0;
	int	HitCount = 0;
	BOOL bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	LPOBJECTSTRUCT lpTargetObj =  &gObj[aTargetIndex];

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������������ ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{	
						bAttack = FALSE;
						if( HitCount > 10 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 5 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							int delay = rand()%500;
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  delay, lpMagic->m_Skill);							
							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
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
	//GCMagicAttackNumberSend(lpObj, AT_SKILL_EXPPOISON, aTargetIndex, 1);
	return TRUE;
}

BOOL CObjUseSkill::SkillSuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int FirstHit = 0;
	int	HitCount = 0;
	BOOL bAttack;

	LPOBJECTSTRUCT lpTargetObj =  &gObj[aTargetIndex];

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������̽��� ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( CalDistance(x, y, gObj[tObjNum].X, gObj[tObjNum].Y) < 4 )
					{	
						bAttack = FALSE;
						if( HitCount > 10 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 5 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							int delay = rand()%500;
							//gObjAddAttackProcMsgSendDelay(lpObj, 
							//							  MSG_ATTACKPROC_ENTIREMAGIC, 
							//							  tObjNum,
							//							  delay, lpMagic->m_Skill);
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);						
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

	//	GCMagicAttackNumberSend(lpObj, AT_SKILL_EXPICE, aTargetIndex, 1);
	return TRUE;
}

BOOL CObjUseSkill::SkillHellFire2(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{	
	
	if( aTargetIndex == AT_SKILL_EXPHELL_START )
		return SkillHellFire2Start(aIndex, lpMagic);		// ����

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	if( !lpObj->SkillHellFire2State)
	{
		LogAddTD("[%s][%s] �����̾�2 ��ư ���������� �ʾҴµ� �Ϸᰡ �Ǿ���", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	lpObj->SkillHellFire2State = FALSE;
	int tObjNum;
	int count = 0;
	
#ifdef MODIFY_HELLBURST_SKILL_TIME_20060522
	lpObj->SkillHellFire2Time = 0;
#endif

	LPOBJECTSTRUCT lpTargetObj =  &gObj[aTargetIndex];

/*
	int range = 2;
	if( lpObj->SkillHellFire2Count > 0 )
	{
		range += (lpObj->SkillHellFire2Count)/2;
	}*/
#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{	
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef DUEL_SYSTEM_20031028
				else if( 
#ifdef ADD_PK_LIMIT_FREE_OPTION_20050324
					aTargetIndex == tObjNum ||
#endif
					DuelIndex == tObjNum )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#endif
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// �����Ʈ�� ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) <= 6 )
					{	
						gObjAddAttackProcMsgSendDelay(lpObj, 
													  MSG_ATTACKPROC_ENTIREMAGIC, 
													  tObjNum,
													  600, lpMagic->m_Skill);							
						//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);						
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
	
	GCMagicAttackNumberSend(lpObj, AT_SKILL_EXPHELL, lpObj->m_Index, 1);
	return TRUE;
}

BOOL CObjUseSkill::SkillHellFire2Start(int aIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	

	if( lpObj->SkillHellFire2State )
	{	// �̹� ���°� �ɷ� ������ ������ ���� �ʴ´�
		LogAddTD("[%s][%s] �����̾�2 ��ư �������µ� �� �������ٴ°� �Դ�", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

#ifdef MODIFY_HELLBURST_SKILL_TIME_20060522
	lpObj->SkillHellFire2Time = GetTickCount();
#endif
	lpObj->SkillHellFire2State = TRUE;
	lpObj->SkillHellFire2Count = 0;

	GCMagicAttackNumberSend(lpObj, AT_SKILL_EXPHELL_START, aIndex, 1);	
	return TRUE;
}

BOOL CObjUseSkill::SkillKnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo)
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;
	int tObjNum;

	float fangle = (float)GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);
	
	SkillFrustrum(aIndex, fangle, 1.5f, 3.f);

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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

					if((g_GlobalConfig.m_bDisableCombo && gObj[tObjNum].Type == OBJTYPE_CHARACTER && isCombo))
					{
						continue;
					}
					
					if( attackcheck )
					{
						if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{	
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0, isCombo);

							if(isCombo)
								GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, gObj[tObjNum].m_Index, 1);						
						}
					}
				}
			}
		}
		count++;
		if( count > MAXVIEWPORTOBJECT-1 )
		{
			loopcount++;
			count = 0;
			if( loopcount >= 3 )			
				break;
		}
	}
	return TRUE;
}
#endif

void CObjUseSkill::SkillFrustrum(int aIndex, float fangle, float tx,float ty)	// ĳ����..
{	
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];
	Vector(-tx ,ty ,0.f,p[0]);
	Vector( tx ,ty ,0.f,p[1]);
	Vector( 1 ,0.f ,0.f,p[2]);
	Vector(-1 ,0.f ,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,fangle,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t vFrustrum[4];
	
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);
		lpObj->fSkillFrustrumX[i] = (int)(vFrustrum[i][0])+lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)(vFrustrum[i][1])+lpObj->Y;
	}
}


#ifdef DARKLORD_WORK

void CObjUseSkill::SkillFrustrum2(int aIndex, float fangle, float tx,float ty)	// ĳ����..
{	
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];

	Vector(-0.f, ty ,0.f,p[0]);
	Vector(0.f, ty ,0.f,p[1]);
	Vector(tx,	1.f ,0.f,p[2]);
	Vector(-tx,	1.f ,0.f,p[3]);

	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,fangle,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t vFrustrum[4];
	
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);
		lpObj->fSkillFrustrumX[i] = (int)(vFrustrum[i][0])+lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)(vFrustrum[i][1])+lpObj->Y;
	}
}

#endif


int CObjUseSkill::GetAngle(int x, int y, int tx, int ty)
{
	double	diffX = x-tx;
	double	diffY = y-ty;
	double rad = atan2(diffY, diffX);
	int angle = (int)(rad*180/Q_PI+90);	
	if( angle < 0 )	angle = 360+angle;	
	
	return angle;
}

int CObjUseSkill::CalDistance(int x, int y, int x1, int y1)
{
	if( (x == x1) && 
		(y == y1) )
	{
		return 0;
	}
	float tx = (float)(x-x1);
	float ty = (float)(y-y1);
			
	return (int)(sqrt((tx*tx)+(ty*ty)));
}

#ifdef DARKLORD_WORK

BOOL CObjUseSkill::SkillAreaCheck(int angel, int x, int y, int tx, int ty, int dis, int arc)
{
	int minangel = angel-arc;
	int maxangel = angel+arc;


	if( CalDistance(x,y, tx, ty) > dis )
		return FALSE;

	int targetangle = GetAngle(x, y, tx, ty);

	if( minangel < 0 )
	{
		if( targetangle > maxangel )
		{
			if( targetangle > 360+minangel )
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if( maxangel < targetangle )
				return FALSE;
		}		
	}
	else
	{
		if( maxangel > 360 )
		{
			if( targetangle < minangel )
			{
				if( targetangle < maxangel-360 )
					return TRUE;
				else
					return FALSE;
			}
			else
			{
				if( maxangel < targetangle )
					return FALSE;
			}
		}
		else
		{
			if( minangel > targetangle )
				return FALSE;

			if( maxangel < targetangle )
				return FALSE;
		}
	}
	return TRUE;
}

#endif


#ifdef DARKLORD_WORK
BOOL CObjUseSkill::SkillSpear(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	//SkillFrustrum2(aIndex, (float)iangle, 1.7f, 6.f);
	//SkillFrustrum(aIndex, fangle, 1.5f, 3.f);

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	//gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);	

	if( !lpObj->SkillLongSpearChange )
	{
		gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);	
		return TRUE;
	}

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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
						if( SkillSpearHitBox.HitCheck(iangle, lpObj->X, lpObj->Y, gObj[tObjNum].X, 
												  gObj[tObjNum].Y) )
						{	
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0);
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
	return TRUE;
}

BOOL CObjUseSkill::SkillFireBurst(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;
	int tObjNum;

	//float fangle = (float)GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);
	//SkillFrustrum(aTargetIndex, fangle, 3.f, 4.f);

	int count = 0;
	int loopcount = 0;
	BOOL attackcheck;	

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);	

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	int		delaytime;

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
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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
						if( gObjCalDistance(&gObj[aTargetIndex], &gObj[tObjNum]) < 3 )
						//if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aTargetIndex) )
						{	
							delaytime = 500+(rand()*17)%300;								

							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  delaytime, lpMagic->m_Skill);
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
	return TRUE;
}

BOOL CObjUseSkill::SkillDarkHorseAttack(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{	
	//return;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;


#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	GCMagicAttackNumberSend(lpObj, AT_SKILL_DARKHORSE_ATTACK, lpObj->m_Index, 1);

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110) 
				{	// ����, ����, ������ �ƴϸ�.
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
						EnableAttack = TRUE;
					}
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
							 || DuelIndex == tObjNum
#endif
							 )
					{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
						EnableAttack = TRUE;
					}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// �����ũ�� ������ �߿� ������Ե� ����ȴ�.
					else if (
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{
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
				}

				if( EnableAttack )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{	
						bAttack = FALSE;
						if( HitCount > 15 )
						{
							bAttack = FALSE;
						}
						if( HitCount >= 10 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  500, lpMagic->m_Skill);

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
	return TRUE;
}

BOOL CObjUseSkill::RecallPartyCheck(int aIndex, int skill_level)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int number;
	int partynum=0, partycount;
	
	LPOBJECTSTRUCT lpPartyObj;
	partynum = lpObj->PartyNumber;	

	int recallcount = 0;

	if( partynum != -1 )
	{
		partycount = gParty.m_PartyS[partynum].Count;
		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

				if( lpPartyObj->m_Index != lpObj->m_Index )
				{
					if( lpPartyObj->MapNumber == lpObj->MapNumber )
					{	
						if( lpPartyObj->SkillRecallParty_Time != 0 )
							return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

void CObjUseSkill::SkillRecallParty(int aIndex, int skill_level)
{
	int skillSuccess = 1;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;


	if( !RecallPartyCheck(aIndex, skill_level) )
	{	
		MsgOutput(aIndex,lMsg.Get(1238));	// ��ų�� ������� ��Ƽ���� �ֽ��ϴ�
		return;
	}

#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
	// �����ڰ� ����, ī����ĳ��, ����������, Į������ ���� �� ��ų ������� ���Ѵ�.
	if( !gMoveCommand.CheckMainToMove( lpObj ) )
	{
		char msg[255];
		wsprintf(msg, lMsg.Get(1249));		// �� �ʿ����� ��ų�� ��� �� �� �����ϴ�.
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
		return;
	}

	if( !gMoveCommand.CheckInterfaceToMove(lpObj) ) 
	{
		char msg[255];	
		wsprintf(msg, lMsg.Get(1239));	// "���� ��� �ϰ� �ִ� â �Ǵ� ���λ����� �ݾ� �ֽʽÿ�"
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
		return;
	}
#endif

#ifdef ADD_RAKLION_20080408
	// ������ ��Ŭ���� �������� �ְ� �������� �������϶��� �Ÿս�ų�� ��� �Ұ�
	if( lpObj->MapNumber==MAP_INDEX_RAKLION_BOSS && lpObj->Connected==CSS_GAMEPLAYING )
	{
		if( g_Raklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_NOTIFY_4
			|| g_Raklion.GetRaklionState() == RAKLION_STATE_END )
		{
			// �������� ��ų ����� �Ұ��ϴٴ� �ý��� �޽��� ����
			char msg[256];
			wsprintf( msg, lMsg.Get(3417) );
			GCServerMsgStringSend( msg, lpObj->m_Index, 1 );
			
			return;
		}
	}
#endif // ADD_RAKLION_20080408

	//if( lpObj->Class != CLASS_DARKLORD ) return;

	int number;
	int partynum=0, partycount;
	
	LPOBJECTSTRUCT lpPartyObj;
	partynum = lpObj->PartyNumber;	

	GCMagicAttackNumberSend(lpObj, AT_SKILL_RECALL_PARTY, lpObj->m_Index, skillSuccess);


	int recallcount = 0;

	if( partynum != -1 )
	{
		partycount = gParty.m_PartyS[partynum].Count;
		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
#ifdef ADD_NEWPVP_PKFIELD
				// ������ ��Ƽ��ȯ�� �ȵǵ��� ����
				if (g_NewPVP.IsDuel(*lpPartyObj))
				{
					//"%s���� �������Դϴ�. �����߿��� ��ȯ�� �Ұ��� �մϴ�."
					char szMsg[255];
					wsprintf(szMsg, lMsg.Get(3433), lpPartyObj->Name);
					GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);

					continue;
				}
#endif // ADD_NEWPVP_PKFIELD

				if( lpPartyObj->m_Index != lpObj->m_Index )
				{

#ifdef MODIFY_DARKLORD_RECALL_IN_CASTLESIEGE_TIME_20051219
#ifdef CASTLE_MAIN_SCHEDULER_20041111
					// ������ �߿��� �ٸ� ������ ��Ƽ���� ��ȯ���� �ʴ´�.
					if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						if( lpPartyObj->m_btCsJoinSide != lpObj->m_btCsJoinSide )
						{
							continue;
						}
					}

#endif // CASTLE_MAIN_SCHEDULER_20041111

#endif // MODIFY_DARKLORD_RECALL_IN_CASTLESIEGE_TIME_20051219




					// 1. �̵� ���� ���� �Ǵ�
					int movelevel = gMoveCommand.GetMoveLevel(lpObj->MapNumber, lpObj->X, lpObj->Y, lpPartyObj->Class);

#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
					// 0. �ֿ� ���� ���� �Ǵ� - ���� ĳ��, ī���� ĳ��, ����������, Į���� ��
					BOOL bCheckMainToMove = gMoveCommand.CheckMainToMove( lpPartyObj );
					
					// 2. �̵� ������ ���� ���� �Ǵ�
					BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove( lpPartyObj, lpObj->MapNumber );
	
					// 3. �̵� �������̽� ���� �Ǵ�
					BOOL bCheckInterfaceToMove = gMoveCommand.CheckInterfaceToMove( lpPartyObj );
#endif


#ifdef MODIFY_DARKLORD_SKILL_RECALL_PARTY_20040831
					if( lpPartyObj->Level >= movelevel && movelevel != -1 
						&& bCheckMainToMove && bCheckEquipmentToMove && bCheckInterfaceToMove	)
#else
					if( lpPartyObj->Level >= movelevel && movelevel != -1)
#endif

					//if( lpPartyObj->MapNumber == lpObj->MapNumber )
					{	
						int		addx, addy;

						int count = 50;
						BOOL	Find = FALSE;
						while(count--)
						{
							addx = lpObj->X+(-4+rand()%9);
							addy = lpObj->Y+(-4+rand()%9);
							if( gObjCheckTeleportArea(lpObj->m_Index, addx, addy) )
							{
								Find =TRUE;
								break;
							}
						}

						if( !Find )
						{	// ��ȯ ��ġ�� �� ã��������.. ��ȯ�ڿ� ��ġ�� �Ѵ�.
							addx = lpObj->X;
							addy = lpObj->Y;
						}

						lpPartyObj->SkillRecallParty_Time = 7;
						lpPartyObj->SkillRecallParty_MapNumber = lpObj->MapNumber;
						lpPartyObj->SkillRecallParty_X = addx;
						lpPartyObj->SkillRecallParty_Y = addy;
#ifdef MODIFY_RECALLSKIL_BUGFIX_20081107
						if( lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS ) {
							// ��Ŭ���� �������� ������ �߰� �����ش�.
							g_RaklionBattleUserMng.AddUserData(lpPartyObj->m_Index);
						}
#endif //MODIFY_RECALLSKIL_BUGFIX_20081107
						


						char msg[255];
						wsprintf(msg, lMsg.Get(1251));	// ��ȯ �����Դϴ�. ������ ��� ��� �˴ϴ�
						GCServerMsgStringSend(msg, lpPartyObj->m_Index, 1);	
						recallcount++;
						//gObjRecall(lpPartyObj->m_Index, lpObj->MapNumber, addx, addy);
					}
					else
					{
						char msg[255];						
						wsprintf(msg, lMsg.Get(1252));	// �̵��Ҽ� �����ϴ�
						GCServerMsgStringSend(msg, lpPartyObj->m_Index, 1);	
					}
				}
			}
		}
	}

	if( recallcount )
	{
		char msg[255];
		wsprintf(msg, lMsg.Get(1244), recallcount);	// %d���� ��ȯ�մϴ�
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);	
	}
}

void CObjUseSkill::SkillAddCriticalDamage(int aIndex, int skill_level)
{
	int skillSuccess = 1;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];


#ifdef AUTH_GAMESERVER_CHECKKEY
	if( szAuthKey[14] != AUTH_CHECK_14 )
		DestroyGIocp();
#endif

#ifdef NEW_FORSKYLAND2
	if( lpObj->Type != OBJTYPE_CHARACTER) 
		if( lpObj->m_RecallMon == -1 ) return;

	if( lpObj->Class != CLASS_DARKLORD ) return;

	int number;
	int partynum=0;
	int totallevel=0;
	int partycount;
	int dis;
	int ApplyPartyIndex[MAX_PARTYUSER];		
	
	LPOBJECTSTRUCT lpPartyObj;

	memset(ApplyPartyIndex, -1, sizeof(int)*MAX_PARTYUSER);
	partynum = lpObj->PartyNumber;	

	if( partynum != -1 )
	{
		partycount = gParty.m_PartyS[partynum].Count;
		for( int n=0; n<MAX_PARTYUSER; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];
			if( number >= 0 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];			
				// ���� �ʿ� �����ϰų�..
				if( lpObj->MapNumber == lpPartyObj->MapNumber) 
				{
					dis = gObjCalDistance(lpObj, &gObj[number]);
					if( dis < 10 )
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;						
					}
				}
			}
		}
	}	

	int addcriticaldamagevalue = ((lpObj->Leadership+lpObj->AddLeadership)/25)+((lpObj->Energy+lpObj->AddEnergy)/30);

	int	SkillTime = 60+((lpObj->Energy+lpObj->AddEnergy)/10);

	//iaddLifepower += (int)(lpObj->MaxLife*viewpercent/100);
	if( partynum == -1 )
	{	// ��Ƽ�� �ȵǾ� �ִ� �����̴�
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjAddBuffEffect( lpObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime );
#else
		lpObj->SkillAddCriticalDamageTime = SkillTime;
		lpObj->SkillAddCriticalDamage = addcriticaldamagevalue;

		lpObj->m_ViewSkillState |= STATE_ADD_CRITICAL_DAMAGE;  //����� ����.
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

		MsgOutput(aIndex, lMsg.Get(1240), SkillTime);	// ������� %d �ʰ� ���ӵ˴ϴ�
		
		GCMagicAttackNumberSend(lpObj, AT_SKILL_ADD_CRITICALDAMAGE, lpObj->m_Index, skillSuccess);		
	}
	else
	{	// ��Ƽ ����
		for( int n = 0; n < MAX_PARTYUSER; n++ )
		{
			if( ApplyPartyIndex[n] != -1 )
			{
				lpPartyObj = (LPOBJECTSTRUCT)&gObj[ApplyPartyIndex[n]];
				
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
				gObjAddBuffEffect( lpPartyObj, BUFFTYPE_CRITICAL_DMG_INC, EFFECTTYPE_CRITICAL_DMG, addcriticaldamagevalue, 0, 0, SkillTime );
#else
				lpPartyObj->SkillAddCriticalDamageTime = SkillTime;
				lpPartyObj->SkillAddCriticalDamage = addcriticaldamagevalue;

				lpPartyObj->m_ViewSkillState |= STATE_ADD_CRITICAL_DAMAGE;  //����� ����.
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

				MsgOutput(aIndex, lMsg.Get(1240), SkillTime);	// ������� %d �ʰ� ���ӵ˴ϴ�

				GCMagicAttackNumberSend(lpObj, AT_SKILL_ADD_CRITICALDAMAGE, lpPartyObj->m_Index, skillSuccess);
			}
		}
	}
#endif
}

void CObjUseSkill::SkillElectricSpark(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int TargePosx = lpObj->X-(8-(TargetPos&0x0F));
	int TargePosy = lpObj->Y-(8-((TargetPos&0xF0)>>4));

	int tObjNum;
	int count = 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif
	
	int iangle = GetAngle(lpObj->X, lpObj->Y, TargePosx, TargePosy);

	int delaytime;
	BOOL	Hit = FALSE;
	BOOL	EnableAttack;
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
//			if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				if( tObjNum >= 0 )
				{	
					EnableAttack = FALSE;

					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
						EnableAttack = TRUE;
					}	
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
#endif
						 )

					{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
						EnableAttack = TRUE;
					}	
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// �Ϸ�Ʈ�� ������ũ�� ������ �߿� ������Ե� ����ȴ�.
					else if (
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{
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
						if( SkillElectricSparkHitBox.HitCheck(iangle, gObj[aIndex].X, gObj[aIndex].Y, gObj[tObjNum].X, gObj[tObjNum].Y) )
						{	
							delaytime = 500;

							Hit = TRUE;

							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_ENTIREMAGIC, 
														  tObjNum,
														  delaytime, lpMagic->m_Skill);

							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 1);
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

	if( Hit )	// ������ �����ϸ� ��Ƽ���� �Ǹ� ��´�
		DecPartyMemberHPandMP(lpObj);


	//DecPartyMemberHPandMP
}

int DecPartyMemberHPandMP(LPOBJECTSTRUCT lpObj)
{
	LPOBJECTSTRUCT lpPartyObj;

	int partynum = lpObj->PartyNumber;
	
	if( !CHECK_LIMIT(partynum, MAX_PARTY) )
	{
		LogAdd("error : DecPartyMemberHPandMP %s %d",__FILE__, __LINE__);
		return 0;
	}

	int partycount = gParty.m_PartyS[partynum].Count;
	int retcount = 0;
	for( int n=0; n<MAX_PARTYUSER; n++)
	{
		int number = gParty.m_PartyS[partynum].Number[n];
		if( number >= 0 )
		{
			lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];
			
			// ���� �ʿ� �����ϰų�..
			if( lpObj->MapNumber == lpPartyObj->MapNumber && 
				lpObj->m_Index != number ) 
			{
				int dis = gObjCalDistance(lpObj, &gObj[number]);
				if( dis < 10 )
				{
					lpPartyObj = (LPOBJECTSTRUCT)&gObj[number];

					int declife = lpPartyObj->Life*20/100;
					int decmana = lpPartyObj->Mana*5/100;

					lpPartyObj->Life -= declife;
					lpPartyObj->Mana -= decmana;

#ifdef ADD_SHIELD_POINT_01_20060403
					GCReFillSend(lpPartyObj->m_Index, (WORD)lpPartyObj->Life, 0xFF, 0, (WORD)lpPartyObj->iShield);
#else
					GCReFillSend(lpPartyObj->m_Index, (WORD)lpPartyObj->Life, 0xFF, 0);
#endif
					GCManaSend(lpPartyObj->m_Index, (short)lpPartyObj->Mana, 0xFF, 0, lpPartyObj->BP);
					retcount++;
				}
			}
		}
	}
	return retcount;
}
#endif


#ifdef ADD_NEW_MAP_KALIMA_20040518

#include "gObjMonster.h"


// ���Ͱ� ���͸� ��ȯ�ϴ� ����
BOOL	CObjUseSkill::SkillSummon			(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	int iSummonCount		= 1;		// �⺻ ��ȯ�� 1����
	int iSummonMonterType	= 0;		// ��ȯ ���� ����
	bool bDoSummon			= true;		// ��ȯ�� ���� ����
	bool bSummonSuccess		= false;	// ��ȯ�� �����ߴ��� ����
	BYTE cSummonX			= 0;
	BYTE cSummonY			= 0;

	LPOBJECTSTRUCT lpObj	= &gObj[aIndex];

	switch(lpObj->Class) {
	case 161 :		// ����� ȯ��1
		{
			iSummonMonterType	= 147 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	case 181 : 		// ����� ȯ��2
		{
			iSummonMonterType	= 177 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	case 189 : 		// ����� ȯ��3
		{
			iSummonMonterType	= 185 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	case 197 : 		// ����� ȯ��4
		{
			iSummonMonterType	= 193 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	case 267 : 		// ����� ȯ��5
		{
			iSummonMonterType	= 263 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	case 275 :		// ����� ȯ��6
		{
			iSummonMonterType	= 271 + rand()%2;
			bDoSummon			= (rand()%15 == 0) ? true : false;
		}
		break;
	}

	if (bDoSummon == false)
		return FALSE;

	for (int iSC = 0 ; iSC < iSummonCount ; iSC++) {
		UCHAR cSX		= lpObj->X;
		UCHAR cSY		= lpObj->Y;

		if (gObjGetRandomFreeLocation(lpObj->MapNumber, cSX, cSY, 2, 2, 30) == TRUE) {
#ifdef MAP_SERVER_ADD_MONSTER_20041216		// �ʼ��� ���� ����, NPC �߰� (������ ������ �ִ� �ʸ� �߰��� �����ϴ�.)
			int result = gObjAddMonster(lpObj->MapNumber);
#else
			int result = gObjAdd();
#endif
			if( result >= 0 )
			{
				gObj[result].m_PosNum		= -1;
				gObj[result].X				= cSX;
				gObj[result].Y				= cSY;
				gObj[result].MapNumber		= lpObj->MapNumber;
				gObj[result].TX				= gObj[result].X;
				gObj[result].TY				= gObj[result].Y;
				gObj[result].m_OldX			= gObj[result].X;
				gObj[result].m_OldY			= gObj[result].Y;
				gObj[result].Dir			= 1;
				gObj[result].StartX			= (BYTE)gObj[result].X;
				gObj[result].StartY			= (BYTE)gObj[result].Y;
				gObjSetMonster(result, iSummonMonterType);

				gObj[result].m_Attribute	= MON_ATTR_SUMMONED;
				gObj[result].MaxRegenTime	= 0;
				gObj[result].Dir			= rand()%8;	// ������ ����

				bSummonSuccess				= true;

				cSummonX					= gObj[result].StartX;
				cSummonY					= gObj[result].StartY;
			}
		}
	}
	
	GCMagicAttackNumberSend(lpObj, AT_SKILL_SUMMON, MAKEWORD(cSummonX, cSummonY), bSummonSuccess);
	return bSummonSuccess;
}


// ������� ����
BOOL	CObjUseSkill::SkillImmuneToMagic	(int aIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;
	
	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE ) == false 
		&& gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE ) == false 
	  )
	{
		gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, 10 );
		return TRUE;
	}

	return FALSE;
#else
	if (lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0) {
		lpObj->m_ImmuneToMagicCount	= 10;	// �������� ���� �ð�
		lpObj->m_ViewSkillState		|= STATE_IMMUNE_TO_MAGIC;

		GCStateInfoSend(lpObj, 1, STATE_IMMUNE_TO_MAGIC);
		GCMagicAttackNumberSend(lpObj, AT_SKILL_IMMUNE_TO_MAGIC, lpObj->m_Index, true);
		return TRUE;
	}
	else
		return FALSE;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
}


// ������� ����
BOOL	CObjUseSkill::SkillImmuneToHarm		(int aIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;
	
	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE ) == false 
		&& gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE ) == false 
	  )
	{
		gObjAddBuffEffect( lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, 10 );
		return TRUE;
	}

	return FALSE;
#else
	if (lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0) {
		lpObj->m_ImmuneToHarmCount	= 10;	// �������� ���� �ð�
		lpObj->m_ViewSkillState		|= STATE_IMMUNE_TO_HARM;

		GCStateInfoSend(lpObj, 1, STATE_IMMUNE_TO_HARM);
		GCMagicAttackNumberSend(lpObj, AT_SKILL_IMMUNE_TO_HARM, lpObj->m_Index, true);
		return TRUE;
	}
	else
		return FALSE;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
}


#endif

#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
BOOL	CObjUseSkill::RemoveSpecificSkillState( LPOBJECTSTRUCT lpObj, INT iSkillNumber )
{
	if( lpObj->Type != OBJTYPE_CHARACTER )
		return FALSE;
	
	INT iViewSkillState = 0;

	if(  iSkillNumber== AT_SKILL_ATTACK )
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjRemoveBuffEffect( lpObj, BUFFTYPE_ATTACK_POWER_INC );
#else
		lpObj->m_SkillAttackTime = 0;
		lpObj->m_SkillAttack = 0;
		iViewSkillState = STATE_ATTACK;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}
	else if( iSkillNumber== AT_SKILL_DEFENSE )
	{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		gObjRemoveBuffEffect( lpObj, BUFFTYPE_DEFENSE_POWER_INC );
#else
		lpObj->m_SkillDefenseTime = 0;
		lpObj->m_SkillDefense = 0;
		iViewSkillState = STATE_DEFENSE;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	}

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, iViewSkillState );
	GCStateInfoSend(lpObj, 0, iViewSkillState);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

#if TESTSERVER == 1
//	MsgOutput( lpObj->m_Index, "��ų������Ʈ-��������(%d)", iSkillNumber );
#endif

	return TRUE;
}

#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
BOOL	CObjUseSkill::RemoveAllCharacterInvalidMagicAndSkillState( LPOBJECTSTRUCT lpObj )
{
	if( lpObj->Type != OBJTYPE_CHARACTER )
		return FALSE;

	// ����� ���� �ø���� ���� �߰� ����
	lpObj->m_SkillAttackTime2 = 0;
	lpObj->m_SkillAttack2 = 0;

#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004					// !! NOT
	lpObj->m_SkillAttackTime = 0;
	lpObj->m_SkillAttack = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ATTACK );
	
	lpObj->m_SkillDefenseTime = 0;
	lpObj->m_SkillDefense = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_DEFENSE );
	
	lpObj->m_WizardSkillDefenseTime = 0;
	lpObj->m_WizardSkillDefense = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_REDUCE_MAGICDEFENSE );
	
	lpObj->m_SkillAddLifeTime = 0;
	lpObj->m_SkillAddLife = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ADD_LIFE );
	
	lpObj->m_SkillHardenTime = 0;
	lpObj->m_SkillHarden = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_HARDEN );
	
	lpObj->m_SkillMagumReduceDefenseTime = 0;
	lpObj->m_SkillMagumReduceDefense = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_REDUCE_DEFENSE );
	
	lpObj->SkillRecallParty_Time  = 0;
	
	
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ADD_CRITICAL_DAMAGE );
	
	lpObj->m_iSkillStunTime = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_STUNED );

	lpObj->m_iSkillBrandOfSkillTime = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BRAND_OF_SKILL );

	lpObj->m_iSkillInvisibleTime = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_CLOAKING );

	lpObj->m_iSkillManaSwellTime = 0;
	lpObj->m_iSkillManaSwell = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_ADD_MANA );
	
	
	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_PoisonType = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_POISON );
	
	lpObj->m_ColdBeattackCount = 0;
	lpObj->m_IceType = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_FREEZE );
	
	lpObj->m_ImmuneToMagicCount = 0;
	lpObj->m_ImmuneToHarmCount = 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_IMMUNE_TO_MAGIC );
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_IMMUNE_TO_HARM );

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	lpObj->m_iMuseElfInfinityArrowSkillTime	= 0;
	DEL_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_INFINITY_ARROW );
#endif

#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	return TRUE;
}

// ���̵� ����Ʈ
BOOL	CObjUseSkill::SkillKnightRush			(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);	

	return TRUE;
}
// �ҿ� ������
BOOL	CObjUseSkill::SkillWizardJavalin		(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

	
	int iDelay = 500;//rand()%2000;
	
	gObjAddAttackProcMsgSendDelay(	lpObj, 
									MSG_ATTACKPROC_ENTIREMAGIC, 
									aTargetIndex,
									iDelay, 
									lpMagic->m_Skill);
	
	gObjAddAttackProcMsgSendDelay(	lpObj, 
									MSG_ATTACKPROC_ENTIREMAGIC, 
									aTargetIndex,
									iDelay+ 200, 
									lpMagic->m_Skill);

	
//	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);

	return TRUE;
}
// ���� ����
BOOL	CObjUseSkill::SkillElfRush				(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
//	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);

	gObjAddAttackProcMsgSendDelay(lpObj, 
								  MSG_ATTACKPROC_ENTIREMAGIC, 
								  aTargetIndex,
								  800, 
								  lpMagic->m_Skill);
	
	return TRUE;
}
// ����� ���˻�
BOOL	CObjUseSkill::SkillMagumOneFlash		(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);

	gObjAddAttackProcMsgSendDelay(lpObj, 
								  MSG_ATTACKPROC_ENTIREMAGIC, 
								  aTargetIndex,
								  100, 
								  lpMagic->m_Skill);

	return TRUE;
}
// ������ ���˻�
BOOL	CObjUseSkill::SkillMagumDeathCannon		(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);

	return TRUE;
}
// ��ũ�ε� ���� ��ų
BOOL	CObjUseSkill::SkillDarklordSpaceSplit	(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

//	if( lpObj->m_iSkillBrandOfSkillTime <= 0 )
//		return FALSE;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
//	gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 0, 1, 0);

	gObjAddAttackProcMsgSendDelay(lpObj, 
								  MSG_ATTACKPROC_ENTIREMAGIC, 
								  aTargetIndex,
								  500, 
								  lpMagic->m_Skill);
	
	return TRUE;
}


// ��ũ�ε� ���� ��ų
BOOL	CObjUseSkill::SkillBrandOfSkill	(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_DARKLORD_CASTLE_SKILL, 0, 0, 0, 0, 10 );
#else
	lpObj->m_iSkillBrandOfSkillTime = 10;
	ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_BRAND_OF_SKILL );
	GCStateInfoSend(lpObj, 1, STATE_BRAND_OF_SKILL);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	GCMagicAttackNumberSend(lpObj, AT_SKILL_BRAND_OF_SKILL, lpObj->m_Index, 1);

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( CHECK_LIMIT(tObjNum, MAX_OBJECT) )
			{
				if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						gObjAddBuffEffect( &gObj[tObjNum], BUFFTYPE_DARKLORD_CASTLE_SKILL, 0, 0, 0, 0, 10 );
#else
						gObj[tObjNum].m_iSkillBrandOfSkillTime = 10;
						ADD_VIEWSKILLSTATE( gObj[tObjNum].m_ViewSkillState, STATE_BRAND_OF_SKILL );
						GCStateInfoSend(&gObj[tObjNum], 1, STATE_BRAND_OF_SKILL);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						GCMagicAttackNumberSend( lpObj, AT_SKILL_BRAND_OF_SKILL, tObjNum, true );
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
	return TRUE;
}
// ����
BOOL	CObjUseSkill::SkillStun			(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos )
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int TargePosx = lpObj->X-(8-(btTargetPos&0x0F));
	int TargePosy = lpObj->Y-(8-((btTargetPos&0xF0)>>4));

	int iangle = GetAngle(lpObj->X, lpObj->Y, TargePosx, TargePosy);


	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

	gObjSetKillCount( aIndex, KILLCOUNT_RESET );
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER || lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if (CHECK_LIMIT(tObjNum, MAX_OBJECT))
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) <= 4 )
					{

						if( SkillElectricSparkHitBox.HitCheck(iangle, lpObj->X, lpObj->Y, gObj[tObjNum].X, gObj[tObjNum].Y) )
						{
						
								// BrandOfSkill�� ȿ�¿� ���� ���� ����?
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							gObjAddBuffEffect( &gObj[tObjNum], BUFFTYPE_STUN, 0, 0, 0, 0, 10 );
#else
								gObj[tObjNum].m_iSkillStunTime = 10;
								ADD_VIEWSKILLSTATE( gObj[tObjNum].m_ViewSkillState, STATE_STUNED );
								GCStateInfoSend(&gObj[tObjNum], 1, STATE_STUNED);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
								GCMagicAttackNumberSend( lpObj, AT_SKILL_STUN, tObjNum, true );
								gObjSetPosition( tObjNum, gObj[tObjNum].X, gObj[tObjNum].Y);
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
	return TRUE;
}
// ���� ����	
BOOL	CObjUseSkill::SkillRemoveStun	(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

	gObjSetKillCount( aIndex, KILLCOUNT_RESET );

	GCMagicAttackNumberSend(lpObj, AT_SKILL_REMOVAL_STUN, lpObj->m_Index, true);

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( CHECK_LIMIT(tObjNum, MAX_OBJECT) )
			{
				if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 6 )
					{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						gObjRemoveBuffEffect( &gObj[tObjNum], BUFFTYPE_STUN );
#else
						gObj[tObjNum].m_iSkillStunTime = 0;
						DEL_VIEWSKILLSTATE( gObj[tObjNum].m_ViewSkillState, STATE_STUNED );
						GCStateInfoSend(&gObj[tObjNum], 0, STATE_STUNED);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						GCMagicAttackNumberSend( lpObj, AT_SKILL_REMOVAL_STUN, tObjNum, true );
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
	return TRUE;
}
// ���� ����
BOOL	CObjUseSkill::SkillAddMana		(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

	gObjSetKillCount( aIndex, KILLCOUNT_RESET );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	int iIncleaseManaValue = 0;
	iIncleaseManaValue = ( lpObj->MaxMana + lpObj->AddMana ) * 50 / 100;
	gObjAddBuffEffect( lpObj, BUFFTYPE_MANA_INC, EFFECTTYPE_MANA, iIncleaseManaValue, 0, 0, 60 );
#else
	lpObj->AddMana -= lpObj->m_iSkillManaSwell;

	INT iSwelledMana = (short)(lpObj->MaxMana+lpObj->AddMana) * 50 / 100;
	lpObj->AddMana += iSwelledMana;
	
	lpObj->m_iSkillManaSwellTime	= 60;
	lpObj->m_iSkillManaSwell		= iSwelledMana;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	GCManaSend(lpObj->m_Index, (short)(lpObj->MaxMana+lpObj->AddMana), 0xFE, 0, lpObj->MaxBP+lpObj->AddBP);
	
	if( lpObj->Mana > (lpObj->MaxMana+lpObj->AddMana) )
	{	// ���� ������ �ִ� �������� ũ�� ������ ������
		lpObj->Mana = (lpObj->MaxMana+lpObj->AddMana);
		GCManaSend(lpObj->m_Index, (short)(lpObj->Mana), 0xFF, 0, lpObj->BP);		
	}

	GCMagicAttackNumberSend(lpObj, AT_SKILL_ADD_MANA, lpObj->m_Index, true);

	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( CHECK_LIMIT(tObjNum, MAX_OBJECT) )
			{
				if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{					
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
						// ��ȿ �������� �ְ� ���� ��� �� ���
						if( lpObj->GuildNumber>0 && gObj[tObjNum].GuildNumber>0 )
						{
							// ���� ���Ͽ����� �˻�
							if( lpObj->GuildNumber!=gObj[tObjNum].GuildNumber )
							{
								// ���� ��常 ����
								count++;
								continue;
//								if( lpObj->lpGuild->iGuildUnion == G_RELATIONSHIP_NONE )
//									continue;
//
//								if( gObj[tObjNum].lpGuild->iGuildUnion == G_RELATIONSHIP_NONE )
//									continue;
//
//								if( lpObj->lpGuild->iGuildUnion!=gObj[tObjNum].lpGuild->iGuildUnion ) 
//									continue;
							}
								
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							iIncleaseManaValue = ( gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana ) * 50 / 100;
							gObjAddBuffEffect( &gObj[tObjNum], BUFFTYPE_MANA_INC, EFFECTTYPE_MANA, iIncleaseManaValue, 0, 0, 60 );
#else
							gObj[tObjNum].AddMana -= gObj[tObjNum].m_iSkillManaSwell;

							iSwelledMana = (short)(gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana) * 50 / 100;
							gObj[tObjNum].AddMana += iSwelledMana;
							
							gObj[tObjNum].m_iSkillManaSwellTime = 60;
							gObj[tObjNum].m_iSkillManaSwell = iSwelledMana;
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							
							GCManaSend(tObjNum, (short)(gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana), 0xFE, 0, gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP);
							
							if( gObj[tObjNum].Mana > (gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana) )
							{	// ���� ������ �ִ� �������� ũ�� ������ ������
								gObj[tObjNum].Mana = (gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana);
								GCManaSend(tObjNum, (short)(gObj[tObjNum].Mana), 0xFF, 0, gObj[tObjNum].BP);		
							}	
							
#ifndef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							ADD_VIEWSKILLSTATE( gObj[tObjNum].m_ViewSkillState, STATE_ADD_MANA );
							GCStateInfoSend(&gObj[tObjNum], 1, STATE_ADD_MANA);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
							GCMagicAttackNumberSend( lpObj, AT_SKILL_ADD_MANA, tObjNum, true );
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
	return TRUE;
}
// ����
BOOL	CObjUseSkill::SkillCloaking		(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	if (!CHECK_LIMIT(aTargetIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

#ifdef MODIFY_CLOAKING_TARGET_BUGFIX_20060608
	// ���� ��ų Ÿ���� üũ : ĳ���͸� ����� �ǵ��� 1�� ����.
	if ( lpTargetObj->Type != OBJTYPE_CHARACTER )
	{
		LogAddTD("[ANTI-HACK][Cloaking Skill] [%s][%s] Invalid Targeting  TargetType [%d] TargetClass [%d]", 
				  lpObj->AccountID, lpObj->Name, 
				  lpTargetObj->Type,
				  lpTargetObj->Class);
		return FALSE;
	}
#endif // MODIFY_CLOAKING_TARGET_BUGFIX_20060608
	
	gObjSetKillCount( aIndex, KILLCOUNT_RESET );
	
	GCMagicAttackNumberSend(lpObj, AT_SKILL_INVISIBLE, lpObj->m_Index, true);

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpTargetObj, BUFFTYPE_INVISABLE, 0, 0, 0, 0, 60 );
#else
	lpTargetObj->m_iSkillInvisibleTime = 60;
	ADD_VIEWSKILLSTATE( lpTargetObj->m_ViewSkillState, STATE_CLOAKING );
	GCStateInfoSend(lpTargetObj, 1, STATE_CLOAKING);
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	GCMagicAttackNumberSend( lpObj, AT_SKILL_INVISIBLE, aTargetIndex, true );

	return TRUE;
}

// ���� ����

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
BOOL	CObjUseSkill::RemoveCloakingEffect( int aIndex )
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjRemoveBuffEffect( &gObj[aIndex], BUFFTYPE_INVISABLE );
#else
	gObj[aIndex].m_iSkillInvisibleTime = 0;
	DEL_VIEWSKILLSTATE( gObj[aIndex].m_ViewSkillState, STATE_CLOAKING );
	GCStateInfoSend(&gObj[aIndex], 0, STATE_CLOAKING );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	
	return TRUE;
}
#endif

BOOL	CObjUseSkill::SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

#ifdef UPDATE_GM_FUNCTION_20070228	// GM �� ���� ���� �ȵǰ�
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( ( ( lpTargetObj->Authority & AUTHORITY_EVENT_GM ) == AUTHORITY_EVENT_GM ) 
		&& ( gObjCheckUsedBuffEffect( lpTargetObj, BUFFTYPE_INVISABLE ) ) )
#else
	if( ( (lpTargetObj->Authority&AUTHORITY_EVENT_GM) == AUTHORITY_EVENT_GM ) 
		&& ( (lpTargetObj->m_ViewSkillState&STATE_CLOAKING) != STATE_CLOAKING ) )
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

	gObjSetKillCount( aIndex, KILLCOUNT_RESET );

	GCMagicAttackNumberSend(lpObj, AT_SKILL_REMOVAL_INVISIBLE, lpObj->m_Index, true);

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( CHECK_LIMIT(tObjNum, MAX_OBJECT) )
			{
				if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{			
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						gObjRemoveBuffEffect( &gObj[tObjNum], BUFFTYPE_INVISABLE );
#else
						gObj[tObjNum].m_iSkillInvisibleTime = 0;
						DEL_VIEWSKILLSTATE( gObj[tObjNum].m_ViewSkillState, STATE_CLOAKING );
						GCStateInfoSend(&gObj[tObjNum], 0, STATE_CLOAKING );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
						GCMagicAttackNumberSend( lpObj, AT_SKILL_REMOVAL_INVISIBLE, tObjNum, true );
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
	return TRUE;
}
// ���� ����
BOOL	CObjUseSkill::SkillRemoveMagic	(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	// ���� ��ų �ߵ� ����.
#if TESTSERVER == 0
	if ( (lpObj->MapNumber != MAP_INDEX_CASTLESIEGE) &&  (lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE) )
		return FALSE;
#endif

	int tObjNum;
	int count = 0;

	gObjSetKillCount( aIndex, KILLCOUNT_RESET );
	
	GCMagicAttackNumberSend(lpObj, AT_SKILL_REMOVAL_MAGIC, lpObj->m_Index, true);

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( CHECK_LIMIT(tObjNum, MAX_OBJECT) )
			{
				if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 5 )
					{
						// ��� SkillTimer�� ����. -0- (�ູ, ��ȥ ���� �� ��� ���ܵǾ����.)
						RemoveAllCharacterInvalidMagicAndSkillState( &gObj[tObjNum] );
	#ifndef MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
						GCStateInfoSend(&gObj[tObjNum], 0, STATE_REMOVAL_MAGIC);
	#endif // MODIFY_SEND_BUFFEFFECT_IN_VIEWPORT_20071121
						GCMagicAttackNumberSend( lpObj, AT_SKILL_REMOVAL_MAGIC, tObjNum, true );
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
	return TRUE;
}
		
#endif



#ifdef ADD_ITEM_FENRIR_01_20051110
BOOL CObjUseSkill::SkillFenrirAttack	(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;


#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

//	GCMagicAttackNumberSend(lpObj, AT_SKILL_FENRIR_ATTACK, lpObj->m_Index, 1);
	
	if( lpObj->Level < MIN_FENRIR_EQUIP_LEVEL )
		return FALSE;

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110) 
				{	
					// ����, ����, ������ �ƴϸ�.
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	
						// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
						EnableAttack = TRUE;
					}
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
							 || DuelIndex == tObjNum
#endif
							 )
					{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
						EnableAttack = TRUE;
					}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// �ö�������� ������ �߿� ������Ե� ����ȴ�.
					else if (
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{
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
				}

				if( EnableAttack )
				{	
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 6 )
					{	
						bAttack = FALSE;
						if( HitCount >= 5 )
						{
							bAttack = FALSE;
						}
						else 
						{
							bAttack = TRUE;
						}

						if( tObjNum == aTargetIndex )
						{ 
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, 
														  MSG_ATTACKPROC_FENRIR, 
														  tObjNum,
														  300, lpMagic->m_Skill);

							HitCount++;
							
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
	return TRUE;

}

#endif
		

#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
// ����� ��ų - ���Ǵ�Ƽ �ַο�
BOOL CObjUseSkill::SkillInfinityArrow( int aIndex, int aTargetIndex, CMagicInf *lpMagic )
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	//BYTE	btSkillSuccess = 1;

	if( lpObj->Type != OBJTYPE_CHARACTER ) 
	{		
		return FALSE;
	}
	
	if( lpObj->Class != CLASS_ELF || lpObj->ChangeUP == 0 )
	{		
		return FALSE;
	}
	
	if( lpObj->m_Index != aTargetIndex )
	{		
		return FALSE;
	}	

	// �̹� �ߵ����� ���Ǵ�Ƽ �ַο� ��ų�� �ٽ� ����� �� ����.
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW ) == true )
#else
	if( lpObj->m_iMuseElfInfinityArrowSkillTime > 0 )
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	{
		return FALSE;
	}
	
#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004
	gObjAddBuffEffect( lpObj, BUFFTYPE_INFINITY_ARROW, 0, 0, 0, 0, g_SkillAdditionInfo.GetInfinityArrowSkillTime() );
#else
	lpObj->m_iMuseElfInfinityArrowSkillTime = g_SkillAdditionInfo.GetInfinityArrowSkillTime();
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	// ��ȹ �������� ���� 20060424
	//int iMaxEnergy = lpObj->Energy + lpObj->AddEnergy;
	//lpObj->m_iMuseElfInfinityArrowSkillTime = ( ( lpObj->Level * 75 / 10 ) + 150 ) + ( iMaxEnergy / 6 );

#ifdef MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004				// !! NOT
	GCMagicAttackNumberSend( lpObj, AT_SKILL_INFINITY_ARROW, lpObj->m_Index, 1 );
	LogAddTD( "[%s][%s] Use Infinity Arrow Skill (Time:%d)(Character Level : %d)(ChangeUp: %d)"
				, lpObj->AccountID, lpObj->Name, g_SkillAdditionInfo.GetInfinityArrowSkillTime(), lpObj->Level, lpObj->ChangeUP );
#else
	ADD_VIEWSKILLSTATE( lpObj->m_ViewSkillState, STATE_INFINITY_ARROW );
	GCStateInfoSend( lpObj, 1, STATE_INFINITY_ARROW );
	GCMagicAttackNumberSend( lpObj, AT_SKILL_INFINITY_ARROW, lpObj->m_Index, 1 );
	LogAddTD( "[%s][%s] Use Infinity Arrow Skill (Time:%d)(Character Level : %d)(ChangeUp: %d)"
				, lpObj->AccountID, lpObj->Name, lpObj->m_iMuseElfInfinityArrowSkillTime, lpObj->Level, lpObj->ChangeUP );
#endif // MODIFY_OLD_BUFF_EFFECT_SYSTEM_20071004

	return TRUE;
}

#endif	// #ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ó��
BOOL CObjUseSkill::SkillFireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic )
{
	if( g_SkillAdditionInfo.GetFireScreamSkill() == FALSE )
	{
		// ���̾� ��ũ���� ó������ �ʴ´�.
		return FALSE;
	}

	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	if( !gObjIsConnected( lpObj->m_Index ) && !gObjIsConnected( lpTargetObj->m_Index ) )
		return FALSE;
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) 
		return FALSE;

	if( lpObj->Class != CLASS_DARKLORD )
		return FALSE;

	// ����
	gObjAttack( lpObj, lpTargetObj, lpMagic, 0, 0 );
	return TRUE;
}

// lpObj : ���̾� ��ũ���� ����� ����
// lpTargetObj : ���̾� ��ũ���� ���� ���� �Ǵ� ����
// �����̾� ��ũ���� ���� ���� �Ǵ� ���Ͱ� Ȯ���� ���� ���� 1X1 ���� ĳ���Ͱ� ���� �������� ��´�.
BOOL CObjUseSkill::FireScreamExplosionAttack( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage )		// ���̾� ��ũ���� ����� �� ���������� �߸� ������ ���͵� �������� �޴´�.
{	
	if( !gObjIsConnected( lpObj->m_Index ) && !gObjIsConnected( lpTargetObj->m_Index ) )
		return FALSE;
	
	// Ȯ�� ���
	if( rand() % 10000 > g_SkillAdditionInfo.GetFireScreamExplosionRate() )
		return FALSE;
	
	// ���� �������� ���� ����Ѵ�.
	int iExplosionDamage = iAttackDamage / 10;
	if( iExplosionDamage < 1 )
	{
		return TRUE;	// ���� �� �������� ���� ������ �����Ѵ�.
	}

	int		iTarObjNum;
	int		iCount = 0;
	BOOL	bEnableAttack;

	while( 1 )
	{		
		if( lpObj->VpPlayer2[iCount].state )
		{	
			iTarObjNum = lpObj->VpPlayer2[iCount].number;

			if( iTarObjNum >= 0 )
			{
				bEnableAttack = FALSE;

				if( ( ( gObj[iTarObjNum].Type == OBJTYPE_MONSTER ) && ( gObj[iTarObjNum].m_RecallMon < 0 ) ) )
				{	// ����� �����̰�, ���� ��ȯ�� ��ȯ���� �ƴϸ� ���ݰ���
					bEnableAttack = TRUE;
				}
				else if( lpTargetObj->Type == OBJTYPE_CHARACTER && lpObj->Type == OBJTYPE_CHARACTER && gObj[iTarObjNum].Type != OBJTYPE_NPC )
				{
	#ifdef MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
					// ���� �����ϰ�� Ÿ�� �ֺ��� ����� ���ݵ��� �ʴ´�
					if( lpTargetObj->m_Index == iTarObjNum )
						bEnableAttack = TRUE;

					// ���� ����� ��쿡 ���� ����
					if( gObjGetRelationShip( lpObj, &gObj[iTarObjNum] ) == G_RELATIONSHIP_RIVAL )
						bEnableAttack = TRUE;				// ��������
	#else // MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
					// ��������
					bEnableAttack = TRUE;
	#endif // MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
				}
				else if( lpTargetObj->m_cChaosCastleIndex == gObj[iTarObjNum].m_cChaosCastleIndex 
						&& lpTargetObj->m_cChaosCastleIndex != -1 )
				{
					// ī���� ĳ�������� ����, ������ ������ �ʴ´�.
					bEnableAttack = TRUE;
				}
#ifdef CASTLE_MAIN_SCHEDULER_20041111
				else if( gObj[iTarObjNum].Type != OBJTYPE_NPC &&
						gObj[iTarObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
				{
	#ifdef MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
					// �����ʿ����� �������� ������Ը� �������� ����ȴ�.
					// m_btCsJoinSide - 0:���þ���, 1:������, 2:������
					if( lpObj->m_btCsJoinSide != gObj[iTarObjNum].m_btCsJoinSide 
						&& gObj[iTarObjNum].m_btCsJoinSide > 0 )
					{
						bEnableAttack = TRUE;
					}
	#else // MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
					// ������
					bEnableAttack = TRUE;
	#endif // MODIFY_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081121
				}
#endif	// #ifdef CASTLE_MAIN_SCHEDULER_20041111

				if( bEnableAttack )
				{
					// ���� ����� ���ݿ� ���ؼ��� ���ߵ������� ���� �ʴ´�.
					if( lpObj->m_Index != gObj[iTarObjNum].m_Index )
					{
						// �Ÿ� ���
						int iSkillDistance = gObjCalDistance( lpTargetObj, &gObj[iTarObjNum] );
						if( iSkillDistance <= g_SkillAdditionInfo.GetFireScreamExplosionAttackDistance() )
						{	
							gObjAddMsgSendDelay( lpObj, MSG_ATTACKPROC_FIRESCREAM, iTarObjNum, 100, iExplosionDamage );									
						}
					}
				}
			}
		}
		iCount++;
		if( iCount > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}
	
	return TRUE;
}
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117
// �Ƿ�
BOOL CObjUseSkill::SkillNewEvil( int aIndex, int aTargetIndex, CMagicInf *lpMagic )
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];

	int tObjNum;
	int count = 0;
	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while( 1 )
	{
		if( lpObj->VpPlayer2[count].state )
		{			
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// 1. ���� ����� ����
				if( lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER )
				{	
					EnableAttack = TRUE;

					// ��ȯ���� ó��
					if( ( gObj[tObjNum].m_RecallMon >= 0 ) && ( tObjNum != aTargetIndex ) )
					{
						EnableAttack = FALSE;
					}
				}
				// 2. ���� ����� ����
				else if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
				{
					// Ŭ���̾�Ʈ���� ����� ���� ��������(���� ���̶� ���� ���� �ؾ���)
					if( tObjNum == aTargetIndex )
					{	
						EnableAttack = TRUE;
					}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������ �߿� ������Ե� ����ȴ�.
					else if( lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
							gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
							g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
					{
						EnableAttack = TRUE;
					}
#endif					
				}
	
				if( EnableAttack )
				{	
					if( gObjCalDistance( lpObj, &gObj[tObjNum] ) < 6 )
					{	
						bAttack = FALSE;
						if( HitCount >= 5 )
						{
							bAttack = FALSE;
						}
						else 
						{
							bAttack = TRUE;
						}

						if( tObjNum == aTargetIndex )
						{ 
							bAttack = TRUE;
						}
						
						if( bAttack )
						{
							if( rand()%3 < 2 )
							{
								int delay = rand() % 2000;
							
								gObjAddAttackProcMsgSendDelay( lpObj, 
															  MSG_ATTACKPROC_ENTIREMAGIC, 
															  tObjNum,
															  300, lpMagic->m_Skill );	
															  //delay, lpMagic->m_Skill );	
								HitCount++;
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
	return TRUE;
}
#endif	// MODIFY_SKILL_EVEIL_AT_SERVER_20070117

//////////////////////////////////////////////////////////////////////////
//	Ư�� ��ų�� ���� �߰� ó��
//////////////////////////////////////////////////////////////////////////
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
void CObjUseSkill::SpecificSkillAdditionTreat( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf* lpMagic, int iAttackDamage )
{	
#ifdef MODIFY_STRENGTHEN_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081017
	int bySkillNum = lpMagic->m_Skill;
		
	if( bySkillNum == AT_SKILL_FIRESCREAM ||					// ���̾� ��ũ�� ��ų�� ���
		(bySkillNum>=AT_SKILL_ML_STRENGTHEN_FIRESCREAM && bySkillNum<AT_SKILL_ML_STRENGTHEN_FIRESCREAM+5) )// ���̾� ��ũ�� ��ȭ�� ���
	{
		gObjUseSkill.FireScreamExplosionAttack( lpObj, lpTargetObj, iAttackDamage );		
	}	
#else	// MODIFY_STRENGTHEN_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081017
	BYTE bySkillNum = lpMagic->m_Skill;
	if( bySkillNum == AT_SKILL_FIRESCREAM )			// ���̾� ��ũ�� ��ų�� ���
	{
		gObjUseSkill.FireScreamExplosionAttack( lpObj, lpTargetObj, iAttackDamage );		
	}
#endif // MODIFY_STRENGTHEN_FIRESCREAM_SPLASH_DAMAGE_BUGFIX_20081017			
}
#endif	// #ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	// ��ų��� ó��

BOOL CObjUseSkill::SkillLightningOrb(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	int count = 0;
	//int loopcount = 0;
	//BOOL attackcheck;	

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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

				if( EnableAttack)
				{
					if( tObjNum == aTargetIndex )
					{	
						//if( SkillLightingOrbHitBox.HitCheck(iangle, lpObj->X, lpObj->Y, gObj[tObjNum].X, gObj[tObjNum].Y) )
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
						{	
							//gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 0, 1, 0);
							gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 700, lpMagic->m_Skill);
							bRet = TRUE;

							break;
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

	return bRet;
}

// ���ݴ��� �ֺ��� �������� ����
BOOL CObjUseSkill::SplashDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int nSkill, int nAttackDamage, int nDistance, int nDamageRate)
{
	if( !gObjIsConnected( lpObj->m_Index ) && !gObjIsConnected( lpTargetObj->m_Index ) )
		return FALSE;

	if(nAttackDamage <= 0)
		return FALSE;

	int nSplashDamage = nAttackDamage * nDamageRate / 100;

	int		iTarObjNum;
	int		iCount = 0;
	BOOL	bEnableAttack;

	while( 1 )
	{		
		if( lpObj->VpPlayer2[iCount].state )
		{	
			iTarObjNum = lpObj->VpPlayer2[iCount].number;

			if( iTarObjNum >= 0 )
			{
				bEnableAttack = FALSE;

				if( ( ( gObj[iTarObjNum].Type == OBJTYPE_MONSTER ) && ( gObj[iTarObjNum].m_RecallMon < 0 ) ) )
				{	// ����� �����̰�, ���� ��ȯ�� ��ȯ���� �ƴϸ� ���ݰ���
					bEnableAttack = TRUE;
				}
				else if( lpTargetObj->m_cChaosCastleIndex != -1
					&&lpTargetObj->m_cChaosCastleIndex == gObj[iTarObjNum].m_cChaosCastleIndex )
				{
					// ī���� ĳ�������� ����, ������ ������ �ʴ´�.
					bEnableAttack = TRUE;
				}
				else if( lpTargetObj->Type == OBJTYPE_CHARACTER && lpObj->Type == OBJTYPE_CHARACTER && gObj[iTarObjNum].Type != OBJTYPE_NPC )
				{
					// ��������
					bEnableAttack = TRUE;
				}
				if( bEnableAttack )
				{
					// ���� ����� ���ݿ� ���ؼ��� ���ߵ������� ���� �ʴ´�.
					if( lpObj->m_Index != gObj[iTarObjNum].m_Index )
					{
						// �Ÿ� ���
						int iSkillDistance = gObjCalDistance( lpTargetObj, &gObj[iTarObjNum] );
						if( iSkillDistance <= nDistance )
						{	
							// *...�޽��� ������ ����...*
							gObjAddMsgSendDelay( lpObj, MSG_ATTACK_LIGHTNINGORB, iTarObjNum, 300, nSplashDamage );									
						}
					}
				}
			}
		}
		iCount++;
		if( iCount > MAXVIEWPORTOBJECT-1 )
		{
			break;
		}
	}

	return TRUE;
}

BOOL CObjUseSkill::SkillDrainLife(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{
	if(!CHECK_LIMIT(aTargetIndex, MAX_OBJECT))
		return FALSE;
	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	int count = 0;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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

				if( EnableAttack)
				{
					if( tObjNum == aTargetIndex )
					{	
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
						{	
							gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 700, lpMagic->m_Skill);

							bRet = TRUE;

							break;
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

	return bRet;
}

BOOL CObjUseSkill::SkillChainLightning(int aIndex, int aTargetIndex,CMagicInf *lpMagic)
{
#ifdef MODIFY_CHAINLIGHTNING_BUGFIX_20080902
	if(!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;
	
	if(!CHECK_LIMIT(aTargetIndex, MAX_OBJECT))
		return FALSE;
#endif	// MODIFY_CHAINLIGHTNING_BUGFIX_20080902
	
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj = &gObj[aTargetIndex];
		
	BOOL bRet = TRUE;
	int tObjNum;

	int count = 0;

	int nChainTarget[3] = {-1,-1,-1};

 	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
 
#ifdef DUEL_SYSTEM_20031028
 	int DuelIndex = lpObj->m_iDuelUser;
#endif
 
 	BOOL	EnableAttack;
 
	if(!gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
	{
		return FALSE;
	}

	// ù��° Ÿ��
	nChainTarget[0] = aTargetIndex;

	// ù��° Ÿ�ٰ� ������ ���͸� ã�´�.
 	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
#ifdef MODIFY_CHAIN_LIGHTNING_USE_TRAP_BUGFIX_20080820
				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER)		// ����� �����̰�
					&& (gObj[tObjNum].m_RecallMon < 0))						// ��ȯ���� �ƴϰ�
					&& (gObj[tObjNum].Class < 100 || gObj[tObjNum].Class > 110)	// Ʈ���� �ƴϸ�
					)
#else
				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
#endif // MODIFY_CHAIN_LIGHTNING_USE_TRAP_BUGFIX_20080820
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
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

				if( EnableAttack)
				{
					if( gObj[tObjNum].X >= lpTargetObj->X-1 && gObj[tObjNum].X  <= lpTargetObj->X+1
 							&& gObj[tObjNum].Y >= lpTargetObj->Y-1 && gObj[tObjNum].Y <= lpTargetObj->Y+1
							&& gObj[tObjNum].Type == OBJTYPE_MONSTER)
					{
						nChainTarget[1] = tObjNum;

						if( nChainTarget[2] != -1)
						{
							break;
						}
					}					
					else if( gObj[tObjNum].X >= lpTargetObj->X-2 && gObj[tObjNum].X  <= lpTargetObj->X+2
 							&& gObj[tObjNum].Y >= lpTargetObj->Y-2 && gObj[tObjNum].Y <= lpTargetObj->Y+2 
							&& gObj[tObjNum].Type == OBJTYPE_MONSTER)
					{
						nChainTarget[2] = tObjNum;
						
						if( nChainTarget[1] != -1)
						{
							break;
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

	if( nChainTarget[2] == -1)
	{
		nChainTarget[2] = nChainTarget[1];
	}

	if( nChainTarget[1] == -1)
	{
		nChainTarget[2] = nChainTarget[1] = nChainTarget[0];
	}

	// ������� ����� ����
	int nOffSet = 0;
	BYTE SendByte[256];
	PMSG_CHAIN_MAGIC	pMsg;
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD	
	pMsg.MagicNumberH	= HIBYTE( lpMagic->m_Skill );
	pMsg.MagicNumberL	= LOBYTE( lpMagic->m_Skill );
#else
	pMsg.MagicNumber	= lpMagic->m_Skill;
#endif
	pMsg.wUserIndex		= aIndex;
	pMsg.nCount			= 3;

	nOffSet += sizeof(pMsg);

	for(int n = 0; n < 3; n++)
	{
		PMSG_CHAIN_MAGIC_OBJECT pMagicObj;
		pMagicObj.nTargetIndex = (WORD) nChainTarget[n];
		
		memcpy(SendByte + nOffSet, (char*)&pMagicObj, sizeof(pMagicObj));
		nOffSet += sizeof(pMagicObj);
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0a, nOffSet);
	memcpy(SendByte, (char*)&pMsg, sizeof(pMsg));

	if( lpObj->Type == OBJTYPE_CHARACTER )
		DataSend(lpObj->m_Index, (LPBYTE)&SendByte, pMsg.h.size);

	MsgSendV2(lpObj, (LPBYTE)&SendByte, pMsg.h.size);

	for( int n = 0; n < 3; n++)
	{
		// subcode2�� Ÿ�� ������ ����
		gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, nChainTarget[n], 200 + (n*200), lpMagic->m_Skill, n+1);
	}

	return bRet;
}

BOOL CObjUseSkill::SkillSummons(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int count = 0;
	int tObjNum;
#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif
	int iangle = GetAngle(lpObj->X, lpObj->Y, x, y);
	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	
	// ��ų ��� ���ɹ���üũ
	int iSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
	if( CalDistance(lpObj->X, lpObj->Y, x, y) <= iSkillDistance )
	{
		BOOL	EnableAttack;
		while(1)
		{
			if( lpObj->VpPlayer2[count].state )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				
				if( tObjNum >= 0 )
				{
					EnableAttack = FALSE;
					
					// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
					
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
						EnableAttack = TRUE;
					}	
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
						|| DuelIndex == tObjNum
#endif
						)
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
					
					if( EnableAttack)
					{
						// x, y���� 2�� �ݰ�
						if( gObj[tObjNum].X >= x-2 && gObj[tObjNum].X  <= x+2
 							&& gObj[tObjNum].Y >= y-2 && gObj[tObjNum].Y <= y+2 )
						{
							gObj[tObjNum].lpAttackObj = lpObj;
							gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 1000, lpMagic->m_Skill);
							bRet = TRUE;
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

	return bRet;
}

// ����(�������ݻ�)
BOOL CObjUseSkill::SkillThorns(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];
	
	if( lpTargetObj->Type != OBJTYPE_CHARACTER 
//		|| lpTargetObj->m_RecallMon == -1 
		|| lpObj->Class != CLASS_SUMMONER) 
	{
		return FALSE;	
	}
		
	BOOL bRet = FALSE;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	int count = 0;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	// �ڱ� �ڽſ��� �ɾ�����
	if( aIndex == aTargetIndex )
	{
		// ������ �ݻ� ���� ����
		int nEffectValue	= (30 + lpObj->Energy / 42);
		int nEffectTime		= 30 + (lpObj->Energy  / 25);

		// ���� ������ ���۴� �������
		if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_DAMAGE_REFLECT, nEffectValue, 0 ) == true )
		{
			GCMagicAttackNumberSend( lpObj, AT_SKILL_THORNS, lpTargetObj->m_Index, 0 );	// ��ų��� ����
			return FALSE;
		}

		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DAMAGE_REFLECT, EFFECTTYPE_DAMAGEREFLECT, nEffectValue, 0, 0, nEffectTime);
		bRet = TRUE;
	}
	else
	{
		

		while(1)
		{	
			// ī����ĳ�������� �ڽſ��Ը� 
			if(CHECK_CHAOSCASTLE(lpObj->MapNumber))
			{
				break;
			}

			if( lpObj->VpPlayer2[count].state )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				
				if( tObjNum >= 0 )
				{
					EnableAttack = FALSE;
					
					// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
					
					if( lpObj->VpPlayer2[count].type == OBJTYPE_CHARACTER )
					{	// ����� ����
						EnableAttack = TRUE;
					}	
					else if( tObjNum == aTargetIndex 
#ifdef DUEL_SYSTEM_20031028
						|| DuelIndex != tObjNum
#endif
						)
					{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
						EnableAttack = TRUE;
					}	
					
					if( EnableAttack)
					{
						if( tObjNum == aTargetIndex )
						{	
							if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
							{
								lpTargetObj->lpAttackObj = lpObj;
								// ������ �ݻ� ���� ����
								int nEffectValue	= (30 + lpObj->Energy / 42);
								int nEffectTime		= 30 + (lpObj->Energy / 25);

								// ���� ������ ���۴� �������
								if( gObjCheckPowerfulEffect( lpTargetObj, BUFFTYPE_DAMAGE_REFLECT, nEffectValue, 0 ) == true )
								{
									GCMagicAttackNumberSend( lpObj, AT_SKILL_THORNS, lpTargetObj->m_Index, 0 );	// ��ų��� ����
									return FALSE;
								}
								
								gObjAddBuffEffect(lpTargetObj, BUFFTYPE_DAMAGE_REFLECT, EFFECTTYPE_DAMAGEREFLECT, nEffectValue, 0, 0, nEffectTime);
								bRet = TRUE;
								
								break;
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

	return bRet;
}

// ����
BOOL CObjUseSkill::SkillSleep(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	int count = 0;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 && DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if(CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					EnableAttack = TRUE;
				}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
				else if (// ������ �߿� ������Ե� ����ȴ�.
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{

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

				if( EnableAttack)
				{
					if( tObjNum == aTargetIndex )
					{	
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
						{	
							gObjAttack(lpObj, &gObj[aTargetIndex], lpMagic, 1, 0);
							bRet = TRUE;

							break;
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

	return bRet;
}

// ����ε�
BOOL CObjUseSkill::SkillBlind(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int iangle = GetAngle(lpObj->X, lpObj->Y, gObj[aTargetIndex].X, gObj[aTargetIndex].Y);

	int count = 0;

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	// ������
	int skillSuccessRate = 0;
	if(gObj[aTargetIndex].Type == OBJTYPE_MONSTER)
	{
		skillSuccessRate = lpObj->m_AttackRating * ( 20  + lpObj->Energy / 50) / 100;
	}
	else
	{
		int iAttackRate = ( lpObj->Level * 3 ) + ( lpObj->Dexterity * 35 / 10 );	// ��ȯ���� ���� ���� ������
		skillSuccessRate = iAttackRate * ( 20  + lpObj->Energy / 50) / 100;
	}
	
	if( skillSuccessRate < rand()%100)
	{
		GCDamageSend(lpObj->m_Index,gObj[aTargetIndex].m_Index , 0, 0, 0, 0);
		return FALSE;
	}

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 && DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if(CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					EnableAttack = TRUE;
				}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
				else if (// ������ �߿� ������Ե� ����ȴ�.
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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

				if( EnableAttack)
				{
					if( tObjNum == aTargetIndex )
					{	
						if(gCheckSkillDistance(aIndex, aTargetIndex, lpMagic->m_Skill))
						{	
							// ���� ������ ������
							int nEffectValue = 20 + (lpObj->Energy / 50);
							int nEffectTime = 0;
							if( gObj[aTargetIndex].Type == OBJTYPE_MONSTER)
							{
								nEffectTime	= 9 + (lpObj->Energy / 70) - (gObj[aTargetIndex].Level / 15);
							}
							else if (gObj[aTargetIndex].Type == OBJTYPE_CHARACTER)
							{
								nEffectTime = ( 9 / 3 ) + (lpObj->Energy / 210) + ((gObj[aTargetIndex].Level - gObj[aTargetIndex].Level) / 100);
							}

							// ���� ������ ���۴� �������
							if( gObjCheckPowerfulEffect( &gObj[aTargetIndex], BUFFTYPE_BLIND, nEffectValue, 0 ) == true )
							{
								GCMagicAttackNumberSend( lpObj, AT_SKILL_BLIND, aTargetIndex, 0 );	// ��ų��� ����
								return FALSE;
							}
							
							gObjAddBuffEffect(&gObj[aTargetIndex], BUFFTYPE_BLIND, EFFECTTYPE_BLIND, nEffectValue, 0, 0, nEffectTime);
							bRet = TRUE;

							break;
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

	return bRet;
}

#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912

#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
// ��ũ�Ͻ�
BOOL CObjUseSkill::SkillWeakness(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int count		= 0;
	int	HitCount	= 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( //tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if(CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					EnableAttack = TRUE;
				}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
				else if (// ������ �߿� ������Ե� ����ȴ�.
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{

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

				if( EnableAttack)
				{
					if(gObjCalDistance(lpObj, &gObj[tObjNum]) < 5)
					{	
						gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 700, lpMagic->m_Skill);
						
						HitCount++;
						
						if( HitCount >= 5 )
						{
							break;
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

	return bRet;
}

// �̳ʺ��̼�
BOOL CObjUseSkill::SkillEnervation(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int tObjNum;

	int count		= 0;
	int	HitCount	= 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);

	BOOL	EnableAttack;

	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if( //tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}	
				else if(CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
					EnableAttack = TRUE;
				}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
				else if (// ������ �߿� ������Ե� ����ȴ�.
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{

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

				if( EnableAttack)
				{
					if(gObjCalDistance(lpObj, &gObj[tObjNum]) < 5)
					{	
						gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 700, lpMagic->m_Skill);
						
						HitCount++;
						
						if( HitCount >= 5 )
						{
							break;
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

	return bRet;
}
#endif	// UPDATE_SUMMONER_NEW_SKILL_20080111

#ifdef SEASON4_PATCH_GROOVING
// --->
// ���� 4.0 �߰� ��ų  grooving 2008/04/24
// aIndex		[IN] : ������ �ε���
// aTargetIndex	[IN] : �ǰ��� �ε���
// lpMagic		[IN] : ���� ����
BOOL	CObjUseSkill::SkillLightningShock( int aIndex, int aTargetIndex, CMagicInf *lpMagic )// ����Ʈ�� ��ũ 
{

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];
	
	BOOL bRet = FALSE;
	BOOL bAttack = FALSE;

	int tObjNum;
	
	int count		= 0;
	int	HitCount	= 0;

	const int iDistance = 7;

	int nHitCount = 0;
	
	GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	
	BOOL	EnableAttack;

	BOOL bPVP = TRUE;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;
			
			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
				
				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if(CHECK_CHAOSCASTLE(lpObj->MapNumber) || CHECK_ILLUSION_TEMPLE(lpObj->MapNumber))
				{
#ifdef MODIFY_LIGHTNINGSHOCK_ILLUSIONTEMPLE_BUGFIX_20090414
					if( aTargetIndex == tObjNum )
#endif // MODIFY_LIGHTNINGSHOCK_ILLUSIONTEMPLE_BUGFIX_20090414
						EnableAttack = TRUE;
				}
				else if( (tObjNum == aTargetIndex && bPVP == TRUE)	// PVP �����ϰ��
#ifdef DUEL_SYSTEM_20031028
					|| DuelIndex == tObjNum
#endif
					)
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
					}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
				else if (// ������ �߿� ������Ե� ����ȴ�.
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
				
				if( EnableAttack)
				{
					if( gObjCalDistance(lpObj, &gObj[tObjNum]) < iDistance )
					{	
						nHitCount++;
						if(nHitCount > 5)
						{
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}
						
						if(nHitCount > 12)
						{
							bAttack = FALSE;
						}
						
						if(bAttack == TRUE)
						{			
							gObjAddAttackProcMsgSendDelay(lpObj, 
								MSG_ATTACKPROC_ENTIREMAGIC, 
								tObjNum,
								250, lpMagic->m_Skill);
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
	
	return TRUE;
}

// ����Ŀ �ɰ� �ٽ� �ɸ� ������ �ٽ� ����(���� �ٽ� ����)
BOOL	CObjUseSkill::SkillBerserker(int aIndex, CMagicInf *lpMagic) // ����Ŀ
{
	if (!CHECK_LIMIT(aIndex, MAX_OBJECT))
		return FALSE;

	LPOBJECTSTRUCT lpObj		= &gObj[aIndex];

	int iDuration = 30 + ( lpObj->Energy / 20 );
	int EffectUPValue = lpObj->Energy/30;
	int EffectDownValue = lpObj->Energy/60;
	
	gObjAddBuffEffect( lpObj, BUFFTYPE_BERSERKER, EFFECTTYPE_BERSERKER_UP, EffectUPValue, EFFECTTYPE_BERSERKER_DOWN, EffectDownValue, iDuration );

	GCMagicAttackNumberSend(lpObj, AT_SKILL_BERSERKER, lpObj->m_Index, true);

	return TRUE;
}

#endif //SEASON4_PATCH_GROOVING

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// ȯ����ų �޷��
// �޷�� ��ų ���
BOOL CObjUseSkill::UseSkillPollution(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int nMsg)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;

	if(nMsg == MSG_SKILL_PULLUTION1)
	{
		GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, 1);
	}	

	// ���� ��ų ���
	bRet = SkillAreaMonsterAttack(aIndex, lpMagic, x, y, aTargetIndex, 3, TRUE);// pvp �����ϰ� ���� (grooving)

	// ���� ��ġ�� ������ ����ϱ� ���� ���� �޽����� ����
	if(nMsg >= MSG_SKILL_PULLUTION1 && nMsg < MSG_SKILL_PULLUTION1 + 5)
	{
		gObjAddMsgSendDelay( lpObj, MSG_SKILL_PULLUTION1, aTargetIndex, 1000, MAKELONG(MAKEWORD(x, y), nMsg) );	
	}

	return bRet;
}

// ���� ��ų ���
BOOL CObjUseSkill::SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP,int nDelay)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	BOOL bRet = FALSE;
	int count = 0;
	int tObjNum;
#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

#ifdef MODIFY_SEASON4_NEWSKILL_20080909
	int	nHitCount = 0;
	BOOL bAttack = FALSE;
#endif	// MODIFY_SEASON4_NEWSKILL_20080909
	
#ifdef MODIFY_MAGUMSA_SKILL_BUGFIX_20081015	// Hit ��ü�� ����
	int nMinCount	= 4;	// 100% �� hit
	int nMaxCount	= 8;	// Max Hit 

	if( lpMagic->m_Skill == AT_SKILL_GIGANTICSTORM)
	{
		nMinCount = 8;
		nMaxCount = 12;

	}
#endif // MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
	
	// aIndex Connected üũ

	// ��ų ��� ���ɹ���üũ
	int iSkillDistance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);
	if( CalDistance(lpObj->X, lpObj->Y, x, y) <= iSkillDistance )
	{
		BOOL	EnableAttack;
		while(1)
		{
			if( lpObj->VpPlayer2[count].state )
			{
				tObjNum = lpObj->VpPlayer2[count].number;
				
				if( tObjNum >= 0 )
				{
					EnableAttack = FALSE;
					
					// ����� ���� ���� �ִ� ����(VpPlayer2[])���� Ȯ���Ѵ�.
					
					if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
					{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
						EnableAttack = TRUE;
					}	
					else if( (tObjNum == aTargetIndex && bPVP == TRUE)	// bPVP �� FALSE �� ���� ���������� ������
#ifdef DUEL_SYSTEM_20031028
						|| DuelIndex == tObjNum
#endif
						)
					{
						EnableAttack = TRUE;
					}
#ifdef GAMESERVER_TO_CASTLESIEGESERVER
					else if (// ������ �߿� ������Ե� ����ȴ�.
						lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
						gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
						g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
						)
					{

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
					
					if( EnableAttack)
					{
						// x, y���� aDistance�� �ݰ�
						if( gObj[tObjNum].X >= x-aDistance && gObj[tObjNum].X  <= x+aDistance
 							&& gObj[tObjNum].Y >= y-aDistance && gObj[tObjNum].Y <= y+aDistance )
						{
							gObj[tObjNum].lpAttackObj = lpObj;

#ifdef MODIFY_SEASON4_NEWSKILL_20080909
							nHitCount++;

#ifdef MODIFY_MAGUMSA_SKILL_BUGFIX_20081015	// Attack ��ü �� ����
							if(nHitCount > nMinCount)
#else	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
							if(nHitCount > 5)
#endif	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
							{
								if( rand()%2 )
								{
									bAttack = TRUE;
								}							
							}
							else 
							{
								bAttack = TRUE;
							}

#ifdef MODIFY_MAGUMSA_SKILL_BUGFIX_20081015	// Attack ��ü �� ����
							if(nHitCount > nMaxCount)
#else	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
							if(nHitCount > 8)
#endif	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
							{
								bAttack = FALSE;
							}

							if(bAttack == TRUE)
							{
								if (nDelay == 0 )
								{
									// ���� ���� ����
									gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);
								}
								else
								{
									// ���� �� ���� ���
									gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, nDelay, lpMagic->m_Skill);
								}
							}
#else	// MODIFY_SEASON4_NEWSKILL_20080909
							if (nDelay == 0 )
							{
								// ���� ���� ����
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);
							}
							else
							{
								// ���� �� ���� ���
								gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, nDelay, lpMagic->m_Skill);
							}
#endif	// MODIFY_SEASON4_NEWSKILL_20080909
							
							bRet = TRUE;
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

	return bRet;
}
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425

#ifdef ADD_SEASON4_NEW_SKILL_20080522	// �ű� ��ų ���
// �ı��� �ϰ�(����)
BOOL CObjUseSkill::SkillDestruction(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	BOOL bRet = FALSE;

	// ���� ��ų ���
	bRet = SkillAreaMonsterAttack(aIndex, lpMagic, x, y, aTargetIndex, 3, TRUE, 500 );

	return bRet;
}

// (-tx1, tx1), (tx1, tx1), (-tx2, ty2), (tx2, ty2) �� ������ (0, 0)�� �������� bAngle ��ŭ ȸ��
void CObjUseSkill::SkillFrustrum3(int aIndex, BYTE bAngle, float tx1, float ty1, float tx2, float ty2 )
{	
	LPOBJECTSTRUCT lpObj=(LPOBJECTSTRUCT)&gObj[aIndex];

	vec3_t p[4];

	Vector(-tx1,	ty1 ,	0.f,	p[0]);
	Vector(tx1,		ty1 ,	0.f,	p[1]);
	Vector(tx2,		ty2 ,	0.f,	p[2]);
	Vector(-tx2,	ty2 ,	0.f,	p[3]);

	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,(float)(bAngle*360/255),Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t vFrustrum[4];
	
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);
		lpObj->fSkillFrustrumX[i] = (int)(vFrustrum[i][0])+lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)(vFrustrum[i][1])+lpObj->Y;
	}
}

// �÷��� ��Ʈ����ũ (���˻�)
void CObjUseSkill::SkillFlameStrike(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;	
	SkillFrustrum3(aIndex,bAngle, 2.f, 4.f, 5.f, 0.f);

	int tObjNum;
	int count = 0;

	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������������ ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
					{
						bAttack = FALSE;
						
						if( HitCount >= 8 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

#ifdef MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
						if( bAttack )
						{
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);					
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);
						}
#else	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015
						if( bAttack )
							gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);					
#endif	// MODIFY_MAGUMSA_SKILL_BUGFIX_20081015				

						HitCount++;
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

// �Ⱓƽ ����(���˻�)
BOOL CObjUseSkill::SkillGiganticStorm(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex)
{
	BOOL bRet = FALSE;

	// ���� ��ų ���
	bRet = SkillAreaMonsterAttack(aIndex, lpMagic, x, y, aTargetIndex, 6, TRUE, 400 );

	return bRet;
}

// ��Ƽ��(����)
void CObjUseSkill::SkillMultiShot(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	SkillFrustrum3(aIndex,bAngle, 6.f, 7.f, 1.f, 0.f);

	//////////////////////////////////////////////////////////////////////////
	// ȭ���� ���󰡴� ��ǥ�� ����
	vec3_t p;
	Vector(0.f, 6.f, 0.f, p);

	// �¿�� 20�� 40�� ȸ��
	vec3_t Angle[4];
	float Matrix[3][4];
	Vector(0.f, 0.f, (float)(40.f), Angle[0]);
	Vector(0.f, 0.f, (float)(20.f), Angle[1]);
	Vector(0.f, 0.f, (float)(340.f), Angle[2]);
	Vector(0.f, 0.f, (float)(320.f), Angle[3]);
	
	vec3_t vLine[5];
	AngleMatrix(Angle[0], Matrix);
	VectorRotate(p, Matrix, vLine[0]);
	AngleMatrix(Angle[1], Matrix);
	VectorRotate(p, Matrix, vLine[1]);
	Vector(0.f, 6.f, 0.f, vLine[2]);
	AngleMatrix(Angle[2], Matrix);
	VectorRotate(p, Matrix, vLine[3]);
	AngleMatrix(Angle[3], Matrix);
	VectorRotate(p, Matrix, vLine[4]);
	
	// vLine �� bAngle ��ŭ ȸ��
	int vSkillLineX[5];
	int vSkillLineY[5];

	vec3_t vFrustrum[5];

	Vector(0.f,0.f,(float)(bAngle*360/255),Angle[0]);
	AngleMatrix(Angle[0],Matrix);
	for(int i=0;i<5;i++)
	{
		VectorRotate(vLine[i], Matrix, vFrustrum[i]);
		vSkillLineX[i] = (int)(vFrustrum[i][0]) + lpObj->X;
		vSkillLineY[i] = (int)(vFrustrum[i][1]) + lpObj->Y;
	}		
	//////////////////////////////////////////////////////////////////////////

	int tObjNum;
	int count = 0;

	int	HitCount = 0;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������������ ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					// (lpObj->X, lpObj->Y), (vSkillLineX, vSkillLineY)�� ������ (gObj[tObjNum].X, gObj[tObjNum].Y) �� �����ϴ��� Ȯ��
					int nResult[5];
					
					for(int i = 0; i < 5; i++)
					{
						nResult[i] = (vSkillLineX[i] - lpObj->X) * (gObj[tObjNum].Y - lpObj->Y) - (vSkillLineY [i] - lpObj->Y) * (gObj[tObjNum].X - lpObj->X);
								
						if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
						{
							if(nResult[i] > -5 && nResult[i] < 5)
							{
								gObjAttack(lpObj, &gObj[tObjNum], lpMagic, 1, 0);	
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
}

// ȸ��(����)
void CObjUseSkill::SkillRecovery(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	int skillSuccess = 1;
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];	
	LPOBJECTSTRUCT lpTargetObj= &gObj[aTargetIndex];

	if( lpTargetObj->Type != OBJTYPE_CHARACTER) 
		if( lpTargetObj->m_RecallMon == -1 ) return;
	
	if( lpObj->Class != CLASS_ELF ) return;

#ifdef CHAOSCASTLE_SYSTEM_20040408		// ī���� ĳ�������� ������ ���� �ڽſ��Ը� ���� �� �ִ�.
	#ifndef WORLD_TOURNAMENT_EVENT_SETTING			
		if(CHECK_CHAOSCASTLE(lpObj->MapNumber)) {
			if (lpObj->m_Index != lpTargetObj->m_Index)
				return;
		}
	#endif
#endif

	if(gObjDuelCheck(lpTargetObj))
	{
		lpTargetObj = lpObj;
	}
	
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �ִ� SD ����
	if( lpTargetObj->iShield >= (lpTargetObj->iMaxShield + lpTargetObj->iAddShield + lpTargetObj->m_MPSkillOpt.iMpsMaxSD) ) 
	{
		GCMagicAttackNumberSend(lpObj, AT_SKILL_RECOVERY, lpTargetObj->m_Index, 0);
		return;
	}
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpTargetObj->iShield >= (lpTargetObj->iMaxShield + lpTargetObj->iAddShield) ) 
	{
		GCMagicAttackNumberSend(lpObj, AT_SKILL_RECOVERY, lpTargetObj->m_Index, 0);
		return;
	}
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

	lpTargetObj->iShield += (lpObj->Energy / 4) + lpObj->Level;

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �ִ� SD ����
	if( lpTargetObj->iShield > (lpTargetObj->iMaxShield + lpTargetObj->iAddShield + lpTargetObj->m_MPSkillOpt.iMpsMaxSD) ) 
	{
		lpTargetObj->iShield =  lpTargetObj->iMaxShield +lpTargetObj->iAddShield + lpTargetObj->m_MPSkillOpt.iMpsMaxSD;
	}
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	if( lpTargetObj->iShield > (lpTargetObj->iMaxShield + lpTargetObj->iAddShield) ) 
	{
		lpTargetObj->iShield =  lpTargetObj->iMaxShield +lpTargetObj->iAddShield;
	}
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

	if( lpTargetObj->Type == OBJTYPE_CHARACTER ) 
	{
		GCReFillSend(lpTargetObj->m_Index, (WORD)lpTargetObj->Life, 0xFF, 0, (WORD)lpTargetObj->iShield);

		GCMagicAttackNumberSend(lpObj, AT_SKILL_RECOVERY, lpTargetObj->m_Index, skillSuccess);

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
		// ġ�� ���� ��� �� Target�� ���� �ִ� ������ Agro�� ���� �����ش�.
		for( int n=0; n<MAXVIEWPORTOBJECT; n++ )
		{
			if( lpObj->VpPlayer2[n].type == OBJTYPE_MONSTER )
			{
				if( lpObj->VpPlayer2[n].state )
				{
					LPOBJECTSTRUCT lpMonster = &gObj[lpObj->VpPlayer2[n].number];

					if( lpMonster->m_iCurrentAI )
						lpMonster->m_Agro.IncAgro(lpObj->m_Index, lpObj->Energy/5/40 );
				}
			}
		}
#endif

	}
}

// ��������(����)
BOOL CObjUseSkill::SkillMagicPower(int aIndex, int aTargetIndex, CMagicInf *lpMagic)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
	
	if( lpObj->Type != OBJTYPE_CHARACTER ) 
	{		
		return FALSE;
	}
	
	if( lpObj->Class != CLASS_WIZARD)
	{		
		return FALSE;
	}
	
	if( lpObj->m_Index != aTargetIndex )
	{		
		return FALSE;
	}	

	// �̹� �ߵ����� ���� ������ �ٽ� ����� �� ����.
	if( gObjCheckUsedBuffEffect( lpObj, BUFFTYPE_MAGIC_POWER_INC ) == true )
	{
		return FALSE;
	}

	// ���� ���밪 ������ 2%�� ����
	int nEffectValue = (float)(lpObj->Energy / 9) * (float)(20/100);

	gObjAddBuffEffect( lpObj, BUFFTYPE_MAGIC_POWER_INC, EFFECTTYPE_MAGICPOWER_INC, nEffectValue, 0, 0, 30*60 );

	GCMagicAttackNumberSend( lpObj, AT_SKILL_MAGICPOWRER, lpObj->m_Index, 1 );

	return TRUE;
}

// ī��ƽ ���̾�(��ũ�ε�)
void CObjUseSkill::SkillchaoticDesair(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex)
{
	LPOBJECTSTRUCT lpObj= &gObj[aIndex];
		
	int StartDis = 1;	
	SkillFrustrum3(aIndex,bAngle, 1.5f, 6.f, 1.5f, 0.f);

	int tObjNum;
	int count = 0;

	int	HitCount = 0;
	int	bAttack;

#ifdef DUEL_SYSTEM_20031028
	int DuelIndex = lpObj->m_iDuelUser;
#endif

	BOOL	EnableAttack;
	
	while(1)
	{
		if( lpObj->VpPlayer2[count].state )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if( tObjNum >= 0 )
			{
				EnableAttack = FALSE;

				if( ((lpObj->VpPlayer2[count].type == OBJTYPE_MONSTER) && (gObj[tObjNum].m_RecallMon < 0)) )
				{	// ����� �����̰�, ��ȯ���� �ƴϸ� ���ݰ���
					EnableAttack = TRUE;
				}
				else if( tObjNum == aTargetIndex 
	#ifdef DUEL_SYSTEM_20031028
						 || DuelIndex == tObjNum
	#endif
						 )
				{	// Ŭ���̾�Ʈ���� ����� ���� ������������ ���� ����϶��� ���ݰ���
					EnableAttack = TRUE;
				}
#ifdef MODIFY_RANGEATTACK_CHECK_20050113		// ������������ ������ �߿� ������Ե� ����ȴ�.
				else if (
					lpObj->VpPlayer2[count].type != OBJTYPE_NPC &&
					gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE &&
					g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE
					)
				{
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
					if( SkillTestFrustrum(gObj[tObjNum].X, gObj[tObjNum].Y, aIndex) )
					{
						bAttack = FALSE;
						
						if( HitCount >= 8 )
						{	
							if( rand()%2 )
							{
								bAttack = TRUE;
							}							
						}
						else 
						{
							bAttack = TRUE;
						}

						if( bAttack )
							gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 200, lpMagic->m_Skill);

#ifdef UPDATE_CHAOTICDESAIR_SKILL_20090109	// target �� ������ ������Ʈ�� �ѹ��� ��Ʈ
						if( CHECK_LIMIT(aTargetIndex, MAX_OBJECT) && tObjNum == aTargetIndex )
						{
							gObjAddAttackProcMsgSendDelay(lpObj, MSG_ATTACKPROC_ENTIREMAGIC, tObjNum, 300, lpMagic->m_Skill);
						}
#endif	// UPDATE_CHAOTICDESAIR_SKILL_20090109						
						
						HitCount++;
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
#endif	// ADD_SEASON4_NEW_SKILL_20080522