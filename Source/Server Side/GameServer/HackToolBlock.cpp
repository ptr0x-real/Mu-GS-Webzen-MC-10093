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

// 1. ��ŷ�� ������ ����
// 1-1. ��ŷ�� ������ �ʱ�ȭ
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

// 1-2. ������� ������ ���� Ȯ��
bool CHackToolData::IsUsed()
{
	return bIsUsed;
}

// 1-3. �̹� �ִ� ������ ���� Ȯ��
bool CHackToolData::IsMatched(char* pchFilename)
{
	if( strcmp( chHackToolName, pchFilename ) == 0 )
	{
		return true;
	}

	return false;
}

// 1-4. ��ŷ���� �����͸� �Է½�Ų��.
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

// 1-5. ��ŷ���� Ű ���� ���Ѵ�.
bool CHackToolData::KeyValueVerify(DWORD dwKey)
{
	if( this->dwKey == dwKey )
	{
		return true;
	}

	return false;
}

// 1-6. ��ŷ���� ��� ���� ����� �����Ų��.
void CHackToolData::SetStatistics(LPOBJECTSTRUCT lpObj, BYTE btResult)
{
	// 0 : ��� ������ ���� , 1 : ��� ��
	if( btResult == HACKTOOL_STATISTICS_USE )
	{
		iHacktoolUserCount++;
		// �ش� ������ ������� ��ŷ �� �����͸� �߰��Ͽ� �ش�.
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

// 1-7. ��ŷ���� ��� ��û ���� ����.
void CHackToolData::BeginStatistics()
{
	// ��ŷ���� ��� ��û���� ƽ ī��Ʈ�� ���´�.
	// ��� ���� 1�еڿ� ��� ó���� �Ϸ��Ѵ�.
	bBeginStatistics = true;
	dwStatisticsBeginTick = GetTickCount();
}

// 1-8. ��ŷ���� ��� �Ϸ� ���� Ȯ��
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

// 1-9. ��� ������ �� Ȯ���� ���� ������ ����
void CHackToolData::ResetData()
{
	iHacktoolUserCount = 0;
	bBeginStatistics = false;
	dwStatisticsBeginTick = 0;
}

// 1-10. ���� �������� ���ο� ��ĳ�� ������ ���� ����
void CHackToolData::ResetSchedule()
{
	dwActiveTime = 0;
	dwMapmoveBlockTerm = 0;
	dwStatisticsBeginTick = 0;
	iMapmoveBlockRate = 0;
}

// 2. ��ŷ�� ���̵� ���� ����
// 2-1. ��ŷ ������� ���̵� ���� ���θ� Ȯ���Ѵ�.
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

// 3. ��Ÿ
// �ش� ��ŷ�� ����� Ȱ��ȭ �������� Ȯ���Ѵ�.
bool CHackToolData::CheckBlockActive()
{
	if( bIsUsed == false || iMapmoveBlockRate <= 0 )
	{
		return false;
	}

	// �Էµ� �ð��� ���� �ð��� ���Ѵ�.
	time_t tTime = dwActiveTime;

	CTime tActiveTime( tTime );
	CTime tCurrentTime = CTime::GetCurrentTime();

	// ���� �ð��� ���� ���¶�� Ȱ��ȭ ��Ų��.
	if( bBlockHacktoolUser == false )
	{
		if( tActiveTime <= tCurrentTime )
		{
			bBlockHacktoolUser = true;

			return true;
		}
	}

	// ����� �����ؾ� �� �ñⰡ �Ǿ����� Ȯ���Ѵ�.
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

// 1. ��ŷ�� ������ ����
// 1-0. ��ŷ�� ���� ���� �ʱ�ȭ
void CHackToolBlock::Initialize()
{
	// ��ŷ�� ���� �α׸� ���� �� ������ ����
	lpHacktoolLog = new CLogToFile( "ANTIHACK", ".\\ANTIHACK_LOG");		// �α����� ��ü
}

// 1-1. ��ŷ�� �����͸� �ʱ�ȭ
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

// 1-2. ��ŷ�� ���� �ɼ� �ε�
bool CHackToolBlock::LoadOption(char* pchOptionFilename)
{
	// ��ŷ �� ���� ������ ���� ����۽� ���� �������� �ʱ� ���� �ɼ� ���� ����
	// 'HacktoolOption.dat' ����
	return true;
}

// 1-3. ��ŷ�� �����͸� �߰�
bool CHackToolBlock::AddHacktoolData(char* pchFilename, DWORD dwBlockTerm, int iBlockRate)
{
	DWORD dwKeyValue = 0;
	CHackToolData* lpHacktoolData = NULL;

	AddLog( "[ANTI-HACK] Add Hacktool block data ( Name : %s, BlockTerm : %d, BlockRate : %d )",
		pchFilename,
		dwKeyValue,
		iBlockRate );

	// �Ȱ��� �����Ͱ� �����Ѵ�.
	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// ���� �����Ͱ� �����ϸ� �̹� �����ϴ� �����ʹ� �ʱ�ȭ �ϰ� ������ �ʱ�ȭ �Ѵ�.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetSchedule();

		// ��ŷ���� �����͸� �Է½�Ų��.
		lpHacktoolData->SetData( pchFilename, dwBlockTerm, iBlockRate );
				
		return true;
	}
	else
	{
		for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
		{
			if( HackToolData[i].IsUsed() == false )
			{
				// ��ŷ���� Ű ���� �����Ѵ�.
				dwKeyValue = _MakeKeyValue();
				if( dwKeyValue == KEY_VALUE_NOT_USE )
				{
					// Ű �� ������ ����
					return false;
				}
				// ��ŷ���� �����͸� �Է½�Ų��.
				HackToolData[i].SetData( pchFilename, dwKeyValue, dwBlockTerm, iBlockRate );
				
				break;;
			}
		}

		// �̹� �����ϴ� �����Ͱ� ���ٸ� ���� ��� ���θ� Ȯ���ϱ� ���� ���� ��� ��踦 �����Ѵ�.
		HacktoolStatisticsAll( pchFilename );

		return true;
	}

	// ���� ������ ����.
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

	// �Ȱ��� �����Ͱ� �����Ѵ�.
	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// ���� �����Ͱ� �����ϸ� �̹� �����ϴ� �����ʹ� �ʱ�ȭ �ϰ� ������ �ʱ�ȭ �Ѵ�.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetSchedule();

		// ��ŷ���� �����͸� �Է½�Ų��.
		lpHacktoolData->SetData( pchFilename, dwKeyValue, dwActiveTime, dwBlockTerm, iBlockRate );

		return true;
	}
	else
	{
		for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
		{
			if( HackToolData[i].IsUsed() == false )
			{
				// ��ŷ���� Ű ���� �����Ѵ�.
				dwKeyValue = _MakeKeyValue();
				if( dwKeyValue == KEY_VALUE_NOT_USE )
				{
					// Ű �� ������ ����
					return false;
				}
				// ��ŷ���� �����͸� �Է½�Ų��.
				HackToolData[i].SetData( pchFilename, dwKeyValue, dwActiveTime, dwBlockTerm, iBlockRate );

				break;
			}
		}

		// �̹� �����ϴ� �����Ͱ� ���ٸ� ���� ��� ���θ� Ȯ���ϱ� ���� ���� ��� ��踦 �����Ѵ�.
		HacktoolStatisticsAll( pchFilename );

		return true;
	}
	
	// ���� ������ ����.
	return false;
}

// 1-4. ��ŷ�� �����͸� ����
bool CHackToolBlock::RemoveHacktoolData(char* pchFilename)
{
	// ��ŷ�� ���� �����ʹ� ��� ������������ �ޱ� ������ �߰��� ������ ��� �� �׸��� ����.
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

// 1-5. ��ŷ�� ������ ����
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

// 1-6. �ش� Ű ���� ������ ��ŷ ���� ���翩�� Ȯ��
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

// 1-7. �ش� �̸��� ���� ��ŷ �� �����͸� ã�´�.
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

// �� ��ŷ���� ������ Ű ���� �����Ѵ�.
DWORD CHackToolBlock::_MakeKeyValue()
{
	DWORD dwKeyValue = 0;

	// 10�� �õ� �Ŀ��� �ߺ��Ǵ� ���� ���´ٸ� ���з� �����Ѵ�.
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

// ��ũ�η� �Է� �� �� ���� �ð��� DWORD ������ �����Ͽ� �ش�.
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

// �Էµ� ��ŷ �����Ͱ� ���� ������ �ð����� Ȯ���Ѵ�.
void CHackToolBlock::CheckBlockActive()
{
	LPOBJECTSTRUCT lpObj = NULL;
	// ��ü ����Ʈ�� �˻��Ͽ� ���� ����� ������ �����͸� �˻��Ѵ�.
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

// ������ ����� GM�� �����Ѵ�.
bool CHackToolBlock::SetGMObject(LPOBJECTSTRUCT lpObj)
{
	if( IsGMUser( lpObj ) == true )
	{
		lpGMObject = lpObj;
	}

	return false;
}

// ������ ����� GM�� ������Ʈ�� ���´�.
LPOBJECTSTRUCT CHackToolBlock::GetGMObject()
{
	if( IsGMUser( lpGMObject ) == true )
	{
		return lpGMObject;
	}
		
	return NULL;
}

// �ش� ����ڰ� GM���� Ȯ���Ѵ�.
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

// GM���� ���� �޽����� ������.
void CHackToolBlock::SendNoticeToGM(char* pchMessage, ... )
{
	va_list argptr;
	char chMessageBuffer[256] = {0,};
	LPOBJECTSTRUCT lpObj = NULL;

	lpObj = GetGMObject();
	if( lpObj == NULL )
	{
		// ���� ������ GM�� ����.
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

// GM���� ä�� �޽����� ������.
void CHackToolBlock::SendChatMessageToGM(char* pchMessage, ... )
{
	va_list argptr;
	char chMessageBuffer[256] = {0,};
	LPOBJECTSTRUCT lpObj = NULL;
	
	lpObj = GetGMObject();
	if( lpObj == NULL )
	{
		// ���� ������ GM�� ����.
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

// 2. ��ŷ ������ ���� ó�� ����
// 2-1. ��ŷ�� ��� ������ ���� ó��
bool CHackToolBlock::IsHackedUser(LPOBJECTSTRUCT lpObj)
{
	DWORD dwKeyValue = KEY_VALUE_NOT_USE;
	CHackToolData* lpHacktoolData = NULL;

	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		// ������ ������� ���� ����Ʈ�� �˻��ؼ� ���̵� ������ �ɷ��ִ��� Ȯ���Ѵ�.
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
				// �� �̵� ������ �ɷ��ִ� �������� Ȯ��
				if( lpHacktoolData->IsBlocked() == true )
				{
					return true;
				}
			}
		}
	}

	return false;
}

// 2-2. ��ŷ�� ��� ���� ����� ���� ó��
void CHackToolBlock::SetHacktoolStatistics(LPOBJECTSTRUCT lpObj, DWORD dwKey, BYTE btResult)
{
	CHackToolData* lpHacktoolData = NULL;
	// dwKey�� �ش� ��ŷ���� �˻�
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

	// Ŭ���̾�Ʈ�� ���� �� �˻� ����� ���� ��迡 �ݿ��Ѵ�.
	lpHacktoolData->SetStatistics( lpObj, btResult );
}

// 3. ��ŷ ���� ��� ���� ����
// 3-1. ��ŷ�� ����� ��� ��û
void CHackToolBlock::HacktoolStatisticsAll(char* pchFilename)
{
	// �ش� ��ŷ�� ����ڿ� ���� ����� ��踦 ��û�Ѵ�.
	// ���� �������� ������� PC�� �ش� ���α׷��� �������� ����ڿ� ���� ������ ��û�Ѵ�.
	if( pchFilename == NULL || strlen( pchFilename ) <= 0 )
	{
		// error
		return;
	}

	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData != NULL )
	{
		// �̹� ���� ���α׷��� ��ϵǾ� �ִٸ� �ش� ���α׷��� ��迡 ���� �α׷� ����� ��� �����͸� �ʱ�ȭ �Ѵ�.
		AddLog( "[ANTI-HACK] Already registered Hacktool Program. Reset Old data ( Name : %s )",
			pchFilename );
		lpHacktoolData->ResetData();
	}
	else
	{
		// ��ŷ���� ���� �����Ͱ� ���� ���¶�� ���α׷��� �߰����� �ش�.
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
		// ���� �÷��� ���� ����� �� ��츸 ����
		if( ( gObj[i]->Connected == CSS_GAMEPLAYING ) && ( gObj[i]->Type == OBJTYPE_CHARACTER ) )
		{
			//DataSend(
		}
	}
*/
}

// 3-2. Ư�� ����ڿ� ���� ��ŷ �� ����� ��� ��û
void CHackToolBlock::HacktoolStatistics(LPOBJECTSTRUCT lpObj, char* pchFilename)
{
	// �ش� ��ŷ�� ����ڿ� ���� ����� ��踦 ��û�Ѵ�.
	// ���� �������� ������� PC�� �ش� ���α׷��� �������� ����ڿ� ���� ������ ��û�Ѵ�.
	if( pchFilename == NULL || strlen( pchFilename ) <= 0 )
	{
		// error
		return;
	}

	CHackToolData* lpHacktoolData = NULL;

	lpHacktoolData = SearchData( pchFilename );
	if( lpHacktoolData == NULL )
	{
		// �� ���� Ư�� ����ڿ��Ը� ������ ���̱� ������ �����Ͱ� ���� ��� �����Ѵ�.
		return;
	}
	else
	{
		// ��ŷ���� ���� �����Ͱ� ���� ���¶�� ���α׷��� �߰����� �ش�.
		if( AddHacktoolData( pchFilename ) == false )
		{
			// error
			return;
		}
		
		lpHacktoolData = SearchData( pchFilename );
	}

	PMSG_REQ_HACKTOOL_STATISTICS pMsg;

	// ���������� ��ȣȭ �ؼ� ������.
	PHeadSetBE( (LPBYTE)&pMsg, 0x8A, sizeof( pMsg ) );
	
	memcpy( pMsg.Filename, pchFilename, MAX_HACKTOOL_FILENAME_LENGTH );
	pMsg.dwKey = lpHacktoolData->dwKey;
	
	DataSend( lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size );
}

// 3-3. ��� ��û�� �� ��ŷ���� ��� ��� �ջ��� �������� Ȯ���Ѵ�.
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

// 3-4. �ű� �������� ������� ��� ���� �� ��ŷ���� ��� ���θ� Ȯ���Ѵ�.
void CHackToolBlock::HacktoolStatisticsNewConnect(LPOBJECTSTRUCT lpObj)
{
	// ������ ��ϵ� ��ŷ���� ��� ���θ� Ȯ�� ��Ų��.
	for( int i = 0 ; i < MAX_HACKTOOL_LIST ; i++ )
	{
		if( HackToolData[i].IsUsed() == true )
		{
			HacktoolStatistics( lpObj, HackToolData[i].chHackToolName );
		}
	}
}

// 5. �����αװ���
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