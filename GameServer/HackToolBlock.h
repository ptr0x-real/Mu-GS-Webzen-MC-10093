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

	// 1. 해킹툴 데이터 관련
	// 1-1. 해킹툴 데이터 초기화
	void Clear();
	// 1-2. 사용중인 데이터 인지 확인
	bool IsUsed();
	// 1-3. 이미 있는 데이터 인지 확인
	bool IsMatched(char* pchFilename);
	// 1-4. 해킹툴의 데이터를 입력시킨다.
	void SetData(char* pchFilename, DWORD dwKeyValue);
	void SetData(char* pchFilename, DWORD dwBlockTerm, int iBlockRate);
	void SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwBlockTerm, int iBlockRate);
	void SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// 1-5. 해킹툴의 키 값을 비교한다.
	bool KeyValueVerify(DWORD iKey);
	// 1-6. 해킹툴의 사용 여부 결과를 적용시킨다.
	void SetStatistics(LPOBJECTSTRUCT lpObj, BYTE btResult);
	// 1-7. 해킹툴의 통계 요청 적용 시작.
	void BeginStatistics();
	// 1-8. 해킹툴의 통계 완료 여부 확인
	bool CheckStatisticsFinish();
	// 1-9. 통계 데이터 재 확인을 위한 데이터 리셋
	void ResetData();
	// 1-10. 기존 데이터의 새로운 스캐쥴 적용을 위한 리셋
	void ResetSchedule();

	// 2. 해킹툴 맵이동 제한 관련
	// 2-1. 해킹 사용자의 맵이동 제한 여부를 확인한다.
	bool IsBlocked();

	// 3. 기타
	// 해당 해킹툴 블록이 활성화 가능한지 확인한다.
	bool CheckBlockActive();

public:
	bool	bIsUsed;

	DWORD	dwActiveTime;										// 해당 해킹툴이 동작을 시작하는 시간
	DWORD	dwKey;												// 등록 된 해킹툴의 고유 키 값 (등록 시 렌덤하게 생성된다.)
	char	chHackToolName[MAX_HACKTOOL_FILENAME_LENGTH];
	int		iHacktoolUserCount;

	// 핵툴 사용자 블록 관련
	bool	bBlockHacktoolUser;
	int		iMapmoveBlockRate;
	DWORD	dwMapmoveBlockTerm;

	// 핵툴 사용자 통계 관련
	bool	bBeginStatistics;
	DWORD	dwStatisticsBeginTick;

	// 핵툴 블록 시간 관련
	CTime	tBlockFinishTime;
};

class CHackToolBlock  
{
public:
	CHackToolBlock();
	virtual ~CHackToolBlock();

	// 1. 해킹툴 데이터 관련
	// 1-0. 해킹툴 관련 설정 초기화
	void Initialize();
	// 1-1. 해킹툴 데이터를 초기화
	void Clear();
	// 1-2. 해킹툴 관련 옵션 로드
	bool LoadOption(char* pchOptionFilename);
	// 1-3. 해킹툴 데이터를 추가
	bool AddHacktoolData(char* pchFilename, DWORD dwBlockTerm = 0, int iBlockRate = 0);
	bool AddHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm = 0, int iBlockRate = 0);
	// 1-4. 해킹툴 데이터를 삭제
	bool RemoveHacktoolData(char* pchFilename); 
	// 1-5. 해킹툴 데이터 변경
	bool ModifyHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// 1-6. 해당 키 값을 가지는 해킹 툴의 존재여부 확인
	bool KeyValueVerify(DWORD iKey);
	// 1-7. 해당 이름을 갖는 해킹 툴 데이터를 찾는다.
	CHackToolData*	SearchData(char* pchFilename);
	CHackToolData*	SearchData(DWORD dwKeyValue);

	// 2. 해킹 유저에 대한 처리 관련
	// 2-1. 해킹툴 사용 유저에 대한 처리
	bool IsHackedUser(LPOBJECTSTRUCT lpObj);
	// 2-2. 해킹툴 사용 여부 결과에 대한 처리
	void SetHacktoolStatistics(LPOBJECTSTRUCT lpObj, DWORD dwKey, BYTE btResult);

	// 3. 해킹 유저 통계 집계 관련
	// 3-1. 전체 사용자에 대한 해킹 툴 사용자 통계 요청
	void HacktoolStatisticsAll(char* pchFilename);
	// 3-2. 특정 사용자에 대한 해킹 툴 사용자 통계 요청
	void HacktoolStatistics(LPOBJECTSTRUCT lpObj, char* pchFilename);
	// 3-3. 통계 요청을 한 해킹툴의 통계 결과 합산이 가능한지 확인한다.
	bool CheckStatisticsResult();
	// 3-4. 신규 유저에게 사용중인 블록 설정 된 해킹툴의 사용 여부를 확인한다.
	void HacktoolStatisticsNewConnect(LPOBJECTSTRUCT lpObj);

	// 4. 기타
	// 각 해킹툴이 가지는 키 값을 생성한다.
	DWORD _MakeKeyValue();
	// 매크로로 입력 된 블럭 시작 시간을 DWORD 값으로 변경하여 준다.
	DWORD MakeDateToDword(char* pchDateString);
	// 입력된 해킹 데이터가 동작 가능한 시간인지 확인한다.
	void CheckBlockActive();
	// 핵툴을 등록한 GM을 저장한다.
	bool SetGMObject(LPOBJECTSTRUCT lpObj);
	// 핵툴을 등록한 GM의 오브젝트를 얻어온다.
	LPOBJECTSTRUCT GetGMObject();
	// 해당 사용자가 GM인지 확인한다.
	bool IsGMUser(LPOBJECTSTRUCT lpObj);
	// GM에게 공지 메시지를 보낸다.
	void SendNoticeToGM(char* pchMessage, ... );
	// GM에게 채팅 메시지를 보낸다.
	void SendChatMessageToGM(char* pchMessage, ... );
	
	// 5. 핵툴로그관련
	void AddLog(char* pchLog, ... );
	
public:
	bool	bRequestAntiHackStatistics;				// 인증 서버로 부터 핵툴 사용자의 통계 확인을 요청 받음
	
	int		iCurrentUserCount;						// 통계확인시 접속자 수
	int		iHackToolUseCount;						// 핵툴을 사용중인 접속자 수
	
	LPOBJECTSTRUCT	lpGMObject;						// 핵툴 통계 데이터를 요청한 GM의 오브젝트
	
	CHackToolData HackToolData[MAX_HACKTOOL_LIST];
	
	CLogToFile*	lpHacktoolLog;
};

extern CHackToolBlock g_HackToolBlock;

#endif // ADD_HACKING_TOOL_BLOCK_20090311

#endif // !defined(AFX_HACKTOOLBLOCK_H__55299A72_86E9_4B83_BBDB_640F7815D3C7__INCLUDED_)
