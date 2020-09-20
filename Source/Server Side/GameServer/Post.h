#ifndef __POST_H__
#define __POST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SETTINGOK	 true
#define SETTINGERROR false
#define CHECKOK	   "OK"
#define CHECKERROR "ERROR"

enum
{
	POST_DISABLED = 0,
	POST_LEVELERROR = 1,
	POST_MONEYERROR = 2,
	POST_ENABLED = 3,
};

class CPostSystem
{
public:
	CPostSystem();

	void Load(const char *filename);

	void SendPostMessage(int aIndex,char* message);

private:
	BOOL m_bPostEnabled;
	int m_iPostMoney;
	int m_iPostLevel;

protected:
	bool CheckSetting();

	int GetState();

	void SendErrorMsg(int aIndex);

	bool CheckUser(int aIndex);
};

extern CPostSystem g_PostSystem;

#endif