// SkillHitBox.cpp: implementation of the CSkillHitBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillHitBox.h"

CSkillHitBox		SkillSpearHitBox;
CSkillHitBox		SkillElectricSparkHitBox;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillHitBox::CSkillHitBox()
{

}

CSkillHitBox::~CSkillHitBox()
{

}

void CSkillHitBox::Init()
{
	memset(m_Table, 0, sizeof(m_Table));
}

BOOL CSkillHitBox::Check(int dir,int x, int y)
{
	if( x < 0 || x >= SKILL_TABLE_X_SIZE || 
		y < 0 || y >= SKILL_TABLE_Y_SIZE || 
		dir < 0 || dir >= SKILL_TABLE_MAX_DIR)
		return FALSE;

	return m_Table[dir][y*SKILL_TABLE_X_SIZE+x];
}

BOOL CSkillHitBox::HitCheck(int dir, int x, int y, int tx, int ty)
{
	dir = dir/(360/SKILL_TABLE_MAX_DIR);
	if( dir < 0 || dir >= SKILL_TABLE_MAX_DIR )
		return FALSE;

	int targetx = tx - x;
	int targety = ty - y;

	targetx += SKILL_TABLE_CENTER_X;
	targety += SKILL_TABLE_CENTER_Y;

	if( targetx < 0 || abs(targetx) >= SKILL_TABLE_X_SIZE ||
		targety < 0 || abs(targety) >= SKILL_TABLE_Y_SIZE )
		return FALSE;

	//LogAddTD("%d, %d/%d, %d/%d, %d/%d %d", dir, x, y, tx, ty, targetx, targety, m_Table[dir][targety*SKILL_TABLE_Y_SIZE+targetx]);
	return	m_Table[dir][targety*SKILL_TABLE_Y_SIZE+targetx];
}

BOOL CSkillHitBox::Load(char* filename)
{
	Init();

	OFSTRUCT	ofst;
	HFILE hfile = OpenFile(filename, &ofst,  OF_READ);

	if( !hfile )
		return FALSE;

	DWORD filesize = GetFileSize((HANDLE)hfile, NULL);

	if( filesize != sizeof(m_Table) )
	{
		_lclose(hfile);
		return FALSE;
	}
	
	DWORD	readfilesize;
	if( !ReadFile((HANDLE)hfile, m_Table, filesize, &readfilesize, NULL) )
	{
		_lclose(hfile);
		return FALSE;
	}

	if( readfilesize != filesize )
	{
		_lclose(hfile);
		return FALSE;
	}
	_lclose(hfile);
	return TRUE;
}
