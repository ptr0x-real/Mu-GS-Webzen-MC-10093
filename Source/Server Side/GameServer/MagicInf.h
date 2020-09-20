// MagicInf.h: interface for the CMagicInf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICINF_H__3262D332_59D0_42C0_9C99_57C7BC827C81__INCLUDED_)
#define AFX_MAGICINF_H__3262D332_59D0_42C0_9C99_57C7BC827C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMagicInf  
{
public:
	BYTE	m_Level;			// ���� ����
#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// CMagicInf m_Skill �� ����
	int		m_Skill;			// ���� ����
#else
	BYTE	m_Skill;			// ���� ����
#endif
	int     m_DamageMin;
	int     m_DamageMax;

public:
	CMagicInf();
	virtual ~CMagicInf();

	BOOL IsMagic();
	void Init();
	void Clear();
	int  Set(BYTE aType, BYTE aIndex, BYTE aLevel);

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122	// CMagicInf::Set(...) Type ����
	int  Set(int aSkill, BYTE aLevel);
#else
	int  Set(BYTE aSkill, BYTE aLevel);
#endif

#ifdef MASTER_LEVEL_SKILL_SYSTEM_20071122
	int	 UpdateMasterSkill( int iSkill, BYTE btLevel );		// �����ͷ��� ��ų ���� �����ϱ�
#endif
	
	int  GetDamage();
};

extern int GetSkillNumberInex(int type, int Index, int level);

extern void MagicByteConvert(LPBYTE buf, CMagicInf Magici[], int maxmagic);
extern CMagicInf	DefMagicInf[];

#endif // !defined(AFX_MAGICINF_H__3262D332_59D0_42C0_9C99_57C7BC827C81__INCLUDED_)
