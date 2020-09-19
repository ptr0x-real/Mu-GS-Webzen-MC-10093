// ObjUseSkill.h: interface for the CObjUseSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_)
#define AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjUseSkill  
{
public:
	CObjUseSkill();
	virtual ~CObjUseSkill();

	// �Ϲ� ���� ���� ������ üũ
	BOOL	SpeedHackCheck(int aIndex);
	// ��밡���� ��ų �����ΰ�?
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107	// ��ų��ȣ BYTE -> WORD 
	BOOL	EnableSkill( int Skill, int SkillLevel );
#else
	BOOL	EnableSkill(BYTE Skill);
#endif
	// �ʿ��� �������� ��´�
	int		GetUseMana(int aIndex, CMagicInf *lpMagic);	

#ifdef ADD_SKILL_ADDITION_INFO_CLASS_20060407
	int		GetAddUseMana( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic );
#endif

#ifdef NEW_FORSKYLAND2
	// �ʿ��� AG�Ŀ����� ��´�	
	int		GetUseBP(int aIndex, CMagicInf *lpMagic);	
#endif

	// ��ų�� ����Ѵ�(���Ӹ��� : ���������� �Ŀ� Ŭ���̾�Ʈ���� ��� �´�)
#ifdef EXPAND_ATTACK_PACKET_0X1E
	void	UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos = 0, int aTargetIndex = -1);
#else
	void	UseSkill(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos = 0);
#endif

	// ��ų ������ �Ѵ�(���� : ���������� ���� �� ��ų ��� ������ �Ѵ�
	void	UseSkill(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ��ų �ߵ�
	BOOL	RunningSkill(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL bCombo = FALSE);

	// ���˻���°���
	void	MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level);
	// ��� ����� ����
	void	KnightSkillAddLife(int aIndex, int skill_level);
	// �����������
#ifdef NEW_FORSKYLAND3
	void	WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level);
#else
	void	WizardMagicDefense(int aIndex, int skill_level);
#endif
	// �������� (��30�ʰ� (�ƾ��۷���+1)*3��ŭ ����)
	void	SkillDefense(int aIndex, int aTargetIndex, int skill_level);
	//���ݷ����� (��30�ʰ� (�ƾ��۷���+1)*3��ŭ ����)
	void	SkillAttack(int aIndex, int aTargetIndex, int skill_level);
	// ���� ���� ���
	BOOL	SkillChangeUse(int aIndex);
	// ������ ���� ������ ����Ѵ�
	void	SkillHealing(int aIndex, int aTargetIndex, int skill_level);
	// ���͸� ��ȯ�Ѵ�.
	BOOL	SkillMonsterCall(int aIndex, int MonsterType, int x, int y);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ����
	////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef NEW_ATTACK_PROC		
	// ����
	void	SkillPenetration(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	// ���� ����(�������)
	void	SkillDefenseDown(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	// �ұ��
	void	SkillFlame(int aIndex, CMagicInf *lpMagic);			
	// �Ƿ�
	void	SkillEvil(int aIndex, CMagicInf *lpMagic);			
	// �����̾�
	void	SkillHellFire(int aIndex, CMagicInf *lpMagic);			
	// ����Ʈ
	void	SkillBlast(int aIndex, CMagicInf *lpMagic);		

	void	SkillWheel(int aIndex, CMagicInf *lpMagic);

	void	SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���� ���� ��
	////////////////////////////////////////////////////////////////////////////////////////////////////////	
#endif

#ifdef ADD_SKILL_WITH_COMBO	
	BOOL	SkillDeathPoison(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	BOOL	SkillSuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);

	// �����̾�2 ����
	BOOL	SkillHellFire2Start(int aIndex, CMagicInf *lpMagic);
	// �����̾�2 ����
	BOOL	SkillHellFire2(int aIndex, int aTargetIndex,CMagicInf *lpMagic);	

	// �г��� �ϰ�
	void	SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo = FALSE);
	// ȸ����
	void	SkillWheel(int aIndex, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo = FALSE);

	// ��ο�(���)
	BOOL	SkillKnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo = FALSE);	
#endif

#ifdef ADD_SKILL_20031022			
	// �Ŀ� ������ (���� ��ų)
	#ifdef EXPAND_ATTACK_PACKET_0X1E
		void SkillPowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	#else
		void SkillPoweSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos);
	#endif
#endif

	void	SkillFrustrum(int aIndex, float bangle, float tx,float ty);
	int		GetTargetLinePath(int sx, int sy, int tx, int ty, int* PathX, int* PathY, int distance);

	int		GetAngle(int x, int y, int tx, int ty);
	int		CalDistance(int x, int y, int x1, int y1);

#ifdef DARKLORD_WORK

	void	SkillFrustrum2(int aIndex, float fangle, float tx,float ty);
	// ��ų ���� ������ �˻� �Ѵ�: ���Ⱒ, ������ġ(x,y), �����ġ(tx,ty) �Ÿ�(dis), ȣ��ũ��(arc)
	BOOL	SkillAreaCheck(int angel, int x, int y, int tx, int ty, int dis, int arc);
	
	// ���Ǿ�(��ũ�ε�)
	BOOL	SkillSpear(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	// ���̾� ����Ʈ(��ũ�ε�)
	BOOL	SkillFireBurst(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	// ��ũȣ�� ����(��ũ�ε�)
	BOOL	SkillDarkHorseAttack(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	// ��Ƽ ��ȯ�ɸ� ��Ƽ���� �ִ��� �˻�
	BOOL	RecallPartyCheck(int aIndex, int skill_level);
	// ��Ƽ ��ȯ(��ũ�ε�)
	void	SkillRecallParty(int aIndex, int skill_level);
	// ũ��Ƽ�� �������߰�(��ũ�ε�)
	void	SkillAddCriticalDamage(int aIndex, int skill_level);
	// �Ϸ�Ʈ�� ����ũ(��ũ�ε�)
	void	SkillElectricSpark(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
#endif
	
#ifdef ADD_NEW_MAP_KALIMA_20040518
	// ���Ͱ� ���͸� ��ȯ�ϴ� ����
	BOOL	SkillSummon			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ������� ����
	BOOL	SkillImmuneToMagic	(int aIndex, CMagicInf *lpMagic);
	// ������� ����
	BOOL	SkillImmuneToHarm	(int aIndex, CMagicInf *lpMagic);
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// [ 0x19 ]

	// ���̵� ����Ʈ
	BOOL	SkillKnightRush			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// �ҿ� ������
	BOOL	SkillWizardJavalin		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ���� ����
	BOOL	SkillElfRush			(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ����� ���˻�
	BOOL	SkillMagumOneFlash		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ������ ���˻�
	BOOL	SkillMagumDeathCannon	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ��ũ�ε� ���� ��ų
	BOOL	SkillDarklordSpaceSplit	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

	// ��ũ�ε� ���� ��ų
	BOOL	SkillBrandOfSkill	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ���� ����	
	BOOL	SkillRemoveStun		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ���� ����
	BOOL	SkillAddMana		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ����
	BOOL	SkillCloaking		(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ���� ����
	BOOL	SkillRemoveCloaking	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// ���� ����
	BOOL	SkillRemoveMagic	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

#ifdef ADD_NEW_SKILL_FOR_CASTLE_02_20050531
	BOOL	RemoveCloakingEffect(int aIndex);
#endif

	// [ 0x1E ]
	// ����
	BOOL	SkillStun			(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos );

	// ��ų ���� ����
	BOOL	RemoveAllCharacterInvalidMagicAndSkillState( LPOBJECTSTRUCT lpObj );	
#endif
	
#ifdef ADD_NEW_MONSTER_SYSTEM_01_20050714
	BOOL	RemoveSpecificSkillState( LPOBJECTSTRUCT lpObj, INT iSkillNumber );

#endif


#ifdef ADD_ITEM_FENRIR_01_20051110

	// [ 0x1E ]
	BOOL	SkillFenrirAttack	(int aIndex, int aTargetIndex, CMagicInf *lpMagic);

#endif
	
#ifdef ADD_SKILL_MUSE_ELF_INFINITYARROW_20060406
	// [ 0x19 ]
	BOOL	SkillInfinityArrow( int aIndex, int aTargetIndex, CMagicInf *lpMagic );// ����� ��ų - ���Ǵ�Ƽ �ַο�
#endif
	
#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601	// ���̾� ��ũ�� ���� ó�� �Լ� ����
	// [ 0x1D ]
	BOOL	SkillFireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic );
	BOOL	FireScreamExplosionAttack( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int iAttackDamage);		// ���̾� ��ũ���� ����� �� ���������� �߸� ������ ���͵� �������� �޴´�.
#endif

#ifdef MODIFY_SKILL_EVEIL_AT_SERVER_20070117
	// [ 0x1E ]
	BOOL	SkillNewEvil( int aIndex, int aTargetIndex, CMagicInf *lpMagic );		// �Ƿ� - ��������
#endif

#ifdef ADD_SKILL_DARKLOAD_FIRESCREAM_20060601
	//////////////////////////////////////////////////////////////////////////
	//	Ư�� ��ų�� ���� �߰� ó��
	//////////////////////////////////////////////////////////////////////////
	void SpecificSkillAdditionTreat( LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, CMagicInf* lpMagic, int iAttackDamage );
#endif

#ifdef ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912
	BOOL	SkillLightningOrb(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	// ���÷��� ������ ���
	BOOL	SplashDamage(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj, int nSkill, int nAttackDamage, int nDistance = 1, int nRate = 50);

	BOOL	SkillDrainLife(int aIndex, int aTargetIndex,CMagicInf *lpMagic);
	BOOL	SkillChainLightning(int aIndex, int aTargetIndex,CMagicInf *lpMagic);

	BOOL	SkillSummons(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// ��ȯ�� ��ų
	
	BOOL	SkillThorns(int aIndex, int aTargetIndex, CMagicInf *lpMagic);	// ����(�������ݻ�)
	BOOL	SkillSleep(int aIndex, int aTargetIndex, CMagicInf *lpMagic);		// ����
	BOOL	SkillBlind(int aIndex, int aTargetIndex, CMagicInf *lpMagic);	// ����ε�
#endif	// ADD_SEASON_3_NEW_CHARACTER_SUMMONER_20070912	
	
#ifdef UPDATE_SUMMONER_NEW_SKILL_20080111
	BOOL	SkillWeakness(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	BOOL	SkillEnervation(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
#endif

#ifdef SEASON4_PATCH_GROOVING
	// --->
	// ���� 4.0 �߰� ��ų  grooving 2008/04/24
	BOOL	SkillLightningShock( int aIndex, int aTargetIndex, CMagicInf *lpMagic );	// ����Ʈ�� ��ũ 
	BOOL	SkillBerserker(int aIndex, CMagicInf *lpMagic);			// ����Ŀ
	// <---
#endif

#ifdef UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	// ȯ����ų �޷��
	BOOL	UseSkillPollution(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int nMsg = MSG_SKILL_PULLUTION1);		// �޷��
	BOOL	SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP = TRUE, int nDelay = 0);	// ���� ��ų ���
#endif	// UPDATE_ADD_SUMMONER_SKILL_POLLUTION_20080425	
	
#ifdef ADD_SEASON4_NEW_SKILL_20080522	//  �ű� ��ų ���
	BOOL	SkillDestruction(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);		// �ı��� �ϰ�(����)
	void	SkillFrustrum3(int aIndex, BYTE bAngle, float tx1, float ty1, float tx2, float ty2 );
	void	SkillFlameStrike(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);		// �÷��� ��Ʈ����ũ(���˻�)
	BOOL	SkillGiganticStorm(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// �Ⱓƽ ����(���˻�)
	void	SkillMultiShot(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);			// ��Ƽ��(����)
	void	SkillRecovery(int aIndex, int aTargetIndex, CMagicInf *lpMagic);						// ȸ��(����)
	BOOL	SkillMagicPower(int aIndex, int aTargetIndex, CMagicInf *lpMagic);						// ���� ����(����)
	void	SkillchaoticDesair(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);		// ī��ƽ ���̾�(��ũ�ε�)
#endif	// ADD_SEASON4_NEW_SKILL_20080522
};

extern CObjUseSkill gObjUseSkill;

#endif // !defined(AFX_OBJUSESKILL_H__E0BD0DDC_575D_4D6E_BCE5_6A3C248B679A__INCLUDED_)
