#include "StdAfx.h"

CNSIniFile::CNSIniFile()
{

}

CNSIniFile::CNSIniFile(std::string _FileName, std::string _Section)
{
	this->changeFile(_FileName);
	this->changeSection(_Section);
}

int CNSIniFile::getInt(std::string g_Key)
{
	return GetPrivateProfileInt(this->g_Section.c_str(), g_Key.c_str(), 0, this->g_FileName.c_str());
}

std::string CNSIniFile::getString(std::string g_Key)
{
	char szTemp[1024] = {NULL};
	// ----
	GetPrivateProfileStringA(this->g_Section.c_str(), g_Key.c_str(), "", szTemp, sizeof(szTemp), this->g_FileName.c_str());
	// ----
	return std::string(szTemp);
}

bool CNSIniFile::changeSection(std::string _NewSection)
{
	this->g_Section = _NewSection;
	// ----
	return true;
}

bool CNSIniFile::changeFile(std::string _NewFile)
{
	this->g_FileName = _NewFile;
	// ----
	return true;
}