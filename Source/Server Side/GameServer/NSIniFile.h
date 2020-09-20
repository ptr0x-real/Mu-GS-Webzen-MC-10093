#if !defined(NS_INI_FILE_H)
#define NS_INI_FILE_H

#if (_MSC_VER > 1000)
#pragma once
#endif /* _MSC_VER > 1000 */

class CNSIniFile
{
public:
	CNSIniFile();
	// ----
	CNSIniFile(std::string _FileName, std::string _Section);
	// ----
	int getInt(std::string g_Key);
	// ----
	std::string getString(std::string g_Key);
	// ----
	bool changeSection(std::string _NewSection);
	// ----
	bool changeFile(std::string _NewFile);
private:
	std::string g_FileName;
	std::string g_Section;
};

#endif /* NS_INI_FILE_H */