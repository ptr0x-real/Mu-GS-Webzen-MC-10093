// MonsterAttr.h: interface for the CMonsterAttr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERATTR_H__D5756E8D_828C_44CA_9C77_568100DFAC1C__INCLUDED_)
#define AFX_MONSTERATTR_H__D5756E8D_828C_44CA_9C77_568100DFAC1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef ENHANCE_MONSTER_TYPE_TO_WORD_20040618
#define MAX_MONSTERATTRIBUTE	512		// ���� 65535 ������ ���߿� �ʿ��� ��쿡 Ȯ��
#else
#define MAX_MONSTERATTRIBUTE	255
#endif 

typedef struct
{
	int  m_Index;
	int  m_Rate;
	char m_Name[20];
	int  m_Level;

#ifdef ADD_NEW_MONSTER_SYSTEM_02_20050725
	int	 m_iAINumber;
#endif

#ifdef MODIFY_CRYWOLF_BENEFIT_01_20060315
	int	 m_iScriptHP;			// Monster.txt�� ��ũ���� �� ���� HP
#endif 
	
	int  m_Hp;
	int  m_Mp;
	int  m_DamageMin;
	int  m_DamageMax;
	int  m_Defense;
	int  m_MagicDefense;
	int  m_AttackRating;
	int  m_Successfulblocking;
	int  m_MoveRange;			// �̵�����
	int  m_AttackRange;			// ���ݹ���	
	int	 m_AttackType;			// ��������
	int  m_ViewRange;			// ���� �����ȿ��� ã���� ����ϴ� ����
	int  m_MoveSpeed;
	int  m_AttackSpeed;
	int  m_RegenTime;
	int  m_Attribute;			// ���� �Ӽ�
	int  m_ItemRate;
	int  m_MoneyRate;
	int  m_MaxItemLevel;		// ����Ʈ���� ������ ���� �Ѱ踦 �ش�.
	BYTE m_Resistance[MAX_RESISTANCE];
	
#ifdef MONSTER_SKILL
	DWORD	m_MonsterSkill;
#endif

} MONSTER_ATTRIBUTE, *LPMONSTER_ATTRIBUTE;

#ifdef MONSTER_SKILL
// ���� ��ų
#define MONSKILL_REDUCE_ATTACKDAMAGE	0x01	// ���ݷ� ���� 20%
#define MONSKILL_REDUCE_MAGIC			0x02	// �����ɷ� ����
#endif

class CMonsterAttr  
{
public :
	MONSTER_ATTRIBUTE m_MonsterAttr[MAX_MONSTERATTRIBUTE];

public:
	LPMONSTER_ATTRIBUTE GetAttr(int aClass);
#ifdef GMCOMMAND_EX
	LPMONSTER_ATTRIBUTE GetAttr(char* lpszName);
#endif // GMCOMMAND_EX

#ifdef SCRIPT_DECODE_WORK
	void LoadAttrDecode(char *filename);
#endif
	
	void LoadAttr(char *filename);

#ifdef AUTH_GAMESERVER
	void LoadAttr(char *Buffer, int iSize);
#endif

	
	CMonsterAttr();
	virtual ~CMonsterAttr();

};

#endif // !defined(AFX_MONSTERATTR_H__D5756E8D_828C_44CA_9C77_568100DFAC1C__INCLUDED_)
