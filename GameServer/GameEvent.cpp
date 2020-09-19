// GameEvent.cpp: implementation of the CGameEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameEvent.h"
#include "Gamemain.h"

#include "LargeRand.h"

#pragma warning(disable: 4786)
#include <string>
using namespace std;

#ifdef HAPPY_POUCH

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma pack(1)
struct _tagFILEINFO_EVENT_DROPITEM_
{
	DWORD	dwDropRate;
	WORD	wType;	
	BYTE	level;
	BYTE	option1;
	BYTE	option2;
	BYTE	option3;
	float	fDurability;	
};
#pragma pack()

CGameEvent	g_GameEvent;


CGameEvent::CGameEvent()
{
}

CGameEvent::~CGameEvent()
{

}

void	CGameEvent::Init()
{
	ZeroMemory(m_eventItemTable, sizeof(m_eventItemTable));
	ZeroMemory(m_eventItemDropRate, sizeof(m_eventItemDropRate));	
}

void	CGameEvent::LoadItem( const char szDataPath[] )
{
	// @Memo
	// �̹� ���ָӴ� �̺�Ʈ�� �����ؿ�.
	// �׼����� �׽�Ʈ �غ��� �Ѵ�. ��ȹ���� �̺�Ʈ �ÿ� ����� ���� ����.
	// �ڵ� ����
	return; // �������� ��� ���ؿ�~
	Init();

	char szFileName[128] = "";
	_tagFILEINFO_EVENT_DROPITEM_	fData;

	FILE * fp = NULL;
	for (int i = 0; i < MAX_EVENT_MAP; ++i)
	{
		wsprintf(szFileName, "%s\\%u.eitm", szDataPath, i);
		
		if ((fp = fopen(szFileName, "r+b")) == NULL) continue;	

		LogAdd("File Loading %s", szFileName);
		
		int j = 0;
		while (fread(&fData, sizeof(fData), 1, fp))
		{
			m_eventItemTable[i][j].m_wType			= fData.wType;
			m_eventItemTable[i][j].m_level			= fData.level;
			m_eventItemTable[i][j].m_option1		= fData.option1;
			m_eventItemTable[i][j].m_option2		= fData.option2;
			m_eventItemTable[i][j].m_option3		= fData.option3;
			m_eventItemTable[i][j].m_fDurability	= fData.fDurability;

			m_eventItemDropRate[i][j]	+= (int) fData.dwDropRate;

			//TODO: System Log
			LogAdd("TYPE :%u Level:%u OPT(%d, %d, %d) Dur :%d", 
								m_eventItemTable[i][j].m_wType,
								m_eventItemTable[i][j].m_level, 
								m_eventItemTable[i][j].m_option1, 
								m_eventItemTable[i][j].m_option2,
								m_eventItemTable[i][j].m_option3, 
								m_eventItemTable[i][j].m_fDurability);
				
			if (++j >= MAX_EVENT_DROPITEM_NUM) break;			
		}		
	}
}


BOOL	CGameEvent::GetEventItem( const int iMapIndex, CMobDropItemInfo & rMobDropItemInfo )
{	
	//BLUE Pouch
	rMobDropItemInfo.m_wType		= 14 * 32 + 11;	
	rMobDropItemInfo.m_fDurability	= 0;
	rMobDropItemInfo.m_option1		= 0;
	rMobDropItemInfo.m_option2		= 0;
	rMobDropItemInfo.m_option3		= 0;

	// apple��20050204
	if( (rand()%10000) < gHappyPouchBlueDropRate )
	{
		rMobDropItemInfo.m_level	= 14;

		if( (rand()%10000) < gHappyPouchRedDropRate )
		{
			rMobDropItemInfo.m_level	= 15;
		}

		return TRUE;
	}
	// ��������� �ӽ��ڵ�. 2005�� 2������ ���.

	// @Memo

	// �̹� ���ָӴ� �̺�Ʈ�� �����ؿ�.
	// �׼����� �׽�Ʈ �غ��� �Ѵ�. ��ȹ���� �̺�Ʈ �ÿ� ����� ���� ����.
	// �ڵ� ����
	return FALSE; // �������� ��� ���ؿ�~

	if (iMapIndex < 0 || iMapIndex >= MAX_EVENT_MAP) return FALSE;

	DWORD	dwRate = GetLargeRand() % MAX_EVENT_DROPITEM_NUM; //TODO: GetLargeRand<- SeedMT�ϰ� ���.

	for (int i = 0; i < MAX_EVENT_DROPITEM_NUM; i++)
	{
		if (m_eventItemDropRate[iMapIndex][i]) break;

		if (dwRate <= m_eventItemDropRate[iMapIndex][i])
		{
			rMobDropItemInfo = m_eventItemTable[iMapIndex][i];
			return TRUE;
		}
	}

	return FALSE;
}

#endif

