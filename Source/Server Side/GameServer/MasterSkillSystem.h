// MasterSkillSystem.h: interface for the CMasterSkillSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_)
#define AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122

#include "protocol.h"

// �����
enum _RESULT_MSG
{
	RM_ERROR				= 0,	// ����
	RM_OK					= 1,	// ��~����!
	RM_INVALID_USER			= 2,	// �̻��� ����
	RM_INVALID_SKILL_NUM	= 3,	// �̻��� ��ų ��ȣ	
	RM_INVALID_CONDITION	= 4,	// ��ų�� �����ϱ� ���� �䱸���ǿ� ���� ����
	RM_ADD_MAGIC_FAIL		= 5,	// ���� �߰� ����
};

const INT	MAX_SKILL_TABLE_DATA	= 21;	// 1~20�� ���

const BYTE	MAX_TABLE_ROW			= 8;
const BYTE	MAX_TABLE_COLUMN		= 4;

const BYTE	MAX_RANK				= 9;	// 1~9
const BYTE	MAX_RANK_LEVEL			= 5;	// Rank1/1~4
const BYTE	BASE_RANK				= 1;	// ��ũ�� 1���� ����
const BYTE	BASE_RANK_LEVEL			= 1;	// ��ũ ������ 1���� ����
const BYTE	BASE_ML_SKILL_LEVEL		= 1;	// �����ͷ��� ��ų ������ 1���� ����
const BYTE	BASE_ML_SKILL_LEVEL_GAP	= 1;	// ���� ��ũ�� �����ͷ��� ��ų ������ 1�� ���̳���.

typedef struct _ML_SKILL_TABLE
{
	// �����͸� �̸� ����� �д�.
	BYTE	btClass;		// ĳ���� Ŭ����
	BYTE	btType;			// 4���� �� � Ÿ���ΰ�.

	// ���� ������
	BYTE	btWidth;									// �� ��¥��?
	INT		iAbility[MAX_TABLE_ROW][MAX_TABLE_COLUMN];	// ��ų���̺� ����
} ML_SKILL_TABLE, *LPML_SKILL_TABLE;


typedef struct  _MASTERLEVEL_PASSIVE_SKILL_OPTION
{
	INT		iMpsAttackSuccessRate;			// ���ݼ����� ���

	INT		iMpsPVPAttackDmgRate;			// ���ΰ��ݼ����� ���
	INT		iMpsPVPBlockingRate;			// ���ι����� ����

	INT		iMpsDownDur1;					// ���� ����1
	BYTE	btMpsDownDur1Level;				// ���� ����1 ����	( **��� �ɼ��� ������ �������� �ʰ� �ʿ��� ���� �߰��Ѵ�. )
	INT		iMpsDownDur2;					// ���� ����2	: ����, ����/�����

	INT		iMpsResistancePoison;			// �� ���׷� ���
	INT		iMpsResistanceThunder;			// ���� ���׷� ���
	INT		iMpsResistanceIce;				// ���� ���׷� ���

	INT		iMpsAutoRecoverLife;			// ����� �ڵ�ȸ�� ��ȭ

	INT		iMpsPlusZen;					// �� ����
	
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	// �нú꽺ų 2�� �߰� ������
	INT		iMpsDefence;					// ���� ����

	INT		iMpsMaxHP;						// �ִ�HP ����
	INT		iMpsMaxBP;						// �ִ�AG(BP) ����

	INT		iMpsMonsterDieGetMana;			// ���� ų �� ����ȸ��
	INT		iMpsMonsterDieGetLife;			// ���� ų �� �����ȸ��
	INT		iMpsMonsterDieGetSD;			// ���� ų �� SDȸ��

	INT		iMpsPlusExp;					// ����ġ ���
#endif	// ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// �нú꽺ų 4��ũ �߰�
	INT		iMpsMaxSD;						// �ִ� SD ����
	INT		iMpsSDSpeed;					// SD ȸ�� �ӵ� ���
	INT		iMpsMaxAttackDamage;			// �ִ� ���ݷ� ���
	INT		iMpsMinAttackDamage;			// �ּ� ���ݷ� ���
	INT		iMpsDecreaseMana;				// ���� ���� ��ȭ
	INT		iMpsMaxMagicDamage;				// �ִ� ���� ���
	INT		iMpsMinMagicDamage;				// �ּ� ���� ���
	INT		iMpsPetDurDownSpeed;			// �� ���� �Ҹ� �ӵ� ����	
	INT		iMpsMaxAttackMagicDamage;		// �ִ� ���ݷ�/���� ���
	INT		iMpsMinAttackMagicDamage;		// �ּ� ���ݷ�/���� ���
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428

} MASTERLEVEL_PASSIVE_SKILL_OPTION, *LPMASTERLEVEL_PASSIVE_SKILL_OPTION;



class CMasterSkillSystem  
{
private:
	INT				m_iRankTable[MAX_RANK][MAX_RANK_LEVEL];		// ��ų �ܰ躰 ���� �䱸 ����Ʈ	����
	INT				m_iRankPointSum[MAX_RANK];					// ��ũ���� �䱸�Ǵ� ����Ʈ(���� ��ũ�� �Ѿ�� ���� ����Ʈ)
	ML_SKILL_TABLE	m_MLSkillTable[MAX_SKILL_TABLE_DATA];		// ��ų Ʈ�� ����(Ŭ���̾�Ʈ ǥ�ÿ�)
																// ������ �Ⱦ��µ� ���߿� �� �� ������� �о�д�.

public:
	CMasterSkillSystem();
	virtual ~CMasterSkillSystem();

	VOID	Init();
	VOID	Load( LPCSTR lpszFileName );


	//////////////////////////////////////////////////////////////////////////
	// S E T
	//////////////////////////////////////////////////////////////////////////
	VOID	InitPassiveSkillData( LPOBJECTSTRUCT lpObj );									// ������ �нú꽺ų �����͸� �ʱ�ȭ�Ѵ�.
	VOID	CalcPassiveSkillData( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );	// ������ ���� �нú꽺ų �����͸� ����Ѵ�.


	//////////////////////////////////////////////////////////////////////////
	// C H E C K
	//////////////////////////////////////////////////////////////////////////	
	INT		CheckSkillCondition( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );		// �����ͷ��� ��ų�� ������ �� �ִ��� ���� �˻�
	BOOL	CheckCompleteBrandOfSkill( LPOBJECTSTRUCT lpObj, INT iMLSkill );				// ���ེų�� �޼��Ǿ����� Ȯ���Ѵ�.

#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	BOOL	CheckMasterLevelSkill( INT iMLSkill );											// ��ų ��ȣ�� �����ͷ��� ��ų���� Ȯ���Ѵ�.
	BOOL	IsExistMLSBasedSkill( LPOBJECTSTRUCT lpObj, INT iSkill );						// �Ϲ� ��ų��ȣ�� �⺻�̵Ǵ� �����ͷ��� ��ų�� �־�?
#endif
	

	//////////////////////////////////////////////////////////////////////////
	// G E T
	//////////////////////////////////////////////////////////////////////////
#ifdef ADD_MASTER_LEVEL_SKILL_PASSIVE_20080107	
	INT		GetBaseMasterLevelSkill( INT iMLSkill );										// ��ų������ �⺻ �����ͷ��� ��ų ��ȣ�� ��´�.	
#endif


#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	//////////////////////////////////////////////////////////////////////////
	// A D D
	//////////////////////////////////////////////////////////////////////////	
	INT		gObjMagicAdd_MLS( LPOBJECTSTRUCT lpObj, INT iMLSkill, INT iSkillLevel );		// �����ͷ�����ų�� ��������Ʈ�� �߰��Ѵ�.	
#endif


	//////////////////////////////////////////////////////////////////////////
	// Client <-> GS ó��
	//////////////////////////////////////////////////////////////////////////
	VOID CGReqGetMasterLevelSkill( LPPMSG_REQ_MASTERLEVEL_SKILL lpMsg, int aIndex );		// CG [0xF3][0x52] �����ͷ��� ��ų ���� ��û		


#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	//////////////////////////////////////////////////////////////////////////
	// A C T I V E    S K I L L   R U N N I N G
	//////////////////////////////////////////////////////////////////////////
#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ������ ���� 4��ũ( ��ų ��� ��ġ �߰�)
	BOOL	RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x = 0, BYTE y = 0, BYTE aTargetPos = 0 );
#else	// UPDATE_MASTER_LEVEL_4RANK_20080428
	BOOL	RunningSkill_MLS( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic, BOOL bCombo );
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
	VOID	MLS_WizardMagicDefense( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );				// �ҿ�ٸ��� ��ȭ
	VOID	MLS_Healing( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );						// ġ�ᰭȭ (����)
	VOID	MLS_Wheel( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic, INT aTargetIndex, BOOL isCombo = FALSE );	// ȸ�������� ��ȭ	
	VOID	MLS_KnightSkillAddLife( LPOBJECTSTRUCT lpObj, CMagicInf *lpMagic );											// ���������� ��ȭ	
	BOOL	MLS_DarkHorseAttack( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );				// �����ũ��ȭ
	BOOL	MLS_FireBurst( LPOBJECTSTRUCT lpObj, INT aTargetIndex, CMagicInf *lpMagic );						// ���̾����Ʈ ��ȭ
#endif

#ifdef UPDATE_MASTER_LEVEL_4RANK_20080428	// ������ ���� 4��ũ
	BOOL	MLS_SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP = TRUE, int nSuccessRate = 0, int nDelay = 0);	// ���� ��ų ���
	BOOL	MLS_SuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);	// ���� ���̽� ��ȭ
	int		MLS_CalDistance(int x, int y, int x1, int y1);
	int		MLS_GetAngle(int x, int y, int tx, int ty);
	void	MLS_SkillFrustrum(int aIndex, float fangle, float tx,float ty);
	BOOL	MLS_KnightBlow(int aIndex, int aTargetIndex,CMagicInf *lpMagic, BOOL isCombo);				// ��ο� ��ȭ
	void	MLS_SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);	// �г��� �ϰ� ��ȭ
	void	MLS_SkillDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic);				// ���� ���� ��ȭ
	void	MLS_SkillAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic);				// ���ݷ� ���� ��ȭ
	void	MLS_MaGumSkillDefenseDown(int aIndex, int aTargetIndex);	// ���� ���� ��ȭ
	void	MLS_PowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE Targetangle, int aTargetIndex);	// �Ŀ� ������ ��ȭ
	BOOL	MLS_FireScream( int aIndex, int aTargetIndex, CMagicInf *lpMagic );		// ���̾� ��ũ�� ��ȭ
#endif	// UPDATE_MASTER_LEVEL_4RANK_20080428
	
#ifdef MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807
// ��ȭ�� ���⽺ų ����(���������δ� lpObj->Magic[n]���� ������ ����)
	BOOL	MLS_WeaponSkillDel(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
// ��ȭ�� ���⽺ų �߰�(���������δ� lpObj->Magic[n]���� �߰����� ����)
	BOOL	MLS_WeaponSkillAdd(LPOBJECTSTRUCT lpObj, BYTE aSkill, BYTE Level);
	BOOL	CheckUsableWeaponSkill(LPOBJECTSTRUCT lpObj, int nCount);
#endif	// MODIFY_WEAPONSKILL_STRENGTHEN_ICON_BUGFIX_20080807	
};

extern CMasterSkillSystem g_MasterSkillSystem;
#endif

#endif // !defined(AFX_MASTERSKILLSYSTEM_H__D1E7613A_BBDC_4296_8186_0043960ABF4B__INCLUDED_)
