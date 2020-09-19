// HackToolBlock.cpp: implementation of the CHackToolBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HackToolBlock.h"

#ifdef ADD_HACKING_TOOL_BLOCK_20090311

#include "..\Common\winutil.h"
#include "Sprotocol.h"

CHackToolBlock g_HackToolBlock;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHackToolData::CHackToolData()
{
	Clear();
}

CHackToolData::~CHackToolData()
{
}

// 1. 해킹툴 데이터 관련
// 1-1. 해킹툴 데이터 초기화
void CHackToolData::Clear()
{
	bIsUsed				= false;
	dwKey				= KEY_VALUE_NOT_USE;
	iHacktoolUserCount	= 0;
	bBlockHacktoolUser	= false;
	iMapmoveBlockRate	= 0;
	chHackToolName[0]	= '\0';
	bBeginStatistics	= false;
	dwStatisticsBeginTick = 0;
}

// 1-2. 사용중인 데이터 인지 확인
bool CHackToolData::IsUsed()
{
	return bIsUsed;
}

// 1-3. 이미 있는 데이터 인지 확인
bool CHackToolData::IsMatched(char* pchFilename)
{
	if( strcmp( chHackToolName, pchFilename ) == 0 )
	{
		return true;
	}

	return false;
}

// 1-4. 해킹툴의 데이터를 입력시킨다.
void CHackToolData::SetData(char* pchFilename, DWORD dwKeyValue)
{
	if( pchFilename == NULL )
	{
		return;
	}

	bIsUsed = true;

	memcpy( chHackToolName, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	dwKey = dwKeyValue;
}

void CHackToolData::SetData(char* pchFilename, DWORD dwBlockTerm, int iBlockRate)
{
	if( pchFilename == NULL )
	{
		return;
	}

	bIsUsed = true;

	memcpy( chHackToolName, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	dwMapmoveBlockTerm = dwBlockTerm;
	iMapmoveBlockRate = iBlockRate;
	if( iBlockRate > 0 )
	{
		bBlockHacktoolUser = true;
	}
	else
	{
		bBlockHacktoolUser = false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tBlockTerm( 0, 0, dwBlockTerm, 0 );
	tBlockFinishTime = tCurrentTime + tBlockTerm;
}

void CHackToolData::SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwBlockTerm, int iBlockRate)
{
	if( pchFilename == NULL )
	{
		return;
	}

	bIsUsed = true;
	
	memcpy( chHackToolName, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	dwKey = dwKeyValue;
	dwMapmoveBlockTerm = dwBlockTerm;
	iMapmoveBlockRate = iBlockRate;
	if( iBlockRate > 0 )
	{
		bBlockHacktoolUser = true;
	}
	else
	{
		bBlockHacktoolUser = false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tBlockTerm( 0, 0, dwBlockTerm, 0 );
	tBlockFinishTime = tCurrentTime + tBlockTerm;
}

void CHackToolData::SetData(char* pchFilename, DWORD dwKeyValue, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate)
{
	if( pchFilename == NULL )
	{
		return;
	}
	
	bIsUsed = true;
	
	memcpy( chHackToolName, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	this->dwKey = dwKeyValue;
	this->dwActiveTime = dwActiveTime;
	this->dwMapmoveBlockTerm = dwBlockTerm;
	this->iMapmoveBlockRate = iBlockRate;
	if( iBlockRate > 0 )
	{
		bBlockHacktoolUser = true;
	}
	else
	{
		bBlockHacktoolUser = false;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tBlockTerm( 0, 0, dwBlockTerm, 0 );
	tBlockFinishTime = tCurrentTime + tBlockTerm;
}

// 1-5. 해킹툴의 키 값을 비교한다.
bool CHackToolData::KeyValueVerify(DWORD dwKey)
{
	if( this->dwKey == dwKey )
	{
		return true;
	}

	return false;
}

// 1-6. 해킹툴의 사용 여부 결과를 적용시킨다.
void CHackToolData::SetStatistics(LPOBJECTSTRUCT lpObj, BYTE btResult)
{
	// 0 : 사용 중이지 않음 , 1 : 사용 중
	if( btResult == HACKTOOL_STATISTICS_USE )
	{
		iHacktoolUserCount++;
		// 해당 유저의 사용중인 해킹 툴 데이터를 추가하여 준다.
		for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
		{
			if( lpObj->m_dwUseHacktoolList[i] == KEY_VALUE_NOT_USE )
			{
				lpObj->m_dwUseHacktoolList[i] = dwKey;
				break;
			}
		}
	}
}

// 1-7. 해킹툴의 통계 요청 적용 시작.
void CHackToolData::BeginStatistics()
{
	// 해킹툴의 통계 요청시의 틱 카운트를 얻어온다.
	// 통계 시작 1분뒤에 통계 처리를 완료한다.
	bBeginStatistics = true;
	dwStatisticsBeginTick = GetTickCount();
}

// 1-8. 해킹툴의 통계 완료 여부 확인
bool CHackToolData::CheckStatisticsFinish()
{
	if( bBeginStatistics == false || dwStatisticsBeginTick == 0 )
	{
		return false;
	}

	if( ( dwStatisticsBeginTick + HACKTOOL_STATISTICS_TIME ) <= GetTickCount() )
	{
		// log
		dwStatisticsBeginTick = 0;
		bBeginStatistics = false;
		return true;
	}

	return false;
}

// 1-9. 통계 데이터 재 확인을 위한 데이터 리셋
void CHackToolData::ResetData()
{
	iHacktoolUserCount = 0;
	bBeginStatistics = false;
	dwStatisticsBeginTick = 0;
}

// 1-10. 기존 데이터의 새로운 스캐쥴 적용을 위한 리셋
void CHackToolData::ResetSchedule()
{
	dwActiveTime = 0;
	dwMapmoveBlockTerm = 0;
	dwStatisticsBeginTick = 0;
	iMapmoveBlockRate = 0;
}

// 2. 해킹툴 맵이동 제한 관련
// 2-1. 해킹 사용자의 맵이동 제한 여부를 확인한다.
bool CHackToolData::IsBlocked()
{
	int iRandomValue = 0;

	if( bBlockHacktoolUser == true )
	{
		iRandomValue = rand() % 100;
		if( iRandomValue <= iMapmoveBlockRate )
		{
			return true;
		}
	}

	return false;
}

// 3. 기타
// 해당 해킹툴 블록이 활성화 가능한지 확인한다.
bool CHackToolData::CheckBlockActive()
{
	if( bIsUsed == false || iMapmoveBlockRate <= 0 )
	{
		return false;
	}

	// 입력된 시간과 현재 시간을 비교한다.
	time_t tTime = dwActiveTime;

	CTime tActiveTime( tTime );
	CTime tCurrentTime = CTime::GetCurrentTime();

	// 실행 시간이 지난 상태라면 활성화 시킨다.
	if( bBlockHacktoolUser == false )
	{
		if( tActiveTime <= tCurrentTime )
		{
			bBlockHacktoolUser = true;

			return true;
		}
	}

	// 블록을 종료해야 될 시기가 되었는지 확인한다.
	if( bBlockHacktoolUser == true )
	{
		if( tBlockFinishTime <= tCurrentTime )
		{
			bBlockHacktoolUser = false;
			bIsUsed = false;

			return true;
		}
	}

	return false;
}

CHackToolBlock::CHackToolBlock()
{
	Clear();
}

CHackToolBlock::~CHackToolBlock()
{

}

// 1. 해킹툴 데이터 관련
// 1-0. 해킹툴 관련 설정 초기화
void CHackToolBlock::Initialize()
{
	// 해킹툴 관련 로그를 저장 할 폴더를 생성
	lpHacktoolLog = new CLogToFile( "ANTIHACK", ".\\ANTIHACK_LOG");		// 로그파일 객체
}

// 1-1. 해킹툴 데이터를 초기화
void CHackToolBlock::Clear()
{
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		HackToolData[i].Clear();
	}

	bRequestAntiHackStatistics = false;
	
	iCurrentUserCount = 0;
	iHackToolUseCount = 0;

	lpGMObject			= 0;
	lpHacktoolLog = NULL;
}

// 1-2. 해킹툴 관련 옵션 로드
bool CHackToolBlock::LoadOption(char* pchOptionFilename)
{
	// 해킹 툴 관련 셋팅을 서버 재시작시 마다 설정하지 않기 위한 옵션 파일 생성
	// 'HacktoolOption.dat' 파일
	return true;
}

// 1-3. 해킹툴 데이터를 추가
bool CHackToolBlock::AddHacktoolData(char* pchFilename, DWORD dwBlockTerm, int iBlockRate)
{
	DWORD dwKeyValue = 0;
	CHackToolData* lpHacktoolData = NULL;

	AddLog( "[ANTI-HACK] Add Hacktool block data ( Name : %s, BlockTerm : %d, BlockRate : %d )",
		pchFilename,
		dwKeyValue,
		iBlockRate );

	// 똑같은 데이터가 존재한다.
	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// 같은 데이터가 존재하면 이미 존재하는 데이터는 초기화 하고 일정을 초기화 한다.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetSchedule();

		// 해킹툴의 데이터를 입력시킨다.
		lpHacktoolData->SetData( pchFilename, dwBlockTerm, iBlockRate );
				
		return true;
	}
	else
	{
		for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
		{
			if( HackToolData[i].IsUsed() == false )
			{
				// 해킹툴의 키 값을 생성한다.
				dwKeyValue = _MakeKeyValue();
				if( dwKeyValue == KEY_VALUE_NOT_USE )
				{
					// 키 값 생성에 실패
					return false;
				}
				// 해킹툴의 데이터를 입력시킨다.
				HackToolData[i].SetData( pchFilename, dwKeyValue, dwBlockTerm, iBlockRate );
				
				break;;
			}
		}

		// 이미 존재하는 데이터가 없다면 핵툴 사용 여부를 확인하기 위해 핵툴 사용 통계를 수집한다.
		HacktoolStatisticsAll( pchFilename );

		return true;
	}

	// 남은 슬롯이 없다.
	return false;
}

bool CHackToolBlock::AddHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate)
{
	DWORD dwKeyValue = 0;
	
	CHackToolData* lpHacktoolData = NULL;

	AddLog( "[ANTI-HACK] Add Hacktool block data ( Name : %s, ActiveTime : %d, BlockTerm : %d, BlockRate : %d )",
		pchFilename,
		dwActiveTime,
		dwBlockTerm,
		iBlockRate );

	// 똑같은 데이터가 존재한다.
	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// 같은 데이터가 존재하면 이미 존재하는 데이터는 초기화 하고 일정을 초기화 한다.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetSchedule();

		// 해킹툴의 데이터를 입력시킨다.
		lpHacktoolData->SetData( pchFilename, dwKeyValue, dwActiveTime, dwBlockTerm, iBlockRate );

		return true;
	}
	else
	{
		for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
		{
			if( HackToolData[i].IsUsed() == false )
			{
				// 해킹툴의 키 값을 생성한다.
				dwKeyValue = _MakeKeyValue();
				if( dwKeyValue == KEY_VALUE_NOT_USE )
				{
					// 키 값 생성에 실패
					return false;
				}
				// 해킹툴의 데이터를 입력시킨다.
				HackToolData[i].SetData( pchFilename, dwKeyValue, dwActiveTime, dwBlockTerm, iBlockRate );

				break;
			}
		}

		// 이미 존재하는 데이터가 없다면 핵툴 사용 여부를 확인하기 위해 핵툴 사용 통계를 수집한다.
		HacktoolStatisticsAll( pchFilename );

		return true;
	}
	
	// 남은 슬롯이 없다.
	return false;
}

// 1-4. 해킹툴 데이터를 삭제
bool CHackToolBlock::RemoveHacktoolData(char* pchFilename)
{
	// 해킹툴 관련 데이터는 모두 인증서버에서 받기 때문에 추가로 관리해 줘야 할 항목은 없다.
	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData == NULL )
	{
		return false;
	}

	AddLog( "[ANTI-HACK] Remove Hacktool block data. ( Name : %s )",
			pchFilename );

	lpHacktoolData->Clear();

	return true;
}

// 1-5. 해킹툴 데이터 변경
bool CHackToolBlock::ModifyHacktoolData(char* pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate)
{
	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData == NULL )
	{
		return false;
	}

	AddLog( "[ANTI-HACK] Modify Hacktool block data. ( Name : %s, ActiveTime : %d, BlockTerm : %d, Rate : %d )",
		pchFilename,
		dwActiveTime,
		dwBlockTerm,
		iBlockRate );

	lpHacktoolData->SetData( pchFilename, lpHacktoolData->dwKey, dwActiveTime, dwBlockTerm, iBlockRate );

	return true;
}

// 1-6. 해당 키 값을 가지는 해킹 툴의 존재여부 확인
bool CHackToolBlock::KeyValueVerify(DWORD dwKey)
{
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].IsUsed() == true && HackToolData[i].KeyValueVerify( dwKey ) == true )
		{
			return true;
		}
	}

	return false;
}

// 1-7. 해당 이름을 갖는 해킹 툴 데이터를 찾는다.
CHackToolData* CHackToolBlock::SearchData(char* pchFilename)
{
	CHackToolData* lpReturn = NULL;

	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].IsMatched( pchFilename ) == true )
		{
			lpReturn = &HackToolData[i];
			break;
		}
	}

	return lpReturn;
}

CHackToolData* CHackToolBlock::SearchData(DWORD dwKeyValue)
{
	CHackToolData* lpReturn = NULL;

	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].dwKey == dwKeyValue )
		{
			lpReturn = &HackToolData[i];
			break;
		}
	}

	return lpReturn;
}

// 각 해킹툴이 가지는 키 값을 생성한다.
DWORD CHackToolBlock::_MakeKeyValue()
{
	DWORD dwKeyValue = 0;

	// 10번 시도 후에도 중복되는 값이 나온다면 실패로 간주한다.
	for( int i = 0 ;  i < 10 ; i++ )
	{
		dwKeyValue = GetLargeRand();
		if( KeyValueVerify( dwKeyValue ) == false )
		{
			return dwKeyValue;
		}
	}

	return KEY_VALUE_NOT_USE;
}

// 매크로로 입력 된 블럭 시작 시간을 DWORD 값으로 변경하여 준다.
DWORD CHackToolBlock::MakeDateToDword(char* pchDateString)
{
	DWORD dwReturnValue = 0;
	char chDateString[20];
	char chToken[] = "-: ";
	char* pchTokenized = NULL;
	int iDateSlot[6];
	int iSlotIndex = 0;
	
	memcpy( chDateString, pchDateString, 20 );
	
	pchTokenized = strtok( chDateString, chToken );
	while( pchTokenized != NULL )
	{
		iDateSlot[iSlotIndex] = atoi( pchTokenized );
		iSlotIndex++;
		pchTokenized = strtok( NULL, chToken );
	}
	
	CTime tSetTime( iDateSlot[0], iDateSlot[1], iDateSlot[2], iDateSlot[3], iDateSlot[4], iDateSlot[5] );
	dwReturnValue = tSetTime.GetTime();
	
	return dwReturnValue;
}

// 입력된 해킹 데이터가 동작 가능한 시간인지 확인한다.
void CHackToolBlock::CheckBlockActive()
{
	LPOBJECTSTRUCT lpObj = NULL;
	// 전체 리스트를 검사하여 핵툴 블록을 시작할 데이터를 검사한다.
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].CheckBlockActive() == true )
		{
			CTime tActiveTime( HackToolData[i].dwActiveTime );
			CString strActiveTime;

			//tActiveTime.Format( "

			AddLog( "[ANTI-HACK] Change Hacktool Mapmove Block State. ( State : %s, Name : %s, ActiveTime : %d, BlockTerm : %d, UserCount : %d )",
				HackToolData[i].bBlockHacktoolUser ? "Block" : "Unblock",
				HackToolData[i].chHackToolName,
				HackToolData[i].dwActiveTime,
				HackToolData[i].dwMapmoveBlockTerm,
				HackToolData[i].iHacktoolUserCount );

			lpObj = GetGMObject();
			if( lpObj != NULL )
			{
				PMSG_NOTICE	pNotice;
				CHAR szNotice[256] = {0,};
				TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);
				wsprintf( szNotice, "[ANTI-HACK] Change Auto Program State. State : %s, Name : %s, ActiveTime : %d, Term : %d, UserCount : %d",
					HackToolData[i].bBlockHacktoolUser ? "Block" : "Unblock",
					HackToolData[i].chHackToolName,
					HackToolData[i].dwActiveTime,
					HackToolData[i].dwMapmoveBlockTerm,
					HackToolData[i].iHacktoolUserCount );
				TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, szNotice );
				TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );	
			}
		}
	}
}

// 핵툴을 등록한 GM을 저장한다.
bool CHackToolBlock::SetGMObject(LPOBJECTSTRUCT lpObj)
{
	if( IsGMUser( lpObj ) == true )
	{
		lpGMObject = lpObj;
	}

	return false;
}

// 핵툴을 등록한 GM의 오브젝트를 얻어온다.
LPOBJECTSTRUCT CHackToolBlock::GetGMObject()
{
	if( IsGMUser( lpGMObject ) == true )
	{
		return lpGMObject;
	}
		
	return NULL;
}

// 해당 사용자가 GM인지 확인한다.
bool CHackToolBlock::IsGMUser(LPOBJECTSTRUCT lpObj)
{
	if( lpObj == NULL )
	{
		return false;
	}
	
	if( lpObj->Connected < CSS_GAMEPLAYING || lpObj->Type != OBJTYPE_CHARACTER )
	{
		return false;
	}
	
	if( ( ( lpObj->Authority & AUTHORITY_ADMIN ) == AUTHORITY_ADMIN ) || ( ( lpObj->Authority & AUTHORITY_SUPER_ADMIN ) == AUTHORITY_SUPER_ADMIN ) )
	{
		return true;
	}

	return false;
}

// GM에게 공지 메시지를 보낸다.
void CHackToolBlock::SendNoticeToGM(char* pchMessage, ... )
{
	va_list argptr;
	char chMessageBuffer[256] = {0,};
	LPOBJECTSTRUCT lpObj = NULL;

	lpObj = GetGMObject();
	if( lpObj == NULL )
	{
		// 현재 서버에 GM이 없다.
		return;
	}
	
	va_start( argptr, pchMessage );
		vsprintf( chMessageBuffer, pchMessage, argptr );
	va_end( argptr );

	PMSG_NOTICE	pNotice;
	TNotice::SetNoticeProperty(&pNotice, TNOTICE_TYPE_BOTTOM, TNOTICE_COLOR_DEFAULT);
	TNotice::MakeNoticeMsg( &pNotice, TNOTICE_TYPE_BOTTOM, chMessageBuffer );
	TNotice::SendNoticeToUser( lpObj->m_Index, &pNotice );

	SendChatMessageToGM( chMessageBuffer );
}

// GM에게 채팅 메시지를 보낸다.
void CHackToolBlock::SendChatMessageToGM(char* pchMessage, ... )
{
	va_list argptr;
	char chMessageBuffer[256] = {0,};
	LPOBJECTSTRUCT lpObj = NULL;
	
	lpObj = GetGMObject();
	if( lpObj == NULL )
	{
		// 현재 서버에 GM이 없다.
		return;
	}
	
	va_start( argptr, pchMessage );
	vsprintf( chMessageBuffer, pchMessage, argptr );
	va_end( argptr );

	PMSG_CHATDATA pMsg = {0,};

	PHeadSetBE( (LPBYTE)&pMsg, 0x00, sizeof( pMsg ) );

	strcpy( pMsg.chatid, "[SERVER]" );
	memcpy( pMsg.chatmsg, chMessageBuffer, MAX_CHAT );

	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );

	//ChatTargetSend( lpObj, chMessageBuffer, lpObj->m_Index );
}

// 2. 해킹 유저에 대한 처리 관련
// 2-1. 해킹툴 사용 유저에 대한 처리
bool CHackToolBlock::IsHackedUser(LPOBJECTSTRUCT lpObj)
{
	DWORD dwKeyValue = KEY_VALUE_NOT_USE;
	CHackToolData* lpHacktoolData = NULL;

	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		// 유저가 사용중인 핵툴 리스트를 검사해서 맵이동 제한이 걸려있는지 확인한다.
		dwKeyValue = lpObj->m_dwUseHacktoolList[i];
		if( dwKeyValue != KEY_VALUE_NOT_USE )
		{
			lpHacktoolData = SearchData( dwKeyValue );
			if( lpHacktoolData == NULL )
			{
				break;
			}

			if( lpHacktoolData->IsUsed() == true )
			{
				// 맵 이동 제한이 걸려있는 상태인지 확인
				if( lpHacktoolData->IsBlocked() == true )
				{
					return true;
				}
			}
		}
	}

	return false;
}

// 2-2. 해킹툴 사용 여부 결과에 대한 처리
void CHackToolBlock::SetHacktoolStatistics(LPOBJECTSTRUCT lpObj, DWORD dwKey, BYTE btResult)
{
	CHackToolData* lpHacktoolData = NULL;
	// dwKey로 해당 해킹툴을 검사
	lpHacktoolData = SearchData( dwKey );
	if( lpHacktoolData == NULL )
	{
		// error
		return;
	}

	if( lpHacktoolData->IsUsed() == false )
	{
		return;
	}

	// 클라이언트로 부터 온 검사 결과에 대해 통계에 반영한다.
	lpHacktoolData->SetStatistics( lpObj, btResult );
}

// 3. 해킹 유저 통계 집계 관련
// 3-1. 해킹툴 사용자 통계 요청
void CHackToolBlock::HacktoolStatisticsAll(char* pchFilename)
{
	// 해당 해킹툴 사용자에 대한 사용자 통계를 요청한다.
	// 현재 접속중인 사용자의 PC에 해당 프로그램을 실행중인 사용자에 대한 응답을 요청한다.
	if( pchFilename == NULL || strlen( pchFilename ) <= 0 )
	{
		// error
		return;
	}

	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// 이미 같은 프로그램이 등록되어 있다면 해당 프로그램의 통계에 대한 로그로 남기고 통계 데이터를 초기화 한다.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetData();
	}
	else
	{
		// 해킹툴에 대한 데이터가 없는 상태라면 프로그램을 추가시켜 준다.
		if( AddHacktoolData( pchFilename ) == false )
		{
			// error
			// log
			return;
		}

		lpHacktoolData = SearchData( pchFilename );
	}

	lpHacktoolData->BeginStatistics();

	PMSG_REQ_HACKTOOL_STATISTICS pMsg;

	PHeadSetBE( (LPBYTE)&pMsg, 0x8A, sizeof( pMsg ) );

	memcpy( pMsg.Filename, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	pMsg.dwKey = lpHacktoolData->dwKey;

	AddLog( "[ANTI-HACK] Request Hacktool Statistics to All Client (Name : %s)",
		pchFilename );

	DataSendAll( (LPBYTE)&pMsg, pMsg.h.size );
/*
	for( int i = ALLOC_USEROBJECTSTART ; i < MAX_OBJECT ; i++ )
	{
		// 게임 플레이 중인 사용자 일 경우만 보냄
		if( ( gObj[i]->Connected == CSS_GAMEPLAYING ) && ( gObj[i]->Type == OBJTYPE_CHARACTER ) )
		{
			//DataSend(
		}
	}
*/
}

// 3-2. 특정 사용자에 대한 해킹 툴 사용자 통계 요청
void CHackToolBlock::HacktoolStatistics(LPOBJECTSTRUCT lpObj, char* pchFilename)
{
	// 해당 해킹툴 사용자에 대한 사용자 통계를 요청한다.
	// 현재 접속중인 사용자의 PC에 해당 프로그램을 실행중인 사용자에 대한 응답을 요청한다.
	if( pchFilename == NULL || strlen( pchFilename ) <= 0 )
	{
		// error
		return;
	}

	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData == NULL )
	{
		// 이 경우는 특정 사용자에게만 보내는 것이기 때문에 데이터가 없는 경우 리턴한다.
		return;
	}
	else
	{
		// 해킹툴에 대한 데이터가 없는 상태라면 프로그램을 추가시켜 준다.
		if( AddHacktoolData( pchFilename ) == false )
		{
			// error
			return;
		}
		
		lpHacktoolData = SearchData( pchFilename );
	}

	PMSG_REQ_HACKTOOL_STATISTICS pMsg;

	// 프로토콜을 암호화 해서 보낸다.
	PHeadSetBE( (LPBYTE)&pMsg, 0x8A, sizeof( pMsg ) );
	
	memcpy( pMsg.Filename, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	pMsg.dwKey = lpHacktoolData->dwKey;
	
	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
}

// 3-3. 통계 요청을 한 해킹툴의 통계 결과 합산이 가능한지 확인한다.
bool CHackToolBlock::CheckStatisticsResult()
{
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].CheckStatisticsFinish() == true )
		{
			GJAntiHackStatisticsResult( HackToolData[i].chHackToolName, HackToolData[i].iHacktoolUserCount );

			SendNoticeToGM( "[ANTI-HACK] Hacktool Use Statistics ( Name : %s, Count : %d )", 
				HackToolData[i].chHackToolName,
				HackToolData[i].iHacktoolUserCount );

			AddLog( "[ANTI-HACK] Hacktool Use Statistics ( Name : %s, Count : %d )",
				HackToolData[i].chHackToolName,
				HackToolData[i].iHacktoolUserCount );
		}
	}

	return true;
}

// 3-4. 신규 유저에게 사용중인 블록 설정 된 해킹툴의 사용 여부를 확인한다.
void CHackToolBlock::HacktoolStatisticsNewConnect(LPOBJECTSTRUCT lpObj)
{
	// 서버에 등록된 해킹툴의 사용 여부를 확인 시킨다.
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].IsUsed() == true )
		{
			HacktoolStatistics( lpObj, HackToolData[i].chHackToolName );
		}
	}
}

// 5. 핵툴로그관련
void CHackToolBlock::AddLog(char* pchLog, ... )
{
	va_list argptr;
	char chMessageBuffer[200];

	va_start( argptr, pchLog );
		vsprintf( chMessageBuffer, pchLog, argptr );
	va_end( argptr );

	lpHacktoolLog->Output( chMessageBuffer );
}

#endif // ADD_HACKING_TOOL_BLOCK_20090311