// DirPath.h: interface for the CDirPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRPATH_H__542CAD2B_73AA_479B_8D1E_5E970A4E8D65__INCLUDED_)
#define AFX_DIRPATH_H__542CAD2B_73AA_479B_8D1E_5E970A4E8D65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirPath  
{
public:
	CDirPath();
	virtual ~CDirPath();

	void SetFirstPath(char *dir_path);
	char* GetNewPath(char *npath);

	char m_path[MAX_PATH];
	char m_newpath[MAX_PATH];
};

#include <stdlib.h>



#endif // !defined(AFX_DIRPATH_H__542CAD2B_73AA_479B_8D1E_5E970A4E8D65__INCLUDED_)
