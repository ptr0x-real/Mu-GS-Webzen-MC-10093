// MagicDamage.h: interface for the CMagicDamage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_)
#define AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
#define MAX_STATUS	3	// ��帶����, �α�帶����, ��Ʋ������
enum SKILL_USABLE_STATUS
{
	SKILL_USABLE_STATUS_GUILDMASTER		= 0,
	SKILL_USABLE_STATUS_SUBMASTER		= 1,
	SKILL_USABLE_STATUS_BATTLEMASTER	= 2,
};

enum SKILL_USETYPE
{
	SKILL_USETYPE_NORMAL	= 0,	// �Ϲݽ�ų
	SKILL_USETYPE_STATUS	= 1,	// ��å��ų
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	// 2���� Ŭ���̾�Ʈ���� ���𰡿� �����ִٰ� �Ѵ�.
	SKILL_USETYPE_MASTERSKILL_PASSIVE	= 3,	// �����ͷ��� ������ ��� ������ ��ų Ÿ�� - �нú꽺ų
#endif
#ifdef ADD_MASTER_LEVEL_SKILL_SYSTEM_ACTIVE_20080107
	SKILL_USETYPE_MASTERSKILL_ACTIVE	= 4,	// �����ͷ��� ������ ��� ������ ��ų Ÿ�� - ��Ƽ�꽺ų
#endif	
};

#endif


class CMagicDamage
{
private :
	int  m_Damage[MAX_SKILL];						// ������ ��
	char m_Name[MAX_SKILL][50];						// ��ų �̸�
	BYTE m_rLevel[MAX_SKILL];						// 
	WORD m_Mana[MAX_SKILL];							// ��ų ���� �Ҹ� ����
	BYTE m_Distance[MAX_SKILL];						// ��ų ��� �Ÿ�
	int  m_RequireEnergy[MAX_SKILL];				// �ʿ� ������
	BYTE m_RequireClass[MAX_SKILL][MAX_CLASSTYPE];	// ��� ������ Ŭ����


#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	int	m_RequireLevel[MAX_SKILL];					// �䱸 ����
#endif

#ifdef DARKLORD_WORK
	int  m_RequireLeadership[MAX_SKILL];			// �ʿ� ���
	int	 m_Delay[MAX_SKILL];						// ��ų ��� �� ������� �ɸ��� �ð�(�ʴ���)
#endif

#ifdef NEW_FORSKYLAND2
	WORD m_BrainPower[MAX_SKILL];					// �ʿ� AG
#endif

	int m_Attr[MAX_SKILL];							// �Ӽ�1 -1:���� 0:���� 1:�� 2:���� 3:�� 4:�� 5:�ٶ� 6:��

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int m_iSkillType[MAX_SKILL];					// �Ӽ�2 -1:���� 0:�������� 1:��������
#endif
	
#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116			// ��ų�� �귣��� ųī��Ʈ�� ��
	int		m_iSkillUseType[MAX_SKILL];				// ��å��ų ��
	int		m_iSkillBrand[MAX_SKILL];
	int		m_iKillCount[MAX_SKILL];
	int		m_iRequireStatus[MAX_SKILL][MAX_STATUS];
#endif
	
#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	short		m_iSkillRank[MAX_SKILL];			// ��ų��ũ	
	short		m_iSkillGroup[MAX_SKILL];			// ��ų�׷�	
	short		m_nRequireMLPoint[MAX_SKILL];		// �ʿ丶���ͷ��� ����Ʈ
	short		m_iHP[MAX_SKILL];					// �Ҹ�ü��	
	short		m_iSD[MAX_SKILL];					// �Ҹ�SD	
	WORD		m_wKeepTime[MAX_SKILL];				// ���ӽð�	
	short		m_iRequireStr[MAX_SKILL];			// �ʿ���	
	short		m_iRequireDex[MAX_SKILL];			// �ʿ��ø
#endif
	
	//int  m_Index[MAX_SKILL];	// ����å �ε��� (� �����ΰ��� �˷���)
	int  Get(int skill);

#ifdef EXPAND_SKILL_SCRIPT_FOR_MASTER_LEVEL_20071122
	// Ȯ�� �� ������ ��ų ������ �����Ѵ�.
	void SetMasterLevelSkillInfo( int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex );
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	void SetEx( int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int *pReqStatus );
#endif
	
#ifdef DARKLORD_WORK

	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay);
	#else
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy, int rLeadership, int rdelay);
	#endif

#else
	#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, int iSkillType, BYTE * ReqClass, int rEnergy);
	#else
		void Set(char *name, int skill, int damage, int rlevel, int mana, int bp, int dis, int Attr, BYTE * ReqClass, int rEnergy);
	#endif
#endif
	
	BOOL	CheckSkillAttr(int skill, int attr);


public:
#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812		// ��ų�� ����/���� �Ӽ� ��
	enum {
		SKILL_TYPE_NONE			= -1,
		SKILL_TYPE_PHYSICS		= 0,
		SKILL_TYPE_MAGIC		= 1,
	};
#endif

	CMagicDamage();
	virtual ~CMagicDamage();


	void Init();
	void LogSkillList(char *filename="..\\data\\skill.txt");
	void LogSkillNameList(char *filename);

#ifdef SCRIPT_DECODE_WORK	
	void LogSkillListDecode(char *filename);
	void LogSkillNameListDecode(char *filename);
#endif
	
#ifdef AUTH_GAMESERVER	
	void LogSkillList(char *Buffer, int	iSize);
	void LogSkillNameList(char *Buffer, int iSize);
#endif	

	int  SkillGet(int skill);
	int  SkillGetMana(int skill);
	int  SkillGetRequireEnergy(int skill);

#ifdef ADD_CHECKING_LEVEL_TO_LEARN_MAGIC_20041115
	int	 SkillGetRequireLevel(int skill);
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// SkillGetRequireClass(...) 3������ �߰�
	int  SkillGetRequireClass(int Cclass, int ChangeUP, int ThirdChangeUp, int skill);
#else
#ifdef NEW_SKILL_FORSKYLAND
	int  SkillGetRequireClass(int Cclass, int ChangeUP, int skill);
#else
	int  SkillGetRequireClass(int Cclass, int skill);
#endif
#endif

#ifdef NEW_FORSKYLAND2
	int SkillGetBP(int skill);	
#endif

	int	GetSkillAttr(int skill);

#ifdef DARKLORD_WORK
	int GetskillRequireLeadership(int skill);
	int	GetDelayTime(int skill);
#endif

#ifdef SKILL_ADD_PHYSICS_MAGIC_TYPE_20040812
	int GetSkillType(int iSkill);			// ��ų�� �Ӽ��� ����/���� ���� �ƴϸ� ������ Ȯ��
#endif

#ifdef ADD_NEW_SKILL_FOR_CASTLE_01_20041116
	// ��å�� �ʿ����� �˻�
	BOOL CheckStatus( INT iSkill, INT iGuildStatus );
	// Brand Of Skill �� �ʿ����� �˻�
	BOOL CheckBrandOfSkill( INT iSkill );
	// KillCount�� �´��� üũ ( 0 >= ųī��Ʈ�� �����. ������ ��ȯ, 0 < ųī��Ʈ�� ������. �������� ������ ǥ�� )
	INT CheckKillCount( INT iSkill, INT iKillCount );
#endif
	
#ifdef MODIFY_SKILL_CHECK_DISTANCE_20060413
	int GetSkillDistance(int skill);
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	INT GetRequireMLPoint( INT iSkill );		// ��ų��ȣ�� �̿��Ͽ� �ʿ� �����ͷ����� ��´�.
	INT GetRequireStrength( INT iSkill );		// ��ų��ȣ�� �̿��Ͽ� �ʿ����� ��´�.
	INT GetRequireDexterity( INT iSkill );		// ��ų��ȣ�� �̿��Ͽ� �ʿ��ø�� ��´�.
	INT GetBrandOfSkill( INT iSkill );			// ��ų��ȣ�� �̿��Ͽ� ���ེų ��ȣ�� ��´�.
	INT	GetSkillUseType( INT iSkill );			// ��ų��ȣ�� �̿��Ͽ� ���� ��� ������ ��´�.
	INT GetSkillLevel( INT iSkill );			// ��ų��ȣ�� �̿��Ͽ� ��ų ���� ������ ��´�.
	INT GetSkillGroup( INT iSkill );			// ��ų��ȣ�� �̿��Ͽ� ��ũ �׷� ������ ��´�.
	INT GetSkillRank( INT iSkill );				// ��ų��ȣ�� �̿��Ͽ� ��ũ ������ ��´�.
#endif
	
};

extern CMagicDamage	MagicDamageC;

#endif // !defined(AFX_MAGICDAMAGE_H__46975186_DBC4_428B_9854_61414B8B3BF4__INCLUDED_)
