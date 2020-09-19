// HackToolBlock.h: interface for the CHackToolBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HACKTOOLBLOCK_H__55299A72_86E9_4B83_BBDB_640F7815D3C7__INCLUDED_)
#define AFX_HACKTOOLBLOCK_H__55299A72_86E9_4B83_BBDB_640F7815D3C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ADD_HACKING_TOOL_BLOCK_20090311

CONST int MAX_HACKTOOL_FILENAME_LENGTH	= 64;
CONST int MAX_HACKTOOL_LIST				= 10;
CONST int KEY_VALUE_NOT_USE				= 0xFFFFFFFF;
CONST int HACKTOOL_STATISTICS_TIME		= ( 60 * 1000 );
CONST int CONVERT_TIME_FAILED			= 0xFFFFFFFF;

enum
{
	HACKTOOL_STATISTICS_NOTUSE		= 0,
	HACKTOOL_STATISTICS_USE			= 1,
};

enum
{
	HACKTOOL_MODIFY_TYPE_NONE		= 0,
	HACKTOOL_MODIFY_TYPE_ADD		= 1,
	HACKTOOL_MODIFY_TYPE_MODIFY		= 2,
	HACKTOOL_MODIFY_TYPE_REMOVE		= 3,
};

#include "LargeRand.h"
#include "LogToFile.h"

class CHackToolData
{
public:
	CHackToolData();
	~CHackToolData();

	// 1. ��ŷ�� ������ ����
	// 1-1. ��ŷ�� ������ �ʱ�ȭ
	void Clear();
	// 1-2. ������� ������ ���� Ȯ��
	bool IsUsed();
	// 1-3. �̹� �ִ� ������ ���� Ȯ��
	bool IsMatched(char* pchFilename);
	// 1-4. ��ŷ���� �����͸� �Է½�Ų��.
	void SetData(char* pchFilename, DWORD dwKeyValue);
	void SetData(char* pchFilename, DWORD dwBlockTerm, int iBlockRate);
	void SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwBlockTerm, int iBlockRate);
	void SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// 1-5. ��ŷ���� Ű ���� ���Ѵ�.
	bool KeyValueVerify(DWORD iKey);
	// 1-6. ��ŷ���� ��� ���� ����� �����Ų��.
	void SetStatistics(LPOBJECTSTRUCT lpObj, BYTE btResult);
	// 1-7. ��ŷ���� ��� ��û ���� ����.
	void BeginStatistics();
	// 1-8. ��ŷ���� ��� �Ϸ� ���� Ȯ��
	bool CheckStatisticsFinish();
	// 1-9. ��� ������ �� Ȯ���� ���� ������ ����
	void ResetData();
	// 1-10. ���� �������� ���ο� ��ĳ�� ������ ���� ����
	void ResetSchedule();

	// 2. ��ŷ�� ���̵� ���� ����
	// 2-1. ��ŷ ������� ���̵� ���� ���θ� Ȯ���Ѵ�.
	bool IsBlocked();

	// 3. ��Ÿ
	// �ش� ��ŷ�� ����� Ȱ��ȭ �������� Ȯ���Ѵ�.
	bool CheckBlockActive();

public:
	bool	bIsUsed;

	DWORD	dwActiveTime;										// �ش� ��ŷ���� ������ �����ϴ� �ð�
	DWORD	dwKey;												// ��� �� ��ŷ���� ���� Ű �� (��� �� �����ϰ� �����ȴ�.)
	char	chHackToolName[MAX_HACKTOOL_FILENAME_LENGTH];
	int		iHacktoolUserCount;

	// ���� ����� ��� ����
	bool	bBlockHacktoolUser;
	int		iMapmoveBlockRate;
	DWORD	dwMapmoveBlockTerm;

	// ���� ����� ��� ����
	bool	bBeginStatistics;
	DWORD	dwStatisticsBeginTick;

	// ���� ��� �ð� ����
	CTime	tBlockFinishTime;
};

class CHackToolBlock  
{
public:
	CHackToolBlock();
	virtual ~CHackToolBlock();

	// 1. ��ŷ�� ������ ����
	// 1-0. ��ŷ�� ���� ���� �ʱ�ȭ
	void Initialize();
	// 1-1. ��ŷ�� �����͸� �ʱ�ȭ
	void Clear();
	// 1-2. ��ŷ�� ���� �ɼ� �ε�
	bool LoadOption(char* pchOptionFilename);
	// 1-3. ��ŷ�� �����͸� �߰�
	bool AddHacktoolData(char* pchFilename, DWORD dwBlockTerm = 0, int iBlockRate = 0);
	bool AddHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm = 0, int iBlockRate = 0);
	// 1-4. ��ŷ�� �����͸� ����
	bool RemoveHacktoolData(char* pchFilename); 
	// 1-5. ��ŷ�� ������ ����
	bool ModifyHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// 1-6. �ش� Ű ���� ������ ��ŷ ���� ���翩�� Ȯ��
	bool KeyValueVerify(DWORD iKey);
	// 1-7. �ش� �̸��� ���� ��ŷ �� �����͸� ã�´�.
	CHackToolData*	SearchData(char* pchFilename);
	CHackToolData*	SearchData(DWORD dwKeyValue);

	// 2. ��ŷ ������ ���� ó�� ����
	// 2-1. ��ŷ�� ��� ������ ���� ó��
	bool IsHackedUser(LPOBJECTSTRUCT lpObj);
	// 2-2. ��ŷ�� ��� ���� ����� ���� ó��
	void SetHacktoolStatistics(LPOBJECTSTRUCT lpObj, DWORD dwKey, BYTE btResult);

	// 3. ��ŷ ���� ��� ���� ����
	// 3-1. ��ü ����ڿ� ���� ��ŷ �� ����� ��� ��û
	void HacktoolStatisticsAll(char* pchFilename);
	// 3-2. Ư�� ����ڿ� ���� ��ŷ �� ����� ��� ��û
	void HacktoolStatistics(LPOBJECTSTRUCT lpObj, char* pchFilename);
	// 3-3. ��� ��û�� �� ��ŷ���� ��� ��� �ջ��� �������� Ȯ���Ѵ�.
	bool CheckStatisticsResult();
	// 3-4. �ű� �������� ������� ��� ���� �� ��ŷ���� ��� ���θ� Ȯ���Ѵ�.
	void HacktoolStatisticsNewConnect(LPOBJECTSTRUCT lpObj);

	// 4. ��Ÿ
	// �� ��ŷ���� ������ Ű ���� �����Ѵ�.
	DWORD _MakeKeyValue();
	// ��ũ�η� �Է� �� �� ���� �ð��� DWORD ������ �����Ͽ� �ش�.
	DWORD MakeDateToDword(char* pchDateString);
	// �Էµ� ��ŷ �����Ͱ� ���� ������ �ð����� Ȯ���Ѵ�.
	void CheckBlockActive();
	// ������ ����� GM�� �����Ѵ�.
	bool SetGMObject(LPOBJECTSTRUCT lpObj);
	// ������ ����� GM�� ������Ʈ�� ���´�.
	LPOBJECTSTRUCT GetGMObject();
	// �ش� ����ڰ� GM���� Ȯ���Ѵ�.
	bool IsGMUser(LPOBJECTSTRUCT lpObj);
	// GM���� ���� �޽����� ������.
	void SendNoticeToGM(char* pchMessage, ... );
	// GM���� ä�� �޽����� ������.
	void SendChatMessageToGM(char* pchMessage, ... );
	
	// 5. �����αװ���
	void AddLog(char* pchLog, ... );
	
public:
	bool	bRequestAntiHackStatistics;				// ���� ������ ���� ���� ������� ��� Ȯ���� ��û ����
	
	int		iCurrentUserCount;						// ���Ȯ�ν� ������ ��
	int		iHackToolUseCount;						// ������ ������� ������ ��
	
	LPOBJECTSTRUCT	lpGMObject;						// ���� ��� �����͸� ��û�� GM�� ������Ʈ
	
	CHackToolData HackToolData[MAX_HACKTOOL_LIST];
	
	CLogToFile*	lpHacktoolLog;
};

extern CHackToolBlock g_HackToolBlock;

#endif // ADD_HACKING_TOOL_BLOCK_20090311

#endif // !defined(AFX_HACKTOOLBLOCK_H__55299A72_86E9_4B83_BBDB_640F7815D3C7__INCLUDED_)
