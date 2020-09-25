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

void CDirPath::SetFirstPath(char *dir_path)		// 절대경로 지정
{
	strcpy(m_path, dir_path);
}


char* CDirPath::GetNewPath(const char *npath)			// 절대경로를 기준으로 상대경로 지정
{
	strcpy(m_newpath, m_path);
	strcat(m_newpath, npath);
	return m_newpath;
}
