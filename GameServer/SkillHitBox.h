// SkillHitBox.h: interface for the CSkillHitBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLHITBOX_H__AE489B08_6B43_4229_BA35_EF3D26E2D824__INCLUDED_)
#define AFX_SKILLHITBOX_H__AE489B08_6B43_4229_BA35_EF3D26E2D824__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define		SKILL_TABLE_X_SIZE	21
#define		SKILL_TABLE_Y_SIZE	21

#define		SKILL_TABLE_CENTER_X	(SKILL_TABLE_X_SIZE/2)
#define		SKILL_TABLE_CENTER_Y	(SKILL_TABLE_Y_SIZE/2)

#define		SKILL_TABLE_MAX_DIR	36

class CSkillHitBox  
{
	BYTE	m_Table[SKILL_TABLE_MAX_DIR][SKILL_TABLE_X_SIZE*SKILL_TABLE_Y_SIZE];
public:
	CSkillHitBox();
	virtual ~CSkillHitBox();

	void	Init();
	BOOL	Check(int dir,int x, int y);
	BOOL	HitCheck(int dir, int x, int y, int tx, int ty);
	
	BOOL	Load(char* filename);
};

extern CSkillHitBox		SkillSpearHitBox;
extern CSkillHitBox		SkillElectricSparkHitBox;

#endif // !defined(AFX_SKILLHITBOX_H__AE489B08_6B43_4229_BA35_EF3D26E2D824__INCLUDED_)
