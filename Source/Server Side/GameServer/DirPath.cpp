// DirPath.cpp: implementation of the CDirPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirPath.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirPath::CDirPath()
{

}

CDirPath::~CDirPath()
{

}

void CDirPath::SetFirstPath(char *dir_path)		// ������ ����
{
	strcpy(m_path, dir_path);
}


char* CDirPath::GetNewPath(const char *npath)			// �����θ� �������� ����� ����
{
	strcpy(m_newpath, m_path);
	strcat(m_newpath, npath);
	return m_newpath;
}
